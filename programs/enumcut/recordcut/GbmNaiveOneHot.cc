
/// @file GbmNaiveOneHot.cc
/// @brief GbmNaiveOneHot の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "GbmNaiveOneHot.h"
#include "GbmEngine.h"
#include "ym_logic/SatStats.h"
#include "ym_logic/SatMsgHandlerImpl1.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

const bool debug = false;

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス GbmNaiveOneHot
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GbmNaiveOneHot::GbmNaiveOneHot()
{
}

// @brief デストラクタ
GbmNaiveOneHot::~GbmNaiveOneHot()
{
}

// @brief 入力順を考慮したマッチング問題を解く
// @param[in] network RcfNetwork
// @param[in] func マッチング対象の関数
// @param[out] conf_bits configuration ビットの値を収める配列
// @param[out] iorder 入力順序
// @note iorder[0] に func の0番めの入力に対応した RcfNetwork の入力番号が入る．
bool
GbmNaiveOneHot::_solve(const RcfNetwork& network,
		       const TvFunc& func,
		       vector<bool>& conf_bits,
		       vector<ymuint>& iorder)
{
#if 1
  SatSolver solver("minisat");
#else
  SatSolver solver;
#endif

  ymuint nc = network.conf_var_num();
  ymuint nn = network.node_num();

  GbmEngine engine(solver, nn, nc);

  SatMsgHandlerImpl1 satmsghandler(cout);
  solver.reg_msg_handler(&satmsghandler);
  solver.timer_on(true);

  solver.set_max_conflict(100 * 1024);

  // configuration 変数を作る．
  vector<VarId> conf_vid_array(nc);
  for (ymuint i = 0; i < nc; ++ i) {
    VarId vid = solver.new_var();
    conf_vid_array[i] = vid;
    engine.set_conf_var(i, GbmLit(vid));
  }
  conf_bits.resize(nc, false);

  // 入力順用の変数を作る．
  ymuint ni = network.input_num();
  vector<VarId> iorder_vid_array(ni * ni);
  for (ymuint i = 0; i < ni; ++ i) {
    for (ymuint j = 0; j < ni; ++ j) {
      iorder_vid_array[i * ni + j] = solver.new_var();
    }
  }
  iorder.resize(ni, 0);

  // 外部出力のノード番号と極性
  RcfNodeHandle output = network.output();
  ymuint oid = output.id();
  bool oinv = output.inv();

  // 外部入力変数に値を割り当てたときの CNF 式を作る．
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
    for (ymuint i = 0; i < ni; ++ i) {
      const RcfNode* node = network.input_node(i);
      ymuint id = node->id();
      VarId vid = solver.new_var();
      if ( debug ) {
	cout << " lut_input#" << i << ": " << vid << endl;
      }
      engine.set_node_var(id, GbmLit(vid));
      // 入力と外部入力の間の関係式を作る．
      for (ymuint j = 0; j < ni; ++ j) {
	Literal lit0(iorder_vid_array[i * ni + j], kPolNega);
	tPol pol = ( b & (1U << j) ) ? kPolPosi : kPolNega;
	Literal lit1(vid, pol);
	if ( debug ) {
	  cout << " added clause = " << lit0 << " " << lit1 << endl;
	}
	solver.add_clause(lit0, lit1);
      }
      // 2つの変数が同時に true になってはいけないというルール
      for (ymuint j = 0; j < ni; ++ j) {
	Literal lit0(iorder_vid_array[i * ni + j], kPolNega);
	for (ymuint k = j + 1; k < ni; ++ k) {
	  Literal lit1(iorder_vid_array[i * ni + k], kPolNega);
	  if ( debug ) {
	    cout << " added clause = " << lit0 << " " << lit1 << endl;
	  }
	  solver.add_clause(lit0, lit1);
	}
      }
      // 最低1つの変数が true にならなければならないというルール
      vector<Literal> tmp_lits(ni);
      for (ymuint j = 0; j < ni; ++ j) {
	tmp_lits[j] = Literal(iorder_vid_array[i * ni + j], kPolPosi);
      }
      if ( debug ) {
	cout << " added clause = ";
	for (ymuint x = 0; x < ni; ++ x) {
	  cout << " " << tmp_lits[x];
	}
	cout << endl;
      }
      solver.add_clause(tmp_lits);
      // 異なる LUT 入力におなじ入力が接続してはいけないというルール
      for (ymuint j = 0; j < ni; ++ j) {
	Literal lit0(iorder_vid_array[i * ni + j], kPolNega);
	for (ymuint k = 0; k < i; ++ k) {
	  Literal lit1(iorder_vid_array[k * ni + j], kPolNega);
	  if ( debug ) {
	    cout << " added clause = " << lit0 << " " << lit1 << endl;
	  }
	  solver.add_clause(lit0, lit1);
	}
      }

      // 対称性を考慮したルール
      ymuint pred;
      if ( network.get_pred(i, pred) ) {
	for (ymuint j = 0; j < ni; ++ j) {
	  Literal lit0(iorder_vid_array[i * ni + j], kPolNega);
	  for (ymuint k = j + 1; k < ni; ++ k) {
	    Literal lit1(iorder_vid_array[pred * ni + k], kPolNega);
	    if ( debug ) {
	      cout << " added clause = " << lit0 << " " << lit1 << endl;
	    }
	    solver.add_clause(lit0, lit1);
	  }
	}
      }
    }
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
      for (ymuint i = 0; i < ni; ++ i) {
	for (ymuint j = 0; j < ni; ++ j) {
	  VarId vid = iorder_vid_array[i * ni + j];
	  if ( model[vid.val()] == kB3True ) {
	    iorder[j] = i;
	    break;
	  }
	}
      }
    }
    return true;
  }
  else if ( stat == kB3X ) {
    cout << "Aborted" << endl;
  }

  return false;
}

END_NAMESPACE_YM
