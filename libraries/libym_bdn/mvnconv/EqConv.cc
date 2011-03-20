
/// @file libym_bdn/mvnconv/EqConv.cc
/// @brief EqConv の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "EqConv.h"
#include "ym_mvn/MvNode.h"
#include "ym_bdn/MvnBdnMap.h"
#include "ym_bdn/BdNetwork.h"
#include "ym_bdn/BdnNodeHandle.h"


BEGIN_NAMESPACE_YM_BDN

// @brief コンストラクタ
EqConv::EqConv()
{
}

// @brief デストラクタ
EqConv::~EqConv()
{
}

// @brief MvNode を BdNetwork に変換する．
// @param[in] node ノード
// @param[in] bdnetwork 変換結果の BdNetwork
// @param[in] nodemap ノードの対応関係を表すマップ
// @retval true このクラスで変換処理を行った．
// @retval false このクラスでは変換処理を行わなかった．
bool
EqConv::operator()(const MvNode* node,
		   BdNetwork& bdnetwork,
		   MvnBdnMap& nodemap)
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
    vector<BdnNodeHandle> input_array(bw);
    for (ymuint i = 0; i < bw; ++ i) {
      BdnNodeHandle handle0 = nodemap.get(src_node0, i);
      BdnNodeHandle handle1 = nodemap.get(src_node1, i);
      input_array[i] = bdnetwork.new_xnor(handle0, handle1);
    }

    BdnNodeHandle handle = bdnetwork.new_and(input_array);
    nodemap.put(node, 0, handle);

    return true;
  }
  return false;
}

END_NAMESPACE_YM_BDN
