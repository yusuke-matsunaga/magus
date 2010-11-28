
/// @file libym_mvn/conv/XorConv.cc
/// @brief XorConv の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "XorConv.h"
#include "ym_mvn/MvNode.h"
#include "ym_mvn/MvNodeMap.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
XorConv::XorConv()
{
}

// @brief デストラクタ
XorConv::~XorConv()
{
}

// @brief MvNode を SbjGraph に変換する．
// @param[in] node ノード
// @param[in] sbjgraph 変換結果のサブジェクトグラフ
// @param[in] nodemap ノードの対応関係を表すマップ
// @retval true このクラスで変換処理を行った．
// @retval false このクラスでは変換処理を行わなかった．
bool
XorConv::conv(const MvNode* node,
	      SbjGraph& sbjgraph,
	      MvNodeMap& nodemap)
{
  if ( node->type() == MvNode::kXor ) {
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
      SbjNode* sbjnode0;
      bool inv0;
      bool stat0 = nodemap.get(src_node0, i, sbjnode0, inv0);
      SbjNode* sbjnode1;
      bool inv1;
      bool stat1 = nodemap.get(src_node1, i, sbjnode1, inv1);
      assert_cond( stat0 && stat1 , __FILE__, __LINE__);
      bool inv = false;
      SbjNode* sbjnode = make_xor(sbjgraph, sbjnode0, sbjnode1,
				  inv0, inv1, inv);
      nodemap.put(node, i, sbjnode, inv);
    }
    return true;
  }
  return false;
}

END_NAMESPACE_YM_MVN
