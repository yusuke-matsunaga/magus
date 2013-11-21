
/// @file reader_test.cc
/// @brief BlifNetworkReader のテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/BlifNetwork.h"

#include "ym_cell/CellMislibReader.h"
#include "ym_cell/CellLibrary.h"

#include "ym_utils/MsgMgr.h"
#include "ym_utils/MsgHandler.h"


BEGIN_NAMESPACE_YM

int
reader_test(int argc,
	    const char** argv)
{
  if ( argc < 2 || argc > 3 ) {
    cerr << "USAGE : " << argv[0] << " blif-file [mislib-file]" << endl;
    return 1;
  }

  try {
    StreamMsgHandler* msg_handler = new StreamMsgHandler(&cerr);
    MsgMgr::reg_handler(msg_handler);

    const CellLibrary* library = NULL;

    if ( argc == 3 ) {
      CellMislibReader read;
      library = read(argv[2]);
      if ( library == NULL ) {
	return 2;
      }
    }

    string filename = argv[1];

    BlifNetwork network;

    if ( !network.read_blif(filename, library) ) {
      cerr << "Error in reading " << filename << endl;
      return 4;
    }

    network.write_blif(cout);

  }
  catch ( const AssertError& x) {
    cout << x << endl;
  }

  return 0;
}

END_NAMESPACE_YM

int
main(int argc,
     const char** argv)
{
  return nsYm::reader_test(argc, argv);
}
