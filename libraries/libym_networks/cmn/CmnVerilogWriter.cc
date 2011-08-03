
/// @file libym_networks/cmn/CmnVerilogWriter.cc
/// @brief CmnVerilogWriter 実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/CmnVerilogWriter.h"
#include "ym_networks/CmnMgr.h"
#include "ym_networks/CmnPort.h"
#include "ym_networks/CmnDff.h"
#include "ym_networks/CmnLatch.h"
#include "ym_networks/CmnNode.h"
#include "ym_networks/CmnDffCell.h"
#include "ym_networks/CmnLatchCell.h"
#include "ym_cell/Cell.h"
#include "ym_cell/CellPin.h"


BEGIN_NAMESPACE_YM_NETWORKS

// @brief コンストラクタ
CmnVerilogWriter::CmnVerilogWriter()
{
}

// @brief デストラクタ
CmnVerilogWriter::~CmnVerilogWriter()
{
}

BEGIN_NONAMESPACE

// ノード名を返す．
string
node_name(const CmnNode* node)
{
  return "n" + node->id_str();
}

// DFF の入力のノード名を返す．
string
dff_node_name(const CmnNode* node)
{
  const CmnNode* inode = node->fanin(0);
  assert_cond( inode != NULL, __FILE__, __LINE__);
  return node_name(inode);
}

END_NONAMESPACE


// 内容を Verilog-HDL 形式で s に出力する．
void
CmnVerilogWriter::operator()(ostream& s,
			     const CmnMgr& network)
{
  const CmnDffList& dff_list = network.dff_list();
  const CmnLatchList& latch_list = network.latch_list();
  const CmnNodeList& input_list = network.input_list();
  const CmnNodeList& output_list = network.output_list();
  const CmnNodeList& logic_list = network.logic_list();

  s << "module " << network.name() << "(";
  ymuint np = network.port_num();
  const char* sep = "";
  for (ymuint i = 0; i < np; ++ i) {
    const CmnPort* port = network.port(i);
    s << sep << "." << port->name() << "(";
    ymuint nb = port->bit_width();
    assert_cond( nb > 0, __FILE__, __LINE__);
    if ( nb == 1 ) {
      const CmnNode* input = port->input(0);
      const CmnNode* output = port->output(0);
      if ( input ) {
	s << node_name(input);
      }
      else if ( output ) {
	s << node_name(output);
      }
    }
    else {
      s << "{";
      const char* comma = "";
      for (ymuint j = 0; j < nb; ++ j) {
	ymuint idx = nb - j - 1;
	const CmnNode* input = port->input(idx);
	const CmnNode* output = port->output(idx);
	s << comma;
	comma = ", ";
	if ( input ) {
	  s << node_name(input);
	}
	else if ( output ) {
	  s << node_name(output);
	}
      }
      s << "}";
    }
    s << ")";
    sep = ", ";
  }
  s << ");" << endl;

  for (CmnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const CmnNode* node = *p;
    if ( node->input_type() != CmnNode::kPRIMARY_INPUT ) {
      continue;
    }
    if ( node->alt_node() ) {
      s << "  inout  ";
    }
    else {
      s << "  input  ";
    }
    s << node_name(node) << ";" << endl;
  }

  for (CmnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const CmnNode* node = *p;
    if ( node->output_type() != CmnNode::kPRIMARY_OUTPUT ) {
      continue;
    }
    if ( node->alt_node() == NULL ) {
      s << "  output " << node_name(node) << ";" << endl;
    }
  }

  for (CmnNodeList::const_iterator p = logic_list.begin();
       p != logic_list.end(); ++ p) {
    const CmnNode* node = *p;
    s << "  wire   " << node_name(node) << ";" << endl;
  }

  for (CmnDffList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const CmnDff* dff = *p;
    const CmnNode* q = dff->output1();
    const CmnNode* iq = dff->output2();
    if ( q->fanout_num() > 0 ) {
      s << "  wire    " << node_name(q) << ";" << endl;
    }
    if ( iq->fanout_num() > 0 ) {
      s << "  wire    " << node_name(iq) << ";" << endl;
    }
  }

  for (CmnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const CmnNode* node = *p;
    if ( node->output_type() != CmnNode::kPRIMARY_OUTPUT ) {
      continue;
    }
    const CmnNode* inode = node->fanin(0);
    assert_cond( inode != NULL, __FILE__, __LINE__);
    s << "  assign ";
    if ( node->alt_node() ) {
      s << node_name(node->alt_node());
    }
    else {
      s << node_name(node);
    }
    s << " = " << node_name(inode) << ";" << endl;
  }

  for (CmnNodeList::const_iterator p = logic_list.begin();
       p != logic_list.end(); ++ p) {
    const CmnNode* node = *p;
    const Cell* cell = node->cell();
    assert_cond( cell != NULL, __FILE__, __LINE__);
    ymuint np = cell->pin_num();
    s << "  " << cell->name() << " U" << node->id() << " (";
    ymuint ipos = 0;
    const char* comma = "";
    for (ymuint i = 0; i < np; ++ i) {
      const CellPin* pin = cell->pin(i);
      const CmnNode* node1 = NULL;
      if ( pin->is_input() ) {
	node1 = node->fanin(ipos);
	++ ipos;
      }
      else if ( pin->is_output() ) {
	node1 = node;
      }
      s << comma << "." << pin->name() << "(" << node_name(node1) << ")";
      comma = ", ";
    }
    s << ");" << endl;
  }

  for (CmnDffList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const CmnDff* dff = *p;
    const CmnDffCell* dffcell = dff->cell();
    const Cell* cell = dffcell->cell();
    const CmnNode* onode1 = dff->output1();
    s << "  " << cell->name() << " U" << onode1->id() << " (";
    // データ入力
    const CellPin* ipin = cell->pin(dffcell->data_pos());
    s << "." << ipin->name() << "(" << dff_node_name(dff->input()) << ")";
    // クロック入力
    const CellPin* cpin = cell->pin(dffcell->clock_pos());
    s << ", ." << cpin->name() << "(" << dff_node_name(dff->clock()) << ")";
    // クリア入力
    if ( dffcell->has_clear() ) {
      const CellPin* rpin = cell->pin(dffcell->clear_pos());
      s << ", ." << rpin->name() << "(" << dff_node_name(dff->clear()) << ")";
    }
    // プリセット入力
    if ( dffcell->has_preset() ) {
      const CellPin* ppin = cell->pin(dffcell->preset_pos());
      s << ", ." << ppin->name() << "(" << dff_node_name(dff->preset()) << ")";
    }
    // 肯定出力
    if ( dff->output1()->fanout_num() > 0 ) {
      const CellPin* opin1 = cell->pin(dffcell->q_pos());
      s << ", ." << opin1->name() << "(" << node_name(dff->output1()) << ")";
    }
    // 否定出力
    if ( dff->output2()->fanout_num() > 0 ) {
      const CellPin* opin2 = cell->pin(dffcell->iq_pos());
      s << ", ." << opin2->name() << "(" << node_name(dff->output2()) << ")";
    }
    s << ");" << endl;
  }

  s << "endmodule" << endl;
}

END_NAMESPACE_YM_NETWORKS
