
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

struct Info
{
  SizeType mOrNum{0};
  SizeType mXorNum{0};
  SizeType mCplxNum{0};

  Info&
  operator+=(
    const Info& right
  )
  {
    mOrNum += right.mOrNum;
    mXorNum += right.mXorNum;
    mCplxNum += right.mCplxNum;
    return *this;
  }

};

Info
count(
  nsDg::DgEdge edge
)
{
  if ( edge.is_const() ) {
    return Info{};
  }
  auto node = edge.node();
  if ( node->is_lit() ) {
    return Info{};
  }

  Info ans;
  if ( node->is_or() ) {
    ans.mOrNum = 1;
  }
  else if ( node->is_xor() ) {
    ans.mXorNum = 1;
  }
  else {
    ans.mCplxNum = 1;
  }
  SizeType nc = node->child_num();
  for ( SizeType i = 0; i < nc; ++ i ) {
    ans += count(node->child(i));
  }
  return ans;
}

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
    cout << basename(filename);
    string buf;
    vector<Bdd> func_list;
    while ( getline(s, buf) ) {
      auto f = mgr.from_truth(buf);
      func_list.push_back(f);
    }
    nsDg::DgMgr dgmgr;
    SizeType o = 0;
    for ( auto f: func_list ) {
      auto dgedge = dgmgr.decomp(f);
      auto info = count(dgedge);
      ++ o;
      cout << " #" << o << ": "
	   << info.mOrNum
	   << ", "
	   << info.mXorNum
	   << ", "
	   << info.mCplxNum
	   << endl;
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
  return MAGUS_NAMESPACE::dg_test(argc, argv);
}
