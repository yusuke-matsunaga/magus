
/// @file CmplConv.cc
/// @brief CmplConv の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "CmplConv.h"
#include "YmNetworks/MvnNode.h"
#include "YmNetworks/MvnBdnMap.h"
#include "YmNetworks/BdnMgr.h"
#include "YmNetworks/BdnNodeHandle.h"


BEGIN_NAMESPACE_YM_NETWORKSBDNCONV

// @brief コンストラクタ
CmplConv::CmplConv()
{
}

// @brief デストラクタ
CmplConv::~CmplConv()
{
}

// @brief MvnNode を BdnMgr に変換する．
// @param[in] node ノード
// @param[in] bdnetwork 変換結果の BdnMgr
// @param[in] nodemap ノードの対応関係を表すマップ
// @retval true このクラスで変換処理を行った．
// @retval false このクラスでは変換処理を行わなかった．
bool
CmplConv::operator()(const MvnNode* node,
		     BdnMgr& bdnetwork,
		     MvnBdnMap& nodemap)
{
  if ( node->type() == MvnNode::kCmpl ) {
    const MvnInputPin* ipin0 = node->input(0);
    const MvnNode* src_node0 = ipin0->src_node();

    ymuint bw = node->bit_width();
    ASSERT_COND( src_node0->bit_width() == bw );

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

END_NAMESPACE_YM_NETWORKSBDNCONV
