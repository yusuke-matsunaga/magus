
/// @file libym_techmap/test/patmgr_test.cc
/// @brief PatMgr のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "patgen/patgen_nsdef.h"
#include "ym_techmap/PatMgr.h"
#include "ym_cell/CellMislibReader.h"


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
  PatMgr::dump_library(os, *library);
  os.close();


  PatMgr pat_mgr;
  {
    ifstream ifs;
    ifs.open(datafile, ios::binary);
    if ( !ifs ) {
      // エラー
      cerr << "Could not open " << filename << endl;
      return 3;
    }

    pat_mgr.load(ifs);
  }

  dump(cout, pat_mgr);
}
