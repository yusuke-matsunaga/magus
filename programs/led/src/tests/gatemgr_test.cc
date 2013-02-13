
/// @file gatemgr_test.cc
/// @brief GateMgr のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "led_nsdef.h"
#include "GateObj.h"
#include "GateMgr.h"


BEGIN_NAMESPACE_YM_LED

class GateMgrTestWidget :
  public QWidget
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のウィジェット
  GateMgrTestWidget(QWidget* parent = NULL);


protected:

  /// @brief 描画イベントのハンドラ
  void
  paintEvent(QPaintEvent* event);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // gateMgr
  GateMgr mGateMgr;

  // GateObj のリスト
  vector<GateObj*> mGateList;

};

// @brief コンストラクタ
// @param[in] parent 親のウィジェット
GateMgrTestWidget::GateMgrTestWidget(QWidget* parent) :
  QWidget(parent)
{
  mGateList.push_back(mGateMgr.new_gate(20, 20, kGtInput));
  mGateList.push_back(mGateMgr.new_gate(500, 20, kGtOutput));
  mGateList.push_back(mGateMgr.new_gate(100, 20, kGtBuffer));
  mGateList.push_back(mGateMgr.new_gate(100, 150, kGtNot));
  mGateList.push_back(mGateMgr.new_gate(100, 300, kGtAnd));
  mGateList.push_back(mGateMgr.new_gate(200, 100, kGtOr));
  mGateList.push_back(mGateMgr.new_gate(200, 400, kGtXor));
}

// @brief 描画イベントのハンドラ
void
GateMgrTestWidget::paintEvent(QPaintEvent* event)
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


int
main(int argc,
     char** argv)
{
  using nsYm::nsLed::GateMgrTestWidget;

  QApplication app(argc, argv);

  QWidget* w = new GateMgrTestWidget();

  w->show();

  return app.exec();
}
