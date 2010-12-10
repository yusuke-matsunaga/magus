
/// @file libym_mvn/conv/SubConv.cc
/// @brief SubConv の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SubConv.h"
#include "ym_mvn/MvNode.h"
#include "ym_mvn/MvNodeMap.h"
#include "ym_sbj/SbjGraph.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
SubConv::SubConv()
{
}

// @brief デストラクタ
SubConv::~SubConv()
{
}

// @brief MvNode を SbjGraph に変換する．
// @param[in] node ノード
// @param[in] sbjgraph 変換結果のサブジェクトグラフ
// @param[in] nodemap ノードの対応関係を表すマップ
// @retval true このクラスで変換処理を行った．
// @retval false このクラスでは変換処理を行わなかった．
bool
SubConv::operator()(const MvNode* node,
		    SbjGraph& sbjgraph,
		    MvNodeMap& nodemap)
{
  if ( node->type() == MvNode::kSub ) {
    const MvInputPin* ipin0 = node->input(0);
    const MvOutputPin* src_pin0 = ipin0->src_pin();
    const MvNode* src_node0 = src_pin0->node();

    const MvInputPin* ipin1 = node->input(1);
    const MvOutputPin* src_pin1 = ipin1->src_pin();
    const MvNode* src_node1 = src_pin1->node();

    ymuint bw = node->output(0)->bit_width();
    assert_cond( src_pin0->bit_width() == bw, __FILE__, __LINE__);
    assert_cond( src_pin1->bit_width() == bw, __FILE__, __LINE__);
    SbjHandle cin(SbjHandle::make_one());
    vector<SbjHandle> tmp_list(3);
    for (ymuint i = 0; i < bw; ++ i) {
      SbjHandle a = nodemap.get(src_node0, i);
      SbjHandle b = ~nodemap.get(src_node1, i);

      tmp_list[0] = a;
      tmp_list[1] = b;
      tmp_list[2] = cin;
      SbjHandle sum = sbjgraph.new_xor(tmp_list);

      tmp_list[0] = sbjgraph.new_and(a, b);
      tmp_list[1] = sbjgraph.new_and(a, cin);
      tmp_list[2] = sbjgraph.new_and(b, cin);
      SbjHandle cout = sbjgraph.new_or(tmp_list);

      nodemap.put(node, i, sum);
      cin = cout;
    }
    return true;
  }
  return false;
}

END_NAMESPACE_YM_MVN
