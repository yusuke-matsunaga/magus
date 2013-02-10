
/// @file SymbolImpl.cc
/// @brief SymbolImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SymbolImpl.h"


BEGIN_NAMESPACE_YM_LED

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// 描画用の定数
//////////////////////////////////////////////////////////////////////

// 三角関数関係の定数
const qreal pi     = 3.14159265358979323846;
const qreal sqrt3  = 1.73205080756887729353;

// 基本単位
const qreal kUnit  = 2;

// ドット(否定)の大きさ
const qreal kDotH  = 3.0 * kUnit;
const qreal kDotW  = 3.0 * kUnit;

// バッファ(正三角形)のサイズ
const qreal kBufH  = 14.0 * kUnit;
const qreal kBufW  = (kBufH / 2.0) * sqrt3;

// 一般のゲート(AND/OR/XOR)のサイズ
const qreal kGateH =  16.0 * kUnit;
const qreal kGateW =  20.0 * kUnit;

// ゲートの上端の Y 座標 (中心からのオフセット)
const qreal kGateUY = - (kGateH / 2.0);

// ゲートの下端の Y 座標 (中心からのオフセット)
const qreal kGateLY =   (kGateH / 2.0);

// 2,3,4,5入力の時の入力のオフセット
// 上下対称なので半分だけ書いてある．
const qreal kOff[][2] = {
  { 4.0 * kUnit, 0.0 * kUnit },
  { 5.0 * kUnit, 0.0 * kUnit },
  { 6.0 * kUnit, 2.0 * kUnit },
  { 6.0 * kUnit, 3.0 * kUnit }
};

// XORの入力側の円弧のオフセット
const qreal kXorM  = 2.0 * kUnit;

// ANDの円弧の半径
const qreal kAndR  = kGateH / 2.0;

// ANDの短辺の長さ
const qreal kAndL  = kGateW - kAndR;

// ORの円弧の半径
const qreal kOrR   = kGateH;

// ORの短辺の長さ
const qreal kOrL   = kGateW - (kOrR / 2.0) * sqrt3;

// XORの円弧の半径
const qreal kXorR  = kGateH;

// XORの短辺の長さ
const qreal kXorL  = kGateW - (kXorR / 2.0) * sqrt3;

// 配線の長さ
const qreal kWireW = 4.0 * kUnit;

// 配線の上下の幅
const qreal kWireH = 2.0 * kUnit;

// 配線のX軸方向のマージン
const qreal kWireXmargin = 2.0 * kUnit;

// 配線のY軸方向のマージン
const qreal kWireYmargin = 2.0 * kUnit;

// 入力側のマージン
const qreal kImargin = 4.0 * kUnit;

// 出力側のマージン
const qreal kOmargin = 4.0 * kUnit;

// 縦方向のマージン
const qreal kHmargin = 2.0 * kUnit;

// ゲート線の幅
const qreal kGateLineWidth = 2.0;

// 配線の幅
const qreal kWireLineWidth = 2.0;

// 枠線の幅
const qreal kBboxLineWidth = 1.0;

// 入力シンボルの長さ
const qreal kInputW = 20.0 * kUnit;

// 入力シンボルの上下の幅
const qreal kInputH = 4.0 * kUnit;

// 出力シンボルの長さ
const qreal kOutputW = 20.0 * kUnit;

// 出力シンボルの上下の幅
const qreal kOutputH = 4.0 * kUnit;

// @brief 円弧を表す点列を points に追加する．
// @param[in] cx, cy 中心の座標
// @param[in] r 半径
// @param[in] ndiv 円弧を折れ線で近似するときの分割数
// @param[in] start_rad 開始位置の角度 (ラジアン)
// @param[in] end_rad 終了位置の角度 (ラジアン)
// @param[in] polygon 追加対象のポリゴン
void
create_arc(qreal cx,
	   qreal cy,
	   qreal r,
	   ymuint ndiv,
	   qreal start_rad,
	   qreal end_rad,
	   QPolygonF& polygon)
{
  qreal rad = pi * start_rad;
  qreal diff = end_rad - start_rad;
  qreal inc = (pi * diff) / ndiv;
  for (size_t i = 1; i < ndiv; i ++) {
    rad += inc;
    qreal dx = cx + cos(rad) * r;
    qreal dy = cy + sin(rad) * r;
    polygon << QPointF(dx, dy);
  }
}

