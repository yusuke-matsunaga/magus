
/// @file MvnDumper.cc
/// @brief MvnDumper の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/MvnDumper.h"

#include "YmNetworks/MvnMgr.h"

#include "YmNetworks/MvnModule.h"
#include "YmNetworks/MvnPort.h"
#include "YmNetworks/MvnNode.h"
#include "YmNetworks/MvnInputPin.h"

#include "YmCell/Cell.h"


BEGIN_NAMESPACE_YM_NETWORKS_MVN

BEGIN_NONAMESPACE

// ノードを表す文字列を返す．
string
node_idstr(const MvnNode* node)
{
  ostringstream buf;
  buf << "Node[" << node->id() << "]";
  return buf.str();
}

// MvnInputPin の内容を出力する．
void
dump_inputpin(ostream& s,
	      const MvnInputPin* pin,
	      const string& pin_name)
{
  s << "  " << pin_name
    << "(" << pin->bit_width() << ")" << endl;
  const MvnNode* onode = pin->src_node();
  if ( onode ) {
    s << "    <== Output@" << node_idstr(onode) << endl;
  }
}

// MvnInputPin の内容を出力する．
void
dump_inputpin(ostream& s,
	      const MvnInputPin* pin)
{
  ostringstream buf;
  buf << "InputPin#" << pin->pos();
  string pin_name = buf.str();
  dump_inputpin(s, pin, pin_name);
}

