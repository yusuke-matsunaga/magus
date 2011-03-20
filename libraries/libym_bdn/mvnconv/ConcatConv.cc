
/// @file libym_bdn/mvnconv/ConcatConv.cc
/// @brief ConcatConv の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ConcatConv.h"
#include "ym_mvn/MvNode.h"
#include "ym_bdn/MvnBdnMap.h"
#include "ym_bdn/BdNetwork.h"
#include "ym_bdn/BdnNodeHandle.h"


BEGIN_NAMESPACE_YM_BDN

// @brief コンストラクタ
ConcatConv::ConcatConv()
{
}

// @brief デストラクタ
ConcatConv::~ConcatConv()
{
}

// @brief MvNode を BdNetwork に変換する．
// @param[in] node ノード
// @param[in] bdnetwork 変換結果のサブジェクトグラフ
// @param[in] nodemap ノードの対応関係を表すマップ
// @retval true このクラスで変換処理を行った．
// @retval false このクラスでは変換処理を行わなかった．
bool
ConcatConv::operator()(const MvNode* node,
		       BdNetwork& bdnetwork,
		       MvnBdnMap& nodemap)
{
  if ( node->type() == MvNode::kConcat ) {
    ymuint bw = node->output(0)->bit_width();
    ymuint ni = node->input_num();
    ymuint offset = bw;
    for (ymuint i = 0; i < ni; ++ i) {
      const MvInputPin* ipin = node->input(i);
      const MvOutputPin* opin = ipin->src_pin();
      const MvNode* src_node = opin->node();
      ymuint bw1 = opin->bit_width();
      offset -= bw1;
      for (ymuint j = 0; j < bw1; ++ j) {
	ymuint index = offset + j;
	BdnNodeHandle handle0 = nodemap.get(src_node, j);
	nodemap.put(node, index, handle0);
      }
    }
    assert_cond( offset == 0U, __FILE__, __LINE__);
    return true;
  }
  return false;
}

END_NAMESPACE_YM_BDN
