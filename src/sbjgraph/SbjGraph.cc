
/// @file SbjGraph.cc
/// @brief SbjGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "SbjGraph.h"
#include "SbjPort.h"
#include "SbjDff.h"
#include "SbjLatch.h"
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
  mMark(0)
{
  for (ymuint i = 0; i < 2; ++ i) {
    SbjEdge& edge = mFanins[i];
    edge.mFrom = nullptr;
    edge.mTo = this;
    edge.mIpos = i;
  }
}

// デストラクタ
SbjNode::~SbjNode()
{
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


///////////////////////////////////////////////////////////////////////
// クラス SbjGraph
///////////////////////////////////////////////////////////////////////

// コンストラクタ
SbjGraph::SbjGraph() :
  mLevel(0),
  mLevelValid(false)
{
}

// コピーコンストラクタ
SbjGraph::SbjGraph(const SbjGraph& src)
{
  vector<SbjNode*> nodemap;
  _copy(src, nodemap);
}

// 代入演算子
const SbjGraph&
SbjGraph::operator=(const SbjGraph& src)
{
  if ( &src != this ) {
    clear();
    vector<SbjNode*> nodemap;
    _copy(src, nodemap);
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
SbjGraph::_copy(const SbjGraph& src,
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
    SbjNode* dst_node = new_input(src_node->is_bipol());
    nodemap[src_node->id()] = dst_node;
  }

  // 論理ノードの生成
  ymuint nlogic = src.logic_num();
  for (ymuint i = 0; i < nlogic; ++ i) {
    const SbjNode* src_node = src.logic(i);

    const SbjNode* src_inode0 = src_node->fanin(0);
    SbjNode* input0 = nodemap[src_inode0->id()];
    ASSERT_COND( input0 );

    const SbjNode* src_inode1 = src_node->fanin(1);
    SbjNode* input1 = nodemap[src_inode1->id()];
    ASSERT_COND( input1 );

    SbjNode* dst_node = _new_logic(src_node->fcode(), input0, input1);
    dst_node->mMark = src_node->mMark;
    dst_node->mLevel = src_node->mLevel;
    nodemap[src_node->id()] = dst_node;
  }

  // 外部出力の生成
  ymuint no = src.output_num();
  for (ymuint i = 0; i < no; ++ i) {
    const SbjNode* src_onode = src.output(i);
    const SbjNode* src_inode = src_onode->output_fanin();
    SbjNode* dst_inode = nullptr;
    if ( src_inode ) {
      dst_inode = nodemap[src_inode->id()];
    }
    SbjNode* dst_node = new_output(SbjHandle(dst_inode, src_onode->output_fanin_inv()));
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

  // ラッチの生成
  ymuint nlatch = src.latch_num();
  for (ymuint i = 0; i < nlatch; ++ i) {
    SbjLatch* src_latch = src.latch(i);

    const SbjNode* src_input = src_latch->data_input();
    SbjNode* dst_input = nodemap[src_input->id()];

    const SbjNode* src_output = src_latch->data_output();
    SbjNode* dst_output = nodemap[src_output->id()];

    const SbjNode* src_enable = src_latch->enable();
    SbjNode* dst_enable = nodemap[src_enable->id()];

    const SbjNode* src_clear = src_latch->clear();
    SbjNode* dst_clear = nullptr;
    if ( src_clear ) {
      dst_clear = nodemap[src_clear->id()];
    }

    const SbjNode* src_preset = src_latch->preset();
    SbjNode* dst_preset = nullptr;
    if ( src_preset ) {
      dst_preset = nodemap[src_preset->id()];
    }

    new_latch(dst_input, dst_output, dst_enable, dst_clear, dst_preset);
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
  for (vector<SbjNode*>::iterator p = mInputArray.begin();
       p != mInputArray.end(); ++ p) {
    SbjNode* node = *p;
    delete node;
  }

  for (vector<SbjNode*>::iterator p = mLogicList.begin();
       p != mLogicList.end(); ++ p) {
    SbjNode* node = *p;
    delete node;
  }

  for (vector<SbjNode*>::iterator p = mOutputArray.begin();
       p != mOutputArray.end(); ++ p) {
    SbjNode* node = *p;
    delete node;
  }

  for (vector<SbjDff*>::iterator p = mDffList.begin();
       p != mDffList.end(); ++ p) {
    SbjDff* dff = *p;
    delete dff;
  }

  for (vector<SbjLatch*>::iterator p = mLatchList.begin();
       p != mLatchList.end(); ++ p) {
    SbjLatch* latch = *p;
    delete latch;
  }

  for (vector<SbjPort*>::iterator p = mPortArray.begin();
       p != mPortArray.end(); ++ p) {
    delete *p;
  }

  mInputArray.clear();
  mInputPortArray.clear();
  mOutputArray.clear();
  mOutputPortArray.clear();
  mLogicList.clear();
  mDffList.clear();
  mLatchList.clear();
  mPortArray.clear();
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

// 入力ノードを作る．
SbjNode*
SbjGraph::new_input(bool bipol)
{
  SbjNode* node = _new_node();

  // 入力ノード配列に登録
  ymuint subid = mInputArray.size();
  mInputArray.push_back(node);

  // ダミーの place-holder を追加
  mInputPortArray.push_back(PortInfo());

  node->set_input(subid, bipol);

  return node;
}

// 出力ノードを作る．
SbjNode*
SbjGraph::new_output(SbjHandle ihandle)
{
  SbjNode* node = _new_node();

  // 出力ノード配列に登録
  ymuint subid = mOutputArray.size();
  mOutputArray.push_back(node);

  // ダミーの place-holder を追加
  mOutputPortArray.push_back(PortInfo());

  SbjNode* inode = ihandle.node();
  bool inv = ihandle.inv();
  node->set_output(subid, inv, inode);

  if ( mLevel < node->mLevel ) {
    mLevel = node->mLevel;
  }

  return node;
}

// @brief 論理式から論理ノード(の木)を作る．
// @param[in] expr 論理式
// @param[in] ihandle_list 入力ハンドルのリスト
// @return 作成したノードのハンドルを返す．
//
// 入力が定数の時も考慮している．
SbjHandle
SbjGraph::new_expr(const Expr& expr,
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
    tmp_list[i] = new_expr(expr.child(i), ihandle_list);
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

  if ( ihandle1.node() == ihandle2.node() ) {
    if ( ihandle1.inv() == ihandle2.inv() ) {
      // 同じ入力
      return ihandle1;
    }
    else {
      // 同じノードで逆相
      return SbjHandle::make_zero();
    }
  }

  ymuint fcode = 0U;
  if ( ihandle1.inv() ) {
    fcode |= 1U;
  }
  if ( ihandle2.inv() ) {
    fcode |= 2U;
  }
  SbjNode* sbjnode = _new_logic(fcode, ihandle1.node(), ihandle2.node());
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

  if ( ihandle1.node() == ihandle2.node() ) {
    if ( ihandle1.inv() == ihandle2.inv() ) {
      // 同じ入力
      return ihandle1;
    }
    else {
      // 同じノードで逆相
      return SbjHandle::make_one();
    }
  }

  ymuint fcode = 0U;
  if ( !ihandle1.inv() ) {
    fcode |= 1U;
  }
  if ( !ihandle2.inv() ) {
    fcode |= 2U;
  }
  SbjNode* sbjnode = _new_logic(fcode, ihandle1.node(), ihandle2.node());
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

  if ( ihandle1.node() == ihandle2.node() ) {
    if ( ihandle1.inv() == ihandle2.inv() ) {
      // 同じ入力
      return ihandle1;
    }
    else {
      // 同じノードで逆相
      return SbjHandle::make_one();
    }
  }

  bool inv = ihandle1.inv() ^ ihandle2.inv();
  SbjNode* node = _new_logic(4U, ihandle1.node(), ihandle2.node());
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
SbjGraph::_new_logic(ymuint fcode,
		     SbjNode* inode0,
		     SbjNode* inode1)
{
  SbjNode* node = _new_node();

  // 論理ノードリストに登録
  mLogicList.push_back(node);

  node->set_logic(fcode, inode0, inode1);

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

// ラッチノードを作る．
SbjLatch*
SbjGraph::new_latch(SbjNode* input,
		    SbjNode* output,
		    SbjNode* enable,
		    SbjNode* clear,
		    SbjNode* preset)
{
  SbjLatch* latch = new SbjLatch(input, output, enable, clear, preset);

  // ラッチリストに登録
  latch->mId = mLatchList.size();
  mLatchList.push_back(latch);

  return latch;
}

// 新しいノードを作成する．
// 作成されたノードを返す．
SbjNode*
SbjGraph::_new_node()
{
  SbjNode* node = new SbjNode;

  node->mId = mNodeArray.size();
  mNodeArray.push_back(node);

  return node;
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
