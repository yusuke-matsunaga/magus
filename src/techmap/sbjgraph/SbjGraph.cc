﻿
/// @file libym_techmap/SbjGraph.cc
/// @brief SbjGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SbjGraph.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SbjGraph.h"
#include "SbjMinDepth.h"


BEGIN_NAMESPACE_YM_SBJ

///////////////////////////////////////////////////////////////////////
// クラス SbjNode
///////////////////////////////////////////////////////////////////////

// コンストラクタ
SbjNode::SbjNode() :
  mId(0),
  mFlags(0U),
  mFanins(NULL),
  mMark(0)
{
}

// デストラクタ
SbjNode::~SbjNode()
{
}

// @brief ファンアウトに出力が含まれているか調べ pomark をセットする．
void
SbjNode::scan_po()
{
  mMark &= ~kPoMask;
  for (SbjEdgeList::iterator p = mFanoutList.begin();
       p != mFanoutList.end(); ++ p) {
    SbjEdge* e = *p;
    SbjNode* to = e->to();
    if ( to->is_ppo() ) {
      mMark |= kPoMask;
      break;
    }
  }
}

// @brief ID を表す文字列の取得
string
SbjNode::id_str() const
{
  ostringstream buf;
  if ( is_input() ) {
    buf << "I" << id();
  }
  else if ( is_output() ) {
    buf << "O" << id();
  }
  else if ( is_logic() ) {
    buf << "L" << id();
  }
  else if ( is_dff() ) {
    buf << "D" << id();
  }
  else {
    buf << "X" << id();
  }
  return buf.str();
}


//////////////////////////////////////////////////////////////////////
// クラス SbjPort
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] body 入出力ノードのベクタ
SbjPort::SbjPort(const string& name,
		 const vector<SbjNode*>& body) :
  mName(name),
  mBody(body)
{
}

// @brief デストラクタ
SbjPort::~SbjPort()
{
}



///////////////////////////////////////////////////////////////////////
// クラス SbjGraph
///////////////////////////////////////////////////////////////////////

// コンストラクタ
SbjGraph::SbjGraph() :
  mAlloc(4096),
  mAlloc2(4096),
  mLevel(0),
  mLevelValid(false)
{
}

// コピーコンストラクタ
SbjGraph::SbjGraph(const SbjGraph& src) :
  mAlloc(4096),
  mAlloc2(4096)
{
  vector<SbjNode*> nodemap;
  copy(src, nodemap);
}

// 代入演算子
const SbjGraph&
SbjGraph::operator=(const SbjGraph& src)
{
  if ( &src != this ) {
    clear();
    vector<SbjNode*> nodemap;
    copy(src, nodemap);
  }
  return *this;
}

// デストラクタ
SbjGraph::~SbjGraph()
{
  clear();
}

