
/// @file ImpNode.cc
/// @brief ImpNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ImpNode.h"
#include "ImpMgr.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス ImpNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] handle0 ファンイン0のハンドル
// @param[in] handle1 ファンイン1のハンドル
ImpNode::ImpNode(ImpNodeHandle handle0,
		 ImpNodeHandle handle1)
{
  mFanins[0].set(handle0, this, 0);
  ImpNode* node0 = handle0.node();
  if ( node0 != NULL ) {
    node0->mFanouts.push_back(&mFanins[0]);
  }
  mFanins[1].set(handle1, this, 1);
  ImpNode* node1 = handle1.node();
  if ( node1 != NULL ) {
    node1->mFanouts.push_back(&mFanins[1]);
  }
}

// @brief デストラクタ
ImpNode::~ImpNode()
{
}

// @brief 外部入力のときに true を返す．
bool
ImpNode::is_input() const
{
  return false;
}

// @brief AND タイプのときに true を返す．
bool
ImpNode::is_and() const
{
  return false;
}

// @brief 定数に設定する．
// @param[in] mgr ImMgr
// @param[in] val 値
void
ImpNode::set_const(ImpMgr& mgr,
		   ymuint val)
{
  const vector<ImpEdge*>& fo_list = fanout_list();
  for (vector<ImpEdge*>::const_iterator p = fo_list.begin();
       p != fo_list.end(); ++ p) {
    ImpEdge* e = *p;
    ImpNode* dst_node = e->dst_node();
    ymuint val1 = val;
    if ( e->src_inv() ) {
      val1 ^= 1;
    }
    dst_node->prop_const(mgr, val1, e->dst_pos());
  }
}

// @brief 0の間接含意を行う．
bool
ImpNode::ind_imp0(ImpMgr& mgr)
{
  for (vector<ImpDst>::iterator p = mImpList[0].begin();
       p != mImpList[0].end(); ++ p) {
    ImpNode* dst_node = p->node();
    bool stat = true;
    if ( p->val() == 0 ) {
      if ( dst_node->val() == kB3X ) {
	stat = mgr.bwd_prop0(dst_node, NULL);
      }
      else if ( dst_node->val() == kB3True ) {
	stat = false;
      }
    }
    else {
      if ( dst_node->val() == kB3X ) {
	stat = mgr.bwd_prop1(dst_node, NULL);
      }
      else if ( dst_node->val() == kB3False ) {
	stat = false;
      }
    }
    if ( !stat ) {
      return false;
    }
  }
  return true;
}

// @brief 1の間接含意を行う．
bool
ImpNode::ind_imp1(ImpMgr& mgr)
{
  for (vector<ImpDst>::iterator p = mImpList[1].begin();
       p != mImpList[1].end(); ++ p) {
    ImpNode* dst_node = p->node();
    bool stat = true;
    if ( p->val() == 0 ) {
      if ( dst_node->val() == kB3X ) {
	stat = mgr.bwd_prop0(p->node(), NULL);
      }
      else if ( dst_node->val() == kB3True ) {
	stat = false;
      }
    }
    else {
      if ( dst_node->val() == kB3X ) {
	stat = mgr.bwd_prop1(p->node(), NULL);
      }
      else if ( dst_node->val() == kB3False ) {
	stat = false;
      }
    }
    if ( !stat ) {
      return false;
    }
  }
  return true;
}

END_NAMESPACE_YM_NETWORKS
