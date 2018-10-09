
/// @file SbjDumper.cc
/// @brief SbjDumper の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2016, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "SbjDumper.h"
#include "SbjGraph.h"
#include "SbjPort.h"
#include "SbjDff.h"
#include "SbjLatch.h"
#include "SbjNode.h"


BEGIN_NAMESPACE_SBJ

/// @brief 独自形式で出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] sbjgraph 対象のサブジェクトグラフ
void
SbjDumper::dump(ostream& s,
		const SbjGraph& sbjgraph)
{
  int id = 0;
  for ( auto port: sbjgraph.port_list() ) {
    s << "Port#" <<id << ":";
    ++ id;
    s << " " << port->name() << " = ";
    int nb = port->bit_width();
    ASSERT_COND( nb > 0 );
    if ( nb == 1 ) {
      const SbjNode* node = port->bit(0);
      s << " " << node->id_str();
    }
    else {
      s << "{";
      const char* comma = "";
      for ( int j = 0; j < nb; ++ j ) {
	int idx = nb - j - 1;
	const SbjNode* node = port->bit(idx);
	s << comma << node->id_str();
	comma = ", ";
      }
      s << "}";
    }
    s << endl;
  }
  s << endl;

  for ( auto node: sbjgraph.input_list() ) {
    s << "Input#" << node->subid() << ": " << node->id_str()
      << " : " << sbjgraph.port(node)->name()
      << "[" << sbjgraph.port_pos(node) << "]"
      << endl;
  }

  for ( auto node: sbjgraph.output_list() ) {
    const SbjNode* inode = node->output_fanin();
    s << "Output#" << node->subid() << ": " << node->id_str()
      << " : " << sbjgraph.port(node)->name()
      << "[" << sbjgraph.port_pos(node) << "]"
      << " = ";
    if ( inode ) {
      // 普通のノードの場合
      if ( node->output_fanin_inv() ) {
	s << "~";
      }
      s << inode->id_str();
    }
    else {
      // 定数ノードの場合
      if ( node->output_fanin_inv() ) {
	s << "1";
      }
      else {
	s << "0";
      }
    }
    s << endl;
  }

  for ( auto dff: sbjgraph.dff_list() ) {
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

  for ( auto latch: sbjgraph.latch_list() ) {
    s << "LATCH(" << latch->id() << ") :";

    const SbjNode* onode = latch->data_output();
    s << "Q = " << onode->id_str();

    const SbjNode* inode = latch->data_input();
    s << "DATA = " << inode->id_str();

    const SbjNode* cnode = latch->enable();
    if ( cnode ) {
      s << ", ENABLE = " << cnode->id_str();
    }

    const SbjNode* rnode = latch->clear();
    if ( rnode ) {
      s << ", CLEAR = " << rnode->id_str();
    }

    const SbjNode* snode = latch->preset();
    if ( snode ) {
      s << ", PRESET = " << snode->id_str();
    }
    s << endl;
  }

  for ( auto node: sbjgraph.logic_list() ) {
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
  for ( auto node: sbjgraph.input_list() ) {
    s << ".inputs " << node->id_str() << endl;
  }

  for ( auto node: sbjgraph.output_list() ) {
    s << ".outputs " << node->id_str() << endl;
  }
  for ( auto node: sbjgraph.output_list() ) {
    const SbjNode* inode = node->fanin(0);
    if ( inode == 0 ) {
      s << ".names " << node->id_str() << endl;
      if ( node->output_fanin_inv() ) {
	s << "1" << endl;
      }
      else {
	s << "0" << endl;
      }
    }
    else {
      s << ".names " << inode->id_str() << " " << node->id_str() << endl;
      if ( node->output_fanin_inv() ) {
	s << "0 1" << endl;
      }
      else {
	s << "1 1" << endl;
      }
    }
    s << endl;
  }

  // blif では DFF を .latch 文で表す．
  for ( auto dff: sbjgraph.dff_list() ) {
    const SbjNode* onode = dff->data_output();
    const SbjNode* inode = dff->data_input();
    s << ".latch " << onode->id_str() << " "
      << inode->id_str() << endl;
  }

  // latch は無視

  for ( auto node: sbjgraph.logic_list() ) {
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
  const char* sep = "";
  for ( auto port: sbjgraph.port_list() ) {
    s << sep << "." << port->name() << "(";
    int nb = port->bit_width();
    ASSERT_COND( nb > 0  );
    if ( nb == 1 ) {
      const SbjNode* node = port->bit(0);
      s << node_name(node);
    }
    else {
      s << "{";
      const char* comma = "";
      for ( int j = 0; j < nb; ++ j ) {
	int idx = nb - j - 1;
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
  for ( auto node: sbjgraph.input_list() ) {
    s << "  input  " << node_name(node) << ";" << endl;
  }
  s << endl;

  // output 文
  for ( auto node: sbjgraph.output_list() ) {
    s << "  output " << node_name(node) << ";" << endl;
  }
  s << endl;

  // reg 定義
  for ( auto dff: sbjgraph.dff_list() ) {
    const SbjNode* node = dff->data_output();
    s << "  reg    " << node_name(node) << ";" << endl;
  }
  s << endl;

  // wire 定義
  for ( auto node: sbjgraph.logic_list() ) {
    s << "  wire   " << node_name(node) << ";" << endl;
  }
  s << endl;

  // output 用の assign 文
  for ( auto node: sbjgraph.output_list() ) {
    const SbjNode* inode = node->fanin(0);
    s << "  assign " << node_name(node) << " = ";
    if ( inode == 0 ) {
      if ( node->output_fanin_inv() ) {
	s << "1'b1";
      }
      else {
	s << "1'b0";
      }
    }
    else {
      if ( node->output_fanin_inv() ) {
	s << "~";
      }
      s << node_name(inode);
    }
    s << ";" << endl;
  }
  s << endl;

  // 論理ノード用の assign 文
  for ( auto node: sbjgraph.logic_list() ) {
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
  for ( auto dff: sbjgraph.dff_list() ) {
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

  // ラッチ用の always 文
  for ( auto latch: sbjgraph.latch_list() ) {
    const SbjNode* node = latch->data_output();
    const SbjNode* dnode = latch->data_input();
    const SbjNode* cnode = latch->enable();
    const SbjNode* snode = latch->preset();
    const SbjNode* rnode = latch->clear();
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

END_NAMESPACE_SBJ
