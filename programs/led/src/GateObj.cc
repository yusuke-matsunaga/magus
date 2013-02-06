
/// @file GateObj.cc
/// @brief GateObj の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2006, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "GateObj.h"
#include "GateTemplate.h"


BEGIN_NAMESPACE_YM_GLV

// @brief 入力数を得る．
ymuint
GateObj::input_num() const
{
  return mGateTemplate->input_num();
}

// @brief 出力数を得る．
ymuint
GateObj::output_num() const
{
  return mGateTemplate->output_num();
}

// @brief このゲートを囲む最小の矩形を表す左上と右下の点を得る．
QRect
GateObj::bounding_box() const
{
  Qrect tmp = mGateTemplate->bounding_box();
  return tmp.move(mLocation);
}

// @brief pos 番目の入力ピン位置を得る．
QPoint
GateObj::ipin_location(ymuint pos) const
{
  QPoint tmp = mGateTemplate->ipin_location(pos);
  return tmp.move(mLocation);
}

// @brief pos 番目の出力ピン位置を得る．
QPoint
GateObj::opin_location(ymuint pos) const
{
  QPoint tmp = mGateTemplate->opin_location(pos);
  return tmp.move(mLocation);
}

END_NAMESPACE_YM_GLV
