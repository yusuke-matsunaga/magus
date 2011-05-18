
/// @file libym_networks/tests/bliftest.cc
/// @brief blif ファイルの読み書きのテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bliftest.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_blif/BlifNetwork.h"
#include "ym_blif/BlifNetworkReader.h"
#include "ym_networks/BdnMgr.h"
#include "ym_networks/BlifBdnConv.h"
#include "ym_networks/BdnDumper.h"
#include "ym_networks/BdnBlifWriter.h"
#include "ym_networks/BdnVerilogWriter.h"

#include "ym_utils/MsgMgr.h"
#include "ym_utils/MsgHandler.h"


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;

  if ( argc != 2 ) {
    cerr << "USAGE : " << argv[0] << " blif-file" << endl;
    return 2;
  }
  string filename = argv[1];

#if !defined(YM_DEBUG)
  try {
#endif
    MsgHandler* msg_handler = new StreamMsgHandler(&cerr);
    MsgMgr::reg_handler(msg_handler);

    BlifNetworkReader reader;

    BlifNetwork blif_network;

    if ( !reader.read(filename, blif_network) ) {
      cerr << "Error in reading " << filename << endl;
      return 4;
    }

    BdnMgr network;
    BlifBdnConv conv;
    bool stat = conv(blif_network, network);
    if ( !stat ) {
      cerr << "Error in converting from BlifNetwork to BdnMgr" << endl;
      return 5;
    }

    BdnDumper dump;
    BdnBlifWriter blif_writer;
    BdnVerilogWriter verilog_writer;

    dump(cout, network);
    blif_writer(cout, network);
    verilog_writer(cout, network);

    cout << endl;
    cout << "after copy" << endl;
    cout << endl;

    BdnMgr network2 = network;

    dump(cout, network2);
    blif_writer(cout, network2);
    verilog_writer(cout, network2);

#if !defined(YM_DEBUG)
  }
  catch ( AssertError x) {
    cout << x << endl;
  }
#endif

  return 0;
}
