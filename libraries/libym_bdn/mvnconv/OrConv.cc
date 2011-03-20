
/// @file libym_bdn/mvnconv/OrConv.cc
/// @brief OrConv の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "OrConv.h"
#include "ym_mvn/MvNode.h"
#include "ym_bdn/MvnBdnMap.h"
#include "ym_bdn/BdNetwork.h"
#include "ym_bdn/BdnNodeHandle.h"


BEGIN_NAMESPACE_YM_BDN

// @brief コンストラクタ
OrConv::OrConv()
{
}

// @brief デストラクタ
OrConv::~OrConv()
{
}

// @brief MvNode を BdNetwork に変換する．
// @param[in] node ノード
// @param[in] bdnetwork 変換結果の BdNetwork
// @param[in] nodemap ノードの対応関係を表すマップ
// @retval true このクラスで変換処理を行った．
// @retval false このクラスでは変換処理を行わなかった．
bool
OrConv::operator()(const MvNode* node,
		   BdNetwork& bdnetwork,
		   MvnBdnMap& nodemap)
{
  if ( node->type() == MvNode::kOr ) {
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
      const MvInputPin* ipin = node->input(i);
      const MvOutputPin* src_pin = ipin->src_pin();
      const MvNode* src_node = src_pin->node();
      assert_cond( src_pin->bit_width() == bw, __FILE__, __LINE__);
      for (ymuint b = 0; b < bw; ++ b) {
	input_list_array[b][i] = nodemap.get(src_node, b);
      }
    }
    for (ymuint b = 0; b < bw; ++ b) {
      BdnNodeHandle handle = bdnetwork.new_or(input_list_array[b]);
      nodemap.put(node, b, handle);
    }
    return true;
  }
  return false;
}

END_NAMESPACE_YM_BDN
