
/// @file BlifWriterImpl.cc
/// @brief BlifWriterImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "BlifWriterImpl.h"

#include "networks/CmnMgr.h"
#include "networks/CmnPort.h"
#include "networks/CmnDff.h"
#include "networks/CmnLatch.h"
#include "networks/CmnNode.h"
#include "networks/CmnDffCell.h"
#include "networks/CmnLatchCell.h"
#include "cell/Cell.h"
#include "cell/CellPin.h"


BEGIN_NAMESPACE_YM_NETWORKS_CMN

// @brief コンストラクタ
BlifWriterImpl::BlifWriterImpl()
{
}

// @brief デストラクタ
BlifWriterImpl::~BlifWriterImpl()
{
}

// @brief CmnMgr の内容を Blif-HDL 形式で出力する．
// @param[in] s 出力先のストリーム
// @param[in] mgr 対象のネットワーク
void
BlifWriterImpl::dump(ostream& s,
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

  s << ".model " << network.name() << endl;
  ymuint np = network.port_num();
  for (ymuint i = 0; i < np; ++ i) {
    const CmnPort* port = network.port(i);
    string port_name = port->name();
    ymuint nb = port->bit_width();
    if ( nb == 1 ) {
      const CmnNode* input = port->input(0);
      const CmnNode* output = port->output(0);
      if ( input ) {
	s << ".inputs " << port_name << endl;
	set_node_name(input, port_name);
      }
      else if ( output ) {
	s << ".outputs " << port_name << endl;
	set_node_name(output, port_name);
      }
      else {
	assert_not_reached(__FILE__, __LINE__);
      }
    }
    else if ( nb > 1 ) {
      for (ymuint b = 0; b < nb; ++ b) {
	const CmnNode* input = port->input(b);
	const CmnNode* output = port->output(b);
	ostringstream buf;
	buf << port_name << "[" << b << "]";
	string port_name1 = buf.str();
	if ( input ) {
	  s << ".inputs " << port_name1 << endl;
	  set_node_name(input, port_name1);
	}
	else if ( output ) {
	  s << ".outputs " << port_name1 << endl;
	  set_node_name(output, port_name1);
	}
	else {
	  assert_not_reached(__FILE__, __LINE__);
	}
      }
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
    s << ".names " << node_name(inode) << " ";
    if ( node->alt_node() ) {
      s << node_name(node->alt_node());
    }
    else {
      s << node_name(node);
    }
    s << endl;
    s << "1 1" << endl;
  }
  for (CmnDffList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const CmnDff* dff = *p;
    const CmnDffCell* dffcell = dff->cell();
    const Cell* cell = dffcell->cell();
    s << ".gate " << cell->name();

    // データ入力
    const CellPin* ipin = cell->input(dffcell->data_pos());
    s << " " << ipin->name() << "=" << dff_node_name(dff->input());
    // クロック入力
    const CellPin* cpin = cell->input(dffcell->clock_pos());
    s << " " << cpin->name() << "=" << dff_node_name(dff->clock());
    // クリア入力
    if ( dffcell->has_clear() ) {
      const CellPin* rpin = cell->input(dffcell->clear_pos());
      s << " " << rpin->name() << "=" << dff_node_name(dff->clear());
    }
    // プリセット入力
    if ( dffcell->has_preset() ) {
      const CellPin* ppin = cell->input(dffcell->preset_pos());
      s << " " << ppin->name() << "=" << dff_node_name(dff->preset());
    }
    // 肯定出力
    if ( dff->output1()->fanout_num() > 0 ) {
      const CellPin* opin1 = cell->output(dffcell->q_pos());
      s << " " << opin1->name() << "=" << node_name(dff->output1());
    }
    // 否定出力
    if ( dff->output2()->fanout_num() > 0 ) {
      const CellPin* opin2 = cell->output(dffcell->xq_pos());
      s << " " << opin2->name() << "=" << node_name(dff->output2());
    }
    s << endl;
  }

  for (CmnLatchList::const_iterator p = latch_list.begin();
       p != latch_list.end(); ++ p) {
    const CmnLatch* latch = *p;
    const CmnLatchCell* latchcell = latch->cell();
    const Cell* cell = latchcell->cell();

    s << ".gate " << cell->name();

    // データ入力
    const CellPin* ipin = cell->input(latchcell->data_pos());
    s << " " << ipin->name() << "=" << dff_node_name(latch->input());
    // イネーブル入力
    if ( latchcell->has_enable() ) {
      const CellPin* epin = cell->input(latchcell->enable_pos());
      s << " " << epin->name() << "=" << dff_node_name(latch->enable());
    }
    // クリア入力
    if ( latchcell->has_clear() ) {
      const CellPin* rpin = cell->input(latchcell->clear_pos());
      s << " " << rpin->name() << "=" << dff_node_name(latch->clear());
    }
    // プリセット入力
    if ( latchcell->has_preset() ) {
      const CellPin* ppin = cell->input(latchcell->preset_pos());
      s << " " << ppin->name() << "=" << dff_node_name(latch->preset());
    }
    // 肯定出力
    if ( latch->output1()->fanout_num() > 0 ) {
      const CellPin* opin1 = cell->output(latchcell->q_pos());
      s << " " << opin1->name() << "=" << node_name(latch->output1());
    }
    s << endl;
  }

  for (CmnNodeList::const_iterator p = logic_list.begin();
       p != logic_list.end(); ++ p) {
    const CmnNode* node = *p;
    const Cell* cell = node->cell();
    assert_cond( cell != NULL, __FILE__, __LINE__);
    ymuint ni = cell->input_num();
    ymuint no = cell->output_num();
    assert_cond( no == 1, __FILE__, __LINE__);
    const CellPin* pin = cell->output(0);
    assert_cond( pin->is_output(), __FILE__, __LINE__);

    s << ".gate " << cell->name()
      << " " << pin->name() << "=" << node_name(node);
    for (ymuint i = 0; i < ni; ++ i) {
      const CellPin* pin = cell->input(i);
      assert_cond( pin->is_input(), __FILE__, __LINE__);
      const CmnNode* node1 = node->fanin(i);
      s << " " << pin->name() << "=" << node_name(node1);
    }
    s << endl;
  }

  s << ".end" << endl;
}

// @brief ノード名を設定する．
void
BlifWriterImpl::set_node_name(const CmnNode* node,
			      const string& name)
{
  mNameArray[node->id()] = name;
}

// @brief ノード名を返す．
string
BlifWriterImpl::node_name(const CmnNode* node) const
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
BlifWriterImpl::dff_node_name(const CmnNode* node) const
{
  const CmnNode* inode = node->fanin(0);
  assert_cond( inode != NULL, __FILE__, __LINE__);
  return node_name(inode);
}

END_NAMESPACE_YM_NETWORKS_CMN
