
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
};

// @brief コンストラクタ
// @param[in] parent 親のウィジェット
SymbolTestWidget::SymbolTestWidget(QWidget* parent) :
  QWidget(parent)
{
  mInputSymbol = new Symbol(kGtInput);
  mOutputSymbol = new Symbol(kGtOutput);
  mBufSymbol = new Symbol(kGtBuffer);
  mNotSymbol = new Symbol(kGtNot);
  mAndSymbol = new Symbol(kGtNand, 7);
  mOrSymbol = new Symbol(kGtNor, 23);
  mXorSymbol = new Symbol(kGtXnor, 28);
}

// @brief 描画イベントのハンドラ
void
SymbolTestWidget::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);

  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
  painter.eraseRect(rect());
  {
    QMatrix matrix;
    matrix.translate(20.0, 20.0);

    painter.setMatrix(matrix);
    mInputSymbol->draw_symbol(painter);
  }
  {
    QMatrix matrix;
    matrix.translate(200.0, 20.0);

    painter.setMatrix(matrix);
    mOutputSymbol->draw_symbol(painter);
  }
  {
    QMatrix matrix;
    matrix.translate(100.0, 20.0);

    painter.setMatrix(matrix);
    mBufSymbol->draw_symbol(painter);
  }
  {
    QMatrix matrix;
    matrix.translate(100.0, 70.0);

    painter.setMatrix(matrix);
    mNotSymbol->draw_symbol(painter);
  }
  {
    QMatrix matrix;
    matrix.translate(150.0, 100.0);

    painter.setMatrix(matrix);
    mAndSymbol->draw_symbol(painter);
  }
  {
    QMatrix matrix;
    matrix.translate(400.0, 100.0);

    painter.setMatrix(matrix);
    mOrSymbol->draw_symbol(painter);
  }
  {
    QMatrix matrix;
    matrix.translate(600.0, 200.0);

    painter.setMatrix(matrix);
    mXorSymbol->draw_symbol(painter);
  }
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
