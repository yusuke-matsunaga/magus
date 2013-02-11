
/// @file SymbolImpl.cc
/// @brief SymbolImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SymbolImpl.h"
#include "DrawObj.h"


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

// XORゲートの幅
const qreal kXorW = kGateW + kXorM;

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
// @param[in] points 追加するベクタ
// @note 最初と最後の点は points には含まれない．
void
create_arc(qreal cx,
	   qreal cy,
	   qreal r,
	   ymuint ndiv,
	   qreal start_rad,
	   qreal end_rad,
	   vector<QPointF>& points)
{
  qreal rad = pi * start_rad;
  qreal diff = end_rad - start_rad;
  qreal inc = (pi * diff) / ndiv;
  for (ymuint i = 1; i < ndiv; i ++) {
    rad += inc;
    qreal dx = cx + cos(rad) * r;
    qreal dy = cy + sin(rad) * r;
    points.push_back(QPointF(dx, dy));
  }
}

// @brief 入力を表すポリゴンを作る．
// @param[in] points 結果を格納するベクタ
void
create_input(vector<QPointF>& points)
{
  const qreal l_x = 0.0;
  const qreal m_x = kInputW - (kInputH / 2.0);
  const qreal r_x = kInputW;
  const qreal u_y = -(kInputH / 2.0);
  const qreal m_y = 0.0;
  const qreal l_y =  (kInputH / 2.0);

  points.push_back(QPointF(l_x, u_y));
  points.push_back(QPointF(m_x, u_y));
  points.push_back(QPointF(r_x, m_y));
  points.push_back(QPointF(m_x, l_y));
  points.push_back(QPointF(l_x, l_y));
}

// @brief 出力を表すポリゴンを作る．
// @param[in] points 結果を格納するベクタ
void
create_output(vector<QPointF>& points)
{
  const qreal l_x = 0;
  const qreal m_x = kOutputW - (kOutputH / 2.0);
  const qreal r_x = kOutputW;
  const qreal u_y = -(kOutputH / 2);
  const qreal c_y = 0;
  const qreal l_y = (kOutputH / 2);

  points.push_back(QPointF(l_x, u_y));
  points.push_back(QPointF(m_x, u_y));
  points.push_back(QPointF(r_x, c_y));
  points.push_back(QPointF(m_x, l_y));
  points.push_back(QPointF(l_x, l_y));
  points.push_back(QPointF(l_x, u_y));
}

// @brief BUF ゲートを表すポリゴンを作る．
// @param[in] points 結果を格納するベクタ
void
create_buffer(vector<QPointF>& points,
	      const QPointF& offset = QPointF(0.0, 0.0))
{
  const qreal l_x = 0.0 + offset.x();
  const qreal r_x = kBufW + offset.x();
  const qreal u_y = -(kBufH / 2) + offset.y();
  const qreal c_y = 0.0 + offset.y();
  const qreal l_y =  (kBufH / 2) + offset.y();

  points.push_back(QPointF(l_x, u_y));
  points.push_back(QPointF(r_x, c_y));
  points.push_back(QPointF(l_x, l_y));
}

// @brief AND ゲートを表すポリゴンを作る．
// @param[in] points 結果を格納するベクタ
void
create_and(vector<QPointF>& points,
	   const QPointF& offset = QPointF(0.0, 0.0))
{
  ymuint ndiv = 24;

  const qreal l_x = 0.0 + offset.x();
  const qreal m_x = kAndL + offset.x();
  const qreal u_y = kGateUY + offset.y();
  const qreal c_y = 0.0 + offset.y();
  const qreal l_y = kGateLY + offset.y();
  const qreal r   = kAndR;

  points.push_back(QPointF(l_x, u_y));
  points.push_back(QPointF(m_x, u_y));

  create_arc(m_x, c_y, r, ndiv, 1.5, 2.5, points);

  points.push_back(QPointF(m_x, l_y));
  points.push_back(QPointF(l_x, l_y));
}

// @brief OR ゲートを表すポリゴンを作る．
// @param[in] points 結果を格納するベクタ
void
create_or(vector<QPointF>& points,
	  const QPointF& offset = QPointF(0.0, 0.0))
{
  ymuint ndiv = 16;

  const qreal l_x = 0.0 + offset.x();
  const qreal m_x = kOrL + offset.x();
  const qreal r_x = kGateW + offset.x();
  const qreal ll_x = - (kOrR / 2.0) * sqrt3 + offset.x();
  const qreal u_y = kGateUY + offset.y();
  const qreal c_y = 0.0 + offset.y();
  const qreal l_y = kGateLY + offset.y();
  const qreal r   = kOrR;

  points.push_back(QPointF(l_x, u_y));
  points.push_back(QPointF(m_x, u_y));

  create_arc(m_x, l_y, r, ndiv, 9.0 / 6.0, 11.0 / 6.0, points);

  points.push_back(QPointF(r_x, c_y));

  create_arc(m_x, u_y, r, ndiv, 1.0 / 6.0, 3.0 / 6.0, points);

  points.push_back(QPointF(m_x, l_y));
  points.push_back(QPointF(l_x, l_y));

  create_arc(ll_x, c_y, r, ndiv, 1.0 / 6.0, -1.0 / 6.0, points);
}

