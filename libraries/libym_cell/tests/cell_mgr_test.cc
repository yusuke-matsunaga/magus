
/// @file cell_mgr_test.cc
/// @brief CellMgr のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellMislibReader.h"
#include "ym_cell/CellDotlibReader.h"
#include "ym_cell/CellLibrary.h"


BEGIN_NAMESPACE_YM_CELL

bool
dump_load_test(const char* in_filename,
	       const char* data_filename)
{


#if 0
  CellMislibReader reader;
#else
  CellDotlibReader reader;
#endif
  const CellLibrary* library = reader.read(in_filename);
  if ( library == NULL ) {
    cerr << in_filename << ": Error in reading library" << endl;
    return false;
  }

  display_library(cout, *library);

#if 0
  {
    ofstream os;
    os.open(data_filename, ios::binary);
    if ( !os ) {
      // エラー
      cerr << "Could not create " << data_filename << endl;
      return false;
    }

    CellMgr::dump_library(os, *library);

    os.close();
  }

  CellMgr cell_mgr;
  {
    ifstream ifs;
    ifs.open(data_filename, ios::binary);
    if ( !ifs ) {
      // エラー
      cerr << "Could not open " << data_filename << endl;
      return false;
    }
    cell_mgr.load_library(ifs);
  }
  dump(cout, cell_mgr);
#endif
  return true;
}

END_NAMESPACE_YM_CELL


int
main(int argc,
     char** argv)
{
  using nsYm::nsCell::dump_load_test;

  const char* filename = argv[1];
  const char* datafile = "patdata.bin";

  if ( !dump_load_test(filename, datafile) ) {
    return -1;
  }

  return 0;
}
