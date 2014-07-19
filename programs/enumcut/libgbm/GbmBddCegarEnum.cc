
/// @file GbmBddCegarEnum.cc
/// @brief GbmBddCegarEnum の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "GbmBddCegarEnum.h"
#include "GbmBddEngineEnum.h"
#include "utils/PermGen.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス GbmBddCegarEnum
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GbmBddCegarEnum::GbmBddCegarEnum()
{
}

// @brief デストラクタ
GbmBddCegarEnum::~GbmBddCegarEnum()
{
}

// @brief 入力順を考慮したマッチング問題を解く
// @param[in] network RcfNetwork
// @param[in] func マッチング対象の関数
// @param[in] rep 関数の対称変数の代表番号を収める配列
//            rep[pos] に pos 番めの入力の代表番号が入る．
// @param[out] conf_bits configuration ビットの値を収める配列
// @param[out] iorder 入力順序
//             iorder[pos] に network の pos 番めの入力に対応した
//             関数の入力番号が入る．
bool
GbmBddCegarEnum::_solve(const RcfNetwork& network,
			const TvFunc& func,
			const vector<ymuint>& rep,
			vector<bool>& conf_bits,
			vector<ymuint>& iorder,
			ymuint& loop_count)
{
  ymuint ni = network.input_num();
  ymuint np = 0;
  loop_count = 0;
  for (PermGen pg(ni, ni); !pg.is_end(); ++ pg) {
    vector<ymuint> tmp_order(ni);
    bool skip = false;
    for (ymuint i = 0; i < ni; ++ i) {
      tmp_order[i] = pg(i);
      ymuint cur = tmp_order[i];
      ymuint pred;
      if ( network.get_pred(i, pred) ) {
	if ( tmp_order[pred] > cur ) {
	  skip = true;
	  break;
	}
      }
      ymuint cur_rep = rep[cur];
      if ( cur_rep != cur ) {
	bool found = false;
	for (ymuint j = 0; j < i; ++ j) {
	  if ( tmp_order[j] == cur_rep ) {
	    found = true;
	    break;
	  }
	}
	if ( !found ) {
	  skip = true;
	}
      }
    }
    if ( skip ) {
      continue;
    }
    ++ np;
    bool stat = _solve_with_order(network, func, tmp_order, conf_bits, loop_count);
    if ( stat ) {
      iorder.resize(ni, 0);
      for (ymuint i = 0; i < ni; ++ i) {
	iorder[i] = tmp_order[i];
      }
      cout << "total " << np << " permutations (success)" << endl;
      return true;
    }
  }
  cout << "total " << np << " permutations (fail)" << endl;
  return false;
}

// @brief 問題を解く
// @param[in] network RcfNetwork
// @param[in] output Reconfigurable Network の出力
// @param[in] func マッチング対象の関数
// @param[in] iorder 入力順序
//            iorder[pos] に network の pos 番めの入力に対応した
//            関数の入力番号が入る．
// @param[out] conf_bits configuration ビットの値を収める配列
bool
GbmBddCegarEnum::_solve_with_order(const RcfNetwork& network,
				   const TvFunc& func,
				   const vector<ymuint>& iorder,
				   vector<bool>& conf_bits,
				   ymuint& loop_count)
{
  BddMgr mgr("bmc", "gbm");

  ymuint nc = network.conf_var_num();

  GbmBddEngineEnum engine(mgr);

  if ( debug() ) {
    engine.debug_on();
  }

  engine.init_vars(network);

  conf_bits.resize(nc, false);

  ymuint ni = network.input_num();
  ymuint ni_exp = 1U << ni;
  vector<bool> check(ni_exp, false);
  ymuint bit_pat = 0U;
  for ( ; ; ++ loop_count) {
    check[bit_pat] = true;
    // 入力に定数を割り当てたときの CNF 式を作る．
    ymuint oval = func.value(bit_pat);
    bool ok = engine.make_bdd(network, bit_pat, iorder, oval);
    if ( !ok ) {
      return false;
    }

    vector<Bool3> model;
    engine.get_model(model);

    // 現在の model で全部の入力が成り立つか調べてみる．
    engine.get_conf_bits(model, conf_bits);
    bool pass = true;
    for (ymuint b = 0; b < ni_exp; ++ b) {
      if ( check[b] ) {
	continue;
      }
      ymuint exp_out = func.value(b);
      vector<bool> ival_list(ni);
      for (ymuint i = 0; i < ni; ++ i) {
	ymuint src_pos = iorder[i];
	bool val = (b & (1U << src_pos)) ? true : false;
	ival_list[i] = val;
      }
      if ( network.simulate(ival_list, conf_bits) != exp_out ) {
	bit_pat = b;
	pass = false;
	break;
      }
    }
    if ( pass ) {
      return true;
    }
  }

  return false;
}

END_NAMESPACE_YM
