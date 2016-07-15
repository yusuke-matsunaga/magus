﻿
/// @file bliftest.cc
/// @brief blif ファイルの読み込みのテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "ym/BnNetwork.h"
#include "ym/BNetBlifReader.h"
#include "SbjGraph.h"
#include "SbjDumper.h"

#include "ym/MsgMgr.h"
#include "ym/MsgHandler.h"


BEGIN_NONAMESPACE

const char* argv0 = nullptr;

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

  bool blif_flag = false;
  bool verilog_flag = false;

  int fpos = 1;
  if ( argc == 3 ) {
    if ( argv[1][0] != '-' ) {
      usage();
      return 1;
    }
    if ( strcmp(argv[1], "-blif") == 0 ) {
      blif_flag = true;
      fpos = 2;
    }
    else if ( strcmp(argv[1], "-verilog") == 0 ) {
      verilog_flag = true;
      fpos = 2;
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
    MsgMgr::reg_handler(msg_handler);

    BNetBlifReader reader;

    BNetwork network;

    if ( !reader.read(filename, network) ) {
      cerr << "Error in reading " << filename << endl;
      return 4;
    }

    decomp(network, 2);

    BNet2Sbj bnet2sbj;

    SbjGraph sbjgraph;

    if ( !bnet2sbj(network, sbjgraph, cerr) ) {
      cerr << "Error occured in BNet2Sbj()" << endl;
      return 5;
    }

    SbjDumper dumper;

    if ( blif_flag ) {
      dumper.dump_blif(cout, sbjgraph);
    }
    else if ( verilog_flag ) {
      dumper.dump_verilog(cout, sbjgraph);
    }
    else {
      dumper.dump(cout, sbjgraph);
    }
  }
  catch ( AssertError x) {
    cout << x << endl;
  }

  return 0;
}