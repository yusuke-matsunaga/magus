
/// @file vlview_main.cc
/// @brief vlview の main ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2009, 2014 Yusuke Matsunaga
/// All rights reserved.


#include <QApplication>
#include <QSplitter>
#include "VlView.h"
#include "VerilogModel.h"
#include "VerilogView.h"

#include "YmVerilog/VlMgr.h"

#include "YmUtils/MsgMgr.h"
#include "YmUtils/MsgHandler.h"


int
main(int argc,
     char** argv)
{
  using namespace nsYm;
  using namespace nsYm::nsVerilog;

  QApplication app(argc, argv);

  list<string> filename_list;
  for (ymuint i = 1; i < argc; ++ i) {
    filename_list.push_back(argv[i]);
  }

  MsgHandler* tmh = new StreamMsgHandler(&cerr);
  MsgMgr::reg_handler(tmh);

  VlMgr vl_mgr;
  for (list<string>::const_iterator p = filename_list.begin();
       p != filename_list.end(); ++ p) {
    const string& name = *p;
    if ( !vl_mgr.read_file(name) ) {
      return 1;
    }
  }
  vl_mgr.elaborate();

  VlView* vlview = new VlView;
  VerilogView* verilog_view = new VerilogView;

  QSplitter* splitter = new QSplitter(Qt::Horizontal);
  splitter->addWidget(verilog_view);
  splitter->addWidget(vlview);
  splitter->setStretchFactor(0, 1);
  splitter->setStretchFactor(1, 1);
  splitter->resize(1024, 760);

  VerilogModel* model = new VerilogModel;
  model->set_vlmgr(vl_mgr);

  verilog_view->setModel(model);

  if ( !vlview->open(argv[1]) ) {
    return 2;
  }

  QObject::connect(verilog_view, SIGNAL(select_token(int, int, int, int)),
		   vlview, SLOT(hilight(int, int, int, int)));


  splitter->show();

  return app.exec();
}
