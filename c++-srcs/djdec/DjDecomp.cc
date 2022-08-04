
/// @file DjDecomp.cc
/// @brief DjDecomp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "DjDecomp.h"


BEGIN_NAMESPACE_DG

// @brief BDD タイプのノードを disjoint decomposition する．
BnNetwork
DjDecomp::decomp(
  const BnNetwork& src_network ///< [in] 元のネットワーク
)
{
  // 結果のネットワーク
  // 最初は src_network そのもののコピー
  BnNetwork dst_network{src_network};


}

END_NAMESPACE_DG
