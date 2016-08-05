
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
#include "ym/BnNetwork.h"
#include "ym/BnPort.h"
#include "ym/BnNode.h"
#include "ym/BnDff.h"
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
  dst_network.set_name(src_network.model_name());

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
      const BnNode* bn_inode = bn_node->fanin(j);
      ASSERT_COND( bn_inode != nullptr );
      bool stat = node_map.find(bn_inode->id(), ihandle_list[j]);
      ASSERT_COND( stat );
    }
    SbjHandle sbj_handle;
    BnLogicType logic_type = bn_node->logic_type();
    switch ( logic_type ) {
    case kBnLt_NONE:
      //ASSERT_NOT_REACHED;
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
      sbj_handle = dst_network.new_logic(bn_node->expr(), ihandle_list);
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
    const BnNode* bn_inode = bn_node->input();
    ASSERT_COND( bn_inode != nullptr );
    SbjHandle ihandle;
    bool stat = node_map.find(bn_inode->id(), ihandle);
    ASSERT_COND( stat );
    SbjNode* sbj_node = dst_network.new_output(ihandle);
    node_map.add(bn_node->id(), SbjHandle(sbj_node));
  }

  // DFFノードの生成
  ymuint ndff = src_network.dff_num();
  for (ymuint i = 0; i < ndff; ++ i) {
    const BnDff* bn_dff = src_network.dff(i);

    const BnNode* bn_input = bn_dff->input();
    SbjHandle ihandle;
    bool stat1 = node_map.find(bn_input->id(), ihandle);
    ASSERT_COND( stat1 );
    ASSERT_COND( !ihandle.inv() );
    SbjNode* sbj_input = ihandle.node();

    const BnNode* bn_clock = bn_dff->clock();
    SbjHandle chandle;
    bool stat2 = node_map.find(bn_clock->id(), chandle);
    ASSERT_COND( stat2 );
    ASSERT_COND( !chandle.inv() );
    SbjNode* sbj_clock = chandle.node();

    const BnNode* bn_output = bn_dff->output();
    SbjHandle ohandle;
    bool stat3 = node_map.find(bn_output->id(), ohandle);
    ASSERT_COND( stat3 );
    ASSERT_COND( !ohandle.inv() );
    SbjNode* sbj_output = ohandle.node();

    const BnNode* bn_clear = bn_dff->clear();
    SbjNode* sbj_clear = nullptr;
    if ( bn_clear != nullptr ) {
      SbjHandle handle1;
      bool stat1 = node_map.find(bn_clear->id(), handle1);
      ASSERT_COND( stat1 );
      ASSERT_COND( !handle1.inv() );
      sbj_clear = handle1.node();
    }

    const BnNode* bn_preset = bn_dff->preset();
    SbjNode* sbj_preset = nullptr;
    if ( bn_preset != nullptr ) {
      SbjHandle handle1;
      bool stat1 = node_map.find(bn_preset->id(), handle1);
      ASSERT_COND( stat1 );
      ASSERT_COND( !handle1.inv() );
      sbj_preset = handle1.node();
    }

    SbjDff* sbj_dff = dst_network.new_dff(sbj_input, sbj_output, sbj_clock, sbj_clear, sbj_preset);
  }

  // ポートの生成
  ymuint np = src_network.port_num();
  for (ymuint i = 0; i < np; ++ i) {
    const BnPort* bn_port = src_network.port(i);
    ymuint bw = bn_port->bit_width();
    vector<SbjNode*> sbj_bits(bw);
    for (ymuint j = 0; j < bw; ++ j) {
      const BnNode* bn_node = bn_port->bit(j);
      SbjHandle handle;
      bool stat = node_map.find(bn_node->id(), handle);
      ASSERT_COND( stat );
      ASSERT_COND( !handle.inv() );
      SbjNode* sbj_node = handle.node();
      sbj_bits[j] = sbj_node;
    }
    dst_network.add_port(bn_port->name(), sbj_bits);
  }
}

END_NAMESPACE_YM_SBJ
