
/// @file libym_cell/tests/dotlib_reader_test.cc
/// @brief CellDotlibReader のテスト用プログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: dotlib_test.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/MsgMgr.h"
#include "ym_utils/MsgHandler.h"
#include "ym_cell/CellDotlibReader.h"
#include "ym_cell/CellLibrary.h"
#include "ym_cell/Cell.h"
#include "ym_cell/CellPin.h"
#include "ym_cell/CellTiming.h"
#include "ym_lexp/LogExpr.h"


BEGIN_NAMESPACE_YM_CELL

int
dotlib_reader_test(const string& filename)
{
  CellDotlibReader reader;

  MsgHandler* mh = new StreamMsgHandler(&cerr);
  mh->set_mask(kMaskAll);
  mh->delete_mask(kMsgInfo);
  mh->delete_mask(kMsgDebug);
  MsgMgr::reg_handler(mh);

  const CellLibrary* library = reader.read(filename);
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
