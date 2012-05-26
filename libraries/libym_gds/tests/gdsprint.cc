
/// @file gdsprint/gdsprint.cc
/// @brief GDS-II ファイルダンププログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/GdsScanner.h"
#include "ym_gds/GdsDumper.h"
#include "ym_gds/Msg.h"

int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm::nsGds;

  GdsScanner scanner(cin);
  GdsDumper dumper(cout);

  MsgMgr& msgmgr = MsgMgr::the_mgr();
  tMsgMask msgmask = kMsgMaskError | kMsgMaskWarning;
  TestMsgHandler* tmh = new TestMsgHandler(msgmask);
  msgmgr.reg_handler(tmh);

  while ( scanner.read_rec() ) {
    dumper(scanner);
  }

  return 0;
}
