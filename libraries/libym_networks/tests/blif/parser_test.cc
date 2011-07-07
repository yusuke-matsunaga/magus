
/// @file libym_networks/tests/blif/parser_test.cc
/// @brief BlifParser のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: parsertest.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BlifParser.h"
#include "TestBlifHandler.h"
#include "NullBlifHandler.h"
#include "ym_utils/MsgMgr.h"
#include "ym_utils/MsgHandler.h"

void
usage(const char* argv0)
{
  using namespace std;

  cerr << "USAGE : " << argv0 << " [-null] blif-file" << endl;
}

int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;
  using namespace nsYm::nsNetworks::nsBlif;

  bool null = false;

  int base = 1;
  if ( argc == 3 && strcmp(argv[1], "-null") == 0 ) {
    null = true;
    base = 2;
  }

  if ( base + 1 != argc ) {
    usage(argv[0]);
    return 2;
  }

  string filename = argv[base];

#if !defined(YM_DEBUG)
  try {
#endif
    BlifParser parser;
    BlifHandler* handler = NULL;
    if ( null ) {
      handler = new NullBlifHandler;
    }
    else {
      handler = new TestBlifHandler(&cout);
    }
    parser.add_handler(handler);
    StreamMsgHandler* msg_handler = new StreamMsgHandler(&cerr);
    MsgMgr::reg_handler(msg_handler);

    if ( !parser.read(filename) ) {
      cerr << "Error in reading " << filename << endl;
      return 4;
    }
#if !defined(YM_DEBUG)
  }
  catch ( const AssertError& x) {
    cout << x << endl;
  }
#endif

  return 0;
}