// 複製する．
void
SbjGraph::copy(const SbjGraph& src,
	       vector<SbjNode*>& nodemap)
{
  ymuint n = src.max_node_id();
  nodemap.clear();
  nodemap.resize(n);

  // 名前のコピー
  mName = src.mName;

  // 外部入力の生成
  const SbjNodeList& input_list = src.input_list();
  for (SbjNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    SbjNode* src_node = *p;
    SbjNode* dst_node = new_input();
    nodemap[src_node->id()] = dst_node;
  }

  // DFF の生成
  const SbjNodeList& dff_list = src.dff_list();
  for (SbjNodeList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    SbjNode* src_node = *p;
    SbjNode* dst_node = new_dff();
    nodemap[src_node->id()] = dst_node;
  }

  // 論理ノードの生成
  vector<const SbjNode*> node_list;
  src.sort(node_list);
  ymuint nl = node_list.size();
  for (ymuint i = 0; i < nl; ++ i) {
    const SbjNode* src_node = node_list[i];

    const SbjNode* src_inode0 = src_node->fanin(0);
    SbjNode* input0 = nodemap[src_inode0->id()];
    ASSERT_COND(input0 );

    const SbjNode* src_inode1 = src_node->fanin(1);
    SbjNode* input1 = nodemap[src_inode1->id()];
    ASSERT_COND(input1 );

    SbjNode* dst_node = new_logic(src_node->fcode(), input0, input1);
    dst_node->mMark = src_node->mMark;
    dst_node->mLevel = src_node->mLevel;
    nodemap[src_node->id()] = dst_node;
  }

  // DFF の入力の接続
  for (SbjNodeList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const SbjNode* src_onode = *p;
    SbjNode* dst_onode = nodemap[src_onode->id()];

    const SbjNode* src_inode0 = src_onode->fanin_data();
    SbjNode* dst_inode0 = nodemap[src_inode0->id()];
    set_dff_data(dst_onode, SbjHandle(dst_inode0, src_onode->fanin_data_inv()));

    const SbjNode* src_inode1 = src_onode->fanin_clock();
    if ( src_inode1 ) {
      SbjNode* dst_inode1 = nodemap[src_inode1->id()];
      set_dff_clock(dst_onode,
		    SbjHandle(dst_inode1, src_onode->fanin_clock_inv()));
    }

    const SbjNode* src_inode2 = src_onode->fanin_set();
    if ( src_inode2 ) {
      SbjNode* dst_inode2 = nodemap[src_inode2->id()];
      set_dff_set(dst_onode, SbjHandle(dst_inode2, src_onode->fanin_set_inv()));
    }

    const SbjNode* src_inode3 = src_onode->fanin_rst();
    if ( src_inode3 ) {
      SbjNode* dst_inode3 = nodemap[src_inode3->id()];
      set_dff_rst(dst_onode, SbjHandle(dst_inode3, src_onode->fanin_rst_inv()));
    }
  }

  // 外部出力の生成
  const SbjNodeList& output_list = src.output_list();
  for (SbjNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const SbjNode* src_onode = *p;
    const SbjNode* src_inode = src_onode->fanin(0);
    SbjNode* dst_inode = NULL;
    if ( src_inode ) {
      dst_inode = nodemap[src_inode->id()];
    }
    SbjNode* dst_node = new_output(SbjHandle(dst_inode, src_onode->output_inv()));
    nodemap[src_onode->id()] = dst_node;
  }

  // ポートの複製
  ymuint np = src.port_num();
  for (ymuint i = 0; i < np; ++ i) {
    const SbjPort* src_port = src.port(i);
    ymuint nb = src_port->bit_width();
    vector<SbjNode*> tmp(nb);
    for (ymuint j = 0; j < nb; ++ j) {
      const SbjNode* src_node = src_port->bit(j);
      SbjNode* dst_node = nodemap[src_node->id()];
      tmp[j] = dst_node;
    }
    add_port(src_port->name(), tmp);
  }

  mLevel = src.mLevel;
  mLevelValid = src.mLevelValid;
}

// 空にする．
void
SbjGraph::clear()
{
  // ポートをクリアする．
  for (vector<SbjPort*>::iterator p = mPortArray.begin();
       p != mPortArray.end(); ++ p) {
    delete *p;
  }
  mPortArray.clear();

  // まず最初に接続を切る．
  for (SbjNodeList::iterator p = mOutputList.begin();
       p != mOutputList.end(); ++ p) {
    SbjNode* node = *p;
    connect(NULL, node, 0);
  }
  for (SbjNodeList::iterator p = mDffList.begin();
       p != mDffList.end(); ++ p) {
    SbjNode* node = *p;
    connect(NULL, node, 0);
    connect(NULL, node, 1);
    connect(NULL, node, 2);
    connect(NULL, node, 3);
  }
  for (SbjNodeList::iterator p = mLnodeList.begin();
       p != mLnodeList.end(); ++ p) {
    SbjNode* node = *p;
    connect(NULL, node, 0);
    connect(NULL, node, 1);
  }

  for (SbjNodeList::iterator p = mInputList.begin();
       p != mInputList.end(); ) {
    // p は実際にはノード内のメンバをアクセスするので delete する前に
    // 進めておく必要がある．
    SbjNode* node = *p;
    ++ p;
    delete_input(node);
  }
  ASSERT_COND(mInputList.empty() );

  for (SbjNodeList::iterator p = mOutputList.begin();
       p != mOutputList.end(); ) {
    // p は実際にはノード内のメンバをアクセスするので delete する前に
    // 進めておく必要がある．
    SbjNode* node = *p;
    ++ p;
    delete_output(node);
  }
  ASSERT_COND(mOutputList.empty() );

  for (SbjNodeList::iterator p = mLnodeList.begin();
       p != mLnodeList.end(); ) {
    // p は実際にはノード内のメンバをアクセスするので delete する前に
    // 進めておく必要がある．
    SbjNode* node = *p;
    ++ p;
    delete_logic(node);
  }
  ASSERT_COND(mLnodeList.empty() );

  for (SbjNodeList::iterator p = mDffList.begin();
       p != mDffList.end(); ) {
    // p は実際にはノード内のメンバをアクセスするので delete する前に
    // 進めておく必要がある．
    SbjNode* node = *p;
    ++ p;
    delete_dff(node);
  }
  ASSERT_COND(mDffList.empty() );

  mInputArray.clear();
  mInputPortArray.clear();
  mOutputArray.clear();
  mOutputPortArray.clear();
}

