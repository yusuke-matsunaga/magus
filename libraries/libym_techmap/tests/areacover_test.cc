
/// @file libym_techmap/tests/areacover_test.cc
/// @brief AreaCover のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_blif/BlifNetwork.h"
#include "ym_blif/BlifNetworkReader.h"
#include "ym_bdn/BdnMgr.h"
#include "ym_bdn/BlifBdnConv.h"
#include "ym_bdn/BdnDumper.h"
#include "ym_techmap/CellMap.h"
#include "ym_techmap/CnGraph.h"
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


BEGIN_NAMESPACE_YM_TECHMAP

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

    BlifNetworkReader reader;

    BlifNetwork blif_network;

    if ( !reader.read(sbj_filename, blif_network) ) {
      cerr << "Error in reading " << sbj_filename << endl;
      return;
    }

    BlifBdnConv conv;
    bool stat = conv(blif_network, sbjgraph);
    if ( !stat ) {
      cerr << "Error in converting form BlifNetwork to BdnMgr" << endl;
      return;
    }
  }

  BdnDumper bdn_dumper;
  bdn_dumper(cout, sbjgraph);

  CnGraph mapnetwork;

  mapper.area_map(sbjgraph, 0, mapnetwork);

#if 1
  dump_verilog(cout, mapnetwork);
#else
  dump_spice(cout, mapnetwork);
#endif
}

END_NAMESPACE_YM_TECHMAP


int
main(int argc,
     char** argv)
{
  argv0 = argv[0];

  if ( argc != 3 ) {
    usage();
    return 1;
  }

  nsYm::nsTechmap::test(argv[1], argv[2]);

  return 0;
}
