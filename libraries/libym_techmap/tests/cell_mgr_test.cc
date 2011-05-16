
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
  TechMap::dump_library(os, *library);
  os.close();

  CellMgr cell_mgr;
  {
    ifstream ifs;
    ifs.open(datafile, ios::binary);
    if ( !ifs ) {
      // エラー
      cerr << "Could not open " << datafile << endl;
      return 3;
    }
    cell_mgr.load(ifs);
  }
  dump(cout, cell_mgr);
}
