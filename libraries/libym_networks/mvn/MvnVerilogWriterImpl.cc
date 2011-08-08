
/// @file libym_networks/mvn/MvnVerilogWriterImpl.cc
/// @brief MvnVerilogWriterImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.

/*
 * アルゴリズム
 *
 * - MvnNode の出力ピンをネットに対応させる．
 * - ほとんどのノードはただ一つの出力を持つが，セルの場合は複数出力の場合がありうる．
 * - 今は使っていないが module instance も複数出力がありうる．
 * - 組み合わせ回路に対応するノードの場合，assign 文に対応させる．
 * - D-FF とラッチは always 文に対応させる．
 */

#include "ym_networks/MvnVerilogWriterImpl.h"

#include "ym_networks/MvnMgr.h"
#include "ym_networks/MvnModule.h"
#include "ym_networks/MvnPort.h"
#include "ym_networks/MvnNode.h"
#include "ym_networks/MvnInputPin.h"
#include "ym_networks/MvnOutputPin.h"

#include "ym_networks/MvnVlMap.h"

#include "ym_cell/Cell.h"

#include "ym_verilog/vl/VlDecl.h"
#include "ym_verilog/vl/VlDeclArray.h"
#include "ym_verilog/vl/VlRange.h"

#include "NodeDumper.h"


BEGIN_NAMESPACE_YM_NETWORKS_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvnVerilogWriterImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MvnVerilogWriterImpl::MvnVerilogWriterImpl() :
  mNextId(0)
{
}

// @brief デストラクタ
MvnVerilogWriterImpl::~MvnVerilogWriterImpl()
{
}

// @brief 内容を Verilog-HDL 形式で出力する．
// @param[in] s 出力先のストリーム
// @param[in] mgr MvnMgr
void
MvnVerilogWriterImpl::dump(ostream& s,
			   const MvnMgr& mgr,
			   const MvnVlMap& node_map)
{
  set_max_id(mgr.max_node_id());

  ymuint n = mgr.max_module_id();
  for (ymuint i = 0; i < n; ++ i) {
    const MvnModule* module = mgr.module(i);
    if ( module == NULL ) continue;

    dump_module(s, module, mgr);
  }

  mNextId = 0;
  mMap.clear();
}

// @brief ノードと Verilog 名の対応関係をコメントで出力する．
// @param[in] s 出力先のストリーム
// @param[in] mgr MvnMgr
// @param[in] node_map ノードと Verilog 名の対応表
void
MvnVerilogWriterImpl::dump_map(ostream& s,
			       const MvnMgr& mgr,
			       const MvnVlMap& node_map)
{
  ymuint node_num = mgr.max_node_id();
  for (ymuint i = 0; i < node_num; ++ i) {
    const MvnNode* node = mgr.node(i);
    if ( node == NULL ) continue;

    s << "// node" << node->id() << " : ";
    if ( node_map.is_single_elem(i) ) {
      const VlDecl* decl = node_map.get_single_elem(i);
      assert_cond( decl != NULL, __FILE__, __LINE__);
      s << decl->full_name();
    }
    else if ( node_map.is_array_elem(i) ) {
      const VlDeclArray* declarray = node_map.get_array_elem(i);
      assert_cond( declarray != NULL, __FILE__, __LINE__);
      ymuint offset = node_map.get_array_offset(i);
      ymuint d = declarray->dimension();
      vector<int> index_array(d);
      for (ymuint i = 0; i < d; ++ i) {
	const VlRange* range = declarray->range(i);
	ymuint n = range->size();
	index_array[i] = offset % n;
	offset /= n;
      }
      s << declarray->full_name();
      for (ymuint i = 0; i < d; ++ i) {
	s << "[" << index_array[d - i - 1] << "]";
      }
    }
    s << endl;
  }
}

