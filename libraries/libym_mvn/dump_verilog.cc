
/// @file libym_mvn/dump.cc
/// @brief dump() の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvMgr.h"

#include "ym_mvn/MvModule.h"
#include "ym_mvn/MvPort.h"
#include "ym_mvn/MvNode.h"
#include "ym_mvn/MvPin.h"


BEGIN_NAMESPACE_YM_MVN

BEGIN_NONAMESPACE

string
node_name(const MvNode* node)
{
  ostringstream buf;
  buf << "node" << node->id();
  return buf.str();
}

void
dump_port_ref(ostream& s,
	      const MvPortRef* port_ref)
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
	  const MvPort* port)
{
  s << "." << port->name() << "(";
  ymuint n = port->port_ref_num();
  if ( n == 1 ) {
    const MvPortRef* port_ref = port->port_ref(0);
    dump_port_ref(s, port_ref);
  }
  else if ( n > 1 ) {
    s << "{";
    const char* comma = "";
    for (ymuint i = 0; i < n; ++ i) {
      s << comma;
      comma = ", ";
      const MvPortRef* port_ref = port->port_ref(i);
      dump_port_ref(s, port_ref);
    }
    s << "}";
  }
  s << ")";
}

void
dump_node(ostream& s,
	  const MvNode* node)
{
  switch ( node->type() ) {
  case MvNode::kInput:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 0, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);
    }
    break;

  case MvNode::kOutput:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 1, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 0, __FILE__, __LINE__);

      const MvInputPin* ipin = node->input(0);
      const MvOutputPin* src_pin = ipin->src_pin();
      const MvNode* src_node = src_pin->node();
      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node)
	<< ";" << endl;
    }
    break;

  case MvNode::kDff:
    { // ピン位置と属性は決め打ち
      ymuint ni = node->input_num();
      assert_cond( ni >= 2, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvInputPin* ipin0 = node->input(0);
      const MvOutputPin* src_pin0 = ipin0->src_pin();
      assert_cond( src_pin0 != NULL, __FILE__, __LINE__);
      const MvNode* src_node0 = src_pin0->node();
      const MvInputPin* ipin1 = node->input(1);
      const MvOutputPin* src_pin1 = ipin1->src_pin();
      assert_cond( src_pin1 != NULL, __FILE__, __LINE__);
      const MvNode* src_node1 = src_pin1->node();

      s << "  always @ ( posedge " << node_name(src_node1);
      ymuint nc = (ni - 2) / 2;
      for (ymuint i = 0; i < nc; ++ i) {
	ymuint base = (i * 2) + 2;
	const MvInputPin* ipin2 = node->input(base);
	const MvOutputPin* src_pin2 = ipin2->src_pin();
	const MvNode* src_node2 = src_pin2->node();
	const char* polstr = node->control_pol(i) ? "posedge" : "negedge";
	s << " or " << polstr << " " << node_name(src_node2);
      }
      s << " )" << endl;
      const char* elif = "if";
      for (ymuint i = 0; i < nc; ++ i) {
	ymuint base = (i * 2) + 2;
	const MvInputPin* ipin2 = node->input(base);
	const MvOutputPin* src_pin2 = ipin2->src_pin();
	const MvNode* src_node2 = src_pin2->node();
	const MvInputPin* ipin3 = node->input(base + 1);
	const MvOutputPin* src_pin3 = ipin3->src_pin();
	const MvNode* src_node3 = src_pin3->node();
	s << "    " << elif << " ( " << node_name(src_node2) << " )" << endl
	  << "      " << node_name(node) << " <= "
	  << node_name(src_node3) << endl;
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

  case MvNode::kLatch:
    { // ピン位置と属性は決め打ち
      ymuint ni = node->input_num();
      assert_cond( ni == 2, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvInputPin* ipin0 = node->input(0);
      const MvOutputPin* src_pin0 = ipin0->src_pin();
      assert_cond( src_pin0 != NULL, __FILE__, __LINE__);
      const MvNode* src_node0 = src_pin0->node();
      const MvInputPin* ipin1 = node->input(1);
      const MvOutputPin* src_pin1 = ipin1->src_pin();
      assert_cond( src_pin1 != NULL, __FILE__, __LINE__);
      const MvNode* src_node1 = src_pin1->node();

      s << "  always @ ( * )" << endl
	<< "    if ( " << node_name(src_node1) << " )" << endl
	<< "      " << node_name(node) << " <= "
	<< node_name(src_node0) << ";" << endl
	<< endl;
    }
    break;

  case MvNode::kThrough:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 1, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvInputPin* ipin = node->input(0);
      const MvOutputPin* src_pin = ipin->src_pin();
      if ( src_pin ) {
	const MvNode* src_node = src_pin->node();
	s << "  assign " << node_name(node)
	  << " = " << node_name(src_node)
	  << ";" << endl;
      }
    }
    break;

  case MvNode::kNot:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 1, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvInputPin* ipin = node->input(0);
      const MvOutputPin* src_pin = ipin->src_pin();
      const MvNode* src_node = src_pin->node();
      s << "  assign " << node_name(node)
	<< " = ~" << node_name(src_node)
	<< ";" << endl;
    }
    break;

  case MvNode::kAnd:
    {
      ymuint ni = node->input_num();
      assert_cond( ni >= 2, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvInputPin* ipin0 = node->input(0);
      const MvOutputPin* src_pin0 = ipin0->src_pin();
      const MvNode* src_node0 = src_pin0->node();

      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node0);
      for (ymuint i = 1; i < ni; ++ i) {
	const MvInputPin* ipin1 = node->input(i);
	const MvOutputPin* src_pin1 = ipin1->src_pin();
	const MvNode* src_node1 = src_pin1->node();
	s << " & " << node_name(src_node1);
      }
      s << ";" << endl;
    }
    break;

  case MvNode::kOr:
    {
      ymuint ni = node->input_num();
      assert_cond( ni >= 2, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvInputPin* ipin0 = node->input(0);
      const MvOutputPin* src_pin0 = ipin0->src_pin();
      const MvNode* src_node0 = src_pin0->node();

      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node0);
      for (ymuint i = 1; i < ni; ++ i) {
	const MvInputPin* ipin1 = node->input(i);
	const MvOutputPin* src_pin1 = ipin1->src_pin();
	const MvNode* src_node1 = src_pin1->node();
	s << " | " << node_name(src_node1);
      }
      s << ";" << endl;
    }
    break;

  case MvNode::kXor:
    {
      ymuint ni = node->input_num();
      assert_cond( ni >= 2, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvInputPin* ipin0 = node->input(0);
      const MvOutputPin* src_pin0 = ipin0->src_pin();
      const MvNode* src_node0 = src_pin0->node();

      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node0);
      for (ymuint i = 1; i < ni; ++ i) {
	const MvInputPin* ipin1 = node->input(i);
	const MvOutputPin* src_pin1 = ipin1->src_pin();
	const MvNode* src_node1 = src_pin1->node();
	s << " ^ " << node_name(src_node1);
      }
      s << ";" << endl;
    }
    break;

  case MvNode::kRand:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 1, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvInputPin* ipin = node->input(0);
      const MvOutputPin* src_pin = ipin->src_pin();
      const MvNode* src_node = src_pin->node();
      s << "  assign " << node_name(node)
	<< " = &" << node_name(src_node)
	<< ";" << endl;
    }
    break;

  case MvNode::kRor:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 1, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvInputPin* ipin = node->input(0);
      const MvOutputPin* src_pin = ipin->src_pin();
      const MvNode* src_node = src_pin->node();
      s << "  assign " << node_name(node)
	<< " = |" << node_name(src_node)
	<< ";" << endl;
    }
    break;

  case MvNode::kRxor:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 1, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvInputPin* ipin = node->input(0);
      const MvOutputPin* src_pin = ipin->src_pin();
      const MvNode* src_node = src_pin->node();
      s << "  assign " << node_name(node)
	<< " = ^" << node_name(src_node)
	<< ";" << endl;
    }
    break;

  case MvNode::kEq:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 2, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvInputPin* ipin0 = node->input(0);
      const MvOutputPin* src_pin0 = ipin0->src_pin();
      const MvNode* src_node0 = src_pin0->node();

      const MvInputPin* ipin1 = node->input(1);
      const MvOutputPin* src_pin1 = ipin1->src_pin();
      const MvNode* src_node1 = src_pin1->node();

      s << "  assign " << node_name(node)
	<< " = (" << node_name(src_node0)
	<< " == " << node_name(src_node1)
	<< ");" << endl;
    }
    break;

  case MvNode::kLt:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 2, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvInputPin* ipin0 = node->input(0);
      const MvOutputPin* src_pin0 = ipin0->src_pin();
      const MvNode* src_node0 = src_pin0->node();

      const MvInputPin* ipin1 = node->input(1);
      const MvOutputPin* src_pin1 = ipin1->src_pin();
      const MvNode* src_node1 = src_pin1->node();

      s << "  assign " << node_name(node)
	<< " = (" << node_name(src_node0)
	<< " < " << node_name(src_node1)
	<< ");" << endl;
    }
    break;

  case MvNode::kSll:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 2, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvInputPin* ipin0 = node->input(0);
      const MvOutputPin* src_pin0 = ipin0->src_pin();
      const MvNode* src_node0 = src_pin0->node();

      const MvInputPin* ipin1 = node->input(1);
      const MvOutputPin* src_pin1 = ipin1->src_pin();
      const MvNode* src_node1 = src_pin1->node();

      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node0)
	<< " << " << node_name(src_node1)
	<< ";" << endl;
    }
    break;

  case MvNode::kSrl:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 2, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvInputPin* ipin0 = node->input(0);
      const MvOutputPin* src_pin0 = ipin0->src_pin();
      const MvNode* src_node0 = src_pin0->node();

      const MvInputPin* ipin1 = node->input(1);
      const MvOutputPin* src_pin1 = ipin1->src_pin();
      const MvNode* src_node1 = src_pin1->node();

      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node0)
	<< " >> " << node_name(src_node1)
	<< ";" << endl;
    }
    break;

  case MvNode::kSla:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 2, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvInputPin* ipin0 = node->input(0);
      const MvOutputPin* src_pin0 = ipin0->src_pin();
      const MvNode* src_node0 = src_pin0->node();

      const MvInputPin* ipin1 = node->input(1);
      const MvOutputPin* src_pin1 = ipin1->src_pin();
      const MvNode* src_node1 = src_pin1->node();

      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node0)
	<< " <<< " << node_name(src_node1)
	<< ";" << endl;
    }
    break;

  case MvNode::kSra:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 2, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvInputPin* ipin0 = node->input(0);
      const MvOutputPin* src_pin0 = ipin0->src_pin();
      const MvNode* src_node0 = src_pin0->node();

      const MvInputPin* ipin1 = node->input(1);
      const MvOutputPin* src_pin1 = ipin1->src_pin();
      const MvNode* src_node1 = src_pin1->node();

      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node0)
	<< " >>> " << node_name(src_node1)
	<< ";" << endl;
    }
    break;

  case MvNode::kAdd:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 2, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvInputPin* ipin0 = node->input(0);
      const MvOutputPin* src_pin0 = ipin0->src_pin();
      const MvNode* src_node0 = src_pin0->node();

      const MvInputPin* ipin1 = node->input(1);
      const MvOutputPin* src_pin1 = ipin1->src_pin();
      const MvNode* src_node1 = src_pin1->node();

      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node0)
	<< " + " << node_name(src_node1)
	<< ";" << endl;
    }
    break;

  case MvNode::kSub:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 2, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvInputPin* ipin0 = node->input(0);
      const MvOutputPin* src_pin0 = ipin0->src_pin();
      const MvNode* src_node0 = src_pin0->node();

      const MvInputPin* ipin1 = node->input(1);
      const MvOutputPin* src_pin1 = ipin1->src_pin();
      const MvNode* src_node1 = src_pin1->node();

      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node0)
	<< " - " << node_name(src_node1)
	<< ";" << endl;
    }
    break;

  case MvNode::kMult:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 2, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvInputPin* ipin0 = node->input(0);
      const MvOutputPin* src_pin0 = ipin0->src_pin();
      const MvNode* src_node0 = src_pin0->node();

      const MvInputPin* ipin1 = node->input(1);
      const MvOutputPin* src_pin1 = ipin1->src_pin();
      const MvNode* src_node1 = src_pin1->node();

      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node0)
	<< " * " << node_name(src_node1)
	<< ";" << endl;
    }
    break;

  case MvNode::kDiv:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 2, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvInputPin* ipin0 = node->input(0);
      const MvOutputPin* src_pin0 = ipin0->src_pin();
      const MvNode* src_node0 = src_pin0->node();

      const MvInputPin* ipin1 = node->input(1);
      const MvOutputPin* src_pin1 = ipin1->src_pin();
      const MvNode* src_node1 = src_pin1->node();

      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node0)
	<< " / " << node_name(src_node1)
	<< ";" << endl;
    }
    break;

  case MvNode::kMod:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 2, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvInputPin* ipin0 = node->input(0);
      const MvOutputPin* src_pin0 = ipin0->src_pin();
      const MvNode* src_node0 = src_pin0->node();

      const MvInputPin* ipin1 = node->input(1);
      const MvOutputPin* src_pin1 = ipin1->src_pin();
      const MvNode* src_node1 = src_pin1->node();

      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node0)
	<< " % " << node_name(src_node1)
	<< ";" << endl;
    }
    break;

  case MvNode::kPow:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 2, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvInputPin* ipin0 = node->input(0);
      const MvOutputPin* src_pin0 = ipin0->src_pin();
      const MvNode* src_node0 = src_pin0->node();

      const MvInputPin* ipin1 = node->input(1);
      const MvOutputPin* src_pin1 = ipin1->src_pin();
      const MvNode* src_node1 = src_pin1->node();

      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node0)
	<< " ** " << node_name(src_node1)
	<< ";" << endl;
    }
    break;

  case MvNode::kIte:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 3, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvInputPin* ipin0 = node->input(0);
      const MvOutputPin* src_pin0 = ipin0->src_pin();
      const MvNode* src_node0 = src_pin0->node();

      const MvInputPin* ipin1 = node->input(1);
      const MvOutputPin* src_pin1 = ipin1->src_pin();
      const MvNode* src_node1 = src_pin1->node();

      const MvInputPin* ipin2 = node->input(2);
      const MvOutputPin* src_pin2 = ipin2->src_pin();
      const MvNode* src_node2 = src_pin2->node();

      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node0)
	<< " ? " << node_name(src_node1)
	<< " : " << node_name(src_node2)
	<< ";" << endl;
    }
    break;

  case MvNode::kConcat:
    {
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      s << "  assign " << node_name(node)
	<< " = {";
      const char* comma = "";
      ymuint ni = node->input_num();
      for (ymuint i = 0; i < ni; ++ i) {
	const MvInputPin* ipin = node->input(i);
	const MvOutputPin* opin = ipin->src_pin();
	const MvNode* src_node = opin->node();
	s << comma << node_name(src_node);
	comma = ", ";
      }
      s << "};" << endl;
    }
    break;

  case MvNode::kConstBitSelect:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 1, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvInputPin* ipin = node->input(0);
      const MvOutputPin* src_pin = ipin->src_pin();
      const MvNode* src_node = src_pin->node();
      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node)
	<< "[" << node->bitpos() << "];" << endl;
    }
    break;

  case MvNode::kConstPartSelect:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 1, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvInputPin* ipin = node->input(0);
      const MvOutputPin* src_pin = ipin->src_pin();
      const MvNode* src_node = src_pin->node();
      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node)
	<< "[" << node->msb()
	<< ":" << node->lsb()
	<< "];" << endl;
    }
    break;

  case MvNode::kBitSelect:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 2, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvInputPin* ipin = node->input(0);
      const MvOutputPin* src_pin = ipin->src_pin();
      const MvNode* src_node = src_pin->node();

      const MvInputPin* ipin1 = node->input(1);
      const MvOutputPin* src_pin1 = ipin1->src_pin();
      const MvNode* src_node1 = src_pin1->node();

      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node)
	<< "[" << node_name(src_node1) << "];" << endl;
    }
    break;

  case MvNode::kPartSelect:
    {
      ymuint ni = node->input_num();
      assert_cond( ni == 2, __FILE__, __LINE__);
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);

      const MvInputPin* ipin = node->input(0);
      const MvOutputPin* src_pin = ipin->src_pin();
      const MvNode* src_node = src_pin->node();

      const MvInputPin* ipin1 = node->input(1);
      const MvOutputPin* src_pin1 = ipin1->src_pin();
      const MvNode* src_node1 = src_pin1->node();

      const MvInputPin* ipin2 = node->input(2);
      const MvOutputPin* src_pin2 = ipin2->src_pin();
      const MvNode* src_node2 = src_pin2->node();

      s << "  assign " << node_name(node)
	<< " = " << node_name(src_node)
	<< "[" << node_name(src_node1)
	<< ":" << node_name(src_node2)
	<< "];" << endl;
    }
    break;

  case MvNode::kCombUdp:
    {
      ymuint no = node->output_num();
      assert_cond( no == 1, __FILE__, __LINE__);
      s << "  MVN_UDP" << node->id()
	<< " U" << node->id()
	<< " ("
	<< " " << node_name(node);
      ymuint ni = node->input_num();
      for (ymuint i = 0; i < ni; ++ i) {
	const MvInputPin* ipin = node->input(i);
	const MvOutputPin* src_pin = ipin->src_pin();
	const MvNode* src_node = src_pin->node();
	s << ", " << node_name(src_node);
      }
      s << ");" << endl;
    }
    break;

  case MvNode::kSeqUdp:
    break;

  case MvNode::kConst:
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
	ymuint blk = idx + 31 / 32;
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

  default:
    break;
  }
}

