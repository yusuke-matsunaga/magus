
/// @file libym_networks/CaseEqConv.cc
/// @brief CaseEqConv の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CaseEqConv.h"
#include "ym_networks/MvnNode.h"
#include "ym_networks/MvnBdnMap.h"
#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnNodeHandle.h"


BEGIN_NAMESPACE_YM_NETWORKSBDNCONV

// @brief コンストラクタ
CaseEqConv::CaseEqConv()
{
}

// @brief デストラクタ
CaseEqConv::~CaseEqConv()
{
}

// @brief MvnNode を BdnMgr に変換する．
// @param[in] node ノード
// @param[in] bdnetwork 変換結果の BdnMgr
// @param[in] nodemap ノードの対応関係を表すマップ
// @retval true このクラスで変換処理を行った．
// @retval false このクラスでは変換処理を行わなかった．
bool
CaseEqConv::operator()(const MvnNode* node,
		       BdnMgr& bdnetwork,
		       MvnBdnMap& nodemap)
{
  if ( node->type() == MvnNode::kCaseEq ) {
    vector<ymuint32> xmask;
    node->xmask(xmask);

    const MvnInputPin* ipin0 = node->input(0);
    const MvnOutputPin* src_pin0 = ipin0->src_pin();
    const MvnNode* src_node0 = src_pin0->node();

    const MvnInputPin* ipin1 = node->input(1);
    const MvnOutputPin* src_pin1 = ipin1->src_pin();
    const MvnNode* src_node1 = src_pin1->node();

    ymuint bw = src_pin0->bit_width();
    assert_cond( src_pin1->bit_width() == bw, __FILE__, __LINE__);
    vector<BdnNodeHandle> input_array;
    input_array.reserve(bw);
    for (ymuint i = 0; i < bw; ++ i) {
      ymuint blk = i / 32;
      ymuint bit = (1U << (i % 32));
      if ( xmask[blk] & bit ) {
	// ドントケアのビットはスキップする．
	continue;
      }
      BdnNodeHandle handle0 = nodemap.get(src_node0, i);
      BdnNodeHandle handle1 = nodemap.get(src_node1, i);
      BdnNodeHandle handle2 = bdnetwork.new_xnor(handle0, handle1);
      input_array.push_back(handle2);
    }
    BdnNodeHandle handle = bdnetwork.new_and(input_array);
    nodemap.put(node, 0, handle);

    return true;
  }
  return false;
}

END_NAMESPACE_YM_NETWORKSBDNCONV
