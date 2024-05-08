#ifndef CELLMAP_H
#define CELLMAP_H

/// @file CellMap.h
/// @brief CellMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "magus.h"
#include "ym/bnet.h"
#include "ym/clib.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class CellMap CellMap.h "CellMap.h"
/// @brief セルライブラリ用ののテクノロジマッピングを行うクラス
///
/// 実はクラスである必要はない．
//////////////////////////////////////////////////////////////////////
class CellMap
{
public:
  //////////////////////////////////////////////////////////////////////
  // テクノロジマッピングを行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 面積優先でテクノロジマッピングを行う．
  /// @return マッピング結果を表す BnNetwork
  static
  BnNetwork
  area_map(
    const BnNetwork& src_network,        ///< [in] マッピング対象のネットワーク
    const ClibCellLibrary& cell_library, ///< [in] セルライブラリ
    const string option = {}             ///< [in] オプションを表すJSON文字列
  );

};

END_NAMESPACE_MAGUS

#endif // CELLMAP_H
