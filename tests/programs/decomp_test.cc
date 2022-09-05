
/// @file decomp_test.cc
/// @brief decomp_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "magus.h"
#include "ym/BddMgr.h"
#include "ym/Bdd.h"
#include "DjDecomp.h"
#include "EquivMgr.h"
#include "ym/BnNetwork.h"
#include "ym/SatBool3.h"


BEGIN_NAMESPACE_MAGUS

BEGIN_NONAMESPACE

inline
string
basename(
  const string& pathname
)
{
  auto pos = pathname.find_last_of('/');
  if ( pos == string::npos ) {
    return pathname;
  }
  return pathname.substr(pos + 1);
}

END_NONAMESPACE

int
decomp_test(
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
    cout << basename(filename) << endl;
    auto network = BnNetwork::read_truth(filename);
    network.write(cout);
    nsDg::DjDecomp op;
    op.decomp(network);
    BnNetwork dg_network{std::move(op)};
    dg_network.write(cout);

    EquivMgr eqmgr;
    auto ans = eqmgr.check(network, dg_network);
    if ( ans.result() != SatBool3::True ) {
      cout << "Not equiv" << endl;
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
  return MAGUS_NAMESPACE::decomp_test(argc, argv);
}
