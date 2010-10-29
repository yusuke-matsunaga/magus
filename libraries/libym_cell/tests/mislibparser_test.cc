
/// @file libym_cell/tests/mislib_test.cc
/// @brief MisLib のテスト用プログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: mislib_test.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "mislib/MislibParser.h"
#include "ym_utils/MsgHandler.h"


int
main(int argc,
     const char** argv)
{
  using namespace std;
  using namespace nsYm;
  using nsYm::nsCell::MislibParser;
  using nsYm::nsCell::MislibPt;

  if ( argc != 2 ) {
    cerr << "USAGE: " << argv[0] << " <filename>" << endl;
    return 255;
  }

  MislibParser parser;

  MsgHandler* mh = new StreamMsgHandler(&cerr);
  mh->set_mask(MsgHandler::kMaskAll);
  mh->delete_mask(kMsgInfo);
  mh->delete_mask(kMsgDebug);
  parser.msg_mgr().reg_handler(mh);

  const MislibPt* root = parser.read(argv[1]);
  if ( root ) {
    root->dump(cout);
  }

  return 0;
}
