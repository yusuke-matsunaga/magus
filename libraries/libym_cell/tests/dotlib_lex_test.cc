
/// @file ym_cell/tests/dotlib_lex_test.cc
/// @brief DotLibLex のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: dotlib_lex_test.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include <iomanip>

#include "ym_utils/StopWatch.h"
#include "../dotlib/DotLibParser.h"
#include "ym_utils/MsgHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

int
dotliblex_test(int argc,
	       char** argv)
{
  DotLibParser lex;

  StopWatch timer;
  timer.start();

  for (int i = 1; i < argc; ++ i) {
    if ( !lex.open_file(argv[i]) ) {
      cerr << argv[i] << " : cannot open" << endl;
      return -2;
    }

    for ( ; ; ) {
      Token token = lex.read_token();
      tTokenType id = token.type();
      string value = token.value();
      FileRegion loc = token.loc();
      if ( id == END ) {
	break;
      }
      cout << "line: " << setw(4) << lex.cur_line()
	   << ", column: " << setw(3) << lex.first_column()
	   << " - " << setw(3) << lex.cur_column() << "\t";
      switch ( id ) {
      case COLON:
	cout << ":";
	break;

      case SEMI:
	cout << ";";
	break;

      case COMMA:
	cout << ",";
	break;

      case PLUS:
	cout << "+";
	break;

      case MINUS:
	cout << "-";
	break;

      case MULT:
	cout << "*";
	break;

      case DIV:
	cout << "/";
	break;

      case LP:
	cout << "(";
	break;

      case RP:
	cout << ")";
	break;

      case LCB:
	cout << "{";
	break;

      case RCB:
	cout << "}";
	break;

      case SYMBOL:
	cout << "SYMBOL(\"" << lex.cur_string() << "\")";
	break;

      case INT_NUM:
	cout << "INT(\"" << lex.cur_string() << "\")";
	break;

      case FLOAT_NUM:
	cout << "FLOAT(\"" << lex.cur_string() << "\")";
	break;

      case NL:
	cout << "NL";
	break;

      case ERROR:
	cout << "ERROR";
	break;

      default:
	assert_not_reached(__FILE__, __LINE__);
	break;
      }
      cout << endl;
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

  int stat = nsYm::nsCell::nsDotlib::dotliblex_test(argc, argv);

  return stat;
}
