
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
// 描画用の定数
//////////////////////////////////////////////////////////////////////

// 三角関数関係の定数
const double pi     = 3.14159265358979323846;
const double sqrt3  = 1.73205080756887729353;

// 基本単位
const double kUnit  = 2;

// ドット(否定)の大きさ
const double kDotH  = 3.0 * kUnit;
const double kDotW  = 3.0 * kUnit;

// バッファ(正三角形)のサイズ
const double kBufH  = 14.0 * kUnit;
const double kBufW  = (kBufH / 2.0) * sqrt3;

// 一般のゲート(AND/OR/XOR)のサイズ
const double kGateH =  16.0 * kUnit;
const double kGateW =  20.0 * kUnit;

// ゲートの上端の Y 座標 (中心からのオフセット)
const double kGateUY = - (kGateH / 2.0);

// ゲートの下端の Y 座標 (中心からのオフセット)
const double kGateLY =   (kGateH / 2.0);

// 2,3,4,5入力の時の入力のオフセット
// 上下対称なので半分だけ書いてある．
const double kOff[][2] = {
  { 4.0 * kUnit, 0.0 * kUnit },
  { 5.0 * kUnit, 0.0 * kUnit },
  { 6.0 * kUnit, 2.0 * kUnit },
  { 6.0 * kUnit, 3.0 * kUnit }
};

// XORの入力側の円弧のオフセット
const double kXorM  = 2.0 * kUnit;

// ANDの円弧の半径
const double kAndR  = kGateH / 2.0;

// ANDの短辺の長さ
const double kAndL  = kGateW - kAndR;

// ORの円弧の半径
const double kOrR   = kGateH;

// ORの短辺の長さ
const double kOrL   = kGateW - (kOrR / 2.0) * sqrt3;

// XORの円弧の半径
const double kXorR  = kGateH;

// XORの短辺の長さ
const double kXorL  = kGateW - (kXorR / 2.0) * sqrt3;

// 配線の長さ
const double kWireW = 4.0 * kUnit;

// 配線の上下の幅
const double kWireH = 2.0 * kUnit;

// 配線のX軸方向のマージン
const double kWireXmargin = 2.0 * kUnit;

// 配線のY軸方向のマージン
const double kWireYmargin = 2.0 * kUnit;

// 入力側のマージン
const double kImargin = 4.0 * kUnit;

// 出力側のマージン
const double kOmargin = 4.0 * kUnit;

// 縦方向のマージン
const double kHmargin = 2.0 * kUnit;

// ゲート線の幅
const double kGateLineWidth = 2.0;

// 配線の幅
const double kWireLineWidth = 2.0;

// 枠線の幅
const double kBboxLineWidth = 1.0;

// 入力シンボルの長さ
const double kInputW = 20.0 * kUnit;

// 入力シンボルの上下の幅
const double kInputH = 4.0 * kUnit;

// 出力シンボルの長さ
const double kOutputW = 20.0 * kUnit;

// 出力シンボルの上下の幅
const double kOutputH = 4.0 * kUnit;

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス Symbol
//////////////////////////////////////////////////////////////////////

// @brief 入力を生成する．
Symbol*
Symbol::input_symbol()
{
  return new InputSymbol();
}

// @brief 出力を生成する．
Symbol*
Symbol::output_symbol()
{
#if 0
  return new OutputSymbol();
#else
  return NULL;
#endif
}

// @brief バッファを生成する．
Symbol*
Symbol::buffer_symbol()
{
#if 0
  return new BufSymbol();
#else
  return NULL;
#endif
}

// @brief インバーター (NOT ゲート) を生成する．
Symbol*
Symbol::not_symbol()
{
#if 0
  return new NotSymbol();
#else
  return NULL;
#endif
}

// @brief AND ゲートを生成する．
// @param[in] ni 入力数
Symbol*
Symbol::and_symbol(ymuint ni)
{
#if 0
  return new AndSymbol(ipols, opol);
#else
  return NULL;
#endif
}

// @brief OR ゲートを生成する．
// @param[in] ni 入力数
Symbol*
Symbol::or_symbol(ymuint ni)
{
#if 0
  return new OrSymbol(ipols, opol);
#else
  return NULL;
#endif
}

// @brief XOR ゲートを生成する．
// @param[in] ni 入力数
Symbol*
Symbol::xor_symbol(ymuint ni)
{
#if 0
  return new XorSymbol(ipols, opol);
#else
  return NULL;
#endif
}


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
InputSymbol::draw(QPainter& painter) const
{
  const double l_x = 0;
  const double m_x = kInputW - (kInputH / 2.0);
  const double r_x = kInputW;
  const double u_y = -(kInputH / 2);
  const double m_y = 0;
  const double l_y = (kInputH / 2);

  const QPoint path[] = {
    QPoint(l_x, u_y),
    QPoint(m_x, u_y),
    QPoint(r_x, m_y),
    QPoint(m_x, l_y),
    QPoint(l_x, l_y)
  };
  const ymuint kNumPoints = sizeof(path) / sizeof(QPoint);

  painter.drawPolygon(path, kNumPoints);
}

END_NAMESPACE_YM_LED
