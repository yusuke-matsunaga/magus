
/// @file libym_lutmap/tests/bliftest.cc
/// @brief blif ファイルの読み込みのテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bliftest.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bnet/BNetwork.h"
#include "ym_bnet/BNetBlifReader.h"
#include "ym_bnet/BNetDecomp.h"
#include "ym_lutmap/BNet2Sbj.h"
#include "ym_lutmap/SbjGraph.h"
#include "ym_utils/MsgHandler.h"
#include "ym_utils/StopWatch.h"


BEGIN_NONAMESPACE

const char* argv0 = NULL;

void
usage()
{
  using namespace std;

  cerr << "USAGE : " << argv0 << " [-blif|-verilog] blif-file" << endl;
}

END_NONAMESPACE


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;

  argv0 = argv[0];

  int fpos = 1;
  if ( argc == 3 ) {
    if ( argv[1][0] != '-' ) {
      usage();
      return 1;
    }
    else {
      usage();
      return 2;
    }
  }
  if ( fpos + 1 != argc ) {
    usage();
    return 3;
  }

  string filename = argv[fpos];

  try {
    MsgHandler* msg_handler = new StreamMsgHandler(&cerr);
    BNetBlifReader reader;

    reader.add_msg_handler(msg_handler);

    BNetwork network;

    if ( !reader.read(filename, network) ) {
      cerr << "Error in reading " << filename << endl;
      return 4;
    }

    BNetDecomp decomp;

    decomp(network, 2);

    nsLutmap::BNet2Sbj bnet2sbj;

    SbjGraph sbjgraph;

    if ( !bnet2sbj(network, sbjgraph, cerr) ) {
      cerr << "Error occured in BNet2Sbj()" << endl;
      return 5;
    }

    StopWatch timer;

    timer.start();
    sbjgraph.get_min_depth(12);
    timer.stop();

    USTime t = timer.time();
    cout << t << endl;
  }
  catch ( AssertError x) {
    cout << x << endl;
  }

  return 0;
}
