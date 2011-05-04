
/// @file libym_cell/tests/dotlib_parser_test.cc
/// @brief DotlibParser のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: dotlib_lex_test.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include <iomanip>

#include "ym_utils/StopWatch.h"
#include "ym_utils/MsgMgr.h"
#include "ym_utils/MsgHandler.h"
#include "../dotlib/PtMgr.h"
#include "../dotlib/DotlibParser.h"
#include "../dotlib/PtDumper.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

int
dotlibparser_test(int argc,
		  char** argv)
{
  PtMgr ptmgr;
  DotlibParser parser(ptmgr);

  MsgHandler* handler = new StreamMsgHandler(&cerr);
  MsgMgr::reg_handler(handler);

  PtDumper pt_dump;

  StopWatch timer;
  timer.start();

  bool error = false;
  bool debug = false;
  for (int i = 1; i < argc; ++ i) {
    if ( strcmp(argv[i], "-d") == 0 ) {
      debug = true;
    }
    else {
      const PtNode* library = parser.read_file(argv[i], debug);
      if ( library == NULL ) {
	error = true;
      }
      else {
	pt_dump(cout, library);
      }
    }
  }

  timer.stop();
  USTime time = timer.time();
  cout << "Time: " << time << endl;

  ptmgr.show_stats(cout);
  for ( ; ; ) { }


  return error ? 0 : -1;
}

END_NAMESPACE_YM_CELL_DOTLIB

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

  int stat = nsYm::nsCell::nsDotlib::dotlibparser_test(argc, argv);

  return stat;
}
