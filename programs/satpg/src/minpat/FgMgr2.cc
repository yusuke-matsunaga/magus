
/// @file FgMgr2.cc
/// @brief FgMgr2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "FgMgr2.h"

#include "GvalCnf.h"
#include "FvalCnf.h"
#include "SatEngine.h"
#include "FaultInfo.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス FgMgr2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] max_node_id ノード番号の最大値 + 1
// @param[in] analyzer 故障解析器
FgMgr2::FgMgr2(ymuint max_node_id,
	       const FaultAnalyzer& analyzer) :
  FgMgrBase(max_node_id, analyzer)
{
  clear_count();
}

// @brief デストラクタ
FgMgr2::~FgMgr2()
{
}

// @brief 新たな条件なしで追加できる既存グループを見つける．
// @param[in] fid 対象の故障番号
// @param[in] group_list 探索最小のグループ番号のリスト
// @param[in] first_hit 最初のグループのみを求めるとき true にするフラグ
// @param[out] gid_list 対象のグループ番号を収めるリスト
// @return 最初のグループ番号を返す．
//
// 見つからない場合は group_num() を返す．
// gid_list は first_hit == true の時，意味を持たない．
ymuint
FgMgr2::find_dom_group(ymuint fid,
		       const vector<ymuint>& group_list,
		       bool first_hit,
		       vector<ymuint>& gid_list)
{
  SatEngine engine(string(), string(), NULL);
  GvalCnf gval_cnf(max_node_id());
  FvalCnf fval_cnf(max_node_id(), gval_cnf);

  // fault が見つからない条件を作る．
  engine.make_fval_cnf(fval_cnf, fault(fid), node_set(fid), kVal0);

  ymuint first_gid = group_num();
  for (ymuint i = 0; i < group_list.size(); ++ i) {
    ymuint gid = group_list[i];
    const NodeValList& suf_list0 = sufficient_assignment(gid);
    if ( engine.check_sat(gval_cnf, suf_list0) == kB3False ) {
      // suf_lib0 のもとでは必ず見つかるということ．
      if ( first_gid == group_num() ) {
	first_gid = gid;
	if ( first_hit ) {
	  break;
	}
      }
      gid_list.push_back(gid);
    }
  }
  return first_gid;
}

// @brief 追加できる既存グループを見つける．
// @param[in] fid0 対象の故障番号
// @param[in] group_list 探索最小のグループ番号のリスト
// @param[in] fast 高速ヒューリスティック
// @param[in] first_hit 最初のグループのみを求めるとき true にするフラグ
// @param[out] gid_list 対象のグループ番号を収めるリスト
// @return 最初のグループ番号を返す．
//
// 見つからない場合は group_num() を返す．
// gid_list は first_hit == true の時，意味を持たない．
ymuint
FgMgr2::find_group(ymuint fid0,
		   const vector<ymuint>& group_list,
		   bool fast,
		   bool first_hit,
		   vector<ymuint>& gid_list)
{
  ymuint first_gid = group_num();

  const FaultInfo& fi0 = fault_info(fid0);

  SatEngine engine0(string(), string(), NULL);
  GvalCnf gval_cnf0(max_node_id());

  // fi0 の必要割当を追加
  const NodeValList ma_list0 = fi0.mandatory_assignment();
  engine0.add_assignments(gval_cnf0, ma_list0);

  if ( !fi0.single_cube() ) {
    // fault を検出する CNF を生成
    FvalCnf fval_cnf0(max_node_id(), gval_cnf0);
    engine0.make_fval_cnf(fval_cnf0, fault(fid0), node_set(fid0), kVal1);
  }

  for (ymuint i = 0; i < group_list.size(); ++ i) {
    ymuint gid = group_list[i];

    { // グループの十分割当が成り立っていたら両立している．
      const NodeValList& suf_list1 = sufficient_assignment(gid);
      if ( engine0.check_sat(gval_cnf0, suf_list1) == kB3True ) {
	if ( first_gid == group_num() ) {
	  first_gid = gid;
	  if ( first_hit ) {
	    break;
	  }
	}
	gid_list.push_back(gid);
	continue;
      }
    }
    if ( fast ) {
      continue;
    }

    { // グループの必要割当が成り立たなかったら衝突している．
      const NodeValList& ma_list1 = mandatory_assignment(gid);
      if ( engine0.check_sat(gval_cnf0, ma_list1) == kB3False ) {
	continue;
      }
    }

    ++ mCheckCount;

    // 簡易検査ではわからなかったので正式に調べる．
    SatEngine engine(string(), string(), NULL);
    GvalCnf gval_cnf(max_node_id());

    // fid0 の必要割当を追加
    engine.add_assignments(gval_cnf, ma_list0);
    // グループの必要割当を追加
    engine.add_assignments(gval_cnf, mandatory_assignment(gid));

    ymuint fnum = 0;

    if ( !fi0.single_cube() ) {
      // fid0 を検出する条件を追加
      FvalCnf fval_cnf0(max_node_id(), gval_cnf);
      engine.make_fval_cnf(fval_cnf0, fault(fid0), node_set(fid0), kVal1);
      ++ fnum;
    }

    ymuint nf = fault_num(gid);
    for (ymuint i = 0; i < nf; ++ i) {
      ymuint fid1 = fault_id(gid, i);
      const FaultInfo& fi1 = fault_info(fid1);
      if ( !fi1.single_cube() ) {
	// fid1 の検出条件を生成
	FvalCnf fval_cnf1(max_node_id(), gval_cnf);
	engine.make_fval_cnf(fval_cnf1, fault(fid1), node_set(fid1), kVal1);
	++ fnum;
      }
    }

    mFsum += fnum;
    if ( mFmax < fnum ) {
      mFmax = fnum;
    }
    ++ mMnum;

    if ( engine.check_sat() == kB3True ) {
      ++ mFoundCount;
      if ( first_gid == group_num() ) {
	first_gid = gid;
	if ( first_hit ) {
	  break;
	}
      }
      gid_list.push_back(gid);
    }
  }
  return first_gid;
}

