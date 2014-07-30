
/// @file libym_smtlibv2/tests/lextest.cc
/// @brief SmtLibLex のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "SmtLibScanner.h"
#include "ym_YmUtils/FileIDO.h"
#include "ym_YmUtils/MsgMgr.h"
#include "ym_YmUtils/MsgHandler.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

void
lextest(const string& filename)
{
  MsgHandler* handler = new StreamMsgHandler(&cerr);
  MsgMgr::reg_handler(handler);

  FileIDO ido(filename);
  if ( !ido ) {
    cerr << "Counld not open " << filename << endl;
    return;
  }

  SmtLibScanner lex(ido);

  for ( ; ; ) {
    FileRegion loc;
    tTokenType tok = lex.read_token(loc);
    if ( tok == kEofToken ) {
      break;
    }
    cout << "loc = " << loc << endl;
    switch ( tok ) {
    case kNumToken:
      cout << "NUM:     " << lex.cur_string() << endl;
      break;

    case kDecToken:
      cout << "DEC:     " << lex.cur_string() << endl;
      break;

    case kBinToken:
      cout << "BIN:     " << lex.cur_string() << endl;
      break;

    case kHexToken:
      cout << "HEX:     " << lex.cur_string() << endl;
      break;

    case kStringToken:
      cout << "STRING:  " << lex.cur_string() << endl;
      break;

    case kSymbolToken:
      cout << "SYMBOL:  " << lex.cur_string() << endl;
      break;

    case kKeywordToken:
      cout << "KEYWORD: " << lex.cur_string() << endl;
      break;

    case kLpToken:
      cout << "(" << endl;
      break;

    case kRpToken:
      cout << ")" << endl;
      break;

    case kNlToken:
      cout << "NL" << endl;
      break;

    case kErrorToken:
      cout << "ERROR" << endl;
      break;

    default:
      cout << "CHAR:    ";
      if ( isprint(tok) ) {
	cout << static_cast<char>(tok);
      }
      cout << "(#" << tok << ")" << endl;
      break;
    }
    cout << endl;
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
