
/// @file GateColor.cc
/// @brief GateColor の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "GateColor.h"


BEGIN_NAMESPACE_YM_LED

//////////////////////////////////////////////////////////////////////
// クラス GateColor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GateColor::GateColor() :
  mFramePen(Qt::SolidLine),
  mBoxPen(Qt::DotLine),
  mGateBrush(Qt::green, Qt::SolidPattern)
{
}

// @brief デストラクタ
GateColor::~GateColor()
{
}

// @brief 輪郭用のペンを得る．
const QPen&
GateColor::frame_pen() const
{
  return mFramePen;
}

// @brief 枠線用のペンを得る．
const QPen&
GateColor::box_pen() const
{
  return mBoxPen;
}

// @brief 塗りつぶし用のブラシを得る．
const QBrush&
GateColor::gate_brush() const
{
  return mGateBrush;
}

// @brief 輪郭用のペンを設定する．
// @param[in] pen 設定するペン
void
GateColor::set_frame_pen(const QPen& pen)
{
  mFramePen = pen;
}

// @brief 枠線用のペンを設定する．
// @param[in] pen 設定するペン
void
GateColor::set_box_pen(const QPen& pen)
{
  mBoxPen = pen;
}

// @brief 塗りつぶし用のブラシを設定する．
void
GateColor::set_gate_brush(const QBrush& brush)
{
  mGateBrush = brush;
}

END_NAMESPACE_YM_LED