// @brief モジュールを出力する．
// @param[in] s 出力先のストリーム
// @param[in] module 対象のモジュール
void
dump_module(ostream& s,
	    const MvnModule* module)
{
  s << "module " << module->name() << "(";

  ymuint np = module->port_num();
  const char* comma = "";
  for (ymuint i = 0; i < np; ++ i) {
    s << comma;
    comma = ", ";
    const MvnPort* port = module->port(i);
    dump_port(s, port);
  }

  s << ");" << endl;

  ymuint ni = module->input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const MvnNode* node = module->input(i);
    ymuint bw = node->output(0)->bit_width();
    assert_cond( bw > 0, __FILE__, __LINE__);
    if ( bw == 1 ) {
      s << "  input ";
    }
    else {
      s << "  input [" << bw - 1 << ":0] ";
    }
    s << node_name(node) << ";" << endl;
  }

  ymuint no = module->output_num();
  for (ymuint i = 0; i < no; ++ i) {
    const MvnNode* node = module->output(i);
    ymuint bw = node->input(0)->bit_width();
    assert_cond( bw > 0, __FILE__, __LINE__);
    if ( bw == 1 ) {
      s << "  output ";
    }
    else {
      s << "  output [" << bw - 1 << ":0] ";
    }
    s << node_name(node) << ";" << endl;
  }

  ymuint nio = module->inout_num();
  for (ymuint i = 0; i < nio; ++ i) {
    const MvnNode* node = module->inout(i);
    ymuint bw = node->input(0)->bit_width();
    assert_cond( bw > 0, __FILE__, __LINE__);
    if ( bw == 1 ) {
      s << "  inout ";
    }
    else {
      s << "  inout [" << bw - 1 << ":0] ";
    }
    s << node_name(node) << ";" << endl;
  }
  s << endl;

  const list<MvnNode*>& node_list = module->node_list();
  for (list<MvnNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    MvnNode* node = *p;
    assert_cond( node->output_num() == 1, __FILE__, __LINE__);
    ymuint bw = node->output(0)->bit_width();
    if ( node->type() == MvnNode::kDff || node->type() == MvnNode::kLatch ) {
      s << "  reg  ";
      if ( bw > 1 ) {
	s << "[" << bw - 1 << ":" << "0]";
      }
      s << " " << node_name(node) << ";" << endl;
    }
    else {
      s << "  wire ";
      if ( bw > 1 ) {
	s << "[" << bw - 1 << ":" << "0]";
      }
      s << " " << node_name(node) << ";" << endl;
    }
  }
  s << endl;

  for (ymuint i = 0; i < ni; ++ i) {
    const MvnNode* node = module->input(i);
    dump_node(s, node);
  }
  for (ymuint i = 0; i < no; ++ i) {
    const MvnNode* node = module->output(i);
    dump_node(s, node);
  }
  for (ymuint i = 0; i < nio; ++ i) {
    const MvnNode* node = module->inout(i);
    dump_node(s, node);
  }
  for (list<MvnNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    MvnNode* node = *p;
    dump_node(s, node);
  }

  s << "endmodule" << endl
    << endl;
}

// @brief ノードを出力する．
// @param[in] s 出力先のストリーム
// @param[in] node 対象のノード
void
MvnVerilogWriterImpl::dump_node(ostream& s,
				const MvnNode* node)
{
  switch ( node->type() ) {
  case MvnNode::kInput:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 0, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);
    }
    break;

  case MvnNode::kOutput:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 1, __FILE__, __LINE__);

      const MvnInputPin* ipin = node->input(0);
      const MvnOutputPin* src_pin = ipin->src_pin();
      if ( src_pin ) {
	s << "  assign " << net_name(node->output(0))
	  << " = " << net_name(src_pin)
	  << ";" << endl;
      }
    }
    break;

  case MvnNode::kInout:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 1, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvnInputPin* ipin = node->input(0);
      const MvnOutputPin* src_pin = ipin->src_pin();
      if ( src_pin ) {
	s << "  assign " << net_name(node->output(0))
	  << " = " << net_name(src_pin)
	  << ";" << endl;
      }
    }
    break;

  case MvnNode::kDff:
    dump_dff(s, node);
    break;

  case MvnNode::kLatch:
    dump_latch(s, node);
    break;

  case MvnNode::kThrough:
    dump_comb(s, node, UnaryOpDumper(""));
    break;

  case MvnNode::kNot:
    dump_comb(s, node, UnaryOpDumper("~"));
    break;

  case MvnNode::kAnd:
    dump_comb(s, node, NaryOpDumper("&"));
    break;

  case MvnNode::kOr:
    dump_comb(s, node, NaryOpDumper("|"));
    break;

  case MvnNode::kXor:
    dump_comb(s, node, NaryOpDumper("^"));
    break;

  case MvnNode::kRand:
    dump_comb(s, node, UnaryOpDumper("&"));
    break;

  case MvnNode::kRor:
    dump_comb(s, node, UnaryOpDumper("|"));
    break;

  case MvnNode::kRxor:
    dump_comb(s, node, UnaryOpDumper("^"));
    break;

  case MvnNode::kEq:
    dump_comb(s, node, BinaryOpDumper("==", "(", ")"));
    break;

  case MvnNode::kLt:
    dump_comb(s, node, BinaryOpDumper("<", "(", ")"));
    break;

  case MvnNode::kCaseEq:
    dump_comb(s, node, CaseEqDumper());
#if 0
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 2, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvnInputPin* ipin0 = node->input(0);
      const MvnOutputPin* src_pin0 = ipin0->src_pin();
      const MvnNode* src_node0 = src_pin0->node();

      const MvnInputPin* ipin1 = node->input(1);
      const MvnOutputPin* src_pin1 = ipin1->src_pin();
      const MvnNode* src_node1 = src_pin1->node();

      vector<ymuint32> xmask;
      node->xmask(xmask);
      ymuint bw = ipin0->bit_width();
      string mask_str;
      for (ymuint i = 0; i < bw; ++ i) {
	ymuint bitpos = bw - i - 1;
	ymuint blk = bitpos / 32;
	ymuint sft = bitpos % 32;
	if ( xmask[blk] & (1U << sft) ) {
	  mask_str += "?";
	}
	else {
	  mask_str += "0";
	}
      }
      s << "  assign " << node_name(node)
	<< " = (" << node_name(src_node0)
	<< " === " << node_name(src_node1)
	<< " ^ " << bw << "'b" << mask_str
	<< ");" << endl;
    }
