
/// @file GateObj.cc
/// @brief GateObj の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2006, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "GateObj.h"
#include "GateColor.h"
#include "Symbol.h"


BEGIN_NAMESPACE_YM_LED

// @brief コンストラクタ
// @param[in] sym シンボル
// @param[in] pos 位置
GateObj::GateObj(const Symbol* sym,
		 const QPoint& pos) :
  mSymbol(sym),
  mColor(NULL),
  mPosition(pos)
{
}

// @brief コンストラクタ
// @param[in] sym シンボル
// @param[in] x 位置のX座標
// @param[in] y 位置のY座標
GateObj::GateObj(const Symbol* sym,
		 ymuint x,
		 ymuint y) :
  mSymbol(sym),
  mColor(NULL),
  mPosition(x, y)
{
}

// @brief デストラクタ
GateObj::~GateObj()
{
}

// @brief 描画用の設定を行なう．
void
GateObj::set_color(const GateColor* color)
{
  mColor = color;
}

// @brief 位置を設定する．
// @param[in] pos 設定する位置
void
GateObj::set_position(const QPoint& pos)
{
  mPosition = pos;
}

// @brief 位置を設定する．
// @param[in] x 位置のX座標
// @param[in] y 位置のY座標
void
GateObj::set_position(ymuint x,
		      ymuint y)
{
  mPosition.setX(x);
  mPosition.setY(y);
}

// @brief 位置を取得する．
QPoint
GateObj::position() const
{
  return mPosition;
}

// @brief このゲートを囲む最小の矩形を表す左上と右下の点を得る．
QRect
GateObj::bounding_box() const
{
  return mSymbol->bounding_box().translated(mPosition);
}

// @brief 入力数を得る．
ymuint
GateObj::ipin_num() const
{
  return mSymbol->ipin_num();
}

// @brief pos 番目の入力ピン位置を得る．
QPoint
GateObj::ipin_location(ymuint pos) const
{
  QPoint tmp = mSymbol->ipin_location(pos);
  return tmp += mPosition;
}

// @brief 出力数を得る．
ymuint
GateObj::opin_num() const
{
  return mSymbol->opin_num();
}

// @brief pos 番目の出力ピン位置を得る．
QPoint
GateObj::opin_location(ymuint pos) const
{
  QPoint tmp = mSymbol->opin_location(pos);
  return tmp += mPosition;
}

// @brief 描画する．
void
GateObj::draw(QPainter& painter) const
{
  if ( mColor == NULL ) {
    return;
  }

  painter.save();

  QMatrix matrix;
  matrix.translate(mPosition.x(), mPosition.y());
  painter.setMatrix(matrix);

  painter.setPen(mColor->frame_pen());
  painter.setBrush(mColor->gate_brush());
  mSymbol->draw_symbol(painter);

  painter.setPen(mColor->box_pen());
  painter.setBrush(QBrush(Qt::NoBrush));
  mSymbol->draw_box(painter);

  painter.restore();
}

END_NAMESPACE_YM_LED
