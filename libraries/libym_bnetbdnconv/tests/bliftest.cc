
/// @file libym_blifbnetconv/tests/bliftest.cc
/// @brief blif ファイルの読み書きのテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bliftest.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bnet/BNetwork.h"
#include "ym_bnet/BNetBlifReader.h"
#include "ym_bdn/BdnMgr.h"
#include "ym_bnetbdnconv/BNetBdnConv.h"
#include "ym_bdn/BdnDumper.h"

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
    StreamMsgHandler msg_handler(&cerr);
    MsgMgr::reg_handler(&msg_handler);

    BNetBlifReader reader;
    BNetwork bnetwork;

    if ( !reader.read(filename, bnetwork) ) {
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
