
/// @file libym_verilog/tests/vlview/vlview_main.cc
/// @brief vlview の main ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: vlview_main.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include <QApplication>
#include <QSplitter>
#include "VlView.h"
#include "VlTokenView.h"
#include "VlTokenList.h"
#include "VlParseTreeModel.h"
#include "VlParseTreeView.h"

#include <ym_verilog/VlMgr.h>


int
main(int argc,
     char** argv)
{
  using namespace nsYm;
  using namespace nsYm::nsVerilog;
  
  QApplication app(argc, argv);
  MsgMgr msg_mgr;

  VlMgr vl_mgr(msg_mgr);
  if ( !vl_mgr.read_file(argv[1]) ) {
    return 1;
  }
  
  VlView* vlview = new VlView;
  VlParseTreeView* pt_view = new VlParseTreeView;
  
  QSplitter* splitter = new QSplitter(Qt::Horizontal);
  splitter->addWidget(pt_view);
  splitter->addWidget(vlview);
  splitter->setStretchFactor(0, 1);
  splitter->setStretchFactor(1, 1);
  splitter->resize(1024, 760);

  VlParseTreeModel* model = new VlParseTreeModel;
  model->set_pt(vl_mgr);
  
  pt_view->setModel(model);
  
  if ( !vlview->open(argv[1]) ) {
    return 2;
  }

  QObject::connect(pt_view, SIGNAL(select_token(int, int, int, int)),
		   vlview, SLOT(hilight(int, int, int, int)));

  
  splitter->show();
  
  return app.exec();
}
