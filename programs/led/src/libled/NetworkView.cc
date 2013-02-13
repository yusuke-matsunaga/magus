
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
  QWidget(parent)
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

// @brief ゲートを追加する．
void
NetworkView::add_gate(GateObj* gate)
{
  mGateList.push_back(gate);
}

// @brief 描画イベントのハンドラ
void
NetworkView::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);

  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
  painter.eraseRect(rect());

  for (vector<GateObj*>::iterator p = mGateList.begin();
       p != mGateList.end(); ++ p) {
    GateObj* gate_obj = *p;
    gate_obj->draw(painter);
  }
}

END_NAMESPACE_YM_LED
