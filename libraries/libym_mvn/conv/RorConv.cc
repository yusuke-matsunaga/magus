
/// @file libym_mvn/conv/RorConv.cc
/// @brief RorConv の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "RorConv.h"
#include "ym_mvn/MvNode.h"
#include "ym_mvn/MvNodeMap.h"
#include "ym_sbj/SbjGraph.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
RorConv::RorConv()
{
}

// @brief デストラクタ
RorConv::~RorConv()
{
}

// @brief MvNode を SbjGraph に変換する．
// @param[in] node ノード
// @param[in] sbjgraph 変換結果のサブジェクトグラフ
// @param[in] nodemap ノードの対応関係を表すマップ
// @retval true このクラスで変換処理を行った．
// @retval false このクラスでは変換処理を行わなかった．
bool
RorConv::operator()(const MvNode* node,
		    SbjGraph& sbjgraph,
		    MvNodeMap& nodemap)
{
  if ( node->type() == MvNode::kRor ) {
    const MvInputPin* ipin0 = node->input(0);
    const MvOutputPin* src_pin0 = ipin0->src_pin();
    const MvNode* src_node0 = src_pin0->node();

    ymuint bw = src_pin0->bit_width();
    vector<SbjHandle> input_list(bw);
    for (ymuint i = 0; i < bw; ++ i) {
      input_list[i] = nodemap.get(src_node0, i);
    }
    SbjHandle sbjhandle = sbjgraph.new_or(input_list);
    nodemap.put(node, 0, sbjhandle);
    return true;
  }
  return false;
}

END_NAMESPACE_YM_MVN
