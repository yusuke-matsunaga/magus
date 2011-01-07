
/// @file libym_mvn/conv/AndConv.cc
/// @brief AndConv の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "AndConv.h"
#include "ym_mvn/MvNode.h"
#include "ym_mvn/MvNodeMap.h"
#include "ym_sbj/SbjGraph.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
AndConv::AndConv()
{
}

// @brief デストラクタ
AndConv::~AndConv()
{
}

// @brief MvNode を SbjGraph に変換する．
// @param[in] node ノード
// @param[in] sbjgraph 変換結果のサブジェクトグラフ
// @param[in] nodemap ノードの対応関係を表すマップ
// @retval true このクラスで変換処理を行った．
// @retval false このクラスでは変換処理を行わなかった．
bool
AndConv::operator()(const MvNode* node,
		    SbjGraph& sbjgraph,
		    MvNodeMap& nodemap)
{
  if ( node->type() == MvNode::kAnd ) {
    ymuint ni = node->input_num();
    assert_cond( ni >= 2, __FILE__, __LINE__);
    ymuint no = node->output_num();
    assert_cond( no == 1, __FILE__, __LINE__);

    ymuint bw = node->output(0)->bit_width();
    vector<vector<SbjHandle> > input_list_array(bw);
    for (ymuint b = 0; b < bw; ++ b) {
      input_list_array[b].resize(ni);
    }
    for (ymuint i = 0; i < ni; ++ i) {
      const MvInputPin* ipin = node->input(i);
      const MvOutputPin* src_pin = ipin->src_pin();
      const MvNode* src_node = src_pin->node();
      assert_cond( src_pin->bit_width() == bw, __FILE__, __LINE__);
      for (ymuint b = 0; b < bw; ++ b) {
	input_list_array[b][i] = nodemap.get(src_node, b);
      }
    }
    for (ymuint b = 0; b < bw; ++ b) {
      SbjHandle sbjhandle = sbjgraph.new_and(input_list_array[b]);
      nodemap.put(node, b, sbjhandle);
    }
    return true;
  }
  return false;
}

END_NAMESPACE_YM_MVN
