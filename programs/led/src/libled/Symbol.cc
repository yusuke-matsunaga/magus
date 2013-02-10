
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
#include "AndSymbol.h"
#include "OrSymbol.h"
#include "XorSymbol.h"

#include <math.h>


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
  return new OutputSymbol();
}

// @brief バッファを生成する．
Symbol*
Symbol::buffer_symbol()
{
  return new BufSymbol();
}

// @brief AND ゲートを生成する．
// @param[in] ni 入力数
Symbol*
Symbol::and_symbol(ymuint ni)
{
  return new AndSymbol(ni);
}

// @brief OR ゲートを生成する．
// @param[in] ni 入力数
Symbol*
Symbol::or_symbol(ymuint ni)
{
  return new OrSymbol(ni);
}

// @brief XOR ゲートを生成する．
// @param[in] ni 入力数
Symbol*
Symbol::xor_symbol(ymuint ni)
{
  return new XorSymbol(ni);
}

// @brief 出力に否定のドットをつけたシンボルを生成する．
// @param[in] src_symbol もととなるシンボル．
Symbol*
Symbol::output_inv_symbol(Symbol* src_symbol)
{
}

// @brief 入力に否定のドットをつけたシンボルを生成する．
// @param[in] src_symbol もととなるシンボル．
// @param[in] pols 入力の否定の情報 (0 で肯定，1 で否定)
Symbol*
Symbol::input_inv_symbol(Symbol* src_symbol,
			 const vector<int>& pols)
{
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
  return QRect(0, -(kInputH / 2.0), kInputW, kInputH);
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


//////////////////////////////////////////////////////////////////////
// クラス OutputSymbol
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
OutputSymbol::OutputSymbol()
{
}

// @brief デストラクタ
OutputSymbol::~OutputSymbol()
{
}

// @brief このゲートを囲む最小の矩形を表す左上と右下の点を得る．
QRect
OutputSymbol::bounding_box() const
{
  return QRect(0, -(kOutputH / 2.0), kOutputW, kOutputH);
}

// @brief 入力数を得る．
ymuint
OutputSymbol::ipin_num() const
{
  return 1;
}

// @brief pos 番目の入力ピン位置を得る．
// @param[in] pos 入力番号 ( 0 <= pos < input_num() )
QPoint
OutputSymbol::ipin_location(ymuint pos) const
{
  assert_cond( pos == 0, __FILE__, __LINE__);
  return QPoint(0, 0);
}

// @brief 出力数を得る．
ymuint
OutputSymbol::opin_num() const
{
  return 0;
}

// @brief pos 番目の出力ピン位置を得る．
// @param[in] pos 出力番号 ( 0 <= pos < output_num() )
QPoint
OutputSymbol::opin_location(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return QPoint();
}

// @brief 描画を行う．
// @param[in] painter 描画を行うオブジェクト
void
OutputSymbol::draw(QPainter& painter) const
{
  const double l_x = 0;
  const double m_x = kOutputW - (kOutputH / 2.0);
  const double r_x = kOutputW;
  const double u_y = -(kOutputH / 2);
  const double m_y = 0;
  const double l_y = (kOutputH / 2);

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


//////////////////////////////////////////////////////////////////////
// クラス BufSymbol
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BufSymbol::BufSymbol()
{
}

// @brief デストラクタ
BufSymbol::~BufSymbol()
{
}

// @brief このゲートを囲む最小の矩形を表す左上と右下の点を得る．
QRect
BufSymbol::bounding_box() const
{
  return QRect(0, -(kBufH / 2.0), kBufW, kBufH);
}

// @brief 入力数を得る．
ymuint
BufSymbol::ipin_num() const
{
  return 1;
}

// @brief pos 番目の入力ピン位置を得る．
// @param[in] pos 入力番号 ( 0 <= pos < input_num() )
QPoint
BufSymbol::ipin_location(ymuint pos) const
{
  assert_cond( pos == 0, __FILE__, __LINE__);
  return QPoint(0, 0);
}

// @brief 出力数を得る．
ymuint
BufSymbol::opin_num() const
{
  return 1;
}

// @brief pos 番目の出力ピン位置を得る．
// @param[in] pos 出力番号 ( 0 <= pos < output_num() )
QPoint
BufSymbol::opin_location(ymuint pos) const
{
  assert_cond( pos == 0, __FILE__, __LINE__);
  return QPoint(kBufW, 0);
}

// @brief 描画を行う．
// @param[in] painter 描画を行うオブジェクト
void
BufSymbol::draw(QPainter& painter) const
{
  const double l_x = 0;
  const double r_x = kBufW;
  const double u_y = -(kBufH / 2);
  const double m_y = 0;
  const double l_y =  (kBufH / 2);

  const QPoint path[] = {
    QPoint(l_x, u_y),
    QPoint(r_x, m_y),
    QPoint(l_x, l_y)
  };
  const ymuint kNumPoints = sizeof(path) / sizeof(QPoint);

  painter.drawPolygon(path, kNumPoints);
}


//////////////////////////////////////////////////////////////////////
// クラス GateSymbol
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] ni 入力数
GateSymbol::GateSymbol(ymuint ni) :
  mInputNum(ni)
{
}

/// @brief デストラクタ
GateSymbol::~GateSymbol()
{
}

// @brief このゲートを囲む最小の矩形を表す左上と右下の点を得る．
QRect
GateSymbol::bounding_box() const
{
  return QRect(0, kGateUY, kGateW, kGateH);
}

// @brief 入力数を得る．
ymuint
GateSymbol::ipin_num() const
{
  return mInputNum;
}

// @brief pos 番目の入力ピン位置を得る．
// @param[in] pos 入力番号 ( 0 <= pos < input_num() )
QPoint
GateSymbol::ipin_location(ymuint pos) const
{
}

// @brief 出力数を得る．
ymuint
GateSymbol::opin_num() const
{
  return 1;
}

// @brief pos 番目の出力ピン位置を得る．
// @param[in] pos 出力番号 ( 0 <= pos < output_num() )
QPoint
GateSymbol::opin_location(ymuint pos) const
{
  return QPoint(kGateW, 0);
}


//////////////////////////////////////////////////////////////////////
// クラス AndSymbol
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] ni 入力数
AndSymbol::AndSymbol(ymuint ni) :
  GateSymbol(ni)
{
}