#endif
    break;

  case MvnNode::kSll:
    dump_comb(s, node, BinaryOpDumper("<<"));
    break;

  case MvnNode::kSrl:
    dump_comb(s, node, BinaryOpDumper(">>"));
    break;

  case MvnNode::kSla:
    dump_comb(s, node, BinaryOpDumper("<<<"));
    break;

  case MvnNode::kSra:
    dump_comb(s, node, BinaryOpDumper(">>>"));
    break;

  case MvnNode::kAdd:
    dump_comb(s, node, BinaryOpDumper("+"));
    break;

  case MvnNode::kSub:
    dump_comb(s, node, BinaryOpDumper("-"));
    break;

  case MvnNode::kMult:
    dump_comb(s, node, BinaryOpDumper("*"));
    break;

  case MvnNode::kDiv:
    dump_comb(s, node, BinaryOpDumper("/"));
    break;

  case MvnNode::kMod:
    dump_comb(s, node, BinaryOpDumper("%"));
    break;

  case MvnNode::kPow:
    dump_comb(s, node, BinaryOpDumper("**"));
    break;

  case MvnNode::kIte:
    dump_comb(s, node, TernaryOpDumper("?", ":"));
#if 0
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 3, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvnInputPin* ipin0 = node->input(0);
      const MvnOutputPin* src_pin0 = ipin0->src_pin();
      const MvnNode* src_node0 = src_pin0->node();

      const MvnInputPin* ipin1 = node->input(1);
      const MvnOutputPin* src_pin1 = ipin1->src_pin();
      const MvnNode* src_node1 = src_pin1->node();

      const MvnInputPin* ipin2 = node->input(2);
      const MvnOutputPin* src_pin2 = ipin2->src_pin();
      const MvnNode* src_node2 = src_pin2->node();

      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node0)
	<< " ? " << node_name(src_node1)
	<< " : " << node_name(src_node2)
	<< ";" << endl;
    }
#endif
    break;

  case MvnNode::kConcat:
    dump_comb(s, node, ConcatDumper());
#if 0
    {
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      s << "  assign " << node_name(node)
	<< " = {";
      const char* comma = "";
      ymuint ni = node->input_num();
      for (ymuint i = 0; i < ni; ++ i) {
	const MvnInputPin* ipin = node->input(i);
	const MvnOutputPin* opin = ipin->src_pin();
	const MvnNode* src_node = opin->node();
	s << comma << node_name(src_node);
	comma = ", ";
      }
      s << "};" << endl;
    }
#endif
    break;

  case MvnNode::kConstBitSelect:
    dump_comb(s, node, ConstBitSelectDumper());
#if 0
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 1, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvnInputPin* ipin = node->input(0);
      const MvnOutputPin* src_pin = ipin->src_pin();
      const MvnNode* src_node = src_pin->node();
      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node)
	<< "[" << node->bitpos() << "];" << endl;
    }
#endif
    break;

  case MvnNode::kConstPartSelect:
    dump_comb(s, node, ConstPartSelectDumper());
#if 0
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 1, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvnInputPin* ipin = node->input(0);
      const MvnOutputPin* src_pin = ipin->src_pin();
      const MvnNode* src_node = src_pin->node();
      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node)
	<< "[" << node->msb()
	<< ":" << node->lsb()
	<< "];" << endl;
    }
#endif
    break;

  case MvnNode::kBitSelect:
    dump_comb(s, node, BitSelectDumper());
#if 0
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 2, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvnInputPin* ipin = node->input(0);
      const MvnOutputPin* src_pin = ipin->src_pin();
      const MvnNode* src_node = src_pin->node();

      const MvnInputPin* ipin1 = node->input(1);
      const MvnOutputPin* src_pin1 = ipin1->src_pin();
      const MvnNode* src_node1 = src_pin1->node();

      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node)
	<< "[" << node_name(src_node1) << "];" << endl;
    }
#endif
    break;

  case MvnNode::kPartSelect:
    dump_comb(s, node, PartSelectDumper());
#if 0
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 2, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvnInputPin* ipin = node->input(0);
      const MvnOutputPin* src_pin = ipin->src_pin();
      const MvnNode* src_node = src_pin->node();

      const MvnInputPin* ipin1 = node->input(1);
      const MvnOutputPin* src_pin1 = ipin1->src_pin();
      const MvnNode* src_node1 = src_pin1->node();

      const MvnInputPin* ipin2 = node->input(2);
      const MvnOutputPin* src_pin2 = ipin2->src_pin();
      const MvnNode* src_node2 = src_pin2->node();

      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node)
	<< "[" << node_name(src_node1)
	<< ":" << node_name(src_node2)
	<< "];" << endl;
    }
#endif
    break;

  case MvnNode::kConst:
    dump_comb(s, node, ConstDumper());
