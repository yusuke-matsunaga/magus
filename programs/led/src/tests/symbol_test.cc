
/// @file symbol_test.cc
/// @brief Symbol のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "led_nsdef.h"
#include "Symbol.h"


BEGIN_NAMESPACE_YM_LED

class SymbolTestWidget :
  public QWidget
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のウィジェット
  SymbolTestWidget(QWidget* parent = NULL);


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

};

// @brief コンストラクタ
// @param[in] parent 親のウィジェット
SymbolTestWidget::SymbolTestWidget(QWidget* parent) :
  QWidget(parent)
{
  mInputSymbol = Symbol::input_symbol();
}

// @brief 描画イベントのハンドラ
void
SymbolTestWidget::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);

  painter.setRenderHint(QPainter::Antialiasing, true);

  QMatrix matrix;
  matrix.translate(20.0, 20.0);

  painter.setMatrix(matrix);
  mInputSymbol->draw(painter);
}



END_NAMESPACE_YM_LED


int
main(int argc,
     char** argv)
{
  using nsYm::nsLed::SymbolTestWidget;

  QApplication app(argc, argv);

  QWidget* w = new SymbolTestWidget();

  w->show();

  return app.exec();
}
