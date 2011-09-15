
/// @file libym_cell/test/dump_restore_test.cc
/// @brief dump/restore のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellLibrary.h"
#include "ym_cell/CellMislibReader.h"
#include "ym_cell/CellDotlibReader.h"


BEGIN_NAMESPACE_YM_CELL

/// @brief genlib 形式のファイルを読み込む．
/// @param[in] filename ファイル名
/// @return 生成されたライブラリを返す．
/// @note エラーが起きたら NULL を返す．
const CellLibrary*
read_mislib(const char* filename)
{
  CellMislibReader reader;
  return reader.read(filename);
}

/// @brief liberty 形式のファイルを読み込む．
/// @param[in] filename ファイル名
/// @return 生成されたライブラリを返す．
/// @note エラーが起きたら NULL を返す．
const CellLibrary*
read_dotlib(const char* filename)
{
  CellDotlibReader reader;
  return reader.read(filename);
}

END_NAMESPACE_YM_CELL


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm::nsCell;

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

  const CellLibrary* library = NULL;
  if ( dotlib ) {
    library = read_dotlib(filename);
  }
  else {
    library = read_mislib(filename);
  }
  if ( library == NULL ) {
    cerr << filename << ": Error in reading library" << endl;
    return 1;
  }

  const char* datafile = "patdata.bin";
  {
    ofstream os;
    os.open(datafile, ios::binary);
    if ( !os ) {
      // エラー
      cerr << "Could not create " << datafile << endl;
      return 2;
    }
    library->dump(os);
  }

  {
    ifstream ifs;
    ifs.open(datafile, ios::binary);
    if ( !ifs ) {
      // エラー
      cerr << "Could not open " << datafile << endl;
      return 3;
    }
    CellLibrary* library2 = CellLibrary::new_obj();
    library2->restore(ifs);
    display_library(cout, *library2);
  }

  return 0;
}
