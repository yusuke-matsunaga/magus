
/// @file CellMap.cc
/// @brief CellMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "CellMap.h"
#include "AreaCover.h"
#include "SbjGraph.h"


BEGIN_NAMESPACE_CELLMAP

// @brief コンストラクタ
CellMap::CellMap()
{
}

// @brief デストラクタ
CellMap::~CellMap()
{
}

// @brief 面積最小化 DAG covering のヒューリスティック関数
// @param[in] cell_library セルライブラリ
// @param[in] src_network もとのネットワーク
// @param[in] mode モード
//  - 0: fanout フロー, resub なし
//  - 1: weighted フロー, resub なし
//  - 2: fanout フロー, resub あり
//  - 3: weighted フロー, resub あり
// @param[out] mapnetwork マッピング結果
BnNetwork
CellMap::area_map(
  const ClibCellLibrary& cell_library,
  const BnNetwork& src_network,
  ymuint mode
)
{
  AreaCover area_cover;
  SbjGraph sbjgraph;
  return area_cover(sbjgraph, cell_library);
}

END_NAMESPACE_CELLMAP
