
/// @file NetworkView.cc
/// @brief NetworkView の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "NetworkView.h"
#include "GateMgr.h"
#include "GateObj.h"
#include "GateColor.h"


BEGIN_NAMESPACE_YM_LED

//////////////////////////////////////////////////////////////////////
// クラス NetworkView
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NetworkView::NetworkView(QWidget* parent) :
  QWidget(parent),
  mBoundingBox(0, 0, 0, 0),
  mScale(2.0),
  mGridSize(20),
  mGridEnable(false)
{
  mGateMgr = new GateMgr();
  mDefaultColor = new GateColor();
}

// @brief デストラクタ
NetworkView::~NetworkView()
{
  delete mDefaultColor;

  for (vector<GateObj*>::iterator p = mGateList.begin();
       p != mGateList.end(); ++ p) {
    GateObj* gate_obj = *p;
    delete gate_obj;
  }
}

// @brief ゲートを作る．
// @param[in] pos 位置
// @param[in] type 型
// @param[in] ni 入力数
GateObj*
NetworkView::new_gate(const QPoint& pos,
		      GateType type,
		      ymuint ni)
{
  GateObj* gate = mGateMgr->new_gate(pos, type, ni);
  gate->set_color(mDefaultColor);
  mGateList.push_back(gate);

  // サイズの再計算を行なう．
  mBoundingBox = mBoundingBox.united(gate->bounding_box());

  calc_size();

  return gate;
}

// @brief ゲートを作る．
// @param[in] x, y 位置
// @param[in] type 型
// @param[in] ni 入力数
GateObj*
NetworkView::new_gate(int x,
		      int y,
		      GateType type,
		      ymuint ni)
{
  return new_gate(QPoint(x, y), type, ni);
}

// @brief 倍率を得る．
qreal
NetworkView::scale() const
{
  return mScale;
}

// @brief 倍率を変更する．
void
NetworkView::setScale(qreal scale)
{
  mScale = scale;

  calc_size();
}

void
NetworkView::setScale2(int val)
{
  qreal scale = val / 100.0;
  setScale(scale);
}

// @brief グリッド表示を行なう
void
NetworkView::enableGrid(int enable)
{
  mGridEnable = enable;
  update();
}

// @brief 描画イベントのハンドラ
void
NetworkView::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);

  painter.setRenderHint(QPainter::Antialiasing, true);

  // ウィンドウとビューポートの設定
  painter.setWindow(mBoundingBox);
  painter.setViewport(rect());

  // 領域のクリア
  painter.eraseRect(mBoundingBox);

  // グリッド線の描画
  if ( mGridEnable ) {
    int start_x = mBoundingBox.x();
    int start_y = mBoundingBox.y();
    start_x = ((start_x + mGridSize - 1) / mGridSize) * mGridSize;
    start_y = ((start_y + mGridSize - 1) / mGridSize) * mGridSize;

    int end_x = mBoundingBox.x() + mBoundingBox.width();
    int end_y = mBoundingBox.y() + mBoundingBox.height();

    painter.save();
    painter.setPen(QPen(Qt::DashLine));
    for (int x = start_x; x < end_x; x += mGridSize) {
      painter.drawLine(x, start_y, x, end_y);
    }
    for (int y = start_y; y < end_y; y += mGridSize) {
      painter.drawLine(start_x, y, end_x, y);
    }
    painter.restore();
  }

  // ゲートの描画
  for (vector<GateObj*>::iterator p = mGateList.begin();
       p != mGateList.end(); ++ p) {
    GateObj* gate_obj = *p;
    gate_obj->draw(painter);
  }
}

// @brief マウスが押された時のハンドラ
void
NetworkView::mousePressEvent(QMouseEvent* event)
{
  cout << "mouse press event: ";
  int vx = event->pos().x();
  int vy = event->pos().y();
  int x = (vx / mScale) + mBoundingBox.x();
  int y = (vy / mScale) + mBoundingBox.y();
  cout << "( " << x << ", " << y << ")" << endl;
}

// @brief サイズや倍率が変更されたときに再計算を行なう．
void
NetworkView::calc_size()
{
  qreal w = mBoundingBox.width() * mScale;
  qreal h = mBoundingBox.height() * mScale;
  resize(w, h);
  update();
}

END_NAMESPACE_YM_LED
