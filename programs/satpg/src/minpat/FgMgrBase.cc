
/// @file FgMgrBase.cc
/// @brief FgMgrBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "FgMgrBase.h"

#include "NodeSet.h"
#include "GvalCnf.h"
#include "FvalCnf.h"
#include "SatEngine.h"
#include "FaultAnalyzer.h"
#include "FaultInfo.h"
#include "YmUtils/HashSet.h"

#define USE_CACHE 1


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス FgMgr
//////////////////////////////////////////////////////////////////////

// @brief 故障グループのリストを出力する．
// @param[in] s 出力先のストリーム
// @param[in] group_list グループ番号のリスト
void
FgMgr::print_group_list(ostream& s,
			const vector<ymuint>& group_list) const
{
  ymuint ng = group_list.size();
  for (ymuint i = 0; i < ng; ++ i) {
    ymuint gid = group_list[i];
    s << setw(4) << i << ": Group#" << gid;
    ymuint nf = fault_num(gid);
    for (ymuint j = 0; j < nf; ++ j) {
      ymuint fid = fault_id(gid, j);
      s << " " << fid;
    }
    s << endl;
  }
  s << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス FgMgrBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] max_node_id ノード番号の最大値 + 1
// @param[in] analyzer 故障解析器
FgMgrBase::FgMgrBase(ymuint max_node_id,
		     const FaultAnalyzer& analyzer) :
  mMaxNodeId(max_node_id),
  mAnalyzer(analyzer)
{
  clear_count();
}

// @brief デストラクタ
FgMgrBase::~FgMgrBase()
{
  clear();
}

// @brief クリアする．
void
FgMgrBase::clear()
{
  for (ymuint gid = 0; gid < mGroupList.size(); ++ gid) {
    delete mGroupList[gid];
  }
  mGroupList.clear();
}

// @brief 現在のグループ数を返す．
ymuint
FgMgrBase::group_num() const
{
  return mGroupList.size();
}

// @brief 新しいグループを作る．
// @param[in] fid 故障番号
// @return グループ番号を返す．
//
// fid のみを要素に持つ．
ymuint
FgMgrBase::new_group(ymuint fid)
{
  ymuint gid = _new_group();
  add_fault(gid, fid);
  return gid;
}

// @brief グループを複製する．
// @param[in] src_gid 複製元のグループ番号
// @return 新しいグループ番号を返す．
ymuint
FgMgrBase::duplicate_group(ymuint src_gid)
{
  ASSERT_COND( src_gid < mGroupList.size() );
  FaultGroup* src_fg = mGroupList[src_gid];
  ASSERT_COND( src_fg != NULL );
  ymuint gid = _new_group();
  FaultGroup* dst_fg = mGroupList[gid];
  dst_fg->copy(*src_fg);
  return gid;
}

// @brief 新しいグループを作る．
// @return グループ番号を返す．
ymuint
FgMgrBase::_new_group()
{
  FaultGroup* fg = NULL;
  for (ymuint i = 0; i < mGroupList.size(); ++ i) {
    if ( mGroupList[i] == NULL ) {
      fg = new FaultGroup(i);
      mGroupList[i] = fg;
      break;
    }
  }
  if ( fg == NULL ) {
    ymuint id = mGroupList.size();
    fg = new FaultGroup(id);
    mGroupList.push_back(fg);
  }
  ymuint gid = fg->id();
  return gid;
}

// @brief グループを置き換える．
// @param[in] old_gid 置き換え対象のグループ番号
// @param[in] new_gid 置き換えるグループ番号
//
// new_gid は削除される．
void
FgMgrBase::replace_group(ymuint old_gid,
			 ymuint new_gid)
{
  delete_group(old_gid);

  ASSERT_COND( new_gid < mGroupList.size() );
  FaultGroup* new_fg = mGroupList[new_gid];
  ASSERT_COND( new_fg != NULL );
  new_fg->set_id(old_gid);
  mGroupList[old_gid] = new_fg;
  mGroupList[new_gid] = NULL;
}

