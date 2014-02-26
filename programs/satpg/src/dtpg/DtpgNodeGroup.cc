
/// @file DtpgNodeGroup.cc
/// @brief DtpgNodeGroup の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgNodeGroup.h"
#include "DtpgNgEdge.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス DtpgNodeGroup
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] node_list ノードのリスト
DtpgNodeGroup::DtpgNodeGroup(const vector<TpgNode*>& node_list) :
  mNodeList(node_list)
{
}

// @brief デストラクタ
DtpgNodeGroup::~DtpgNodeGroup()
{
}

// @brief ファンインのリスト中に src_node があるかチェックする．
bool
DtpgNodeGroup::check_fanin(DtpgNodeGroup* src_node) const
{
  for (vector<DtpgNgEdge*>::const_iterator p = mFaninList.begin();
       p != mFaninList.end(); ++ p) {
    DtpgNgEdge* edge = *p;
    if ( edge->src_node() == src_node ) {
      return true;
    }
  }
  return false;
}

// @brief ファンアウトのリスト中に dst_node があるかチェックする．
bool
DtpgNodeGroup::check_fanout(DtpgNodeGroup* dst_node) const
{
  for (vector<DtpgNgEdge*>::const_iterator p = mFanoutList.begin();
       p != mFanoutList.end(); ++ p) {
    DtpgNgEdge* edge = *p;
    if ( edge->dst_node() == dst_node ) {
      return true;
    }
  }
  return false;
}

// @brief ファンインを削除する．
void
DtpgNodeGroup::delete_fanin(DtpgNgEdge* edge)
{
  vector<DtpgNgEdge*>::iterator wpos = mFaninList.begin();
  for (vector<DtpgNgEdge*>::iterator rpos = wpos;
       rpos != mFaninList.end(); ++ rpos) {
    DtpgNgEdge* edge1 = *rpos;
    if ( edge1 != edge ) {
      if ( wpos != rpos ) {
	*wpos = edge1;
      }
      ++ wpos;
    }
  }
  if ( wpos != mFaninList.end() ) {
    mFaninList.erase(wpos, mFaninList.end());
  }
}

// @brief ファンアウトを削除する．
void
DtpgNodeGroup::delete_fanout(DtpgNgEdge* edge)
{
  vector<DtpgNgEdge*>::iterator wpos = mFanoutList.begin();
  for (vector<DtpgNgEdge*>::iterator rpos = wpos;
       rpos != mFanoutList.end(); ++ rpos) {
    DtpgNgEdge* edge1 = *rpos;
    if ( edge1 != edge ) {
      if ( wpos != rpos ) {
	*wpos = edge1;
      }
      ++ wpos;
    }
  }
  if ( wpos != mFanoutList.end() ) {
    mFanoutList.erase(wpos, mFanoutList.end());
  }
}

// @brief 内部状態のチェックを行う．
void
DtpgNodeGroup::sanity_check() const
{
  for (vector<DtpgNgEdge*>::const_iterator p = mFaninList.begin();
       p != mFaninList.end(); ++ p) {
    DtpgNgEdge* edge = *p;
    assert_cond( edge->dst_node() == this, __FILE__, __LINE__);
  }

  for (vector<DtpgNgEdge*>::const_iterator p = mFanoutList.begin();
       p != mFanoutList.end(); ++ p) {
    DtpgNgEdge* edge = *p;
    assert_cond( edge->src_node() == this, __FILE__, __LINE__);
  }
}

END_NAMESPACE_YM_SATPG
