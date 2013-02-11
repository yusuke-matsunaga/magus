
/// @file DrawObj.cc
/// @brief DrawObj の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "DrawObj.h"


BEGIN_NAMESPACE_YM_LED

//////////////////////////////////////////////////////////////////////
// クラス PolylineObj
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] points 設定する点列
PolylineObj::PolylineObj(const vector<QPointF>& points) :
  mPointNum(points.size()),
  mPoints(new QPointF[mPointNum])
{
  for (ymuint i = 0; i < mPointNum; ++ i) {
    mPoints[i] = points[i];
  }
}

// @brief デストラクタ
PolylineObj::~PolylineObj()
{
  delete [] mPoints;
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
  mPointNum(points.size()),
  mPoints(new QPointF[mPointNum])
{
  for (ymuint i = 0; i < mPointNum; ++ i) {
    mPoints[i] = points[i];
  }
}

// @brief デストラクタ
PolygonObj::~PolygonObj()
{
  delete [] mPoints;
}

// @brief 描画する．
void
PolygonObj::draw(QPainter& painter)
{
  painter.drawPolygon(mPoints, mPointNum);
}

END_NAMESPACE_YM_LED
