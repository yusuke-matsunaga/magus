
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

#include "../verilog/VlWriter.h"
#include "../verilog/VlwModule.h"
#include "../verilog/VlwModuleHeader.h"
#include "../verilog/VlwIO.h"
#include "../verilog/VlwAssign.h"
#include "../verilog/VlwInst.h"


BEGIN_NAMESPACE_YM_NETWORKS_CMN

BEGIN_NONAMESPACE

// Verilog 用のエスケープ文字列を作る．
string
vescape(const string& str)
{
  string::const_iterator p = str.begin();
  char c = *p ++;
  if ( c == '\\' ) {
    return str;
  }
  if ( !isalpha(c) && c != '_' ) {
    goto need_escape;
  }
  for ( ; p != str.end(); ++ p) {
    char c = *p;
    if ( !isalnum(c) && c != '_' ) {
      goto need_escape;
    }
  }
  return str;

 need_escape:
  // 先頭が '\' ならば次の空白までを識別子と見なす(らしい)
  string ans = "\\" + str + " ";
  return ans;
}

END_NONAMESPACE

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
  VlWriter writer(s);

  ymuint n = network.max_node_id();
  mNameArray.clear();
  mNameArray.resize(n, string());

  const CmnDffList& dff_list = network.dff_list();
  const CmnLatchList& latch_list = network.latch_list();
  //const CmnNodeList& input_list = network.input_list();
  const CmnNodeList& output_list = network.output_list();
  const CmnNodeList& logic_list = network.logic_list();
  ymuint np = network.port_num();

  VlwModule vlw_module(writer, network.name());

  {
    VlwModuleHeader vlw_module_hader(writer);

    const char* sep = "";
    for (ymuint i = 0; i < np; ++ i) {
      const CmnPort* port = network.port(i);
      writer.put_str(sep);
      writer.put_idstr(port->name());
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
  }

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
	  VlwInout vlw_decl(writer);
	  writer.put_elem(port_name);
	}
	else {
	  VlwInput vlw_decl(writer);
	  writer.put_elem(port_name);
	}
      }
      else if ( output ) {
	set_node_name(output, port_name);
	VlwOutput vlw_decl(writer);
	writer.put_elem(port_name);
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
	buf << vescape(port_name) << "[" << j << "]";
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
	VlwInput vlw_decl(writer, nb - 1, 0);
	writer.put_elem(port_name);
      }
      else if ( !has_input ) {
	VlwOutput vlw_decl(writer, nb - 1, 0);
	writer.put_elem(port_name);
      }
      else {
	VlwInout vlw_decl(writer, nb - 1, 0);
	writer.put_elem(port_name);
      }
    }
  }

  for (CmnNodeList::const_iterator p = logic_list.begin();
       p != logic_list.end(); ++ p) {
    const CmnNode* node = *p;
    VlwWire vlw_decl(writer);
    writer.put_elem(node_name(node));
  }

  for (CmnDffList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const CmnDff* dff = *p;
    const CmnNode* q = dff->output1();
    const CmnNode* xq = dff->output2();
    if ( q->fanout_num() > 0 ) {
      VlwWire vlw_decl(writer);
      writer.put_elem(node_name(q));
    }
    if ( xq->fanout_num() > 0 ) {
      VlwWire vlw_decl(writer);
      writer.put_elem(node_name(xq));
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

    VlwAssign vlw_assign(writer);
    if ( node->alt_node() ) {
      vlw_assign.put_lhs(node_name(node->alt_node()));
    }
    else {
      vlw_assign.put_lhs(node_name(node));
    }
    writer.put_idstr(node_name(inode));
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

    VlwInst vlw_inst(writer, cell->name());

    ostringstream buf;
    buf << "U" << node->id();
    string inst_name = buf.str();
    VlwInstElem vlw_elem(vlw_inst, inst_name);

    ymuint ni = cell->input_num();
    for (ymuint ipos = 0; ipos < ni; ++ ipos) {
      const CellPin* pin = cell->input(ipos);
      assert_cond( pin->is_input(), __FILE__, __LINE__);
      const CmnNode* node1 = node->fanin(ipos);
      vlw_elem.put_pinassign(pin->name(), node_name(node1));
    }
    ymuint no = cell->output_num();
    assert_cond( no == 1, __FILE__, __LINE__);
    const CellPin* pin = cell->output(0);
    assert_cond( pin->is_output(), __FILE__, __LINE__);
    vlw_elem.put_pinassign(pin->name(), node_name(node));
  }
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
