
/// @file FgMgr.cc
/// @brief FgMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "FgMgr.h"
#include "TvMgr.h"
#include "TpgCnf0.h"
#include "TpgCnf1.h"


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
  fg->mId = mGroupList.size();
  mGroupList.push_back(fg);
  return fg->mId;
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

  TpgCnf1 tpg_cnf(string(), string(), NULL);
  tpg_cnf.make_fval_cnf(fault, mMaxNodeId);

  NodeValList suf_list;
  bool stat = tpg_cnf.get_suf_list(fg->mSufList, suf_list);
  ASSERT_COND( stat );

  fg->mFaultList.push_back(fault);
  fg->mSufList.merge(suf_list);
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
FgMgr::suf_list(ymuint gid) const
{
  ASSERT_COND( gid < group_num() );
  FaultGroup* fg = mGroupList[gid];
  return fg->mSufList;
}

// @brief テストパタンを作る．
// @param[in] gid グループ番号
// @param[in] network ネットワーク
// @param[in] tv テストベクタ
void
FgMgr::make_testvector(ymuint gid,
		       TpgNetwork& network,
		       TestVector* tv)
{
  ASSERT_COND( gid < group_num() );
  const NodeValList& suf_list = mGroupList[gid]->mSufList;
  TpgCnf0 tpg_cnf0(string(), string(), NULL);
  bool stat = tpg_cnf0.get_testvector(network, suf_list, tv);
  ASSERT_COND( stat );
}

END_NAMESPACE_YM_SATPG
