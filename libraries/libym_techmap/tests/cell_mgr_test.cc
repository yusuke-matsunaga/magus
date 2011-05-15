
/// @file libym_techmap/test/cell_mgr_test.cc
/// @brief PatMgr のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "patgen/patgen_nsdef.h"
#include "ym_techmap/TechMap.h"
#include "CellMgr.h"
#include "ym_cell/CellMislibReader.h"
#include "ym_cell/CellLibrary.h"

int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;
  using namespace nsYm::nsTechmap;

  const char* filename = argv[1];
  CellMislibReader reader;
  const CellLibrary* library = reader.read(filename);
  if ( library == NULL ) {
    return 1;
  }

  const char* datafile = "patdata.bin";

  ofstream os;
  os.open(datafile, ios::binary);
  if ( !os ) {
    // エラー
    cerr << "Could not create " << datafile << endl;
    return 2;
  }
#if 0
  TechMap::dump_library(os, *library);
#else
  nsYm::nsCell::dump_library(os, *library);
#endif
  os.close();

  CellMgr cell_mgr;
  {
    ifstream ifs;
    ifs.open(datafile, ios::binary);
    if ( !ifs ) {
      // エラー
      cerr << "Could not open " << filename << endl;
      return 3;
    }
#if 0
    cell_mgr.load(ifs);
#else
    const CellLibrary* library2 = nsYm::nsCell::restore_library(ifs);
    if ( library2 ) {
      display_library(cout, *library2);
    }
#endif
  }
#if 0
  dump(cout, cell_mgr);
#endif
}
