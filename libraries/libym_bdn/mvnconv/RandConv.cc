
/// @file libym_bdn/mvnconv/RandConv.cc
/// @brief RandConv の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "RandConv.h"
#include "ym_mvn/MvNode.h"
#include "ym_bdn/MvnBdnMap.h"
#include "ym_bdn/BdNetwork.h"
#include "ym_bdn/BdnNodeHandle.h"


BEGIN_NAMESPACE_YM_BDN

// @brief コンストラクタ
RandConv::RandConv()
{
}

// @brief デストラクタ
RandConv::~RandConv()
{
}

// @brief MvNode を BdNetwork に変換する．
// @param[in] node ノード
// @param[in] bdnetwork 変換結果の BdNetwork
// @param[in] nodemap ノードの対応関係を表すマップ
// @retval true このクラスで変換処理を行った．
// @retval false このクラスでは変換処理を行わなかった．
bool
RandConv::operator()(const MvNode* node,
		     BdNetwork& bdnetwork,
		     MvnBdnMap& nodemap)
{
  if ( node->type() == MvNode::kRand ) {
    const MvInputPin* ipin0 = node->input(0);
    const MvOutputPin* src_pin0 = ipin0->src_pin();
    const MvNode* src_node0 = src_pin0->node();

    ymuint bw = src_pin0->bit_width();
    vector<BdnNodeHandle> input_list(bw);
    for (ymuint i = 0; i < bw; ++ i) {
      input_list[i] = nodemap.get(src_node0, i);
    }
    BdnNodeHandle handle = bdnetwork.new_and(input_list);
    nodemap.put(node, 0, handle);
    return true;
  }
  return false;
}

END_NAMESPACE_YM_BDN