// @brief 入力を表すポリゴンを作る．
// @param[in] polygon 結果を格納するポリゴン
void
create_input(QPolygonF& polygon)
{
  const qreal l_x = 0.0;
  const qreal m_x = kInputW - (kInputH / 2.0);
  const qreal r_x = kInputW;
  const qreal u_y = -(kInputH / 2.0);
  const qreal m_y = 0.0;
  const qreal l_y =  (kInputH / 2.0);

  polygon << QPointF(l_x, u_y)
	  << QPointF(m_x, u_y)
	  << QPointF(r_x, m_y)
	  << QPointF(m_x, l_y)
	  << QPointF(l_x, l_y)
	  << QPointF(l_x, u_y);
}

// @brief 出力を表すポリゴンを作る．
// @param[in] polygon 結果を格納するポリゴン
void
create_output(QPolygonF& polygon)
{
  const qreal l_x = 0;
  const qreal m_x = kOutputW - (kOutputH / 2.0);
  const qreal r_x = kOutputW;
  const qreal u_y = -(kOutputH / 2);
  const qreal c_y = 0;
  const qreal l_y = (kOutputH / 2);

  polygon << QPointF(l_x, u_y)
	  << QPointF(m_x, u_y)
	  << QPointF(r_x, c_y)
	  << QPointF(m_x, l_y)
	  << QPointF(l_x, l_y)
	  << QPointF(l_x, u_y);
}

// @brief BUF ゲートを表すポリゴンを作る．
// @param[in] polygon 結果を格納するポリゴン
void
create_buffer(QPolygonF& polygon)
{
  const qreal l_x = 0;
  const qreal r_x = kBufW;
  const qreal u_y = -(kBufH / 2);
  const qreal c_y = 0;
  const qreal l_y =  (kBufH / 2);

  polygon << QPointF(l_x, u_y)
	  << QPointF(r_x, c_y)
	  << QPointF(l_x, l_y)
	  << QPointF(l_x, u_y);
}

// @brief AND ゲートを表すポリゴンを作る．
// @param[in] polygon 結果を格納するポリゴン
void
create_and(QPolygonF& polygon)
{
  ymuint ndiv = 24;

  const qreal l_x = 0.0;
  const qreal m_x = kAndL;
  const qreal u_y = kGateUY;
  const qreal c_y = 0.0;
  const qreal l_y = kGateLY;
  const qreal r   = kAndR;

  polygon << QPointF(l_x, u_y)
	  << QPointF(m_x, u_y);

  create_arc(m_x, c_y, r, ndiv, 1.5, 2.5, polygon);

  polygon << QPointF(m_x, l_y)
	  << QPointF(l_x, l_y)
	  << QPointF(l_x, u_y);
}

