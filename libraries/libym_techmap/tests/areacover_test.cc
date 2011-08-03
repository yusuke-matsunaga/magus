
/// @file libym_techmap/tests/areacover_test.cc
/// @brief AreaCover のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnBlifReader.h"
#include "ym_networks/BdnDumper.h"
#include "ym_techmap/CellMap.h"
#include "ym_networks/CmnMgr.h"
#include "ym_networks/CmnDumper.h"
#include "ym_utils/MsgMgr.h"
#include "ym_utils/MsgHandler.h"


BEGIN_NONAMESPACE

const char* argv0 = NULL;

void
usage()
{
  using namespace std;

  cerr << "USAGE : " << argv0 << " <pattern-filename> <network-filename>"
       << endl;
}

END_NONAMESPACE


BEGIN_NAMESPACE_YM_CELLMAP

void
test(string pat_filename,
     string sbj_filename)
{
  CellMap mapper;

  {
    ifstream ifs;
    ifs.open(pat_filename.c_str(), ios::binary);
    if ( !ifs ) {
      // エラー
      cerr << "Could not open " << pat_filename << endl;
      return;
    }

    if ( !mapper.load_library(ifs) ) {
      // エラー
      cerr << "Error occured during load_library()" << endl;
      return;
    }
  }

  BdnMgr sbjgraph;
  {
    MsgHandler* msg_handler = new StreamMsgHandler(&cerr);
    MsgMgr::reg_handler(msg_handler);

    BdnBlifReader reader;

    if ( !reader.read(sbj_filename, sbjgraph) ) {
      cerr << "Error in reading " << sbj_filename << endl;
      return;
    }
  }

  BdnDumper bdn_dumper;
  bdn_dumper(cout, sbjgraph);

  CmnMgr mapnetwork;

  mapper.area_map(sbjgraph, 0, mapnetwork);

  CmnDumper dump;
  dump(cout, mapnetwork);
}

END_NAMESPACE_YM_CELLMAP


int
main(int argc,
     char** argv)
{
  using nsYm::nsCellmap::test;

  argv0 = argv[0];

  if ( argc != 3 ) {
    usage();
    return 1;
  }

  test(argv[1], argv[2]);

  return 0;
}
