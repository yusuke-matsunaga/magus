
/// @file gateobj_test.cc
/// @brief Symbol のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "led_nsdef.h"
#include "Symbol.h"
#include "GateObj.h"
#include "GateColor.h"


BEGIN_NAMESPACE_YM_LED

class GateObjTestWidget :
  public QWidget
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のウィジェット
  GateObjTestWidget(QWidget* parent = NULL);


protected:

  /// @brief 描画イベントのハンドラ
  void
  paintEvent(QPaintEvent* event);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力シンボル
  Symbol* mInputSymbol;

  // 出力シンボル
  Symbol* mOutputSymbol;

  // バッファシンボル
  Symbol* mBufSymbol;

  // NOT シンボル
  Symbol* mNotSymbol;

  // AND シンボル
  Symbol* mAndSymbol;

  // OR シンボル
  Symbol* mOrSymbol;

  // XOR シンボル
  Symbol* mXorSymbol;

  // 描画用の設定
  GateColor mColor;

  // GateObj のリスト
  vector<GateObj*> mGateList;

};

// @brief コンストラクタ
// @param[in] parent 親のウィジェット
GateObjTestWidget::GateObjTestWidget(QWidget* parent) :
  QWidget(parent)
{
  mInputSymbol = new Symbol(kGtInput);
  mOutputSymbol = new Symbol(kGtOutput);
  mBufSymbol = new Symbol(kGtBuffer);
  mNotSymbol = new Symbol(kGtNot);
  mAndSymbol = new Symbol(kGtNand, 7);
  mOrSymbol = new Symbol(kGtNor, 23);
  mXorSymbol = new Symbol(kGtXnor, 28);

  mGateList.push_back(new GateObj(mInputSymbol, QPoint(20, 20)));
  mGateList.push_back(new GateObj(mOutputSymbol, QPoint(500, 20)));
  mGateList.push_back(new GateObj(mBufSymbol, QPoint(100, 20)));
  mGateList.push_back(new GateObj(mNotSymbol, QPoint(100, 150)));
  mGateList.push_back(new GateObj(mAndSymbol, QPoint(100, 300)));
  mGateList.push_back(new GateObj(mOrSymbol, QPoint(200, 100)));
  mGateList.push_back(new GateObj(mXorSymbol, QPoint(200, 400)));

  for (vector<GateObj*>::iterator p = mGateList.begin();
       p != mGateList.end(); ++ p) {
    GateObj* gate_obj = *p;
    gate_obj->set_color(&mColor);
  }
}

// @brief 描画イベントのハンドラ
void
GateObjTestWidget::paintEvent(QPaintEvent* event)
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
  using nsYm::nsLed::GateObjTestWidget;

  QApplication app(argc, argv);

  QWidget* w = new GateObjTestWidget();

  w->show();

  return app.exec();
}