// @brief ポートを追加する(ベクタ版)．
// @param[in] name ポート名
// @param[in] body 対応付ける入出力ノードのベクタ
void
SbjGraph::add_port(const string& name,
		   const vector<SbjNode*>& body)
{
  SbjPort* port = new SbjPort(name, body);
  mPortArray.push_back(port);
  ymuint n = body.size();
  for (ymuint i = 0; i < n; ++ i) {
    SbjNode* node = body[i];
    if ( node->is_input() ) {
      PortInfo& port_info = mInputPortArray[node->subid()];
      port_info.mPort = port;
      port_info.mPos = i;
    }
    else if ( node->is_output() ) {
      PortInfo& port_info = mOutputPortArray[node->subid()];
      port_info.mPort = port;
      port_info.mPos = i;
    }
    else {
      ASSERT_NOT_REACHED;
    }
  }
}

// @brief 入出力ノードに関連づけられたポートを得る．
// @param[in] node 入出力ノード
const SbjPort*
SbjGraph::port(const SbjNode* node) const
{
  if ( node->is_input() ) {
    return mInputPortArray[node->subid()].mPort;
  }
  else if ( node->is_output() ) {
    return mOutputPortArray[node->subid()].mPort;
  }
  else {
    return NULL;
  }
}

// @brief 入出力ノードのポートにおけるビット位置を得る．
// @param[in] node 入出力ノード
ymuint
SbjGraph::port_pos(const SbjNode* node) const
{
  if ( node->is_input() ) {
    return mInputPortArray[node->subid()].mPos;
  }
  else if ( node->is_output() ) {
    return mOutputPortArray[node->subid()].mPos;
  }
  else {
    return 0;
  }
}

// @brief 入力ノードと DFF ノードのリストを返す．
// @param[out] node_list ノードを格納するリスト
// @return 要素数を返す．
ymuint
SbjGraph::ppi_list(vector<const SbjNode*>& node_list) const
{
  node_list.clear();
  node_list.reserve(input_num() + dff_num());
  for (SbjNodeList::const_iterator p = input_list().begin();
       p != input_list().end(); ++ p) {
    node_list.push_back(*p);
  }
  for (SbjNodeList::const_iterator p = dff_list().begin();
       p != dff_list().end(); ++ p) {
    node_list.push_back(*p);
  }
  return node_list.size();
}

// @brief 出力ノードと DFF ノードのリストを返す．
// @param[out] node_list ノードを格納するリスト
// @return 要素数を返す．
ymuint
SbjGraph::ppo_list(vector<const SbjNode*>& node_list) const
{
  node_list.clear();
  node_list.reserve(output_num() + dff_num());
  for (SbjNodeList::const_iterator p = output_list().begin();
       p != output_list().end(); ++ p) {
    node_list.push_back(*p);
  }
  for (SbjNodeList::const_iterator p = dff_list().begin();
       p != dff_list().end(); ++ p) {
    node_list.push_back(*p);
  }
  return node_list.size();
}


BEGIN_NONAMESPACE

