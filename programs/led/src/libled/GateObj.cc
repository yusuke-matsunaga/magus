
/// @file GateObj.cc
/// @brief GateObj の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2006, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "GateObj.h"
#include "Symbol.h"


BEGIN_NAMESPACE_YM_LED

// @brief 入力数を得る．
ymuint
GateObj::ipin_num() const
{
  return mSymbol->ipin_num();
}

// @brief 出力数を得る．
ymuint
GateObj::opin_num() const
{
  return mSymbol->opin_num();
}

// @brief このゲートを囲む最小の矩形を表す左上と右下の点を得る．
QRect
GateObj::bounding_box() const
{
  QRect tmp = mSymbol->bounding_box();
  tmp.translate(mPosition);
  return tmp;
}

// @brief pos 番目の入力ピン位置を得る．
QPoint
GateObj::ipin_location(ymuint pos) const
{
  QPoint tmp = mSymbol->ipin_location(pos);
  return tmp += mPosition;
}

// @brief pos 番目の出力ピン位置を得る．
QPoint
GateObj::opin_location(ymuint pos) const
{
  QPoint tmp = mSymbol->opin_location(pos);
  return tmp += mPosition;
}

END_NAMESPACE_YM_LED
