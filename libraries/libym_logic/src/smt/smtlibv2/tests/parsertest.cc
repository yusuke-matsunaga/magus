
/// @file libym_smtlibv2/test/parsertest.cc
/// @brief SmtLibParser のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "SmtLibNode.h"
#include "SmtLibParser.h"
#include "YmUtils/FileIDO.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

void
parsertest(const string& filename)
{
  FileIDO ido(filename);
  if ( !ido ) {
    cout << filename << ": No such file" << endl;
    return;
  }

  SmtLibParser parser;
  parser.init(ido);

  for (bool run = true; run; ) {
    SmtLibNode* node;
    SmtLibParser::tResult res = parser.read(node);
    switch ( res ) {
    case SmtLibParser::kOK:
      display(cout, node);
      break;

    case SmtLibParser::kError:
      cout << "Error" << endl;
      run = false;
      break;

    case SmtLibParser::kEOF:
      run = false;
      break;

    case SmtLibParser::kOpen:
      cout << "unexpected EOF" << endl;
      run = false;
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

  nsYm::nsSmtLibV2::parsertest(filename);

  return 0;
}
