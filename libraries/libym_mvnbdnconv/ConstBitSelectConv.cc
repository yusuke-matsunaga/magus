
/// @file libym_mvnbdnconv/ConstBitSelectConv.cc
/// @brief ConstBitSelectConv の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ConstBitSelectConv.h"
#include "ym_mvn/MvNode.h"
#include "ym_mvnbdnconv/MvnBdnMap.h"
#include "ym_bdn/BdNetwork.h"
#include "ym_bdn/BdnNodeHandle.h"


BEGIN_NAMESPACE_YM_MVNBDNCONV

// @brief コンストラクタ
ConstBitSelectConv::ConstBitSelectConv()
{
}

// @brief デストラクタ
ConstBitSelectConv::~ConstBitSelectConv()
{
}

// @brief MvNode を BdNetwork に変換する．
// @param[in] node ノード
// @param[in] bdnetwork 変換結果の BdNetwork
// @param[in] nodemap ノードの対応関係を表すマップ
// @retval true このクラスで変換処理を行った．
// @retval false このクラスでは変換処理を行わなかった．
bool
ConstBitSelectConv::operator()(const MvNode* node,
			       BdNetwork& bdnetwork,
			       MvnBdnMap& nodemap)
{
  if ( node->type() == MvNode::kConstBitSelect ) {
    const MvInputPin* ipin = node->input(0);
    const MvOutputPin* src_pin = ipin->src_pin();
    const MvNode* src_node = src_pin->node();

    BdnNodeHandle handle0 = nodemap.get(src_node, node->bitpos());
    nodemap.put(node, handle0);
    return true;
  }
  return false;
}

END_NAMESPACE_YM_MVNBDNCONV
