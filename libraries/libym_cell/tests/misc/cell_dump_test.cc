
/// @file cell_dump_test.cc
/// @brief CellLibrary::dump(), restore() のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmCell/CellLibrary.h"
#include "YmCell/CellMislibReader.h"
#include "YmCell/CellDotlibReader.h"

#include "YmUtils/FileIDO.h"
#include "YmUtils/FileODO.h"


BEGIN_NAMESPACE_YM_CELL

bool
dump_load_test(const char* in_filename,
	       bool dotlib)
{
  const CellLibrary* library = NULL;
  if ( dotlib ) {
    CellDotlibReader read;
    library = read(in_filename);
  }
  else {
    CellMislibReader read;
    library = read(in_filename);
  }

  if ( library == NULL ) {
    cerr << in_filename << ": Error in reading library" << endl;
    return false;
  }

  const char* data_filename = "cell_dump.bin";
  {
    FileODO bo;
    if ( !bo.open(data_filename) ) {
      // エラー
      cerr << "Could not create " << data_filename << endl;
      return false;
    }
    library->dump(bo);
  }

  CellLibrary* library2 = NULL;
  {
    FileIDO bi;
    if ( !bi.open(data_filename) ) {
      // エラー
      cerr << "Could not open " << data_filename << endl;
      return false;
    }

    library2 = CellLibrary::new_obj();
    library2->restore(bi);
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
