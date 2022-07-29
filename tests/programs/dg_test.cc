
/// @file dg_test.cc
/// @brief dg_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "magus.h"
#include "ym/BddMgr.h"
#include "ym/Bdd.h"
#include "DgMgr.h"
#include "DgEdge.h"


BEGIN_NAMESPACE_MAGUS

int
dg_test(
  int argc,
  char** argv
)
{
  BddMgr mgr;

  for ( int i = 1; i < argc; ++ i ) {
    string filename = argv[i];
    ifstream s{filename};
    if ( !s ) {
      cerr << filename << ": No such file." << endl;
      return 1;
    }
    string buf;
    vector<Bdd> func_list;
    while ( getline(s, buf) ) {
      auto f = mgr.from_truth(buf);
      func_list.push_back(f);
    }
    nsDg::DgMgr dgmgr{mgr};
    for ( auto f: func_list ) {
      auto dgedge = dgmgr.decomp(f);

    }
  }
  return 0;
}

END_NAMESPACE_MAGUS


int
main(
  int argc,
  char** argv
)
{
  return MAGUS_NAMESPACE::dg_test(argc, argv);
}
