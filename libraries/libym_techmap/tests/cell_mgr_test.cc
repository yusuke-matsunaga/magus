
/// @file libym_techmap/test/cell_mgr_test.cc
/// @brief CellMgr のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_techmap/CellMap.h"
#include "cellmap/CellMgr.h"
//#include "cellmap/patgen/patgen_nsdef.h"
#include "ym_cell/CellMislibReader.h"
#include "ym_cell/CellLibrary.h"


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;
  using namespace nsYm::nsCellmap;

  const char* filename = argv[1];
  CellMislibReader reader;
  const CellLibrary* library = reader.read(filename);
  if ( library == NULL ) {
    return 1;
  }

  const char* datafile = "patdata.bin";

  CellMap cell_map;
  {
    ofstream os;
    os.open(datafile, ios::binary);
    if ( !os ) {
      // エラー
      cerr << "Could not create " << datafile << endl;
      return 2;
    }

    cell_map.dump_library(os, *library);
    os.close();
  }

  CellMgr cell_mgr;
  {
    ifstream ifs;
    ifs.open(datafile, ios::binary);
    if ( !ifs ) {
      // エラー
      cerr << "Could not open " << datafile << endl;
      return 3;
    }
    cell_mgr.load_library(ifs);
  }
  dump(cout, cell_mgr);
}
