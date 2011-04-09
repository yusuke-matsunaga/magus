
/// @file libym_bdn/BdnVerilogWriter.cc
/// @brief ブーリアンネットワークの内容を出力する関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BdnVerilogWriter.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bdn/BdnVerilogWriter.h"
#include "ym_bdn/BdNetwork.h"
#include "ym_bdn/BdnPort.h"
#include "ym_bdn/BdnNode.h"
#include "ym_bdn/BdnDff.h"
#include "ym_utils/NameMgr.h"


BEGIN_NAMESPACE_YM_BDN


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
    BdnNode* input = port->input(i);
    BdnNode* output = port->output(i);
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


// ブーリアンネットワークをblif形式で表示
void
BdnVerilogWriter::operator()(ostream& s,
			     const BdNetwork& network)
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

  for (ymuint i = 0; i < np; ++ i) {
    const BdnPort* port = network.port(i);
    ymuint n = port->bit_width();
    for (ymuint j = 0; j < n; ++ j) {
      BdnNode* input = port->input(j);
      BdnNode* output = port->output(j);
      if ( input ) {
	s << "  input " << node_name(input) << ";" << endl;
      }
      else if ( output ) {
	s << "  output " << node_name(output) << ";" << endl;
      }
    }
  }

  const BdnDffList& dff_list = network.dff_list();
  const BdnNodeList& lnode_list = network.lnode_list();

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

    const BdnNode* set = dff->set();
    const BdnNode* s_node = set->output_fanin();
    bool s_inv = set->output_fanin_inv();

    const BdnNode* reset = dff->reset();
    const BdnNode* r_node = reset->output_fanin();
    bool r_inv = reset->output_fanin_inv();

    s << "  always @ ( "
      << inv_to_edge(c_inv) << " " << node_name(c_node);
    if ( s_node ) {
      s << " or " << inv_to_edge(s_inv) << " " << node_name(s_node);
    }
    if ( r_node ) {
      s << " or " << inv_to_edge(r_inv) << " " << node_name(r_node);
    }
    s << ") begin" << endl;
    bool has_set_reset = false;
    const char* else_str = "";
    if ( s_node ) {
      s << "    if (" << inv_to_symbol(s_inv) << node_name(s_node)
	<< ")" << endl
	<< "      " << node_name(node) << " <= 1'b1;" << endl;
      has_set_reset = true;
      else_str = "else ";
    }
    if ( r_node ) {
      s << "    " << else_str << "if ("
	<< inv_to_symbol(r_inv) << node_name(r_node) << ")" << endl
	<< "      " << node_name(node) << " <= 1'b0;" << endl;
      has_set_reset = true;
      else_str = "else";
    }
    if ( has_set_reset ) {
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

  for (BdnNodeList::const_iterator p = lnode_list.begin();
       p != lnode_list.end(); ++ p) {
    const BdnNode* node = *p;
    const BdnNode* input0 = node->fanin0();
    const BdnNode* input1 = node->fanin1();
    string i0_str = node_name(input0);
    string i1_str = node_name(input1);
    const char* i0_inv = "";
    const char* i1_inv = "";
    const char* op_str = "&";
    switch ( node->fcode() ) {
    case 0x0: // 0000
    case 0x3: // 0011
    case 0x5: // 0101
    case 0xa: // 1010
    case 0xc: // 1100
    case 0xf: // 1111
      // 定数関数や1入力関数は現れないはず．
      assert_not_reached(__FILE__, __LINE__);
      break;

    case 0x1: // 0001 = ~x0 & ~x1
      i0_inv = "~";
      i1_inv = "~";
      op_str = "&";
      break;

    case 0x2: // 0010 =  x0 & ~x1
      i1_inv = "~";
      op_str = "&";
      break;

    case 0x4: // 0100 = ~x0 &  x1
      i0_inv = "~";
      op_str = "&";
      break;

    case 0x6: // 0110 =  x0 ^  x1
      op_str = "^";
      break;

    case 0x7: // 0111 = ~x0 | ~x1
      i0_inv = "~";
      i1_inv = "~";
      op_str = "|";
      break;

    case 0x8: // 1000 =  x0 &  x1
      op_str = "&";
      break;

    case 0x9: // 1001 = ~(x0 ^ x1)
      op_str = "~^";
      break;

    case 0xb: // 1011 =  x0 | ~x1
      i1_inv = "~";
      op_str = "|";
      break;

    case 0xd: // 1101 = ~x0 |  x1
      i0_inv = "~";
      op_str = "|";
      break;

    case 0xe: // 1110 =  x0 |  x1
      op_str = "|";
      break;
    }
    s << "  assign " << node_name(node) << " = "
      << i0_inv << i0_str
      << " " << op_str << " "
      << i1_inv << i1_str << ";" << endl;
  }
  s << "endmodule" << endl;
}

END_NAMESPACE_YM_BDN
