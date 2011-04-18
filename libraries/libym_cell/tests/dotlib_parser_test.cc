
/// @file libym_cell/tests/dotlib_parser_test.cc
/// @brief DotLibParser のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: dotlib_lex_test.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include <iomanip>

#include "ym_utils/StopWatch.h"
#include "../dotlib/DotLibParser.h"
#include "ym_utils/MsgHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

int
dotlibparser_test(int argc,
		  char** argv)
{
  DotLibParser parser;

  MsgHandler* handler = new StreamMsgHandler(&cerr);
  parser.msg_mgr().reg_handler(handler);

  StopWatch timer;
  timer.start();

  for (int i = 1; i < argc; ++ i) {
    bool stat = parser.read_file(argv[i]);
    if ( !stat ) {
      return -1;
    }
  }

  timer.stop();
  USTime time = timer.time();
  cout << "Time: " << time << endl;

  return 0;
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