#if 0
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 0, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      ymuint bw = node->output(0)->bit_width();
      s << "  assign " << node_name(node)
	<< " = " << bw << "'b";
      vector<ymuint32> cv;
      node->const_value(cv);
      for (ymuint i = 0; i < bw; ++ i) {
	ymuint idx = bw - i - 1;
	ymuint blk = idx / 32;
	ymuint sft = idx % 32;
	if ( (cv[blk] >> sft) & 1 ) {
	  s << "1";
	}
	else {
	  s << "0";
	}
      }
      s << ";" << endl;
    }
#endif
    break;

  case MvnNode::kCell:
    dump_cell(s, node);
#if 0
    {
      const Cell* cell = node->cell();
      ymuint np = cell->pin_num();
      for (ymuint i = 0; i < np; ++ i) {
	const CellPin* pin = cell->pin(i);
	ymuint pp = node->cell_pin_pos(i);
	ymuint pos = po >> 1;
	if ( pp & 1U ) {
	  const MvnOutputPin* opin = node->output(pos);
	}
	else {
	  const MvnInputPin* ipin = node->input(pos);
	  const MvnOutputPin* opin = ipin->src_pin();
	  const MvnNode* src_node = opin->node();
	}
      }
    }
#endif
    break;

  default:
    break;
  }
}

// @brief 組み合わせ回路用のノードの内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] node 対象のノード
// @param[in] dumper ノードの内容を出力するためのクラス
void
MvnVerilogWriterImpl::dump_comb(ostream& s,
				const MvnNode* node,
				const NodeDumper& dumper)
{
  // 出力ピン
  const MvnOutputPin* opin = node->output(0);

  s << "  assign " << net_name(opin) << " = ";
  dumper(s, node);
  s << ";" << endl;
}

// @brief D-FF 用のノードの内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] node 対象のノード
void
MvnVerilogWriterImpl::dump_dff(ostream& s,
			       const MvnNode* node)
{
  // ピン位置と属性は決め打ち
  ymuint ni = node->input_num();
  assert_cond( ni >= 2, __FILE__, __LINE__);
  ymuint no = node->output_num();
  assert_cond( no == 1, __FILE__, __LINE__);

  // データ入力
  const MvnInputPin* ipin0 = node->input(0);
  const MvnOutputPin* src_pin0 = ipin0->src_pin();
  assert_cond( src_pin0 != NULL, __FILE__, __LINE__);

  // クロック入力
  const MvnInputPin* ipin1 = node->input(1);
  const MvnOutputPin* src_pin1 = ipin1->src_pin();
  assert_cond( src_pin1 != NULL, __FILE__, __LINE__);

  // データ出力
  const MvnOutputPin* opin = node->output(0);

  s << "  always @ ( ";
  if ( node->clock_pol() == 1 ) {
    s << "posedge";
  }
  else {
    s << "negedge";
  }
  s << " " << net_name(src_pin1);
  ymuint nc = ni - 2;
  for (ymuint i = 0; i < nc; ++ i) {
    const MvnInputPin* ipin2 = node->input(i + 2);
    const MvnOutputPin* src_pin2 = ipin2->src_pin();
    const char* polstr = node->control_pol(i) ? "posedge" : "negedge";
    s << " or " << polstr << " " << net_name(src_pin2);
  }
  s << " ) begin" << endl;
  const char* elif = "if";
  for (ymuint i = 0; i < nc; ++ i) {
    const char* not_str = "";
    if ( node->control_pol(i) == 0 ) {
      not_str = "!";
    }
    const MvnInputPin* ipin2 = node->input(i + 2);
    const MvnOutputPin* src_pin2 = ipin2->src_pin();
    const MvnNode* src_node2 = src_pin2->node();
    const MvnNode* src_node3 = node->control_val(i);
    const MvnOutputPin* src_pin3 = src_node3->output(0);
    s << "    " << elif << " ( "
      << not_str << net_name(src_pin2) << " )" << endl
      << "      " << net_name(opin)
      << " <= " << net_name(src_pin3) << ";" << endl;
    elif = "else if";
  }
  if ( nc > 0 ) {
    s << "    else" << endl
      << "  ";
  }
  s << "    " << net_name(opin) << " <= "
    << net_name(src_pin0) << ";" << endl
    << "  end" << endl
    << endl;
}

// @brief ラッチ用のノードの内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] node 対象のノード
void
MvnVerilogWriterImpl::dump_latch(ostream& s,
				 const MvnNode* node)
{
  // ピン位置と属性は決め打ち
  ymuint ni = node->input_num();
  assert_cond( ni == 2, __FILE__, __LINE__);
  ymuint no = node->output_num();
  assert_cond( no == 1, __FILE__, __LINE__);

  // データ入力
  const MvnInputPin* ipin0 = node->input(0);
  const MvnOutputPin* src_pin0 = ipin0->src_pin();
  assert_cond( src_pin0 != NULL, __FILE__, __LINE__);

  // イネーブル入力
  const MvnInputPin* ipin1 = node->input(1);
  const MvnOutputPin* src_pin1 = ipin1->src_pin();
  assert_cond( src_pin1 != NULL, __FILE__, __LINE__);

  // データ出力
  const MvnOutputPin* opin = node->output(0);

  s << "  always @ ( * )" << endl
    << "    if ( " << net_name(src_pin1) << " )" << endl
    << "      " << net_name(opin) << " <= "
    << net_name(src_pin0) << ";" << endl
    << endl;
}

