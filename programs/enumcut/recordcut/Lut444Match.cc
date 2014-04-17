
/// @file Lut444Match.cc
/// @brief Lut444Match の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "Lut444Match.h"


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
  ymuint pred_a0[] = { 1, 0,
		       2, 1,
		       3, 2,
		       4, 0,
		       5, 4,
		       6, 5,
		       7, 6,
		       9, 8,
		       10, 10 };
  make_lut444(10, id_a0, pred_a0);

  // LUT-A1 を作る．
  ymuint id_a1[] = { 0, 1, 2, 3, 0, 4, 5, 6, 7, 8 };
  ymuint pred_a1[] = { 2, 1,
		       3, 2,
		       4, 1,
		       5, 4,
		       6, 5,
		       8, 7,
		       9, 9 };
  make_lut444(9, id_a1, pred_a1);

  // LUT-A2 を作る．
  ymuint id_a2[] = { 0, 1, 2, 3, 0, 1, 4, 5, 6, 7 };
  ymuint pred_a2[] = { 1, 0,
		       3, 2,
		       4, 2,
		       5, 4,
		       7, 6,
		       8, 8 };
  make_lut444(8, id_a2, pred_a2);

  // LUT-A3 を作る．
  ymuint id_a3[] = { 0, 1, 2, 3, 0, 1, 2, 4, 5, 6 };
  ymuint pred_a3[] = { 1, 0,
		       2, 1,
		       4, 3,
		       6, 5,
		       7, 7 };
  make_lut444(7, id_a3, pred_a3);

  // LUT-A4 を作る．
  ymuint id_a4[] = { 0, 1, 2, 3, 0, 1, 2, 3, 4, 5 };
  ymuint pred_a4[] = { 1, 0,
		       2, 1,
		       3, 2,
		       5, 4,
		       6, 6 };
  make_lut444(6, id_a4, pred_a4);

  // LUT-B0 を作る．
  ymuint id_b0[] = { 0, 1, 2, 3, 4, 5, 6, 7, 0, 8 };
  ymuint pred_b0[] = { 2, 1,
		       3, 2,
		       5, 4,
		       6, 5,
		       7, 6,
		       9, 9 };
  make_lut444(9, id_b0, pred_b0);

  // LUT-B1 を作る．
  ymuint id_b1[] = { 0, 1, 2, 3, 1, 4, 5, 6, 0, 7 };
  ymuint pred_b1[] = { 3, 2,
		       5, 4,
		       6, 5,
		       8, 8 };
  make_lut444(8, id_b1, pred_b1);

  // LUT-B2 を作る．
  ymuint id_b2[] = { 0, 1, 2, 3, 1, 2, 4, 5, 0, 6 };
  ymuint pred_b2[] = { 2, 1,
		       5, 4,
		       7, 7 };
  make_lut444(7, id_b2, pred_b2);

  // LUT-B3 を作る．
  ymuint id_b3[] = { 0, 1, 2, 3, 1, 2, 3, 4, 0, 5 };
  ymuint pred_b3[] = { 2, 1,
		       3, 2,
		       6, 6 };
  make_lut444(6, id_b3, pred_b3);

  // LUT-C0 を作る．
  ymuint id_c0[] = { 0, 1, 2, 3, 4, 5, 6, 7, 0, 1 };
  ymuint pred_c0[] = { 1, 0,
		       3, 2,
		       5, 4,
		       6, 5,
		       7, 6,
		       8, 8 };
  make_lut444(8, id_c0, pred_c0);

  // LUT-C1 を作る．
  ymuint id_c1[] = { 0, 1, 2, 3, 2, 4, 5, 6, 0, 1 };
  ymuint pred_c1[] = { 1, 0,
			5, 4,
			6, 5,
			7, 7 };
  make_lut444(7, id_c1, pred_c1);

  // LUT-C2 を作る．
  ymuint id_c2[] = { 0, 1, 2, 3, 2, 3, 4, 5, 0, 1 };
  ymuint pred_c2[] = { 1, 0,
		       3, 2,
		       5, 4,
		       6, 6 };
  make_lut444(6, id_c2, pred_c2);

  // LUT-D0 を作る．
  ymuint id_d0[] = { 0, 1, 2, 3, 4, 5, 6, 7, 0, 4 };
  ymuint pred_d0[] = { 2, 1,
		       3, 2,
		       4, 0,
		       6, 5,
		       7, 6,
		       8, 8 };
  make_lut444(8, id_d0, pred_d0);

  // LUT-D1 を作る．
  ymuint id_d1[] = { 0, 1, 2, 3, 4, 1, 5, 6, 0, 4 };
  ymuint pred_d1[] = { 3, 2,
		       4, 0,
		       6, 5,
		       7, 7 };
  make_lut444(7, id_d1, pred_d1);

  // LUT-D2 を作る．
  ymuint id_d2[] = { 0, 1, 2, 3, 4, 1, 2, 5, 0, 4 };
  ymuint pred_d2[] = { 2, 1,
		       4, 0,
		       6, 6 };
  make_lut444(6, id_d2, pred_d2);

  // LUT-E0 を作る．
  ymuint id_e0[] = { 0, 1, 2, 3, 0, 4, 5, 6, 0, 7 };
  ymuint pred_e0[] = { 2, 1,
		       3, 2,
		       4, 1,
		       5, 4,
		       6, 5,
		       8, 8 };
  make_lut444(8, id_e0, pred_e0);

  // LUT-E1 を作る．
  ymuint id_e1[] = { 0, 1, 2, 3, 0, 1, 4, 5, 0, 6 };
  ymuint pred_e1[] = { 3, 2,
		       4, 2,
		       5, 4,
		       7, 7 };
  make_lut444(7, id_e1, pred_e1);

  // LUT-E2 を作る．
  ymuint id_e2[] = { 0, 1, 2, 3, 0, 1, 2, 4, 0, 5 };
  ymuint pred_e2[] = { 2, 1,
		       4, 3,
		       6, 6 };
  make_lut444(6, id_e2, pred_e2);

  // LUT-F0 を作る．
  ymuint id_f0[] = { 0, 1, 2, 3, 0, 4, 5, 6, 0, 1 };
  ymuint pred_f0[] = { 3, 2,
		       5, 4,
		       6, 5,
		       7, 7 };
  make_lut444(7, id_f0, pred_f0);

  // LUT-F1 を作る．
  ymuint id_f1[] = { 0, 1, 2, 3, 0, 2, 4, 5, 0, 1 };
  ymuint pred_f1[] = { 5, 4,
		       6, 6 };
  make_lut444(6, id_f1, pred_f1);

  // LUT-G0 を作る．
  ymuint id_g0[] = { 0, 1, 2, 3, 0, 1, 4, 5, 0, 1 };
  ymuint pred_g0[] = { 1, 0,
		       3, 2,
		       4, 2,
		       5, 4,
		       6, 6 };
  make_lut444(6, id_g0, pred_g0);

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
  return mMatcher.match(func, solver);
}

