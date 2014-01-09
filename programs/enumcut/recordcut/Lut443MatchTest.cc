
/// @file Lut443MatchTest.cc
/// @brief Lut443Match のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "Lut443Match.h"
#include "GbmNaiveBinary.h"
#include "GbmNaiveEnum.h"
#include "GbmNaiveOneHot.h"
#include "GbmIncrEnum.h"
#include "GbmIncrBinary.h"
#include "GbmIncrOneHot.h"
#include "GbmCegarBinary.h"
#include "GbmCegarOneHot.h"
#include "GbmCegarEnum.h"
#include "ym_utils/RandGen.h"
#include "ym_utils/RandPermGen.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

TvFunc
make_lutfunc(ymuint ni,
	     ymuint nfi,
	     const vector<int>& tv,
	     const vector<TvFunc>& inputs)
{
  ymuint nexp = 1U << nfi;
  assert_cond( nexp == tv.size(), __FILE__, __LINE__);
  assert_cond( nfi == inputs.size(), __FILE__, __LINE__);
  TvFunc func = TvFunc::const_zero(ni);
  for (ymuint b = 0; b < nexp; ++ b) {
    if ( tv[b] ) {
      TvFunc prod = TvFunc::const_one(9);
      for (ymuint i = 0; i < nfi; ++ i) {
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

void
test_common(GbmSolver& solver,
	    ymuint id_list[],
	    const char* testname)
{
  ymuint ni = 0;
  for (ymuint i = 0; i < 9; ++ i) {
    ymuint id = id_list[i];
    if ( ni < id ) {
      ni = id;
    }
  }
  ++ ni;

  RandGen rg;
  RandPermGen rpg(ni);
  Lut443Match matcher;

  vector<TvFunc> orig_inputs(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    orig_inputs[i] = TvFunc::posi_literal(ni, VarId(i));
  }

  for (ymuint i = 0; i < 10; ++ i) {
    vector<TvFunc> inputs(ni);
    rpg.generate(rg);
    for (ymuint j = 0; j < ni; ++ j) {
      inputs[j] = orig_inputs[rpg.elem(j)];
    }

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
    inputs1[0] = inputs[id_list[0]];
    inputs1[1] = inputs[id_list[1]];
    inputs1[2] = inputs[id_list[2]];
    inputs1[3] = inputs[id_list[3]];
    TvFunc lut1 = make_lutfunc(ni, 4, tv1, inputs1);

    vector<TvFunc> inputs2(4);
    inputs2[0] = inputs[id_list[4]];
    inputs2[1] = inputs[id_list[5]];
    inputs2[2] = inputs[id_list[6]];
    inputs2[3] = inputs[id_list[7]];
    TvFunc lut2 = make_lutfunc(ni, 4, tv2, inputs2);

    vector<TvFunc> inputs3(3);
    inputs3[0] = lut1;
    inputs3[1] = lut2;
    inputs3[2] = inputs[id_list[8]];
    TvFunc lut3 = make_lutfunc(ni, 3, tv3, inputs3);

    bool stat = matcher.match(lut3, solver);
    if ( !stat ) {
      cout << "Error in " << testname << endl
	   << "  " << lut3 << endl;
    }
  }
}

END_NONAMESPACE


void
A0Test(GbmSolver& solver)
{
  ymuint id_list[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
  test_common(solver, id_list, "A0 test");
}

void
A1Test(GbmSolver& solver)
{
  ymuint id_list[] = { 0, 1, 2, 3, 0, 4, 5, 6, 7 };
  test_common(solver, id_list, "A1 test");
}

void
A2Test(GbmSolver& solver)
{
  ymuint id_list[] = { 0, 1, 2, 3, 0, 1, 4, 5, 6 };
  test_common(solver, id_list, "A2 test");
}

void
A3Test(GbmSolver& solver)
{
  ymuint id_list[] = { 0, 1, 2, 3, 0, 1, 2, 4, 5 };
  test_common(solver, id_list, "A3 test");
}

void
B0Test(GbmSolver& solver)
{
  ymuint id_list[] = { 0, 1, 2, 3, 4, 5, 6, 7, 0 };
  test_common(solver, id_list, "B0 test");
}

void
B1Test(GbmSolver& solver)
{
  ymuint id_list[] = { 0, 1, 2, 3, 1, 4, 5, 6, 0 };
  test_common(solver, id_list, "B1 test");
}

void
B2Test(GbmSolver& solver)
{
  ymuint id_list[] = { 0, 1, 2, 3, 1, 2, 4, 5, 0 };
  test_common(solver, id_list, "B2 test");
}

void
C0Test(GbmSolver& solver)
{
  ymuint id_list[] = { 0, 1, 2, 3, 0, 4, 5, 6, 0 };
  test_common(solver, id_list, "C0 test");
}

void
C1Test(GbmSolver& solver)
{
  ymuint id_list[] = { 0, 1, 2, 3, 0, 1, 4, 5, 0 };
  test_common(solver, id_list, "C1 test");
}

int
Lut443MatchTest(int argc,
		const char** argv)
{
  GbmSolver* solver = NULL;
  if ( argc > 1 ) {
    if ( strcmp(argv[1], "naive_binary") == 0 ) {
      solver = new GbmNaiveBinary();
    }
    else if ( strcmp(argv[1], "naive_enum") == 0 ) {
      solver = new GbmNaiveEnum();
    }
    else if ( strcmp(argv[1], "naive_onehot") == 0 ) {
      solver = new GbmNaiveOneHot();
    }
    else if ( strcmp(argv[1], "incr_enum") == 0 ) {
      solver = new GbmIncrEnum();
    }
    else if ( strcmp(argv[1], "incr_binary") == 0 ) {
      solver = new GbmIncrBinary();
    }
    else if ( strcmp(argv[1], "incr_onehot") == 0 ) {
      solver = new GbmIncrOneHot();
    }
    else if ( strcmp(argv[1], "cegar_binary") == 0 ) {
      solver = new GbmCegarBinary();
    }
    else if ( strcmp(argv[1], "cegar_onehot") == 0 ) {
      solver = new GbmCegarOneHot();
    }
    else if ( strcmp(argv[1], "cegar_enum") == 0 ) {
      solver = new GbmCegarEnum();
    }
    else {
      cerr << "Illegal method: " << argv[0] << endl;
      return -1;
    }
  }
  if ( solver == NULL ) {
    // fall-back
    solver = new GbmNaiveBinary();
  }

  A0Test(*solver);
  A1Test(*solver);
  A2Test(*solver);
  A3Test(*solver);
  B0Test(*solver);
  B1Test(*solver);
  B2Test(*solver);
  C0Test(*solver);
  C1Test(*solver);

  delete solver;

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