// @brief ノードを表す右辺式を出力する．
// @param[in] s 出力先のストリーム
// @param[in] node 対象のノード
void
MvnVerilogWriterImpl::dump_node_rhs(ostream& s,
				    const MvnNode* node)
{
  switch ( node->type() ) {
  case MvnNode::kInput:
  case MvnNode::kOutput:
  case MvnNode::kInout:
  case MvnNode::kDff:
  case MvnNode::kLatch:
    assert_not_reached(__FILE__, __LINE__);
    break;

  case MvnNode::kThrough:
    dump_unary_op(s, node, "");
    break;

  case MvnNode::kNot:
    dump_unary_op(s, node, "~");
    break;

  case MvnNode::kAnd:
    dump_nary_op(s, node, "&");
    break;

  case MvnNode::kOr:
    dump_nary_op(s, node, "|");
    break;

  case MvnNode::kXor:
    dump_nary_op(s, node, "^");
    break;

  case MvnNode::kRand:
    dump_unary_op(s, node, "&");
    break;

  case MvnNode::kRor:
    dump_nary_uop(s, node, "|");
    break;

  case MvnNode::kRxor:
    dump_uary_op(s, node, "^");
    break;

  case MvnNode::kEq:
    dump_binary_op(s, node, "==", true);
    break;

  case MvnNode::kLt:
    dump_binary_op(s, node, "<", true);
    break;

  case MvnNode::kCaseEq:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 2, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvnInputPin* ipin0 = node->input(0);
      const MvnOutputPin* src_pin0 = ipin0->src_pin();

      const MvnInputPin* ipin1 = node->input(1);
      const MvnOutputPin* src_pin1 = ipin1->src_pin();

      vector<ymuint32> xmask;
      node->xmask(xmask);
      ymuint bw = ipin0->bit_width();
      string mask_str;
      for (ymuint i = 0; i < bw; ++ i) {
	ymuint bitpos = bw - i - 1;
	ymuint blk = bitpos / 32;
	ymuint sft = bitpos % 32;
	if ( xmask[blk] & (1U << sft) ) {
	  mask_str += "?";
	}
	else {
	  mask_str += "0";
	}
      }
      s << "(" << net_name(src_pin0)
	<< " === " << net_name(src_pin1)
	<< " ^ " << bw << "'b" << mask_str
	<< ")";
    }
    break;

  case MvnNode::kSll:
    dump_binary_op(s, node, "<<");
    break;

  case MvnNode::kSrl:
    dump_binary_op(s, node, ">>");
    break;

  case MvnNode::kSla:
    dump_binary_op(s, node, "<<<");
    break;

  case MvnNode::kSra:
    dump_binary_op(s, node, ">>>");
    break;

  case MvnNode::kAdd:
    dump_binary_op(s, node, "+");
    break;

  case MvnNode::kSub:
    dump_binary_op(s, node, "-");
    break;

  case MvnNode::kMult:
    dump_binary_op(s, node, "*");
    break;

  case MvnNode::kDiv:
    dump_binary_op(s, node, "/");
    break;

  case MvnNode::kMod:
    dump_binary_op(s, node, "%");
    break;

  case MvnNode::kPow:
    dump_binary_op(s, node, "**");
    break;

  case MvnNode::kIte:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 3, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvnInputPin* ipin0 = node->input(0);
      const MvnOutputPin* src_pin0 = ipin0->src_pin();

      const MvnInputPin* ipin1 = node->input(1);
      const MvnOutputPin* src_pin1 = ipin1->src_pin();

      const MvnInputPin* ipin2 = node->input(2);
      const MvnOutputPin* src_pin2 = ipin2->src_pin();

      s << net_name(src_pin0)
	<< " ? " << net_name(src_pin1)
	<< " : " << net_name(src_pin2);
    }
    break;

  case MvnNode::kConcat:
    {
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      s << "{";
      const char* comma = "";
      ymuint ni = node->input_num();
      for (ymuint i = 0; i < ni; ++ i) {
	const MvnInputPin* ipin = node->input(i);
	const MvnOutputPin* opin = ipin->src_pin();
	s << comma << net_name(opin);
	comma = ", ";
      }
      s << "}";
    }
    break;

  case MvnNode::kConstBitSelect:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 1, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvnInputPin* ipin = node->input(0);
      const MvnOutputPin* src_pin = ipin->src_pin();
      s << net_name(src_pin)
	<< "[" << node->bitpos() << "]";
    }
    break;

  case MvnNode::kConstPartSelect:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 1, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvnInputPin* ipin = node->input(0);
      const MvnOutputPin* src_pin = ipin->src_pin();
      s << net_name(src_pin)
	<< "[" << node->msb() << ":" << node->lsb()<< "]";
    }
    break;

  case MvnNode::kBitSelect:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 2, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvnInputPin* ipin = node->input(0);
      const MvnOutputPin* src_pin = ipin->src_pin();

      const MvnInputPin* ipin1 = node->input(1);
      const MvnOutputPin* src_pin1 = ipin1->src_pin();

      s << net_name(src_pin)
	<< "[" << net_name(src_pin1) << "]";
    }
    break;

  case MvnNode::kPartSelect:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 2, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvnInputPin* ipin = node->input(0);
      const MvnOutputPin* src_pin = ipin->src_pin();

      const MvnInputPin* ipin1 = node->input(1);
      const MvnOutputPin* src_pin1 = ipin1->src_pin();

      const MvnInputPin* ipin2 = node->input(2);
      const MvnOutputPin* src_pin2 = ipin2->src_pin();

      s << net_name(src_pin)
	<< "[" << net_name(src_pin1)
	<< ":" << net_name(src_pin2)
	<< "]";
    }
    break;

  case MvnNode::kConst:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 0, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      ymuint bw = node->output(0)->bit_width();
      s << bw << "'b";
      vector<ymuint32> cv;
      node->const_value(cv);
      for (ymuint i = 0; i < bw; ++ i) {
	ymuint idx = bw - i - 1;
	ymuint blk = idx / 32;
	ymuint sft = idx % 32;
	if ( (cv[blk] >> sft) & 1 ) {
	  s << "1";
	}
	else {
	  s << "0";
	}
      }
    }
    break;

  case MvnNode::kCell:
    {
#if 0
      const Cell* cell = node->cell();
      ymuint np = cell->pin_num();
      for (ymuint i = 0; i < np; ++ i) {
	const CellPin* pin = cell->pin(i);
	ymuint pp = node->cell_pin_pos(i);
	ymuint pos = po >> 1;
	if ( pp & 1U ) {
	  const MvnOutputPin* opin = node->output(pos);
	}
	else {
	  const MvnInputPin* ipin = node->input(pos);
	  const MvnOutputPin* opin = ipin->src_pin();
	  const MvnNode* src_node = opin->node();
	}
      }
#endif
    }
    break;

  default:
    break;
  }
}

