
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
#include "ym_networks/BdnNode.h"


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
  mInputArray.clear();
  mNodeList.clear();
  mNodeArray.clear();
  mChgStack.clear();
}

// @brief ネットワークを設定する．
// @param[in] src_network 元となるネットワーク
void
ImpMgr::set(const BdnMgr& src_network)
{
  clear();

  ymuint n = src_network.max_node_id();

  // 配列を確保する．
  mNodeArray.clear();
  mNodeArray.resize(n, NULL);

  // node_list に src_network のノードをトポロジカル順に並べる．
  vector<BdnNode*> node_list;
  src_network.sort(node_list);

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
  const BdnNodeList& input_list = src_network.input_list();
  mInputArray.reserve(input_list.size());
  for (BdnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const BdnNode* bnode = *p;
    ymuint id = bnode->id();
    StrNode* node = new SnInput(id);
    node->mListIter = mUnodeList.end();
    mInputArray.push_back(node);
    mNodeArray[id] = node;
    node->set_nfo(fo_count[id]);
  }

  // 論理ノードを作る．
  mNodeList.reserve(node_list.size());
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
    node->mListIter = mUnodeList.end();
    mNodeList.push_back(node);
    mNodeArray[id] = node;
    node->set_nfo(fo_count[id]);
  }
}

// @brief ノードに値を設定し含意操作を行う．
// @param[in] node ノード
// @param[in] val 設定する値 ( 0 or 1 )
// @param[out] imp_list 含意の結果を格納するリスト
bool
ImpMgr::assert(StrNode* node,
	       ymuint val,
	       vector<ImpVal>& imp_list)
{
  mSrcId = node->id();
  mMarkerStack.push_back(mChgStack.size());
  if ( val == 0 ) {
    return bwd_prop0(node, NULL, imp_list);
  }
  else {
    return bwd_prop1(node, NULL, imp_list);
  }
}

// @brief 指定されたところまで値を戻す．
void
ImpMgr::backtrack()
{
  ymuint pos = mMarkerStack.back();
  mMarkerStack.pop_back();
  for (ymuint i = mChgStack.size(); i > pos; ) {
    -- i;
    NodeChg& nc = mChgStack[i];
    StrNode* node = nc.mNode;
    node->restore(nc.mState);
    if ( node->is_unjustified() ) {
      if ( node->mListIter == mUnodeList.end() ) {
	set_unjustified(node);
      }
    }
    else {
      if ( node->mListIter != mUnodeList.end() ) {
	reset_unjustified(node);
      }
    }
  }
  mChgStack.erase(mChgStack.begin() + pos, mChgStack.end());
}

