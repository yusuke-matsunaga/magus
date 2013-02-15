
/// @file NetworkView.cc
/// @brief NetworkView の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "NetworkView.h"
#include "GateObj.h"


BEGIN_NAMESPACE_YM_LED

//////////////////////////////////////////////////////////////////////
// クラス NetworkView
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NetworkView::NetworkView(QWidget* parent) :
  QWidget(parent),
  mBoundingBox(0, 0, 0, 0),
  mScale(2.0)
{
}

// @brief デストラクタ
NetworkView::~NetworkView()
{
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
  GateObj* gate = mGateMgr.new_gate(pos, type, ni);
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

// @brief 描画イベントのハンドラ
void
NetworkView::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);

  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));

  painter.setWindow(mBoundingBox);
  painter.setViewport(rect());
  painter.eraseRect(mBoundingBox);

  for (vector<GateObj*>::iterator p = mGateList.begin();
       p != mGateList.end(); ++ p) {
    GateObj* gate_obj = *p;
    gate_obj->draw(painter);
  }
}

// @brief サイズや倍率が変更されたときに再計算を行なう．
void
NetworkView::calc_size()
{
  qreal w = mBoundingBox.width() * mScale;
  qreal h = mBoundingBox.height() * mScale;
  resize(w, h);
}

END_NAMESPACE_YM_LED
