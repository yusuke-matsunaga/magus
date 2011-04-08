
/// @file libym_mvnbdnconv/RxorConv.cc
/// @brief RxorConv の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "RxorConv.h"
#include "ym_mvn/MvnNode.h"
#include "ym_mvnbdnconv/MvnBdnMap.h"
#include "ym_bdn/BdNetwork.h"
#include "ym_bdn/BdnNodeHandle.h"


BEGIN_NAMESPACE_YM_MVNBDNCONV

// @brief コンストラクタ
RxorConv::RxorConv()
{
}

// @brief デストラクタ
RxorConv::~RxorConv()
{
}

// @brief MvnNode を BdNetwork に変換する．
// @param[in] node ノード
// @param[in] bdnetwork 変換結果の BdNetwork
// @param[in] nodemap ノードの対応関係を表すマップ
// @retval true このクラスで変換処理を行った．
// @retval false このクラスでは変換処理を行わなかった．
bool
RxorConv::operator()(const MvnNode* node,
		     BdNetwork& bdnetwork,
		     MvnBdnMap& nodemap)
{
  if ( node->type() == MvnNode::kRxor ) {
    const MvnInputPin* ipin0 = node->input(0);
    const MvnOutputPin* src_pin0 = ipin0->src_pin();
    const MvnNode* src_node0 = src_pin0->node();

    ymuint bw = src_pin0->bit_width();
    vector<BdnNodeHandle> input_list(bw);
    for (ymuint i = 0; i < bw; ++ i) {
      input_list[i] = nodemap.get(src_node0, i);
    }
    BdnNodeHandle handle = bdnetwork.new_xor(input_list);
    nodemap.put(node, 0, handle);
    return true;
  }
  return false;
}

END_NAMESPACE_YM_MVNBDNCONV