// node のファンアウトのノードのうち，ファンインがすべてマークされている
// ノード を node_list に積む．
void
sort_sub(const SbjNode* node,
	 vector<bool>& mark,
	 vector<const SbjNode*>& node_list)
{
  const SbjEdgeList& fo_list = node->fanout_list();
  for (SbjEdgeList::const_iterator p = fo_list.begin();
       p != fo_list.end(); ++ p) {
    const SbjEdge* e = *p;
    const SbjNode* onode = e->to();
    if ( mark[onode->id()] || !onode->is_logic() ) continue;
    bool ready = true;
    for (ymuint i = 0; i < 2; ++ i) {
      const SbjNode* inode0 = onode->fanin(i);
      if ( !mark[inode0->id()] ) {
	ready = false;
	break;
      }
    }
    if ( ready ) {
      mark[onode->id()] = true;
      node_list.push_back(onode);
    }
  }
}

END_NONAMESPACE

// @brief ソートされた論理ノードのリストを得る．
void
SbjGraph::sort(vector<const SbjNode*>& node_list) const
{
  node_list.clear();
  node_list.reserve(lnode_num());

  vector<bool> mark(max_node_id(), false);

  // 外部入力とDFFのみをファンインにするノードを node_list に追加する．
  vector<const SbjNode*> tmp_list;
  ppi_list(tmp_list);
  for (vector<const SbjNode*>::const_iterator p = tmp_list.begin();
       p != tmp_list.end(); ++ p) {
    const SbjNode* node = *p;
    mark[node->id()] = true;
    sort_sub(node, mark, node_list);
  }

  ymuint rpos = 0;
  while ( rpos < node_list.size() ) {
    const SbjNode* node = node_list[rpos];
    ++ rpos;
    sort_sub(node, mark, node_list);
  }
  ASSERT_COND(node_list.size() == lnode_num() );
}


BEGIN_NONAMESPACE

// node のファンアウトのノードのうち，ファンインがすべてマークされている
// ノード を node_list に積む．
void
rsort_sub(const SbjNode* node,
	  vector<bool>& mark,
	  vector<const SbjNode*>& node_list)
{
  ymuint ni = node->is_logic() ? 2 : 1;
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* inode = node->fanin(i);
    if ( inode == NULL || mark[inode->id()] || !inode->is_logic() ) continue;
    const SbjEdgeList& fo_list = inode->fanout_list();
    bool ready = true;
    for (SbjEdgeList::const_iterator p = fo_list.begin();
	 p != fo_list.end(); ++ p) {
      const SbjEdge* e = *p;
      const SbjNode* onode = e->to();
      if ( !mark[onode->id()] ) {
	ready = false;
	break;
      }
    }
    if ( ready ) {
      mark[inode->id()] = true;
      node_list.push_back(inode);
    }
  }
}

END_NONAMESPACE

// @brief 逆順でソートされたノードのリストを得る．
void
SbjGraph::rsort(vector<const SbjNode*>& node_list) const
{
  node_list.clear();
  node_list.reserve(lnode_num());

  vector<bool> mark(max_node_id(), false);

  // 外部出力とDFFのみをファンアウトにするノードを node_list に追加する．
  vector<const SbjNode*> tmp_list;
  ppo_list(tmp_list);
  for (vector<const SbjNode*>::const_iterator p = tmp_list.begin();
       p != tmp_list.end(); ++ p) {
    const SbjNode* node = *p;
    mark[node->id()] = true;
    rsort_sub(node, mark, node_list);
  }

  // node_list からノードを取り出し，同様の処理を行う．
  ymuint rpos = 0;
  while ( rpos < node_list.size() ) {
    const SbjNode* node = node_list[rpos];
    ++ rpos;
    rsort_sub(node, mark, node_list);
  }
  ASSERT_COND(node_list.size() == lnode_num() );
}

// 入力ノードを作る．
SbjNode*
SbjGraph::new_input()
{
  SbjNode* node = new_node(0);

  // 入力ノード配列に登録
  ymuint subid = mInputArray.size();
  mInputArray.push_back(node);
  // ダミーの place-holder を追加
  mInputPortArray.push_back(PortInfo());

  // 入力リストに登録
  mInputList.push_back(node);

  node->set_input(subid);

  node->mLevel = 0;

  return node;
}

