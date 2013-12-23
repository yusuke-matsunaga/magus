
/// @file Lut443Match.cc
/// @brief Lut443Match の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "Lut443Match.h"
#include "GbmNaive.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス Lut443Match
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Lut443Match::Lut443Match()
{
  // 入力ノードを作る．
  vector<GbmNodeHandle> input_list(9);
  for (ymuint i = 0; i < 9; ++ i) {
    GbmNodeHandle h = mMgr.new_input();
    input_list[i] = h;
  }

  // LUT-L を作る．
  vector<GbmNodeHandle> tmp_inputs1(4);
  for (ymuint i = 0; i < 4; ++ i) {
    tmp_inputs1[i] = input_list[i];
  }
  GbmNodeHandle lut_l = mMgr.new_lut(tmp_inputs1);

  // LUT-R0 を作る．
  for (ymuint i = 0; i < 4; ++ i) {
    tmp_inputs1[i] = input_list[i + 4];
  }
  GbmNodeHandle lut_r0 = mMgr.new_lut(tmp_inputs1);

  // LUT-R1 を作る．
  for (ymuint i = 0; i < 1; ++ i) {
    tmp_inputs1[i] = input_list[i];
  }
  for (ymuint i = 1; i < 4; ++ i) {
    tmp_inputs1[i] = input_list[i + 4];
  }
  GbmNodeHandle lut_r1 = mMgr.new_lut(tmp_inputs1);

  // LUT-R2 を作る．
  for (ymuint i = 0; i < 2; ++ i) {
    tmp_inputs1[i] = input_list[i];
  }
  for (ymuint i = 2; i < 4; ++ i) {
    tmp_inputs1[i] = input_list[i + 4];
  }
  GbmNodeHandle lut_r2 = mMgr.new_lut(tmp_inputs1);

  // LUT-R3 を作る．
  for (ymuint i = 0; i < 3; ++ i) {
    tmp_inputs1[i] = input_list[i];
  }
  for (ymuint i = 3; i < 4; ++ i) {
    tmp_inputs1[i] = input_list[i + 4];
  }
  GbmNodeHandle lut_r3 = mMgr.new_lut(tmp_inputs1);

  // LUT-A0 を作る．
  vector<GbmNodeHandle> tmp_inputs2(3);
  tmp_inputs2[0] = lut_l;
  tmp_inputs2[1] = lut_r0;
  tmp_inputs2[2] = input_list[8];
  mA0Root = mMgr.new_lut(tmp_inputs2);

  // LUT-A1 を作る．
  tmp_inputs2[0] = lut_l;
  tmp_inputs2[1] = lut_r1;
  tmp_inputs2[2] = input_list[7];
  mA1Root = mMgr.new_lut(tmp_inputs2);

  // LUT-A2 を作る．
  tmp_inputs2[0] = lut_l;
  tmp_inputs2[1] = lut_r2;
  tmp_inputs2[2] = input_list[6];
  mA2Root = mMgr.new_lut(tmp_inputs2);

  // LUT-A3 を作る．
  tmp_inputs2[0] = lut_l;
  tmp_inputs2[1] = lut_r3;
  tmp_inputs2[2] = input_list[5];
  mA3Root = mMgr.new_lut(tmp_inputs2);

  // LUT-B0 を作る．
  tmp_inputs2[0] = lut_l;
  tmp_inputs2[1] = lut_r0;
  tmp_inputs2[2] = input_list[0];
  mB0Root = mMgr.new_lut(tmp_inputs2);

  // LUT-B1 を作る．
  tmp_inputs2[0] = lut_l;
  tmp_inputs2[1] = lut_r1;
  tmp_inputs2[2] = input_list[1];
  mB1Root = mMgr.new_lut(tmp_inputs2);

  // LUT-B2 を作る．
  tmp_inputs2[0] = lut_l;
  tmp_inputs2[1] = lut_r2;
  tmp_inputs2[2] = input_list[2];
  mB2Root = mMgr.new_lut(tmp_inputs2);

  // LUT-C1 を作る．
  tmp_inputs2[0] = lut_l;
  tmp_inputs2[1] = lut_r1;
  tmp_inputs2[2] = input_list[0];
  mC1Root = mMgr.new_lut(tmp_inputs2);

  // LUT-C2 を作る．
  tmp_inputs2[0] = lut_l;
  tmp_inputs2[1] = lut_r2;
  tmp_inputs2[2] = input_list[0];
  mC2Root = mMgr.new_lut(tmp_inputs2);

}

