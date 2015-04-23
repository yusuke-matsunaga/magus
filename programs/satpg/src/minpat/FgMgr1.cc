
/// @file FgMgr1.cc
/// @brief FgMgr1 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "FgMgr1.h"

#include "TpgFault.h"
#include "NodeSet.h"
#include "GvalCnf.h"
#include "FvalCnf.h"
#include "SatEngine.h"
#include "FaultAnalyzer.h"
#include "YmUtils/HashSet.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス FgMgr1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] max_node_id ノード番号の最大値 + 1
// @param[in] analyzer 故障解析器
FgMgr1::FgMgr1(ymuint max_node_id,
	       const FaultAnalyzer& analyzer) :
  mMaxNodeId(max_node_id),
  mAnalyzer(analyzer)
{
}

// @brief デストラクタ
FgMgr1::~FgMgr1()
{
  clear();
}

// @brief クリアする．
void
FgMgr1::clear()
{
  for (ymuint gid = 0; gid < mGroupList.size(); ++ gid) {
    delete mGroupList[gid];
  }
  mGroupList.clear();
}

// @brief 現在のグループ数を返す．
ymuint
FgMgr1::group_num() const
{
  return mGroupList.size();
}

// @brief 新しいグループを作る．
// @return グループ番号を返す．
ymuint
FgMgr1::new_group()
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
  return fg->id();
}

// @brief グループを複製する．
// @param[in] src_gid 複製元のグループ番号
// @return 新しいグループ番号を返す．
ymuint
FgMgr1::duplicate_group(ymuint src_gid)
{
  ASSERT_COND( src_gid < mGroupList.size() );
  FaultGroup* src_fg = mGroupList[src_gid];
  ASSERT_COND( src_fg != NULL );
  ymuint gid = new_group();
  FaultGroup* dst_fg = mGroupList[gid];
  dst_fg->copy(*src_fg);
  return gid;
}

