
/// @file SbjDumper.cc
/// @brief SbjDumper の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "SbjDumper.h"
#include "SbjGraph.h"
#include "SbjPort.h"
#include "SbjDff.h"
#include "SbjNode.h"


BEGIN_NAMESPACE_YM_SBJ

// @brief コンストラクタ
SbjDumper::SbjDumper()
{
}

// @brief デストラクタ
SbjDumper::~SbjDumper()
{
}

/// @brief 独自形式で出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] sbjgraph 対象のサブジェクトグラフ
void
SbjDumper::dump(ostream& s,
		const SbjGraph& sbjgraph)
{
  ymuint np = sbjgraph.port_num();
  for (ymuint i = 0; i < np; ++ i) {
    s << "Port#" << i << ":";
    const SbjPort* port = sbjgraph.port(i);
    s << " " << port->name() << " = ";
    ymuint nb = port->bit_width();
    ASSERT_COND( nb > 0 );
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

  ymuint ni = sbjgraph.input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* node = sbjgraph.input(i);
    s << "Input#" << node->subid() << ": " << node->id_str()
      << " : " << sbjgraph.port(node)->name()
      << "[" << sbjgraph.port_pos(node) << "]"
      << endl;
  }

  ymuint no = sbjgraph.output_num();
  for (ymuint i = 0; i < no; ++ i) {
    const SbjNode* node = sbjgraph.output(i);
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

  ymuint nf = sbjgraph.dff_num();
  for (ymuint i = 0; i < nf; ++ i) {
    const SbjDff* dff = sbjgraph.dff(i);
    s << "DFF(" << dff->id() << ") :";

    const SbjNode* onode = dff->data_output();
    s << "Q = " << onode->id_str();

    const SbjNode* inode = dff->data_input();
    s << "DATA = " << inode->id_str();

    const SbjNode* cnode = dff->clock();
    if ( cnode ) {
      s << ", CLOCK = " << cnode->id_str();
    }

    const SbjNode* rnode = dff->clear();
    if ( rnode ) {
      s << ", CLEAR = " << rnode->id_str();
    }

    const SbjNode* snode = dff->preset();
    if ( snode ) {
      s << ", PRESET = " << snode->id_str();
    }
    s << endl;
  }

  ymuint nl = sbjgraph.lnode_num();
  for (ymuint i = 0; i < nl; ++ i) {
    const SbjNode* node = sbjgraph.lnode(i);
    const char* pol0 = node->fanin_inv(0) ? "~" : "";
    const char* pol1 = node->fanin_inv(1) ? "~" : "";
    const char* op   = node->is_xor() ? "^" : "&";
    s << "Logic(" << node->id_str() << ") = "
      << pol0 << node->fanin(0)->id_str()
      << " " << op << " "
      << pol1 << node->fanin(1)->id_str()
      << endl;
  }
}

// @brief blif 形式で出力する．
// @param[in] s 出力先のストリーム
// @param[in] sbjgraph 対象のサブジェクトグラフ
void
SbjDumper::dump_blif(ostream& s,
		     const SbjGraph& sbjgraph)
{
  s << ".model " << sbjgraph.name() << endl;
  ymuint ni = sbjgraph.input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* node = sbjgraph.input(i);
    s << ".inputs " << node->id_str() << endl;
  }

  ymuint no = sbjgraph.output_num();
  for (ymuint i = 0; i < no; ++ i) {
    const SbjNode* node = sbjgraph.output(i);
    s << ".outputs " << node->id_str() << endl;
  }
  for (ymuint i = 0; i < no; ++ i) {
    const SbjNode* node = sbjgraph.output(i);
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

  ymuint nf = sbjgraph.dff_num();
  for (ymuint i = 0; i < nf; ++ i) {
    const SbjDff* dff = sbjgraph.dff(i);
    const SbjNode* onode = dff->data_output();
    const SbjNode* inode = dff->data_input();
    s << ".latch " << onode->id_str() << " "
      << inode->id_str() << endl;
  }

  ymuint nl = sbjgraph.lnode_num();
  for (ymuint i = 0; i < nl; ++ i) {
    const SbjNode* node = sbjgraph.lnode(i);
    s << ".names " << node->fanin(0)->id_str()
      << " " << node->fanin(1)->id_str()
      << " " << node->id_str() << endl;
    if ( node->is_and() ) {
      if ( node->fanin_inv(0) ) {
	if ( node->fanin_inv(1) ) {
	  s << "00 1" << endl;
	}
	else {
	  s << "01 1" << endl;
	}
      }
      else {
	if ( node->fanin_inv(1) ) {
	  s << "10 1" << endl;
	}
	else {
	  s << "11 1" << endl;
	}
      }
    }
    else {
      s << "10 1" << endl
	<< "01 1" << endl;
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
  return node->id_str();
}

END_NONAMESPACE


// @brief Verilog-HDL 形式で出力する関数
// @param[in] s 出力先のストリーム
// @param[in] sbjgraph 対象のネットワーク
void
SbjDumper::dump_verilog(ostream& s,
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
    ASSERT_COND( nb > 0  );
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

  // input 文
  ymuint ni = sbjgraph.input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* node = sbjgraph.input(i);
    s << "  input  " << node_name(node) << ";" << endl;
  }
  s << endl;

  // output 文
  ymuint no = sbjgraph.output_num();
  for (ymuint i = 0; i < no; ++ i) {
    const SbjNode* node = sbjgraph.output(i);
    s << "  output " << node_name(node) << ";" << endl;
  }
  s << endl;

  // reg 定義
  ymuint nf = sbjgraph.dff_num();
  for (ymuint i = 0; i < nf; ++ i) {
    const SbjDff* dff = sbjgraph.dff(i);
    const SbjNode* node = dff->data_output();
    s << "  reg    " << node_name(node) << ";" << endl;
  }
  s << endl;

  // wire 定義
  ymuint nl = sbjgraph.lnode_num();
  for (ymuint i = 0; i < nl; ++ i) {
    const SbjNode* node = sbjgraph.lnode(i);
    s << "  wire   " << node_name(node) << ";" << endl;
  }
  s << endl;

  // output 用の assign 文
  for (ymuint i = 0; i < no; ++ i) {
    const SbjNode* node = sbjgraph.output(i);
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
  s << endl;

  // 論理ノード用の assign 文
  for (ymuint i = 0; i < nl; ++ i) {
    const SbjNode* node = sbjgraph.lnode(i);
    s << "  assign " << node_name(node) << " = ";

    const char* pol0 = node->fanin_inv(0) ? "~" : "";
    const char* pol1 = node->fanin_inv(1) ? "~" : "";
    const char* op   = node->is_xor() ? "^" : "&";
    s << pol0 << node_name(node->fanin(0))
      << " " << op << " "
      << pol1 << node_name(node->fanin(1))
      << ";" << endl;
  }
  s << endl;

  // ff 用の always 文
  for (ymuint i = 0; i < nf; ++ i) {
    const SbjDff* dff = sbjgraph.dff(i);
    const SbjNode* node = dff->data_output();
    const SbjNode* dnode = dff->data_input();
    const SbjNode* cnode = dff->clock();
    const SbjNode* snode = dff->preset();
    const SbjNode* rnode = dff->clear();
    ASSERT_COND( cnode != nullptr );
    s << "  always @ ( ";
    s << "posedge";
    s << " " << node_name(cnode);
    if ( snode ) {
      s << " or ";
      s << "posedge";
      s << " " << node_name(snode);
    }
    if ( rnode ) {
      s << " or ";
      s << "posedge";
      s << " " << node_name(rnode);
    }
    s << " )" << endl;
    if ( snode ) {
      s << "    if ( ";
      s << " " << node_name(snode) << " )" << endl;
      s << "      " << node_name(node)
	<< " <= 1;" << endl;
    }
    if ( rnode ) {
      s << "    ";
      if ( snode ) {
	s << "else ";
      }
      s << "if ( ";
      s << " " << node_name(rnode) << " )" << endl;
      s << "      " << node_name(node)
	<< " <= 0;" << endl;
    }
    if ( snode || rnode ) {
      s << "    else" << endl
	<< "  ";
    }
    s << "    " << node_name(node) << " <= ";
    s << node_name(dnode) << ";" << endl;
    s << endl;
  }
  s << "endmodule" << endl;
}

END_NAMESPACE_YM_SBJ
