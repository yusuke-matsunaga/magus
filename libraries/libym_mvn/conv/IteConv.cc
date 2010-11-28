
/// @file libym_mvn/conv/IteConv.cc
/// @brief IteConv の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "IteConv.h"
#include "ym_mvn/MvNode.h"
#include "ym_mvn/MvNodeMap.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
IteConv::IteConv()
{
}

// @brief デストラクタ
IteConv::~IteConv()
{
}

// @brief MvNode を SbjGraph に変換する．
// @param[in] node ノード
// @param[in] sbjgraph 変換結果のサブジェクトグラフ
// @param[in] nodemap ノードの対応関係を表すマップ
// @retval true このクラスで変換処理を行った．
// @retval false このクラスでは変換処理を行わなかった．
bool
IteConv::conv(const MvNode* node,
	      SbjGraph& sbjgraph,
	      MvNodeMap& nodemap)
{
  if ( node->type() == MvNode::kIte ) {
    const MvInputPin* ipin0 = node->input(0);
    const MvOutputPin* src_pin0 = ipin0->src_pin();
    const MvNode* src_node0 = src_pin0->node();

    const MvInputPin* ipin1 = node->input(1);
    const MvOutputPin* src_pin1 = ipin1->src_pin();
    const MvNode* src_node1 = src_pin1->node();

    const MvInputPin* ipin2 = node->input(2);
    const MvOutputPin* src_pin2 = ipin2->src_pin();
    const MvNode* src_node2 = src_pin2->node();

    ymuint bw = node->output(0)->bit_width();
    assert_cond( src_pin1->bit_width() == bw, __FILE__, __LINE__);
    assert_cond( src_pin2->bit_width() == bw, __FILE__, __LINE__);
    SbjNode* sbjnode0;
    bool inv0;
    bool stat0 = nodemap.get(src_node0, sbjnode0, inv0);
    assert_cond( stat0, __FILE__, __LINE__);

    for (ymuint i = 0; i < bw; ++ i) {
      SbjNode* sbjnode1;
      bool inv1;
      bool stat1 = nodemap.get(src_node1, i, sbjnode1, inv1);
      SbjNode* sbjnode2;
      bool inv2;
      bool stat2 = nodemap.get(src_node2, i, sbjnode2, inv2);
      assert_cond( stat1 && stat2 , __FILE__, __LINE__);
      bool and1_inv = false;
      SbjNode* and1 = make_and(sbjgraph, sbjnode0, sbjnode1,
			       inv0, inv1, and1_inv);
      bool and2_inv = false;
      SbjNode* and2 = make_and(sbjgraph, sbjnode0, sbjnode2,
			       !inv0, inv2, and2_inv);
      bool or1_inv = false;
      SbjNode* or1 = make_or(sbjgraph, and1, and2,
			     and1_inv, and2_inv, or1_inv);
      nodemap.put(node, i, or1, or1_inv);
    }
    return true;
  }
  return false;
}

END_NAMESPACE_YM_MVN
