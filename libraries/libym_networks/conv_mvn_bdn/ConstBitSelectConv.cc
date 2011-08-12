
/// @file ConstBitSelectConv.cc
/// @brief ConstBitSelectConv の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ConstBitSelectConv.h"
#include "ym_networks/MvnNode.h"
#include "ym_networks/MvnBdnMap.h"
#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnNodeHandle.h"


BEGIN_NAMESPACE_YM_NETWORKSBDNCONV

// @brief コンストラクタ
ConstBitSelectConv::ConstBitSelectConv()
{
}

// @brief デストラクタ
ConstBitSelectConv::~ConstBitSelectConv()
{
}

// @brief MvnNode を BdnMgr に変換する．
// @param[in] node ノード
// @param[in] bdnetwork 変換結果の BdnMgr
// @param[in] nodemap ノードの対応関係を表すマップ
// @retval true このクラスで変換処理を行った．
// @retval false このクラスでは変換処理を行わなかった．
bool
ConstBitSelectConv::operator()(const MvnNode* node,
			       BdnMgr& bdnetwork,
			       MvnBdnMap& nodemap)
{
  if ( node->type() == MvnNode::kConstBitSelect ) {
    const MvnInputPin* ipin = node->input(0);
    const MvnNode* src_node = ipin->src_node();

    BdnNodeHandle handle0 = nodemap.get(src_node, node->bitpos());
    nodemap.put(node, handle0);
    return true;
  }
  return false;
}

END_NAMESPACE_YM_NETWORKSBDNCONV
