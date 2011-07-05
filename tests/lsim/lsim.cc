
/// @file tests/lsim/lsim.cc
/// @brief 論理シミュレータ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnBlifReader.h"
//#include "ym_networks/BdnNode.h"

#include "ym_utils/MsgMgr.h"
#include "ym_utils/MsgHandler.h"

#include "ym_utils/RandGen.h"

#include "ym_utils/StopWatch.h"

#include "LsimNaive.h"


BEGIN_NAMESPACE_YM

void
lsim(const string& filename)
{
  MsgHandler* msg_handler = new StreamMsgHandler(&cerr);
  MsgMgr::reg_handler(msg_handler);

  BdnBlifReader reader;
  BdnMgr network;

  if ( !reader.read(filename, network) ) {
    cerr << "Error in reading " << filename << endl;
    return;
  }

  RandGen rg;

  LsimNaive lsim;

  lsim.set_network(network);

  ymuint nloop = 2000;
  ymuint ni = network.input_num();
  ymuint no = network.output_num();
  vector<ymuint32> iv(ni);
  vector<ymuint32> ov(no);
  for (ymuint i = 0; i < nloop; ++ i) {
    for (ymuint j = 0; j < ni; ++ j) {
      iv[j] = rg.int32();
    }
    lsim.eval(iv, ov);
  }
}

END_NAMESPACE_YM


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

  StopWatch sw;
  sw.start();

  lsim(argv[1]);

  sw.stop();

  USTime time = sw.time();
  cout << time << endl;
}
