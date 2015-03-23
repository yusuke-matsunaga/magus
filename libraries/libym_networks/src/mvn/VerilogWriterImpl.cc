
/// @file VerilogWriterImpl.cc
/// @brief VerilogWriterImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
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

#include "VerilogWriterImpl.h"

#include "YmNetworks/MvnMgr.h"
#include "YmNetworks/MvnModule.h"
#include "YmNetworks/MvnPort.h"
#include "YmNetworks/MvnNode.h"
#include "YmNetworks/MvnInputPin.h"

#include "YmNetworks/MvnVlMap.h"

#include "YmCell/Cell.h"
#include "YmCell/CellPin.h"

#include "YmVerilog/vl/VlDecl.h"
#include "YmVerilog/vl/VlDeclArray.h"
#include "YmVerilog/vl/VlRange.h"


BEGIN_NAMESPACE_YM_NETWORKS_MVN

//////////////////////////////////////////////////////////////////////
// クラス VerilogWriterImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VerilogWriterImpl::VerilogWriterImpl()
{
}

// @brief デストラクタ
VerilogWriterImpl::~VerilogWriterImpl()
{
}

// @brief 内容を Verilog-HDL 形式で出力する
// @param[in] s 出力先のストリーム
// @param[in] mgr MvnMgr
void
VerilogWriterImpl::dump(ostream& s,
			const MvnMgr& mgr)
{
  mNameArray.clear();
  mNameArray.resize(mgr.max_node_id(), string());
  ymuint n = mgr.max_module_id();
  for (ymuint i = 0; i < n; ++ i) {
    const MvnModule* module = mgr.module(i);
    if ( module == NULL ) continue;

    dump_module(s, module, mgr);
  }
}