// @brief カウンタをリセットする．
void
Lut444Match::reset_count()
{
  mMatcher.reset_count();
}

// @brief カウンタの値を得る．
void
Lut444Match::get_count(ymuint& trivial_num,
		       ymuint& a0_num,
		       ymuint& a1_num,
		       ymuint& a2_num,
		       ymuint& a3_num,
		       ymuint& a4_num,
		       ymuint& b0_num,
		       ymuint& b1_num,
		       ymuint& b2_num,
		       ymuint& b3_num,
		       ymuint& c0_num,
		       ymuint& c1_num,
		       ymuint& c2_num,
		       ymuint& d0_num,
		       ymuint& d1_num,
		       ymuint& d2_num,
		       ymuint& e0_num,
		       ymuint& e1_num,
		       ymuint& e2_num,
		       ymuint& f0_num,
		       ymuint& f1_num,
		       ymuint& g0_num,
		       ymuint& fail_num)
{
  mMatcher.get_count(trivial_num, fail_num);
  a0_num = mMatcher.get_templ_count(0);
  a1_num = mMatcher.get_templ_count(1);
  a2_num = mMatcher.get_templ_count(2);
  a3_num = mMatcher.get_templ_count(3);
  a4_num = mMatcher.get_templ_count(4);
  b0_num = mMatcher.get_templ_count(5);
  b1_num = mMatcher.get_templ_count(6);
  b2_num = mMatcher.get_templ_count(7);
  b3_num = mMatcher.get_templ_count(8);
  c0_num = mMatcher.get_templ_count(9);
  c1_num = mMatcher.get_templ_count(10);
  c2_num = mMatcher.get_templ_count(11);
  d0_num = mMatcher.get_templ_count(12);
  d1_num = mMatcher.get_templ_count(13);
  d2_num = mMatcher.get_templ_count(14);
  e0_num = mMatcher.get_templ_count(15);
  e1_num = mMatcher.get_templ_count(16);
  e2_num = mMatcher.get_templ_count(17);
  f0_num = mMatcher.get_templ_count(18);
  f1_num = mMatcher.get_templ_count(19);
  g0_num = mMatcher.get_templ_count(20);
}

void
Lut444Match::make_lut444(ymuint input_num,
			 ymuint iorder[],
			 ymuint pred_list[])
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

  for (ymuint i = 0; ; i += 2) {
    ymuint pos = pred_list[i];
    if ( pos == input_num ) {
      break;
    }
    ymuint pred = pred_list[i + 1];
    network->set_input_pred(pos, pred);
  }

  mMatcher.add_templ(input_num, network);
}

END_NAMESPACE_YM
