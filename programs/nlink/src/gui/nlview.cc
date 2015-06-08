
/// @file nlview.cc
/// @brief nlview の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlMainWindow.h"
#include <QApplication>


int
main(int argc,
     char** argv)
{
  using nsYm::nsNlink::NlMainWindow;

  QApplication app(argc, argv);

  QMainWindow* mw = new NlMainWindow;
  mw->show();

  return app.exec();
}
