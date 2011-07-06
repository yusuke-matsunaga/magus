
/// @file tests/lsim/lsim.cc
/// @brief 論理シミュレータ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnBlifReader.h"
#include "ym_networks/BdnIscas89Reader.h"

#include "ym_utils/MsgMgr.h"
#include "ym_utils/MsgHandler.h"

#include "ym_utils/RandGen.h"

#include "ym_utils/StopWatch.h"

#include "LsimNaive.h"


BEGIN_NAMESPACE_YM

void
lsim(const string& filename,
     bool blif,
     bool iscas89)
{
  MsgHandler* msg_handler = new StreamMsgHandler(&cerr);
  MsgMgr::reg_handler(msg_handler);

  BdnMgr network;

  if ( blif ) {
    BdnBlifReader reader;
    if ( !reader.read(filename, network) ) {
      cerr << "Error in reading " << filename << endl;
      return;
    }
  }
  else {
    BdnIscas89Reader reader;
    if ( !reader.read(filename, network) ) {
      cerr << "Error in reading " << filename << endl;
      return;
    }
  }

  RandGen rg;

  LsimNaive lsim;

  lsim.set_network(network);

  ymuint nloop = 20000;
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

  bool blif = false;
  bool iscas = false;

  int i = 1;
  for ( ; i < argc; ++ i) {
    if ( argv[i][0] != '-' ) {
      break;
    }
    if ( strcmp(argv[i], "-blif") == 0 ) {
      blif = true;
      iscas = false;
    }
    else if ( strcmp(argv[i], "-iscas89") == 0 ) {
      blif = false;
      iscas = true;
    }
    else {
      break;
    }
  }
  if ( blif == false && iscas == false ) {
    blif = true;
  }

  if ( i + 1 != argc ) {
    cerr << "USAGE : " << argv[0] << " [-blif|-iscas89] file-name" << endl;
    return 2;
  }

  StopWatch sw;
  sw.start();

  lsim(argv[i], blif, iscas);

  sw.stop();

  USTime time = sw.time();
  cout << time << endl;
}
