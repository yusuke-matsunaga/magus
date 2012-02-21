
/// @file ImpMgr.cc
/// @brief ImpMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ImpMgr.h"
#include "SnInput.h"
#include "SnAnd.h"
#include "SnXor.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス ImpMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ImpMgr::ImpMgr()
{
}

// @brief デストラクタ
ImpMgr::~ImpMgr()
{
  clear();
}

// @brief 内容を空にする．
void
ImpMgr::clear()
{
  for (vector<StrNode*>::iterator p = mNodeArray.begin();
       p != mNodeArray.end(); ++ p) {
    delete *p;
  }
  mNodeArray.clear();
  mChgStack.clear();
}

// @brief ネットワークを設定する．
// @param[in] src_network 元となるネットワーク
void
ImpMgr::set(const BdnMgr& src_network)
{
  clear();

  ymuint n = network.max_node_id();

  // 配列を確保する．
  mNodeArray.clear();
  mNodeArray.resize(n, NULL);

  // node_list に network のノードをトポロジカル順に並べる．
  vector<BdnNode*> node_list;
  network.sort(node_list);

  // 各ノードのファンアウト数を数える．
  vector<ymuint> fo_count(n, 0);
  for (vector<BdnNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    const BdnNode* bnode = *p;
    const BdnNode* bnode0 = bnode->fanin0();
    ++ fo_count[bnode0->id()];
    const BdnNode* bnode1 = bnode->fanin1();
    ++ fo_count[bnode1->id()];
  }

  // 外部入力ノードを作る．
  const BdnNodeList& input_list = network.input_list();
  for (BdnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const BdnNode* bnode = *p;
    ymuint id = bnode->id();
    StrNode* node = new SnInput(id);
    mNodeArray[id] = node;
    node->set_nfo(fo_count[id]);
  }

  // 論理ノードを作る．
  for (vector<BdnNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    const BdnNode* bnode = *p;
    ymuint id = bnode->id();
    const BdnNode* bnode0 = bnode->fanin0();
    bool inv0 = bnode->fanin0_inv();
    StrNode* node0 = mNodeArray[bnode0->id()];
    assert_cond(node0 != NULL, __FILE__, __LINE__);
    const BdnNode* bnode1 = bnode->fanin1();
    bool inv1 = bnode->fanin1_inv();
    StrNode* node1 = mNodeArray[bnode1->id()];
    assert_cond(node1 != NULL, __FILE__, __LINE__);
    StrNode* node = NULL;
    if ( bnode->is_and() ) {
      node = new SnAnd(id, node0, inv0, node1, inv1);
    }
    else if ( bnode->is_xor() ) {
      node = new SnXor(id, node0, inv0, node1, inv1);
    }
    else {
      assert_not_reached(__FILE__, __LINE__);
    }
    mNodeArray[id] = node;
    node->set_nfo(fo_count[id]);
  }
}

// @brief ノードに値を設定し含意操作を行う．
// @param[in] node ノード
// @param[in] val 設定する値 ( 0 or 1 )
bool
ImpMgr::assert(StrNode* node,
	       ymuint val)
{
  if ( val == 0 ) {
    return bwd_prop0(node, NULL);
  }
  else {
    return bwd_prop1(node, NULL);
  }
}

// @brief 現在のスタックポインタの値を得る．
ymuint
ImpMgr::cur_sp() const
{
}

// @brief 指定されたところまで値を戻す．
// @param[in] pos スタックポインタの値
void
ImpMgr::back(ymuint pos)
{
}

// @brief ノードのファンアウト先に0を伝搬する．
// @param[in] node ノード
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
ImpMgr::fwd_prop0(StrNode* node)
{
  //mLearnedList.push_back(ImpCell(id(), 0));
  const vector<StrEdge*>& fo_list = node->fanout_list();
  for (vector<StrEdge*>::iterator p = fo_list.begin();
       p != fo_list.end(); ++ p) {
    StrEdge* e = *p;
    StrNode* node = e->dst_node();
    bool stat;
    if ( e->src_inv() ) {
      if ( e->dst_pos() == 0 ) {
	stat = node->fwd0_imp1();
      }
      else {
	stat = node->fwd1_imp1();
      }
    }
    else {
      if ( e->dst_pos() == 0 ) {
	stat = node->fwd0_imp0();
      }
      else {
	stat = node->fwd1_imp0();
      }
    }
    if ( !stat ) {
      return stat;
    }
  }
  return true;
}