// @brief ノードのファンアウト先に0を伝搬する．
// @param[in] node ノード
// @param[out] imp_list 含意の結果を格納するリスト
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
ImpMgr::fwd_prop0(StrNode* node,
		  vector<ImpVal>& imp_list)
{
  if ( node->id() != mSrcId ) {
    imp_list.push_back(ImpVal(node->id(), 0));
  }
  const vector<StrEdge*>& fo_list = node->fanout_list();
  for (vector<StrEdge*>::const_iterator p = fo_list.begin();
       p != fo_list.end(); ++ p) {
    StrEdge* e = *p;
    StrNode* node = e->dst_node();
    bool stat;
    if ( e->src_inv() ) {
      if ( e->dst_pos() == 0 ) {
	stat = node->fwd0_imp1(*this, imp_list);
      }
      else {
	stat = node->fwd1_imp1(*this, imp_list);
      }
    }
    else {
      if ( e->dst_pos() == 0 ) {
	stat = node->fwd0_imp0(*this, imp_list);
      }
      else {
	stat = node->fwd1_imp0(*this, imp_list);
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
// @param[out] imp_list 含意の結果を格納するリスト
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
ImpMgr::fwd_prop1(StrNode* node,
		  vector<ImpVal>& imp_list)
{
  if ( node->id() != mSrcId ) {
    imp_list.push_back(ImpVal(node->id(), 1));
  }
  const vector<StrEdge*>& fo_list = node->fanout_list();
  for (vector<StrEdge*>::const_iterator p = fo_list.begin();
       p != fo_list.end(); ++ p) {
    StrEdge* e = *p;
    StrNode* node = e->dst_node();
    bool stat;
    if ( e->src_inv() ) {
      if ( e->dst_pos() == 0 ) {
	stat = node->fwd0_imp0(*this, imp_list);
      }
      else {
	stat = node->fwd1_imp0(*this, imp_list);
      }
    }
    else {
      if ( e->dst_pos() == 0 ) {
	stat = node->fwd0_imp1(*this, imp_list);
      }
      else {
	stat = node->fwd1_imp1(*this, imp_list);
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
// @param[out] imp_list 含意の結果を格納するリスト
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
ImpMgr::fanin0_prop0(StrNode* node,
		     vector<ImpVal>& imp_list)
{
  const StrEdge& e = node->fanin0();
  StrNode* dst_node = e.src_node();
  if ( e.src_inv() ) {
    return bwd_prop1(dst_node, node, imp_list);
  }
  else {
    return bwd_prop0(dst_node, node, imp_list);
  }
}

// @brief ノードのファンイン0に1を伝搬する．
// @param[in] node ノード
// @param[out] imp_list 含意の結果を格納するリスト
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
ImpMgr::fanin0_prop1(StrNode* node,
		     vector<ImpVal>& imp_list)
{
  const StrEdge& e = node->fanin0();
  StrNode* dst_node = e.src_node();
  if ( e.src_inv() ) {
    return bwd_prop0(dst_node, node, imp_list);
  }
  else {
    return bwd_prop1(dst_node, node, imp_list);
  }
}

// @brief ノードのファンイン1に0を伝搬する．
// @param[in] node ノード
// @param[out] imp_list 含意の結果を格納するリスト
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
ImpMgr::fanin1_prop0(StrNode* node,
		     vector<ImpVal>& imp_list)
{
  const StrEdge& e = node->fanin1();
  StrNode* dst_node = e.src_node();
  if ( e.src_inv() ) {
    return bwd_prop1(dst_node, node, imp_list);
  }
  else {
    return bwd_prop0(dst_node, node, imp_list);
  }
}

// @brief ノードのファンイン1に1を伝搬する．
// @param[in] node ノード
// @param[out] imp_list 含意の結果を格納するリスト
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
ImpMgr::fanin1_prop1(StrNode* node,
		     vector<ImpVal>& imp_list)
{
  const StrEdge& e = node->fanin1();
  StrNode* dst_node = e.src_node();
  if ( e.src_inv() ) {
    return bwd_prop0(dst_node, node, imp_list);
  }
  else {
    return bwd_prop1(dst_node, node, imp_list);
  }
}

// @brief ノードに後方含意で0を割り当てる．
// @param[in] node ノード
// @param[in] from_node 含意元のノード
// @param[out] imp_list 含意の結果を格納するリスト
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
ImpMgr::bwd_prop0(StrNode* node,
		  StrNode* from_node,
		  vector<ImpVal>& imp_list)
{
  const vector<StrEdge*>& fo_list = node->fanout_list();
  for (vector<StrEdge*>::const_iterator p = fo_list.begin();
       p != fo_list.end(); ++ p) {
    StrEdge* e = *p;
    StrNode* dst_node = e->dst_node();
    if ( dst_node == from_node ) continue;
    bool stat;
    if ( e->src_inv() ) {
      if ( e->dst_pos() == 0 ) {
	stat = dst_node->fwd0_imp1(*this, imp_list);
      }
      else {
	stat = dst_node->fwd1_imp1(*this, imp_list);
      }
    }
    else {
      if ( e->dst_pos() == 0 ) {
	stat = dst_node->fwd0_imp0(*this, imp_list);
      }
      else {
	stat = dst_node->fwd1_imp0(*this, imp_list);
      }
    }
    if ( !stat ) {
      return stat;
    }
  }
  if ( node->id() != mSrcId && node->val() == kB3X ) {
    imp_list.push_back(ImpVal(node->id(), 0));
  }
  return node->bwd_imp0(*this, imp_list);
}

// @brief ノードに後方含意で1を割り当てる．
// @param[in] node ノード
// @param[in] from_node 含意元のノード
// @param[out] imp_list 含意の結果を格納するリスト
// @retval true 矛盾なく含意が行われた．
// @retval false 矛盾が発生した．
bool
ImpMgr::bwd_prop1(StrNode* node,
		  StrNode* from_node,
		  vector<ImpVal>& imp_list)
{
  const vector<StrEdge*>& fo_list = node->fanout_list();
  for (vector<StrEdge*>::const_iterator p = fo_list.begin();
       p != fo_list.end(); ++ p) {
    StrEdge* e = *p;
    StrNode* dst_node = e->dst_node();
    if ( dst_node == from_node ) continue;
    bool stat;
    if ( e->src_inv() ) {
      if ( e->dst_pos() == 0 ) {
	stat = dst_node->fwd0_imp0(*this, imp_list);
      }
      else {
	stat = dst_node->fwd1_imp0(*this, imp_list);
      }
    }
    else {
      if ( e->dst_pos() == 0 ) {
	stat = dst_node->fwd0_imp1(*this, imp_list);
      }
      else {
	stat = dst_node->fwd1_imp1(*this, imp_list);
      }
    }
    if ( !stat ) {
      return stat;
    }
  }
  if ( node->id() != mSrcId && node->val() == kB3X ) {
    imp_list.push_back(ImpVal(node->id(), 1));
  }
  return node->bwd_imp1(*this, imp_list);
}

// @brief unjustified ノードを得る．
void
ImpMgr::get_unodelist(vector<StrNode*>& unode_list)
{
  unode_list.clear();
  unode_list.reserve(mUnodeList.size());
  unode_list.insert(unode_list.begin(), mUnodeList.begin(), mUnodeList.end());
  ymuint n = mNodeArray.size();
  vector<bool> umark(n, false);
  ymuint c = 0;
  for (ymuint i = 0; i < n; ++ i) {
    StrNode* node = mNodeArray[i];
    if ( node == NULL ) continue;
    if ( node->is_unjustified() ) {
      umark[i] = true;
      ++ c;
    }
  }
  bool error = false;
  if ( c != mUnodeList.size() ) {
    error = true;
  }
  else {
    for (vector<StrNode*>::iterator p = unode_list.begin();
	 p != unode_list.end(); ++ p) {
      StrNode* node = *p;
      if ( !node->is_unjustified() ) {
	error = true;
	break;
      }
    }
  }
  if ( error ) {
    cout << "Error in ImpMgr::get_unode_list()" << endl;
    cout << "mUnodeList";
    for (list<StrNode*>::iterator p = mUnodeList.begin();
	 p != mUnodeList.end(); ++ p) {
      cout << " " << (*p)->id();
    }
    cout << endl
	 << "unjustified node";
    for (ymuint i = 0; i < n; ++ i) {
      if ( umark[i] ) {
	cout << " " << i;
      }
    }
    cout << endl
	 << endl;
  }
}

// @brief ノードが unjustified になったときの処理を行なう．
void
ImpMgr::set_unjustified(StrNode* node)
{
  assert_cond( node->mListIter == mUnodeList.end(), __FILE__, __LINE__);
  mUnodeList.push_back(node);
  node->mListIter = mUnodeList.end();
  -- node->mListIter;
}

// @brief ノードが unjustified でなくなったときの処理を行なう．
void
ImpMgr::reset_unjustified(StrNode* node)
{
  mUnodeList.erase(node->mListIter);
  node->mListIter = mUnodeList.end();
}

// @brief ノードの値をスタックに積む．
// @param[in] node ノード
// @param[in] old_state 変更前の値
void
ImpMgr::save_value(StrNode* node,
		   ymuint32 old_state)
{
  ymuint cur_level = mMarkerStack.size();
  if ( true || node->mStackLevel < cur_level ) {
    mChgStack.push_back(NodeChg(node, node->cur_state()));
    node->mStackLevel = cur_level;
  }
}

// @brief 内容を書き出す．
void
ImpMgr::print_network(ostream& s) const
{
  ymuint n = mNodeArray.size();
  for (ymuint i = 0; i < n; ++ i) {
    StrNode* node = mNodeArray[i];
    if ( node == NULL ) continue;
    cout << "Node#" << node->id() << ":";
    if ( node->is_input() ) {
      cout << "INPUT";
    }
    else if ( node->is_and() ) {
      cout << "AND";
    }
    else if ( node->is_xor() ) {
      cout << "XOR";
    }
    cout << endl;

    if ( node->is_and() || node->is_xor() ) {
      const StrEdge& e0 = node->fanin0();
      cout << "  Fanin0: " << e0.src_node()->id();
      if ( e0.src_inv() ) {
	cout << "~";
      }
      cout << endl;
      const StrEdge& e1 = node->fanin1();
      cout << "  Fanin1: " << e1.src_node()->id();
      if ( e1.src_inv() ) {
	cout << "~";
      }
      cout << endl;
    }

    cout << "  Fanouts: ";
    const vector<StrEdge*>& fo_list = node->fanout_list();
    for (vector<StrEdge*>::const_iterator p = fo_list.begin();
	 p != fo_list.end(); ++ p ) {
      StrEdge* e = *p;
      cout << " (" << e->dst_node()->id() << ", " << e->dst_pos() << ")";
    }
    cout << endl;
  }
}

// @brief ランダムシミュレーションを行なう．
void
ImpMgr::random_sim()
{
  for (vector<StrNode*>::iterator p = mInputArray.begin();
       p != mInputArray.end(); ++ p) {
    StrNode* node = *p;
    ymuint64 val0 = mRandGen.int32();
    ymuint64 val1 = mRandGen.int32();
    ymuint64 bitval = (val0 << 32) | val1;
    node->set_bitval(bitval);
  }

  for (vector<StrNode*>::iterator p = mNodeList.begin();
       p != mNodeList.end(); ++ p) {
    StrNode* node = *p;
    node->calc_bitval();
  }
}

END_NAMESPACE_YM_NETWORKS