// @brief グループを置き換える．
// @param[in] old_gid 置き換え対象のグループ番号
// @param[in] new_gid 置き換えるグループ番号
//
// new_gid は削除される．
void
FgMgr1::replace_group(ymuint old_gid,
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
FgMgr1::delete_group(ymuint gid)
{
  ASSERT_COND( gid < mGroupList.size() );
  FaultGroup* fg = mGroupList[gid];
  ASSERT_COND( fg != NULL );
  delete fg;
  mGroupList[gid] = NULL;
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
FgMgr1::find_dom_group(const TpgFault* fault,
		      const vector<ymuint>& group_list,
		      bool first_hit,
		      vector<ymuint>& gid_list)
{
  GvalCnf gval_cnf(mMaxNodeId);
  FvalCnf fval_cnf(mMaxNodeId, gval_cnf);
  SatEngine engine(string(), string(), NULL);

  const NodeSet& node_set = mAnalyzer.node_set(fault->id());

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
FgMgr1::find_group(const TpgFault* fault,
		  const vector<ymuint>& group_list,
		  bool first_hit,
		  vector<ymuint>& gid_list)
{
  const FaultInfo& fi = mAnalyzer.fault_info(fault->id());

  GvalCnf gval_cnf(mMaxNodeId);
  SatEngine engine(string(), string(), NULL);

  ymuint first_gid = group_num();

  if ( fi.single_cube() ) {
    NodeValList suf_list0 = fi.sufficient_assignment();
    for (ymuint i = 0; i < group_list.size(); ++ i) {
      ymuint gid = group_list[i];
      const NodeValList& suf_list1 = sufficient_assignment(gid);
      if ( engine.check_sat(gval_cnf, suf_list0, suf_list1) == kB3True ) {
	if ( first_gid == group_num() ) {
	  first_gid = gid;
	  if ( first_hit ) {
	    break;
	  }
	}
	gid_list.push_back(gid);
      }
    }
  }
  else {
    GvalCnf gval_cnf(mMaxNodeId);
    FvalCnf fval_cnf(mMaxNodeId, gval_cnf);
    SatEngine engine(string(), string(), NULL);

    const NodeSet& node_set = mAnalyzer.node_set(fault->id());

    engine.make_fval_cnf(fval_cnf, fault, node_set, kVal1);

    for (ymuint i = 0; i < group_list.size(); ++ i) {
      ymuint gid = group_list[i];
      const NodeValList& suf_list1 = sufficient_assignment(gid);
      Bool3 sat_ans = engine.check_sat(gval_cnf, suf_list1);
      if ( sat_ans == kB3True ) {
	if ( first_gid == group_num() ) {
	  first_gid = gid;
	  if ( first_hit ) {
	    break;
	  }
	}
	gid_list.push_back(gid);
      }
    }
  }

  return first_gid;
}

// @brief 既存のグループに故障を追加する．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
// @param[in] fault 故障
void
FgMgr1::add_fault(ymuint gid,
		 const TpgFault* fault)
{
  ASSERT_COND( gid < group_num() );
  FaultGroup* fg = mGroupList[gid];

  const FaultInfo& fi = mAnalyzer.fault_info(fault->id());
  const NodeValList& ma_list = fi.mandatory_assignment();

  if ( fi.single_cube() ) {
    const NodeValList& pi_suf_list = fi.pi_sufficient_assignment();
    fg->add_fault(fault, true, ma_list, ma_list, pi_suf_list);
    return;
  }

  GvalCnf gval_cnf(mMaxNodeId);
  FvalCnf fval_cnf(mMaxNodeId, gval_cnf);
  SatEngine engine(string(), string(), NULL);

  const NodeSet& node_set = mAnalyzer.node_set(fault->id());

  engine.make_fval_cnf(fval_cnf, fault, node_set, kVal1);

  vector<Bool3> sat_model;
  Bool3 sat_ans = engine.check_sat(gval_cnf, fg->sufficient_assignment(), sat_model);
  ASSERT_COND( sat_ans == kB3True );

  NodeValList suf_list;
  NodeValList pi_suf_list;
  fval_cnf.get_pi_suf_list(sat_model, fault, node_set, suf_list, pi_suf_list);

  fg->add_fault(fault, false, suf_list, ma_list, pi_suf_list);
}

// @brief 故障を取り除く
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
// @param[in] fault_list 故障リスト
void
FgMgr1::delete_fault(ymuint gid,
		     const vector<const TpgFault*>& fault_list)
{
  ASSERT_COND( gid < group_num() );
  FaultGroup* fg = mGroupList[gid];
  fg->delete_faults(fault_list);
}

// @brief グループの故障数を返す．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
ymuint
FgMgr1::fault_num(ymuint gid) const
{
  ASSERT_COND( gid < group_num() );
  FaultGroup* fg = mGroupList[gid];
  return fg->fault_num();
}

// @brief グループの故障を返す．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
// @param[in] pos ( 0 <= pos < fault_num(gid) )
const TpgFault*
FgMgr1::fault(ymuint gid,
	      ymuint pos) const
{
  ASSERT_COND( gid < group_num() );
  FaultGroup* fg = mGroupList[gid];
  return fg->fault(pos);
}

// @brief 十分割当リストを返す．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
const NodeValList&
FgMgr1::sufficient_assignment(ymuint gid) const
{
  ASSERT_COND( gid < group_num() );
  FaultGroup* fg = mGroupList[gid];
  return fg->sufficient_assignment();
}

// @brief 必要割当リストを返す．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
const NodeValList&
FgMgr1::mandatory_assignment(ymuint gid) const
{
  ASSERT_COND( gid < group_num() );
  FaultGroup* fg = mGroupList[gid];
  return fg->mandatory_assignment();
}

// @brief 外部入力上の十分割当リストを返す．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
const NodeValList&
FgMgr1::pi_sufficient_assignment(ymuint gid) const
{
  ASSERT_COND( gid < group_num() );
  FaultGroup* fg = mGroupList[gid];
  return fg->pi_sufficient_assignment();
}


//////////////////////////////////////////////////////////////////////
// クラス FgMgr1::FaultGroup
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
FgMgr1::FaultGroup::FaultGroup(ymuint id) :
  mId(id)
{
  mCplxNum = 0;
}

// @brief デストラクタ
FgMgr1::FaultGroup::~FaultGroup()
{
}

// @brief ID番号を返す．
ymuint
FgMgr1::FaultGroup::id() const
{
  return mId;
}

// 故障数を返す．
ymuint
FgMgr1::FaultGroup::fault_num() const
{
  return mFaultDataList.size();
}

// single cube でない故障数を返す．
ymuint
FgMgr1::FaultGroup::complex_fault_num() const
{
  return mCplxNum;
}

// 故障を返す．
const TpgFault*
FgMgr1::FaultGroup::fault(ymuint pos) const
{
  ASSERT_COND( pos < fault_num() );
  return mFaultDataList[pos].mFault;
}

// @brief 十分割当を返す．
const NodeValList&
FgMgr1::FaultGroup::sufficient_assignment() const
{
  return mSufList;
}

// @brief 必要割当を返す．
const NodeValList&
FgMgr1::FaultGroup::mandatory_assignment() const
{
  return mMaList;
}

// @brief 外部入力上の十分割当を返す．
const NodeValList&
FgMgr1::FaultGroup::pi_sufficient_assignment() const
{
  return mPiSufList;
}

// @brief ID番号以外の内容をコピーする
void
FgMgr1::FaultGroup::copy(const FaultGroup& dst)
{
  mFaultDataList = dst.mFaultDataList;
  mCplxNum = dst.mCplxNum;
  mSufList = dst.mSufList;
  mMaList = dst.mMaList;
  mPiSufList = dst.mPiSufList;
}

// @brief ID番号をセットする．
void
FgMgr1::FaultGroup::set_id(ymuint id)
{
  mId = id;
}

// @brief 故障を追加する．
void
FgMgr1::FaultGroup::add_fault(const TpgFault* fault,
			      bool single_cube,
			      const NodeValList& suf_list,
			      const NodeValList& ma_list,
			      const NodeValList& pi_suf_list)
{
  mFaultDataList.push_back(FaultData(fault, single_cube, suf_list, ma_list, pi_suf_list));
  mSufList.merge(suf_list);
  mMaList.merge(ma_list);
  mPiSufList.merge(pi_suf_list);
}

// @brief 故障を削除する．
void
FgMgr1::FaultGroup::delete_faults(const vector<const TpgFault*>& fault_list)
{
  // 削除対象の故障番号を持つハッシュ表
  HashSet<ymuint> fault_hash;
  for (ymuint i = 0; i < fault_list.size(); ++ i) {
    const TpgFault* fault = fault_list[i];
    fault_hash.add(fault->id());
  }

  mSufList.clear();
  mMaList.clear();
  ymuint nf = fault_num();
  ymuint wpos = 0;
  for (ymuint i = 0; i < nf; ++ i) {
    const TpgFault* fault = this->fault(i);
    if ( fault_hash.check(fault->id()) ) {
      continue;
    }
    if ( wpos != i ) {
      mFaultDataList[wpos] = mFaultDataList[i];
    }
    ++ wpos;
    mSufList.merge(mFaultDataList[i].mSufList);
    mMaList.merge(mFaultDataList[i].mMaList);
  }
  mFaultDataList.erase(mFaultDataList.begin() + wpos, mFaultDataList.end());
}


//////////////////////////////////////////////////////////////////////
// クラス FgMgr1::FaultData
//////////////////////////////////////////////////////////////////////

// コンストラクタ
FgMgr1::FaultGroup::FaultData::FaultData(const TpgFault* fault,
					 bool single_cube,
					 const NodeValList& suf_list,
					 const NodeValList& ma_list,
					 const NodeValList& pi_suf_list) :
  mFault(fault),
  mSingleCube(single_cube),
  mSufList(suf_list),
  mMaList(ma_list),
  mPiSufList(pi_suf_list)
{
}

END_NAMESPACE_YM_SATPG