// @brief ノードのファンアウト先に1を伝搬する．
// @param[in] node ノード
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
ImpMgr::fwd_prop1(StrNode* node)
{
  //mLearnedList.push_back(ImpCell(id(), 1));
  const vector<StrEdge*>& fo_list = node->fanout_list();
  for (vector<StrEdge*>::iterator p = fo_list.begin();
       p != fo_list.end(); ++ p) {
    StrEdge* e = *p;
    StrNode* node = e->dst_node();
    bool stat;
    if ( e->src_inv() ) {
      if ( e->dst_pos() == 0 ) {
	stat = node->fwd0_imp0();
      }
      else {
	stat = node->fwd1_imp0();
      }
    }
    else {
      if ( e->dst_pos() == 0 ) {
	stat = node->fwd0_imp1();
      }
      else {
	stat = node->fwd1_imp1();
      }
    }
    if ( !stat ) {
      return stat;
    }
  }
  return true;
}

// @brief ノードのファンイン0に0を伝搬する．
// @param[in] node ノード
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
ImpMgr::fanin0_prop0(StrNode* node)
{
  const StrEdge& e = fanin0();
  StrNode* dst_node = e.src_node();
  if ( e.src_inv() ) {
    return bwd_prop1(dst_node, node);
  }
  else {
    return bwd_prop0(dst_node, node);
  }
}

// @brief ノードのファンイン0に1を伝搬する．
// @param[in] node ノード
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
ImpMgr::fanin0_prop1(StrNode* node)
{
  const StrEdge& e = fanin0();
  StrNode* dst_node = e.src_node();
  if ( e.src_inv() ) {
    return bwd_prop0(dst_node, node);
  }
  else {
    return bwd_prop1(dst_node, node);
  }
}

// @brief ノードのファンイン1に0を伝搬する．
// @param[in] node ノード
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
ImpMgr::fanin1_prop0(StrNode* node)
{
  const StrEdge& e = fanin1();
  StrNode* dst_node = e.src_node();
  if ( e.src_inv() ) {
    return bwd_prop1(dst_node, node);
  }
  else {
    return bwd_prop0(dst_node, node);
  }
}

// @brief ノードのファンイン1に1を伝搬する．
// @param[in] node ノード
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
ImpMgr::fanin1_prop1(StrNode* node)
{
  const StrEdge& e = fanin1();
  StrNode* dst_node = e.src_node();
  if ( e.src_inv() ) {
    return bwd_prop0(dst_node, node);
  }
  else {
    return bwd_prop1(dst_node, node);
  }
}

// @brief ノードに後方含意で0を割り当てる．
// @param[in] node ノード
// @param[in] from_node 含意元のノード
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
ImpMgr::bwd_prop0(StrNode* node,
		  StrNode* from_node)
{
  const vector<StrEdge*>& fo_list = node->fanout_list();
  for (vector<StrEdge*>::iterator p = fo_list.begin();
       p != fo_list.end(); ++ p) {
    StrEdge* e = *p;
    StrNode* dst_node = e->dst_node();
    if ( dst_node == from_node ) continue;
    bool stat;
    if ( e->src_inv() ) {
      if ( e->dst_pos() == 0 ) {
	stat = dst_node->fwd0_imp1();
      }
      else {
	stat = dst_node->fwd1_imp1();
      }
    }
    else {
      if ( e->dst_pos() == 0 ) {
	stat = dst_node->fwd0_imp0();
      }
      else {
	stat = dst_node->fwd1_imp0();
      }
    }
    if ( !stat ) {
      return stat;
    }
  }
  if ( node->val() == kB3X ) {
    //mLearnedList.push_back(ImpCell(id(), 0));
  }
  return node->bwd_imp0();
}

// @brief ノードに後方含意で1を割り当てる．
// @param[in] node ノード
// @param[in] from_node 含意元のノード
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
ImpMgr::bwd_prop1(StrNode* node,
		  StrNode* from_node)
{
  const vector<StrEdge*>& fo_list = node->fanout_list();
  for (vector<StrEdge*>::iterator p = fo_list.begin();
       p != fo_list.end(); ++ p) {
    StrEdge* e = *p;
    StrNode* dst_node = e->dst_node();
    if ( dst_node == from_node ) continue;
    bool stat;
    if ( e->src_inv() ) {
      if ( e->dst_pos() == 0 ) {
	stat = dst_node->fwd0_imp0();
      }
      else {
	stat = dst_node->fwd1_imp0();
      }
    }
    else {
      if ( e->dst_pos() == 0 ) {
	stat = dst_node->fwd0_imp1();
      }
      else {
	stat = dst_node->fwd1_imp1();
      }
    }
    if ( !stat ) {
      return stat;
    }
  }
  if ( val() == kB3X ) {
    //mLearnedList.push_back(ImpCell(id(), 1));
  }
  return node->bwd_imp1();
}

END_NAMESPACE_YM_NETWORKS

