
/// @file Lut444Match.cc
/// @brief Lut444Match の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "Lut444Match.h"
#include "YmUtils/StopWatch.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

bool debug = false;

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス Lut444Match
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

END_NONAMESPACE

// @brief コンストラクタ
Lut444Match::Lut444Match()
{
  mMatcher.set_input_range(6, 10);

  // LUT-A0 を作る．
  ymuint id_a0[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  ymuint sym_a0[] = { 0, 1, 2, 3, 0,
		      4, 5, 6, 7, 4,
		      0, 4, 0,
		      8, 9, 8,
		      10 };
  make_lut444(10, id_a0, sym_a0);

  // LUT-A1 を作る．
  ymuint id_a1[] = { 0, 1, 2, 3, 0, 4, 5, 6, 7, 8 };
  ymuint sym_a1[] = { 1, 2, 3, 1,
		      4, 5, 6, 4,
		      1, 4, 1,
		      7, 8, 7,
		      9 };
  make_lut444(9, id_a1, sym_a1);

  // LUT-A2 を作る．
  ymuint id_a2[] = { 0, 1, 2, 3, 0, 1, 4, 5, 6, 7 };
  ymuint sym_a2[] = { 0, 1, 0,
		      2, 3, 2,
		      4, 5, 4,
		      2, 4, 2,
		      6, 7, 6,
		      8 };
  make_lut444(8, id_a2, sym_a2);

  // LUT-A3 を作る．
  ymuint id_a3[] = { 0, 1, 2, 3, 0, 1, 2, 4, 5, 6 };
  ymuint sym_a3[] = { 0, 1, 2, 0,
		      3, 4, 3,
		      5, 6, 5,
		      7 };
  make_lut444(7, id_a3, sym_a3);

  // LUT-A4 を作る．
  ymuint id_a4[] = { 0, 1, 2, 3, 0, 1, 2, 3, 4, 5 };
  ymuint sym_a4[] = { 0, 1, 2, 3, 0,
		      4, 5, 4,
		      6 };
  make_lut444(6, id_a4, sym_a4);

  // LUT-B0 を作る．
  ymuint id_b0[] = { 0, 1, 2, 3, 4, 5, 6, 7, 0, 8 };
  ymuint sym_b0[] = { 1, 2, 3, 1,
		      4, 5, 6, 7, 4,
		      9 };
  make_lut444(9, id_b0, sym_b0);

  // LUT-B1 を作る．
  ymuint id_b1[] = { 0, 1, 2, 3, 1, 4, 5, 6, 0, 7 };
  ymuint sym_b1[] = { 2, 3, 2,
		      4, 5, 6, 4,
		      8 };
  make_lut444(8, id_b1, sym_b1);

  // LUT-B2 を作る．
  ymuint id_b2[] = { 0, 1, 2, 3, 1, 2, 4, 5, 0, 6 };
  ymuint sym_b2[] = { 1, 2, 1,
		      4, 5, 4,
		      7 };
  make_lut444(7, id_b2, sym_b2);

  // LUT-B3 を作る．
  ymuint id_b3[] = { 0, 1, 2, 3, 1, 2, 3, 4, 0, 5 };
  ymuint sym_b3[] = { 1, 2, 3, 1,
		      6 };
  make_lut444(6, id_b3, sym_b3);

  // LUT-C0 を作る．
  ymuint id_c0[] = { 0, 1, 2, 3, 4, 5, 6, 7, 0, 1 };
  ymuint sym_c0[] = { 0, 1, 0,
		      2, 3, 2,
		      4, 5, 6, 7, 4,
		      8 };
  make_lut444(8, id_c0, sym_c0);

  // LUT-C1 を作る．
  ymuint id_c1[] = { 0, 1, 2, 3, 2, 4, 5, 6, 0, 1 };
  ymuint sym_c1[] = { 0, 1, 0,
		      4, 5, 6, 4,
		      7 };
  make_lut444(7, id_c1, sym_c1);

  // LUT-C2 を作る．
  ymuint id_c2[] = { 0, 1, 2, 3, 2, 3, 4, 5, 0, 1 };
  ymuint sym_c2[] = { 0, 1, 0,
		      2, 3, 2,
		      4, 5, 4,
		      6 };
  make_lut444(6, id_c2, sym_c2);

  // LUT-D0 を作る．
  ymuint id_d0[] = { 0, 1, 2, 3, 4, 5, 6, 7, 0, 4 };
  ymuint sym_d0[] = { 1, 2, 3, 1,
		      5, 6, 7, 5,
		      0, 4, 0,
		      1, 5, 1,
		      8 };
  make_lut444(8, id_d0, sym_d0);

  // LUT-D1 を作る．
  ymuint id_d1[] = { 0, 1, 2, 3, 4, 1, 5, 6, 0, 4 };
  ymuint sym_d1[] = { 2, 3, 2,
		      5, 6, 5,
		      0, 4, 0,
		      2, 5, 2,
		      7 };
  make_lut444(7, id_d1, sym_d1);

  // LUT-D2 を作る．
  ymuint id_d2[] = { 0, 1, 2, 3, 4, 1, 2, 5, 0, 4 };
  ymuint sym_d2[] = { 1, 2, 1,
		      0, 4, 0,
		      3, 5, 3,
		      6 };
  make_lut444(6, id_d2, sym_d2);

  // LUT-E0 を作る．
  ymuint id_e0[] = { 0, 1, 2, 3, 0, 4, 5, 6, 0, 7 };
  ymuint sym_e0[] = { 1, 2, 3, 1,
		      4, 5, 6, 4,
		      1, 4, 1,
		      8 };
  make_lut444(8, id_e0, sym_e0);

  // LUT-E1 を作る．
  ymuint id_e1[] = { 0, 1, 2, 3, 0, 1, 4, 5, 0, 6 };
  ymuint sym_e1[] = { 2, 3, 2,
		      4, 5, 4,
		      2, 4, 2,
		      7 };
  make_lut444(7, id_e1, sym_e1);

  // LUT-E2 を作る．
  ymuint id_e2[] = { 0, 1, 2, 3, 0, 1, 2, 4, 0, 5 };
  ymuint sym_e2[] = { 1, 2, 1,
		      3, 4, 3,
		      6 };
  make_lut444(6, id_e2, sym_e2);

  // LUT-F0 を作る．
  ymuint id_f0[] = { 0, 1, 2, 3, 0, 4, 5, 6, 0, 1 };
  ymuint sym_f0[] = { 2, 3, 2,
		      4, 5, 6, 4,
		      7 };
  make_lut444(7, id_f0, sym_f0);

  // LUT-F1 を作る．
  ymuint id_f1[] = { 0, 1, 2, 3, 0, 2, 4, 5, 0, 1 };
  ymuint sym_f1[] = { 4, 5, 4,
		      6 };
  make_lut444(6, id_f1, sym_f1);

  // LUT-G0 を作る．
  ymuint id_g0[] = { 0, 1, 2, 3, 0, 1, 4, 5, 0, 1 };
  ymuint sym_g0[] = { 0, 1, 0,
		      2, 3, 2,
		      4, 5, 4,
		      2, 4, 2,
		      6 };
  make_lut444(6, id_g0, sym_g0);

  reset_count();
}

// @brief デストラクタ
Lut444Match::~Lut444Match()
{
}

// @brief 与えられた関数を LUT ネットワークで実現できるか調べる．
// @param[in] func 対象の関数
// @param[in] solver GBM ソルバ
bool
Lut444Match::match(const TvFunc& func,
		   GbmSolver& solver)
{
  StopWatch timer;
  timer.start();
  bool stat = mMatcher.match(func, solver);
  timer.stop();
  if ( stat ) {
    mOKTime += timer.time();
  }
  else {
    mNGTime += timer.time();
  }
  return stat;
}

// @brief カウンタをリセットする．
void
Lut444Match::reset_count()
{
  mMatcher.reset_count();
  mOKTime.set(0.0, 0.0, 0.0);
  mNGTime.set(0.0, 0.0, 0.0);
}

// @brief カウンタの値を得る．
void
Lut444Match::get_count(ymuint& trivial_num,
		       ymuint& a0_num,
		       ymuint& a0_loop,
		       ymuint& a1_num,
		       ymuint& a1_loop,
		       ymuint& a2_num,
		       ymuint& a2_loop,
		       ymuint& a3_num,
		       ymuint& a3_loop,
		       ymuint& a4_num,
		       ymuint& a4_loop,
		       ymuint& b0_num,
		       ymuint& b0_loop,
		       ymuint& b1_num,
		       ymuint& b1_loop,
		       ymuint& b2_num,
		       ymuint& b2_loop,
		       ymuint& b3_num,
		       ymuint& b3_loop,
		       ymuint& c0_num,
		       ymuint& c0_loop,
		       ymuint& c1_num,
		       ymuint& c1_loop,
		       ymuint& c2_num,
		       ymuint& c2_loop,
		       ymuint& d0_num,
		       ymuint& d0_loop,
		       ymuint& d1_num,
		       ymuint& d1_loop,
		       ymuint& d2_num,
		       ymuint& d2_loop,
		       ymuint& e0_num,
		       ymuint& e0_loop,
		       ymuint& e1_num,
		       ymuint& e1_loop,
		       ymuint& e2_num,
		       ymuint& e2_loop,
		       ymuint& f0_num,
		       ymuint& f0_loop,
		       ymuint& f1_num,
		       ymuint& f1_loop,
		       ymuint& g0_num,
		       ymuint& g0_loop,
		       ymuint& fail_num,
		       USTime& ok_time,
		       USTime& ng_time)
{
  mMatcher.get_count(trivial_num, fail_num);
  a0_num = mMatcher.get_templ_count(0);
  a0_loop = mMatcher.get_templ_loop_count(0);
  a1_num = mMatcher.get_templ_count(1);
  a1_loop = mMatcher.get_templ_loop_count(1);
  a2_num = mMatcher.get_templ_count(2);
  a2_loop = mMatcher.get_templ_loop_count(2);
  a3_num = mMatcher.get_templ_count(3);
  a3_loop = mMatcher.get_templ_loop_count(3);
  a4_num = mMatcher.get_templ_count(4);
  a4_loop = mMatcher.get_templ_loop_count(4);
  b0_num = mMatcher.get_templ_count(5);
  b0_loop = mMatcher.get_templ_loop_count(5);
  b1_num = mMatcher.get_templ_count(6);
  b1_loop = mMatcher.get_templ_loop_count(6);
  b2_num = mMatcher.get_templ_count(7);
  b2_loop = mMatcher.get_templ_loop_count(7);
  b3_num = mMatcher.get_templ_count(8);
  b3_loop = mMatcher.get_templ_loop_count(8);
  c0_num = mMatcher.get_templ_count(9);
  c0_loop = mMatcher.get_templ_loop_count(9);
  c1_num = mMatcher.get_templ_count(10);
  c1_loop = mMatcher.get_templ_loop_count(10);
  c2_num = mMatcher.get_templ_count(11);
  c2_loop = mMatcher.get_templ_loop_count(11);
  d0_num = mMatcher.get_templ_count(12);
  d0_loop = mMatcher.get_templ_loop_count(12);
  d1_num = mMatcher.get_templ_count(13);
  d1_loop = mMatcher.get_templ_loop_count(13);
  d2_num = mMatcher.get_templ_count(14);
  d2_loop = mMatcher.get_templ_loop_count(14);
  e0_num = mMatcher.get_templ_count(15);
  e0_loop = mMatcher.get_templ_loop_count(15);
  e1_num = mMatcher.get_templ_count(16);
  e1_loop = mMatcher.get_templ_loop_count(16);
  e2_num = mMatcher.get_templ_count(17);
  e2_loop = mMatcher.get_templ_loop_count(17);
  f0_num = mMatcher.get_templ_count(18);
  f0_loop = mMatcher.get_templ_loop_count(18);
  f1_num = mMatcher.get_templ_count(19);
  f1_loop = mMatcher.get_templ_loop_count(19);
  g0_num = mMatcher.get_templ_count(20);
  g0_loop = mMatcher.get_templ_loop_count(20);
  ok_time = mOKTime;
  ng_time = mNGTime;
}

void
Lut444Match::make_lut444(ymuint input_num,
			 ymuint iorder[],
			 ymuint sym_list[])
{
  RcfNetwork* network = new RcfNetwork;
  vector<RcfNodeHandle> input_list(input_num);
  for (ymuint i = 0; i < input_num; ++ i) {
    input_list[i] = network->new_input();
  }

  // LUT1 を作る．
  vector<RcfNodeHandle> tmp_inputs1(4);
  for (ymuint i = 0; i < 4; ++ i) {
    tmp_inputs1[i] = input_list[iorder[i]];
  }
  RcfNodeHandle lut1 = network->new_lut(tmp_inputs1);

  // LUT2 を作る．
  for (ymuint i = 0; i < 4; ++ i) {
    tmp_inputs1[i] = input_list[iorder[i + 4]];
  }
  RcfNodeHandle lut2 = network->new_lut(tmp_inputs1);

  // LUT3 を作る．
  tmp_inputs1[0] = lut1;
  tmp_inputs1[1] = lut2;
  tmp_inputs1[2] = input_list[iorder[8]];
  tmp_inputs1[3] = input_list[iorder[9]];
  RcfNodeHandle lut3 = network->new_lut(tmp_inputs1);

  network->set_output(lut3);

  for (ymuint i = 0; ; ++ i) {
     ymuint pos0 = sym_list[i];
    if ( pos0 == input_num ) {
      break;
    }
    ymuint pred = pos0;
    for (++ i; ; ++ i) {
      ymuint pos = sym_list[i];
      if ( pos == pos0 ) {
	break;
      }
      network->set_input_pred(pos, pred);
      pred = pos;
    }
  }

  mMatcher.add_templ(input_num, network);
}

END_NAMESPACE_YM
