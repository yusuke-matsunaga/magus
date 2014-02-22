
/// @file bliftest.cc
/// @brief blif ファイルの読み書きのテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "networks/BdnMgr.h"
#include "networks/BdnBlifReader.h"
#include "networks/BdnDumper.h"
#include "networks/BdnBlifWriter.h"
#include "networks/BdnVerilogWriter.h"

#include "utils/MsgMgr.h"
#include "utils/MsgHandler.h"


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

    BdnBlifReader read;

    BdnMgr network;
    if ( !read(filename, network) ) {
      cerr << "Error in reading " << filename << endl;
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