// 出力ノードを作る．
SbjNode*
SbjGraph::new_output(SbjHandle ihandle)
{
  SbjNode* node = new_node(1);

  // 出力ノード配列に登録
  ymuint subid = mOutputArray.size();
  mOutputArray.push_back(node);
  // ダミーの place-holder を追加
  mOutputPortArray.push_back(PortInfo());

  // 出力リストに登録
  mOutputList.push_back(node);

  SbjNode* inode = ihandle.node();
  bool inv = ihandle.inv();
  node->set_output(subid, inv);
  connect(inode, node, 0);

  return node;
}

// @brief 論理ノードを作る．
// @param[in] fcode 機能コード
// @param[in] inode1 1番めの入力ノード
// @param[in] inode2 2番めの入力ノード
// @return 作成したノードを返す．
SbjNode*
SbjGraph::new_logic(ymuint fcode,
		    SbjNode* inode0,
		    SbjNode* inode1)
{
  SbjNode* node = new_node(2);

  // 論理ノードリストに登録
  mLnodeList.push_back(node);

  node->set_logic(fcode);
  connect(inode0, node, 0);
  connect(inode1, node, 1);

  return node;
}

// @brief ANDノードを作る．
// @param[in] ihandle1 1番めの入力ハンドル
// @param[in] ihandle2 2番めの入力ハンドル
// @return 作成したノードのハンドルを返す．
// @note ihandle1/ihandle2 が定数の時も考慮している．
SbjHandle
SbjGraph::new_and(SbjHandle ihandle1,
		  SbjHandle ihandle2)
{
  if ( ihandle1.is_const0() ) {
    // 入力1が0固定
    return SbjHandle::make_zero();
  }
  if ( ihandle1.is_const1() ) {
    // 入力1が1固定
    return ihandle2;
  }
  if ( ihandle2.is_const0() ) {
    // 入力2が0固定
    return SbjHandle::make_zero();
  }
  if ( ihandle2.is_const1() ) {
    // 入力2が1固定
    return ihandle1;
  }

  ymuint fcode = 0U;
  if ( ihandle1.inv() ) {
    fcode |= 1U;
  }
  if ( ihandle2.inv() ) {
    fcode |= 2U;
  }
  SbjNode* sbjnode = new_logic(fcode, ihandle1.node(), ihandle2.node());
  return SbjHandle(sbjnode, false);
}

// @brief ANDノードを作る．
// @param[in] ihandle_list 入力ハンドルのリスト
// @return 作成したノードのハンドルを返す．
// @note 入力が定数の時も考慮している．
SbjHandle
SbjGraph::new_and(const vector<SbjHandle>& ihandle_list)
{
  ymuint n = ihandle_list.size();
  vector<SbjHandle> tmp_list;
  tmp_list.reserve(n);
  for (ymuint i = 0; i < n; ++ i) {
    SbjHandle h = ihandle_list[i];
    if ( h.is_const0() ) {
      return SbjHandle::make_zero();
    }
    else if ( !h.is_const1() ) {
      tmp_list.push_back(h);
    }
  }
  if ( tmp_list.empty() ) {
    return SbjHandle::make_one();
  }
  return _new_and(tmp_list, 0, tmp_list.size());
}

// @brief new_and の下請け関数
// @param[in] ihandle_list 入力ハンドルのリスト
// @param[in] start 開始位置
// @param[in] num 要素数
SbjHandle
SbjGraph::_new_and(const vector<SbjHandle>& ihandle_list,
		   ymuint start,
		   ymuint num)
{
  ASSERT_COND( num > 0 );
  if ( num == 1 ) {
    return ihandle_list[start];
  }

  ymuint h = num / 2;
  SbjHandle l = _new_and(ihandle_list, start, h);
  SbjHandle r = _new_and(ihandle_list, start + h, num - h);
  return new_and(l, r);
}

