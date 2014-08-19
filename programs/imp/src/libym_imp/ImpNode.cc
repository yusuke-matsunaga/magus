
/// @file ImpNode.cc
/// @brief ImpNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ImpNode.h"
#include "ImpMgr.h"
#include "ImpRec.h"


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
  mFanins[1].set(handle1, this, 1);

  mFoNum = 0;
  mFanouts = NULL;

  mConstState = 0U;
}

// @brief デストラクタ
ImpNode::~ImpNode()
{
  delete [] mFanouts;
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
  mConstState = (1U << val);
  for (ymuint i = 0; i < mFoNum; ++ i) {
    ImpEdge* e = mFanouts[i];
    ImpNode* dst_node = e->dst_node();
    ymuint val1 = val;
    if ( e->src_inv() ) {
      val1 ^= 1;
    }
    dst_node->prop_const(mgr, val1, e->dst_pos());
  }
}

// @brief ノードに後方含意で0を割り当てる．
// @param[in] mgr ImpMgr
// @param[in] from_node 含意元のノード
// @param[in] rec 含意を記録するオブジェクト
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
ImpNode::bwd_prop0(ImpMgr& mgr,
		   ImpNode* from_node,
		   ImpRec& rec)
{
  return bwd_imp0(mgr, rec) && fanout_prop0(mgr, from_node, rec);
}

// @brief ノードに後方含意で1を割り当てる．
// @param[in] mgr ImpMgr
// @param[in] from_node 含意元のノード
// @param[in] rec 含意を記録するオブジェクト
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
ImpNode::bwd_prop1(ImpMgr& mgr,
		   ImpNode* from_node,
		   ImpRec& rec)
{
  return bwd_imp1(mgr, rec) && fanout_prop1(mgr, from_node, rec);
}

// @brief ノードのファンアウト先に0を伝搬する．
// @param[in] mgr ImpMgr
// @param[in] from 後方含意の場合の含意元のノード
// @param[in] rec 含意を記録するオブジェクト
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
ImpNode::fanout_prop0(ImpMgr& mgr,
		      ImpNode* from,
		      ImpRec& rec)
{
  rec.record(this, 0);

  for (ymuint i = 0; i < mFoNum; ++ i) {
    ImpEdge* e = mFanouts[i];
    ImpNode* node = e->dst_node();
    if ( node == from ) {
      continue;
    }
    bool stat;
    if ( e->src_inv() ) {
      if ( e->dst_pos() == 0 ) {
	stat = node->fwd0_imp1(mgr, rec);
      }
      else {
	stat = node->fwd1_imp1(mgr, rec);
      }
    }
    else {
      if ( e->dst_pos() == 0 ) {
	stat = node->fwd0_imp0(mgr, rec);
      }
      else {
	stat = node->fwd1_imp0(mgr, rec);
      }
    }
    if ( !stat ) {
      return stat;
    }
  }
  return true;
}

// @brief ノードのファンアウト先に1を伝搬する．
// @param[in] mgr ImpMgr
// @param[in] from 後方含意の場合の含意元のノード
// @param[in] rec 含意を記録するオブジェクト
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
ImpNode::fanout_prop1(ImpMgr& mgr,
		      ImpNode* from,
		      ImpRec& rec)
{
  rec.record(this, 1);

  for (ymuint i = 0; i < mFoNum; ++ i) {
    ImpEdge* e = mFanouts[i];
    ImpNode* node = e->dst_node();
    if ( node == from ) {
      continue;
    }
    bool stat;
    if ( e->src_inv() ) {
      if ( e->dst_pos() == 0 ) {
	stat = node->fwd0_imp0(mgr, rec);
      }
      else {
	stat = node->fwd1_imp0(mgr, rec);
      }
    }
    else {
      if ( e->dst_pos() == 0 ) {
	stat = node->fwd0_imp1(mgr, rec);
      }
      else {
	stat = node->fwd1_imp1(mgr, rec);
      }
    }
    if ( !stat ) {
      return stat;
    }
  }
  return true;
}

// @brief ノードに後方含意で0を割り当てる．
// @param[in] mgr ImpMgr
// @param[in] from_node 含意元のノード
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
ImpNode::bwd_prop0(ImpMgr& mgr,
		   ImpNode* from_node)
{
  return bwd_imp0(mgr) && fanout_prop0(mgr, from_node);
}

// @brief ノードに後方含意で1を割り当てる．
// @param[in] mgr ImpMgr
// @param[in] from_node 含意元のノード
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
ImpNode::bwd_prop1(ImpMgr& mgr,
		   ImpNode* from_node)
{
  return bwd_imp1(mgr) && fanout_prop1(mgr, from_node);
}

// @brief ノードのファンアウト先に0を伝搬する．
// @param[in] node ノード
// @param[in] from 後方含意の場合の含意元のノード
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
ImpNode::fanout_prop0(ImpMgr& mgr,
		      ImpNode* from)
{
  for (ymuint i = 0; i < mFoNum; ++ i) {
    ImpEdge* e = mFanouts[i];
    ImpNode* node = e->dst_node();
    if ( node == from ) {
      continue;
    }
    bool stat;
    if ( e->src_inv() ) {
      if ( e->dst_pos() == 0 ) {
	stat = node->fwd0_imp1(mgr);
      }
      else {
	stat = node->fwd1_imp1(mgr);
      }
    }
    else {
      if ( e->dst_pos() == 0 ) {
	stat = node->fwd0_imp0(mgr);
      }
      else {
	stat = node->fwd1_imp0(mgr);
      }
    }
    if ( !stat ) {
      return stat;
    }
  }

  ind_imp0(mgr);

  return true;
}

