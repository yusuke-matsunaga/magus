
/// @file GbmIncrBinary.cc
/// @brief GbmIncrBinary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "GbmIncrBinary.h"
#include "GbmEngine.h"
#include "ym_logic/SatStats.h"
#include "ym_logic/SatMsgHandlerImpl1.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

const bool debug = false;

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス GbmIncrBinary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GbmIncrBinary::GbmIncrBinary()
{
}

// @brief デストラクタ
GbmIncrBinary::~GbmIncrBinary()
{
}

// @brief 入力順を考慮したマッチング問題を解く
// @param[in] network RcfNetwork
// @param[in] func マッチング対象の関数
// @param[out] conf_bits configuration ビットの値を収める配列
// @param[out] iorder 入力順序
// @note iorder[0] に func の0番めの入力に対応した RcfNetwork の入力番号が入る．
bool
GbmIncrBinary::_solve(const RcfNetwork& network,
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
  bool conflict = false;
  for (ymuint b = 0U; b < ni_exp && !conflict; ++ b) {
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
      vector<Literal> tmp_lits(m + 1);
      for (ymuint j = 0; j < ni; ++ j) {
	for (ymuint k = 0; k < m; ++ k) {
	  VarId kvar = iorder_vid_array[i * m + k];
	  // こちらは含意の左辺なので否定する．
	  if ( j & (1U << k) ) {
	    tmp_lits[k] = Literal(kvar, kPolNega);
	  }
	  else {
	    tmp_lits[k] = Literal(kvar, kPolPosi);
	  }
	}
	if ( b & (1U << j) ) {
	  tmp_lits[m] = Literal(vid, kPolPosi);
	}
	else {
	  tmp_lits[m] = Literal(vid, kPolNega);
	}
	if ( debug ) {
	  cout << " added clause = ";
	  for (ymuint x = 0; x <= m; ++ x) {
	    cout << " " << tmp_lits[x];
	  }
	  cout << endl;
	}
	solver.add_clause(tmp_lits);
      }
      // 使っていない変数の組み合わせを禁止する．
      vector<Literal> tmp_lits2(m);
      for (ymuint j = ni; j < (1U << m); ++ j) {
	for (ymuint k = 0; k < m; ++ k) {
	  VarId kvar = iorder_vid_array[i * m + k];
	  if ( j & (1U << k) ) {
	    tmp_lits2[k] = Literal(kvar, kPolNega);
	  }
	  else {
	    tmp_lits2[k] = Literal(kvar, kPolPosi);
	  }
	}
	if ( debug ) {
	  cout << " added clause = ";
	  for (ymuint x = 0; x < m; ++ x) {
	    cout << " " << tmp_lits2[x];
	  }
	  cout << endl;
	}
	solver.add_clause(tmp_lits2);
      }
#if 0
      // 異なる LUT 入力におなじ入力が接続してはいけないというルール
      for (ymuint j = 0; j < ni; ++ j) {
	vector<Literal> tmp_lits3(m * 2);
	for (ymuint k = 0; k < i; ++ k) {
	  for (ymuint l = 0; l < m; ++ l) {
	    tPol pol = ( j & (1U << l) ) ? kPolNega : kPolPosi;
	    tmp_lits3[l] = Literal(iorder_vid_array[k * m + l], pol);
	    tmp_lits3[l + m] = Literal(iorder_vid_array[i * m + l], pol);
	  }
	  if ( debug ) {
	    cout << " added clause = ";
	    for (ymuint x = 0; x < m; ++ x) {
	      cout << " " << tmp_lits3[x];
	    }
	    cout << endl;
	  }
	  solver.add_clause(tmp_lits3);
	}
      }
      // 対称性を考慮したルール
      ymuint pred;
      if ( network.get_pred(i, pred) ) {
	for (ymuint j = 0; j < ni; ++ j) {
	  vector<Literal> tmp_lits3(m * 2);
	  for (ymuint l = 0; l < m; ++ l) {
	    tPol pol = ( j & (1U << l) ) ? kPolNega : kPolPosi;
	    tmp_lits3[l] = Literal(iorder_vid_array[i * m + l], pol);
	  }
	  for (ymuint k = j + 1; k < ni; ++ k) {
	    for (ymuint l = 0; l < m; ++ l) {
	      tPol pol = ( j & (1U << l) ) ? kPolNega : kPolPosi;
	      tmp_lits3[l + m] = Literal(iorder_vid_array[pred * m + l], pol);
	    }
	    if ( debug ) {
	      cout << " added clause = ";
	      for (ymuint x = 0; x < m; ++ x) {
		cout << " " << tmp_lits3[x];
	      }
	      cout << endl;
	    }
	    solver.add_clause(tmp_lits3);
	  }
	}
      }
#endif
    }
    // 内部のノードに変数番号を割り当てる．
    for (vector<const RcfNode*>::iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      const RcfNode* node = *p;
      if ( node->id() == oid ) {
	// 外部出力の時は関数の値に応じた定数となる．
	if ( static_cast<bool>(func.value(b)) ^ oinv ) {
	  engine.set_node_var(oid, GbmLit::make_one());
	}
	else {
	  engine.set_node_var(oid, GbmLit::make_zero());
	}
      }
      else {
	VarId vid = solver.new_var();
	engine.set_node_var(node->id(), GbmLit(vid));
      }
    }
    // 内部のノードに対する CNF 式を作る．
    for (vector<const RcfNode*>::iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      const RcfNode* node = *p;
      bool stat = engine.make_node_cnf(node);
      if ( !stat ) {
	// 矛盾が起こった．
	conflict = true;
	break;
      }
    }
    if ( conflict ) {
      break;
    }
    stat = solver.solve(model);
    if ( stat == kB3False ) {
      conflict = true;
      break;
    }
  }

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
	ymuint pos = 0;
	for (ymuint j = 0; j < m; ++ j) {
	  VarId vid = iorder_vid_array[i * m + j];
	  if ( model[vid.val()] == kB3True ) {
	    pos += (1U << j);
	  }
	}
	iorder[pos] = i;
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