// @brief ORノードを作る．
// @param[in] ihandle1 1番めの入力ハンドル
// @param[in] ihandle2 2番めの入力ハンドル
// @return 作成したノードのハンドルを返す．
// @note ihandle1/ihandle2 が定数の時も考慮している．
SbjHandle
SbjGraph::new_or(SbjHandle ihandle1,
		 SbjHandle ihandle2)
{
  if ( ihandle1.is_const0() ) {
    // 入力1が0固定
    return ihandle2;
  }
  if ( ihandle1.is_const1() ) {
    // 入力1が1固定
    return SbjHandle::make_one();
  }
  if ( ihandle2.is_const0() ) {
    // 入力2が0固定
    return ihandle1;
  }
  if ( ihandle2.is_const1() ) {
    // 入力2が1固定
    return SbjHandle::make_one();
  }

  ymuint fcode = 0U;
  if ( !ihandle1.inv() ) {
    fcode |= 1U;
  }
  if ( !ihandle2.inv() ) {
    fcode |= 2U;
  }
  SbjNode* sbjnode = new_logic(fcode, ihandle1.node(), ihandle2.node());
  return SbjHandle(sbjnode, true);
}

// @brief ORノードを作る．
// @param[in] ihandle_list 入力ハンドルのリスト
// @return 作成したノードのハンドルを返す．
// @note 入力が定数の時も考慮している．
SbjHandle
SbjGraph::new_or(const vector<SbjHandle>& ihandle_list)
{
  ymuint n = ihandle_list.size();
  vector<SbjHandle> tmp_list;
  tmp_list.reserve(n);
  for (ymuint i = 0; i < n; ++ i) {
    SbjHandle h = ihandle_list[i];
    if ( h.is_const1() ) {
      return SbjHandle::make_one();
    }
    else if ( !h.is_const0() ) {
      tmp_list.push_back(h);
    }
  }
  if ( tmp_list.empty() ) {
    return SbjHandle::make_zero();
  }
  return _new_or(tmp_list, 0, tmp_list.size());
}

// @brief new_or の下請け関数
// @param[in] ihandle_list 入力ハンドルのリスト
// @param[in] start 開始位置
// @param[in] num 要素数
SbjHandle
SbjGraph::_new_or(const vector<SbjHandle>& ihandle_list,
		  ymuint start,
		  ymuint num)
{
  ASSERT_COND( num > 0 );
  if ( num == 1 ) {
    return ihandle_list[start];
  }

  ymuint h = num / 2;
  SbjHandle l = _new_or(ihandle_list, start, h);
  SbjHandle r = _new_or(ihandle_list, start + h, num - h);
  return new_or(l, r);
}

// @brief XORノードを作る．
// @param[in] ihandle1 1番めの入力ハンドル
// @param[in] ihandle2 2番めの入力ハンドル
// @return 作成したノードのハンドルを返す．
// @note ihandle1/ihandle2 が定数の時も考慮している．
SbjHandle
SbjGraph::new_xor(SbjHandle ihandle1,
		  SbjHandle ihandle2)
{
  if ( ihandle1.is_const0() ) {
    // 入力1が0固定
    return ihandle2;
  }
  if ( ihandle1.is_const1() ) {
    // 入力1が1固定
    return ~ihandle2;
  }
  if ( ihandle2.is_const0() ) {
    // 入力2が0固定
    return ihandle1;
  }
  if ( ihandle2.is_const1() ) {
    // 入力2が1固定
    return ~ihandle1;
  }

  bool inv = ihandle1.inv() ^ ihandle2.inv();
  SbjNode* node = new_logic(4U, ihandle1.node(), ihandle2.node());
  return SbjHandle(node, inv);
}

// @brief XORノードを作る．
// @param[in] ihandle_list 入力ハンドルのリスト
// @return 作成したノードのハンドルを返す．
// @note 入力が定数の時も考慮している．
SbjHandle
SbjGraph::new_xor(const vector<SbjHandle>& ihandle_list)
{
  ymuint n = ihandle_list.size();
  vector<SbjHandle> tmp_list;
  tmp_list.reserve(n);
  bool inv = false;
  for (ymuint i = 0; i < n; ++ i) {
    SbjHandle h = ihandle_list[i];
    if ( h.is_const1() ) {
      inv = !inv;
    }
    else if ( !h.is_const0() ) {
      tmp_list.push_back(h);
    }
  }
  if ( tmp_list.empty() ) {
    return SbjHandle::make_zero();
  }
  if ( inv ) {
    tmp_list[0].invert();
  }
  return _new_xor(tmp_list, 0, tmp_list.size());
}

