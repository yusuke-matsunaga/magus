
/// @file libym_cell/tests/dotlib_parser_test.cc
/// @brief DotlibParser のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: dotlib_lex_test.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include <iomanip>

#include "ym_dotlib/DotlibMgr.h"
#include "ym_dotlib/DotlibParser.h"
#include "ym_dotlib/DotlibNode.h"
#include "ym_utils/StopWatch.h"
#include "ym_utils/MsgMgr.h"
#include "ym_utils/MsgHandler.h"


BEGIN_NAMESPACE_YM_DOTLIB

int
dotlibparser_test(int argc,
		  char** argv)
{
  StreamMsgHandler handler(&cerr);
  MsgMgr::reg_handler(&handler);

  DotlibMgr mgr;

  DotlibParser parser;

  StopWatch timer;
  timer.start();

  bool error = false;
  bool debug = false;
  bool loop = false;
  bool dump = false;
  for (int i = 1; i < argc; ++ i) {
    if ( strcmp(argv[i], "-d") == 0 ) {
      debug = true;
    }
    else if ( strcmp(argv[i], "-l") == 0 ) {
      loop = true;
    }
    else if ( strcmp(argv[i], "-v") == 0 ) {
      dump = true;
    }
    else {
      bool stat = parser.read_file(argv[i], mgr, debug);
      if ( !stat ) {
	error = true;
      }
      break;
    }
  }

  if ( dump ) {
    const DotlibNode* library = mgr.root_node();
    library->dump(cout);
  }

  timer.stop();
  USTime time = timer.time();
  cout << "Time: " << time << endl;

  mgr.show_stats(cout);

  if ( loop ) {
    for ( ; ; ) { }
  }

  return error ? 0 : -1;
}

END_NAMESPACE_YM_DOTLIB

int
main(int argc,
     char** argv)
{
  using namespace nsYm;

  if ( argc < 2 ) {
    cerr << "USAGE: " << argv[0]
	 << " <file-name> ..." << endl;
    return -1;
  }

  int stat = nsYm::nsDotlib::dotlibparser_test(argc, argv);

  return stat;
}
