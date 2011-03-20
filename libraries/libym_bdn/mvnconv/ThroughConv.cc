
/// @file libym_mvn/conv/ThroughConv.cc
/// @brief ThroughConv の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ThroughConv.h"
#include "ym_mvn/MvNode.h"
#include "ym_bdn/MvnBdnMap.h"
#include "ym_bdn/BdNetwork.h"
#include "ym_bdn/BdnNodeHandle.h"


BEGIN_NAMESPACE_YM_BDN

// @brief コンストラクタ
ThroughConv::ThroughConv()
{
}

// @brief デストラクタ
ThroughConv::~ThroughConv()
{
}

// @brief MvNode を BdNetwork に変換する．
// @param[in] node ノード
// @param[in] bdnetwork 変換結果の BdNetwork
// @param[in] nodemap ノードの対応関係を表すマップ
// @retval true このクラスで変換処理を行った．
// @retval false このクラスでは変換処理を行わなかった．
bool
ThroughConv::operator()(const MvNode* node,
			BdNetwork& sbjgraph,
			MvnBdnMap& nodemap)
{
  if ( node->type() == MvNode::kThrough ) {
    const MvInputPin* ipin = node->input(0);
    const MvOutputPin* opin = ipin->src_pin();
    const MvNode* src_node = opin->node();
    ymuint bw = opin->bit_width();
    for (ymuint i = 0; i < bw; ++ i) {
      BdnNodeHandle handle = nodemap.get(src_node, i);
      nodemap.put(node, i, handle);
    }
    return true;
  }
  return false;
}

END_NAMESPACE_YM_BDN
