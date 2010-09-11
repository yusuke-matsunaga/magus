
/// @file libym_lutmap/SbjGraph_dump.cc
/// @brief SbjGraph 用の dump() の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SbjGraph.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_lutmap/SbjGraph.h"


BEGIN_NAMESPACE_YM_LUTMAP

// @brief SbjGraph の内容をダンプする関数
void
dump(ostream& s,
     const SbjGraph& sbjgraph)
{
  ymuint np = sbjgraph.port_num();
  for (ymuint i = 0; i < np; ++ i) {
    s << "Port#" << i << ":";
    const SbjPort* port = sbjgraph.port(i);
    s << " " << port->name() << " = ";
    ymuint nb = port->bit_width();
    assert_cond( nb > 0, __FILE__, __LINE__);
    if ( nb == 1 ) {
      const SbjNode* node = port->bit(0);
      s << " " << node->id_str();
    }
    else {
      s << "{";
      const char* comma = "";
      for (ymuint i = 0; i < nb; ++ i) {
	ymuint idx = nb - i - 1;
	const SbjNode* node = port->bit(idx);
	s << comma << node->id_str();
	comma = ", ";
      }
      s << "}";
    }
    s << endl;
  }
  s << endl;

  const SbjNodeList& input_list = sbjgraph.input_list();
  for (SbjNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const SbjNode* node = *p;
    s << "Input#" << node->subid() << ": " << node->id_str()
      << " : " << sbjgraph.port(node)->name()
      << "[" << sbjgraph.port_pos(node) << "]"
      << endl;
  }

  const SbjNodeList& output_list = sbjgraph.output_list();
  for (SbjNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const SbjNode* node = *p;
    const SbjEdge* e = node->fanin_edge(0);
    s << "Output#" << node->subid() << ": " << node->id_str()
      << " : " << sbjgraph.port(node)->name()
      << "[" << sbjgraph.port_pos(node) << "]"
      << " = ";
    const SbjNode* inode = e->from();
    if ( inode ) {
      // 普通のノードの場合
      if ( node->output_inv() ) {
	s << "~";
      }
      s << inode->id_str();
    }
    else {
      // 定数ノードの場合
      if ( node->output_inv() ) {
	s << "1";
      }
      else {
	s << "0";
      }
    }
    s << endl;
  }

  const SbjNodeList& dff_list = sbjgraph.dff_list();
  for (SbjNodeList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const SbjNode* node = *p;
    s << "DFF(" << node->id_str() << ") :";
    s << "DATA = ";
    const SbjNode* inode = node->fanin_data();
    if ( inode ) {
      // 普通のノードの場合
      if ( node->fanin_data_inv() ) {
	s << "~";
      }
      s << inode->id_str();
    }
    else {
      // 定数ノードの場合
      if ( node->fanin_data_inv() ) {
	s << "1";
      }
      else {
	s << "0";
      }
    }

    const SbjNode* cnode = node->fanin_clock();
    if ( cnode ) {
      s << ", CLOCK = ";
      if ( node->fanin_clock_inv() ) {
	s << "~";
      }
      s << cnode->id_str();
    }

    const SbjNode* snode = node->fanin_set();
    if ( snode ) {
      s << ", SET = ";
      if ( node->fanin_set_inv() ) {
	s << "~";
      }
      s << snode->id_str();
    }

    const SbjNode* rnode = node->fanin_rst();
    if ( rnode ) {
      s << ", RST = ";
      if ( node->fanin_rst_inv() ) {
	s << "~";
      }
      s << rnode->id_str();
    }

    s << endl;
  }

  const SbjNodeList& lnode_list = sbjgraph.lnode_list();
  for (SbjNodeList::const_iterator p = lnode_list.begin();
       p != lnode_list.end(); ++ p) {
    const SbjNode* node = *p;
    s << "Logic(" << node->id_str() << ") = [";
    ymuint fcode = node->fcode();
    s << ((fcode >> 3) & 1) << ((fcode >> 2) & 1)
      << ((fcode >> 1) & 1) << (fcode & 1) << "] ("
      << " " << node->fanin(0)->id_str()
      << ", " << node->fanin(1)->id_str()
      << ")" << endl;
    s << endl;
  }
}

