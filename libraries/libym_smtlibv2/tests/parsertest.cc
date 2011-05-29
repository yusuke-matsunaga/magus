
/// @file libym_smtlibv2/test/parsertest.cc
/// @brief SmtLibParser のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "SmtLibParser.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

void
parsertest(const string& filename)
{
  SmtLibParser parser;

  parser.read(filename);

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
