
/// @file Bn2Sbj.cc
/// @brief Bn2Sbj の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "Bn2Sbj.h"

#include "SbjGraph.h"
#include "SbjNode.h"
#include "SbjHandle.h"
#include "SbjPort.h"
#include "SbjDff.h"
#include "SbjLatch.h"

#include "ym/BnNetwork.h"
#include "ym/BnPort.h"
#include "ym/BnNode.h"
#include "ym/BnNodeList.h"
#include "ym/BnDff.h"
#include "ym/Range.h"
#include "ym/Expr.h"


BEGIN_NAMESPACE_SBJ

//////////////////////////////////////////////////////////////////////
// クラス Bn2Sbj
//////////////////////////////////////////////////////////////////////

// @brief BnNetwork を SbjGraph に変換する．
void
Bn2Sbj::convert(
  const BnNetwork& src_network,
  SbjGraph& dst_network
)
{
  dst_network.clear();

  // ネットワーク名の設定
  dst_network.set_name(src_network.name());

  // BnNode::id() をキーにして SbjHandle を記録する配列
  vector<SbjHandle> node_map(src_network.node_num());

  // 外部入力ノードの生成
  for ( auto node: src_network.input_list() ) {
    auto sbj_node = dst_network.new_input(false);
    node_map[node.id()] = SbjHandle{sbj_node};
  }

  // 論理ノードの生成
  for ( auto bn_node: src_network.logic_list() ) {
    auto id = bn_node.id();
    auto ni = bn_node.fanin_num();
    vector<SbjHandle> ihandle_list;
    ihandle_list.reserve(ni);
    for ( auto inode: bn_node.fanin_list() ) {
      ihandle_list.push_back(node_map[inode.id()]);
    }
    auto logic_type = bn_node.type();
    switch ( logic_type ) {
    case BnNodeType::Prim:
      switch ( bn_node.primitive_type() ) {
      case PrimType::C0:
	node_map[id] = SbjHandle::make_zero();
	break;

      case PrimType::C1:
	node_map[id] = SbjHandle::make_one();
	break;

      case PrimType::Buff:
	node_map[id] = ihandle_list[0];
	break;

      case PrimType::Not:
	node_map[id] = ~ihandle_list[0];
	break;

      case PrimType::And:
	node_map[id] = dst_network.new_and(ihandle_list);
	break;

      case PrimType::Nand:
	node_map[id] = ~dst_network.new_and(ihandle_list);
	break;

      case PrimType::Or:
	node_map[id] = dst_network.new_or(ihandle_list);
	break;

      case PrimType::Nor:
	node_map[id] = ~dst_network.new_or(ihandle_list);
	break;

      case PrimType::Xor:
	node_map[id] = dst_network.new_xor(ihandle_list);
	break;

      case PrimType::Xnor:
	node_map[id] = ~dst_network.new_xor(ihandle_list);
	break;

      case PrimType::None:
	ASSERT_NOT_REACHED;
	break;
      }
      break;
    case BnNodeType::Expr:
      {
	const Expr& expr = src_network.expr(bn_node.expr_id());
	node_map[id] = dst_network.new_expr(expr, ihandle_list);
      }
      break;

    case BnNodeType::TvFunc:
      cerr << "Sorry, truth vector type is not supported!" << endl;
      node_map[id] = SbjHandle::make_zero();
      break;

    case BnNodeType::Bdd:
      cerr << "Sorry, BDD type is not supported!" << endl;
      node_map[id] = SbjHandle::make_zero();
      break;

    default:
      ASSERT_NOT_REACHED;
      break;
    }
  }

  // 外部出力ノードの生成
  for ( auto node: src_network.output_list() ) {
    SbjHandle ihandle = node_map[node.output_src().id()];
    SbjNode* sbj_node = dst_network.new_output(ihandle);
    node_map[node.id()] = SbjHandle(sbj_node);
  }

  // DFFノードの生成
  for ( auto bn_dff: src_network.dff_list() ) {
    if ( bn_dff.is_cell() ) {
      cerr << "Sorry, CELL type DFF is not supported!" << endl;
      continue;
    }

    auto iid = bn_dff.data_in().id();
    auto ihandle = node_map[iid];
    ASSERT_COND( !ihandle.inv() );
    auto sbj_input = ihandle.node();

    auto cid = bn_dff.clock().id();
    auto chandle = node_map[cid];
    ASSERT_COND( !chandle.inv() );
    auto sbj_clock = chandle.node();

    auto oid = bn_dff.data_out().id();
    auto ohandle = node_map[oid];
    ASSERT_COND( !ohandle.inv() );
    auto sbj_output = ohandle.node();

    auto rid = bn_dff.clear().id();
    SbjNode* sbj_clear = nullptr;
    if ( rid != BNET_NULLID ) {
      auto handle1 = node_map[rid];
      ASSERT_COND( !handle1.inv() );
      sbj_clear = handle1.node();
    }

    auto pid = bn_dff.preset().id();
    SbjNode* sbj_preset = nullptr;
    if ( pid != BNET_NULLID ) {
      auto handle1 = node_map[pid];
      ASSERT_COND( !handle1.inv() );
      sbj_preset = handle1.node();
    }

    if ( bn_dff.is_dff() ) {
      dst_network.new_dff(sbj_input, sbj_output,
			  sbj_clock, sbj_clear, sbj_preset);
    }
    else { // bn_dff.is_latch()
      dst_network.new_latch(sbj_input, sbj_output,
			    sbj_clock, sbj_clear, sbj_preset);
    }
  }

  // ポートの生成
  for ( auto bn_port: src_network.port_list() ) {
    SizeType bw = bn_port.bit_width();
    vector<SbjNode*> sbj_bits(bw);
    for ( int j: Range(bw) ) {
      SizeType id = bn_port.bit(j).id();
      SbjHandle handle = node_map[id];
      ASSERT_COND( !handle.inv() );
      SbjNode* sbj_node = handle.node();
      sbj_bits[j] = sbj_node;
    }
    dst_network.add_port(bn_port.name(), sbj_bits);
  }
}

END_NAMESPACE_SBJ
