
/// @file libym_networks/AddConv.cc
/// @brief AddConv の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "AddConv.h"
#include "ym_networks/MvnNode.h"
#include "ym_networks/MvnBdnMap.h"
#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnNodeHandle.h"


BEGIN_NAMESPACE_YM_NETWORKSBDNCONV

// @brief コンストラクタ
AddConv::AddConv()
{
}

// @brief デストラクタ
AddConv::~AddConv()
{
}

// @brief MvnNode を BdnMgr に変換する．
// @param[in] node ノード
// @param[in] bdnetwork 変換結果の BdnMgr
// @param[in] nodemap ノードの対応関係を表すマップ
// @retval true このクラスで変換処理を行った．
// @retval false このクラスでは変換処理を行わなかった．
bool
AddConv::operator()(const MvnNode* node,
		    BdnMgr& bdnetwork,
		    MvnBdnMap& nodemap)
{
  if ( node->type() == MvnNode::kAdd ) {
    const MvnInputPin* ipin0 = node->input(0);
    const MvnOutputPin* src_pin0 = ipin0->src_pin();
    const MvnNode* src_node0 = src_pin0->node();

    const MvnInputPin* ipin1 = node->input(1);
    const MvnOutputPin* src_pin1 = ipin1->src_pin();
    const MvnNode* src_node1 = src_pin1->node();

    ymuint bw = node->output(0)->bit_width();
    assert_cond( src_pin0->bit_width() == bw, __FILE__, __LINE__);
    assert_cond( src_pin1->bit_width() == bw, __FILE__, __LINE__);
    BdnNodeHandle cin(BdnNodeHandle::make_zero());
    vector<BdnNodeHandle> tmp_list(3);
    for (ymuint i = 0; i < bw; ++ i) {
      BdnNodeHandle a = nodemap.get(src_node0, i);
      BdnNodeHandle b = nodemap.get(src_node1, i);

      tmp_list[0] = a;
      tmp_list[1] = b;
      tmp_list[2] = cin;
      BdnNodeHandle sum = bdnetwork.new_xor(tmp_list);

      tmp_list[0] = bdnetwork.new_and(a, b);
      tmp_list[1] = bdnetwork.new_and(a, cin);
      tmp_list[2] = bdnetwork.new_and(b, cin);
      BdnNodeHandle cout = bdnetwork.new_or(tmp_list);

      nodemap.put(node, i, sum);
      cin = cout;
    }
    return true;
  }
  return false;
}

END_NAMESPACE_YM_NETWORKSBDNCONV
