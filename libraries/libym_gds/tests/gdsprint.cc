
/// @file gdsprint/gdsprint.cc
/// @brief GDS-II ファイルダンププログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "gds/GdsScanner.h"
#include "gds/GdsDumper.h"
#include "gds/Msg.h"

int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm::nsGds;

  if ( argc != 2 ) {
    cerr << "USAGE: " << argv[0] << " <gds2 filename>" << endl;
    return 1;
  }

  GdsScanner scanner;
  GdsDumper dumper(cout);

  if ( !scanner.open_file(argv[1]) ) {
    cerr << argv[1] << ": No such file" << endl;
    return 2;
  }

  MsgMgr& msgmgr = MsgMgr::the_mgr();
  tMsgMask msgmask = kMsgMaskError | kMsgMaskWarning;
  TestMsgHandler* tmh = new TestMsgHandler(msgmask);
  msgmgr.reg_handler(tmh);

  while ( scanner.read_rec() ) {
    dumper(scanner);
  }

  scanner.close_file();

  return 0;
}