// @brief SbjGraph の内容を blif 形式で出力する関数
void
dump_blif(ostream& s,
	  const SbjGraph& sbjgraph)
{
  s << ".model " << sbjgraph.name() << endl;
  const SbjNodeList& input_list = sbjgraph.input_list();
  for (SbjNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const SbjNode* node = *p;
    s << ".inputs " << node->id_str() << endl;
  }

  const SbjNodeList& output_list = sbjgraph.output_list();
  for (SbjNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const SbjNode* node = *p;
    s << ".outputs " << node->id_str() << endl;
  }
  for (SbjNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const SbjNode* node = *p;
    const SbjNode* inode = node->fanin(0);
    if ( inode == 0 ) {
      s << ".names " << node->id_str() << endl;
      if ( node->output_inv() ) {
	s << "1" << endl;
      }
      else {
	s << "0" << endl;
      }
    }
    else {
      s << ".names " << inode->id_str() << " " << node->id_str() << endl;
      if ( node->output_inv() ) {
	s << "0 1" << endl;
      }
      else {
	s << "1 1" << endl;
      }
    }
    s << endl;
  }

  const SbjNodeList& dff_list = sbjgraph.dff_list();
  for (SbjNodeList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const SbjNode* node = *p;
    const SbjNode* inode = node->fanin(0);
    s << ".latch " << node->id_str() << " "
      << inode->id_str() << endl;
  }

  const SbjNodeList& lnode_list = sbjgraph.lnode_list();
  for (SbjNodeList::const_iterator p = lnode_list.begin();
       p != lnode_list.end(); ++ p) {
    const SbjNode* node = *p;
    s << ".names " << node->fanin(0)->id_str()
      << " " << node->fanin(1)->id_str()
      << " " << node->id_str() << endl;
    switch ( node->fcode() ) {
    case 0x0: // 0000
    case 0x3: // 0011
    case 0x5: // 0101
    case 0xa: // 1010
    case 0xc: // 1100
    case 0xf: // 1111
      assert_not_reached(__FILE__, __LINE__);
      break;
    case 0x1: // 0001
      s << "00 1" << endl;
      break;
    case 0x2: // 0010
      s << "10 1" << endl;
      break;
    case 0x4: // 0100
      s << "01 1" << endl;
      break;
    case 0x6: // 0110
      s << "10 1" << endl
	<< "01 1" << endl;
      break;
    case 0x7: // 0111
      s << "0- 1" << endl
	<< "-0 1" << endl;
      break;
    case 0x8: // 1000
      s << "11 1" << endl;
      break;
    case 0x9: // 1001
      s << "00 1" << endl
	<< "11 1" << endl;
      break;
    case 0xb: // 1011
      s << "1- 1" << endl
	<< "-0 1" << endl;
      break;
    case 0xd: // 1101
      s << "0- 1" << endl
	<< "-1 1" << endl;
      break;
    case 0xe: // 1110
      s << "1- 1" << endl
	<< "-1 1" << endl;
      break;
    }
    s << endl;
  }
  s << ".end" << endl;
}


BEGIN_NONAMESPACE

// ノード名を返す．
string
node_name(const SbjNode* node)
{
  return "n" + node->id_str();
}

END_NONAMESPACE


