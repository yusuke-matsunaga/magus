
/// @file GbmNaive.cc
/// @brief GbmNaive の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "GbmNaive.h"
#include "GbmEngine.h"
#include "ym_logic/SatStats.h"
#include "ym_logic/SatMsgHandler.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

const bool debug = false;

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス GbmNaive
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GbmNaive::GbmNaive()
{
}

// @brief デストラクタ
GbmNaive::~GbmNaive()
{
}

// @brief 問題を解く
// @param[in] network RcfNetwork
// @param[in] output Reconfigurable Network の出力
// @param[in] func マッチング対象の関数
// @param[out] conf_bits configuration ビットの値を収める配列
bool
GbmNaive::_solve(const RcfNetwork& network,
		 const TvFunc& func,
		 vector<bool>& conf_bits)
{
  SatSolver solver;

  // configuration 変数を作る．
  ymuint nc = network.conf_var_num();
  vector<VarId> conf_vid_array(nc);
  vector<GbmLit> conf_var_array(nc);
  for (ymuint i = 0; i < nc; ++ i) {
    VarId vid = solver.new_var();
    conf_vid_array[i] = vid;
    conf_var_array[i] = GbmLit(vid);
  }
  conf_bits.resize(nc, false);

  // 外部出力のノード番号と極性
  RcfNodeHandle output = network.output();
  ymuint oid = output.id();
  bool oinv = output.inv();

  // 外部入力変数に値を割り当てたときの CNF 式を作る．
  ymuint nn = network.node_num();
  vector<const RcfNode*> node_list;
  node_list.reserve(nn);
  for (ymuint id = 0; id < nn; ++ id) {
    const RcfNode* node = network.node(id);
    if ( !node->is_input() ) {
      node_list.push_back(node);
    }
  }
  vector<GbmLit> node_var_array(nn);
  ymuint ni = network.input_num();
  ymuint ni_exp = 1U << ni;
  bool conflict = false;
  for (ymuint b = 0U; b < ni_exp && !conflict; ++ b) {
    // 入力に定数を割り当てる．
    for (ymuint i = 0; i < ni; ++ i) {
      const RcfNode* node = network.input_node(i);
      ymuint id = node->id();
      if ( b & (1U << i) ) {
	node_var_array[id] = GbmLit::make_one();
      }
      else {
	node_var_array[id] = GbmLit::make_zero();
      }
    }
    // 内部のノードに変数番号を割り当てる．
    for (vector<const RcfNode*>::iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      const RcfNode* node = *p;
      if ( node->id() == oid ) {
	// 外部出力の時は関数の値に応じた定数となる．
	if ( static_cast<bool>(func.value(b)) ^ oinv ) {
	  node_var_array[oid] = GbmLit::make_one();
	}
	else {
	  node_var_array[oid] = GbmLit::make_zero();
	}
      }
      else {
	VarId vid = solver.new_var();
	node_var_array[node->id()] = GbmLit(vid);
      }
    }
    // 内部のノードに対する CNF 式を作る．
    for (vector<const RcfNode*>::iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      const RcfNode* node = *p;
      bool stat = make_node_cnf(solver, node, node_var_array, conf_var_array);
      if ( !stat ) {
	// 矛盾が起こった．
	conflict = true;
	break;
      }
    }
  }

  vector<Bool3> model;
  Bool3 stat = solver.solve(model);

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
    return true;
  }

  return false;
}


BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// ymsat 用の SatMsgHandler
//////////////////////////////////////////////////////////////////////
class YmsatMsgHandler :
  public SatMsgHandler
{
public:

  /// @brief コンストラクタ
  YmsatMsgHandler(ostream& s);

  /// @brief デストラクタ
  virtual
  ~YmsatMsgHandler();

  /// @brief メッセージ出力関数
  virtual
  void
  operator()(const SatStats& stats);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力ストリーム
  ostream& mS;

};

// @brief コンストラクタ
YmsatMsgHandler::YmsatMsgHandler(ostream& s) :
  mS(s)
{
}

// @brief デストラクタ
YmsatMsgHandler::~YmsatMsgHandler()
{
}

// @brief メッセージ出力関数
void
YmsatMsgHandler::operator()(const SatStats& stats)
{
  mS << "| "
     << setw(9) << stats.mConflictNum
     << " | "
     << setw(9) << stats.mConstrClauseNum
     << " "
     << setw(9) << stats.mConstrLitNum
     << " | "
     << setw(9) << stats.mLearntLimit
     << " "
     << setw(9) << stats.mLearntClauseNum
     << " "
     << setw(9) << stats.mLearntLitNum
     << " |" << endl;
}

END_NONAMESPACE