// @brief OR ゲートの入力辺を表す点列を作る．
// @param[in] points 結果を格納するベクタ
void
create_or_edge(vector<QPointF>& points,
	       const QPointF& offset = QPointF(0.0, 0.0))
{
  ymuint ndiv = 16;

  const qreal l_x  = 0.0 + offset.x();
  const qreal ll_x = - (kOrR / 2.0) * sqrt3 + offset.x();
  const qreal u_y  = kGateUY + offset.y();
  const qreal c_y  = 0.0 + offset.y();
  const qreal l_y  = kGateLY + offset.y();
  const qreal r    = kOrR;

  points.push_back(QPointF(l_x, l_y));
  create_arc(ll_x, c_y, r, ndiv, 1.0 / 6.0, -1.0 / 6.0, points);
  points.push_back(QPointF(l_x, u_y));
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス SymbolImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SymbolImpl::SymbolImpl() :
  mObjNum(0),
  mObjList(NULL)
{
}

// @brief デストラクタ
SymbolImpl::~SymbolImpl()
{
  delete [] mObjList;
}

// @brief 描画用のオブジェクトを設定する．
void
SymbolImpl::set_drawobj(DrawObj* obj)
{
  delete [] mObjList;
  mObjNum = 1;
  mObjList = new DrawObj*[mObjNum];
  mObjList[0] = obj;
}

// @brief 描画用のオブジェクトを設定する．
void
SymbolImpl::set_drawobj(DrawObj* obj1,
			DrawObj* obj2)
{
  delete [] mObjList;
  mObjNum = 2;
  mObjList = new DrawObj*[mObjNum];
  mObjList[0] = obj1;
  mObjList[1] = obj2;
}

// @brief 描画用のオブジェクトを設定する．
void
SymbolImpl::set_drawobj(DrawObj* obj1,
			DrawObj* obj2,
			DrawObj* obj3)
{
  delete [] mObjList;
  mObjNum = 3;
  mObjList = new DrawObj*[mObjNum];
  mObjList[0] = obj1;
  mObjList[1] = obj2;
  mObjList[2] = obj3;
}

// @brief 描画用のオブジェクトを設定する．
void
SymbolImpl::set_drawobj(const vector<DrawObj*>& obj_list)
{
  delete [] mObjList;
  mObjNum = obj_list.size();
  mObjList = new DrawObj*[mObjNum];
  for (ymuint i = 0; i < mObjNum; ++ i) {
    mObjList[i] = obj_list[i];
  }
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

  // 描画用オブジェクトを設定する．
  vector<QPointF> points;
  create_input(points);
  DrawObj* obj = new PolygonObj(points);
  set_drawobj(obj);
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

  // 描画用オブジェクトを設定する．
  vector<QPointF> points;
  create_output(points);
  DrawObj* obj = new PolygonObj(points);
  set_drawobj(obj);
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

  // 描画用オブジェクトを設定する．
  vector<QPointF> points;
  create_buffer(points);
  DrawObj* obj = new PolygonObj(points);
  set_drawobj(obj);
}

// @brief AND ゲートに設定する．
// @param[in] ni 入力数
void
SymbolImpl::set_to_and(ymuint ni)
{
  calc_points(ni, kGateW, false);

  // 出力ピンリストを設定する．
  mOpinList.resize(1);
  mOpinList[0] = QPoint(kGateW, 0);

  // 描画用オブジェクトを設定する．
  vector<QPointF> points;
  create_and(points);
  DrawObj* obj = new PolygonObj(points);
  set_drawobj(obj);
}

// @brief OR ゲートに設定する．
// @param[in] ni 入力数
void
SymbolImpl::set_to_or(ymuint ni)
{
  calc_points(ni, kGateW, true);

  // 出力ピンリストを設定する．
  mOpinList.resize(1);
  mOpinList[0] = QPoint(kGateW, 0);

  // 描画用オブジェクトを設定する．
  vector<QPointF> points;
  create_or(points);
  DrawObj* obj = new PolygonObj(points);
  set_drawobj(obj);
}

// @brief XOR ゲートに設定する．
// @param[in] ni 入力数
void
SymbolImpl::set_to_xor(ymuint ni)
{
  calc_points(ni, kXorW, true);

  // 出力ピンリストを設定する．
  mOpinList.resize(1);
  mOpinList[0] = QPoint(kXorW, 0);

  // 描画用オブジェクトを設定する．
  vector<QPointF> points;
  create_or(points, QPointF(kXorM, 0.0));
  DrawObj* or_obj = new PolygonObj(points);

  vector<QPointF> points2;
  create_or_edge(points2);
  DrawObj* edge_obj = new PolylineObj(points2);

  set_drawobj(or_obj, edge_obj);
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
  for (ymuint i = 0; i < mObjNum; ++ i) {
    mObjList[i]->draw(painter);
  }
  painter.drawRect(mBBox);
}

BEGIN_NONAMESPACE

// 各セグメントにピンを分配する．
void
distribute_pins(ymuint ni,
		vector<ymuint>& npins)
{
  if ( ni >= 6 && ni <= 10 ) {
    assert_cond( npins.size() == 3, __FILE__, __LINE__);
    switch (ni) {
    case 6:
      npins[0] = 2;
      npins[1] = 2;
      npins[2] = 2;
      break;

    case 7:
      npins[0] = 2;
      npins[1] = 3;
      npins[2] = 2;
      break;

    case 8:
      npins[0] = 3;
      npins[1] = 3;
      npins[2] = 2;
      break;

    case 9:
      npins[0] = 3;
      npins[1] = 3;
      npins[2] = 3;
      break;

    case 10:
      npins[0] = 3;
      npins[1] = 4;
      npins[2] = 3;
      break;
    }
  }
  else {
    ymuint nseg = npins.size();
    ymuint c = nseg / 2;  // 真ん中のセグメント
    bool up = false;
    ymuint u = c - 1; // c が 0 の時はやばいけど, そのときは使われない．
    ymuint l = c;
    while ( ni > 0 ) {
      if ( up ) {
	if ( ni >= 9 ) {
	  npins[u] = 5;
	}
	else if ( ni == 8 || ni == 7 ) {
	  npins[u] = 4;
	}
	else if ( ni == 6 ) {
	  npins[u] = 3;
	}
	else {
	  npins[u] = ni;
	}
	ni -= npins[u];
	u --;
	up = false;
      }
      else {
	if ( ni >= 9 ) {
	  npins[l] = 5;
	}
	else if ( ni == 8 || ni == 7 ) {
	  npins[l] = 4;
	}
	else if ( ni == 6 ) {
	  npins[l] = 3;
	}
	else {
	  npins[l] = ni;
	}
	ni -= npins[l];
	l ++;
	up = true;
      }
    }
  }
}

// AND/OR/XOR の入力辺上のピンの座標を計算する関数
// x_off : X 軸のオフセット
// y_off : Y 軸のオフセット
// pos   : 入力ピン位置(上が0)
// n     : 入力ピン数
// or_xor: OR/XOR の時 true
QPoint
ipos(qreal x_off,
     qreal y_off,
     ymuint pos,
     ymuint n,
     bool or_xor)
{
  qreal y1;
  ymuint dpos = pos * 2 + 1;
  if ( dpos < n ) {
    y1 = -kOff[n - 2][pos];
  }
  else if ( dpos == n ) {
    y1 = 0.0;
  }
  else {
    y1 = kOff[n - 2][n - pos - 1];
  }
  qreal x1 = 0.0;
  if ( or_xor ) {
    qreal y2 = kGateH * 0.5;
    if ( y1 > 0.0 ) {
      y2 -= y1;
    }
    else {
      y2 += y1;
    }
    qreal tmp1 = sqrt(3.0 * kOrR * kOrR / 4.0 + kOrR * y2 - y2 * y2);
    x1 = tmp1 - sqrt3 * kOrR / 2.0;
  }
  return QPoint(x_off + x1, y_off + y1);
}

END_NONAMESPACE


// @brief AND/OR/XOR ゲートの時の諸元を設定する．
// @param[in] ni 入力数
// @param[in] gate_w ゲートの幅
// @param[in] or_xor OR/XOR ゲートの時 true にするフラグ
void
SymbolImpl::calc_points(ymuint ni,
			qreal gate_w,
			bool or_xor)
{
  // 本体を含めたセグメント数
  ymuint nseg = (ni + 4) / 5;
  if ( nseg == 2 ) {
    // これだけ例外
    nseg = 3;
  }

  // 上側のセグメント数
  ymuint nseg_u = nseg / 2;

  qreal u0_y = - kGateH / 2.0;
  mBBox = QRect(0.0, u0_y - (kGateH * nseg_u), gate_w, nseg * kGateH);

  qreal u_y = u0_y - (kGateH * nseg_u);

  // 各セグメントごとの入力数
  vector<ymuint> npins(nseg);
  distribute_pins(ni, npins);

  mIpinList.resize(ni);
  ymuint pin_offset = 0;
  for (ymuint i = 0; i < nseg; ++ i) {
    ymuint np = npins[i];
    qreal cy = u_y + kGateH * (i + 0.5);
    for (ymuint j = 0; j < np; ++ j) {
      mIpinList[pin_offset + j] = ipos(0.0, cy, j, np, or_xor);
    }
    pin_offset += np;
  }
 }

END_NAMESPACE_YM_LED
