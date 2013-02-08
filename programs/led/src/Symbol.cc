
/// @file Symbol.cc
/// @brief Symbol の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "Symbol.h"
#include "InputSymbol.h"
#include "OutputSymbol.h"
#include "GateSymbol.h"
#include "BufSymbol.h"
#include "NotSymbol.h"
#include "AndSymbol.h"
#include "OrSymbol.h"
#include "XorSymbol.h"


BEGIN_NAMESPACE_YM_LED

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// 描画用の定数                                                     //
//////////////////////////////////////////////////////////////////////

// 三角関数関係の定数
const double pi     = 3.14159265358979323846;
const double sqrt3  = 1.73205080756887729353;

// 基本ユニット
const ymuint kUnit  = 4;

// ドットの大きさ
const ymuint kDotH  = static_cast<ymuint>(1.5 * kUnit);
const ymuint kDotW  = static_cast<ymuint>(1.5 * kUnit);

// バッファ(正三角形)のサイズ
const ymuint kBufH  = static_cast<ymuint>(7.0 * kUnit);
const ymuint kBufW  = static_cast<ymuint>((kBufH / 2.0) * sqrt3);

// 一般のゲート(AND/OR/XOR)のサイズ
const ymuint kGateH =  static_cast<ymuint>(8.0 * kUnit);
const ymuint kGateW =  static_cast<ymuint>(10.0 * kUnit);

// ゲートの上端の Y 座標
const ymuint kGateUY = - static_cast<ymuint>(kGateH / 2.0);

// ゲートの下端の Y 座標
const ymuint kGateLY =   static_cast<ymuint>(kGateH / 2.0);

// 2,3,4,5入力の時の入力のオフセット
// 上下対称なので半分だけ書いてある．
const double kOff[][2] = {
  { static_cast<ymuint>(2.0 * kUnit), static_cast<ymuint>(0.0 * kUnit) },
  { static_cast<ymuint>(2.5 * kUnit), static_cast<ymuint>(0.0 * kUnit) },
  { static_cast<ymuint>(3.0 * kUnit), static_cast<ymuint>(1.0 * kUnit) },
  { static_cast<ymuint>(3.0 * kUnit), static_cast<ymuint>(1.5 * kUnit) }
};

// XORの入力側の円弧のオフセット
const ymuint kXorM  = static_cast<ymuint>(1.0 * kUnit);

// ANDの円弧の半径
const ymuint kAndR  = static_cast<ymuint>(kGateH / 2.0);

// ANDの短辺の長さ
const ymuint kAndL  = static_cast<ymuint>(kGateW - kAndR);

// ORの円弧の半径
const ymuint kOrR   = static_cast<ymuint>(kGateH);

// ORの短辺の長さ
const ymuint kOrL   = static_cast<ymuint>(kGateW - (kOrR / 2.0) * sqrt3);

// XORの円弧の半径
const ymuint kXorR  = static_cast<ymuint>(kGateH);

// XORの短辺の長さ
const ymuint kXorL  = static_cast<ymuint>(kGateW - (kXorR / 2.0) * sqrt3);

// 配線の長さ
const ymuint kWireW = static_cast<ymuint>(2.0 * kUnit);

// 配線の上下の幅
const ymuint kWireH = static_cast<ymuint>(1.0 * kUnit);

// 配線のX軸方向のマージン
const ymuint kWireXmargin = static_cast<ymuint>(1.0 * kUnit);

// 配線のY軸方向のマージン
const ymuint kWireYmargin = static_cast<ymuint>(1.0 * kUnit);

// 入力側のマージン
const ymuint kImargin = static_cast<ymuint>(2.0 * kUnit);

// 出力側のマージン
const ymuint kOmargin = static_cast<ymuint>(2.0 * kUnit);

// 縦方向のマージン
const ymuint kHmargin = static_cast<ymuint>(1.0 * kUnit);

// ゲート線の幅
const ymuint kGateLineWidth = static_cast<ymuint>(2.0);

// 配線の幅
const ymuint kWireLineWidth = static_cast<ymuint>(2.0);

// 枠線の幅
const ymuint kBboxLineWidth = static_cast<ymuint>(1.0);

// 入力シンボルの長さ
const ymuint kInputW = static_cast<ymuint>(10.0 * kUnit);

// 入力シンボルの上下の幅
const ymuint kInputH = static_cast<ymuint>(2.0 * kUnit);

// 出力シンボルの長さ
const ymuint kOutputW = static_cast<ymuint>(10.0 * kUnit);

// 出力シンボルの上下の幅
const ymuint kOutputH = static_cast<ymuint>(2.0 * kUnit);


//////////////////////////////////////////////////////////////////////
// クラス InputSymbol
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
InputSymbol::InputSymbol()
{
}

// @brief デストラクタ
InputSymbol::~InputSymbol()
{
}

// @brief このゲートを囲む最小の矩形を表す左上と右下の点を得る．
QRect
InputSymbol::bounding_box() const
{
  return QRect(0, -(kInputH / 2), kInputW, kInputH);
}

// @brief 入力数を得る．
ymuint
InputSymbol::ipin_num() const
{
  return 0;
}

// @brief pos 番目の入力ピン位置を得る．
// @param[in] pos 入力番号 ( 0 <= pos < input_num() )
QPoint
InputSymbol::ipin_location(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return QPoint();
}

// @brief 出力数を得る．
ymuint
InputSymbol::opin_num() const
{
  return 1;
}

// @brief pos 番目の出力ピン位置を得る．
// @param[in] pos 出力番号 ( 0 <= pos < output_num() )
QPoint
InputSymbol::opin_location(ymuint pos) const
{
  assert_cond( pos == 0, __FILE__, __LINE__);
  return QPoint(kInputW, 0);
}

// @brief 描画を行う．
// @param[in] painter 描画を行うオブジェクト
// @param[in] rect 描画矩形
void
InputSymbol::draw(QPainter& painter,
		  const QRect& rect) const
{
  const double l_x = 0;
  const double m_x = kInputW - (kInputH / 2.0);
  const double r_x = kInputW;
  const double u_y = -(kInputH / 2);
  const double m_y = 0;
  const double l_y = (kInputH / 2);

  const ymuint kNumPoints = 5;
  QPoint path[kNumPoints];
  path[0] = QPoint(l_x, u_y);
  path[1] = QPoint(m_x, u_y);
  path[2] = QPoint(r_x, m_y);
  path[3] = QPoint(m_x, l_y);
  path[4] = QPoint(l_x, l_y);

  painter.drawPolygon(path, kNumPoints);
}

END_NAMESPACE_YM_LED