// @brief デストラクタ
Lut443Match::~Lut443Match()
{
}

// @brief 与えられた関数を LUT ネットワークで実現できるか調べる．
// @param[in] func 対象の関数
bool
Lut443Match::match(const TvFunc& func)
{
  cout << "Lut443Match::match(" << func << ")" << endl;

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
  cout << " ni1 = " << ni1 << endl;
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
    return true;
  }

  GbmNaive solver;

  vector<bool> conf_bits;
  vector<ymuint> iorder;

  if ( ni1 == 6 ) {
    bool stat = solver.solve(mMgr, mA3Root, func1, conf_bits, iorder);
    if ( stat ) {
      cout << "Type A-3 Match" << endl;
      return true;
    }

    stat = solver.solve(mMgr, mB2Root, func1, conf_bits, iorder);
    if ( stat ) {
      cout << "Type B-2 Match" << endl;
      return true;
    }

    stat = solver.solve(mMgr, mC2Root, func1, conf_bits, iorder);
    if ( stat ) {
      cout << "Type C-2 Match" << endl;
      return true;
    }
  }

  if ( ni1 == 7 ) {
    bool stat = solver.solve(mMgr, mA2Root, func1, conf_bits, iorder);
    if ( stat ) {
      cout << "Type A-2 Match" << endl;
      return true;
    }

    stat = solver.solve(mMgr, mB1Root, func1, conf_bits, iorder);
    if ( stat ) {
      cout << "Type B-1 Match" << endl;
      return true;
    }

    stat = solver.solve(mMgr, mC1Root, func1, conf_bits, iorder);
    if ( stat ) {
      cout << "Type C-1 Match" << endl;
      return true;
    }
  }

  if ( ni1 == 8 ) {
    bool stat = solver.solve(mMgr, mA1Root, func1, conf_bits, iorder);
    if ( stat ) {
      cout << "Type A-1 Match" << endl;
      return true;
    }

    stat = solver.solve(mMgr, mB0Root, func1, conf_bits, iorder);
    if ( stat ) {
      cout << "Type B-0 Match" << endl;
      return true;
    }
  }

  if ( ni1 == 9 ) {
    cout << "Trying to match against mA0Root" << endl;
#if 0
    bool stat = solver.solve(mMgr, mA0Root, func1, conf_bits, iorder);
#else
    GbmMgr mgr;

    vector<GbmNodeHandle> inputs(9);
    for (ymuint i = 0; i < 9; ++ i) {
      inputs[i] = mgr.new_input();
    }

    vector<GbmNodeHandle> fanin1(4);
    for (ymuint i = 0; i < 4; ++ i) {
      fanin1[i] = inputs[i];
    }
    GbmNodeHandle lut1 = mgr.new_lut(fanin1);

    for (ymuint i = 0; i < 4; ++ i) {
      fanin1[i] = inputs[i + 4];
    }
    GbmNodeHandle lut2 = mgr.new_lut(fanin1);

    vector<GbmNodeHandle> fanin2(3);
    fanin2[0] = lut1;
    fanin2[1] = lut2;
    fanin2[2] = inputs[8];
    GbmNodeHandle lut3 = mgr.new_lut(fanin2);

    bool stat = solver.solve(mgr, lut3, func1, conf_bits, iorder);
#endif
    if ( stat ) {
      cout << "Type A-0 Match" << endl;
      return true;
    }
  }

  cout << "Not found" << endl;
  return false;
}

END_NAMESPACE_YM
