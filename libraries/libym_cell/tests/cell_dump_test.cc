
/// @file cell_dump_test.cc
/// @brief CellLibrary::dump(), restore() のテストプログラム
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
	       bool dotlib)
{
  const CellLibrary* library = NULL;
  if ( dotlib ) {
    CellDotlibReader reader;
    library = reader.read(in_filename);
  }
  else {
    CellMislibReader reader;
    library = reader.read(in_filename);
  }

  if ( library == NULL ) {
    cerr << in_filename << ": Error in reading library" << endl;
    return false;
  }

  const char* data_filename = "cell_dump.bin";
  {
    ofstream os;
    os.open(data_filename, ios::binary);
    if ( !os ) {
      // エラー
      cerr << "Could not create " << data_filename << endl;
      return false;
    }

    dump_library(os, *library);

    os.close();
  }

  const CellLibrary* library2 = NULL;
  {
    ifstream ifs;
    ifs.open(data_filename, ios::binary);
    if ( !ifs ) {
      // エラー
      cerr << "Could not open " << data_filename << endl;
      return false;
    }
    library2 = restore_library(ifs);
  }

  display_library(cout, *library2);

  return true;
}

END_NAMESPACE_YM_CELL


int
main(int argc,
     char** argv)
{
  using namespace std;
  using nsYm::nsCell::dump_load_test;

  if ( argc < 2 ) {
    cerr << "Usage: " << argv[0] << " [--liberty] <liberty-file>" << endl;
    return 1;
  }
  ymuint base = 1;
  bool dotlib = false;
  if ( argc == 3 && strcmp(argv[1], "--liberty") == 0 ) {
    dotlib = true;
    base = 2;
  }

  const char* filename = argv[base];

  if ( !dump_load_test(filename, dotlib) ) {
    return -1;
  }

  return 0;
}
