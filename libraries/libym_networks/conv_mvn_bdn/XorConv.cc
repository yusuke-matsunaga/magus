
/// @file libym_networks/XorConv.cc
/// @brief XorConv の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "XorConv.h"
#include "ym_networks/MvnNode.h"
#include "ym_networks/MvnBdnMap.h"
#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnNodeHandle.h"


BEGIN_NAMESPACE_YM_NETWORKSBDNCONV

// @brief コンストラクタ
XorConv::XorConv()
{
}

// @brief デストラクタ
XorConv::~XorConv()
{
}

// @brief MvnNode を BdnMgr に変換する．
// @param[in] node ノード
// @param[in] sbjgraph 変換結果の BdnMgr
// @param[in] nodemap ノードの対応関係を表すマップ
// @retval true このクラスで変換処理を行った．
// @retval false このクラスでは変換処理を行わなかった．
bool
XorConv::operator()(const MvnNode* node,
		    BdnMgr& bdnetwork,
		    MvnBdnMap& nodemap)
{
  if ( node->type() == MvnNode::kXor ) {
    ymuint ni = node->input_num();
    assert_cond( ni >= 2, __FILE__, __LINE__);
    ymuint no = node->output_num();
    assert_cond( no == 1, __FILE__, __LINE__);

    ymuint bw = node->output(0)->bit_width();
    vector<vector<BdnNodeHandle> > input_list_array(bw);
    for (ymuint b = 0; b < bw; ++ b) {
      input_list_array[b].resize(ni);
    }
    for (ymuint i = 0; i < ni; ++ i) {
      const MvnInputPin* ipin = node->input(i);
      const MvnOutputPin* src_pin = ipin->src_pin();
      const MvnNode* src_node = src_pin->node();
      assert_cond( src_pin->bit_width() == bw, __FILE__, __LINE__);
      for (ymuint b = 0; b < bw; ++ b) {
	input_list_array[b][i] = nodemap.get(src_node, b);
      }
    }
    for (ymuint b = 0; b < bw; ++ b) {
      BdnNodeHandle handle = bdnetwork.new_xor(input_list_array[b]);
      nodemap.put(node, b, handle);
    }
    return true;
  }
  return false;
}

END_NAMESPACE_YM_NETWORKSBDNCONV
