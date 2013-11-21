
/// @file reader_test.cc
/// @brief Iscas89NetworkReader のテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "Iscas89Network.h"
#include "Iscas89NetworkReader.h"
#include "ym_utils/MsgMgr.h"
#include "ym_utils/MsgHandler.h"


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;
  using namespace nsYm::nsNetworks::nsIscas89;

  if ( argc != 2 ) {
    cerr << "USAGE : " << argv[0] << " iscas89-file" << endl;
    return 2;
  }
  string filename = argv[1];

  for (ymuint i = 0; i < 2; ++ i) {
    try {
      StreamMsgHandler* msg_handler = new StreamMsgHandler(&cerr);
      MsgMgr::reg_handler(msg_handler);
      Iscas89NetworkReader reader;

      Iscas89Network network;

      if ( !reader.read(filename, network) ) {
	cerr << "Error in reading " << filename << endl;
	return 4;
      }

      network.write_iscas89(cout);

    }
    catch ( const AssertError& x) {
      cout << x << endl;
    }
  }

  return 0;
}
