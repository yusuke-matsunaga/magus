
/// @file Bn2Sbj.cc
/// @brief Bn2Sbj の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "Bn2Sbj.h"
#include "SbjGraph.h"
#include "ym/BnNetwork.h"
#include "ym/BnNode.h"
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
#if 0
  if ( !src_network.wrap_up() ) {
    // もとのネットワークが適切でなかった．
    return;
  }
#endif

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

  // DFFノードの生成
  ymuint ndff = src_network.dff_num();
  for (ymuint i = 0; i < ndff; ++ i) {
    const BnNode* bn_node = src_network.dff(i);
    SbjNode* sbj_node = dst_network.new_dff();
    node_map.add(bn_node->id(), SbjHandle(sbj_node));
  }

  // 論理ノードの生成
  ymuint nl = src_network.logic_num();
  for (ymuint i = 0; i < nl; ++ i) {
    const BnNode* bn_node = src_network.logic(i);
    ymuint ni = bn_node->fanin_num();
    vector<SbjHandle> ihandle_list(ni);
    for (ymuint j = 0; j < ni; ++ j) {
      const BnNode* bn_inode = src_network.find_node(bn_node->fanin(j));
      ASSERT_COND( bn_inode != nullptr );
      bool stat = node_map.find(bn_inode->id(), ihandle_list[j]);
      ASSERT_COND( stat );
    }
    SbjHandle sbj_handle;
    BnLogicType logic_type = bn_node->logic_type();
    switch ( logic_type ) {
    kBnLt_NONE:
      ASSERT_NOT_REACHED;
      break;

    kBnLt_C0:
      sbj_handle = SbjHandle::make_zero();
      break;

    kBnLt_C1:
      sbj_handle = SbjHandle::make_one();
      break;

    kBnLt_BUFF:
      sbj_handle = ihandle_list[0];
      break;

    kBnLt_NOT:
      sbj_handle = ~ihandle_list[0];
      break;

    kBnLt_AND:
      sbj_handle = dst_network.new_and(ihandle_list);
      break;

    kBnLt_NAND:
      sbj_handle = ~dst_network.new_and(ihandle_list);
      break;

    kBnLt_OR:
      sbj_handle = dst_network.new_or(ihandle_list);
      break;

    kBnLt_NOR:
      sbj_handle = ~dst_network.new_or(ihandle_list);
      break;

    kBnLt_XOR:
      sbj_handle = dst_network.new_xor(ihandle_list);
      break;

    kBnLt_XNOR:
      sbj_handle = ~dst_network.new_xor(ihandle_list);
      break;

    kBnLt_EXPR:
      sbj_handle = dst_network.new_logic(bn_node->expr(), ihandle_list);
      break;

    kBnLt_TV:
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
    const BnNode* bn_inode = src_network.find_node(bn_node->input());
    ASSERT_COND( bn_inode != nullptr );
    SbjHandle ihandle;
    bool stat = node_map.find(bn_inode->id(), ihandle);
    ASSERT_COND( stat );
    SbjNode* sbj_node = dst_network.new_output(ihandle);
    node_map.add(bn_node->id(), SbjHandle(sbj_node));
  }

  // DFFノードの入力の設定
  for (ymuint i = 0; i < ndff; ++ i) {
    const BnNode* bn_node = src_network.dff(i);
    const BnNode* bn_inode = src_network.find_node(bn_node->input());
    ASSERT_COND( bn_inode != nullptr );
    SbjHandle ohandle;
    bool stat1 = node_map.find(bn_node->id(), ohandle);
    ASSERT_COND( stat1 );
    ASSERT_COND( !ohandle.inv() );
    SbjNode* sbj_node = ohandle.node();
    ASSERT_COND( sbj_node->is_dff() );

    SbjHandle ihandle;
    bool stat2 = node_map.find(bn_inode->id(), ihandle);
    ASSERT_COND( stat2 );

    dst_network.set_dff_data(sbj_node, ihandle);
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
