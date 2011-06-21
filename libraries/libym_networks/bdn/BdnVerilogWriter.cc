
/// @file libym_networks/BdnVerilogWriter.cc
/// @brief ブーリアンネットワークの内容を出力する関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BdnVerilogWriter.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/BdnVerilogWriter.h"
#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnPort.h"
#include "ym_networks/BdnNode.h"
#include "ym_networks/BdnDff.h"
#include "ym_utils/NameMgr.h"


BEGIN_NAMESPACE_YM_NETWORKS


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
node_name(const BdnNode* node)
{
  if ( node ) {
    ostringstream buf;
    buf << "N" << node->id();
    return buf.str();
  }
  else {
    return "1'b0";
  }
}

const char*
inv_to_edge(bool inv)
{
  if ( inv ) {
    return "negedge";
  }
  else {
    return "posedge";
  }
}

const char*
inv_to_symbol(bool inv)
{
  if ( inv ) {
    return "!";
  }
  else {
    return "";
  }
}

void
dump_port(ostream& s,
	  const BdnPort* port)
{
  s << "." << verilog_name(port->name()) << "(";
  ymuint n = port->bit_width();
  const char* comma = "";
  for (ymuint i = 0; i < n; ++ i) {
    const BdnNode* input = port->input(i);
    const BdnNode* output = port->output(i);
    s << comma;
    comma = ", ";
    if ( input ) {
      s << node_name(input);
    }
    else if ( output ) {
      s << node_name(output);
    }
    else {
      assert_not_reached(__FILE__, __LINE__);
    }
  }
  s << ")";
}

END_NONAMESPACE


// ブーリアンネットワークを Verilog 形式で表示
void
BdnVerilogWriter::operator()(ostream& s,
			     const BdnMgr& network)
{
  s << "module " << verilog_name(network.name()) << "(";
  ymuint np = network.port_num();
  const char* comma = "";
  for (ymuint i = 0; i < np; ++ i) {
    s << comma;
    comma = ", ";
    const BdnPort* port = network.port(i);
    dump_port(s, port);
  }
  s << ");" << endl;

  const BdnNodeList& input_list = network.input_list();
  const BdnNodeList& output_list = network.output_list();
  const BdnDffList& dff_list = network.dff_list();
  const BdnLatchList& latch_list = network.latch_list();
  const BdnNodeList& lnode_list = network.lnode_list();

  for (BdnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const BdnNode* node = *p;
    if ( node->alt_node() ) {
      s << "  inout";
    }
    else {
      s << "  input";
    }
    s << " " << node_name(node) << ";" << endl;
  }

  for (BdnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const BdnNode* node = *p;
    if ( node->alt_node() == NULL ) {
      s << "  output " << node_name(node) << ";" << endl;
    }
  }

  for (BdnNodeList::const_iterator p = lnode_list.begin();
       p != lnode_list.end(); ++ p) {
    const BdnNode* node = *p;
    s << "  wire " << node_name(node) << ";" << endl;
  }

  for (BdnDffList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const BdnDff* dff = *p;
    const BdnNode* node = dff->output();
    s << "  reg " << node_name(node) << ";" << endl;
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
      << inv_to_edge(c_inv) << " " << node_name(c_node);
    if ( s_node ) {
      s << " or " << inv_to_edge(s_inv) << " " << node_name(s_node);
    }
    if ( r_node ) {
      s << " or " << inv_to_edge(r_inv) << " " << node_name(r_node);
    }
    s << ") begin" << endl;
    bool has_clear_preset = false;
    const char* else_str = "";
    if ( s_node ) {
      s << "    if (" << inv_to_symbol(s_inv) << node_name(s_node)
	<< ")" << endl
	<< "      " << node_name(node) << " <= 1'b1;" << endl;
      has_clear_preset = true;
      else_str = "else ";
    }
    if ( r_node ) {
      s << "    " << else_str << "if ("
	<< inv_to_symbol(r_inv) << node_name(r_node) << ")" << endl
	<< "      " << node_name(node) << " <= 1'b0;" << endl;
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
    s << node_name(node) << " <= "
      << inv_to_symbol(i_inv) << node_name(i_node) << ";" << endl
      << "  end" << endl;
  }

  for (BdnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const BdnNode* node = *p;
    if ( node->alt_node() ) {
      node = node->alt_node();
    }
    const BdnNode* input = node->fanin0();
    if ( input ) {
      string i_str = node_name(input);
      const char* i_inv = node->fanin0_inv() ? "~" : "";
      s << "  assign " << node_name(node) << " = "
	<< i_inv << i_str << ";" << endl;
    }
  }

  for (BdnNodeList::const_iterator p = lnode_list.begin();
       p != lnode_list.end(); ++ p) {
    const BdnNode* node = *p;
    const BdnNode* input0 = node->fanin0();
    const BdnNode* input1 = node->fanin1();
    string i0_str = node_name(input0);
    string i1_str = node_name(input1);
    const char* i0_inv = node->fanin0_inv() ? "~" : "";
    const char* i1_inv = node->fanin1_inv() ? "~" : "";
    const char* op_str = node->is_xor() ? "^" : "&";
    s << "  assign " << node_name(node) << " = "
      << i0_inv << i0_str
      << " " << op_str << " "
      << i1_inv << i1_str << ";" << endl;
  }
  s << "endmodule" << endl;
}

END_NAMESPACE_YM_NETWORKS