// @brief 内容を Verilog-HDL 形式で出力する
// @param[in] s 出力先のストリーム
// @param[in] mgr MvnMgr
// @param[in] node_map ノードと Verilog 名の対応表
void
VerilogWriterImpl::dump(ostream& s,
			const MvnMgr& mgr,
			const MvnVlMap& node_map)
{
  dump(s, mgr);

  ymuint node_num = mgr.max_node_id();
  for (ymuint i = 0; i < node_num; ++ i) {
    const MvnNode* node = mgr.node(i);
    if ( node == NULL ) continue;

    s << "// node" << node->id() << " : ";
    if ( node_map.is_single_elem(i) ) {
      const VlDecl* decl = node_map.get_single_elem(i);
      ASSERT_COND( decl != NULL );
      s << decl->full_name();
    }
    else if ( node_map.is_array_elem(i) ) {
      const VlDeclArray* declarray = node_map.get_array_elem(i);
      ASSERT_COND( declarray != NULL );
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

void
VerilogWriterImpl::dump_module(ostream& s,
			       const MvnModule* module,
			       const MvnMgr& mgr)
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
    ymuint bw = node->bit_width();
    ASSERT_COND( bw > 0 );
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
    ASSERT_COND( bw > 0 );
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
    ASSERT_COND( bw > 0 );
    if ( bw == 1 ) {
      s << "  inout ";
    }
    else {
      s << "  inout [" << bw - 1 << ":0] ";
    }
    s << node_name(node) << ";" << endl;
  }
  s << endl;

  const MvnNodeList& node_list = module->node_list();
  for (MvnNodeList::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    MvnNode* node = *p;
    ymuint bw = node->bit_width();
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
  for (MvnNodeList::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    MvnNode* node = *p;
    dump_node(s, node);
  }

  s << "endmodule" << endl
    << endl;
}

void
VerilogWriterImpl::dump_port(ostream& s,
			     const MvnPort* port)
{
  string port_name = port->name();
  ymuint n = port->port_ref_num();
  if ( n == 1 ) {
    const MvnPortRef* port_ref = port->port_ref(0);
    if ( !port_ref->has_bitselect() && !port_ref->has_partselect() ) {
      s << port_name;
      set_node_name(port_ref->node(), port_name);
      return;
    }
  }

  s << "." << port_name << "(";
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
VerilogWriterImpl::dump_port_ref(ostream& s,
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
VerilogWriterImpl::dump_node(ostream& s,
			     const MvnNode* node)
{
  switch ( node->type() ) {
  case MvnNode::kInput:
    {
      ymuint ni = node->input_num();
      ASSERT_COND( ni == 0 );
    }
    break;

  case MvnNode::kOutput:
    {
      ymuint ni = node->input_num();
      ASSERT_COND( ni == 1 );

      const MvnInputPin* ipin = node->input(0);
      const MvnNode* src_node = ipin->src_node();
      if ( src_node ) {
	s << "  assign " << node_name(node)
	  << " = " << node_name(src_node)
	  << ";" << endl;
      }
    }
    break;

  case MvnNode::kInout:
    {
      ymuint ni = node->input_num();
      ASSERT_COND( ni == 1 );

      const MvnInputPin* ipin = node->input(0);
      const MvnNode* src_node = ipin->src_node();
      if ( src_node ) {
	s << "  assign " << node_name(node)
	  << " = " << node_name(src_node)
	  << ";" << endl;
      }
    }
    break;

  case MvnNode::kDff:
    { // ピン位置と属性は決め打ち
      ymuint ni = node->input_num();
      ASSERT_COND( ni >= 2 );

      const MvnInputPin* ipin0 = node->input(0);
      const MvnNode* src_node0 = ipin0->src_node();
      ASSERT_COND( src_node0 != NULL );
      const MvnInputPin* ipin1 = node->input(1);
      const MvnNode* src_node1 = ipin1->src_node();
      ASSERT_COND( src_node1 != NULL );

      s << "  always @ ( ";
      if ( node->clock_pol() == 1 ) {
	s << "posedge";
      }
      else {
	s << "negedge";
      }
      s << " " << node_name(src_node1);
      ymuint nc = ni - 2;
      for (ymuint i = 0; i < nc; ++ i) {
	const MvnInputPin* ipin2 = node->input(i + 2);
	const MvnNode* src_node2 = ipin2->src_node();
	const char* polstr = node->control_pol(i) ? "posedge" : "negedge";
	s << " or " << polstr << " " << node_name(src_node2);
      }
      s << " )" << endl;
      const char* elif = "if";
      for (ymuint i = 0; i < nc; ++ i) {
	const char* not_str = "";
	if ( node->control_pol(i) == 0 ) {
	  not_str = "!";
	}
	const MvnInputPin* ipin2 = node->input(i + 2);
	const MvnNode* src_node2 = ipin2->src_node();
	const MvnNode* src_node3 = node->control_val(i);
	s << "    " << elif << " ( "
	  << not_str << node_name(src_node2) << " )" << endl
	  << "      " << node_name(node) << " <= "
	  << node_name(src_node3) << ";" << endl;
	elif = "else if";
      }
      if ( nc > 0 ) {
	s << "    else" << endl
	  << "  ";
      }
      s << "    " << node_name(node) << " <= "
	<< node_name(src_node0) << ";" << endl
	<< endl;
    }
    break;

  case MvnNode::kLatch:
    { // ピン位置と属性は決め打ち
      ymuint ni = node->input_num();
      ASSERT_COND( ni == 2 );

      const MvnInputPin* ipin0 = node->input(0);
      const MvnNode* src_node0 = ipin0->src_node();
       ASSERT_COND( src_node0 != NULL );
      const MvnInputPin* ipin1 = node->input(1);
      const MvnNode* src_node1 = ipin1->src_node();
      ASSERT_COND( src_node1 != NULL );

      s << "  always @ ( * )" << endl
	<< "    if ( " << node_name(src_node1) << " )" << endl
	<< "      " << node_name(node) << " <= "
	<< node_name(src_node0) << ";" << endl
	<< endl;
    }
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
      ASSERT_COND( ni == 2 );

      const MvnInputPin* ipin0 = node->input(0);
      const MvnNode* src_node0 = ipin0->src_node();

      const MvnInputPin* ipin1 = node->input(1);
      const MvnNode* src_node1 = ipin1->src_node();

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
      ASSERT_COND( ni == 3 );

      const MvnInputPin* ipin0 = node->input(0);
      const MvnNode* src_node0 = ipin0->src_node();

      const MvnInputPin* ipin1 = node->input(1);
      const MvnNode* src_node1 = ipin1->src_node();

      const MvnInputPin* ipin2 = node->input(2);
      const MvnNode* src_node2 = ipin2->src_node();

      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node0)
	<< " ? " << node_name(src_node1)
	<< " : " << node_name(src_node2)
	<< ";" << endl;
    }
    break;

  case MvnNode::kConcat:
    {
      s << "  assign " << node_name(node)
	<< " = {";
      const char* comma = "";
      ymuint ni = node->input_num();
      for (ymuint i = 0; i < ni; ++ i) {
	const MvnInputPin* ipin = node->input(i);
	const MvnNode* src_node = ipin->src_node();
	s << comma << node_name(src_node);
	comma = ", ";
      }
      s << "};" << endl;
    }
    break;

  case MvnNode::kConstBitSelect:
    {
      ymuint ni = node->input_num();
      ASSERT_COND( ni == 1 );

      const MvnInputPin* ipin = node->input(0);
      const MvnNode* src_node = ipin->src_node();
      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node)
	<< "[" << node->bitpos() << "];" << endl;
    }
    break;

  case MvnNode::kConstPartSelect:
    {
      ymuint ni = node->input_num();
      ASSERT_COND( ni == 1 );

      const MvnInputPin* ipin = node->input(0);
      const MvnNode* src_node = ipin->src_node();
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
      ASSERT_COND( ni == 2 );

      const MvnInputPin* ipin = node->input(0);
      const MvnNode* src_node = ipin->src_node();

      const MvnInputPin* ipin1 = node->input(1);
      const MvnNode* src_node1 = ipin1->src_node();

      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node)
	<< "[" << node_name(src_node1) << "];" << endl;
    }
    break;

  case MvnNode::kPartSelect:
    {
      ymuint ni = node->input_num();
      ASSERT_COND( ni == 2 );

      const MvnInputPin* ipin = node->input(0);
      const MvnNode* src_node = ipin->src_node();

      const MvnInputPin* ipin1 = node->input(1);
      const MvnNode* src_node1 = ipin1->src_node();

      const MvnInputPin* ipin2 = node->input(2);
      const MvnNode* src_node2 = ipin2->src_node();

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
      ASSERT_COND( ni == 0 );

      ymuint bw = node->bit_width();
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
      const Cell* cell = node->cell();
      ymuint ni = cell->input_num();
      ymuint no = cell->output_num();
      ymuint nio = cell->inout_num();
      ASSERT_COND( no == 1 );
      ASSERT_COND( nio == 0 );

      s << cell->name() << " " << node_name(node)
	<< " (";

      // 出力
      s << "." << cell->output(0)->name()
	<< "(" << node_name(node) << ")";

      // 入力
      for (ymuint i = 0; i < ni; ++ i) {
	const CellPin* pin = cell->input(i);
	const MvnInputPin* ipin = node->input(i);
	const MvnNode* src_node = ipin->src_node();

	s << ", ." << pin->name()
	  << "(" << node_name(src_node) << ")";
      }

      s << ");" << endl;
    }
    break;

  default:
    break;
  }
}

