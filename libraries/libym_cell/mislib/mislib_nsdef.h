#ifndef LIBYM_CELL_MISLIB_MISLIB_NSDEF_H
#define LIBYM_CELL_MISLIB_MISLIB_NSDEF_H

/// @file libym_cell/mislib/mislib_nsdef.h
/// @brief mislib サブモジュールの定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"


//////////////////////////////////////////////////////////////////////
// 名前空間を定義するためのマクロ
//////////////////////////////////////////////////////////////////////

/// @brief mislib の名前空間の開始
#define BEGIN_NAMESPACE_YM_CELL_MISLIB \
BEGIN_NAMESPACE_YM_CELL \
BEGIN_NAMESPACE(nsMislib)

/// @brief mislib の名前空間の終了
#define END_NAMESPACE_YM_CELL_MISLIB \
END_NAMESPACE(nsMislib) \
END_NAMESPACE_YM_CELL


BEGIN_NAMESPACE_YM_CELL_MISLIB

/// @brief ノードの種類
enum tType {
  /// @brief 文字列
  kStr,
  /// @brief 数値
  kNum,
  /// @brief NONINV 極性
  kNoninv,
  /// @brief INV 極性
  kInv,
  /// @brief UNKOWN 極性
  kUnknown,
  /// @brief 定数0
  kConst0,
  /// @brief 定数1
  kConst1,
  /// @brief リスト
  kList,
  /// @brief NOT論理式
  kNot,
  /// @brief AND論理式
  kAnd,
  /// @brief OR論理式
  kOr,
  /// @brief XOR論理式
  kXor,
  /// @brief 入力ピン
  kPin,
  /// @brief ゲート
  kGate
};

// クラス名の前方宣言
class MislibParser;
class MislibPt;

END_NAMESPACE_YM_CELL_MISLIB

#endif // LIBYM_CELL_MISLIB_MISLIB_NSDEF_H
