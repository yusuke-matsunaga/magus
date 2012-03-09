
/// @file ImpMgr.cc
/// @brief ImpMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ImpMgr.h"
#include "ImpInput.h"
#include "ImpAnd.h"


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
  for (vector<ImpNode*>::iterator p = mNodeArray.begin();
       p != mNodeArray.end(); ++ p) {
    delete *p;
  }
  mInputArray.clear();
  mNodeList.clear();
  mNodeArray.clear();
  mNodeMap.clear();
  mChgStack.clear();
}

BEGIN_NONAMESPACE

ymuint lnum;
ymuint gnum;

void
mark_tfi(const BdnNode* node,
	 vector<bool>& mark,
	 vector<bool>& gmark,
	 vector<const BdnNode*>& marked_inputs)
{
  if ( mark[node->id()] ) {
    return;
  }
  mark[node->id()] = true;
  ++ lnum;

  if ( !gmark[node->id()] ) {
    gmark[node->id()] = true;
    ++ gnum;
  }

  if ( node->is_input() ) {
    marked_inputs.push_back(node);
  }
  else if ( node->is_logic() ) {
    mark_tfi(node->fanin0(), mark, gmark, marked_inputs);
    mark_tfi(node->fanin1(), mark, gmark, marked_inputs);
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }
}

void
mark_tfi2(const BdnNode* node,
	  vector<bool>& mark,
	  vector<const BdnNode*>& marked_inputs)
{
  if ( mark[node->id()] ) {
    return;
  }
  mark[node->id()] = true;

  if ( node->is_input() ) {
    marked_inputs.push_back(node);
  }
  else if ( node->is_logic() ) {
    ++ lnum;
    mark_tfi2(node->fanin0(), mark, marked_inputs);
    mark_tfi2(node->fanin1(), mark, marked_inputs);
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }
}

void
mark_tfo(const BdnNode* node,
	 vector<bool>& mark,
	 vector<const BdnNode*>& marked_outputs)
{
  if ( mark[node->id()] ) {
    return;
  }
  mark[node->id()] = true;

  if ( node->is_output() ) {
    marked_outputs.push_back(node);
  }
  else {
    const BdnFanoutList& fo_list = node->fanout_list();
    for (BdnFanoutList::const_iterator p = fo_list.begin();
	 p != fo_list.end(); ++ p) {
      const BdnEdge* e = *p;
      const BdnNode* onode = e->to();
      mark_tfo(onode, mark, marked_outputs);
    }
  }
}

END_NONAMESPACE

// @brief ネットワークを設定する．
// @param[in] src_network 元となるネットワーク
void
ImpMgr::set(const BNetwork& src_network)
{
  clear();

  ymuint n = src_network.max_node_id();

  // 配列を確保する．
  mNodeMap.clear();
  mNodeMap.resize(n);

  // node_list に src_network のノードをトポロジカル順に並べる．
  BNodeVector node_list;
  src_network.tsort(node_list);

  // 各ノードのファンアウト数を数える．
  vector<ymuint> fo_count(n, 0);
  for (BNodeVector::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    const BNode* bnode = *p;
    ymuint ni = bnode->ni();
    for (ymuint i = 0; i < ni; ++ i) {
      BNode* inode = bnode->fanin(i);
      ++ fo_count[inode->id()];
    }
  }

  // 外部入力ノードを作る．
  const BNodeList& input_list = src_network.inputs();
  mInputArray.reserve(input_list.size());
  for (BNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const BNode* bnode = *p;
    ymuint id = bnode->id();
    ImpNode* node = new_input(id);
    node->mListIter = mUnodeList.end();
    node->set_nfo(fo_count[id]);
    mInputArray.push_back(node);
  }

  // 論理ノードを作る．
  for (BNodeVector::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    const BNode* bnode = *p;
    ymuint id = bnode->id();
    ymuint ni = bnode->ni();
    vector<ImpNodeHandle> fanins(ni);
    for (ymuint i = 0; i < ni; ++ i) {
      const BNode* ibnode = bnode->fanin(i);
      ImpNodeHandle ihandle = mNodeMap[ibnode->id()];
      fanins[i] = ihandle;
    }
    ImpNodeHandle handle = make_tree(bnode->func(), fanins);
    ImpNode* node = handle.node();
    node->mListIter = mUnodeList.end();
    node->set_nfo(fo_count[id]);
    set_bnode_id(node, handle.inv(), id);
  }
}

// @brief 論理式に対応したノードの木を作る．
ImpNodeHandle
ImpMgr::make_tree(const LogExpr& expr,
		  const vector<ImpNodeHandle>& fanins)
{
  if ( expr.is_zero() ) {
    return ImpNodeHandle::make_zero();
  }
  if ( expr.is_one() ) {
    return ImpNodeHandle::make_one();
  }
  if ( expr.is_posiliteral() ) {
    VarId vid = expr.varid();
    return fanins[vid.val()];
  }
  if ( expr.is_negaliteral() ) {
    VarId vid = expr.varid();
    return ~fanins[vid.val()];
  }

  ymuint nc = expr.child_num();
  vector<ImpNodeHandle> child_array(nc);
  bool child_inv = expr.is_or();
  for (ymuint i = 0; i < nc; ++ i) {
    child_array[i] = make_tree(expr.child(i), fanins);
    if ( child_inv ) {
      child_array[i] = ~child_array[i];
    }
  }

  if ( expr.is_and() ) {
    return make_and(child_array, 0, nc);
  }
  if ( expr.is_or() ) {
    return ~make_and(child_array, 0, nc);
  }
  if ( expr.is_xor() ) {
    return make_xor(child_array, 0, nc);
  }
  assert_not_reached(__FILE__, __LINE__);
  return ImpNodeHandle::make_zero();
}

