
/// @file libdump_test.cc
/// @brief LibDump のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmCell/CellLibrary.h"
#include "YmCell/CellDotlibReader.h"
#include "YmUtils/MsgMgr.h"
#include "YmUtils/MsgHandler.h"
#include "libdump/LibDump.h"


BEGIN_NAMESPACE_YM_CELL

bool
libdump_test(const char* in_filename)
{
  using nsLibdump::LibDump;

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

END_NAMESPACE_YM_CELL


int
main(int argc,
     char** argv)
{
  using nsYm::nsCell::libdump_test;

  const char* filename = argv[1];

  if ( !libdump_test(filename) ) {
    return -1;
  }

  return 0;
}
