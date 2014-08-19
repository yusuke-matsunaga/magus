
/// @file dump_restore_test.cc
/// @brief dump/restore のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmCell/CellLibrary.h"
#include "YmCell/CellMislibReader.h"
#include "YmCell/CellDotlibReader.h"

#include "YmUtils/FileIDO.h"
#include "YmUtils/FileODO.h"


BEGIN_NAMESPACE_YM

/// @brief genlib 形式のファイルを読み込む．
/// @param[in] filename ファイル名
/// @return 生成されたライブラリを返す．
/// @note エラーが起きたら NULL を返す．
const CellLibrary*
read_mislib(const char* filename)
{
  CellMislibReader read;
  return read(filename);
}

/// @brief liberty 形式のファイルを読み込む．
/// @param[in] filename ファイル名
/// @return 生成されたライブラリを返す．
/// @note エラーが起きたら NULL を返す．
const CellLibrary*
read_dotlib(const char* filename)
{
  CellDotlibReader read;
  return read(filename);
}

END_NAMESPACE_YM


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;

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
  ofstream os1;
  os1.open("dump1.cell", ios::binary);
  if ( !os1 ) {
    cerr << "Could not create " << "dump1.cell" << endl;
    return 2;
  }
  display_library(os1, *library);
  os1.close();

  const char* datafile = "patdata.bin";
  {
    FileODO bo;
    if ( !bo.open(datafile) ) {
      // エラー
      cerr << "Could not create " << datafile << endl;
      return 2;
    }
    library->dump(bo);
  }

  {
    FileIDO bi;
    if ( !bi.open(datafile) ) {
      // エラー
      cerr << "Could not open " << datafile << endl;
      return 3;
    }

    CellLibrary* library2 = CellLibrary::new_obj();
    library2->restore(bi);

    ofstream os2;
    os2.open("dump2.cell", ios::binary);
    if ( !os2 ) {
      cerr << "Could not create " << "dump2.cell" << endl;
      return 2;
    }
    display_library(os2, *library2);
    os2.close();
  }

  return 0;
}
