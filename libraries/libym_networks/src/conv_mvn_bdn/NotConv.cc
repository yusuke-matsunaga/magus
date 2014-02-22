
/// @file NotConv.cc
/// @brief NotConv の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "NotConv.h"
#include "networks/MvnNode.h"
#include "networks/MvnBdnMap.h"
#include "networks/BdnMgr.h"
#include "networks/BdnNodeHandle.h"


BEGIN_NAMESPACE_YM_NETWORKSBDNCONV

// @brief コンストラクタ
NotConv::NotConv()
{
}

// @brief デストラクタ
NotConv::~NotConv()
{
}

// @brief MvnNode を BdnMgr に変換する．
// @param[in] node ノード
// @param[in] bdnetwork 変換結果の BdnMgr
// @param[in] nodemap ノードの対応関係を表すマップ
// @retval true このクラスで変換処理を行った．
// @retval false このクラスでは変換処理を行わなかった．
bool
NotConv::operator()(const MvnNode* node,
		    BdnMgr& bdnetwork,
		    MvnBdnMap& nodemap)
{
  if ( node->type() == MvnNode::kNot ) {
    const MvnInputPin* ipin = node->input(0);
    const MvnNode* src_node = ipin->src_node();
    ymuint bw = src_node->bit_width();
    assert_cond( node->bit_width(), __FILE__, __LINE__);
    for (ymuint i = 0; i < bw; ++ i) {
      BdnNodeHandle handle = nodemap.get(src_node, i);
      nodemap.put(node, i, ~handle);
    }
    return true;
  }
  return false;
}

END_NAMESPACE_YM_NETWORKSBDNCONV