// @brief ノードIDの最大値を設定する．
// @param[in] max_id ノードIDの最大値
void
MvnVerilogWriterImpl::set_max_id(ymuint max_id)
{
  mNextId = 0U;
  mMap.clear();
  mMap.resize(max_id, vector<ymuint32>(0));
}

// @brief 出力ピンに対応するネットIDを返す．
// @param[in] pin 出力ピン
ymuint
MvnVerilogWriterimpl::net_id(const MvnOutputPin* pin)
{
  const MvnNode* node = pin->node();
  ymuint onum = node->output_num();
  ymuint pos = pin->pos();
  vector<ymuint32>& tmp = mMap[node->id()];
  if ( tmp.size() == 0 ) {
    tmp.resize(onum);
    for (ymuint i = 0; i < onum; ++ i) {
      tmp[i] = mNextId;
      ++ mNextId;
    }
  }
  else {
    assert_cond( tmp.size() == onum, __FILE__, __LINE__);
  }
  return tmp[pos];
}

// @brief 出力ピンに対応するネット名を返す．
// @param[in] pin 出力ピン
string
MvnVerilogWriterImpl::net_name(const MvnOutputPin* pin)
{
  ostringstream buf;
  buf << "net" << net_id(opin);
  return buf.str();
}


BEGIN_NONAMESPACE

void
dump_port_ref(ostream& s,
	      const MvnPortRef* port_ref)
{
  s << node_name(port_ref->node());
  if ( port_ref->has_bitselect() ) {
    s << "[" << port_ref->bitpos() << "]";
  }
  else if ( port_ref->has_partselect() ) {
    s << "[" << port_ref->msb() << ":" << port_ref->lsb() << "]";
  }
}

void
dump_port(ostream& s,
	  const MvnPort* port)
{
  s << "." << port->name() << "(";
  ymuint n = port->port_ref_num();
  if ( n == 1 ) {
    const MvnPortRef* port_ref = port->port_ref(0);
    dump_port_ref(s, port_ref);
  }
  else if ( n > 1 ) {
    s << "{";
    const char* comma = "";
    for (ymuint i = 0; i < n; ++ i) {
      s << comma;
      comma = ", ";
      const MvnPortRef* port_ref = port->port_ref(i);
      dump_port_ref(s, port_ref);
    }
    s << "}";
  }
  s << ")";
}

void
dump_uop(ostream& s,
	 const MvnNode* node,
	 const char* opr_str)
{
  ymuint ni = node->input_num();
  assert_cond( ni == 1, __FILE__, __LINE__);
  ymuint no = node->output_num();
  assert_cond( no == 1, __FILE__, __LINE__);

  const MvnInputPin* ipin = node->input(0);
  const MvnOutputPin* src_pin = ipin->src_pin();
  const MvnNode* src_node = src_pin->node();
  s << "  assign " << node_name(node)
    << " = " << opr_str << node_name(src_node)
    << ";" << endl;
}