void
VerilogWriterImpl::dump_uop(ostream& s,
			    const MvnNode* node,
			    const char* opr_str)
{
  ymuint ni = node->input_num();
  ASSERT_COND( ni == 1 );

  const MvnInputPin* ipin = node->input(0);
  const MvnNode* src_node = ipin->src_node();
  s << "  assign " << node_name(node)
    << " = " << opr_str << node_name(src_node)
    << ";" << endl;
}

void
VerilogWriterImpl::dump_binop(ostream& s,
			      const MvnNode* node,
			      const char* opr_str,
			      bool need_paren)
{
  ymuint ni = node->input_num();
  ASSERT_COND( ni == 2 );

  const MvnInputPin* ipin0 = node->input(0);
  const MvnNode* src_node0 = ipin0->src_node();

  const MvnInputPin* ipin1 = node->input(1);
  const MvnNode* src_node1 = ipin1->src_node();

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
VerilogWriterImpl::dump_nop(ostream& s,
			    const MvnNode* node,
			    const char* opr_str)
{
  ymuint ni = node->input_num();
  ASSERT_COND( ni >= 2 );

  const MvnInputPin* ipin0 = node->input(0);
  const MvnNode* src_node0 = ipin0->src_node();

  s << "  assign " << node_name(node)
    << " = " << node_name(src_node0);
  for (ymuint i = 1; i < ni; ++ i) {
    const MvnInputPin* ipin1 = node->input(i);
    const MvnNode* src_node1 = ipin1->src_node();
    s << " " << opr_str << " " << node_name(src_node1);
  }
  s << ";" << endl;
}

string
VerilogWriterImpl::node_name(const MvnNode* node)
{
  string name = mNameArray[node->id()];
  if ( name == string() ) {
    ostringstream buf;
    buf << "node" << node->id();
    name = buf.str();
  }
  return name;
}

void
VerilogWriterImpl::set_node_name(const MvnNode* node,
				 const string& name)
{
  mNameArray[node->id()] = name;
}

END_NAMESPACE_YM_NETWORKS_MVN