// @brief OR ゲートを表すポリゴンを作る．
// @param[in] polygon 結果を格納するポリゴン
void
create_or(QPolygonF& polygon)
{
  ymuint ndiv = 16;

  const qreal l_x = 0.0;
  const qreal m_x = kOrL;
  const qreal r_x = kGateW;
  const qreal ll_x = - (kOrR / 2.0) * sqrt3;
  const qreal u_y = kGateUY;
  const qreal c_y = 0.0;
  const qreal l_y = kGateLY;
  const qreal r   = kOrR;

  polygon << QPointF(l_x, u_y)
	  << QPointF(m_x, u_y);

  create_arc(m_x, l_y, r, ndiv, 9.0 / 6.0, 11.0 / 6.0, polygon);

  polygon << QPointF(r_x, c_y);

  create_arc(m_x, u_y, r, ndiv, 1.0 / 6.0, 3.0 / 6.0, polygon);

  polygon << QPointF(m_x, l_y)
	  << QPointF(l_x, l_y);

  create_arc(ll_x, c_y, r, ndiv, 1.0 / 6.0, -1.0 / 6.0, polygon);

  polygon << QPointF(l_x, u_y);
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス SymbolImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SymbolImpl::SymbolImpl()
{
}

// @brief デストラクタ
SymbolImpl::~SymbolImpl()
{
}

// @brief 入力に設定する．
void
SymbolImpl::set_to_input()
{
  // bounding box を設定する．
  mBBox.setRect(0, -(kInputH / 2.0), kInputW, kInputH);

  // 入力ピンリストを設定する．
  mIpinList.resize(0);

  // 出力ピンリストを設定する．
  mOpinList.resize(1);
  mOpinList[0] = QPoint(kInputW, 0);

  // painter path を設定する．
  QPolygonF polygon;
  create_input(polygon);

  mPath.addPolygon(polygon);
}

// @brief 出力に設定する．
void
SymbolImpl::set_to_output()
{
  // bounding box を設定する．
  mBBox.setRect(0, -(kOutputH / 2.0), kOutputW, kOutputH);

  // 入力ピンリストを設定する．
  mIpinList.resize(1);
  mIpinList[0] = QPoint(0, 0);

  // 出力ピンリストを設定する．
  mOpinList.resize(0);

  // painter path を設定する．
  QPolygonF polygon;
  create_output(polygon);

  mPath.addPolygon(polygon);
}

// @brief バッファに設定する．
void
SymbolImpl::set_to_buffer()
{
  // bounding box を設定する．
  mBBox.setRect(0, -(kBufH / 2.0), kBufW, kBufH);

  // 入力ピンリストを設定する．
  mIpinList.resize(1);
  mIpinList[0] = QPoint(0, 0);

  // 出力ピンリストを設定する．
  mOpinList.resize(1);
  mOpinList[0] = QPoint(kBufW, 0);

  // painter path を設定する．
  QPolygonF polygon;
  create_buffer(polygon);

  mPath.addPolygon(polygon);
}

// @brief AND ゲートに設定する．
// @param[in] ni 入力数
void
SymbolImpl::set_to_and(ymuint ni)
{
  // bounding box を設定する．
  mBBox.setRect(0, kGateUY, kGateW, kGateH);

  // 入力ピンリストを設定する．
  mIpinList.resize(ni);

  // 出力ピンリストを設定する．
  mOpinList.resize(1);
  mOpinList[0] = QPoint(kGateW, 0);

  // painter path を設定する．
  QPolygonF polygon;
  create_and(polygon);

  mPath.addPolygon(polygon);
}

// @brief OR ゲートに設定する．
// @param[in] ni 入力数
void
SymbolImpl::set_to_or(ymuint ni)
{
  // bounding box を設定する．
  mBBox.setRect(0, kGateUY, kGateW, kGateH);

  // 入力ピンリストを設定する．
  mIpinList.resize(ni);

  // 出力ピンリストを設定する．
  mOpinList.resize(1);
  mOpinList[0] = QPoint(kGateW, 0);

  // painter path を設定する．
  QPolygonF polygon;
  create_or(polygon);

  mPath.addPolygon(polygon);
}

// @brief XOR ゲートに設定する．
// @param[in] ni 入力数
void
SymbolImpl::set_to_xor(ymuint ni)
{
  // bounding box を設定する．
  mBBox.setRect(0, kGateUY, kGateW, kGateH);

  // 入力ピンリストを設定する．
  mIpinList.resize(ni);

  // 出力ピンリストを設定する．
  mOpinList.resize(1);
  mOpinList[0] = QPoint(kGateW, 0);

  // painter path を設定する．
  QPolygonF polygon;
  create_or(polygon);

  mPath.addPolygon(polygon);
}

// @brief このゲートを囲む最小の矩形を表す左上と右下の点を得る．
QRect
SymbolImpl::bounding_box() const
{
  return mBBox;
}

// @brief 入力数を得る．
ymuint
SymbolImpl::ipin_num() const
{
  return mIpinList.size();
}

// @brief pos 番目の入力ピン位置を得る．
// @param[in] pos 入力番号 ( 0 <= pos < input_num() )
QPoint
SymbolImpl::ipin_location(ymuint pos) const
{
  assert_cond( pos < mIpinList.size(), __FILE__, __LINE__);
  return mIpinList[pos];
}

// @brief 出力数を得る．
ymuint
SymbolImpl::opin_num() const
{
  return mOpinList.size();
}

// @brief pos 番目の出力ピン位置を得る．
// @param[in] pos 出力番号 ( 0 <= pos < output_num() )
QPoint
SymbolImpl::opin_location(ymuint pos) const
{
  assert_cond( pos < mOpinList.size(), __FILE__, __LINE__);
  return mOpinList[pos];
}

// @brief 描画を行う．
// @param[in] painter 描画を行うオブジェクト
void
SymbolImpl::draw(QPainter& painter) const
{
  painter.drawPath(mPath);
}

// @brief 描画用の PainterPath を得る．
QPainterPath&
SymbolImpl::path()
{
  return mPath;
}

END_NAMESPACE_YM_LED
