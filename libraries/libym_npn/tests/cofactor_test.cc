
/// @file cofactor_test.cc
/// @brief TvFunc::cofactor() のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_npn/TvFunc.h"
#include "TvFuncTest.h"


BEGIN_NAMESPACE_YM

TvFunc
str2func(const char* pat)
{
  const char* bar = strchr(pat, '|');
  ymuint ni = 0;
  for (const char* p = pat; p != bar; ++ p) {
    char c = *p;
    if ( c == ' ' || c == '\t' ) {
      continue;
    }
    if ( c >= '0' && c <= '9' ) {
      ni *= 10;
      ni += static_cast<ymuint>(c) - static_cast<ymuint>('0');
      continue;
    }
    cerr << pat << " : Invalid form for TvFunc description" << endl
	 << "\tIt should be <#-of-inputs>|<bit pattern of length 2^ni>" << endl;
    exit(-1);
  }

  ymuint ni_pow = 1U << ni;
  vector<int> vect(ni_pow);
  const char* bp = bar + 1;
  for (char c = *bp; c != '\0'; ++ bp, c = *bp) {
    if ( c != ' ' && c != '\t' ) {
      break;
    }
  }
  for (ymuint i = 0; i < ni_pow; ++ i) {
    char c = bp[i];
    if ( c == '0' ) {
      vect[i] = 0;
    }
    else if ( c == '1' ) {
      vect[i] = 1;
    }
    else {
      cerr << pat << " : Invalid form for TvFunc description" << endl
	   << "\tIt should be <#-of-inputs>|<bit pattern of length 2^ni>" << endl;
      exit(-1);
    }
  }

  return TvFunc(ni, vect);
}

void
cofactor_test(int argc,
	      char** argv)
{
  TvFuncTest test(cout);

  TvFunc func = str2func(argv[1]);
  ymuint pos = atoi(argv[2]);
  tPol pol;
  if ( strcmp(argv[3], "p") == 0 ) {
    pol = kPolPosi;
  }
  else {
    pol = kPolNega;
  }

  test.check_cofactor(func, pos, pol);

  cout << test.nerr() << " error(s)." << endl;
}

END_NAMESPACE_YM

int
main(int argc,
     char** argv)
{
  using namespace std;

  if ( argc != 4 ) {
    cerr << "Usage: " << argv[0] << " <func> <pos> <pol>" << endl;
    return -1;
  }

  nsYm::cofactor_test(argc, argv);

  return 1;
}
