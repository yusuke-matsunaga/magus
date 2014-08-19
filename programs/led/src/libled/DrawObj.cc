
/// @file DrawObj.cc
/// @brief DrawObj の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "DrawObj.h"


BEGIN_NAMESPACE_YM_LED

//////////////////////////////////////////////////////////////////////
// クラス LineObj
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] point1, point2 線分の両端点
LineObj::LineObj(const QPointF& point1,
		 const QPointF& point2)
{
  mPoints[0] = point1;
  mPoints[1] = point2;
}

// @brief デストラクタ
LineObj::~LineObj()
{
}

// @brief 描画する．
void
LineObj::draw(QPainter& painter)
{
  painter.drawLine(mPoints[0], mPoints[1]);
}

// @brief 描画領域を囲む最小の矩形を求める．
QRectF
LineObj::bounding_box()
{
  qreal x0 = mPoints[0].x();
  qreal y0 = mPoints[0].y();
  qreal x1 = mPoints[1].x();
  qreal y1 = mPoints[1].y();

  qreal min_x;
  qreal max_x;
  if ( x0 < x1 ) {
    min_x = x0;
    max_x = x1;
  }
  else {
    min_x = x1;
    max_x = x0;
  }

  qreal min_y;
  qreal max_y;
  if ( y0 < y1 ) {
    min_y = y0;
    max_y = y1;
  }
  else {
    min_y = y1;
    max_y = y0;
  }
  qreal w = max_x - min_x;
  qreal h = max_y - min_y;
  return QRectF(min_x, min_y, w, h);
}


//////////////////////////////////////////////////////////////////////
// クラス PointsObj
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] points 設定する点列
PointsObj::PointsObj(const vector<QPointF>& points) :
  mPointNum(points.size()),
  mPoints(new QPointF[mPointNum])
{
  for (ymuint i = 0; i < mPointNum; ++ i) {
    mPoints[i] = points[i];
  }
}

// @brief デストラクタ
PointsObj::~PointsObj()
{
  delete [] mPoints;
}

// @brief 描画領域を囲む最小の矩形を求める．
QRectF
PointsObj::bounding_box()
{
  if ( mPointNum == 0 ) {
    return QRectF(0.0, 0.0, 0.0, 0.0);
  }

  qreal min_x = mPoints[0].x();
  qreal max_x = min_x;
  qreal min_y = mPoints[0].y();
  qreal max_y = min_y;
  for (ymuint i = 1; i < mPointNum; ++ i) {
    qreal x = mPoints[i].x();
    if ( min_x > x ) {
      min_x = x;
    }
    if ( max_x < x ) {
      max_x = x;
    }
    qreal y = mPoints[i].y();
    if ( min_y > y ) {
      min_y = y;
    }
    if ( max_y < y ) {
      max_y = y;
    }
  }
  qreal w = max_x - min_x;
  qreal h = max_y - min_y;
  return QRectF(min_x, min_y, w, h);
}



//////////////////////////////////////////////////////////////////////
// クラス PolylineObj
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] points 設定する点列
PolylineObj::PolylineObj(const vector<QPointF>& points) :
  PointsObj(points)
{
}

// @brief デストラクタ
PolylineObj::~PolylineObj()
{
}

// @brief 描画する．
void
PolylineObj::draw(QPainter& painter)
{
  painter.drawPolyline(mPoints, mPointNum);
}


//////////////////////////////////////////////////////////////////////
// クラス PolygonObj
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] points 設定する点列
PolygonObj::PolygonObj(const vector<QPointF>& points) :
  PointsObj(points)
{
}

// @brief デストラクタ
PolygonObj::~PolygonObj()
{
}

// @brief 描画する．
void
PolygonObj::draw(QPainter& painter)
{
  painter.drawPolygon(mPoints, mPointNum);
}


//////////////////////////////////////////////////////////////////////
// クラス EllipseObj
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] rect 楕円を囲む矩形
EllipseObj::EllipseObj(const QRectF& rect) :
  mRect(rect)
{
}

// @brief デストラクタ
EllipseObj::~EllipseObj()
{
}

// @brief 描画する．
void
EllipseObj::draw(QPainter& painter)
{
  painter.drawEllipse(mRect);
}

// @brief 描画領域を囲む最小の矩形を求める．
QRectF
EllipseObj::bounding_box()
{
  return mRect;
}

END_NAMESPACE_YM_LED