// @brief 既存のグループに故障を追加する．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
// @param[in] fid0 故障番号
void
FgMgr2::add_fault(ymuint gid,
		  ymuint fid0)
{
  FaultGroup* fg = fault_group(gid);

  const FaultInfo& fi0 = fault_info(fid0);

  SatEngine engine(string(), string(), NULL);
  GvalCnf gval_cnf(max_node_id());

  // fid0 の必要割当を追加
  const NodeValList& ma_list0 = fi0.mandatory_assignment();
  engine.add_assignments(gval_cnf, ma_list0);

  // グループの必要割当を追加
  const NodeValList& group_ma_list = mandatory_assignment(gid);
  engine.add_assignments(gval_cnf, group_ma_list);

  FvalCnf fval_cnf0(max_node_id(), gval_cnf);

  ymuint fnum = 0;
  if ( !fi0.single_cube() ) {
    // fid0 を検出する条件を追加
    engine.make_fval_cnf(fval_cnf0, fault(fid0), node_set(fid0), kVal1);
    ++ fnum;
  }

  ymuint nf = fg->fault_num();
  vector<FvalCnf> fval_cnf_array(nf, FvalCnf(max_node_id(), gval_cnf));
  for (ymuint i = 0; i < nf; ++ i) {
    ymuint fid1 = fg->fault_id(i);
    const FaultInfo& fi1 = fault_info(fid1);
    // fault を検出する条件を追加
    if ( !fi1.single_cube() ) {
      engine.make_fval_cnf(fval_cnf_array[i], fault(fid1), node_set(fid1), kVal1);
      ++ fnum;
    }
  }

  vector<Bool3> sat_model;
  Bool3 sat_ans = engine.check_sat(sat_model);
  ASSERT_COND( sat_ans == kB3True );

  for (ymuint i = 0; i < nf; ++ i) {
    ymuint fid1 = fg->fault_id(i);
    const FaultInfo& fi1 = fault_info(fid1);
    if ( !fi1.single_cube() ) {
      NodeValList suf_list;
      NodeValList pi_suf_list;
      fval_cnf_array[i].get_pi_suf_list(sat_model, fault(fid1), node_set(fid1),
					suf_list, pi_suf_list);
      fg->set_suf_list(i, suf_list, pi_suf_list);
    }
  }
  ++ mMnum;
  if ( mFmax < fnum ) {
    mFmax = fnum;
  }
  mFsum += fnum;

  fg->update();

  if ( fi0.single_cube() ) {
    const NodeValList& pi_suf_list = fi0.pi_sufficient_assignment();
    fg->add_fault(fid0, ma_list0, ma_list0, pi_suf_list);
  }
  else {
    NodeValList suf_list;
    NodeValList pi_suf_list;
    fval_cnf0.get_pi_suf_list(sat_model, fault(fid0), node_set(fid0), suf_list, pi_suf_list);
    fg->add_fault(fid0, suf_list, ma_list0, pi_suf_list);
  }
}

// @brief 複数故障の検出検査回数
ymuint
FgMgr2::mfault_num() const
{
  return mMnum;
}

// @brief 複数故障の平均多重度
double
FgMgr2::mfault_avg() const
{
  if ( mMnum == 0 ) {
    return 0.0;
  }
  return static_cast<double>(mFsum) / static_cast<double>(mMnum);
}

// @brief 複数故障の最大値
ymuint
FgMgr2::mfault_max() const
{
  return mFmax;
}

// @brief チェック回数
ymuint
FgMgr2::check_count() const
{
  return mCheckCount;
}

// @brief 成功回数
ymuint
FgMgr2::found_count() const
{
  return mFoundCount;
}

// @brief 統計データをクリアする．
void
FgMgr2::clear_count()
{
  mMnum = 0;
  mFsum = 0;
  mFmax = 0;
  mCheckCount = 0;
  mFoundCount = 0;
}

END_NAMESPACE_YM_SATPG
