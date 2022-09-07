
/// @file SbjGraph.cc
/// @brief SbjGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2016, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "SbjGraph.h"
#include "SbjPort.h"
#include "SbjDff.h"
#include "SbjLatch.h"
#include "SbjNode.h"
#include "SbjHandle.h"
#include "SbjMinDepth.h"
#include "IOInfo.h"
#include "SbjDumper.h"

#include "ym/Expr.h"


BEGIN_NAMESPACE_SBJ

///////////////////////////////////////////////////////////////////////
// クラス SbjGraph
///////////////////////////////////////////////////////////////////////

// コピーコンストラクタ
SbjGraph::SbjGraph(
  const SbjGraph& src
)
{
  vector<SbjNode*> nodemap;
  _copy(src, nodemap);
}

// 代入演算子
SbjGraph&
SbjGraph::operator=(
  const SbjGraph& src
)
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
SbjGraph::_copy(
  const SbjGraph& src,
  vector<SbjNode*>& nodemap
)
{
  SizeType n = src.node_num();
  nodemap.clear();
  nodemap.resize(n);

  // 名前のコピー
  mName = src.mName;

  // 外部入力の生成
  for ( auto src_node: src.input_list() ) {
    auto dst_node = new_input(src_node->is_bipol());
    nodemap[src_node->id()] = dst_node;
  }

  // 論理ノードの生成
  for ( auto src_node: src.logic_list() ) {
    auto src_inode0 = src_node->fanin0();
    auto input0 = nodemap[src_inode0->id()];
    ASSERT_COND( input0 );
    SbjHandle ihandle0{input0, src_node->fanin0_inv()};

    auto src_inode1 = src_node->fanin1();
    auto input1 = nodemap[src_inode1->id()];
    ASSERT_COND( input1 );
    SbjHandle ihandle1{input1, src_node->fanin1_inv()};

    auto dst_node = _new_logic_node(src_node->type(), ihandle0, ihandle1);
    nodemap[src_node->id()] = dst_node;
  }

  // 外部出力の生成
  for ( auto src_onode: src.output_list() ) {
    auto src_inode = src_onode->output_fanin();
    SbjNode* dst_inode{nullptr};
    if ( src_inode ) {
      dst_inode = nodemap[src_inode->id()];
    }
    auto dst_node = new_output(SbjHandle(dst_inode, src_onode->output_fanin_inv()));
    nodemap[src_onode->id()] = dst_node;
  }

  // DFF の生成
  for ( auto src_dff: src.dff_list() ) {
    auto src_input = src_dff->data_input();
    auto dst_input = nodemap[src_input->id()];

    auto src_output = src_dff->data_output();
    auto dst_output = nodemap[src_output->id()];

    auto src_clock = src_dff->clock();
    auto dst_clock = nodemap[src_clock->id()];

    auto src_clear = src_dff->clear();
    SbjNode* dst_clear{nullptr};
    if ( src_clear ) {
      dst_clear = nodemap[src_clear->id()];
    }

    auto src_preset = src_dff->preset();
    SbjNode* dst_preset{nullptr};
    if ( src_preset ) {
      dst_preset = nodemap[src_preset->id()];
    }

    new_dff(dst_input, dst_output, dst_clock, dst_clear, dst_preset);
  }

  // ラッチの生成
  for ( auto src_latch: src.latch_list() ) {
    auto src_input = src_latch->data_input();
    auto dst_input = nodemap[src_input->id()];

    auto src_output = src_latch->data_output();
    auto dst_output = nodemap[src_output->id()];

    auto src_enable = src_latch->enable();
    auto dst_enable = nodemap[src_enable->id()];

    auto src_clear = src_latch->clear();
    SbjNode* dst_clear{nullptr};
    if ( src_clear ) {
      dst_clear = nodemap[src_clear->id()];
    }

    auto src_preset = src_latch->preset();
    SbjNode* dst_preset{nullptr};
    if ( src_preset ) {
      dst_preset = nodemap[src_preset->id()];
    }

    new_latch(dst_input, dst_output, dst_enable, dst_clear, dst_preset);
  }

  // ポートの複製
  for ( auto src_port: src.port_list() ) {
    SizeType nb = src_port->bit_width();
    vector<SbjNode*> tmp(nb);
    for ( int j = 0; j < nb; ++ j ) {
      auto src_node = src_port->bit(j);
      auto dst_node = nodemap[src_node->id()];
      tmp[j] = dst_node;
    }
    add_port(src_port->name(), tmp);
  }

  mLevel = src.mLevel;
}

