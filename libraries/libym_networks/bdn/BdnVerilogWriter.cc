
/// @file BdnVerilogWriter.cc
/// @brief BdnVerilogWriter の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/BdnVerilogWriter.h"

#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnPort.h"
#include "ym_networks/BdnNode.h"
#include "ym_networks/BdnDff.h"
#include "ym_utils/NameMgr.h"

#include "../verilog/VlwModule.h"
#include "../verilog/VlwModuleHeader.h"
#include "../verilog/VlwIO.h"


BEGIN_NAMESPACE_YM_NETWORKS_BDN


// @brief コンストラクタ
BdnVerilogWriter::BdnVerilogWriter()
{
}

// @brief デストラクタ
BdnVerilogWriter::~BdnVerilogWriter()
{
}


BEGIN_NONAMESPACE

string
verilog_name(const string& name)
{
  ymuint n = name.size();
  bool need_escape = false;
  for (ymuint i = 0; i < n; ++ i) {
    int c = name[i];
    if ( i == 0 ) {
      if ( !isalpha(c) && c != '_' ) {
	need_escape = true;
	break;
      }
    }
    else {
      if ( !isalnum(c) && c != '_' ) {
	need_escape = true;
	break;
      }
    }
  }
  if ( need_escape ) {
    return "\\" + name + " ";
  }
  else {
    return name;
  }
}

string
node_name(const BdnNode* node,
	  bool inv)
{
  if ( node ) {
    ostringstream buf;
    if ( inv ) {
      buf << "!";
    }
    buf << "N" << node->id();
    return buf.str();
  }
  else if ( inv ) {
    return "1'b1";
  }
  else {
    return "1'b0";
  }
}

string
edge_name(const BdnNode* node,
	  bool inv)
{
  assert_cond( node != NULL, __FILE__, __LINE__);

  ostringstream buf;
  if ( inv ) {
    buf << "negedge";
  }
  else {
    buf << "posedge";
  }
  buf << " N" << node->id();
  return buf.str();
}

void
dump_port(ostream& s,
	  const BdnPort* port)
{
  s << "." << verilog_name(port->name()) << "(";
  ymuint n = port->bit_width();
  const char* rcb = "";
  if ( n > 1 ) {
    s << "{";
    rcb = "}";
  }
  const char* comma = "";
  for (ymuint i = 0; i < n; ++ i) {
    ymuint pos = n - i - 1;
    const BdnNode* input = port->input(pos);
    const BdnNode* output = port->output(pos);
    s << comma;
    comma = ", ";
    if ( input ) {
      s << node_name(input, false);
    }
    else if ( output ) {
      s << node_name(output, false);
    }
    else {
      assert_not_reached(__FILE__, __LINE__);
    }
  }
  s << rcb << ")";
}

END_NONAMESPACE


// ブーリアンネットワークを Verilog 形式で表示
void
BdnVerilogWriter::operator()(ostream& s,
			     const BdnMgr& network)
{
  VlWriter writer(s);

  VlwModule vlw_module(writer, network.name());

  {
    VlwModuleHeader vlw_module_header(writer);
    ymuint np = network.port_num();
    const char* comma = "";
    for (ymuint i = 0; i < np; ++ i) {
      s << comma;
      comma = ", ";
      const BdnPort* port = network.port(i);
      dump_port(s, port);
    }
  }

  const BdnNodeList& input_list = network.input_list();
  const BdnNodeList& output_list = network.output_list();
  const BdnDffList& dff_list = network.dff_list();
  const BdnLatchList& latch_list = network.latch_list();
  const BdnNodeList& lnode_list = network.lnode_list();

  for (BdnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const BdnNode* node = *p;
    if ( node->input_type() == BdnNode::kPRIMARY_INPUT ) {
      if ( node->alt_node() ) {
	VlwInout vlw_inout(writer);
	writer.put_elem(node_name(node, false));
      }
      else {
	VlwInput vlw_input(writer);
	writer.put_elem(node_name(node, false));
      }
    }
  }

  for (BdnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const BdnNode* node = *p;
    if ( node->output_type() == BdnNode::kPRIMARY_OUTPUT ) {
      if ( node->alt_node() == NULL ) {
	VlwOutput vlw_output(writer);
	writer.put_elem(node_name(node, false));
      }
    }
  }

  for (BdnNodeList::const_iterator p = lnode_list.begin();
       p != lnode_list.end(); ++ p) {
    const BdnNode* node = *p;
    VlwWire vlw_wire(writer);
    writer.put_elem(node_name(node, false));
  }

  for (BdnDffList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const BdnDff* dff = *p;
    const BdnNode* node = dff->output();
    VlwReg vlw_reg(writer);
    writer.put_elem(node_name(node, false));
  }

  for (BdnDffList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const BdnDff* dff = *p;
    const BdnNode* node = dff->output();

    const BdnNode* input = dff->input();
    const BdnNode* i_node = input->output_fanin();
    bool i_inv = input->output_fanin_inv();

    const BdnNode* clock = dff->clock();
    const BdnNode* c_node = clock->output_fanin();
    bool c_inv = clock->output_fanin_inv();

    const BdnNode* clear = dff->clear();
    const BdnNode* r_node = clear->output_fanin();
    bool r_inv = clear->output_fanin_inv();

    const BdnNode* preset = dff->preset();
    const BdnNode* s_node = preset->output_fanin();
    bool s_inv = preset->output_fanin_inv();

    s << "  always @ ( "
      << edge_name(c_node, c_inv);
    if ( s_node ) {
      s << " or " << edge_name(s_node, s_inv);
    }
    if ( r_node ) {
      s << " or " << edge_name(r_node, r_inv);
    }
    s << " ) begin" << endl;
    bool has_clear_preset = false;
    const char* else_str = "";
    if ( s_node ) {
      s << "    if (" << node_name(s_node, s_inv) << ")" << endl
	<< "      " << node_name(node, false) << " <= 1'b1;" << endl;
      has_clear_preset = true;
      else_str = "else ";
    }
    if ( r_node ) {
      s << "    " << else_str
	<< "if (" << node_name(r_node, r_inv) << ")" << endl
	<< "      " << node_name(node, false) << " <= 1'b0;" << endl;
      has_clear_preset = true;
      else_str = "else";
    }
    if ( has_clear_preset ) {
      s << "    else" << endl
	<< "      ";
    }
    else {
      s << "    ";
    }
    s << node_name(node, false) << " <= "
      << node_name(i_node, i_inv) << ";" << endl
      << "  end" << endl;
  }

  for (BdnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const BdnNode* node = *p;
    if ( node->output_type() != BdnNode::kPRIMARY_OUTPUT ) {
      continue;
    }
    if ( node->alt_node() ) {
      node = node->alt_node();
    }
    const BdnNode* input = node->output_fanin();
    bool i_inv = node->output_fanin_inv();
    s << "  assign " << node_name(node, false) << " = "
      << node_name(input, i_inv) << ";" << endl;
  }

  for (BdnNodeList::const_iterator p = lnode_list.begin();
       p != lnode_list.end(); ++ p) {
    const BdnNode* node = *p;
    const BdnNode* input0 = node->fanin0();
    const BdnNode* input1 = node->fanin1();
    bool i0_inv = node->fanin0_inv();
    bool i1_inv = node->fanin1_inv();
    const char* op_str = node->is_xor() ? "^" : "&";
    s << "  assign " << node_name(node, false) << " = "
      << node_name(input0, i0_inv)
      << " " << op_str << " "
      << node_name(input1, i1_inv)
      << ";" << endl;
  }
}

END_NAMESPACE_YM_NETWORKS_BDN
