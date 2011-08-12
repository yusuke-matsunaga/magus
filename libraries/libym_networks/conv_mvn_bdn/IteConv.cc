
/// @file libym_networks/IteConv.cc
/// @brief IteConv の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "IteConv.h"
#include "ym_networks/MvnNode.h"
#include "ym_networks/MvnBdnMap.h"
#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnNodeHandle.h"


BEGIN_NAMESPACE_YM_NETWORKSBDNCONV

// @brief コンストラクタ
IteConv::IteConv()
{
}

// @brief デストラクタ
IteConv::~IteConv()
{
}

// @brief MvnNode を BdnNetwork に変換する．
// @param[in] node ノード
// @param[in] bdnetwork 変換結果のサブジェクトグラフ
// @param[in] nodemap ノードの対応関係を表すマップ
// @retval true このクラスで変換処理を行った．
// @retval false このクラスでは変換処理を行わなかった．
bool
IteConv::operator()(const MvnNode* node,
		    BdnMgr& bdnetwork,
		    MvnBdnMap& nodemap)
{
  if ( node->type() == MvnNode::kIte ) {
    const MvnInputPin* ipin0 = node->input(0);
    const MvnNode* src_node0 = ipin0->src_node();

    const MvnInputPin* ipin1 = node->input(1);
    const MvnNode* src_node1 = ipin1->src_node();

    const MvnInputPin* ipin2 = node->input(2);
    const MvnNode* src_node2 = ipin2->src_node();

    ymuint bw = node->bit_width();
    assert_cond( src_node1->bit_width() == bw, __FILE__, __LINE__);
    assert_cond( src_node2->bit_width() == bw, __FILE__, __LINE__);
    BdnNodeHandle handle0 = nodemap.get(src_node0);

    for (ymuint i = 0; i < bw; ++ i) {
      BdnNodeHandle handle1 = nodemap.get(src_node1, i);
      BdnNodeHandle handle2 = nodemap.get(src_node2, i);
      BdnNodeHandle and1 = bdnetwork.new_and(handle0, handle1);
      BdnNodeHandle and2 = bdnetwork.new_and(~handle0, handle2);
      BdnNodeHandle or1 = bdnetwork.new_or(and1, and2);
      nodemap.put(node, i, or1);
    }
    return true;
  }
  return false;
}

END_NAMESPACE_YM_NETWORKSBDNCONV