// 空にする．
void
SbjGraph::clear()
{
  for ( auto node: mInputArray ) {
    delete node;
  }

  for ( auto node: mLogicList ) {
    delete node;
  }

  for ( auto node: mOutputArray ) {
    delete node;
  }

  for ( auto dff: mDffList ) {
    delete dff;
  }

  for ( auto latch: mLatchList ) {
    delete latch;
  }

  for ( auto port: mPortArray ) {
    delete port;
  }

  for ( auto info: mInputInfoArray ) {
    delete info;
  }

  for ( auto info: mOutputInfoArray ) {
    delete info;
  }

  mInputArray.clear();
  mInputInfoArray.clear();
  mOutputArray.clear();
  mOutputInfoArray.clear();
  mLogicList.clear();
  mDffList.clear();
  mLatchList.clear();
  mPortArray.clear();
}

// @brief ポートを追加する(ベクタ版)．
void
SbjGraph::add_port(
  const string& name,
  const vector<SbjNode*>& body
)
{
  auto port = new SbjPort{name, body};
  mPortArray.push_back(port);
  SizeType n = body.size();
  for ( SizeType i = 0; i < n; ++ i ) {
    auto node = body[i];
    if ( node->is_input() ) {
      auto info = new IOPortInfo{port, i};
      mInputInfoArray[node->subid()] = info;
    }
    else if ( node->is_output() ) {
      auto info = new IOPortInfo{port, i};
      mOutputInfoArray[node->subid()] = info;
    }
    else {
      ASSERT_NOT_REACHED;
    }
  }
}

// @brief 入出力ノードに関連づけられたポートを得る．
const SbjPort*
SbjGraph::port(
  const SbjNode* node
) const
{
  if ( node->is_input() ) {
    return mInputInfoArray[node->subid()]->port();
  }
  else if ( node->is_output() ) {
    return mOutputInfoArray[node->subid()]->port();
  }
  else {
    return nullptr;
  }
}

// @brief 入出力ノードのポートにおけるビット位置を得る．
SizeType
SbjGraph::port_pos(
  const SbjNode* node
) const
{
  if ( node->is_input() ) {
    return mInputInfoArray[node->subid()]->port_bitpos();
  }
  else if ( node->is_output() ) {
    return mOutputInfoArray[node->subid()]->port_bitpos();
  }
  else {
    return -1;
  }
}

// 入力ノードを作る．
SbjNode*
SbjGraph::new_input(
  bool bipol
)
{
  SizeType id = mNodeArray.size();
  SizeType subid = mInputArray.size();
  auto node = new SbjNode{id, bipol, subid};

  // ノードリストの登録
  mNodeArray.push_back(node);

  // 入力ノード配列に登録
  mInputArray.push_back(node);

  // ダミーの place-holder を追加
  mInputInfoArray.push_back(nullptr);

  return node;
}

// 出力ノードを作る．
SbjNode*
SbjGraph::new_output(
  SbjHandle ihandle
)
{
  SizeType id = mNodeArray.size();
  SizeType subid = mOutputArray.size();
  auto node = new SbjNode{id, subid, ihandle};

  // ノードリストの登録
  mNodeArray.push_back(node);

  // 出力ノード配列に登録
  mOutputArray.push_back(node);

  // ダミーの place-holder を追加
  mOutputInfoArray.push_back(nullptr);

  if ( mLevel < node->level() ) {
    mLevel = node->level();
  }

  return node;
}

