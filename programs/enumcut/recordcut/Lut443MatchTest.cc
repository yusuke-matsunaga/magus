
/// @file Lut443MatchTest.cc
/// @brief Lut443Match のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "Lut443Match.h"
#include "ym_utils/RandGen.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

TvFunc
make_lutfunc(ymuint ni,
	     const vector<int>& tv,
	     const vector<TvFunc>& inputs)
{
  ymuint nexp = 1U << ni;
  assert_cond( nexp == tv.size(), __FILE__, __LINE__);
  assert_cond( ni == inputs.size(), __FILE__, __LINE__);
  TvFunc func = TvFunc::const_zero(9);
  for (ymuint b = 0; b < nexp; ++ b) {
    if ( tv[b] ) {
      TvFunc prod = TvFunc::const_one(9);
      for (ymuint i = 0; i < ni; ++ i) {
	if ( b & (1U << i) ) {
	  prod &= inputs[i];
	}
	else {
	  prod &= ~inputs[i];
	}
      }
      func |= prod;
    }
  }
  return func;
}

END_NONAMESPACE


int
Lut443MatchTest(int argc,
		const char** argv)
{
  RandGen rg;
  Lut443Match matcher;

  vector<TvFunc> inputs(9);
  for (ymuint i = 0; i < 9; ++ i) {
    inputs[i] = TvFunc::posi_literal(9, VarId(i));
  }

#if 0
  TvFunc and9 = TvFunc::const_one(9);
  for (ymuint i = 0; i < 9; ++ i) {
    and9 &= inputs[i];
  }
  if ( !matcher.match(and9) ) {
    cout << "Error on and9" << endl;
  }
#endif

  for (ymuint i = 0; i < 10; ++ i) {
    vector<int> tv1(16);
    for (ymuint b = 0; b < 16; ++ b) {
      tv1[b] = rg.int32() & 1;
    }

    vector<int> tv2(16);
    for (ymuint b = 0; b < 16; ++ b) {
      tv2[b] = rg.int32() & 1;
    }

    vector<int> tv3(8);
    for (ymuint b = 0; b < 8; ++ b) {
      tv3[b] = rg.int32() & 1;
    }

    vector<TvFunc> inputs1(4);
    inputs1[0] = inputs[0];
    inputs1[1] = inputs[1];
    inputs1[2] = inputs[2];
    inputs1[3] = inputs[3];
    TvFunc lut1 = make_lutfunc(4, tv1, inputs1);

    vector<TvFunc> inputs2(4);
    inputs2[0] = inputs[4];
    inputs2[1] = inputs[5];
    inputs2[2] = inputs[6];
    inputs2[3] = inputs[7];
    TvFunc lut2 = make_lutfunc(4, tv2, inputs2);

    vector<TvFunc> inputs3(3);
    inputs3[0] = lut1;
    inputs3[1] = lut2;
    inputs3[2] = inputs[8];
    TvFunc lut3 = make_lutfunc(3, tv3, inputs3);

    bool stat = matcher.match(lut3);
    if ( !stat ) {
      cout << "Error: " << lut3 << endl;
    }
  }

  return 0;
}

END_NAMESPACE_YM


int
main(int argc,
     const char** argv)
{
  using nsYm::Lut443MatchTest;

  return Lut443MatchTest(argc, argv);
}
