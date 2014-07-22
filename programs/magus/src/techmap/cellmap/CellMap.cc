
/// @file libym_techmap/cellmap/CellMap.cc
/// @brief CellMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CellMap.h"
#include "AreaCover.h"


BEGIN_NAMESPACE_YM_CELLMAP

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
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] mode モード
//  - 0: fanout フロー, resub なし
//  - 1: weighted フロー, resub なし
//  - 2: fanout フロー, resub あり
//  - 3: weighted フロー, resub あり
// @param[out] mapnetwork マッピング結果
void
CellMap::area_map(const CellLibrary& cell_library,
		  const BdnMgr& sbjgraph,
		  ymuint mode,
		  CmnMgr& mapnetwork)
{
  AreaCover area_cover;

  area_cover(sbjgraph, cell_library, mapnetwork);
}

END_NAMESPACE_YM_CELLMAP
