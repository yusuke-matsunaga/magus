
/// @file LutMap.cc
/// @brief LutMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "LutMap.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス LutMap
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LutMap::LutMap()
{
}

// @brief デストラクタ
LutMap::~LutMap()
{
}

// @brief マッチングを行なう．
// @param[in] f マッチング対象の関数
// @param[in] solver マッチングを行なうオブジェクト
// @return 成功したら true を返す．
bool
LutMap::matching(const TvFunc& f,
		 GbmSolver& solver)
{
  ymuint n = f.input_num();
  ymuint m = input_num();

  if ( m < n ) {
    // f にサポートでない変数が含まれていたらマッチ可能だけど
    // ここではそれは調べない．
    return false;
  }

  // f を GbmMgr に登録する．
  vector<GbmNodeHandle> inputs(n);
  for (ymuint i = 0; i < n; ++ i) {
    GbmNodeHandle input = mMgr.new_input();
    inputs[i] = input;
  }

  // LUT マクロの入力に MUX を接続する．
  vector<GbmNodeHandle> mux_inputs(n + 2);
  for (ymuint i = 0; i < n; ++ i) {
    mux_inputs[i] = inputs[i];
  }
  mux_inputs[n] = GbmNodeHandle::make_zero();
  mux_inputs[n + 1] = GbmNodeHandle::make_one();

  vector<GbmNodeHandle> minputs(m);
  for (ymuint i = 0; i < m; ++ i) {
    GbmNodeHandle minput = mMgr.new_mux(mux_inputs);
    minputs[i] = minput;
  }

  // LUT マクロを作る．
  GbmNodeHandle output1 = gen_macro(minputs);

  bool stat = solver.solve(mMgr, output1, output2);

  return stat;
}

END_NAMESPACE_YM
