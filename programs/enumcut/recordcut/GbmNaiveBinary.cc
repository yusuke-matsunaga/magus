
/// @file GbmNaiveBinary.cc
/// @brief GbmNaiveBinary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "GbmNaiveBinary.h"
#include "GbmEngineBinary.h"
#include "ym_logic/SatSolver.h"
#include "ym_logic/SatStats.h"
#include "ym_logic/SatMsgHandlerImpl1.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

const bool debug = false;

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス GbmNaiveBinary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GbmNaiveBinary::GbmNaiveBinary()
{
}

// @brief デストラクタ
GbmNaiveBinary::~GbmNaiveBinary()
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
GbmNaiveBinary::_solve(const RcfNetwork& network,
		       const TvFunc& func,
		       vector<bool>& conf_bits,
		       vector<ymuint>& iorder)
{
#if 1
  SatSolver solver("minisat");
#else
  SatSolver solver;
#endif

  ymuint nn = network.node_num();
  ymuint nc = network.conf_var_num();
  ymuint ni = network.input_num();

  GbmEngineBinary engine(solver, nn, nc, ni);

  SatMsgHandlerImpl1 satmsghandler(cout);
  solver.reg_msg_handler(&satmsghandler);
  solver.timer_on(true);

  solver.set_max_conflict(100 * 1024);

  conf_bits.resize(nc, false);
  iorder.resize(ni, 0);

  // 外部出力のノード番号と極性
  RcfNodeHandle output = network.output();
  ymuint oid = output.id();
  bool oinv = output.inv();

  ymuint ni_exp = 1U << ni;
  Bool3 stat = kB3X;
  vector<Bool3> model;
  for (ymuint b = 0U; b < ni_exp; ++ b) {
    // 入力に定数を割り当てる．
    if ( debug ) {
      cout << "INPUT: ";
      for (ymuint i = 0; i < ni; ++ i) {
	if ( b & (1U << (ni - i - 1)) ) {
	  cout << "1";
	}
	else {
	  cout << "0";
	}
      }
      cout << endl;
    }
    // 外部入力変数に値を割り当てたときの CNF 式を作る．
    ymuint oval = static_cast<bool>(func.value(b)) ^ oinv;
    bool ok = engine.make_cnf(network, b, oid, oval);
    if ( !ok ) {
      break;
    }
  }

  stat = solver.solve(model);

  if ( stat == kB3True ) {
    engine.get_conf_bits(model, conf_bits);
    engine.get_iorder(model, iorder);
    return true;
  }
  else if ( stat == kB3X ) {
    cout << "Aborted" << endl;
  }

  return false;
}

END_NAMESPACE_YM
