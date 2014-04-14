
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

void
make_lut444(RcfNetwork& network,
	    ymuint ni,
	    ymuint id[])
{
  vector<RcfNodeHandle> input_list(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    input_list[i] = network.new_input();
  }

  // LUT1 を作る．
  vector<RcfNodeHandle> tmp_inputs1(4);
  for (ymuint i = 0; i < 4; ++ i) {
    tmp_inputs1[i] = input_list[id[i]];
  }
  RcfNodeHandle lut1 = network.new_lut(tmp_inputs1);

  // LUT2 を作る．
  for (ymuint i = 0; i < 4; ++ i) {
    tmp_inputs1[i] = input_list[id[i + 4]];
  }
  RcfNodeHandle lut2 = network.new_lut(tmp_inputs1);

  // LUT3 を作る．
  tmp_inputs1[0] = lut1;
  tmp_inputs1[1] = lut2;
  tmp_inputs1[2] = input_list[id[8]];
  tmp_inputs1[3] = input_list[id[9]];
  RcfNodeHandle lut3 = network.new_lut(tmp_inputs1);

  network.set_output(lut3);
}

END_NONAMESPACE

// @brief コンストラクタ
Lut444Match::Lut444Match()
{
  // LUT-A0 を作る．
  ymuint id_a0[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  make_lut444(mA0.mNetwork, 10, id_a0);
  mA0.mNetwork.set_input_pred(1, 0);
  mA0.mNetwork.set_input_pred(2, 1);
  mA0.mNetwork.set_input_pred(3, 2);
  mA0.mNetwork.set_input_pred(4, 0);
  mA0.mNetwork.set_input_pred(5, 4);
  mA0.mNetwork.set_input_pred(6, 5);
  mA0.mNetwork.set_input_pred(7, 6);
  mA0.mNetwork.set_input_pred(9, 8);

  mN10list.push_back(&mA0);

  // LUT-A1 を作る．
  ymuint id_a1[] = { 0, 1, 2, 3, 0, 4, 5, 6, 7, 8 };
  make_lut444(mA1.mNetwork, 9, id_a1);
  mA1.mNetwork.set_input_pred(2, 1);
  mA1.mNetwork.set_input_pred(3, 2);
  mA1.mNetwork.set_input_pred(4, 1);
  mA1.mNetwork.set_input_pred(5, 4);
  mA1.mNetwork.set_input_pred(6, 5);
  mA1.mNetwork.set_input_pred(8, 7);

  mN9list.push_back(&mA1);

  // LUT-A2 を作る．
  ymuint id_a2[] = { 0, 1, 2, 3, 0, 1, 4, 5, 6, 7 };
  make_lut444(mA2.mNetwork, 8, id_a2);
  mA2.mNetwork.set_input_pred(1, 0);
  mA2.mNetwork.set_input_pred(3, 2);
  mA2.mNetwork.set_input_pred(4, 2);
  mA2.mNetwork.set_input_pred(5, 4);
  mA2.mNetwork.set_input_pred(7, 6);

  mN8list.push_back(&mA2);

  // LUT-A3 を作る．
  ymuint id_a3[] = { 0, 1, 2, 3, 0, 1, 2, 4, 5, 6 };
  make_lut444(mA3.mNetwork, 7, id_a3);
  mA3.mNetwork.set_input_pred(1, 0);
  mA3.mNetwork.set_input_pred(2, 1);
  mA3.mNetwork.set_input_pred(4, 3);
  mA3.mNetwork.set_input_pred(6, 5);

  mN7list.push_back(&mA3);

  // LUT-A4 を作る．
  ymuint id_a4[] = { 0, 1, 2, 3, 0, 1, 2, 3, 4, 5 };
  make_lut444(mA4.mNetwork, 6, id_a4);
  mA4.mNetwork.set_input_pred(1, 0);
  mA4.mNetwork.set_input_pred(2, 1);
  mA4.mNetwork.set_input_pred(3, 2);
  mA4.mNetwork.set_input_pred(5, 4);

  mN6list.push_back(&mA4);

  // LUT-B0 を作る．
  ymuint id_b0[] = { 0, 1, 2, 3, 4, 5, 6, 7, 0, 8 };
  make_lut444(mB0.mNetwork, 9, id_b0);
  mB0.mNetwork.set_input_pred(2, 1);
  mB0.mNetwork.set_input_pred(3, 2);
  mB0.mNetwork.set_input_pred(5, 4);
  mB0.mNetwork.set_input_pred(6, 5);
  mB0.mNetwork.set_input_pred(7, 6);

  mN9list.push_back(&mB0);

  // LUT-B1 を作る．
  ymuint id_b1[] = { 0, 1, 2, 3, 1, 4, 5, 6, 0, 7 };
  make_lut444(mB1.mNetwork, 8, id_b1);
  mB1.mNetwork.set_input_pred(3, 2);
  mB1.mNetwork.set_input_pred(5, 4);
  mB1.mNetwork.set_input_pred(6, 5);

  mN8list.push_back(&mB1);

  // LUT-B2 を作る．
  ymuint id_b2[] = { 0, 1, 2, 3, 1, 2, 4, 5, 0, 6 };
  make_lut444(mB2.mNetwork, 7, id_b2);
  mB2.mNetwork.set_input_pred(2, 1);
  mB2.mNetwork.set_input_pred(5, 4);

  mN7list.push_back(&mB2);

  // LUT-B3 を作る．
  ymuint id_b3[] = { 0, 1, 2, 3, 1, 2, 3, 4, 0, 5 };
  make_lut444(mB3.mNetwork, 6, id_b3);
  mB2.mNetwork.set_input_pred(2, 1);
  mB2.mNetwork.set_input_pred(3, 2);

  mN6list.push_back(&mB3);

  // LUT-C0 を作る．
  ymuint id_c0[] = { 0, 1, 2, 3, 4, 5, 6, 7, 0, 1 };
  make_lut444(mC0.mNetwork, 8, id_c0);
  mC0.mNetwork.set_input_pred(1, 0);
  mC0.mNetwork.set_input_pred(3, 2);
  mC0.mNetwork.set_input_pred(5, 4);
  mC0.mNetwork.set_input_pred(6, 5);
  mC0.mNetwork.set_input_pred(7, 6);

  mN8list.push_back(&mC0);

  // LUT-C1 を作る．
  ymuint id_c1[] = { 0, 1, 2, 3, 2, 4, 5, 6, 0, 1 };
  make_lut444(mC1.mNetwork, 7, id_c1);
  mC1.mNetwork.set_input_pred(1, 0);
  mC1.mNetwork.set_input_pred(5, 4);
  mC1.mNetwork.set_input_pred(6, 5);

  mN7list.push_back(&mC1);

  // LUT-C2 を作る．
  ymuint id_c2[] = { 0, 1, 2, 3, 2, 3, 4, 5, 0, 1 };
  make_lut444(mC2.mNetwork, 6, id_c2);
  mC2.mNetwork.set_input_pred(1, 0);
  mC2.mNetwork.set_input_pred(3, 2);
  mC2.mNetwork.set_input_pred(5, 4);

  mN6list.push_back(&mC2);

  // LUT-D0 を作る．
  ymuint id_d0[] = { 0, 1, 2, 3, 4, 5, 6, 7, 0, 4 };
  make_lut444(mD0.mNetwork, 8, id_d0);
  mD0.mNetwork.set_input_pred(2, 1);
  mD0.mNetwork.set_input_pred(3, 2);
  mD0.mNetwork.set_input_pred(4, 0);
  mD0.mNetwork.set_input_pred(6, 5);
  mD0.mNetwork.set_input_pred(7, 6);

  mN8list.push_back(&mD0);

  // LUT-D1 を作る．
  ymuint id_d1[] = { 0, 1, 2, 3, 4, 1, 5, 6, 0, 4 };
  make_lut444(mD1.mNetwork, 7, id_d1);
  mD1.mNetwork.set_input_pred(3, 2);
  mD1.mNetwork.set_input_pred(4, 0);
  mD1.mNetwork.set_input_pred(6, 5);

  mN7list.push_back(&mD1);

  // LUT-D2 を作る．
  ymuint id_d2[] = { 0, 1, 2, 3, 4, 1, 2, 5, 0, 4 };
  make_lut444(mD2.mNetwork, 6, id_d2);
  mD2.mNetwork.set_input_pred(2, 1);
  mD2.mNetwork.set_input_pred(4, 0);

  mN6list.push_back(&mD2);

  // LUT-E0 を作る．
  ymuint id_e0[] = { 0, 1, 2, 3, 0, 4, 5, 6, 0, 7 };
  make_lut444(mE0.mNetwork, 8, id_e0);
  mE0.mNetwork.set_input_pred(2, 1);
  mE0.mNetwork.set_input_pred(3, 2);
  mE0.mNetwork.set_input_pred(4, 1);
  mE0.mNetwork.set_input_pred(5, 4);
  mE0.mNetwork.set_input_pred(6, 5);

  mN8list.push_back(&mE0);

  // LUT-E1 を作る．
  ymuint id_e1[] = { 0, 1, 2, 3, 0, 1, 4, 5, 0, 6 };
  make_lut444(mE1.mNetwork, 7, id_e1);
  mE1.mNetwork.set_input_pred(3, 2);
  mE1.mNetwork.set_input_pred(4, 2);
  mE1.mNetwork.set_input_pred(5, 4);

  mN7list.push_back(&mE1);

  // LUT-E2 を作る．
  ymuint id_e2[] = { 0, 1, 2, 3, 0, 1, 2, 4, 0, 5 };
  make_lut444(mE2.mNetwork, 6, id_e2);
  mE2.mNetwork.set_input_pred(2, 1);
  mE2.mNetwork.set_input_pred(4, 3);

  mN6list.push_back(&mE2);

  // LUT-F0 を作る．
  ymuint id_f0[] = { 0, 1, 2, 3, 0, 4, 5, 6, 0, 1 };
  make_lut444(mF0.mNetwork, 7, id_f0);
  mF0.mNetwork.set_input_pred(3, 2);
  mF0.mNetwork.set_input_pred(5, 4);
  mF0.mNetwork.set_input_pred(6, 5);

  mN7list.push_back(&mF0);

  // LUT-F1 を作る．
  ymuint id_f1[] = { 0, 1, 2, 3, 0, 2, 4, 5, 0, 1 };
  make_lut444(mF1.mNetwork, 6, id_f1);
  mF1.mNetwork.set_input_pred(5, 4);

  mN6list.push_back(&mF1);

  // LUT-G0 を作る．
  ymuint id_g0[] = { 0, 1, 2, 3, 0, 1, 4, 5, 0, 1 };
  make_lut444(mG0.mNetwork, 6, id_g0);
  mG0.mNetwork.set_input_pred(1, 0);
  mG0.mNetwork.set_input_pred(3, 2);
  mG0.mNetwork.set_input_pred(4, 2);
  mG0.mNetwork.set_input_pred(5, 4);

  mN6list.push_back(&mG0);

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
  ymuint ni = func.input_num();

  // まず関数の真のサポートを求める．
  vector<ymuint> sup_list;
  sup_list.reserve(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    if ( func.check_sup(VarId(i)) ) {
      sup_list.push_back(i);
    }
  }

  TvFunc func1 = func;
  ymuint ni1 = sup_list.size();
  if ( debug ) {
    cout << " ni1 = " << ni1 << endl;
  }
  if ( ni1 < ni ) {
    // 冗長な入力があった．
    ymuint nexp1 = (1U << ni1);
    vector<int> new_tv(nexp1);
    for (ymuint b = 0; b < nexp1; ++ b) {
      ymuint pos = 0;
      for (ymuint i = 0; i < ni1; ++ i) {
	if ( b & (1U << i) ) {
	  pos += (1U << sup_list[i]);
	}
      }
      new_tv[b] = func.value(pos);
    }
    func1 = TvFunc(ni1, new_tv);
  }

  if ( ni1 <= 5 ) {
    // 自明
    ++ mTrivialNum;
    return true;
  }

  vector<bool> conf_bits;
  vector<ymuint> iorder;

  if ( ni1 == 6 ) {
    for (ymuint i = 0; i < mN6list.size(); ++ i) {
      RcfInfo& info = *mN6list[i];
      bool stat = solver.solve(info.mNetwork, func1, conf_bits, iorder);
      if ( stat ) {
	if ( debug ) {
	  cout << "Type A-4 Match" << endl;
	  cout << "  iorder = ";
	  for (ymuint i = 0; i < ni1; ++ i) {
	    cout << " " << iorder[i];
	  }
	  cout << endl;
	}
	++ info.mCount;
	return true;
      }
    }
  }

  if ( ni1 == 7 ) {
    for (ymuint i = 0; i < mN7list.size(); ++ i) {
      RcfInfo& info = *mN7list[i];
      bool stat = solver.solve(info.mNetwork, func1, conf_bits, iorder);
      if ( stat ) {
	if ( debug ) {
	  cout << "Type A-2 Match" << endl;
	  cout << "  iorder = ";
	  for (ymuint i = 0; i < ni1; ++ i) {
	    cout << " " << iorder[i];
	  }
	  cout << endl;
	}
	++ info.mCount;
	return true;
      }
    }
  }

  if ( ni1 == 8 ) {
    for (ymuint i = 0; i < mN8list.size(); ++ i) {
      RcfInfo& info = *mN8list[i];
      bool stat = solver.solve(info.mNetwork, func1, conf_bits, iorder);
      if ( stat ) {
	if ( debug ) {
	  cout << "Type A-1 Match" << endl;
	  cout << "  iorder = ";
	  for (ymuint i = 0; i < ni1; ++ i) {
	    cout << " " << iorder[i];
	  }
	  cout << endl;
	}
	++ info.mCount;
	return true;
      }
    }
  }

  if ( ni1 == 9 ) {
    for (ymuint i = 0; i < mN9list.size(); ++ i) {
      RcfInfo& info = *mN9list[i];
      bool stat = solver.solve(info.mNetwork, func1, conf_bits, iorder);
      if ( stat ) {
	if ( debug ) {
	  cout << "Type A-0 Match" << endl;
	  cout << "  iorder = ";
	  for (ymuint i = 0; i < ni1; ++ i) {
	    cout << " " << iorder[i];
	  }
	  cout << endl;
	}
	++ info.mCount;
	return true;
      }
    }
  }

  if ( ni1 == 10 ) {
    for (ymuint i = 0; i < mN10list.size(); ++ i) {
      RcfInfo& info = *mN10list[i];
      bool stat = solver.solve(info.mNetwork, func1, conf_bits, iorder);
      if ( stat ) {
	if ( debug ) {
	  cout << "Type A-0 Match" << endl;
	  cout << "  iorder = ";
	  for (ymuint i = 0; i < ni1; ++ i) {
	    cout << " " << iorder[i];
	  }
	  cout << endl;
	}
	++ info.mCount;
	return true;
      }
    }
  }

  if ( debug ) {
    cout << "Not found" << endl;
  }
  ++ mFailNum;
  return false;
}

// @brief カウンタをリセットする．
void
Lut444Match::reset_count()
{
  mTrivialNum = 0;
  mFailNum = 0;
  for (ymuint i = 0; i < mAllList.size(); ++ i) {
    mAllList[i]->mCount = 0;
  }
}

// @brief カウンタの値を得る．
void
Lut444Match::get_count(ymuint& trival_num,
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
  trival_num = mTrivialNum;
  a0_num = mA0.mCount;
  a1_num = mA1.mCount;
  a2_num = mA2.mCount;
  a3_num = mA3.mCount;
  a4_num = mA4.mCount;
  b0_num = mB0.mCount;
  b1_num = mB1.mCount;
  b2_num = mB2.mCount;
  b3_num = mB3.mCount;
  c0_num = mC0.mCount;
  c1_num = mC1.mCount;
  c2_num = mC2.mCount;
  d0_num = mD0.mCount;
  d1_num = mD1.mCount;
  d2_num = mD2.mCount;
  e0_num = mE0.mCount;
  e1_num = mE1.mCount;
  e2_num = mE2.mCount;
  f0_num = mF0.mCount;
  f1_num = mF1.mCount;
  g0_num = mG0.mCount;
  fail_num = mFailNum;
}

END_NAMESPACE_YM
