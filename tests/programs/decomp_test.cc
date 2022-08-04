
/// @file decomp_test.cc
/// @brief decomp_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "magus.h"
#include "ym/BddMgr.h"
#include "ym/Bdd.h"
#include "DgMgr.h"
#include "ym/BnNetwork.h"


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
    cout << basename(filename);
    string buf;
    vector<Bdd> func_list;
    SizeType ni = 0;
    while ( getline(s, buf) ) {
      auto f = mgr.from_truth(buf);
      func_list.push_back(f);
      SizeType ni_exp = buf.size();
      while ( (1 << ni) < ni_exp ) {
	++ ni;
      }
      ASSERT_COND( (1 << ni) == ni_exp );
    }
    nsDg::DgMgr dgmgr;
    SizeType o = 0;
    for ( auto f: func_list ) {
      auto network = dgmgr.decomp(f, ni);
      cout << " #" << o << endl;
      ++ o;
      network.write(cout);
    }
    cout << endl;
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