// @brief new_xor の下請け関数
// @param[in] ihandle_list 入力ハンドルのリスト
// @param[in] start 開始位置
// @param[in] num 要素数
SbjHandle
SbjGraph::_new_xor(const vector<SbjHandle>& ihandle_list,
		   ymuint start,
		   ymuint num)
{
  ASSERT_COND( num > 0 );
  if ( num == 1 ) {
    return ihandle_list[start];
  }

  ymuint h = num / 2;
  SbjHandle l = _new_xor(ihandle_list, start, h);
  SbjHandle r = _new_xor(ihandle_list, start + h, num - h);
  return new_xor(l, r);
}

// DFFノードを作る．
SbjNode*
SbjGraph::new_dff(SbjNode* inode,
		  bool inv)
{
  SbjNode* node1 = new_node(4);

  // DFFリストに登録
  mDffList.push_back(node1);

  node1->set_dff(inv);
  connect(inode, node1, 0);

  return node1;
}

// 入力ノードの削除
void
SbjGraph::delete_input(SbjNode* node)
{
  ASSERT_COND(node->is_input() );
  mInputList.erase(node);
  delete_node(node, 0);
}

// 出力ノードの削除
void
SbjGraph::delete_output(SbjNode* node)
{
  ASSERT_COND(node->is_output() );
  mOutputList.erase(node);
  delete_node(node, 1);

  mLevel = 0;
  mLevelValid = false;
}

// 論理ノードの削除
void
SbjGraph::delete_logic(SbjNode* node)
{
  ASSERT_COND(node->is_logic() );
  ASSERT_COND(node->fanout_num() == 0 );
  connect(NULL, node, 0);
  connect(NULL, node, 1);

  mLnodeList.erase(node);
  delete_node(node, 2);
}

// DFFノードの削除
void
SbjGraph::delete_dff(SbjNode* node)
{
  ASSERT_COND(node->is_dff() );
  mDffList.erase(node);
  delete_node(node, 4);

  mLevel = 0;
  mLevelValid = false;
}

// 新しいノードを作成する．
// 作成されたノードを返す．
SbjNode*
SbjGraph::new_node(ymuint ni)
{
  SbjNode* node = NULL;

  // 空いているIDを探してノード配列へ登録
  int id = mItvlMgr.avail_num();
  mItvlMgr.erase(id);

  ymuint uid = static_cast<ymuint>(id);
  while( mNodeArray.size() <= uid ) {
    // mNodeArray の大きさが小さいときのための埋め草
    void* p = mAlloc.get_memory(sizeof(SbjNode));
    SbjNode* node = new (p) SbjNode;
    mNodeArray.push_back(node);
  }
  node = mNodeArray[uid];
  node->mId = uid;
  if ( ni > 0 ) {
    void* q = mAlloc2.get_memory(sizeof(SbjEdge) * ni);
    node->mFanins = new (q) SbjEdge[ni];
    for (ymuint i = 0; i < ni; ++ i) {
      node->mFanins[i].set_to(node, i);
    }
  }
  node->mMark = 0;

  return node;
}

// node を削除する．
void
SbjGraph::delete_node(SbjNode* node,
		      ymuint ni)
{
  // new_node の逆の処理を行なう．
  mAlloc2.put_memory(sizeof(SbjEdge) * ni, node->mFanins);
  node->mFanins = NULL;
  mItvlMgr.add(static_cast<int>(node->mId));
}

// @brief 出力ノードの内容を変更する
// @param[in] 変更対象の出力ノード
// @param[in] inode 入力のノード
// @param[in] inv 極性
void
SbjGraph::change_output(SbjNode* node,
			SbjHandle ihandle)
{
  ASSERT_COND( node->is_output() );
  node->set_output(node->subid(), ihandle.inv());
  connect(ihandle.node(), node, 0);
}

