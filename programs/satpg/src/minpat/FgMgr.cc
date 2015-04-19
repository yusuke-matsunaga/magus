
/// @file FgMgr.cc
/// @brief FgMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "FgMgr.h"

#include "GvalCnf.h"
#include "FvalCnf.h"
#include "SatEngine.h"
#include "ModelValMap.h"
#include "NodeSet.h"
#include "TpgFault.h"
#include "YmUtils/HashSet.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス FgMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] max_node_id ノード番号の最大値 + 1
FgMgr::FgMgr(ymuint max_node_id) :
  mMaxNodeId(max_node_id)
{
}

// @brief デストラクタ
FgMgr::~FgMgr()
{
  clear();
}

// @brief クリアする．
void
FgMgr::clear()
{
  for (ymuint gid = 0; gid < mGroupList.size(); ++ gid) {
    delete mGroupList[gid];
  }
  mGroupList.clear();
}

// @brief 現在のグループ数を返す．
ymuint
FgMgr::group_num() const
{
  return mGroupList.size();
}

// @brief 新しいグループを作る．
// @return グループ番号を返す．
ymuint
FgMgr::new_group()
{
  FaultGroup* fg = new FaultGroup;
  bool found = false;
  for (ymuint i = 0; i < mGroupList.size(); ++ i) {
    if ( mGroupList[i] == NULL ) {
      fg->mId = i;
      mGroupList[i] = fg;
      found = true;
      break;
    }
  }
  if ( !found ) {
    fg->mId = mGroupList.size();
    mGroupList.push_back(fg);
  }
  return fg->mId;
}

// @brief グループを複製する．
// @param[in] src_gid 複製元のグループ番号
// @return 新しいグループ番号を返す．
ymuint
FgMgr::duplicate_group(ymuint src_gid)
{
  ASSERT_COND( src_gid < mGroupList.size() );
  FaultGroup* src_fg = mGroupList[src_gid];
  ASSERT_COND( src_fg != NULL );
  ymuint gid = new_group();
  FaultGroup* dst_fg = mGroupList[gid];
  dst_fg->mFaultList = src_fg->mFaultList;
  dst_fg->mFaultSufList = src_fg->mFaultSufList;
  dst_fg->mSufList = src_fg->mSufList;
  return gid;
}

// @brief グループを置き換える．
// @param[in] old_gid 置き換え対象のグループ番号
// @param[in] new_gid 置き換えるグループ番号
//
// new_gid は削除される．
void
FgMgr::replace_group(ymuint old_gid,
		     ymuint new_gid)
{
  delete_group(old_gid);

  ASSERT_COND( new_gid < mGroupList.size() );
  FaultGroup* new_fg = mGroupList[new_gid];
  ASSERT_COND( new_fg != NULL );
  new_fg->mId = old_gid;
  mGroupList[old_gid] = new_fg;
  mGroupList[new_gid] = NULL;
}

// @brief グループを削除する．
// @param[in] gid グループ番号
void
FgMgr::delete_group(ymuint gid)
{
  ASSERT_COND( gid < mGroupList.size() );
  FaultGroup* fg = mGroupList[gid];
  ASSERT_COND( fg != NULL );
  delete fg;
  mGroupList[gid] = NULL;
}

// @brief 既存のグループに故障を追加する．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
// @param[in] fault 故障
void
FgMgr::add_fault(ymuint gid,
		 TpgFault* fault)
{
  ASSERT_COND( gid < group_num() );
  FaultGroup* fg = mGroupList[gid];

  GvalCnf gval_cnf(mMaxNodeId);
  FvalCnf fval_cnf(mMaxNodeId, gval_cnf);
  SatEngine engine(string(), string(), NULL);

  NodeSet node_set;
  node_set.mark_region(mMaxNodeId, fault->node());

  fval_cnf.make_cnf(engine, fault, node_set, kVal1);

  NodeValList suf_list;
  NodeValList pi_suf_list;
  Bool3 sat_ans = engine.get_pi_suf_list(fval_cnf, fault, fg->mSufList, suf_list, pi_suf_list);
  ASSERT_COND( sat_ans == kB3True );

  fg->add_fault(fault, suf_list, pi_suf_list);
}

// @brief 故障を取り除く
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
// @param[in] fault_list 故障リスト
void
FgMgr::delete_fault(ymuint gid,
		    const vector<TpgFault*>& fault_list)
{
  ASSERT_COND( gid < group_num() );
  FaultGroup* fg = mGroupList[gid];

  HashSet<ymuint> fault_hash;
  for (ymuint i = 0; i < fault_list.size(); ++ i) {
    TpgFault* fault = fault_list[i];
    fault_hash.add(fault->id());
  }

  fg->mSufList = NodeValList();
  ymuint nf = fg->mFaultList.size();
  ymuint wpos = 0;
  for (ymuint i = 0; i < nf; ++ i) {
    TpgFault* fault = fg->mFaultList[i];
    if ( fault_hash.check(fault->id()) ) {
      continue;
    }
    if ( wpos != i ) {
      fg->mFaultList[wpos] = fault;
      fg->mFaultSufList[wpos] = fg->mFaultSufList[i];
    }
    ++ wpos;
    fg->mSufList.merge(fg->mFaultSufList[i]);
  }
  fg->mFaultList.erase(fg->mFaultList.begin() + wpos, fg->mFaultList.end());
  fg->mFaultSufList.erase(fg->mFaultSufList.begin() + wpos, fg->mFaultSufList.end());
}

// @brief 故障リストを返す．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
const vector<TpgFault*>&
FgMgr::fault_list(ymuint gid) const
{
  ASSERT_COND( gid < group_num() );
  FaultGroup* fg = mGroupList[gid];
  return fg->mFaultList;
}

// @brief 十分割当リストを返す．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
const NodeValList&
FgMgr::sufficient_assignment(ymuint gid) const
{
  ASSERT_COND( gid < group_num() );
  FaultGroup* fg = mGroupList[gid];
  return fg->mSufList;
}

// @brief 外部入力上の十分割当リストを返す．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
const NodeValList&
FgMgr::pi_sufficient_assignment(ymuint gid) const
{
  ASSERT_COND( gid < group_num() );
  FaultGroup* fg = mGroupList[gid];
  return fg->mPiSufList;
}

END_NAMESPACE_YM_SATPG
