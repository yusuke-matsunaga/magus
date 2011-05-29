
/// @file libym_cell/test/dump_restore_test.cc
/// @brief dump/restore のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellLibrary.h"
#include "ym_cell/CellMislibReader.h"


BEGIN_NAMESPACE_YM_CELL

int
dump_restore_test(const char* filename)
{
  const char* datafile = "patdata.bin";

  {
    CellMislibReader reader;
    const CellLibrary* library = reader.read(filename);
    if ( library == NULL ) {
      return 1;
    }

    {
      ofstream os;
      os.open(datafile, ios::binary);
      if ( !os ) {
	// エラー
	cerr << "Could not create " << datafile << endl;
	return 2;
      }
      dump_library(os, *library);
      os.close();
    }
  }

  {
    ifstream ifs;
    ifs.open(datafile, ios::binary);
    if ( !ifs ) {
      // エラー
      cerr << "Could not open " << datafile << endl;
      return 3;
    }
    const CellLibrary* library2 = restore_library(ifs);
    if ( library2 ) {
      display_library(cout, *library2);
    }
  }

  return 0;
}

END_NAMESPACE_YM_CELL


int
main(int argc,
     char** argv)
{
  using namespace std;

  if ( argc != 2 ) {
    cout << "usage: " << argv[0] << " <genlib-filename>" << endl;
    return -1;
  }

  const char* filename = argv[1];

  int stat = nsYm::nsCell::dump_restore_test(filename);
  if ( stat != 0 ) {
    cout << "Error occured. status code = " << stat << endl;
  }

  return stat;
}
