
/// @file GbmNaiveEnum.cc
/// @brief GbmNaiveEnum の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "GbmNaiveEnum.h"
#include "GbmEngineEnum.h"
#include "ym_logic/SatStats.h"
#include "ym_logic/SatMsgHandler.h"
#include "ym_utils/PermGen.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

const bool debug = false;

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス GbmNaiveEnum
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GbmNaiveEnum::GbmNaiveEnum()
{
}

// @brief デストラクタ
GbmNaiveEnum::~GbmNaiveEnum()
{
}

// @brief 入力順を考慮したマッチング問題を解く
// @param[in] network RcfNetwork
// @param[in] func マッチング対象の関数
// @param[out] conf_bits configuration ビットの値を収める配列
// @param[out] iorder 入力順序
//             iorder[pos] に network の pos 番めの入力に対応した
//             関数の入力番号が入る．
bool
GbmNaiveEnum::_solve(const RcfNetwork& network,
		     const TvFunc& func,
		     vector<bool>& conf_bits,
		     vector<ymuint>& iorder)
{
  ymuint ni = network.input_num();
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
    }
    if ( skip ) {
      continue;
    }
    bool stat = _solve_with_order(network, func, tmp_order, conf_bits);
    if ( stat ) {
      iorder.resize(ni, 0);
      for (ymuint i = 0; i < ni; ++ i) {
	iorder[i] = tmp_order[i];
      }
      return true;
    }
  }
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
GbmNaiveEnum::_solve_with_order(const RcfNetwork& network,
				const TvFunc& func,
				const vector<ymuint>& iorder,
				vector<bool>& conf_bits)
{
  SatSolver solver("minisat");

  ymuint nc = network.conf_var_num();
  ymuint nn = network.node_num();

  GbmEngineEnum engine(solver, nn, nc);

  conf_bits.resize(nc, false);

  // 外部出力のノード番号と極性
  RcfNodeHandle output = network.output();
  ymuint oid = output.id();
  bool oinv = output.inv();

  ymuint ni = network.input_num();
  ymuint ni_exp = 1U << ni;
  Bool3 stat = kB3X;
  vector<Bool3> model;
  for (ymuint bit_pat = 0U; bit_pat < ni_exp; ++ bit_pat) {
    // 入力に定数を割り当てて CNF 式を作る．
    ymuint oval = static_cast<bool>(func.value(bit_pat)) ^ oinv;
    bool ok = engine.make_cnf(network, bit_pat, iorder, oid, oval);
    if ( !ok ) {
      return false;
    }
  }

  stat = solver.solve(model);

  if ( stat == kB3True ) {
    engine.get_conf_bits(model, conf_bits);
    return true;
  }

  return false;
}

END_NAMESPACE_YM
