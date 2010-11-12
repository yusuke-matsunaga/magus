
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
#include "ym_lutmap/SbjMinDepth.h"
#include "ym_utils/MsgHandler.h"
#include "ym_utils/StopWatch.h"


BEGIN_NONAMESPACE

const char* argv0 = NULL;

void
usage()
{
  using namespace std;

  cerr << "USAGE : " << argv0 << " input-size blif-file" << endl;
}

END_NONAMESPACE


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;

  argv0 = argv[0];
  if ( argc != 3 ) {
    usage();
    return 3;
  }

  ymuint k = atoi(argv[1]);
  string filename = argv[2];

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

#if 1
    timer.start();
    ymuint d1 = sbjgraph.get_min_depth(k);
    timer.stop();

    cout << "k = " << k << ", depth = " << d1 << endl;
    USTime t = timer.time();
    cout << t << endl;
#endif

#if 1
    nsLutmap::SbjMinDepth mindepth;

    timer.reset();
    timer.start();
    ymuint d2 = mindepth.get_min_depth(sbjgraph, k);
    timer.stop();

    cout << "k = " << k << ", depth = " << d2 << endl;
    USTime t2 = timer.time();
    cout << t2 << endl;
#endif
  }
  catch ( AssertError x) {
    cout << x << endl;
  }

  return 0;
}
