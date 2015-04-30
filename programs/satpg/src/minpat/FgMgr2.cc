
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
// @param[in] fault 対象の故障
// @param[in] group_list 探索最小のグループ番号のリスト
// @param[in] first_hit 最初のグループのみを求めるとき true にするフラグ
// @param[out] gid_list 対象のグループ番号を収めるリスト
// @return 最初のグループ番号を返す．
//
// 見つからない場合は group_num() を返す．
// gid_list は first_hit == true の時，意味を持たない．
ymuint
FgMgr2::find_dom_group(const TpgFault* fault,
		      const vector<ymuint>& group_list,
		      bool first_hit,
		      vector<ymuint>& gid_list)
{
  GvalCnf gval_cnf(max_node_id());
  FvalCnf fval_cnf(max_node_id(), gval_cnf);
  SatEngine engine(string(), string(), NULL);

  const NodeSet& node_set = this->node_set(fault);

  // fault が見つからない条件を作る．
  engine.make_fval_cnf(fval_cnf, fault, node_set, kVal0);

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
// @param[in] fault 対象の故障
// @param[in] group_list 探索最小のグループ番号のリスト
// @param[in] first_hit 最初のグループのみを求めるとき true にするフラグ
// @param[out] gid_list 対象のグループ番号を収めるリスト
// @return 最初のグループ番号を返す．
//
// 見つからない場合は group_num() を返す．
// gid_list は first_hit == true の時，意味を持たない．
ymuint
FgMgr2::find_group(const TpgFault* fault,
		   const vector<ymuint>& group_list,
		   bool first_hit,
		   vector<ymuint>& gid_list)
{
  ymuint first_gid = group_num();

  SatEngine engine0(string(), string(), NULL);
  GvalCnf gval_cnf0(max_node_id());

  const FaultInfo& fi0 = fault_info(fault);
  NodeValList suf_list0;
  if ( fi0.single_cube() ) {
    // fault を検出する条件を追加
    suf_list0 = fi0.sufficient_assignment();
  }
  else {
    // fault を検出する CNF を生成
    FvalCnf fval_cnf0(max_node_id(), gval_cnf0);
    engine0.make_fval_cnf(fval_cnf0, fault, node_set(fault), kVal1);
  }
  for (ymuint i = 0; i < group_list.size(); ++ i) {
    ymuint gid = group_list[i];

    {
      const NodeValList& suf_list1 = sufficient_assignment(gid);
      if ( engine0.check_sat(gval_cnf0, suf_list0, suf_list1) == kB3True ) {
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

    {
      const NodeValList& ma_list1 = mandatory_assignment(gid);
      if ( engine0.check_sat(gval_cnf0, suf_list0, ma_list1) == kB3False ) {
	continue;
      }
    }

    SatEngine engine(string(), string(), NULL);
    GvalCnf gval_cnf(max_node_id());

    ymuint fnum = 0;
    NodeValList suf_list1;
    if ( fi0.single_cube() ) {
      // fault を検出する条件を追加
      suf_list1 = fi0.sufficient_assignment();
    }
    else {
      FvalCnf fval_cnf(max_node_id(), gval_cnf);
      engine.make_fval_cnf(fval_cnf, fault, node_set(fault), kVal1);
    }
    ymuint nf = fault_num(gid);
    vector<FvalCnf*> fval_cnf_array(nf, NULL);
    for (ymuint i = 0; i < nf; ++ i) {
      const TpgFault* fault = this->fault(gid, i);
      const FaultInfo& fi = fault_info(fault);
      if ( fi.single_cube() ) {
	const NodeValList& suf_list2 = fi.sufficient_assignment();
	suf_list1.merge(suf_list2);
      }
      else {
	FvalCnf* fval_cnfp = new FvalCnf(max_node_id(), gval_cnf);
	fval_cnf_array[i] = fval_cnfp;
	engine.make_fval_cnf(*fval_cnfp, fault, node_set(fault), kVal1);
	++ fnum;
      }
    }
    for (ymuint i = 0; i < nf; ++ i) {
      delete fval_cnf_array[i];
    }
    mFsum += fnum;
    if ( mFmax < fnum ) {
      mFmax = fnum;
    }
    ++ mMnum;
    if ( engine.check_sat(gval_cnf, suf_list1) == kB3True ) {
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
// @param[in] fault 故障
void
FgMgr2::add_fault(ymuint gid,
		  const TpgFault* fault)
{
  FaultGroup* fg = fault_group(gid);

  const FaultInfo& fi = fault_info(fault);
  const NodeValList& ma_list = fi.mandatory_assignment();

  GvalCnf gval_cnf(max_node_id());
  FvalCnf fval_cnf(max_node_id(), gval_cnf);
  SatEngine engine(string(), string(), NULL);

  ymuint nf = fg->fault_num();
  vector<FvalCnf*> fval_cnf_array(nf, NULL);
  NodeValList suf_list1;

  if ( fi.single_cube() ) {
    suf_list1 = ma_list;
  }
  else {
    engine.make_fval_cnf(fval_cnf, fault, node_set(fault), kVal1);
  }

  ymuint fnum = 0;
  for (ymuint i = 0; i < nf; ++ i) {
    const TpgFault* fault = fg->fault(i);
    const FaultInfo& fi = fault_info(fault);
    if ( fi.single_cube() ) {
      const NodeValList& suf_list2 = fi.sufficient_assignment();
      suf_list1.merge(suf_list2);
    }
    else {
      FvalCnf* fval_cnfp = new FvalCnf(max_node_id(), gval_cnf);
      fval_cnf_array[i] = fval_cnfp;
      engine.make_fval_cnf(*fval_cnfp, fault, node_set(fault), kVal1);
      ++ fnum;
    }
  }

  vector<Bool3> sat_model;
  Bool3 sat_ans = engine.check_sat(gval_cnf, suf_list1, sat_model);
  ASSERT_COND( sat_ans == kB3True );

  for (ymuint i = 0; i < nf; ++ i) {
    const TpgFault* fault = fg->fault(i);
    const FaultInfo& fi = fault_info(fault);
    if ( !fi.single_cube() ) {
      NodeValList suf_list;
      NodeValList pi_suf_list;
      fval_cnf_array[i]->get_pi_suf_list(sat_model, fault, node_set(fault),
					 suf_list, pi_suf_list);
      fg->set_suf_list(i, suf_list, pi_suf_list);
      delete fval_cnf_array[i];
    }
  }
  ++ mMnum;
  if ( mFmax < fnum ) {
    mFmax = fnum;
  }
  mFsum += fnum;

  fg->update();

  if ( fi.single_cube() ) {
    const NodeValList& pi_suf_list = fi.pi_sufficient_assignment();
    fg->add_fault(fault, ma_list, ma_list, pi_suf_list);
  }
  else {
    NodeValList suf_list;
    NodeValList pi_suf_list;
    fval_cnf.get_pi_suf_list(sat_model, fault, node_set(fault), suf_list, pi_suf_list);
    fg->add_fault(fault, suf_list, ma_list, pi_suf_list);
  }
}

// @brief 故障を取り除く
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
// @param[in] fault_list 故障リスト
void
FgMgr2::delete_fault(ymuint gid,
		     const vector<const TpgFault*>& fault_list)
{
  FaultGroup* fg = fault_group(gid);
  fg->delete_faults(fault_list);
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

// @brief 統計データをクリアする．
void
FgMgr2::clear_count()
{
  mMnum = 0;
  mFsum = 0;
  mFmax = 0;
}

END_NAMESPACE_YM_SATPG
