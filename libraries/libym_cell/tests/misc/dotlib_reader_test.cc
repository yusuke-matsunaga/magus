
/// @file dotlib_reader_test.cc
/// @brief CellDotlibReader のテスト用プログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmUtils/MsgMgr.h"
#include "YmUtils/MsgHandler.h"
#include "YmCell/CellDotlibReader.h"
#include "YmCell/CellLibrary.h"


BEGIN_NAMESPACE_YM_CELL

int
dotlib_reader_test(const string& filename)
{
  CellDotlibReader read;

  MsgHandler* mh = new StreamMsgHandler(&cerr);
  mh->set_mask(kMaskAll);
  mh->delete_mask(kMsgInfo);
  mh->delete_mask(kMsgDebug);
  MsgMgr::reg_handler(mh);

  const CellLibrary* library = read(filename);
  if ( library == NULL) {
    return 1;
  }

  display_library(cout, *library);

  return 0;
}

END_NAMESPACE_YM_CELL


int
main(int argc,
     const char** argv)
{
  using namespace std;

  if ( argc != 2 ) {
    cerr << "USAGE: " << argv[0] << " <filename>" << endl;
    return 255;
  }

  int stat = nsYm::nsCell::dotlib_reader_test(argv[1]);

  return stat;
}
