
/// @file Bn2Sbj.cc
/// @brief Bn2Sbj の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016, 2018 Yusuke Matsunaga
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
#include "ym/BnDff.h"
#include "ym/BnLatch.h"
#include "ym/Range.h"
#include "ym/Expr.h"


BEGIN_NAMESPACE_SBJ

//////////////////////////////////////////////////////////////////////
// クラス Bn2Sbj
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Bn2Sbj::Bn2Sbj()
{
}

// @brief デストラクタ
Bn2Sbj::~Bn2Sbj()
{
}

// @brief BnNetwork を SbjGraph に変換する．
// @param[in] src_network もとのネットワーク
// @param[in] dst_network 変換されたネットワーク
void
Bn2Sbj::convert(const BnNetwork& src_network,
		SbjGraph& dst_network)
{
  dst_network.clear();

  // ネットワーク名の設定
  dst_network.set_name(src_network.name());

  // BnNode::id() をキーにして SbjHandle を記録するハッシュ表
  HashMap<int, SbjHandle> node_map;

  // 外部入力ノードの生成
  for ( auto id: src_network.input_id_list() ) {
    SbjNode* sbj_node = dst_network.new_input(false);
    node_map.add(id, SbjHandle(sbj_node));
  }

  // 論理ノードの生成
  for ( auto id: src_network.logic_id_list() ) {
    auto bn_node = src_network.node(id);
    int ni = bn_node->fanin_num();
    vector<SbjHandle> ihandle_list(ni);
    for ( int j: Range(ni) ) {
      int iid = bn_node->fanin_id(j);
      bool stat = node_map.find(iid, ihandle_list[j]);
      ASSERT_COND( stat );
    }
    SbjHandle sbj_handle;
    BnNodeType logic_type = bn_node->type();
    switch ( logic_type ) {
    case BnNodeType::C0:
      sbj_handle = SbjHandle::make_zero();
      break;

    case BnNodeType::C1:
      sbj_handle = SbjHandle::make_one();
      break;

    case BnNodeType::Buff:
      sbj_handle = ihandle_list[0];
      break;

    case BnNodeType::Not:
      sbj_handle = ~ihandle_list[0];
      break;

    case BnNodeType::And:
      sbj_handle = dst_network.new_and(ihandle_list);
      break;

    case BnNodeType::Nand:
      sbj_handle = ~dst_network.new_and(ihandle_list);
      break;

    case BnNodeType::Or:
      sbj_handle = dst_network.new_or(ihandle_list);
      break;

    case BnNodeType::Nor:
      sbj_handle = ~dst_network.new_or(ihandle_list);
      break;

    case BnNodeType::Xor:
      sbj_handle = dst_network.new_xor(ihandle_list);
      break;

    case BnNodeType::Xnor:
      sbj_handle = ~dst_network.new_xor(ihandle_list);
      break;

    case BnNodeType::Expr:
      {
	const Expr& expr = src_network.expr(bn_node->expr_id());
	sbj_handle = dst_network.new_expr(expr, ihandle_list);
      }
      break;

    case BnNodeType::TvFunc:
      cerr << "Sorry, truth vector type is not supported!" << endl;
      sbj_handle = SbjHandle::make_zero();
      break;

    default:
      ASSERT_NOT_REACHED;
      break;
    }
    node_map.add(id, sbj_handle);
  }

  // 外部出力ノードの生成
  for ( auto id: src_network.output_src_id_list() ) {
    SbjHandle ihandle;
    bool stat = node_map.find(id, ihandle);
    ASSERT_COND( stat );
    SbjNode* sbj_node = dst_network.new_output(ihandle);
    node_map.add(id, SbjHandle(sbj_node));
  }

  // DFFノードの生成
  for ( auto bn_dff: src_network.dff_list() ) {
    int iid = bn_dff->input();
    SbjHandle ihandle;
    bool stat1 = node_map.find(iid, ihandle);
    ASSERT_COND( stat1 );
    ASSERT_COND( !ihandle.inv() );
    SbjNode* sbj_input = ihandle.node();

    int cid = bn_dff->clock();
    SbjHandle chandle;
    bool stat2 = node_map.find(cid, chandle);
    ASSERT_COND( stat2 );
    ASSERT_COND( !chandle.inv() );
    SbjNode* sbj_clock = chandle.node();

    int oid = bn_dff->output();
    SbjHandle ohandle;
    bool stat3 = node_map.find(oid, ohandle);
    ASSERT_COND( stat3 );
    ASSERT_COND( !ohandle.inv() );
    SbjNode* sbj_output = ohandle.node();

    int rid = bn_dff->clear();
    SbjNode* sbj_clear = nullptr;
    if ( rid != kBnNullId ) {
      SbjHandle handle1;
      bool stat1 = node_map.find(rid, handle1);
      ASSERT_COND( stat1 );
      ASSERT_COND( !handle1.inv() );
      sbj_clear = handle1.node();
    }

    int pid = bn_dff->preset();
    SbjNode* sbj_preset = nullptr;
    if ( pid != kBnNullId ) {
      SbjHandle handle1;
      bool stat1 = node_map.find(pid, handle1);
      ASSERT_COND( stat1 );
      ASSERT_COND( !handle1.inv() );
      sbj_preset = handle1.node();
    }

    SbjDff* sbj_dff = dst_network.new_dff(sbj_input, sbj_output,
					  sbj_clock, sbj_clear, sbj_preset);
  }

  // ラッチノードの生成
  for ( auto bn_latch: src_network.latch_list() ) {
    int iid = bn_latch->input();
    SbjHandle ihandle;
    bool stat1 = node_map.find(iid, ihandle);
    ASSERT_COND( stat1 );
    ASSERT_COND( !ihandle.inv() );
    SbjNode* sbj_input = ihandle.node();

    int eid = bn_latch->enable();
    SbjHandle chandle;
    bool stat2 = node_map.find(eid, chandle);
    ASSERT_COND( stat2 );
    ASSERT_COND( !chandle.inv() );
    SbjNode* sbj_enable = chandle.node();

    int oid = bn_latch->output();
    SbjHandle ohandle;
    bool stat3 = node_map.find(oid, ohandle);
    ASSERT_COND( stat3 );
    ASSERT_COND( !ohandle.inv() );
    SbjNode* sbj_output = ohandle.node();

    int rid = bn_latch->clear();
    SbjNode* sbj_clear = nullptr;
    if ( rid != kBnNullId ) {
      SbjHandle handle1;
      bool stat1 = node_map.find(rid, handle1);
      ASSERT_COND( stat1 );
      ASSERT_COND( !handle1.inv() );
      sbj_clear = handle1.node();
    }

    int pid = bn_latch->preset();
    SbjNode* sbj_preset = nullptr;
    if ( pid != kBnNullId ) {
      SbjHandle handle1;
      bool stat1 = node_map.find(pid, handle1);
      ASSERT_COND( stat1 );
      ASSERT_COND( !handle1.inv() );
      sbj_preset = handle1.node();
    }

    SbjLatch* sbj_latch = dst_network.new_latch(sbj_input, sbj_output,
						sbj_enable, sbj_clear, sbj_preset);
  }

  // ポートの生成
  for ( auto bn_port: src_network.port_list() ) {
    int bw = bn_port->bit_width();
    vector<SbjNode*> sbj_bits(bw);
    for ( int j: Range(bw) ) {
      int id = bn_port->bit(j);
      SbjHandle handle;
      bool stat = node_map.find(id, handle);
      ASSERT_COND( stat );
      ASSERT_COND( !handle.inv() );
      SbjNode* sbj_node = handle.node();
      sbj_bits[j] = sbj_node;
    }
    dst_network.add_port(bn_port->name(), sbj_bits);
  }
}

END_NAMESPACE_SBJ