// @brief AND ノードの木を作る．
// @param[in] fanins 葉のノードの配列
// @param[in] begin 開始位置
// @param[in] end 終了位置
ImpNodeHandle
ImpMgr::make_and(const vector<ImpNodeHandle>& fanins,
		 ymuint begin,
		 ymuint end)
{
  ymuint n = end - begin;
  assert_cond( n > 0, __FILE__, __LINE__);

  if ( n == 1 ) {
    return fanins[begin];
  }

  ymuint h = n / 2;
  ImpNodeHandle l = make_and(fanins, begin, begin + h);
  ImpNodeHandle r = make_and(fanins, begin + h, end);
  ImpNode* lnode = l.node();
  lnode->set_nfo(1);
  ImpNode* rnode = r.node();
  rnode->set_nfo(1);
  ImpNode* node = new_and(l, r);
  return ImpNodeHandle(node, false);
}

// @brief XOR ノードの木を作る．
ImpNodeHandle
ImpMgr::make_xor(const vector<ImpNodeHandle>& fanins,
		 ymuint begin,
		 ymuint end)
{
  ymuint n = end - begin;
  assert_cond( n > 0, __FILE__, __LINE__);

  if ( n == 1 ) {
    return fanins[begin];
  }

  ymuint h = n / 2;
  ImpNodeHandle l = make_xor(fanins, begin, begin + h);
  ImpNodeHandle r = make_xor(fanins, begin + h, end);
  ImpNode* lnode = l.node();
  lnode->set_nfo(2);
  ImpNode* rnode = r.node();
  rnode->set_nfo(2);
  ImpNode* node1 = new_and(l, ~r);
  node1->set_nfo(1);
  ImpNode* node2 = new_and(~l, r);
  node2->set_nfo(1);
  ImpNode* node = new_and(ImpNodeHandle(node1, true),
			  ImpNodeHandle(node2, true));
  return ImpNodeHandle(node, true);
}

// @brief 入力ノードを作る．
// @param[in] bnode_id BNode-ID
ImpNode*
ImpMgr::new_input(ymuint bnode_id)
{
  ImpNode* node = new ImpInput();
  reg_node(node);
  set_bnode_id(node, false, bnode_id);
  return node;
}

// @brief ANDノードを作る．
// @param[in] handle0 ファンイン0のハンドル
// @param[in] handle1 ファンイン1のハンドル
ImpNode*
ImpMgr::new_and(ImpNodeHandle handle0,
		ImpNodeHandle handle1)
{
  ImpNode* node = new ImpAnd(handle0, handle1);
  reg_node(node);
  mNodeList.push_back(node);
  return node;
}

// @brief ノードに BNode-ID を割り当てる．
void
ImpMgr::set_bnode_id(ImpNode* node,
		     bool inv,
		     ymuint bnode_id)
{
  node->mBNodeId = (bnode_id << 1) | static_cast<ymuint>(inv);
  mNodeMap[bnode_id] = ImpNodeHandle(node, inv);
}

// @brief ノードを登録する．
void
ImpMgr::reg_node(ImpNode* node)
{
  node->mId = mNodeArray.size();
  mNodeArray.push_back(node);
}

