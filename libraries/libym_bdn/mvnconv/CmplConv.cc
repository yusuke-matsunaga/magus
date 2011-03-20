
/// @file libym_mvn/conv/CmplConv.cc
/// @brief CmplConv の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CmplConv.h"
#include "ym_mvn/MvNode.h"
#include "ym_bdn/MvnBdnMap.h"
#include "ym_bdn/BdNetwork.h"
#include "ym_bdn/BdnNodeHandle.h"


BEGIN_NAMESPACE_YM_BDN

// @brief コンストラクタ
CmplConv::CmplConv()
{
}

// @brief デストラクタ
CmplConv::~CmplConv()
{
}

// @brief MvNode を BdNetwork に変換する．
// @param[in] node ノード
// @param[in] bdnetwork 変換結果の BdNetwork
// @param[in] nodemap ノードの対応関係を表すマップ
// @retval true このクラスで変換処理を行った．
// @retval false このクラスでは変換処理を行わなかった．
bool
CmplConv::operator()(const MvNode* node,
		     BdNetwork& bdnetwork,
		     MvnBdnMap& nodemap)
{
  if ( node->type() == MvNode::kCmpl ) {
    const MvInputPin* ipin0 = node->input(0);
    const MvOutputPin* src_pin0 = ipin0->src_pin();
    const MvNode* src_node0 = src_pin0->node();

    ymuint bw = node->output(0)->bit_width();
    assert_cond( src_pin0->bit_width() == bw, __FILE__, __LINE__);
    BdnNodeHandle cin(BdnNodeHandle::make_one());
    vector<BdnNodeHandle> tmp_list(3);
    for (ymuint i = 0; i < bw; ++ i) {
      BdnNodeHandle a = ~(nodemap.get(src_node0, i));
      BdnNodeHandle x = bdnetwork.new_xor(a, cin);
      cin = bdnetwork.new_and(a, cin);
      BdnNodeHandle cout = bdnetwork.new_or(tmp_list);

      nodemap.put(node, i, x);
    }
    return true;
  }
  return false;
}

END_NAMESPACE_YM_BDN