void
dump_binop(ostream& s,
	   const MvnNode* node,
	   const char* opr_str,
	   bool need_paren = false)
{
  ymuint ni = node->input_num();
  assert_cond( ni == 2, __FILE__, __LINE__);
  ymuint no = node->output_num();
  assert_cond( no == 1, __FILE__, __LINE__);

  const MvnInputPin* ipin0 = node->input(0);
  const MvnOutputPin* src_pin0 = ipin0->src_pin();
  const MvnNode* src_node0 = src_pin0->node();

  const MvnInputPin* ipin1 = node->input(1);
  const MvnOutputPin* src_pin1 = ipin1->src_pin();
  const MvnNode* src_node1 = src_pin1->node();

  const char* lp = "";
  const char* rp = "";
  if ( need_paren ) {
    lp = "(";
    rp = ")";
  }

  s << "  assign " << node_name(node) << " = "
    << lp << node_name(src_node0)
    << " " << opr_str << " " << node_name(src_node1)
    << rp << ";" << endl;
}

void
dump_nop(ostream& s,
	 const MvnNode* node,
	 const char* opr_str)
{
  ymuint ni = node->input_num();
  assert_cond( ni >= 2, __FILE__, __LINE__);
  ymuint no = node->output_num();
  assert_cond( no == 1, __FILE__, __LINE__);

  const MvnInputPin* ipin0 = node->input(0);
  const MvnOutputPin* src_pin0 = ipin0->src_pin();
  const MvnNode* src_node0 = src_pin0->node();

  s << "  assign " << node_name(node)
    << " = " << node_name(src_node0);
  for (ymuint i = 1; i < ni; ++ i) {
    const MvnInputPin* ipin1 = node->input(i);
    const MvnOutputPin* src_pin1 = ipin1->src_pin();
    const MvnNode* src_node1 = src_pin1->node();
    s << " " << opr_str << " " << node_name(src_node1);
  }
  s << ";" << endl;
}

