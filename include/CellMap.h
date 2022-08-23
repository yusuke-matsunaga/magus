#ifndef CELLMAP_H
#define CELLMAP_H

/// @file CellMap.h
/// @brief CellMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016 Yusuke Matsunaga
/// All rights reserved.

#include "cellmap_nsdef.h"
#include "ym/bnet.h"
#include "ym/clib.h"


BEGIN_NAMESPACE_CELLMAP

//////////////////////////////////////////////////////////////////////
/// @class CellMap CellMap.h "CellMap.h"
/// @brief セルライブラリ用ののテクノロジマッピングを行うクラス
//////////////////////////////////////////////////////////////////////
class CellMap
{
public:

  /// @brief コンストラクタ
  CellMap();

  /// @brief デストラクタ
  ~CellMap();


public:
  //////////////////////////////////////////////////////////////////////
  // テクノロジマッピングを行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 面積最小化 DAG covering のヒューリスティック関数
  /// @param[in] cell_library セルライブラリ
  /// @param[in] src_network もとのネットワーク
  /// @param[in] mode モード
  ///  - 0: fanout フロー, resub なし
  ///  - 1: weighted フロー, resub なし
  ///  - 2: fanout フロー, resub あり
  ///  - 3: weighted フロー, resub あり
  /// @param[out] mapnetwork マッピング結果
  BnNetwork
  area_map(
    const ClibCellLibrary& cell_library,
    const BnNetwork& src_network,
    ymuint mode
  );

#if 0
  /// @brief 段数最小化 DAG covering のヒューリスティック関数
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] limit カットサイズ
  /// @param[in] slack 最小段数に対するスラック
  /// @param[in] mode モード
  ///  - 0: fanout フロー, resub なし
  ///  - 1: weighted フロー, resub なし
  ///  - 2: fanout フロー, resub あり
  ///  - 3: weighted フロー, resub あり
  /// @param[out] mapnetwork マッピング結果
  /// @param[out] lut_num LUT数
  /// @param[out] depth 段数
  void
  delay_map(const BnNetwork& sbjgraph,
	    ymuint limit,
	    ymuint slack,
	    ymuint mode,
	    BnNetwork& mapnetwork);
#endif

};

END_NAMESPACE_CELLMAP

#endif // CELLMAP_H
