
/// @file Lut443Match.cc
/// @brief Lut443Match の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "Lut443Match.h"
#include "GbmNaive.h"
#include "GbmNaiveOneHot.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

bool debug = false;

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス Lut443Match
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

void
make_lut443(RcfNetwork& network,
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
  vector<RcfNodeHandle> tmp_inputs2(3);
  tmp_inputs2[0] = lut1;
  tmp_inputs2[1] = lut2;
  tmp_inputs2[2] = input_list[id[8]];
  RcfNodeHandle lut3 = network.new_lut(tmp_inputs2);

  network.set_output(lut3);
}

END_NONAMESPACE

// @brief コンストラクタ
Lut443Match::Lut443Match()
{
  // LUT-A0 を作る．
  ymuint id_a0[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
  make_lut443(mA0, 9, id_a0);
  mA0.set_input_pred(1, 0);
  mA0.set_input_pred(2, 1);
  mA0.set_input_pred(3, 2);
  mA0.set_input_pred(4, 0);
  mA0.set_input_pred(5, 4);
  mA0.set_input_pred(6, 5);
  mA0.set_input_pred(7, 6);

  // LUT-A1 を作る．
  ymuint id_a1[] = { 0, 1, 2, 3, 0, 4, 5, 6, 7 };
  make_lut443(mA1, 8, id_a1);
  mA1.set_input_pred(2, 1);
  mA1.set_input_pred(3, 2);
  mA1.set_input_pred(4, 1);
  mA1.set_input_pred(5, 4);
  mA1.set_input_pred(6, 5);

  // LUT-A2 を作る．
  ymuint id_a2[] = { 0, 1, 2, 3, 0, 1, 4, 5, 6 };
  make_lut443(mA2, 7, id_a2);
  mA2.set_input_pred(1, 0);
  mA2.set_input_pred(3, 2);
  mA2.set_input_pred(4, 2);
  mA2.set_input_pred(5, 4);

  // LUT-A3 を作る．
  ymuint id_a3[] = { 0, 1, 2, 3, 0, 1, 2, 4, 5 };
  make_lut443(mA3, 6, id_a3);
  mA3.set_input_pred(1, 0);
  mA3.set_input_pred(2, 1);
  mA3.set_input_pred(4, 3);

  // LUT-B0 を作る．
  ymuint id_b0[] = { 0, 1, 2, 3, 4, 5, 6, 7, 0 };
  make_lut443(mB0, 8, id_b0);
  mB0.set_input_pred(2, 1);
  mB0.set_input_pred(3, 2);
  mB0.set_input_pred(5, 4);
  mB0.set_input_pred(6, 5);
  mB0.set_input_pred(7, 6);

  // LUT-B1 を作る．
  ymuint id_b1[] = { 0, 1, 2, 3, 1, 4, 5, 6, 0 };
  make_lut443(mB1, 7, id_b1);
  mB1.set_input_pred(3, 2);
  mB1.set_input_pred(5, 4);
  mB1.set_input_pred(6, 5);

  // LUT-B2 を作る．
  ymuint id_b2[] = { 0, 1, 2, 3, 1, 2, 4, 5, 0 };
  make_lut443(mB2, 6, id_b2);
  mB2.set_input_pred(2, 1);
  mB2.set_input_pred(5, 4);

  // LUT-C0 を作る．
  ymuint id_c0[] = { 0, 1, 2, 3, 0, 4, 5, 6, 0 };
  make_lut443(mC0, 7, id_c0);
  mC0.set_input_pred(2, 1);
  mC0.set_input_pred(3, 2);
  mC0.set_input_pred(4, 1);
  mC0.set_input_pred(5, 4);
  mC0.set_input_pred(6, 5);

  // LUT-C1 を作る．
  ymuint id_c1[] = { 0, 1, 2, 3, 0, 1, 4, 5, 0 };
  make_lut443(mC1, 7, id_c1);
  mC1.set_input_pred(3, 2);
  mC1.set_input_pred(4, 2);
  mC1.set_input_pred(5, 4);

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
    bool stat = solver.solve(mA3, func1, conf_bits, iorder);
    if ( stat ) {
      if ( debug ) {
	cout << "Type A-3 Match" << endl;
	cout << "  iorder = ";
	for (ymuint i = 0; i < ni1; ++ i) {
	  cout << " " << iorder[i];
	}
	cout << endl;
      }
      ++ mA3Num;
      return true;
    }

    stat = solver.solve(mB2, func1, conf_bits, iorder);
    if ( stat ) {
      if ( debug ) {
	cout << "Type B-2 Match" << endl;
	cout << "  iorder = ";
	for (ymuint i = 0; i < ni1; ++ i) {
	  cout << " " << iorder[i];
	}
	cout << endl;
      }
      ++ mB2Num;
      return true;
    }

    stat = solver.solve(mC1, func1, conf_bits, iorder);
    if ( stat ) {
      if ( debug ) {
	cout << "Type C-1 Match" << endl;
	cout << "  iorder = ";
	for (ymuint i = 0; i < ni1; ++ i) {
	  cout << " " << iorder[i];
	}
	cout << endl;
      }
      ++ mC1Num;
      return true;
    }
  }

  if ( ni1 == 7 ) {
    bool stat = solver.solve(mA2, func1, conf_bits, iorder);
    if ( stat ) {
      if ( debug ) {
	cout << "Type A-2 Match" << endl;
	cout << "  iorder = ";
	for (ymuint i = 0; i < ni1; ++ i) {
	  cout << " " << iorder[i];
	}
	cout << endl;
      }
      ++ mA2Num;
      return true;
    }

    stat = solver.solve(mB1, func1, conf_bits, iorder);
    if ( stat ) {
      if ( debug ) {
	cout << "Type B-1 Match" << endl;
	cout << "  iorder = ";
	for (ymuint i = 0; i < ni1; ++ i) {
	  cout << " " << iorder[i];
	}
	cout << endl;
      }
      ++ mB1Num;
      return true;
    }

    stat = solver.solve(mC0, func1, conf_bits, iorder);
    if ( stat ) {
      if ( debug ) {
	cout << "Type C-0 Match" << endl;
	cout << "  iorder = ";
	for (ymuint i = 0; i < ni1; ++ i) {
	  cout << " " << iorder[i];
	}
	cout << endl;
      }
      ++ mC0Num;
      return true;
    }
  }

  if ( ni1 == 8 ) {
    bool stat = solver.solve(mA1, func1, conf_bits, iorder);
    if ( stat ) {
      if ( debug ) {
	cout << "Type A-1 Match" << endl;
	cout << "  iorder = ";
	for (ymuint i = 0; i < ni1; ++ i) {
	  cout << " " << iorder[i];
	}
	cout << endl;
      }
      ++ mA1Num;
      return true;
    }

    stat = solver.solve(mB0, func1, conf_bits, iorder);
    if ( stat ) {
      if ( debug ) {
	cout << "Type B-0 Match" << endl;
	cout << "  iorder = ";
	for (ymuint i = 0; i < ni1; ++ i) {
	  cout << " " << iorder[i];
	}
	cout << endl;
      }
      ++ mB0Num;
      return true;
    }
  }

  if ( ni1 == 9 ) {
    bool stat = solver.solve(mA0, func1, conf_bits, iorder);
    if ( stat ) {
      if ( debug ) {
	cout << "Type A-0 Match" << endl;
	cout << "  iorder = ";
	for (ymuint i = 0; i < ni1; ++ i) {
	  cout << " " << iorder[i];
	}
	cout << endl;
      }
      ++ mA0Num;
      return true;
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
Lut443Match::reset_count()
{
  mTrivialNum = 0;
  mA0Num = 0;
  mA1Num = 0;
  mA2Num = 0;
  mA3Num = 0;
  mB0Num = 0;
  mB1Num = 0;
  mB2Num = 0;
  mC0Num = 0;
  mC1Num = 0;
  mFailNum = 0;
}

// @brief カウンタの値を得る．
void
Lut443Match::get_count(ymuint& trival_num,
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
  trival_num = mTrivialNum;
  a0_num = mA0Num;
  a1_num = mA1Num;
  a2_num = mA2Num;
  a3_num = mA3Num;
  b0_num = mB0Num;
  b1_num = mB1Num;
  b2_num = mB2Num;
  c0_num = mC0Num;
  c1_num = mC1Num;
  fail_num = mFailNum;
}

END_NAMESPACE_YM
