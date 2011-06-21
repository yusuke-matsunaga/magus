
/// @file libym_networks/SraConv.cc
/// @brief SraConv の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "SraConv.h"
#include "ym_networks/MvnNode.h"
#include "ym_networks/MvnBdnMap.h"
#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnNodeHandle.h"


BEGIN_NAMESPACE_YM_NETWORKSBDNCONV

// @brief コンストラクタ
SraConv::SraConv()
{
}

// @brief デストラクタ
SraConv::~SraConv()
{
}

// @brief MvnNode を BdnMgr に変換する．
// @param[in] node ノード
// @param[in] bdnetwork 変換結果の BdnMgr
// @param[in] nodemap ノードの対応関係を表すマップ
// @retval true このクラスで変換処理を行った．
// @retval false このクラスでは変換処理を行わなかった．
bool
SraConv::operator()(const MvnNode* node,
		    BdnMgr& bdnetwork,
		    MvnBdnMap& nodemap)
{
  if ( node->type() == MvnNode::kSra ) {
    cerr << "Converter for SRA is not implemented" << endl;
#if 0
    const MvInputPin* ipin0 = node->input(0);
    const MvOutputPin* src_pin0 = ipin0->src_pin();
    const MvnNode* src_node0 = src_pin0->node();

    const MvInputPin* ipin1 = node->input(1);
    const MvOutputPin* src_pin1 = ipin1->src_pin();
    const MvnNode* src_node1 = src_pin1->node();

    ymuint bw = node->output(0)->bit_width();
    assert_cond( src_pin0->bit_width() == bw, __FILE__, __LINE__);
    assert_cond( src_pin1->bit_width() == bw, __FILE__, __LINE__);
    for (ymuint i = 0; i < bw; ++ i) {
      BdnNodeHandle handle0 = nodemap.get(src_node0, i);
      BdnNodeHandle handle1 = nodemap.get(src_node1, i);
      BdnNodeHandle handle = bdnetwork.new_and(handle0, handle1);
      nodemap.put(node, i, handle);
    }
#endif
    return true;
  }
  return false;
}

END_NAMESPACE_YM_NETWORKSBDNCONV
