
/// @file ConstPartSelectConv.cc
/// @brief ConstPartSelectConv の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ConstPartSelectConv.h"
#include "YmNetworks/MvnNode.h"
#include "YmNetworks/MvnBdnMap.h"
#include "YmNetworks/BdnMgr.h"
#include "YmNetworks/BdnNodeHandle.h"


BEGIN_NAMESPACE_YM_NETWORKSBDNCONV

// @brief コンストラクタ
ConstPartSelectConv::ConstPartSelectConv()
{
}

// @brief デストラクタ
ConstPartSelectConv::~ConstPartSelectConv()
{
}

// @brief MvnNode を BdnNetwork に変換する．
// @param[in] node ノード
// @param[in] bdnetwork 変換結果の BdnMgr
// @param[in] nodemap ノードの対応関係を表すマップ
// @retval true このクラスで変換処理を行った．
// @retval false このクラスでは変換処理を行わなかった．
bool
ConstPartSelectConv::operator()(const MvnNode* node,
				BdnMgr& bdnetwork,
				MvnBdnMap& nodemap)
{
  if ( node->type() == MvnNode::kConstPartSelect ) {
    const MvnInputPin* ipin = node->input(0);
    const MvnNode* src_node = ipin->src_node();

    ymuint bw = node->bit_width();
    ymuint msb = node->msb();
    ymuint lsb = node->lsb();
    ASSERT_COND( bw == msb - lsb + 1 );
    for (ymuint i = 0; i < bw; ++ i) {
      BdnNodeHandle handle0 = nodemap.get(src_node, i + lsb);
      nodemap.put(node, i, handle0);
    }
    return true;
  }
  return false;
}

END_NAMESPACE_YM_NETWORKSBDNCONV