// @brief 入力順を考慮したマッチング問題を解く
// @param[in] network RcfNetwork
// @param[in] func マッチング対象の関数
// @param[out] conf_bits configuration ビットの値を収める配列
// @param[out] iorder 入力順序
// @note iorder[0] に func の0番めの入力に対応した RcfNetwork の入力番号が入る．
bool
GbmNaive::_solve(const RcfNetwork& network,
		 const TvFunc& func,
		 vector<bool>& conf_bits,
		 vector<ymuint>& iorder)
{
#if 1
  SatSolver solver("minisat");
#else
  SatSolver solver;
#endif

  YmsatMsgHandler satmsghandler(cout);
  solver.reg_msg_handler(&satmsghandler);
  solver.timer_on(true);

  solver.set_max_conflict(100 * 1024);

#if 0
  cout << "===================================================================" << endl;
  cout << "| conflicts |       ORIGINAL      |             LEARNT            |" << endl;
  cout << "|           |   Clauses      Lits |     limit   Clauses      Lits |" << endl;
  cout << "===================================================================" << endl;
#endif

  // configuration 変数を作る．
  ymuint nc = network.conf_var_num();
  vector<VarId> conf_vid_array(nc);
  vector<GbmLit> conf_var_array(nc);
  for (ymuint i = 0; i < nc; ++ i) {
    VarId vid = solver.new_var();
    conf_vid_array[i] = vid;
    conf_var_array[i] = GbmLit(vid);
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
  ymuint nn = network.node_num();
  vector<const RcfNode*> node_list;
  node_list.reserve(nn);
  for (ymuint id = 0; id < nn; ++ id) {
    const RcfNode* node = network.node(id);
    if ( !node->is_input() ) {
      node_list.push_back(node);
    }
  }
  vector<GbmLit> node_var_array(nn);
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
      node_var_array[id] = GbmLit(vid);
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
	  node_var_array[oid] = GbmLit::make_one();
	}
	else {
	  node_var_array[oid] = GbmLit::make_zero();
	}
      }
      else {
	VarId vid = solver.new_var();
	node_var_array[node->id()] = GbmLit(vid);
      }
    }
    // 内部のノードに対する CNF 式を作る．
    for (vector<const RcfNode*>::iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      const RcfNode* node = *p;
      bool stat = make_node_cnf(solver, node, node_var_array, conf_var_array);
      if ( !stat ) {
	// 矛盾が起こった．
	conflict = true;
	break;
      }
    }
    if ( conflict ) {
      break;
    }
  }

  stat = solver.solve(model);

#if 0
    SatStats stats;
    solver.get_stats(stats);

    cout << "===================================================================" << endl;
    cout << "restarts          : " << stats.mRestart << endl
	 << "conflicts         : " << stats.mConflictNum << endl
	 << "decisions         : " << stats.mDecisionNum << endl
	 << "propagations      : " << stats.mPropagationNum << endl
	 << "conflict literals : " << stats.mLearntLitNum << endl
	 << "CPU time          : " << stats.mTime << endl;
#endif

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

// @brief ノードの入出力の関係を表す CNF 式を作る．
// @param[in] solver SAT ソルバ
// @param[in] node 対象のノード
// @param[in] node_var_array ノードの変数番号の配列
// @param[in] conf_var_array 設定変数番号の配列
bool
GbmNaive::make_node_cnf(SatSolver& solver,
			const RcfNode* node,
			const vector<GbmLit>& node_var_array,
			const vector<GbmLit>& conf_var_array)
{
  if ( node->is_input() ) {
    return true;
  }

  GbmEngine engine;

  GbmLit output = node_var_array[node->id()];

  ymuint ni = node->fanin_num();
  if ( node->is_and() ) {
    vector<GbmLit> inputs(ni);
    make_inputs(node, node_var_array, inputs);
    bool stat = engine.make_AND(solver, inputs, output);
    if ( !stat ) {
      return false;
    }
  }
  else if ( node->is_lut() ) {
    vector<GbmLit> inputs(ni);
    make_inputs(node, node_var_array, inputs);
    ymuint ni_exp = 1U << ni;
    vector<GbmLit> lut_vars(ni_exp);
    ymuint conf0 = node->conf_base();
    for (ymuint i = 0; i < ni_exp; ++ i) {
      lut_vars[i] = conf_var_array[conf0 + i];
    }
    bool stat = engine.make_LUT(solver, inputs, lut_vars, output);
    if ( !stat ) {
      return false;
    }
  }
  else if ( node->is_mux() ) {
    vector<GbmLit> inputs(ni);
    make_inputs(node, node_var_array, inputs);
    ymuint ns = node->conf_size();
    vector<GbmLit> s_vars(ns);
    ymuint s0 = node->conf_base();
    for (ymuint i = 0; i < ns; ++ i) {
      s_vars[i] = conf_var_array[s0 + i];
    }
    bool stat = engine.make_MUX(solver, inputs, s_vars, output);
    if ( !stat ) {
      return false;
    }
  }

  return true;
}

// @brief ノードの入力を表すリテラルを作る．
// @param[in] node 対象のノード
// @param[in] node_var_array ノードの変数番号の配列
// @param[out] inputs 結果のリテラルを格納する配列
void
GbmNaive::make_inputs(const RcfNode* node,
		      const vector<GbmLit>& node_var_array,
		      vector<GbmLit>& inputs)
{
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    RcfNodeHandle ih = node->fanin(i);
    inputs[i] = handle_to_lit(ih, node_var_array);
  }
}

// @brief RcfNodeHandle から GbmLit を作る．
// @param[in] handle ハンドル
// @param[in] node_var_array ノードの変数番号の配列
GbmLit
GbmNaive::handle_to_lit(RcfNodeHandle handle,
			const vector<GbmLit>& node_var_array)
{
  if ( handle.is_zero() ) {
    return GbmLit::make_zero();
  }
  if ( handle.is_one() ) {
    return GbmLit::make_one();
  }
  GbmLit lit = node_var_array[handle.id()];
  if ( handle.inv() ) {
    lit.negate();
  }
  return lit;
}

END_NAMESPACE_YM