// MvnNode の内容を出力する．
void
dump_node(ostream& s,
	  const MvnNode* node)
{
  s << node_idstr(node) << " : ";
  switch ( node->type() ) {
  case MvnNode::kInput:      s << "Input"; break;
  case MvnNode::kInout:      s << "Inout"; break;
  case MvnNode::kOutput:     s << "Output"; break;
  case MvnNode::kDff:        s << "DFF"; break;
  case MvnNode::kLatch:      s << "Latch"; break;
  case MvnNode::kThrough:    s << "Through"; break;
  case MvnNode::kNot:        s << "Not"; break;
  case MvnNode::kAnd:        s << "And"; break;
  case MvnNode::kOr:         s << "Or"; break;
  case MvnNode::kXor:        s << "Xor"; break;
  case MvnNode::kRand:       s << "Rand"; break;
  case MvnNode::kRor:        s << "Ror"; break;
  case MvnNode::kRxor:       s << "Rxor"; break;
  case MvnNode::kEq:         s << "Eq"; break;
  case MvnNode::kLt:         s << "Lt"; break;
  case MvnNode::kCaseEq:
    {
      s << "CaseEq[";
      vector<ymuint32> xmask;
      node->xmask(xmask);
      ymuint bw = node->input(0)->bit_width();
      for (ymuint i = 0; i < bw; ++ i) {
	ymuint bitpos = bw - i - 1;
	ymuint blk = bitpos / 32;
	ymuint sft = bitpos % 32;
	if ( xmask[blk] & (1U << sft) ) {
	  s << "-";
	}
	else {
	  s << "1";
	}
      }
      s << "]";
    }
    break;
  case MvnNode::kSll:        s << "Sll"; break;
  case MvnNode::kSrl:        s << "Srl"; break;
  case MvnNode::kSla:        s << "Sla"; break;
  case MvnNode::kSra:        s << "Sra"; break;
  case MvnNode::kAdd:        s << "Add"; break;
  case MvnNode::kSub:        s << "Sub"; break;
  case MvnNode::kMult:       s << "Mult"; break;
  case MvnNode::kDiv:        s << "Div"; break;
  case MvnNode::kMod:        s << "Mod"; break;
  case MvnNode::kPow:        s << "Pow"; break;
  case MvnNode::kIte:        s << "Ite"; break;
  case MvnNode::kConcat:     s << "Concat"; break;
  case MvnNode::kConstBitSelect:
    s << "ConstBitSelect["
      << node->bitpos()
      << "]";
    break;
  case MvnNode::kConstPartSelect:
    s << "ConstPartSelect["
      << node->msb()
      << ":"
      << node->lsb()
      << "]";
    break;
  case MvnNode::kBitSelect:  s << "BitSelect"; break;
  case MvnNode::kPartSelect: s << "PartSelect"; break;
  case MvnNode::kConst:
    {
      s << "Const(";
      vector<ymuint32> val;
      node->const_value(val);
      ymuint n = val.size();
      s << hex;
      for (ymuint i = 0; i < n; ++ i) {
	s << " " << val[n - i - 1];
      }
      s << dec << ")";
    }
    break;
  case MvnNode::kCell:
    {
      s << "Cell(" << node->cell()->name() << ")";
    }
    break;
  default:
    assert_not_reached(__FILE__, __LINE__);
  }
  s << endl;

  if ( node->type() == MvnNode::kDff ) {
    const MvnInputPin* input = node->input(0);
    dump_inputpin(s, input, "DataInput");
    const MvnInputPin* clock = node->input(1);
    dump_inputpin(s, clock, "Clock");
    s << "    ";
    if ( node->clock_pol() ) {
      s << "posedge";
    }
    else {
      s << "negedge";
    }
    s << endl;
    ymuint ni = node->input_num();
    ymuint nc = ni - 2;
    for (ymuint i = 0; i < nc; ++ i) {
      const MvnInputPin* cpin = node->input(i + 2);
      ostringstream buf;
      buf << "Control#" << i;
      string pin_name = buf.str();
      dump_inputpin(s, cpin, pin_name);
      s << "    ";
      if ( node->control_pol(i) ) {
	s << "posedge";
      }
      else {
	s << "negedge";
      }
      s << endl;
      const MvnNode* dnode = node->control_val(i);
      s << "  Data#" << i << " <== " << node_idstr(dnode) << endl;
    }
  }
  else if ( node->type() == MvnNode::kLatch ) {
    #warning "TODO: 未完"
  }
  else {
    ymuint ni = node->input_num();
    for (ymuint i = 0; i < ni; ++ i) {
      const MvnInputPin* pin = node->input(i);
      dump_inputpin(s, pin);
    }
    s << "  Output(" << node->bit_width() << ")" << endl;
    const MvnInputPinList& fo_list = node->dst_pin_list();
    for (MvnInputPinList::const_iterator p = fo_list.begin();
	 p != fo_list.end(); ++ p) {
      const MvnInputPin* ipin = *p;
      s << "    ==> InputPin#" << ipin->pos()
	<< "@" << node_idstr(ipin->node()) << endl;
    }
  }
  s << endl;
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス MvnDumper
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MvnDumper::MvnDumper()
{
}

// @brief デストラクタ
MvnDumper::~MvnDumper()
{
}

// @brief 内容を出力する
// @param[in] s 出力先のストリーム
// @param[in] mgr MvnMgr
void
MvnDumper::operator()(ostream& s,
		      const MvnMgr& mgr)
{
  ymuint n = mgr.max_module_id();
  for (ymuint i = 0; i < n; ++ i) {
    const MvnModule* module = mgr.module(i);
    if ( module == NULL ) continue;

    s << "Module#" << module->id() << "(" << module->name() << ")" << endl;
    const MvnNode* pnode = module->parent();
    if ( pnode ) {
      s << "  parent node: Module#" << pnode->parent()->id()
	<< ":" << node_idstr(pnode) << endl;
    }
    else {
      s << "  toplevel module" << endl;
    }

    ymuint np = module->port_num();
    for (ymuint j = 0; j < np; ++ j) {
      const MvnPort* port = module->port(j);
      s << "  Port#" << j << "(" << port->name() << ")" << endl;
      ymuint n = port->port_ref_num();
      for (ymuint k = 0; k < n; ++ k) {
	const MvnPortRef* port_ref = port->port_ref(k);
	s << "    " << node_idstr(port_ref->node());
	if ( port_ref->has_bitselect() ) {
	  s << "[" << port_ref->bitpos() << "]";
	}
	else if ( port_ref->has_partselect() ) {
	  s << "[" << port_ref->msb() << ":" << port_ref->lsb() << "]";
	}
	s << endl;
      }
    }

    ymuint ni = module->input_num();
    for (ymuint j = 0; j < ni; ++ j) {
      dump_node(s, module->input(j));
    }
    ymuint no = module->output_num();
    for (ymuint j = 0;j < no; ++ j) {
      dump_node(s, module->output(j));
    }
    ymuint nio = module->inout_num();
    for (ymuint j = 0; j < nio; ++ j) {
      dump_node(s, module->inout(j));
    }
    for (MvnNodeList::const_iterator p = module->nodes_begin();
	 p != module->nodes_end(); ++ p) {
      MvnNode* node = *p;
      dump_node(s, node);
    }

    s << endl;
  }
}

END_NAMESPACE_YM_NETWORKS_MVN
