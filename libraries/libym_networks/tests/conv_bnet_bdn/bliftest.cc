
/// @file bliftest.cc
/// @brief blif ファイルの読み書きのテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "networks/BNetwork.h"
#include "networks/BNetBlifReader.h"
#include "networks/BdnMgr.h"
#include "networks/BNetBdnConv.h"
#include "networks/BdnDumper.h"

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
    StreamMsgHandler msg_handler(&cerr);
    MsgMgr::reg_handler(&msg_handler);

    BNetBlifReader read;
    BNetwork bnetwork;

    if ( !read(filename, bnetwork) ) {
      cerr << "Error in reading " << filename << endl;
      return 4;
    }

    BdnMgr bdn;

    BNetBdnConv conv;
    if ( !conv(bnetwork, bdn) ) {
      cerr << "Error in converting BNetwork to Bdn" << endl;
      return 5;
    }

    BdnDumper dump;

    dump(cout, bdn);
#if !defined(YM_DEBUG)
  }
  catch ( AssertError x) {
    cout << x << endl;
  }
#endif

  return 0;
}