// @brief SbjGraph の内容を Verilog-HDL 形式で出力する関数
void
dump_verilog(ostream& s,
	     const SbjGraph& sbjgraph)
{
  // module 文
  s << "module " << sbjgraph.name() << "(";
  ymuint np = sbjgraph.port_num();
  const char* sep = "";
  for (ymuint i = 0; i < np; ++ i) {
    const SbjPort* port = sbjgraph.port(i);
    s << sep << "." << port->name() << "(";
    ymuint nb = port->bit_width();
    assert_cond( nb > 0 , __FILE__, __LINE__);
    if ( nb == 1 ) {
      const SbjNode* node = port->bit(0);
      s << node_name(node);
    }
    else {
      s << "{";
      const char* comma = "";
      for (ymuint j = 0; j < nb; ++ j) {
	ymuint idx = nb - j - 1;
	const SbjNode* node = port->bit(idx);
	s << comma << node_name(node);
	comma = ", ";
      }
      s << "}";
    }
    s << ")";
    sep = ", ";
  }
  s << ");" << endl;

  const SbjNodeList& input_list = sbjgraph.input_list();
  const SbjNodeList& output_list = sbjgraph.output_list();
  const SbjNodeList& lnode_list = sbjgraph.lnode_list();

  // input 文
  for (SbjNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const SbjNode* node = *p;
    s << "  input  " << node_name(node) << ";" << endl;
  }

  // output 文
  for (SbjNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const SbjNode* node = *p;
    s << "  output " << node_name(node) << ";" << endl;
  }

  // wire 定義
  for (SbjNodeList::const_iterator p = lnode_list.begin();
       p != lnode_list.end(); ++ p) {
    const SbjNode* node = *p;
    s << "  wire   " << node_name(node) << ";" << endl;
  }

  // output 用の assign 文
  for (SbjNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const SbjNode* node = *p;
    const SbjNode* inode = node->fanin(0);
    s << "  assign " << node_name(node) << " = ";
    if ( inode == 0 ) {
      if ( node->output_inv() ) {
	s << "1'b1";
      }
      else {
	s << "1'b0";
      }
    }
    else {
      if ( node->output_inv() ) {
	s << "~";
      }
      s << node_name(inode);
    }
    s << ";" << endl;
  }

#if 0
  const SbjNodeList& dff_list = sbjgraph.dff_list();
  for (SbjNodeList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const SbjNode* node = *p;
    const SbjNode* inode = node->fanin(0);
    s << ".latch " << node->id_str() << " "
      << inode->id_str() << endl;
  }
#endif

  // 論理ノード用の assign 文
  for (SbjNodeList::const_iterator p = lnode_list.begin();
       p != lnode_list.end(); ++ p) {
    const SbjNode* node = *p;
    s << "  assign " << node_name(node) << " = ";

    const char* pol0 = "";
    const char* pol1 = "";
    const char* op   = "&";
    switch ( node->fcode() ) {
    case 0x0: // 0000
    case 0x3: // 0011
    case 0x5: // 0101
    case 0xa: // 1010
    case 0xc: // 1100
    case 0xf: // 1111
      assert_not_reached(__FILE__, __LINE__);
      break;

    case 0x1: // 0001
      pol0 = "~";
      pol1 = "~";
      break;

    case 0x2: // 0010
      pol1 = "~";
      break;

    case 0x4: // 0100
      pol0 = "~";
      break;

    case 0x6: // 0110
      op   = "^";
      break;

    case 0x7: // 0111
      pol0 = "~";
      pol1 = "~";
      op   = "|";
      break;

    case 0x8: // 1000
      break;

    case 0x9: // 1001
      pol0 = "~";
      op   = "^";
      break;

    case 0xb: // 1011
      pol1 = "~";
      op   = "|";
      break;

    case 0xd: // 1101
      pol0 = "~";
      op   = "|";
      break;

    case 0xe: // 1110
      op   = "|";
      break;
    }
    s << pol0 << node_name(node->fanin(0))
      << " " << op << " "
      << pol1 << node_name(node->fanin(1))
      << ";" << endl;
  }
  s << "endmodule" << endl;
}

END_NAMESPACE_YM_LUTMAP
