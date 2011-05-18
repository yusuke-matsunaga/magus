
/// @file libym_networks/MvnDumper.cc
/// @brief MvnDumper の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/MvnDumper.h"

#include "ym_networks/MvnMgr.h"

#include "ym_networks/MvnModule.h"
#include "ym_networks/MvnPort.h"
#include "ym_networks/MvnNode.h"
#include "ym_networks/MvnInputPin.h"
#include "ym_networks/MvnOutputPin.h"

#include "ym_networks/MvnVlMap.h"

#include "ym_verilog/vl/VlDecl.h"
#include "ym_verilog/vl/VlDeclArray.h"
#include "ym_verilog/vl/VlRange.h"


BEGIN_NAMESPACE_YM_MVN

BEGIN_NONAMESPACE

// MvnInputPin の内容を出力する．
void
dump_inputpin(ostream& s,
	      const MvnInputPin* pin)
{
  s << "  InputPin#" << pin->pos()
    << "(" << pin->bit_width() << ")" << endl;
  const MvnOutputPin* opin = pin->src_pin();
  if ( opin ) {
    s << "    <== OutputPin#" << opin->pos()
      << "@node#" << opin->node()->id() << endl;
  }
}

// MvnOutputPin の内容を出力する．
void
dump_outputpin(ostream& s,
	       const MvnOutputPin* pin)
{
  s << "  OutputPin#" << pin->pos()
    << "(" << pin->bit_width() << ")" << endl;
  const MvnInputPinList& fo_list = pin->dst_pin_list();
  for (MvnInputPinList::const_iterator p = fo_list.begin();
       p != fo_list.end(); ++ p) {
    const MvnInputPin* ipin = *p;
    s << "    ==> InputPin#" << ipin->pos()
      << "@node#" << ipin->node()->id() << endl;
  }
}

// MvnNode の内容を出力する．
void
dump_node(ostream& s,
	  const MvnNode* node)
{
  s << "Node#" << node->id()
    << ":";
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
  case MvnNode::kCombUdp:    s << "Combinational UDP"; break;
  case MvnNode::kSeqUdp:     s << "Sequential UDP"; break;
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
  default:
    assert_not_reached(__FILE__, __LINE__);
  }
  s << endl;
  ymuint ni = node->input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const MvnInputPin* pin = node->input(i);
    dump_inputpin(s, pin);
  }
  ymuint no = node->output_num();
  for (ymuint i = 0; i < no; ++ i) {
    const MvnOutputPin* pin = node->output(i);
    dump_outputpin(s, pin);
  }
  if ( node->type() == MvnNode::kDff ) {
    ymuint nc = (ni - 2) / 2;
    for (ymuint i = 0; i < nc; ++ i) {
      s << "  Control#" << i << "(InputPin#" << (i * 2) + 2 << ")" << endl
	<< "    ";
      if ( node->control_pol(i) ) {
	s << "posedge";
      }
      else {
	s << "negedge";
      }
      s << endl;
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
	<< ":" << pnode->id() << endl;
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
	s << "    node#" << port_ref->node()->id();
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
    for (list<MvnNode*>::const_iterator p = module->nodes_begin();
	 p != module->nodes_end(); ++ p) {
      MvnNode* node = *p;
      dump_node(s, node);
    }

    s << endl;
  }
}

END_NAMESPACE_YM_MVN
