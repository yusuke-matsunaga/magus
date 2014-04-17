
/// @file Lut443Match.cc
/// @brief Lut443Match の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "Lut443Match.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

bool debug = false;

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス Lut443Match
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Lut443Match::Lut443Match()
{
  mMatcher.set_input_range(6, 9);

  // LUT-A0 を作る．
  ymuint id_a0[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
  ymuint pred_a0[] = { 1, 0,
		       2, 1,
		       3, 2,
		       4, 0,
		       5, 4,
		       6, 5,
		       7, 6,
		       9, 9 };
  make_lut443(9, id_a0, pred_a0);

  // LUT-A1 を作る．
  ymuint id_a1[] = { 0, 1, 2, 3, 0, 4, 5, 6, 7 };
  ymuint pred_a1[] = { 2, 1,
		       3, 2,
		       4, 1,
		       5, 4,
		       6, 5,
		       8, 8 };
  make_lut443(8, id_a1, pred_a1);

  // LUT-A2 を作る．
  ymuint id_a2[] = { 0, 1, 2, 3, 0, 1, 4, 5, 6 };
  ymuint pred_a2[] = { 1, 0,
		       3, 2,
		       4, 2,
		       5, 4,
                       7, 7 };
  make_lut443(7, id_a2, pred_a2);

  // LUT-A3 を作る．
  ymuint id_a3[] = { 0, 1, 2, 3, 0, 1, 2, 4, 5 };
  ymuint pred_a3[] = { 1, 0,
		       2, 1,
		       4, 3,
		       6, 6 };
  make_lut443(6, id_a3, pred_a3);

  // LUT-B0 を作る．
  ymuint id_b0[] = { 0, 1, 2, 3, 4, 5, 6, 7, 0 };
  ymuint pred_b0[] = { 2, 1,
		       3, 2,
		       5, 4,
		       6, 5,
		       7, 6,
		       8, 8 };
  make_lut443(8, id_b0, pred_b0);

  // LUT-B1 を作る．
  ymuint id_b1[] = { 0, 1, 2, 3, 1, 4, 5, 6, 0 };
  ymuint pred_b1[] = { 3, 2,
		       5, 4,
		       6, 5,
		       7, 7 };
  make_lut443(7, id_b1, pred_b1);

  // LUT-B2 を作る．
  ymuint id_b2[] = { 0, 1, 2, 3, 1, 2, 4, 5, 0 };
  ymuint pred_b2[] = { 2, 1,
		       5, 4,
		       6, 6 };
  make_lut443(6, id_b2, pred_b2);

  // LUT-C0 を作る．
  ymuint id_c0[] = { 0, 1, 2, 3, 0, 4, 5, 6, 0 };
  ymuint pred_c0[] = { 2, 1,
		       3, 2,
		       4, 1,
		       5, 4,
		       6, 5,
		       7, 7 };
  make_lut443(7, id_c0, pred_c0);

  // LUT-C1 を作る．
  ymuint id_c1[] = { 0, 1, 2, 3, 0, 1, 4, 5, 0 };
  ymuint pred_c1[] = { 3, 2,
		       4, 2,
		       5, 4,
		       6, 6 };
  make_lut443(6, id_c1, pred_c1);

  reset_count();
}

// @brief デストラクタ
Lut443Match::~Lut443Match()
{
}

// @brief 与えられた関数を LUT ネットワークで実現できるか調べる．
// @param[in] func 対象の関数
// @param[in] solver GBM ソルバ
bool
Lut443Match::match(const TvFunc& func,
		   GbmSolver& solver)
{
  return mMatcher.match(func, solver);
}

// @brief カウンタをリセットする．
void
Lut443Match::reset_count()
{
  mMatcher.reset_count();
}

// @brief カウンタの値を得る．
void
Lut443Match::get_count(ymuint& trivial_num,
		       ymuint& a0_num,
		       ymuint& a1_num,
		       ymuint& a2_num,
		       ymuint& a3_num,
		       ymuint& b0_num,
		       ymuint& b1_num,
		       ymuint& b2_num,
		       ymuint& c0_num,
		       ymuint& c1_num,
		       ymuint& fail_num)
{
  mMatcher.get_count(trivial_num, fail_num);
  a0_num = mMatcher.get_templ_count(0);
  a1_num = mMatcher.get_templ_count(1);
  a2_num = mMatcher.get_templ_count(2);
  a3_num = mMatcher.get_templ_count(3);
  b0_num = mMatcher.get_templ_count(4);
  b1_num = mMatcher.get_templ_count(5);
  b2_num = mMatcher.get_templ_count(6);
  c0_num = mMatcher.get_templ_count(7);
  c1_num = mMatcher.get_templ_count(8);
}

void
Lut443Match::make_lut443(ymuint input_num,
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
  vector<RcfNodeHandle> tmp_inputs2(3);
  tmp_inputs2[0] = lut1;
  tmp_inputs2[1] = lut2;
  tmp_inputs2[2] = input_list[iorder[8]];
  RcfNodeHandle lut3 = network->new_lut(tmp_inputs2);

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
