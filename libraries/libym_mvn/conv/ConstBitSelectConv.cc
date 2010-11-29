
/// @file libym_mvn/conv/ConstBitSelectConv.cc
/// @brief ConstBitSelectConv の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ConstBitSelectConv.h"
#include "ym_mvn/MvNode.h"
#include "ym_mvn/MvNodeMap.h"
#include "ym_sbj/SbjGraph.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
ConstBitSelectConv::ConstBitSelectConv()
{
}

// @brief デストラクタ
ConstBitSelectConv::~ConstBitSelectConv()
{
}

// @brief MvNode を SbjGraph に変換する．
// @param[in] node ノード
// @param[in] sbjgraph 変換結果のサブジェクトグラフ
// @param[in] nodemap ノードの対応関係を表すマップ
// @retval true このクラスで変換処理を行った．
// @retval false このクラスでは変換処理を行わなかった．
bool
ConstBitSelectConv::conv(const MvNode* node,
			 SbjGraph& sbjgraph,
			 MvNodeMap& nodemap)
{
  if ( node->type() == MvNode::kConstBitSelect ) {
    const MvInputPin* ipin = node->input(0);
    const MvOutputPin* src_pin = ipin->src_pin();
    const MvNode* src_node = src_pin->node();

    SbjHandle sbjhandle0 = nodemap.get(src_node, node->bitpos());
    nodemap.put(node, sbjhandle0);
    return true;
  }
  return false;
}

END_NAMESPACE_YM_MVN