void
dump_node(ostream& s,
	  const MvnNode* node)
{
  switch ( node->type() ) {
  case MvnNode::kInput:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 0, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);
    }
    break;

  case MvnNode::kOutput:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 1, __FILE__, __LINE__);

      const MvnInputPin* ipin = node->input(0);
      const MvnOutputPin* src_pin = ipin->src_pin();
      if ( src_pin ) {
	const MvnNode* src_node = src_pin->node();
	s << "  assign " << node_name(node)
	  << " = " << node_name(src_node)
	  << ";" << endl;
      }
    }
    break;

  case MvnNode::kInout:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 1, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvnInputPin* ipin = node->input(0);
      const MvnOutputPin* src_pin = ipin->src_pin();
      if ( src_pin ) {
	const MvnNode* src_node = src_pin->node();
	s << "  assign " << node_name(node)
	  << " = " << node_name(src_node)
	  << ";" << endl;
      }
    }
    break;

  case MvnNode::kDff:
    dump_dff(s, node);
    break;

  case MvnNode::kLatch:
    dump_latch(s, node);
    break;

  case MvnNode::kThrough:
    dump_uop(s, node, "");
    break;

  case MvnNode::kNot:
    dump_uop(s, node, "~");
    break;

  case MvnNode::kAnd:
    dump_nop(s, node, "&");
    break;

  case MvnNode::kOr:
    dump_nop(s, node, "|");
    break;

  case MvnNode::kXor:
    dump_nop(s, node, "^");
    break;

  case MvnNode::kRand:
    dump_uop(s, node, "&");
    break;

  case MvnNode::kRor:
    dump_uop(s, node, "|");
    break;

  case MvnNode::kRxor:
    dump_uop(s, node, "^");
    break;

  case MvnNode::kEq:
    dump_binop(s, node, "==", true);
    break;

  case MvnNode::kLt:
    dump_binop(s, node, "<", true);
    break;

  case MvnNode::kCaseEq:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 2, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvnInputPin* ipin0 = node->input(0);
      const MvnOutputPin* src_pin0 = ipin0->src_pin();
      const MvnNode* src_node0 = src_pin0->node();

      const MvnInputPin* ipin1 = node->input(1);
      const MvnOutputPin* src_pin1 = ipin1->src_pin();
      const MvnNode* src_node1 = src_pin1->node();

      vector<ymuint32> xmask;
      node->xmask(xmask);
      ymuint bw = ipin0->bit_width();
      string mask_str;
      for (ymuint i = 0; i < bw; ++ i) {
	ymuint bitpos = bw - i - 1;
	ymuint blk = bitpos / 32;
	ymuint sft = bitpos % 32;
	if ( xmask[blk] & (1U << sft) ) {
	  mask_str += "?";
	}
	else {
	  mask_str += "0";
	}
      }
      s << "  assign " << node_name(node)
	<< " = (" << node_name(src_node0)
	<< " === " << node_name(src_node1)
	<< " ^ " << bw << "'b" << mask_str
	<< ");" << endl;
    }
    break;

  case MvnNode::kSll:
    dump_binop(s, node, "<<");
    break;

  case MvnNode::kSrl:
    dump_binop(s, node, ">>");
    break;

  case MvnNode::kSla:
    dump_binop(s, node, "<<<");
    break;

  case MvnNode::kSra:
    dump_binop(s, node, ">>>");
    break;

  case MvnNode::kAdd:
    dump_binop(s, node, "+");
    break;

  case MvnNode::kSub:
    dump_binop(s, node, "-");
    break;

  case MvnNode::kMult:
    dump_binop(s, node, "*");
    break;

  case MvnNode::kDiv:
    dump_binop(s, node, "/");
    break;

  case MvnNode::kMod:
    dump_binop(s, node, "%");
    break;

  case MvnNode::kPow:
    dump_binop(s, node, "**");
    break;

  case MvnNode::kIte:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 3, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvnInputPin* ipin0 = node->input(0);
      const MvnOutputPin* src_pin0 = ipin0->src_pin();
      const MvnNode* src_node0 = src_pin0->node();

      const MvnInputPin* ipin1 = node->input(1);
      const MvnOutputPin* src_pin1 = ipin1->src_pin();
      const MvnNode* src_node1 = src_pin1->node();

      const MvnInputPin* ipin2 = node->input(2);
      const MvnOutputPin* src_pin2 = ipin2->src_pin();
      const MvnNode* src_node2 = src_pin2->node();

      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node0)
	<< " ? " << node_name(src_node1)
	<< " : " << node_name(src_node2)
	<< ";" << endl;
    }
    break;

  case MvnNode::kConcat:
    {
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      s << "  assign " << node_name(node)
	<< " = {";
      const char* comma = "";
      ymuint ni = node->input_num();
      for (ymuint i = 0; i < ni; ++ i) {
	const MvnInputPin* ipin = node->input(i);
	const MvnOutputPin* opin = ipin->src_pin();
	const MvnNode* src_node = opin->node();
	s << comma << node_name(src_node);
	comma = ", ";
      }
      s << "};" << endl;
    }
    break;

  case MvnNode::kConstBitSelect:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 1, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvnInputPin* ipin = node->input(0);
      const MvnOutputPin* src_pin = ipin->src_pin();
      const MvnNode* src_node = src_pin->node();
      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node)
	<< "[" << node->bitpos() << "];" << endl;
    }
    break;

  case MvnNode::kConstPartSelect:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 1, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvnInputPin* ipin = node->input(0);
      const MvnOutputPin* src_pin = ipin->src_pin();
      const MvnNode* src_node = src_pin->node();
      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node)
	<< "[" << node->msb()
	<< ":" << node->lsb()
	<< "];" << endl;
    }
    break;

  case MvnNode::kBitSelect:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 2, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvnInputPin* ipin = node->input(0);
      const MvnOutputPin* src_pin = ipin->src_pin();
      const MvnNode* src_node = src_pin->node();

      const MvnInputPin* ipin1 = node->input(1);
      const MvnOutputPin* src_pin1 = ipin1->src_pin();
      const MvnNode* src_node1 = src_pin1->node();

      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node)
	<< "[" << node_name(src_node1) << "];" << endl;
    }
    break;

  case MvnNode::kPartSelect:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 2, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvnInputPin* ipin = node->input(0);
      const MvnOutputPin* src_pin = ipin->src_pin();
      const MvnNode* src_node = src_pin->node();

      const MvnInputPin* ipin1 = node->input(1);
      const MvnOutputPin* src_pin1 = ipin1->src_pin();
      const MvnNode* src_node1 = src_pin1->node();

      const MvnInputPin* ipin2 = node->input(2);
      const MvnOutputPin* src_pin2 = ipin2->src_pin();
      const MvnNode* src_node2 = src_pin2->node();

      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node)
	<< "[" << node_name(src_node1)
	<< ":" << node_name(src_node2)
	<< "];" << endl;
    }
    break;

  case MvnNode::kConst:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 0, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      ymuint bw = node->output(0)->bit_width();
      s << "  assign " << node_name(node)
	<< " = " << bw << "'b";
      vector<ymuint32> cv;
      node->const_value(cv);
      for (ymuint i = 0; i < bw; ++ i) {
	ymuint idx = bw - i - 1;
	ymuint blk = idx / 32;
	ymuint sft = idx % 32;
	if ( (cv[blk] >> sft) & 1 ) {
	  s << "1";
	}
	else {
	  s << "0";
	}
      }
      s << ";" << endl;
    }
    break;

  case MvnNode::kCell:
    {
#if 0
      const Cell* cell = node->cell();
      ymuint np = cell->pin_num();
      for (ymuint i = 0; i < np; ++ i) {
	const CellPin* pin = cell->pin(i);
	ymuint pp = node->cell_pin_pos(i);
	ymuint pos = po >> 1;
	if ( pp & 1U ) {
	  const MvnOutputPin* opin = node->output(pos);
	}
	else {
	  const MvnInputPin* ipin = node->input(pos);
	  const MvnOutputPin* opin = ipin->src_pin();
	  const MvnNode* src_node = opin->node();
	}
      }
#endif
    }
    break;

  default:
    break;
  }
}

END_NONAMESPACE

END_NAMESPACE_YM_NETWORKS_MVN
