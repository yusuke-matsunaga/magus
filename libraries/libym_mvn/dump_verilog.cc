
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
#include "ym_mvn/MvNet.h"
#include "ym_mvn/MvPin.h"


BEGIN_NAMESPACE_YM_MVN

BEGIN_NONAMESPACE

void
dump_port_ref(ostream& s,
	      const MvPortRef* port_ref)
{
  s << "node" << port_ref->node()->id();
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
  s << port->name() << "(";
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
      const MvOutputPin* opin = node->output(0);
      const MvNetList& net_list = opin->net_list();
      for (MvNetList::const_iterator p = net_list.begin();
	   p != net_list.end(); ++ p) {
	const MvNet* net = *p;
	s << "assign net" << net->id() << " = node" << node->id()
	  << ";" << endl;
      }
    }
    break;

  case MvNode::kOutput:
    {
      const MvInputPin* ipin = node->input(0);
      const MvNet* net = ipin->net();
      s << "assign node" << node->id() << " = net" << net->id()
	<< ";" << endl;
    }
    break;

  case MvNode::kDff1:
    break;

  case MvNode::kDff2:
    break;

  case MvNode::kThrough:
    break;

  case MvNode::kNot:
    break;

  case MvNode::kAnd:
    break;

  case MvNode::kOr:
    break;

  case MvNode::kXor:
    break;

  case MvNode::kRand:
    break;

  case MvNode::kRor:
    break;

  case MvNode::kRxor:
    break;

  case MvNode::kEq:
    break;

  case MvNode::kLt:
    break;

  case MvNode::kSll:
    break;

  case MvNode::kSrl:
    break;

  case MvNode::kSla:
    break;

  case MvNode::kSra:
    break;

  case MvNode::kAdd:
    break;

  case MvNode::kSub:
    break;

  case MvNode::kMult:
    break;

  case MvNode::kDiv:
    break;

  case MvNode::kMod:
    break;

  case MvNode::kPow:
    break;

  case MvNode::kIte:
    break;

  case MvNode::kConcat:
    break;

  case MvNode::kConstBitSelect:
    break;

  case MvNode::kConstPartSelect:
    break;

  case MvNode::kBitSelect:
    break;

  case MvNode::kPartSelect:
    break;

  case MvNode::kCombUdp:
    break;

  case MvNode::kSeqUdp:
    break;

  case MvNode::kConst:
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
    s << "node" << node->id() << ";" << endl;
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
    s << "node" << node->id() << ";" << endl;
  }

  const list<MvNode*>& node_list = module->node_list();
  for (list<MvNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    MvNode* node = *p;
    dump_node(s, node);
  }
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
