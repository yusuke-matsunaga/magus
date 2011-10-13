
/// @file VerilogWriterImpl.cc
/// @brief VerilogWriterImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "VerilogWriterImpl.h"

#include "ym_networks/CmnMgr.h"
#include "ym_networks/CmnPort.h"
#include "ym_networks/CmnDff.h"
#include "ym_networks/CmnLatch.h"
#include "ym_networks/CmnNode.h"
#include "ym_networks/CmnDffCell.h"
#include "ym_networks/CmnLatchCell.h"
#include "ym_cell/Cell.h"
#include "ym_cell/CellPin.h"


BEGIN_NAMESPACE_YM_NETWORKS_CMN

// @brief コンストラクタ
VerilogWriterImpl::VerilogWriterImpl()
{
}

// @brief デストラクタ
VerilogWriterImpl::~VerilogWriterImpl()
{
}

// @brief CmnMgr の内容を Verilog-HDL 形式で出力する．
// @param[in] s 出力先のストリーム
// @param[in] mgr 対象のネットワーク
void
VerilogWriterImpl::dump(ostream& s,
			const CmnMgr& network)
{
  ymuint n = network.max_node_id();
  mNameArray.clear();
  mNameArray.resize(n, string());

  const CmnDffList& dff_list = network.dff_list();
  const CmnLatchList& latch_list = network.latch_list();
  //const CmnNodeList& input_list = network.input_list();
  const CmnNodeList& output_list = network.output_list();
  const CmnNodeList& logic_list = network.logic_list();

  s << "module " << network.name() << "(";
  ymuint np = network.port_num();
  const char* sep = "";
  for (ymuint i = 0; i < np; ++ i) {
    const CmnPort* port = network.port(i);
    s << sep << port->name();
    sep = ", ";

    ymuint nb = port->bit_width();
    assert_cond( nb > 0, __FILE__, __LINE__);
    if ( nb == 1 ) {
      const CmnNode* input = port->input(0);
      const CmnNode* output = port->output(0);
      if ( input ) {
	set_node_name(input, port->name());
      }
      else if ( output ) {
	set_node_name(output, port->name());
      }
      else {
	assert_not_reached(__FILE__, __LINE__);
      }
    }
  }
  s << ");" << endl;

  for (ymuint i = 0; i < np; ++ i) {
    const CmnPort* port = network.port(i);
    string port_name = port->name();
    ymuint nb = port->bit_width();
    assert_cond( nb > 0, __FILE__, __LINE__);
    if ( nb == 1 ) {
      const CmnNode* input = port->input(0);
      const CmnNode* output = port->output(0);
      if ( input ) {
	set_node_name(input, port_name);
	if ( input->alt_node() ) {
	  s << "  inout  ";
	}
	else {
	  s << "  input  ";
	}
	s << port_name << ";" << endl;
      }
      else if ( output ) {
	set_node_name(output, port_name);
	s << "  output  " << port_name << ";" << endl;
      }
      else {
	assert_not_reached(__FILE__, __LINE__);
      }
    }
    else {
      bool has_input = false;
      bool has_output = false;
      for (ymuint j = 0; j < nb; ++ j) {
	const CmnNode* input = port->input(j);
	const CmnNode* output = port->output(j);
	ostringstream buf;
	buf << port_name << "[" << j << "]";
	if ( input != NULL ) {
	  has_input = true;
	  set_node_name(input, buf.str());
	}
	if ( output != NULL ) {
	  has_output = true;
	  set_node_name(output, buf.str());
	}
      }
      if ( !has_output ) {
	s << "  input";
      }
      else if ( !has_input ) {
	s << "  output";
      }
      else {
	s << "  inout";
      }
      s << " [" << nb - 1 << ":" << 0 << "]  " << port_name << ";" << endl;
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

  for (CmnDffList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const CmnDff* dff = *p;
    const CmnDffCell* dffcell = dff->cell();
    const Cell* cell = dffcell->cell();
    const CmnNode* onode1 = dff->output1();
    s << "  " << cell->name() << " U" << onode1->id() << " (";
    // データ入力
    const CellPin* ipin = cell->input(dffcell->data_pos());
    s << "." << ipin->name() << "(" << dff_node_name(dff->input()) << ")";
    // クロック入力
    const CellPin* cpin = cell->input(dffcell->clock_pos());
    s << ", ." << cpin->name() << "(" << dff_node_name(dff->clock()) << ")";
    // クリア入力
    if ( dffcell->has_clear() ) {
      const CellPin* rpin = cell->input(dffcell->clear_pos());
      s << ", ." << rpin->name() << "(" << dff_node_name(dff->clear()) << ")";
    }
    // プリセット入力
    if ( dffcell->has_preset() ) {
      const CellPin* ppin = cell->input(dffcell->preset_pos());
      s << ", ." << ppin->name() << "(" << dff_node_name(dff->preset()) << ")";
    }
    // 肯定出力
    if ( dff->output1()->fanout_num() > 0 ) {
      const CellPin* opin1 = cell->output(dffcell->q_pos());
      s << ", ." << opin1->name() << "(" << node_name(dff->output1()) << ")";
    }
    // 否定出力
    if ( dff->output2()->fanout_num() > 0 ) {
      const CellPin* opin2 = cell->output(dffcell->xq_pos());
      s << ", ." << opin2->name() << "(" << node_name(dff->output2()) << ")";
    }
    s << ");" << endl;
  }

  for (CmnLatchList::const_iterator p = latch_list.begin();
       p != latch_list.end(); ++ p) {
    const CmnLatch* latch = *p;
    const CmnLatchCell* latchcell = latch->cell();
    const Cell* cell = latchcell->cell();
    const CmnNode* onode1 = latch->output1();
    s << "  " << cell->name() << " U" << onode1->id() << " (";
    // データ入力
    const CellPin* ipin = cell->input(latchcell->data_pos());
    s << "." << ipin->name() << "(" << dff_node_name(latch->input()) << ")";
    // イネーブル入力
    if ( latchcell->has_enable() ) {
      const CellPin* epin = cell->input(latchcell->enable_pos());
      s << ", ." << epin->name() << "(" << dff_node_name(latch->enable()) << ")";
    }
    // クリア入力
    if ( latchcell->has_clear() ) {
      const CellPin* rpin = cell->input(latchcell->clear_pos());
      s << ", ." << rpin->name() << "(" << dff_node_name(latch->clear()) << ")";
    }
    // プリセット入力
    if ( latchcell->has_preset() ) {
      const CellPin* ppin = cell->input(latchcell->preset_pos());
      s << ", ." << ppin->name() << "(" << dff_node_name(latch->preset()) << ")";
    }
    // 肯定出力
    if ( latch->output1()->fanout_num() > 0 ) {
      const CellPin* opin1 = cell->output(latchcell->q_pos());
      s << ", ." << opin1->name() << "(" << node_name(latch->output1()) << ")";
    }
    s << ");" << endl;
  }

  for (CmnNodeList::const_iterator p = logic_list.begin();
       p != logic_list.end(); ++ p) {
    const CmnNode* node = *p;
    const Cell* cell = node->cell();
    assert_cond( cell != NULL, __FILE__, __LINE__);
    s << "  " << cell->name() << " U" << node->id() << " (";
    const char* comma = "";
    ymuint ni = cell->input_num();
    for (ymuint ipos = 0; ipos < ni; ++ ipos) {
      const CellPin* pin = cell->input(ipos);
      assert_cond( pin->is_input(), __FILE__, __LINE__);
      const CmnNode* node1 = node->fanin(ipos);
      s << comma << "." << pin->name() << "(" << node_name(node1) << ")";
      comma = ", ";
    }
    ymuint no = cell->output_num();
    assert_cond( no == 1, __FILE__, __LINE__);
    const CellPin* pin = cell->output(0);
    assert_cond( pin->is_output(), __FILE__, __LINE__);
    s << comma << "." << pin->name() << "(" << node_name(node) << ")";
    s << ");" << endl;
  }

  s << "endmodule" << endl;
}

// @brief ノード名を設定する．
void
VerilogWriterImpl::set_node_name(const CmnNode* node,
				 const string& name)
{
  mNameArray[node->id()] = name;
}

// @brief ノード名を返す．
string
VerilogWriterImpl::node_name(const CmnNode* node) const
{
  string name = mNameArray[node->id()];
  if ( name == string() ) {
#warning "TODO: 名前の衝突回避のための NameMgr の導入"
    name = "n" + node->id_str();
  }
  return name;
}

// DFF の入力のノード名を返す．
string
VerilogWriterImpl::dff_node_name(const CmnNode* node) const
{
  const CmnNode* inode = node->fanin(0);
  assert_cond( inode != NULL, __FILE__, __LINE__);
  return node_name(inode);
}

END_NAMESPACE_YM_NETWORKS_CMN
