
/// @file libym_smtlibv2/tests/lextest.cc
/// @brief SmtLibLex のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "SmtLibLex.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

void
lextest(const string& filename)
{
  MsgMgr msg_mgr;
  MsgHandler* handler = new StreamMsgHandler(&cerr);
  msg_mgr.reg_handler(handler);

  SmtLibLex lex(msg_mgr);

  if ( !lex.open_file(filename) ) {
    cerr << "Counld not open " << filename << endl;
    return;
  }

  for ( ; ; ) {
    int tok = lex.read_token();
    if ( tok == EOF ) {
      break;
    }
    switch ( tok ) {
    case NUM:
      cout << "NUM:     " << lex.cur_string() << endl;
      break;

    case DEC:
      cout << "DEC:     " << lex.cur_string() << endl;
      break;

    case BIN:
      cout << "BIN:     " << lex.cur_string() << endl;
      break;

    case HEX:
      cout << "HEX:     " << lex.cur_string() << endl;
      break;

    case STRING:
      cout << "STRING:  " << lex.cur_string() << endl;
      break;

    case S_SYM:
      cout << "S_SYM:   " << lex.cur_string() << endl;
      break;

    case Q_SYM:
      cout << "Q_SYM:   " << lex.cur_string() << endl;
      break;

    case KEYWORD:
      cout << "KEYWORD: " << lex.cur_string() << endl;
      break;

    case ERROR:
      cout << "ERROR" << endl;
      break;

    default:
      cout << "CHAR:    " << static_cast<char>(tok) << endl;
      break;
    }
  }
}

END_NAMESPACE_YM_SMTLIBV2


int
main(int argc,
     char** argv)
{
  using namespace std;

  if ( argc != 2 ) {
    cerr << "USAGE: " << argv[0] << " <filename>" << endl;
    return 1;
  }

  string filename(argv[1]);

  nsYm::nsSmtLibV2::lextest(filename);

  return 0;
}
