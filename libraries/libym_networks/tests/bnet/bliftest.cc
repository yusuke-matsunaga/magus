
/// @file bliftest.cc
/// @brief blif ファイルの読み書きのテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/BNetwork.h"
#include "YmNetworks/BNetBlifReader.h"
#include "YmNetworks/BNetBlifWriter.h"
#include "YmUtils/MsgHandler.h"


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

  try {
    MsgHandler* msg_handler = new StreamMsgHandler(&cerr);
    BNetBlifReader reader;

    reader.add_msg_handler(msg_handler);

    BNetwork network;

    if ( !reader.read(filename, network) ) {
      cerr << "Error in reading " << filename << endl;
      return 4;
    }

    BNetwork network2 = network;

    BNetBlifWriter writer;
    writer.dump(cout, network2);
  }
  catch ( AssertError x) {
    cout << x << endl;
  }

  return 0;
}
