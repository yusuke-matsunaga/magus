
/// @file libym_mvn/conv/EqConv.cc
/// @brief EqConv の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EqConv.h"
#include "ym_mvn/MvNode.h"
#include "ym_mvn/MvNodeMap.h"
#include "ym_sbj/SbjGraph.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
EqConv::EqConv()
{
}

// @brief デストラクタ
EqConv::~EqConv()
{
}

// @brief MvNode を SbjGraph に変換する．
// @param[in] node ノード
// @param[in] sbjgraph 変換結果のサブジェクトグラフ
// @param[in] nodemap ノードの対応関係を表すマップ
// @retval true このクラスで変換処理を行った．
// @retval false このクラスでは変換処理を行わなかった．
bool
EqConv::conv(const MvNode* node,
	     SbjGraph& sbjgraph,
	     MvNodeMap& nodemap)
{
  if ( node->type() == MvNode::kEq ) {
    const MvInputPin* ipin0 = node->input(0);
    const MvOutputPin* src_pin0 = ipin0->src_pin();
    const MvNode* src_node0 = src_pin0->node();

    const MvInputPin* ipin1 = node->input(1);
    const MvOutputPin* src_pin1 = ipin1->src_pin();
    const MvNode* src_node1 = src_pin1->node();

    ymuint bw = src_pin0->bit_width();
    assert_cond( src_pin1->bit_width() == bw, __FILE__, __LINE__);
    vector<SbjHandle> input_array(bw);
    for (ymuint i = 0; i < bw; ++ i) {
      SbjHandle sbjhandle0 = nodemap.get(src_node0, i);
      SbjHandle sbjhandle1 = nodemap.get(src_node1, i);
      input_array[i] = ~sbjgraph.new_xor(sbjhandle0, sbjhandle1);
    }

    SbjHandle sbjhandle = sbjgraph.new_and(input_array);
    nodemap.put(node, 0, sbjhandle);

    return true;
  }
  return false;
}

END_NAMESPACE_YM_MVN