// @brief グループを削除する．
// @param[in] gid グループ番号
void
FgMgrBase::delete_group(ymuint gid)
{
  FaultGroup* fg = fault_group(gid);
  delete fg;
  mGroupList[gid] = NULL;
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
FgMgrBase::find_dom_group(ymuint fid,
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
FgMgrBase::find_group(ymuint fid0,
		      const vector<ymuint>& group_list,
		      bool fast,
		      bool first_hit,
		      vector<ymuint>& gid_list)
{
  StopWatch local_timer;
  local_timer.start();

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

    if ( check_compat_cache(gid, fid0) ) {
      if ( first_gid == group_num() ) {
	first_gid = gid;
	if ( first_hit ) {
	  break;
	}
      }
      gid_list.push_back(gid);
      continue;
    }
    if ( check_conflict_cache(gid, fid0) ) {
      continue;
    }

    { // グループの十分割当が成り立っていたら両立している．
      const NodeValList& suf_list1 = sufficient_assignment(gid);
      if ( engine0.check_sat(gval_cnf0, suf_list1) == kB3True ) {
	add_compat_cache(gid, fid0);
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
	add_conflict_cache(gid, fid0);
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
      add_compat_cache(gid, fid0);
      ++ mFoundCount;
      if ( first_gid == group_num() ) {
	first_gid = gid;
	if ( first_hit ) {
	  break;
	}
      }
      gid_list.push_back(gid);
    }
    else {
      add_conflict_cache(gid, fid0);
    }
  }

  local_timer.stop();
  mCheckTime += local_timer.time();

  return first_gid;
}

// @brief 追加できる既存グループを見つけて追加する．
// @param[in] fid 対象の故障番号
// @param[in] group_list 探索最小のグループ番号のリスト
// @param[in] fast 高速ヒューリスティック
// @return 見つかったグループ番号を返す．
//
// 見つからない場合は group_num() を返す．
ymuint
FgMgrBase::find_group2(ymuint fid0,
		       const vector<ymuint>& group_list,
		       bool fast)
{
  StopWatch local_timer;
  local_timer.start();

  const FaultInfo& fi0 = fault_info(fid0);

  SatEngine engine0(string(), string(), NULL);
  GvalCnf gval_cnf0(max_node_id());

  // fi0 の必要割当を追加
  const NodeValList ma_list0 = fi0.mandatory_assignment();
  engine0.add_assignments(gval_cnf0, ma_list0);

  FvalCnf fval_cnf0(max_node_id(), gval_cnf0);
  if ( !fi0.single_cube() ) {
    // fault を検出する CNF を生成
    engine0.make_fval_cnf(fval_cnf0, fault(fid0), node_set(fid0), kVal1);
  }

  ymuint ans_gid = group_num();
  for (ymuint i = 0; i < group_list.size(); ++ i) {
    ymuint gid = group_list[i];

    if ( check_conflict_cache(gid, fid0) ) {
      continue;
    }

    { // グループの十分割当が成り立っていたら両立している．
      const NodeValList& suf_list1 = sufficient_assignment(gid);
      vector<Bool3> sat_model;
      if ( engine0.check_sat(gval_cnf0, suf_list1, sat_model) == kB3True ) {
	add_compat_cache(gid, fid0);

	FaultGroup* fg = fault_group(gid);
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
	ans_gid = gid;
	break;
      }
    }
    if ( fast ) {
      continue;
    }

    { // グループの必要割当が成り立たなかったら衝突している．
      const NodeValList& ma_list1 = mandatory_assignment(gid);
      if ( engine0.check_sat(gval_cnf0, ma_list1) == kB3False ) {
	add_conflict_cache(gid, fid0);
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

    FvalCnf fval_cnf0(max_node_id(), gval_cnf);
    if ( !fi0.single_cube() ) {
      // fid0 を検出する条件を追加
      engine.make_fval_cnf(fval_cnf0, fault(fid0), node_set(fid0), kVal1);
      ++ fnum;
    }

    ymuint nf = fault_num(gid);
    vector<FvalCnf> fval_cnf_array(nf, FvalCnf(max_node_id(), gval_cnf));
    for (ymuint i = 0; i < nf; ++ i) {
      ymuint fid1 = fault_id(gid, i);
      const FaultInfo& fi1 = fault_info(fid1);
      if ( !fi1.single_cube() ) {
	// fid1 の検出条件を生成
	engine.make_fval_cnf(fval_cnf_array[i], fault(fid1), node_set(fid1), kVal1);
	++ fnum;
      }
    }

    mFsum += fnum;
    if ( mFmax < fnum ) {
      mFmax = fnum;
    }
    ++ mMnum;

    vector<Bool3> sat_model;
    if ( engine.check_sat(sat_model) == kB3True ) {
      add_compat_cache(gid, fid0);
      ++ mFoundCount;

      FaultGroup* fg = fault_group(gid);

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
      ans_gid = gid;
      break;
    }
    else {
      add_conflict_cache(gid, fid0);
    }
  }

  local_timer.stop();
  mCheckTime += local_timer.time();

  return ans_gid;
}

// @brief 既存のグループに故障を追加する．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
// @param[in] fid0 故障番号
void
FgMgrBase::add_fault(ymuint gid,
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

// @brief 故障を取り除く
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
// @param[in] fid_list 削除する故障番号のリスト
void
FgMgrBase::delete_faults(ymuint gid,
			 const vector<ymuint>& fid_list)
{
  FaultGroup* fg = fault_group(gid);
  fg->delete_faults(fid_list);
}

// @brief グループの故障数を返す．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
ymuint
FgMgrBase::fault_num(ymuint gid) const
{
  const FaultGroup* fg = fault_group(gid);
  return fg->fault_num();
}

// @brief グループの故障を返す．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
// @param[in] pos ( 0 <= pos < fault_num(gid) )
ymuint
FgMgrBase::fault_id(ymuint gid,
		    ymuint pos) const
{
  const FaultGroup* fg = fault_group(gid);
  return fg->fault_id(pos);
}

// @brief 十分割当リストを返す．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
const NodeValList&
FgMgrBase::sufficient_assignment(ymuint gid) const
{
  const FaultGroup* fg = fault_group(gid);
  return fg->sufficient_assignment();
}

// @brief 必要割当リストを返す．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
const NodeValList&
FgMgrBase::mandatory_assignment(ymuint gid) const
{
  const FaultGroup* fg = fault_group(gid);
  return fg->mandatory_assignment();
}

// @brief 外部入力上の十分割当リストを返す．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
const NodeValList&
FgMgrBase::pi_sufficient_assignment(ymuint gid) const
{
  const FaultGroup* fg = fault_group(gid);
  return fg->pi_sufficient_assignment();
}

// @brief ノード番号の最大値を返す．
ymuint
FgMgrBase::max_node_id() const
{
  return mMaxNodeId;
}

// @brief 故障を返す．
// @param[in] fid 故障番号
const TpgFault*
FgMgrBase::fault(ymuint fid) const
{
  return fault_info(fid).fault();
}

// @brief 故障の解析情報を返す．
// @param[in] fid 故障番号
const FaultInfo&
FgMgrBase::fault_info(ymuint fid) const
{
  return mAnalyzer.fault_info(fid);
}

// @brief 故障に関係するノード集合を返す．
// @param[in] fid 故障番号
const NodeSet&
FgMgrBase::node_set(ymuint fid) const
{
  return mAnalyzer.node_set(fid);
}

// @brief 故障グループを返す．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
FgMgrBase::FaultGroup*
FgMgrBase::fault_group(ymuint gid)
{
  ASSERT_COND( gid < group_num() );
  FaultGroup* fg = mGroupList[gid];
  ASSERT_COND( fg != NULL );
  return fg;
}

// @brief 故障グループを返す．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
const FgMgrBase::FaultGroup*
FgMgrBase::fault_group(ymuint gid) const
{
  ASSERT_COND( gid < group_num() );
  const FaultGroup* fg = mGroupList[gid];
  ASSERT_COND( fg != NULL );
  return fg;
}

// @brief 両立キャッシュに登録する．
void
FgMgrBase::add_compat_cache(ymuint gid,
			    ymuint fid)
{
  FaultGroup* fg = fault_group(gid);
  fg->add_compat_cache(fid);
}

// @brief 両立キャッシュを調べる．
bool
FgMgrBase::check_compat_cache(ymuint gid,
			      ymuint fid)
{
  FaultGroup* fg = fault_group(gid);
  return fg->check_compat_cache(fid);
}

// @brief 衝突キャッシュに登録する
void
FgMgrBase::add_conflict_cache(ymuint gid,
			      ymuint fid)
{
  FaultGroup* fg = fault_group(gid);
  fg->add_conflict_cache(fid);
}

// @brief 衝突キャッシュを調べる．
bool
FgMgrBase::check_conflict_cache(ymuint gid,
				ymuint fid)
{
  FaultGroup* fg = fault_group(gid);
  return fg->check_conflict_cache(fid);
}

// @brief 複数故障の検出検査回数
ymuint
FgMgrBase::mfault_num() const
{
  return mMnum;
}

// @brief 複数故障の平均多重度
double
FgMgrBase::mfault_avg() const
{
  if ( mMnum == 0 ) {
    return 0.0;
  }
  return static_cast<double>(mFsum) / static_cast<double>(mMnum);
}

// @brief 複数故障の最大値
ymuint
FgMgrBase::mfault_max() const
{
  return mFmax;
}

// @brief チェック回数
ymuint
FgMgrBase::check_count() const
{
  return mCheckCount;
}

// @brief チェック時間
USTime
FgMgrBase::check_time() const
{
  return mCheckTime;
}

// @brief 成功回数
ymuint
FgMgrBase::found_count() const
{
  return mFoundCount;
}

// @brief 統計データをクリアする．
void
FgMgrBase::clear_count()
{
  mMnum = 0;
  mFsum = 0;
  mFmax = 0;
  mCheckCount = 0;
  mFoundCount = 0;
  mCheckTime.set(0.0, 0.0, 0.0);
}


//////////////////////////////////////////////////////////////////////
// クラス FgMgrBase::FaultGroup
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
FgMgrBase::FaultGroup::FaultGroup(ymuint id) :
  mId(id)
{
  mCplxNum = 0;
}

// @brief デストラクタ
FgMgrBase::FaultGroup::~FaultGroup()
{
}

// @brief ID番号を返す．
ymuint
FgMgrBase::FaultGroup::id() const
{
  return mId;
}

// 故障数を返す．
ymuint
FgMgrBase::FaultGroup::fault_num() const
{
  return mFaultDataList.size();
}

// single cube でない故障数を返す．
ymuint
FgMgrBase::FaultGroup::complex_fault_num() const
{
  return mCplxNum;
}

// 故障を返す．
ymuint
FgMgrBase::FaultGroup::fault_id(ymuint pos) const
{
  ASSERT_COND( pos < fault_num() );
  return mFaultDataList[pos].mFaultId;
}

// @brief 十分割当を返す．
const NodeValList&
FgMgrBase::FaultGroup::sufficient_assignment() const
{
  return mSufList;
}

// @brief 必要割当を返す．
const NodeValList&
FgMgrBase::FaultGroup::mandatory_assignment() const
{
  return mMaList;
}

// @brief 外部入力上の十分割当を返す．
const NodeValList&
FgMgrBase::FaultGroup::pi_sufficient_assignment() const
{
  return mPiSufList;
}

// @brief 両立キャッシュに登録する．
void
FgMgrBase::FaultGroup::add_compat_cache(ymuint fid)
{
#if defined(USE_CACHE)
  mCompatCache.add(fid);
#endif
}

// @brief 両立キャッシュを調べる．
bool
FgMgrBase::FaultGroup::check_compat_cache(ymuint fid) const
{
#if defined(USE_CACHE)
  return mCompatCache.check(fid);
#else
  return false;
#endif
}

// @brief 衝突キャッシュに登録する．
void
FgMgrBase::FaultGroup::add_conflict_cache(ymuint fid)
{
#if defined(USE_CACHE)
  mConflictCache.add(fid);
#endif
}

// @brief 衝突キャッシュを調べる．
bool
FgMgrBase::FaultGroup::check_conflict_cache(ymuint fid) const
{
#if defined(USE_CACHE)
  return mConflictCache.check(fid);
#else
  return false;
#endif
}

// @brief ID番号以外の内容をコピーする
void
FgMgrBase::FaultGroup::copy(const FaultGroup& dst)
{
  mFaultDataList = dst.mFaultDataList;
  mCplxNum = dst.mCplxNum;
  mSufList = dst.mSufList;
  mMaList = dst.mMaList;
  mPiSufList = dst.mPiSufList;
}

// @brief ID番号をセットする．
void
FgMgrBase::FaultGroup::set_id(ymuint id)
{
  mId = id;
}

// @brief 故障を追加する．
void
FgMgrBase::FaultGroup::add_fault(ymuint fid,
				 const NodeValList& suf_list,
				 const NodeValList& ma_list,
				 const NodeValList& pi_suf_list)
{
  mFaultDataList.push_back(FaultData(fid, suf_list, ma_list, pi_suf_list));
  mSufList.merge(suf_list);
  mMaList.merge(ma_list);
  mPiSufList.merge(pi_suf_list);
  mCompatCache.clear();
}

// @brief 故障を削除する．
void
FgMgrBase::FaultGroup::delete_faults(const vector<ymuint>& fid_list)
{
  // 削除対象の故障番号を持つハッシュ表
  HashSet<ymuint> fault_hash;
  for (ymuint i = 0; i < fid_list.size(); ++ i) {
    ymuint fid = fid_list[i];
    fault_hash.add(fid);
  }

  ymuint nf = fault_num();
  ymuint wpos = 0;
  for (ymuint i = 0; i < nf; ++ i) {
    ymuint fid = fault_id(i);
    if ( fault_hash.check(fid) ) {
      continue;
    }
    if ( wpos != i ) {
      mFaultDataList[wpos] = mFaultDataList[i];
    }
    ++ wpos;
  }
  mFaultDataList.erase(mFaultDataList.begin() + wpos, mFaultDataList.end());

  mConflictCache.clear();

  update();
}

// @brief 故障の十分割当リストを設定する．
void
FgMgrBase::FaultGroup::set_suf_list(ymuint pos,
				    const NodeValList& suf_list,
				    const NodeValList& pi_suf_list)
{
  ASSERT_COND( pos < fault_num() );
  mFaultDataList[pos].mSufList = suf_list;
  mFaultDataList[pos].mPiSufList = pi_suf_list;
}

// @brief 故障リストが変更された時の更新処理を行う．
void
FgMgrBase::FaultGroup::update()
{
  mSufList.clear();
  mMaList.clear();
  ymuint nf = fault_num();
  ymuint wpos = 0;
  for (ymuint i = 0; i < nf; ++ i) {
    mSufList.merge(mFaultDataList[i].mSufList);
    mMaList.merge(mFaultDataList[i].mMaList);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス FgMgrBase::FaultData
//////////////////////////////////////////////////////////////////////

// コンストラクタ
FgMgrBase::FaultGroup::FaultData::FaultData(ymuint fid,
					    const NodeValList& suf_list,
					    const NodeValList& ma_list,
					    const NodeValList& pi_suf_list) :
  mFaultId(fid),
  mSufList(suf_list),
  mMaList(ma_list),
  mPiSufList(pi_suf_list)
{
}

END_NAMESPACE_YM_SATPG
