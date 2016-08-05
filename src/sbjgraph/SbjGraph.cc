
/// @file SbjGraph.cc
/// @brief SbjGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "SbjGraph.h"
#include "SbjPort.h"
#include "SbjDff.h"
#include "SbjNode.h"
#include "SbjHandle.h"
#include "SbjMinDepth.h"

#include "ym/Expr.h"


BEGIN_NAMESPACE_YM_SBJ

///////////////////////////////////////////////////////////////////////
// クラス SbjNode
///////////////////////////////////////////////////////////////////////

// コンストラクタ
SbjNode::SbjNode() :
  mId(0),
  mFlags(0U),
  mFanins(nullptr),
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
  ymuint nfo = fanout_num();
  for (ymuint i = 0; i < nfo; ++ i) {
    const SbjEdge* e = fanout_edge(i);
    const SbjNode* to = e->to();
    if ( to->is_output() ) {
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
  ymuint ni = src.input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    SbjNode* src_node = src.input(i);
    SbjNode* dst_node = new_input();
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

    SbjNode* dst_node = _new_lnode(src_node->fcode(), input0, input1);
    dst_node->mMark = src_node->mMark;
    dst_node->mLevel = src_node->mLevel;
    nodemap[src_node->id()] = dst_node;
  }

  // 外部出力の生成
  ymuint no = src.output_num();
  for (ymuint i = 0; i < no; ++ i) {
    const SbjNode* src_onode = src.output(i);
    const SbjNode* src_inode = src_onode->fanin(0);
    SbjNode* dst_inode = nullptr;
    if ( src_inode ) {
      dst_inode = nodemap[src_inode->id()];
    }
    SbjNode* dst_node = new_output(SbjHandle(dst_inode, src_onode->output_inv()));
    nodemap[src_onode->id()] = dst_node;
  }

  // DFF の生成
  ymuint nf = src.dff_num();
  for (ymuint i = 0; i < nf; ++ i) {
    SbjDff* src_dff = src.dff(i);

    const SbjNode* src_input = src_dff->data_input();
    SbjNode* dst_input = nodemap[src_input->id()];

    const SbjNode* src_output = src_dff->data_output();
    SbjNode* dst_output = nodemap[src_output->id()];

    const SbjNode* src_clock = src_dff->clock();
    SbjNode* dst_clock = nodemap[src_clock->id()];

    const SbjNode* src_clear = src_dff->clear();
    SbjNode* dst_clear = nullptr;
    if ( src_clear ) {
      dst_clear = nodemap[src_clear->id()];
    }

    const SbjNode* src_preset = src_dff->preset();
    SbjNode* dst_preset = nullptr;
    if ( src_preset ) {
      dst_preset = nodemap[src_preset->id()];
    }

    new_dff(dst_input, dst_output, dst_clock, dst_clear, dst_preset);
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
  for (vector<SbjNode*>::iterator p = mOutputArray.begin();
       p != mOutputArray.end(); ++ p) {
    SbjNode* node = *p;
    connect(nullptr, node, 0);
  }
  for (vector<SbjNode*>::iterator p = mLnodeList.begin();
       p != mLnodeList.end(); ++ p) {
    SbjNode* node = *p;
    connect(nullptr, node, 0);
    connect(nullptr, node, 1);
  }

  for (vector<SbjNode*>::iterator p = mInputArray.begin();
       p != mInputArray.end(); ) {
    // p は実際にはノード内のメンバをアクセスするので delete する前に
    // 進めておく必要がある．
    SbjNode* node = *p;
    ++ p;
    delete_input(node);
  }

  for (vector<SbjNode*>::iterator p = mOutputArray.begin();
       p != mOutputArray.end(); ) {
    // p は実際にはノード内のメンバをアクセスするので delete する前に
    // 進めておく必要がある．
    SbjNode* node = *p;
    ++ p;
    delete_output(node);
  }

  for (vector<SbjNode*>::iterator p = mLnodeList.begin();
       p != mLnodeList.end(); ) {
    // p は実際にはノード内のメンバをアクセスするので delete する前に
    // 進めておく必要がある．
    SbjNode* node = *p;
    ++ p;
    delete_logic(node);
  }

  for (vector<SbjDff*>::iterator p = mDffList.begin();
       p != mDffList.end(); ) {
    // p は実際にはノード内のメンバをアクセスするので delete する前に
    // 進めておく必要がある．
    SbjDff* dff = *p;
    ++ p;
    delete_dff(dff);
  }

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
    return nullptr;
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


BEGIN_NONAMESPACE

// node のファンアウトのノードのうち，ファンインがすべてマークされている
// ノード を node_list に積む．
void
sort_sub(const SbjNode* node,
	 vector<bool>& mark,
	 vector<const SbjNode*>& node_list)
{
  ymuint nfo = node->fanout_num();
  for (ymuint i = 0; i < nfo; ++ i) {
    const SbjEdge* e = node->fanout_edge(i);
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

  // 外部入力のみをファンインにするノードを node_list に追加する．
  for (ymuint i = 0; i < input_num(); ++ i) {
    const SbjNode* node = input(i);
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
    if ( inode == nullptr || mark[inode->id()] || !inode->is_logic() ) continue;
    bool ready = true;
    ymuint nfo = inode->fanout_num();
    for (ymuint i = 0; i < nfo; ++ i) {
      const SbjEdge* e = inode->fanout_edge(i);
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
  for (ymuint i = 0; i < output_num(); ++ i) {
    const SbjNode* node = output(i);
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

  SbjNode* inode = ihandle.node();
  bool inv = ihandle.inv();
  node->set_output(subid, inv);
  connect(inode, node, 0);

  return node;
}

// @brief 論理式から論理ノード(の木)を作る．
// @param[in] expr 論理式
// @param[in] ihandle_list 入力ハンドルのリスト
// @return 作成したノードのハンドルを返す．
//
// 入力が定数の時も考慮している．
SbjHandle
SbjGraph::new_logic(const Expr& expr,
		    const vector<SbjHandle>& ihandle_list)
{
  if ( expr.is_zero() ) {
    return SbjHandle::make_zero();
  }
  if ( expr.is_one() ) {
    return SbjHandle::make_one();
  }

  if ( expr.is_posiliteral() ) {
    VarId vid = expr.varid();
    return ihandle_list[vid.val()];
  }
  if ( expr.is_negaliteral() ) {
    VarId vid = expr.varid();
    return ~ihandle_list[vid.val()];
  }

  ASSERT_COND( expr.is_op() );

  ymuint nc = expr.child_num();
  vector<SbjHandle> tmp_list(nc);
  for (ymuint i = 0;i < nc; ++ i) {
    tmp_list[i] = new_logic(expr.child(i), ihandle_list);
  }

  if ( expr.is_and() ) {
    return new_and(tmp_list);
  }
  if ( expr.is_or() ) {
    return new_or(tmp_list);
  }
  if ( expr.is_xor() ) {
    return new_xor(tmp_list);
  }

  ASSERT_NOT_REACHED;
  return SbjHandle::make_zero();
}

// @brief ANDノードを作る．
// @param[in] ihandle1 1番めの入力ハンドル
// @param[in] ihandle2 2番めの入力ハンドル
// @return 作成したノードのハンドルを返す．
//
// ihandle1/ihandle2 が定数の時も考慮している．
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
  SbjNode* sbjnode = _new_lnode(fcode, ihandle1.node(), ihandle2.node());
  return SbjHandle(sbjnode, false);
}

// @brief ANDノードを作る．
// @param[in] ihandle_list 入力ハンドルのリスト
// @return 作成したノードのハンドルを返す．
//
// 入力が定数の時も考慮している．
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
  SbjNode* sbjnode = _new_lnode(fcode, ihandle1.node(), ihandle2.node());
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
//
// ihandle1/ihandle2 が定数の時も考慮している．
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
  SbjNode* node = _new_lnode(4U, ihandle1.node(), ihandle2.node());
  return SbjHandle(node, inv);
}

// @brief XORノードを作る．
// @param[in] ihandle_list 入力ハンドルのリスト
// @return 作成したノードのハンドルを返す．
//
// 入力が定数の時も考慮している．
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

// @brief 論理ノードを作る．
// @param[in] fcode 機能コード
// @param[in] inode1 1番めの入力ノード
// @param[in] inode2 2番めの入力ノード
// @return 作成したノードを返す．
SbjNode*
SbjGraph::_new_lnode(ymuint fcode,
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

// DFFノードを作る．
SbjDff*
SbjGraph::new_dff(SbjNode* input,
		  SbjNode* output,
		  SbjNode* clock,
		  SbjNode* clear,
		  SbjNode* preset)
{
  SbjDff* dff = new SbjDff(input, output, clock, clear, preset);

  // DFFリストに登録
  dff->mId = mDffList.size();
  mDffList.push_back(dff);

  return dff;
}

// 入力ノードの削除
void
SbjGraph::delete_input(SbjNode* node)
{
  ASSERT_COND(node->is_input() );
  delete_node(node, 0);
}

// 出力ノードの削除
void
SbjGraph::delete_output(SbjNode* node)
{
  ASSERT_COND(node->is_output() );
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
  connect(nullptr, node, 0);
  connect(nullptr, node, 1);

  delete_node(node, 2);
}

// DFFノードの削除
void
SbjGraph::delete_dff(SbjDff* dff)
{
  delete dff;
}

// 新しいノードを作成する．
// 作成されたノードを返す．
SbjNode*
SbjGraph::new_node(ymuint ni)
{
  SbjNode* node = nullptr;

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
  node->mFanins = nullptr;
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

// from を to の pos 番目のファンインとする．
// to の pos 番目にすでに接続があった場合には自動的に削除される．
void
SbjGraph::connect(SbjNode* from,
		  SbjNode* to,
		  ymuint pos)
{
  // SbjNode::mFaoutList を変更するのはここだけ

  SbjEdge* edge = to->fanin_edge(pos);
  ASSERT_COND( edge->from() == nullptr );
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
    for (ymuint i = 0; i < input_num(); ++ i) {
      const SbjNode* node = input(i);
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
    for (ymuint i = 0; i < output_num(); ++ i) {
      const SbjNode* node = output(i);
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
