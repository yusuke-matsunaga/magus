
/// @file libym_mvn/conv/ThroughConv.cc
/// @brief ThroughConv の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ThroughConv.h"
#include "ym_mvn/MvNode.h"
#include "ym_mvn/MvNodeMap.h"
#include "ym_sbj/SbjGraph.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
ThroughConv::ThroughConv()
{
}

// @brief デストラクタ
ThroughConv::~ThroughConv()
{
}

// @brief MvNode を SbjGraph に変換する．
// @param[in] node ノード
// @param[in] sbjgraph 変換結果のサブジェクトグラフ
// @param[in] nodemap ノードの対応関係を表すマップ
// @retval true このクラスで変換処理を行った．
// @retval false このクラスでは変換処理を行わなかった．
bool
ThroughConv::conv(const MvNode* node,
		  SbjGraph& sbjgraph,
		  MvNodeMap& nodemap)
{
  if ( node->type() == MvNode::kThrough ) {
    const MvInputPin* ipin = node->input(0);
    const MvOutputPin* opin = ipin->src_pin();
    const MvNode* src_node = opin->node();
    ymuint bw = opin->bit_width();
    for (ymuint i = 0; i < bw; ++ i) {
      SbjHandle sbjhandle = nodemap.get(src_node, i);
      nodemap.put(node, i, sbjhandle);
    }
    return true;
  }
  return false;
}

END_NAMESPACE_YM_MVN