void
dump_module(ostream& s,
	    const MvModule* module,
	    const MvMgr& mgr)
{
  s << "module " << module->name() << "(";

  ymuint np = module->port_num();
  const char* comma = "";
  for (ymuint i = 0; i < np; ++ i) {
    s << comma;
    comma = ", ";
    const MvPort* port = module->port(i);
    dump_port(s, port);
  }

  s << ");" << endl;

  ymuint ni = module->input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const MvNode* node = module->input(i);
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
    const MvNode* node = module->output(i);
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
  s << endl;

  const list<MvNode*>& node_list = module->node_list();
  for (list<MvNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    MvNode* node = *p;
    assert_cond( node->output_num() == 1, __FILE__, __LINE__);
    ymuint bw = node->output(0)->bit_width();
    if ( node->type() == MvNode::kDff || node->type() == MvNode::kLatch ) {
      s << "  reg  " << node_name(node) << ";" << endl;
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

  for (ymuint i = 0; i < no; ++ i) {
    const MvNode* node = module->output(i);
    const MvInputPin* ipin = node->input(0);
    const MvOutputPin* src_pin = ipin->src_pin();
    const MvNode* src_node = src_pin->node();
    s << "  assign " << node_name(node)
      << " = " << node_name(src_node)
      << ";" << endl;
  }
  for (list<MvNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    MvNode* node = *p;
    dump_node(s, node);
  }

  s << "endmodule" << endl
    << endl;
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// グローバル関数
//////////////////////////////////////////////////////////////////////

// @brief 内容を Verilog-HDL 形式で出力する
// @param[in] s 出力先のストリーム
// @param[in] mgr MvMgr
void
dump_verilog(ostream& s,
	     const MvMgr& mgr)
{
  ymuint n = mgr.max_module_id();
  for (ymuint i = 0; i < n; ++ i) {
    const MvModule* module = mgr.module(i);
    if ( module == NULL ) continue;

    dump_module(s, module, mgr);
  }
}

END_NAMESPACE_YM_MVN