// @brief ノードに値を設定し含意操作を行う．
// @param[in] node ノード
// @param[in] val 設定する値 ( 0 or 1 )
// @param[out] imp_list 含意の結果を格納するリスト
bool
ImpMgr::assert(ImpNode* node,
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
    ImpNode* node = nc.mNode;
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
ImpMgr::fwd_prop0(ImpNode* node,
		  vector<ImpVal>& imp_list)
{
  if ( node->id() != mSrcId ) {
    imp_list.push_back(ImpVal(node->id(), 0));
  }
  const vector<ImpEdge*>& fo_list = node->fanout_list();
  for (vector<ImpEdge*>::const_iterator p = fo_list.begin();
       p != fo_list.end(); ++ p) {
    ImpEdge* e = *p;
    ImpNode* node = e->dst_node();
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
ImpMgr::fwd_prop1(ImpNode* node,
		  vector<ImpVal>& imp_list)
{
  if ( node->id() != mSrcId ) {
    imp_list.push_back(ImpVal(node->id(), 1));
  }
  const vector<ImpEdge*>& fo_list = node->fanout_list();
  for (vector<ImpEdge*>::const_iterator p = fo_list.begin();
       p != fo_list.end(); ++ p) {
    ImpEdge* e = *p;
    ImpNode* node = e->dst_node();
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
ImpMgr::fanin0_prop0(ImpNode* node,
		     vector<ImpVal>& imp_list)
{
  const ImpEdge& e = node->fanin0();
  ImpNode* dst_node = e.src_node();
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
ImpMgr::fanin0_prop1(ImpNode* node,
		     vector<ImpVal>& imp_list)
{
  const ImpEdge& e = node->fanin0();
  ImpNode* dst_node = e.src_node();
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
ImpMgr::fanin1_prop0(ImpNode* node,
		     vector<ImpVal>& imp_list)
{
  const ImpEdge& e = node->fanin1();
  ImpNode* dst_node = e.src_node();
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
ImpMgr::fanin1_prop1(ImpNode* node,
		     vector<ImpVal>& imp_list)
{
  const ImpEdge& e = node->fanin1();
  ImpNode* dst_node = e.src_node();
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
ImpMgr::bwd_prop0(ImpNode* node,
		  ImpNode* from_node,
		  vector<ImpVal>& imp_list)
{
  const vector<ImpEdge*>& fo_list = node->fanout_list();
  for (vector<ImpEdge*>::const_iterator p = fo_list.begin();
       p != fo_list.end(); ++ p) {
    ImpEdge* e = *p;
    ImpNode* dst_node = e->dst_node();
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
ImpMgr::bwd_prop1(ImpNode* node,
		  ImpNode* from_node,
		  vector<ImpVal>& imp_list)
{
  const vector<ImpEdge*>& fo_list = node->fanout_list();
  for (vector<ImpEdge*>::const_iterator p = fo_list.begin();
       p != fo_list.end(); ++ p) {
    ImpEdge* e = *p;
    ImpNode* dst_node = e->dst_node();
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
ImpMgr::get_unodelist(vector<ImpNode*>& unode_list)
{
  unode_list.clear();
  unode_list.reserve(mUnodeList.size());
  unode_list.insert(unode_list.begin(), mUnodeList.begin(), mUnodeList.end());
  ymuint n = mNodeArray.size();
  vector<bool> umark(n, false);
  ymuint c = 0;
  for (ymuint i = 0; i < n; ++ i) {
    ImpNode* node = mNodeArray[i];
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
    for (vector<ImpNode*>::iterator p = unode_list.begin();
	 p != unode_list.end(); ++ p) {
      ImpNode* node = *p;
      if ( !node->is_unjustified() ) {
	error = true;
	break;
      }
    }
  }
  if ( error ) {
    cout << "Error in ImpMgr::get_unode_list()" << endl;
    cout << "mUnodeList";
    for (list<ImpNode*>::iterator p = mUnodeList.begin();
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
ImpMgr::set_unjustified(ImpNode* node)
{
  assert_cond( node->mListIter == mUnodeList.end(), __FILE__, __LINE__);
  mUnodeList.push_back(node);
  node->mListIter = mUnodeList.end();
  -- node->mListIter;
}

// @brief ノードが unjustified でなくなったときの処理を行なう．
void
ImpMgr::reset_unjustified(ImpNode* node)
{
  mUnodeList.erase(node->mListIter);
  node->mListIter = mUnodeList.end();
}

// @brief ノードの値をスタックに積む．
// @param[in] node ノード
// @param[in] old_state 変更前の値
void
ImpMgr::save_value(ImpNode* node,
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
    ImpNode* node = mNodeArray[i];
    if ( node == NULL ) continue;
    cout << "Node#" << node->id() << ":";
    if ( node->is_input() ) {
      cout << "INPUT";
    }
    else if ( node->is_and() ) {
      cout << "AND";
    }
    cout << endl;

    if ( node->is_and() ) {
      const ImpEdge& e0 = node->fanin0();
      cout << "  Fanin0: " << e0.src_node()->id();
      if ( e0.src_inv() ) {
	cout << "~";
      }
      cout << endl;
      const ImpEdge& e1 = node->fanin1();
      cout << "  Fanin1: " << e1.src_node()->id();
      if ( e1.src_inv() ) {
	cout << "~";
      }
      cout << endl;
    }

    cout << "  Fanouts: ";
    const vector<ImpEdge*>& fo_list = node->fanout_list();
    for (vector<ImpEdge*>::const_iterator p = fo_list.begin();
	 p != fo_list.end(); ++ p ) {
      ImpEdge* e = *p;
      cout << " (" << e->dst_node()->id() << ", " << e->dst_pos() << ")";
    }
    cout << endl;
  }
}

// @brief ランダムシミュレーションを行なう．
void
ImpMgr::random_sim()
{
  for (vector<ImpNode*>::iterator p = mInputArray.begin();
       p != mInputArray.end(); ++ p) {
    ImpNode* node = *p;
    ymuint64 val0 = mRandGen.int32();
    ymuint64 val1 = mRandGen.int32();
    ymuint64 bitval = (val0 << 32) | val1;
    node->set_bitval(bitval);
  }

  for (vector<ImpNode*>::iterator p = mNodeList.begin();
       p != mNodeList.end(); ++ p) {
    ImpNode* node = *p;
    node->calc_bitval();
  }
}

END_NAMESPACE_YM_NETWORKS

