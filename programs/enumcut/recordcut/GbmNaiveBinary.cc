
/// @file GbmNaiveBinary.cc
/// @brief GbmNaiveBinary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "GbmNaiveBinary.h"
#include "GbmEngine.h"
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
// @note iorder[0] に func の0番めの入力に対応した RcfNetwork の入力番号が入る．
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

  GbmEngine engine(solver, nn, nc);

  SatMsgHandlerImpl1 satmsghandler(cout);
  solver.reg_msg_handler(&satmsghandler);
  solver.timer_on(true);

  solver.set_max_conflict(100 * 1024);

  // configuration 変数を作る．
  vector<VarId> conf_vid_array(nc);
  vector<GbmLit> conf_var_array(nc);
  for (ymuint i = 0; i < nc; ++ i) {
    VarId vid = solver.new_var();
    conf_vid_array[i] = vid;
    engine.set_conf_var(i, GbmLit(vid));
  }
  conf_bits.resize(nc, false);

  // 入力順用の変数を作る．
  ymuint ni = network.input_num();
  ymuint m = 0;
  for (ymuint t = 1; t < ni; t <<= 1, ++ m) ;
  vector<VarId> iorder_vid_array(ni * m);
  for (ymuint i = 0; i < ni; ++ i) {
    for (ymuint j = 0; j < m; ++ j) {
      iorder_vid_array[i * m + j] = solver.new_var();
    }
  }
  iorder.resize(ni, 0);

  // 外部出力のノード番号と極性
  RcfNodeHandle output = network.output();
  ymuint oid = output.id();
  bool oinv = output.inv();

  vector<const RcfNode*> input_list(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    input_list[i] = network.input_node(i);
  }
  vector<const RcfNode*> node_list;
  node_list.reserve(nn);
  for (ymuint id = 0; id < nn; ++ id) {
    const RcfNode* node = network.node(id);
    if ( !node->is_input() ) {
      node_list.push_back(node);
    }
  }
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
    engine.make_inputs_cnf_binary(input_list, iorder_vid_array, m, b);
    ymuint oval = static_cast<bool>(func.value(b)) ^ oinv;
    bool ok = engine.make_nodes_cnf(node_list, oid, oval);
    if ( !ok ) {
      break;
    }
  }

  stat = solver.solve(model);

  if ( stat == kB3True ) {
    for (ymuint i = 0; i < nc; ++ i) {
      VarId vid = conf_vid_array[i];
      if ( model[vid.val()] == kB3True ) {
	conf_bits[i] = true;
      }
      else {
	conf_bits[i] = false;
      }
    }
    for (ymuint i = 0; i < ni; ++ i) {
      ymuint pos = 0;
      for (ymuint j = 0; j < m; ++ j) {
	VarId vid = iorder_vid_array[i * m + j];
	if ( model[vid.val()] == kB3True ) {
	  pos += (1U << j);
	}
      }
      iorder[pos] = i;
    }
    return true;
  }
  else if ( stat == kB3X ) {
    cout << "Aborted" << endl;
  }

  return false;
}

END_NAMESPACE_YM
