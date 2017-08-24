
/// @file Bn2Sbj.cc
/// @brief Bn2Sbj の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
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

#include "ym/Expr.h"


BEGIN_NAMESPACE_YM_SBJ

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
  HashMap<ymuint, SbjHandle> node_map;

  // 外部入力ノードの生成
  ymuint ni = src_network.input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const BnNode* bn_node = src_network.input(i);
    SbjNode* sbj_node = dst_network.new_input();
    node_map.add(bn_node->id(), SbjHandle(sbj_node));
  }

  // 論理ノードの生成
  ymuint nl = src_network.logic_num();
  for (ymuint i = 0; i < nl; ++ i) {
    const BnNode* bn_node = src_network.logic(i);
    ymuint ni = bn_node->fanin_num();
    vector<SbjHandle> ihandle_list(ni);
    for (ymuint j = 0; j < ni; ++ j) {
      ymuint iid = bn_node->fanin(j);
      bool stat = node_map.find(iid, ihandle_list[j]);
      ASSERT_COND( stat );
    }
    SbjHandle sbj_handle;
    BnLogicType logic_type = bn_node->logic_type();
    switch ( logic_type ) {
    case kBnLt_NONE:
      ASSERT_NOT_REACHED;
      break;

    case kBnLt_C0:
      sbj_handle = SbjHandle::make_zero();
      break;

    case kBnLt_C1:
      sbj_handle = SbjHandle::make_one();
      break;

    case kBnLt_BUFF:
      sbj_handle = ihandle_list[0];
      break;

    case kBnLt_NOT:
      sbj_handle = ~ihandle_list[0];
      break;

    case kBnLt_AND:
      sbj_handle = dst_network.new_and(ihandle_list);
      break;

    case kBnLt_NAND:
      sbj_handle = ~dst_network.new_and(ihandle_list);
      break;

    case kBnLt_OR:
      sbj_handle = dst_network.new_or(ihandle_list);
      break;

    case kBnLt_NOR:
      sbj_handle = ~dst_network.new_or(ihandle_list);
      break;

    case kBnLt_XOR:
      sbj_handle = dst_network.new_xor(ihandle_list);
      break;

    case kBnLt_XNOR:
      sbj_handle = ~dst_network.new_xor(ihandle_list);
      break;

    case kBnLt_EXPR:
      sbj_handle = dst_network.new_expr(bn_node->expr(), ihandle_list);
      break;

    case kBnLt_TV:
      cerr << "Sorry, truth vector type is not supported!" << endl;
      sbj_handle = SbjHandle::make_zero();
      break;

    default:
      ASSERT_NOT_REACHED;
      break;
    }
    node_map.add(bn_node->id(), sbj_handle);
  }

  // 外部出力ノードの生成
  ymuint no = src_network.output_num();
  for (ymuint i = 0; i < no; ++ i) {
    const BnNode* bn_node = src_network.output(i);
    ymuint iid = bn_node->fanin();
    SbjHandle ihandle;
    bool stat = node_map.find(iid, ihandle);
    ASSERT_COND( stat );
    SbjNode* sbj_node = dst_network.new_output(ihandle);
    node_map.add(bn_node->id(), SbjHandle(sbj_node));
  }

  // DFFノードの生成
  ymuint ndff = src_network.dff_num();
  for (ymuint i = 0; i < ndff; ++ i) {
    const BnDff* bn_dff = src_network.dff(i);

    ymuint iid = bn_dff->input();
    SbjHandle ihandle;
    bool stat1 = node_map.find(iid, ihandle);
    ASSERT_COND( stat1 );
    ASSERT_COND( !ihandle.inv() );
    SbjNode* sbj_input = ihandle.node();

    ymuint cid = bn_dff->clock();
    SbjHandle chandle;
    bool stat2 = node_map.find(cid, chandle);
    ASSERT_COND( stat2 );
    ASSERT_COND( !chandle.inv() );
    SbjNode* sbj_clock = chandle.node();

    ymuint oid = bn_dff->output();
    SbjHandle ohandle;
    bool stat3 = node_map.find(oid, ohandle);
    ASSERT_COND( stat3 );
    ASSERT_COND( !ohandle.inv() );
    SbjNode* sbj_output = ohandle.node();

    ymuint rid = bn_dff->clear();
    SbjNode* sbj_clear = nullptr;
    if ( rid != kBnNullId ) {
      SbjHandle handle1;
      bool stat1 = node_map.find(rid, handle1);
      ASSERT_COND( stat1 );
      ASSERT_COND( !handle1.inv() );
      sbj_clear = handle1.node();
    }

    ymuint pid = bn_dff->preset();
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
  ymuint nlatch = src_network.latch_num();
  for (ymuint i = 0; i < nlatch; ++ i) {
    const BnLatch* bn_latch = src_network.latch(i);

    ymuint iid = bn_latch->input();
    SbjHandle ihandle;
    bool stat1 = node_map.find(iid, ihandle);
    ASSERT_COND( stat1 );
    ASSERT_COND( !ihandle.inv() );
    SbjNode* sbj_input = ihandle.node();

    ymuint eid = bn_latch->enable();
    SbjHandle chandle;
    bool stat2 = node_map.find(eid, chandle);
    ASSERT_COND( stat2 );
    ASSERT_COND( !chandle.inv() );
    SbjNode* sbj_enable = chandle.node();

    ymuint oid = bn_latch->output();
    SbjHandle ohandle;
    bool stat3 = node_map.find(oid, ohandle);
    ASSERT_COND( stat3 );
    ASSERT_COND( !ohandle.inv() );
    SbjNode* sbj_output = ohandle.node();

    ymuint rid = bn_latch->clear();
    SbjNode* sbj_clear = nullptr;
    if ( rid != kBnNullId ) {
      SbjHandle handle1;
      bool stat1 = node_map.find(rid, handle1);
      ASSERT_COND( stat1 );
      ASSERT_COND( !handle1.inv() );
      sbj_clear = handle1.node();
    }

    ymuint pid = bn_latch->preset();
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
  ymuint np = src_network.port_num();
  for (ymuint i = 0; i < np; ++ i) {
    const BnPort* bn_port = src_network.port(i);
    ymuint bw = bn_port->bit_width();
    vector<SbjNode*> sbj_bits(bw);
    for (ymuint j = 0; j < bw; ++ j) {
      ymuint id = bn_port->bit(j);
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

END_NAMESPACE_YM_SBJ
