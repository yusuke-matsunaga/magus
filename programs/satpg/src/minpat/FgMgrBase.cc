
/// @file FgMgrBase.cc
/// @brief FgMgrBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "FgMgrBase.h"

#include "TpgFault.h"
#include "NodeSet.h"
#include "GvalCnf.h"
#include "FvalCnf.h"
#include "SatEngine.h"
#include "FaultAnalyzer.h"
#include "YmUtils/HashSet.h"


BEGIN_NAMESPACE_YM_SATPG

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
// @return グループ番号を返す．
ymuint
FgMgrBase::new_group()
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
FgMgrBase::duplicate_group(ymuint src_gid)
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
  ASSERT_COND( gid < mGroupList.size() );
  FaultGroup* fg = mGroupList[gid];
  ASSERT_COND( fg != NULL );
  delete fg;
  mGroupList[gid] = NULL;
}

// @brief グループの故障数を返す．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
ymuint
FgMgrBase::fault_num(ymuint gid) const
{
  ASSERT_COND( gid < group_num() );
  FaultGroup* fg = mGroupList[gid];
  return fg->fault_num();
}

// @brief グループの故障を返す．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
// @param[in] pos ( 0 <= pos < fault_num(gid) )
const TpgFault*
FgMgrBase::fault(ymuint gid,
		 ymuint pos) const
{
  ASSERT_COND( gid < group_num() );
  FaultGroup* fg = mGroupList[gid];
  return fg->fault(pos);
}

// @brief 十分割当リストを返す．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
const NodeValList&
FgMgrBase::sufficient_assignment(ymuint gid) const
{
  ASSERT_COND( gid < group_num() );
  FaultGroup* fg = mGroupList[gid];
  return fg->sufficient_assignment();
}

// @brief 必要割当リストを返す．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
const NodeValList&
FgMgrBase::mandatory_assignment(ymuint gid) const
{
  ASSERT_COND( gid < group_num() );
  FaultGroup* fg = mGroupList[gid];
  return fg->mandatory_assignment();
}

// @brief 外部入力上の十分割当リストを返す．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
const NodeValList&
FgMgrBase::pi_sufficient_assignment(ymuint gid) const
{
  ASSERT_COND( gid < group_num() );
  FaultGroup* fg = mGroupList[gid];
  return fg->pi_sufficient_assignment();
}

// @brief ノード番号の最大値を返す．
ymuint
FgMgrBase::max_node_id() const
{
  return mMaxNodeId;
}

// @brief 故障の解析情報を返す．
// @param[in] fault 故障
const FaultInfo&
FgMgrBase::fault_info(const TpgFault* fault) const
{
  return mAnalyzer.fault_info(fault->id());
}

// @brief 故障に関係するノード集合を返す．
const NodeSet&
FgMgrBase::node_set(const TpgFault* fault) const
{
  return mAnalyzer.node_set(fault->id());
}

// @brief 故障グループを返す．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
FgMgrBase::FaultGroup*
FgMgrBase::fault_group(ymuint gid)
{
  ASSERT_COND( gid < group_num() );
  return mGroupList[gid];
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
const TpgFault*
FgMgrBase::FaultGroup::fault(ymuint pos) const
{
  ASSERT_COND( pos < fault_num() );
  return mFaultDataList[pos].mFault;
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
FgMgrBase::FaultGroup::add_fault(const TpgFault* fault,
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
FgMgrBase::FaultGroup::delete_faults(const vector<const TpgFault*>& fault_list)
{
  // 削除対象の故障番号を持つハッシュ表
  HashSet<ymuint> fault_hash;
  for (ymuint i = 0; i < fault_list.size(); ++ i) {
    const TpgFault* fault = fault_list[i];
    fault_hash.add(fault->id());
  }

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
  }
  mFaultDataList.erase(mFaultDataList.begin() + wpos, mFaultDataList.end());

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
    const TpgFault* fault = this->fault(i);
    mSufList.merge(mFaultDataList[i].mSufList);
    mMaList.merge(mFaultDataList[i].mMaList);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス FgMgrBase::FaultData
//////////////////////////////////////////////////////////////////////

// コンストラクタ
FgMgrBase::FaultGroup::FaultData::FaultData(const TpgFault* fault,
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
