
/// @file libym_techmap/test/libdump_test.cc
/// @brief LibDump のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellLibrary.h"
#include "ym_cell/CellDotlibReader.h"
#include "ym_utils/MsgMgr.h"
#include "ym_utils/MsgHandler.h"
#include "cellmap/libdump/LibDump.h"


BEGIN_NAMESPACE_YM_CELLMAP

bool
libdump_test(const char* in_filename)
{
  using nsLibDump::LibDump;

  CellDotlibReader reader;

  MsgHandler* mh = new StreamMsgHandler(&cerr);
  mh->set_mask(kMaskAll);
  mh->delete_mask(kMsgInfo);
  mh->delete_mask(kMsgDebug);
  MsgMgr::reg_handler(mh);

  const CellLibrary* library = reader.read(in_filename);
  if ( library == NULL ) {
    cerr << in_filename << ": Error in reading library" << endl;
    return false;
  }

  LibDump libdump;
  libdump.display(cout, *library);

  return true;
}

END_NAMESPACE_YM_CELLMAP


int
main(int argc,
     char** argv)
{
  using nsYm::nsCellmap::libdump_test;

  const char* filename = argv[1];

  if ( !libdump_test(filename) ) {
    return -1;
  }

  return 0;
}
