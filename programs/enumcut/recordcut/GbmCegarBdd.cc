
/// @file GbmCegarBdd.cc
/// @brief GbmCegarBdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "GbmCegarBdd.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス GbmCegarBdd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GbmCegarBdd::GbmCegarBdd()
{
}

// @brief デストラクタ
GbmCegarBdd::~GbmCegarBdd()
{
}

// @brief 入力順を考慮したマッチング問題を解く
// @param[in] network RcfNetwork
// @param[in] output Reconfigurable Network の出力
// @param[in] func マッチング対象の関数
// @param[in] rep 関数の対称変数の代表番号を収める配列
//            rep[pos] に pos 番めの入力の代表番号が入る．
// @param[out] conf_bits configuration ビットの値を収める配列
// @param[out] iorder 入力順序
//             iorder[pos] に network の pos 番めの入力に対応した
//             関数の入力番号が入る．
bool
GbmCegarBdd::_solve(const RcfNetwork& network,
		    const TvFunc& func,
		    const vector<ymuint>& rep,
		    vector<bool>& conf_bits,
		    vector<ymuint>& iorder)
{
  ymuint nc = network.conf_var_num();
  ymuint nn = network.node_num();
  ymuint ni = network.input_num();

  GbmEngineBdd(nn, nc, ni, rep);

  if ( debug() ) {
    engine.debug_on();
  }

  engine.init_vars();

  conf_bits.resize(nc, false);
  iorder.resize(ni, 0);

  // 外部出力のノード番号と極性
  RcfNodeHandle output = network.output();
  ymuint oid = output.id();
  bool oinv = output.inv();

  ymuint ni_exp = 1U << ni;
  vector<bool> check(ni_exp, false);
  Bool3 stat = kB3X;
  vector<Bool3> model;
  ymuint bit_pat = 0;
  for ( ;; ) {
    check[bit_pat] = true;
    // 入力に定数を割り当てる．
    if ( debug() ) {
      cout << "INPUT: ";
      for (ymuint i = 0; i < ni; ++ i) {
	if ( bit_pat & (1U << (ni - i - 1)) ) {
	  cout << "1";
	}
	else {
	  cout << "0";
	}
      }
      cout << endl;
    }
    // 外部入力変数に値を割り当てたときのBDDを作る．
    ymuint oval = static_cast<bool>(func.value(bit_pat)) ^ oinv;
    Bdd model = engine.make_bdd(network, bit_pat, oid, oval);
    if ( model.is_zero() ) {
      break;
    }
    if ( model.is_invalid() ) {
      break;
    }
    // 現在の model で全部の入力が成り立つか調べてみる．
    BddLitSet one_path = model.onepath();
    vector<Bool3> model_array(nc, kB3False);
    for (BddLitSet::iterator p = one_path.begin();
	 p != one_path.end(); ++ p) {
      Literal lit = *p;
      VarId vid = lit.varid();
      model_array[vid.val()] = (lit.pol() == kPolPosi) ? kB3True : kB3False;
    }

    engine.get_conf_bits(model_array, conf_bits);
    engine.get_iorder(model_array, iorder);
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

  if ( stat == kB3X ) {
    cout << "Aborted" << endl;
  }

  return false;
}

END_NAMESPACE_YM
