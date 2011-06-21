
/// @file libym_networks/RorConv.cc
/// @brief RorConv の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "RorConv.h"
#include "ym_networks/MvnNode.h"
#include "ym_networks/MvnBdnMap.h"
#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnNodeHandle.h"


BEGIN_NAMESPACE_YM_NETWORKSBDNCONV

// @brief コンストラクタ
RorConv::RorConv()
{
}

// @brief デストラクタ
RorConv::~RorConv()
{
}

// @brief MvnNode を BdnMgr に変換する．
// @param[in] node ノード
// @param[in] bdnetwork 変換結果の BdnMgr
// @param[in] nodemap ノードの対応関係を表すマップ
// @retval true このクラスで変換処理を行った．
// @retval false このクラスでは変換処理を行わなかった．
bool
RorConv::operator()(const MvnNode* node,
		    BdnMgr& bdnetwork,
		    MvnBdnMap& nodemap)
{
  if ( node->type() == MvnNode::kRor ) {
    const MvnInputPin* ipin0 = node->input(0);
    const MvnOutputPin* src_pin0 = ipin0->src_pin();
    const MvnNode* src_node0 = src_pin0->node();

    ymuint bw = src_pin0->bit_width();
    vector<BdnNodeHandle> input_list(bw);
    for (ymuint i = 0; i < bw; ++ i) {
      input_list[i] = nodemap.get(src_node0, i);
    }
    BdnNodeHandle handle = bdnetwork.new_or(input_list);
    nodemap.put(node, 0, handle);
    return true;
  }
  return false;
}

END_NAMESPACE_YM_NETWORKSBDNCONV
