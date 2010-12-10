
/// @file libym_mvn/conv/SrlConv.cc
/// @brief SrlConv の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SrlConv.h"
#include "ym_mvn/MvNode.h"
#include "ym_mvn/MvNodeMap.h"
#include "ym_sbj/SbjGraph.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
SrlConv::SrlConv()
{
}

// @brief デストラクタ
SrlConv::~SrlConv()
{
}

// @brief MvNode を SbjGraph に変換する．
// @param[in] node ノード
// @param[in] sbjgraph 変換結果のサブジェクトグラフ
// @param[in] nodemap ノードの対応関係を表すマップ
// @retval true このクラスで変換処理を行った．
// @retval false このクラスでは変換処理を行わなかった．
bool
SrlConv::operator()(const MvNode* node,
		    SbjGraph& sbjgraph,
		    MvNodeMap& nodemap)
{
  if ( node->type() == MvNode::kSrl ) {
    cerr << "Converter for SRL is not implemented" << endl;
#if 0
    const MvInputPin* ipin0 = node->input(0);
    const MvOutputPin* src_pin0 = ipin0->src_pin();
    const MvNode* src_node0 = src_pin0->node();

    const MvInputPin* ipin1 = node->input(1);
    const MvOutputPin* src_pin1 = ipin1->src_pin();
    const MvNode* src_node1 = src_pin1->node();

    ymuint bw = node->output(0)->bit_width();
    assert_cond( src_pin0->bit_width() == bw, __FILE__, __LINE__);
    assert_cond( src_pin1->bit_width() == bw, __FILE__, __LINE__);
    for (ymuint i = 0; i < bw; ++ i) {
      SbjHandle sbjhandle0 = nodemap.get(src_node0, i);
      SbjHandle sbjhandle1 = nodemap.get(src_node1, i);
      SbjHandle sbjhandle = sbjgraph.new_and(sbjhandle0, sbjhandle1);
      nodemap.put(node, i, sbjhandle);
    }
#endif
    return true;
  }
  return false;
}

END_NAMESPACE_YM_MVN
