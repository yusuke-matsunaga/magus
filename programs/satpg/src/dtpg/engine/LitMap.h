#ifndef LITMAP_H
#define LITMAP_H

/// @file LitMap.h
/// @brief LitMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "YmLogic/Literal.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class LitMap LitMap.h "LitMap.h"
/// @brief make_gate_cnf で用いられるファンクター
///
/// ゲートの入力に対応するリテラルを返すファクターオブジェクト
//////////////////////////////////////////////////////////////////////
struct LitMap
{

  /// @brief 入力数を返す．
  virtual
  ymuint
  input_size() const = 0;

  /// @brief 入力のリテラルを返す．
  /// @param[in] pos 入力位置 ( 0 <= pos < input_size() )
  virtual
  Literal
  input(ymuint pos) const = 0;

  /// @brief 出力のリテラルを返す．
  virtual
  Literal
  output() const = 0;

};

END_NAMESPACE_YM_SATPG

#endif // LITMAP_H
