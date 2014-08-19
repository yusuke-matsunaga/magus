
/// @file gctest.cc
/// @brief GcSolver のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "utils/GcSolver.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

ymuint
read_num(const string& str,
	 ymuint& rpos)
{
  ymuint ans = 0;
  for ( ; ; ) {
    char c = str[rpos];
    ++ rpos;
    if ( c >= '0' && c <= '9' ) {
      ans = ans * 10 + static_cast<ymuint>(c - '0');
    }
    if ( c == ' ' || c == '\0' ) {
      return ans;
    }
  }
}

END_NONAMESPACE

int
gctest(int argc,
       const char** argv)
{
  istream* p_s = NULL;

  if ( argc == 1 ) {
    p_s = &cin;
  }
  else {
    p_s = new ifstream(argv[1]);
    if ( !*p_s ) {
      cerr << argv[1] << ": No such file." << endl;
      return -1;
    }
  }

  istream& s = *p_s;

  string buf;
  if ( !getline(s, buf) ) {
    cerr << "read error at line 1." << endl;
    return -1;
  }

  ymuint rpos = 0;
  ymuint n = read_num(buf, rpos);
  if ( n == 0 ) {
    cerr << "illegal character at line 1." << endl;
    return -1;
  }

  GcSolver gcmgr;
  gcmgr.init(n);

  for (ymuint i = 0; i < n; ++ i) {
    if ( !getline(s, buf) ) {
      cerr << "read error at line " << (i + 2) << endl;
      return -1;
    }
    ymuint len = buf.size();
    for (ymuint rpos = 0; rpos < len; ) {
      ymuint id = read_num(buf, rpos);
      gcmgr.connect(i, id);
    }
  }

  gcmgr.dump(cout);

  vector<vector<ymuint> > color_group;
  ymint nc = gcmgr.coloring(color_group);

  for (ymuint i = 0; i < nc; ++ i) {
    const vector<ymuint>& id_list = color_group[i];
    for (vector<ymuint>::const_iterator p = id_list.begin();
	 p != id_list.end(); ++ p) {
      cout << " " << *p;
    }
    cout << endl;
  }

  return 0;
}

END_NAMESPACE_YM


int
main(int argc,
     const char** argv)
{
  return nsYm::gctest(argc, argv);
}