// @brief ノードのファンアウト先に1を伝搬する．
// @param[in] mgr ImpMgr
// @param[in] from 後方含意の場合の含意元のノード
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
ImpNode::fanout_prop1(ImpMgr& mgr,
		      ImpNode* from)
{
  for (ymuint i = 0; i < mFoNum; ++ i) {
    ImpEdge* e = mFanouts[i];
    ImpNode* node = e->dst_node();
    if ( node == from ) {
      continue;
    }
    bool stat;
    if ( e->src_inv() ) {
      if ( e->dst_pos() == 0 ) {
	stat = node->fwd0_imp0(mgr);
      }
      else {
	stat = node->fwd1_imp0(mgr);
      }
    }
    else {
      if ( e->dst_pos() == 0 ) {
	stat = node->fwd0_imp1(mgr);
      }
      else {
	stat = node->fwd1_imp1(mgr);
      }
    }
    if ( !stat ) {
      return stat;
    }
  }

  ind_imp1(mgr);

  return true;
}

// @brief ノードのファンイン0に0を伝搬する．
// @param[in] mgr ImpMgr
// @param[in] rec 含意を記録するオブジェクト
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
ImpNode::fanin0_prop0(ImpMgr& mgr,
		      ImpRec& rec)
{
  const ImpEdge& e = fanin0();
  ImpNode* dst_node = e.src_node();
  if ( e.src_inv() ) {
    return dst_node->bwd_prop1(mgr, this, rec);
  }
  else {
    return dst_node->bwd_prop0(mgr, this, rec);
  }
}

// @brief ノードのファンイン0に1を伝搬する．
// @param[in] mgr ImpMgr
// @param[in] rec 含意を記録するオブジェクト
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
ImpNode::fanin0_prop1(ImpMgr& mgr,
		      ImpRec& rec)
{
  const ImpEdge& e = fanin0();
  ImpNode* dst_node = e.src_node();
  if ( e.src_inv() ) {
    return dst_node->bwd_prop0(mgr, this, rec);
  }
  else {
    return dst_node->bwd_prop1(mgr, this, rec);
  }
}

// @brief ノードのファンイン1に0を伝搬する．
// @param[in] mgr ImpMgr
// @param[in] rec 含意を記録するオブジェクト
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
ImpNode::fanin1_prop0(ImpMgr& mgr,
		      ImpRec& rec)
{
  const ImpEdge& e = fanin1();
  ImpNode* dst_node = e.src_node();
  if ( e.src_inv() ) {
    return dst_node->bwd_prop1(mgr, this, rec);
  }
  else {
    return dst_node->bwd_prop0(mgr, this, rec);
  }
}

// @brief ノードのファンイン1に1を伝搬する．
// @param[in] mgr ImpMgr
// @param[in] rec 含意を記録するオブジェクト
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
ImpNode::fanin1_prop1(ImpMgr& mgr,
		      ImpRec& rec)
{
  const ImpEdge& e = fanin1();
  ImpNode* dst_node = e.src_node();
  if ( e.src_inv() ) {
    return dst_node->bwd_prop0(mgr, this, rec);
  }
  else {
    return dst_node->bwd_prop1(mgr, this, rec);
  }
}

// @brief ノードのファンイン0に0を伝搬する．
// @param[in] mgr ImpMgr
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
ImpNode::fanin0_prop0(ImpMgr& mgr)
{
  const ImpEdge& e = fanin0();
  ImpNode* dst_node = e.src_node();
  if ( e.src_inv() ) {
    return dst_node->bwd_prop1(mgr, this);
  }
  else {
    return dst_node->bwd_prop0(mgr, this);
  }
}

// @brief ノードのファンイン0に1を伝搬する．
// @param[in] mgr ImpMgr
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
ImpNode::fanin0_prop1(ImpMgr& mgr)
{
  const ImpEdge& e = fanin0();
  ImpNode* dst_node = e.src_node();
  if ( e.src_inv() ) {
    return dst_node->bwd_prop0(mgr, this);
  }
  else {
    return dst_node->bwd_prop1(mgr, this);
  }
}

// @brief ノードのファンイン1に0を伝搬する．
// @param[in] mgr ImpMgr
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
ImpNode::fanin1_prop0(ImpMgr& mgr)
{
  const ImpEdge& e = fanin1();
  ImpNode* dst_node = e.src_node();
  if ( e.src_inv() ) {
    return dst_node->bwd_prop1(mgr, this);
  }
  else {
    return dst_node->bwd_prop0(mgr, this);
  }
}

// @brief ノードのファンイン1に1を伝搬する．
// @param[in] mgr ImpMgr
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
ImpNode::fanin1_prop1(ImpMgr& mgr)
{
  const ImpEdge& e = fanin1();
  ImpNode* dst_node = e.src_node();
  if ( e.src_inv() ) {
    return dst_node->bwd_prop0(mgr, this);
  }
  else {
    return dst_node->bwd_prop1(mgr, this);
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
	stat = dst_node->bwd_prop0(mgr, NULL);
      }
      else if ( dst_node->val() == kB3True ) {
	stat = false;
      }
    }
    else {
      if ( dst_node->val() == kB3X ) {
	stat = dst_node->bwd_prop1(mgr, NULL);
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
	stat = dst_node->bwd_prop0(mgr, NULL);
      }
      else if ( dst_node->val() == kB3True ) {
	stat = false;
      }
    }
    else {
      if ( dst_node->val() == kB3X ) {
	stat = dst_node->bwd_prop1(mgr, NULL);
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
