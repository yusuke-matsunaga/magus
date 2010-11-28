
/// @file libym_mvn/conv/NotConv.cc
/// @brief NotConv の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "NotConv.h"
#include "ym_mvn/MvNode.h"
#include "ym_mvn/MvNodeMap.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
NotConv::NotConv()
{
}

// @brief デストラクタ
NotConv::~NotConv()
{
}

// @brief MvNode を SbjGraph に変換する．
// @param[in] node ノード
// @param[in] sbjgraph 変換結果のサブジェクトグラフ
// @param[in] nodemap ノードの対応関係を表すマップ
// @retval true このクラスで変換処理を行った．
// @retval false このクラスでは変換処理を行わなかった．
bool
NotConv::conv(const MvNode* node,
	      SbjGraph& sbjgraph,
	      MvNodeMap& nodemap)
{
  if ( node->type() == MvNode::kNot ) {
    const MvInputPin* ipin = node->input(0);
    const MvOutputPin* opin = ipin->src_pin();
    const MvNode* src_node = opin->node();
    ymuint bw = opin->bit_width();
    assert_cond( node->output(0)->bit_width(), __FILE__, __LINE__);
    for (ymuint i = 0; i < bw; ++ i) {
      SbjNode* sbjnode;
      bool inv;
      bool stat = nodemap.get(src_node, i, sbjnode, inv);
      assert_cond( stat , __FILE__, __LINE__);
      nodemap.put(node, i, sbjnode, !inv);
    }
    return true;
  }
  return false;
}

END_NAMESPACE_YM_MVN