// @brief デストラクタ
AndSymbol::~AndSymbol()
{
}


BEGIN_NONAMESPACE

// 円弧を表す点列を作る．
void
create_arc(double cx,
	   double cy,
	   double r,
	   ymuint ndiv,
	   double start_rad,
	   double end_rad,
	   QVector<QPoint>& points)
{
  double rad = pi * start_rad;
  double diff = end_rad - start_rad;
  double inc = (pi * diff) / ndiv;
  for (size_t i = 1; i < ndiv; i ++) {
    rad += inc;
    double dx = cx + cos(rad) * r;
    double dy = cy + sin(rad) * r;
    points.push_back(QPoint(dx, dy));
  }
}

END_NONAMESPACE

// @brief 描画を行う．
// @param[in] painter 描画を行うオブジェクト
void
AndSymbol::draw(QPainter& painter) const
{
  ymuint ndiv = 24;

  QVector<QPoint> tmp_list;
  tmp_list.push_back(QPoint(0.0, kGateUY));
  tmp_list.push_back(QPoint(kAndL, kGateUY));
  create_arc(kAndL, 0.0, kAndR, ndiv, 1.5, 2.5, tmp_list);
  tmp_list.push_back(QPoint(kAndL, kGateLY));
  tmp_list.push_back(QPoint(0.0, kGateLY));
  QPolygon poly(tmp_list);

  QImage image_buffer(kGateW, kGateH, QImage::Format_ARGB32_Premultiplied);
  QPainter image_painter(&image_buffer);
  //image_painter.initFrom(this);
  image_painter.setRenderHint(QPainter::Antialiasing, true);

  QMatrix matrix;
  matrix.translate(0.0, kGateH / 2.0);
  image_painter.setMatrix(matrix);
  image_painter.drawPolygon(poly);
  image_painter.end();

  painter.drawImage(0, - kGateH / 2.0, image_buffer);
}


//////////////////////////////////////////////////////////////////////
// クラス OrSymbol
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] ni 入力数
OrSymbol::OrSymbol(ymuint ni) :
  GateSymbol(ni)
{
}

// @brief デストラクタ
OrSymbol::~OrSymbol()
{
}

// @brief 描画を行う．
// @param[in] painter 描画を行うオブジェクト
void
OrSymbol::draw(QPainter& painter) const
{
  ymuint ndiv = 16;

  QVector<QPoint> tmp_list;
  tmp_list.push_back(QPoint(0.0, kGateUY));
  tmp_list.push_back(QPoint(kOrL, kGateUY));
  create_arc(kOrL, kGateLY, kOrR, ndiv, 9.0 / 6.0, 11.0 / 6.0, tmp_list);
  tmp_list.push_back(QPoint(kGateW, 0.0));
  create_arc(kOrL, kGateUY, kOrR, ndiv, 1.0 / 6.0, 3.0 / 6.0, tmp_list);
  tmp_list.push_back(QPoint(kOrL, kGateLY));
  tmp_list.push_back(QPoint(0.0, kGateLY));
  double cx = - (kOrR / 2.0) * sqrt3;
  create_arc(cx, 0.0, kOrR, ndiv, 1.0 / 6.0, -1.0 / 6.0, tmp_list);
  QPolygon poly(tmp_list);

  painter.drawPolygon(poly);
}


//////////////////////////////////////////////////////////////////////
// クラス XorSymbol
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] ni 入力数
XorSymbol::XorSymbol(ymuint ni) :
  GateSymbol(ni)
{
}

// @brief デストラクタ
XorSymbol::~XorSymbol()
{
}

// @brief 描画を行う．
// @param[in] painter 描画を行うオブジェクト
void
XorSymbol::draw(QPainter& painter) const
{
}

END_NAMESPACE_YM_LED
