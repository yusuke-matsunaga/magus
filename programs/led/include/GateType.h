#ifndef GATETYPE_H
#define GATETYPE_H

/// @file GateType.h
/// @brief GateType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "led_nsdef.h"


BEGIN_NAMESPACE_YM_LED

//////////////////////////////////////////////////////////////////////
/// @brief ゲートの種類を表す列挙型
//////////////////////////////////////////////////////////////////////
enum GateType
{
  /// @brief 入力
  kGtInput,
  /// @brief 出力
  kGtOutput,
  /// @brief バッファ
  kGtBuffer,
  /// @brief NOTゲート
  kGtNot,
  /// @brief ANDゲート
  kGtAnd,
  /// @brief NANDゲート
  kGtNand,
  /// @brief ORゲート
  kGtOr,
  /// @brief NORゲート
  kGtNor,
  /// @brief XORゲート
  kGtXor,
  /// @brief XNORゲート
  kGtXnor
};

END_NAMESPACE_YM_LED

#endif // GATETYPE_H