// @brief DFFノードの内容を変更する
// @param[in] 変更対象の出力ノード
// @param[in] ihandle 入力のハンドル
void
SbjGraph::set_dff_data(SbjNode* node,
		       SbjHandle ihandle)
{
  ASSERT_COND( node->is_dff() );
  node->set_dff(ihandle.inv());
  connect(ihandle.node(), node, 0);
}

// @brief DFFノードのクロック入力を設定する．
// @param[in] 変更対象のDFFノード
// @param[in] ihandle 入力のハンドル
void
SbjGraph::set_dff_clock(SbjNode* node,
			SbjHandle ihandle)
{
  ASSERT_COND( node->is_dff() );
  node->set_dff_clock(ihandle.inv());
  connect(ihandle.node(), node, 1);
}

// @brief DFFノードのセット入力を設定する．
// @param[in] 変更対象のDFFノード
// @param[in] ihandle 入力のハンドル
void
SbjGraph::set_dff_set(SbjNode* node,
		      SbjHandle ihandle)
{
  ASSERT_COND( node->is_dff() );
  node->set_dff_set(ihandle.inv());
  connect(ihandle.node(), node, 2);
}

// @brief DFFノードのリセット入力を設定する．
// @param[in] 変更対象のDFFノード
// @param[in] ihandle 入力のハンドル
void
SbjGraph::set_dff_rst(SbjNode* node,
		      SbjHandle ihandle)
{
  ASSERT_COND( node->is_dff() );
  node->set_dff_rst(ihandle.inv());
  connect(ihandle.node(), node, 3);
}

// from を to の pos 番目のファンインとする．
// to の pos 番目にすでに接続があった場合には自動的に削除される．
void
SbjGraph::connect(SbjNode* from,
		  SbjNode* to,
		  ymuint pos)
{
  // SbjNode::mFaoutList を変更するのはここだけ

  SbjEdge* edge = to->fanin_edge(pos);
  SbjNode* old_from = edge->from();
  if ( old_from ) {
    old_from->mFanoutList.erase(edge);
    old_from->scan_po();
  }
  edge->set_from(from);
  if ( from ) {
    from->mFanoutList.push_back(edge);
    from->scan_po();
  }

  mLevel = 0;
  mLevelValid = false;
}

// @brief 段数を求める．
// @note 副作用として各 SbjNode のレベルが設定される．
ymuint
SbjGraph::level() const
{
  if ( !mLevelValid ) {
    vector<const SbjNode*> tmp_list;
    ppi_list(tmp_list);
    for (vector<const SbjNode*>::const_iterator p = tmp_list.begin();
	 p != tmp_list.end(); ++ p) {
      const SbjNode* node = *p;
      // node の const を取り去るギミック
      mNodeArray[node->id()]->mLevel = 0;
    }

    vector<const SbjNode*> node_list;
    sort(node_list);
    for (vector<const SbjNode*>::const_iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      const SbjNode* node = *p;
      const SbjNode* inode0 = node->fanin(0);
      ymuint l = inode0->level();
      const SbjNode* inode1 = node->fanin(1);
      ymuint l1 = inode1->level();
      if ( l < l1 ) {
	l = l1;
      }
      // node の const を取り去るギミック
      mNodeArray[node->id()]->mLevel = l + 1;
    }

    ymuint max_l = 0;
    ppo_list(tmp_list);
    for (vector<const SbjNode*>::const_iterator p = tmp_list.begin();
	 p != tmp_list.end(); ++ p) {
      const SbjNode* node = *p;
      const SbjNode* inode = node->fanin(0);
      if ( inode ) {
	ymuint l1 = inode->level();
	if ( max_l < l1 ) {
	  max_l = l1;
	}
      }
    }

    mLevel = max_l;
    mLevelValid = true;
  }
  return mLevel;
}

// @brief 各ノードの minimum depth を求める．
// @param[in] k LUT の最大入力数
// @param[out] depth_array 各ノードの深さを収める配列
// @return 出力の最大深さを返す．
ymuint
SbjGraph::get_min_depth(ymuint k,
			vector<ymuint>& depth_array) const
{
  SbjMinDepth smd(*this);

  return smd(k, depth_array);
}

END_NAMESPACE_YM_SBJ
