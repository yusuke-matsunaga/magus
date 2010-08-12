
/// @file libym_verilog/tests/ptview/vlview_main.cc
/// @brief vlview の main ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: vlview_main.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include <QApplication>
#include <QSplitter>
#include "VerilogView.h"
#include "TokenView.h"
#include "TokenList.h"
#include "ParseTreeModel.h"
#include "ParseTreeView.h"

#include "Lex.h"
#include <ym_verilog/VlMgr.h>


int
main(int argc,
     char** argv)
{
  using namespace nsYm;
  using namespace nsYm::nsVerilog;
  
  QApplication app(argc, argv);
  
#if 0
  // Lexer を作る．
  MsgMgr msg_mgr;
  FileDescMgr fd_mgr;
  Lex lex(msg_mgr, fd_mgr);

  // サーチパスリスト
  const char* spath = NULL;
  // 今は未設定
  if ( spath ) {
    SearchPathList splist(spath);
    lex.set_searchpath(splist);
  }

  VerilogView* vlview = new VerilogView;
  TokenView* token_view = new TokenView;
  
  QSplitter* splitter = new QSplitter(Qt::Horizontal);
  splitter->addWidget(vlview);
  splitter->addWidget(token_view);
  splitter->setStretchFactor(0, 2);
  splitter->setStretchFactor(1, 1);
  splitter->resize(800, 800);

  TokenList* token_list = new TokenList;
  // ファイルのオープン
  if ( !lex.open_file(argv[1]) ) {
    cerr << argv[1] << " : could not open" << endl;
    return 1;
  }
  // トークンの読み出し
  for ( ; ; ) {
#if 0
    int id = lex.get_raw_token();
#else
    int id = lex.get_token();
#endif
    if ( id == EOF ) {
      break;
    }
    token_list->add_token(id, lex.cur_token_loc(), lex.cur_string());
  }
  token_view->setModel(token_list);
  
  if ( !vlview->open(argv[1]) ) {
    return 2;
  }

  QObject::connect(token_view, SIGNAL(select_token(int, int, int, int)),
		   vlview, SLOT(hilight(int, int, int, int)));

#else
  MsgMgr msg_mgr;

  VlMgr vl_mgr(msg_mgr);
  if ( !vl_mgr.read_file(argv[1]) ) {
    return 1;
  }
  
  VerilogView* vlview = new VerilogView;
  ParseTreeView* pt_view = new ParseTreeView;
  
  QSplitter* splitter = new QSplitter(Qt::Horizontal);
  splitter->addWidget(pt_view);
  splitter->addWidget(vlview);
  splitter->setStretchFactor(0, 1);
  splitter->setStretchFactor(1, 1);
  splitter->resize(1024, 760);

  ParseTreeModel* model = new ParseTreeModel;
  model->set_pt(vl_mgr);
  
  pt_view->setModel(model);
  
  if ( !vlview->open(argv[1]) ) {
    return 2;
  }

  QObject::connect(pt_view, SIGNAL(select_token(int, int, int, int)),
		   vlview, SLOT(hilight(int, int, int, int)));

  
#endif
  
  splitter->show();
  
  return app.exec();
}
