
/// @file areacover_test.cc
/// @brief AreaCover のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "YmCell/CellLibrary.h"
#include "YmCell/CellMislibReader.h"
#include "YmCell/CellDotlibReader.h"
#include "YmNetworks/BdnMgr.h"
#include "YmNetworks/BdnBlifReader.h"
#include "YmNetworks/BdnDumper.h"
#include "CellMap.h"
#include "YmNetworks/CmnMgr.h"
#include "YmNetworks/CmnDumper.h"
#include "YmUtils/MsgMgr.h"
#include "YmUtils/MsgHandler.h"


BEGIN_NONAMESPACE

const char* argv0 = nullptr;

void
usage()
{
  using namespace std;

  cerr << "USAGE : " << argv0 << " <pattern-filename> <network-filename>"
       << endl;
}

END_NONAMESPACE


BEGIN_NAMESPACE_YM

/// @brief genlib 形式のファイルを読み込む．
/// @param[in] filename ファイル名
/// @return 生成されたライブラリを返す．
/// @note エラーが起きたら nullptr を返す．
const CellLibrary*
read_mislib(const char* filename)
{
  CellMislibReader read;
  return read(filename);
}

/// @brief liberty 形式のファイルを読み込む．
/// @param[in] filename ファイル名
/// @return 生成されたライブラリを返す．
/// @note エラーが起きたら nullptr を返す．
const CellLibrary*
read_dotlib(const char* filename)
{
  CellDotlibReader read;
  return read(filename);
}

END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_CELLMAP

void
test(const CellLibrary* library,
     string sbj_filename)
{
  BdnMgr sbjgraph;
  {
    MsgHandler* msg_handler = new StreamMsgHandler(&cerr);
    MsgMgr::reg_handler(msg_handler);

    BdnBlifReader read;

    if ( !read(sbj_filename, sbjgraph) ) {
      cerr << "Error in reading " << sbj_filename << endl;
      return;
    }
  }

  BdnDumper bdn_dumper;
  bdn_dumper(cout, sbjgraph);

  CmnMgr mapnetwork;

  CellMap mapper;
  mapper.area_map(*library, sbjgraph, 0, mapnetwork);

  CmnDumper dump;
  dump(cout, mapnetwork);
}

END_NAMESPACE_YM_CELLMAP


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;
  using nsYm::nsCellmap::test;

  argv0 = argv[0];

  ymuint base = 1;
  bool dotlib = false;
  if ( argc == 4 && strcmp(argv[1], "--liberty") == 0 ) {
    dotlib = true;
    base = 2;
  }

  if ( argc - base != 2 ) {
    usage();
    return 1;
  }

  const char* libname = argv[base];

  const CellLibrary* library = nullptr;
  if ( dotlib ) {
    library = read_dotlib(libname);
  }
  else {
    library = read_mislib(libname);
  }
  if ( library == nullptr ) {
    cerr << libname << ": Error in reading library" << endl;
    return 1;
  }

  test(library, argv[base + 1]);

  return 0;
}