// @brief 論理式から論理ノード(の木)を作る．
SbjHandle
SbjGraph::new_expr(
  const Expr& expr,
  const vector<SbjHandle>& ihandle_list
)
{
  if ( expr.is_zero() ) {
    return SbjHandle::make_zero();
  }
  if ( expr.is_one() ) {
    return SbjHandle::make_one();
  }

  if ( expr.is_posi_literal() ) {
    VarId vid = expr.varid();
    return ihandle_list[vid.val()];
  }
  if ( expr.is_nega_literal() ) {
    VarId vid = expr.varid();
    return ~ihandle_list[vid.val()];
  }

  ASSERT_COND( expr.is_op() );

  vector<SbjHandle> tmp_list;
  tmp_list.reserve(expr.operand_num());
  for ( auto& expr1: expr.operand_list() ) {
    auto h = new_expr(expr1, ihandle_list);
    tmp_list.push_back(h);
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
SbjHandle
SbjGraph::new_and(
  SbjHandle ihandle1,
  SbjHandle ihandle2
)
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

  auto sbjnode = _new_logic_node(SbjNodeType::And, ihandle1, ihandle2);
  return SbjHandle{sbjnode, false};
}

// @brief ANDノードを作る．
SbjHandle
SbjGraph::new_and(
  const vector<SbjHandle>& ihandle_list
)
{
  SizeType n = ihandle_list.size();
  vector<SbjHandle> tmp_list;
  tmp_list.reserve(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto h = ihandle_list[i];
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
  return _new_and_tree(tmp_list, 0, tmp_list.size());
}

// @brief new_and の下請け関数
SbjHandle
SbjGraph::_new_and_tree(
  const vector<SbjHandle>& ihandle_list,
  SizeType start,
  SizeType end
)
{
  SizeType num = end - start;
  ASSERT_COND( num > 0 );
  if ( num == 1 ) {
    return ihandle_list[start];
  }

  SizeType h = (start + end) / 2;
  auto l = _new_and_tree(ihandle_list, start, h);
  auto r = _new_and_tree(ihandle_list, h, end);
  return new_and(l, r);
}

// @brief ORノードを作る．
SbjHandle
SbjGraph::new_or(
  SbjHandle ihandle1,
  SbjHandle ihandle2
)
{
  return ~new_and(~ihandle1, ~ihandle2);
}

// @brief ORノードを作る．
SbjHandle
SbjGraph::new_or(
  const vector<SbjHandle>& ihandle_list
)
{
  SizeType n = ihandle_list.size();
  vector<SbjHandle> tmp_list;
  tmp_list.reserve(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto h = ihandle_list[i];
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
  return _new_or_tree(tmp_list, 0, tmp_list.size());
}

// @brief new_or の下請け関数
SbjHandle
SbjGraph::_new_or_tree(
  const vector<SbjHandle>& ihandle_list,
  SizeType start,
  SizeType end
)
{
  SizeType num = end - start;
  ASSERT_COND( num > 0 );
  if ( num == 1 ) {
    return ihandle_list[start];
  }

  SizeType h = (start + end) / 2;
  auto l = _new_or_tree(ihandle_list, start, h);
  auto r = _new_or_tree(ihandle_list, h, end);
  return new_or(l, r);
}

// @brief XORノードを作る．
SbjHandle
SbjGraph::new_xor(
  SbjHandle ihandle1,
  SbjHandle ihandle2
)
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
  auto node = _new_logic_node(SbjNodeType::Xor,
			      ihandle1.normalize(), ihandle2.normalize());
  return SbjHandle{node, inv};
}

// @brief XORノードを作る．
SbjHandle
SbjGraph::new_xor(
  const vector<SbjHandle>& ihandle_list
)
{
  SizeType n = ihandle_list.size();
  vector<SbjHandle> tmp_list;
  tmp_list.reserve(n);
  bool inv = false;
  for ( SizeType i = 0; i < n; ++ i ) {
    auto h = ihandle_list[i];
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
  return _new_xor_tree(tmp_list, 0, tmp_list.size());
}

// @brief new_xor の下請け関数
SbjHandle
SbjGraph::_new_xor_tree(
  const vector<SbjHandle>& ihandle_list,
  SizeType start,
  SizeType end
)
{
  SizeType num = end - start;
  ASSERT_COND( num > 0 );
  if ( num == 1 ) {
    return ihandle_list[start];
  }

  SizeType h = (start + end) / 2;
  auto l = _new_xor_tree(ihandle_list, start, h);
  auto r = _new_xor_tree(ihandle_list, h, end);
  return new_xor(l, r);
}

// @brief 新しい論理ノードを作る．
inline
SbjNode*
SbjGraph::_new_logic_node(
  SbjNodeType type,
  SbjHandle ihandle1,
  SbjHandle ihandle2
)
{
  SizeType id = mNodeArray.size();
  auto node = new SbjNode{id, type, ihandle1, ihandle2};

  // ノードリストに登録
  mNodeArray.push_back(node);

  // 論理ノードリストに登録
  mLogicList.push_back(node);

  return node;
}

// DFFノードを作る．
SbjDff*
SbjGraph::new_dff(
  SbjNode* input,
  SbjNode* output,
  SbjNode* clock,
  SbjNode* clear,
  SbjNode* preset
)
{
  SizeType id = mDffList.size();
  auto dff = new SbjDff{id, input, output, clock, clear, preset};

  // DFFリストに登録
  mDffList.push_back(dff);

  // 端子の情報を作る．
  mOutputInfoArray[input->subid()] = new IODffInputInfo(dff);
  mInputInfoArray[output->subid()] = new IODffOutputInfo(dff);
  mOutputInfoArray[clock->subid()] = new IODffClockInfo(dff);
  if ( clear != nullptr ) {
    mOutputInfoArray[clear->subid()] = new IODffClearInfo(dff);
  }
  if ( preset != nullptr ) {
    mOutputInfoArray[preset->subid()] = new IODffPresetInfo(dff);
  }

  return dff;
}

// @brief node に関連付けられている DFF を得る．
const SbjDff*
SbjGraph::dff(
  const SbjNode* node
) const
{
  if ( node->is_input() ) {
    return mInputInfoArray[node->subid()]->dff();
  }
  else if ( node->is_output() ) {
    return mOutputInfoArray[node->subid()]->dff();
  }
  return nullptr;
}

// @brief node がDFFの入力だった時に true を返す．
bool
SbjGraph::is_dff_input(
  const SbjNode* node
) const
{
  if ( node->is_output() ) {
    return mOutputInfoArray[node->subid()]->is_dff_input();
  }
  return false;
}

// @brief node がDFFの出力だった時に true を返す．
bool
SbjGraph::is_dff_output(
  const SbjNode* node
) const
{
  if ( node->is_input() ) {
    return mInputInfoArray[node->subid()]->is_dff_output();
  }
  return false;
}

// @brief node がDFFのクロック端子だった時に true を返す．
bool
SbjGraph::is_dff_clock(
  const SbjNode* node
) const
{
  if ( node->is_output() ) {
    return mOutputInfoArray[node->subid()]->is_dff_clock();
  }
  return false;
}

// @brief node がDFFのクリア端子だった時に true を返す．
bool
SbjGraph::is_dff_clear(
  const SbjNode* node
) const
{
  if ( node->is_output() ) {
    return mOutputInfoArray[node->subid()]->is_dff_clear();
  }
  return false;
}

// @brief node がDFFのセット端子だった時に true を返す．
bool
SbjGraph::is_dff_preset(
  const SbjNode* node
) const
{
  if ( node->is_output() ) {
    return mOutputInfoArray[node->subid()]->is_dff_preset();
  }
  return false;
}

// ラッチノードを作る．
SbjLatch*
SbjGraph::new_latch(
  SbjNode* input,
  SbjNode* output,
  SbjNode* enable,
  SbjNode* clear,
  SbjNode* preset
)
{
  SizeType id = mLatchList.size();
  auto latch = new SbjLatch{id, input, output, enable, clear, preset};

  // ラッチリストに登録
  mLatchList.push_back(latch);

  // 端子の情報を作る．
  mOutputInfoArray[input->subid()] = new IOLatchInputInfo(latch);
  mInputInfoArray[output->subid()] = new IOLatchOutputInfo(latch);
  mOutputInfoArray[enable->subid()] = new IOLatchEnableInfo(latch);
  if ( clear != nullptr ) {
    mOutputInfoArray[clear->subid()] = new IOLatchClearInfo(latch);
  }
  if ( preset != nullptr ) {
    mOutputInfoArray[preset->subid()] = new IOLatchPresetInfo(latch);
  }

  return latch;
}

// @brief node に関連付けられているラッチを返す．
const SbjLatch*
SbjGraph::latch(
  const SbjNode* node
) const
{
  if ( node->is_input() ) {
    return mInputInfoArray[node->subid()]->latch();
  }
  else if ( node->is_output() ) {
    return mOutputInfoArray[node->subid()]->latch();
  }
  return nullptr;
}

// @brief node がラッチの入力だった場合に true を返す．
bool
SbjGraph::is_latch_input(
  const SbjNode* node
) const
{
  if ( node->is_output() ) {
    return mOutputInfoArray[node->subid()]->is_latch_input();
  }
  return false;
}

// @brief node がラッチの出力だった場合に true を返す．
bool
SbjGraph::is_latch_output(
  const SbjNode* node
) const
{
  if ( node->is_input() ) {
    return mInputInfoArray[node->subid()]->is_latch_output();
  }
  return false;
}

// @brief node がラッチのイネーブル端子だった場合に true を返す．
bool
SbjGraph::is_latch_enable(
  const SbjNode* node
) const
{
  if ( node->is_output() ) {
    return mOutputInfoArray[node->subid()]->is_latch_enable();
  }
  return false;
}

// @brief node がラッチのクリア端子だった場合に true を返す．
bool
SbjGraph::is_latch_clear(
  const SbjNode* node
) const
{
  if ( node->is_output() ) {
    return mOutputInfoArray[node->subid()]->is_latch_clear();
  }
  return false;
}

// @brief node がラッチのセット端子だった場合に true を返す．
bool
SbjGraph::is_latch_preset(
  const SbjNode* node
) const
{
  if ( node->is_output() ) {
    return mOutputInfoArray[node->subid()]->is_latch_preset();
  }
  return false;
}

// @brief 各ノードの minimum depth を求める．
SizeType
SbjGraph::get_min_depth(
  SizeType k,
  vector<SizeType>& depth_array
) const
{
  SbjMinDepth smd{*this};

  return smd(k, depth_array);
}

END_NAMESPACE_SBJ
