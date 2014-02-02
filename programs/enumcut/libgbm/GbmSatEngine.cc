
/// @file GbmSatEngine.cc
/// @brief GbmSatEngine の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "GbmSatEngine.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス GbmSatEngine
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] solver SATソルバ
GbmSatEngine::GbmSatEngine(SatSolver& solver) :
  mSolver(solver),
  mDebug(false)
{
}

// @brief デストラクタ
GbmSatEngine::~GbmSatEngine()
{
}

// @brief debug フラグを立てる
void
GbmSatEngine::debug_on()
{
  mDebug = true;
}

// @brief debug フラグを降ろす
void
GbmSatEngine::debug_off()
{
  mDebug = false;
}

// @brief debug フラグの値を得る．
bool
GbmSatEngine::debug() const
{
  return mDebug;
}

// @brief 設定変数を初期化する．
// @param[in] network 対象の LUT ネットワーク
void
GbmSatEngine::init_conf_vars(const RcfNetwork& network)
{
  ymuint nc = network.conf_var_num();
  mConfVarArray.clear();
  mConfVarArray.resize(nc);
  for (ymuint i = 0; i < nc; ++ i) {
    VarId vid = new_var();
    mConfVarArray[i] = vid;
    if ( debug() ) {
      cout << "conf_bits[" << i << "] = " << vid << endl;
    }
  }

  ymuint nn = network.node_num();
  mNodeVarArray.clear();
  mNodeVarArray.resize(nn);
}

// @brief ノードに対応するリテラルを登録する．
// @param[in] id ノード番号
// @param[in] lit リテラル
void
GbmSatEngine::set_node_var(ymuint id,
			   GbmLit lit)
{
  assert_cond( id < mNodeVarArray.size(), __FILE__, __LINE__);
  mNodeVarArray[id] = lit;
  if ( debug() ) {
    cout << "Node#" << id << ": " << lit << endl;
  }
}

// @brief SAT用の新しい変数を作る．
VarId
GbmSatEngine::new_var()
{
  return mSolver.new_var();
}

// @brief 節を追加する．
void
GbmSatEngine::add_clause(Literal lit1,
			 Literal lit2)
{
  mSolver.add_clause(lit1, lit2);

  if ( mDebug ) {
    cout << " added clause (" << lit1 << " + " << lit2 << ")" << endl;
  }
}

// @brief 節を追加する．
void
GbmSatEngine::add_clause(Literal lit1,
			 Literal lit2,
			 Literal lit3)
{
  mSolver.add_clause(lit1, lit2, lit3);

  if ( mDebug ) {
    cout << " added clause ("
	 << lit1 << " + "
	 << lit2 << " + "
	 << lit3 << ")" << endl;
  }
}

// @brief 節を追加する．
void
GbmSatEngine::add_clause(Literal lit1,
			 Literal lit2,
			 Literal lit3,
			 Literal lit4)
{
  mSolver.add_clause(lit1, lit2, lit3, lit4);

  if ( mDebug ) {
    cout << " added clause ("
	 << lit1 << " + "
	 << lit2 << " + "
	 << lit3 << " + "
	 << lit4 << ")" << endl;
  }
}

// @brief 節を追加する．
void
GbmSatEngine::add_clause(const vector<Literal>& lits)
{
  mSolver.add_clause(lits);

  if ( mDebug ) {
    cout << " added clause (";
    const char* plus = "";
    for (ymuint i = 0; i < lits.size(); ++ i) {
      cout << plus << lits[i];
      plus = " + ";
    }
    cout << ")" << endl;
  }
}

// @brief SAT モデルから設定変数の割り当てを取り出す．
// @param[in] model SAT モデル
// @param[out] conf_bits 設定変数の割り当て
void
GbmSatEngine::get_conf_bits(const vector<Bool3>& model,
			    vector<bool>& conf_bits) const
{
  ymuint nc = mConfVarArray.size();
  for (ymuint i = 0; i < nc; ++ i) {
    VarId vid = mConfVarArray[i];
    if ( model[vid.val()] == kB3True ) {
      conf_bits[i] = true;
    }
    else {
      conf_bits[i] = false;
    }
  }
}

// @brief 内部ノードに変数番号を割り当て，CNF式を作る．
// @param[in] network 対象の LUT ネットワーク
// @param[in] oval 出力値
// @return 割り当てが矛盾を起こしたら false を返す．
bool
GbmSatEngine::make_nodes_cnf(const RcfNetwork& network,
			     ymuint oval)
{
  // 外部出力のノード番号と極性
  RcfNodeHandle output = network.output();
  ymuint oid = output.id();
  bool oinv = output.inv();

  if ( oinv ) {
    oval = !oval;
  }

  // 内部のノードに変数番号を割り当てて，
  // ノードの入出力の関係を表す CNF 式を作る．
  ymuint nf = network.func_node_num();
  for (ymuint i = 0; i < nf; ++ i) {
    const RcfNode* node = network.func_node(i);
    if ( node->id() == oid ) {
      // 外部出力の時は関数の値に応じた定数となる．
      if ( oval ) {
	set_node_var(oid, GbmLit::make_one());
      }
      else {
	set_node_var(oid, GbmLit::make_zero());
      }
    }
    else {
      // そうでなければ新しい変数を作る．
      VarId vid = mSolver.new_var();
      set_node_var(node->id(), GbmLit(vid));
      if ( debug() ) {
	cout << "Node#" << node->id() << ": " << vid << endl;
      }
    }
    bool stat = make_node_cnf(node);
    if ( !stat ) {
      // 矛盾が起こった．
      return false;
    }
  }
  return true;
}

// @brief ノードの入出力の関係を表す CNF 式を作る．
// @param[in] node 対象のノード
// @return 割り当てが矛盾を起こしたら false を返す．
bool
GbmSatEngine::make_node_cnf(const RcfNode* node)
{
  if ( node->is_input() ) {
    return true;
  }

  GbmLit output = mNodeVarArray[node->id()];

  ymuint ni = node->fanin_num();
  vector<GbmLit> inputs(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    RcfNodeHandle ih = node->fanin(i);
    inputs[i] = handle_to_lit(ih);
  }

  bool stat = false;
  if ( node->is_and() ) {
    stat = make_AND(inputs, output);
  }
  else if ( node->is_lut() ) {
    ymuint ni_exp = 1U << ni;
    vector<GbmLit> lut_vars(ni_exp);
    ymuint conf0 = node->conf_base();
    for (ymuint i = 0; i < ni_exp; ++ i) {
      lut_vars[i] = GbmLit(mConfVarArray[conf0 + i]);
    }
    stat = make_LUT(inputs, lut_vars, output);
  }
  else if ( node->is_mux() ) {
    ymuint ns = node->conf_size();
    vector<GbmLit> s_vars(ns);
    ymuint s0 = node->conf_base();
    for (ymuint i = 0; i < ns; ++ i) {
      s_vars[i] = GbmLit(mConfVarArray[s0 + i]);
    }
    stat = make_MUX(inputs, s_vars, output);
  }

  return stat;
}

// @brief AND ゲートを表す節を追加する．
// @param[in] input_vars 入力変数のリスト
// @param[in] output_var 出力変数
// @return 割り当てが矛盾を起こしたら false を返す．
bool
GbmSatEngine::make_AND(const vector<GbmLit>& input_vars,
		       GbmLit output_var)
{
  ymuint n = input_vars.size();
  // 入力に定数0がないかチェック
  bool have_zero = false;
  vector<GbmLit> tmp_lits;
  tmp_lits.reserve(n);
  for (ymuint i = 0; i < n; ++ i) {
    const GbmLit& var = input_vars[i];
    if ( var.is_zero() ) {
      have_zero = true;
      break;
    }
    if ( var.is_lit() ) {
      tmp_lits.push_back(var);
    }
  }
  if ( have_zero ) {
    if ( output_var.is_zero() ) {
      // なにもしない．
      return true;
    }
    if ( output_var.is_one() ) {
      // 矛盾
      return false;
    }
    // 出力は0になる．
    mSolver.add_clause(Literal(output_var.var_id(), kPolNega));
    return true;
  }

  ymuint n1 = tmp_lits.size();
  if ( output_var.is_zero() ) {
    // 出力が0なので入力のうち1つは0でなければならない．
    if ( n1 == 0 ) {
      // 矛盾
      return false;
    }
    vector<Literal> tmp_list;
    tmp_list.reserve(n1);
    for (ymuint i = 0; i < n1; ++ i) {
      const GbmLit& var = tmp_lits[i];
      tmp_list.push_back(~var.literal());
    }
    mSolver.add_clause(tmp_list);
    return true;
  }
  if ( output_var.is_one() ) {
    // 全ての入力が1でなければならない．
    for (ymuint i = 0; i < n1; ++ i) {
      const GbmLit& var = tmp_lits[i];
      mSolver.add_clause(var.literal());
    }
    return true;
  }

  // 普通に節を作る．
  vector<Literal> tmp_list;
  tmp_list.reserve(n1 + 1);
  for (ymuint i = 0; i < n1; ++ i) {
    const GbmLit& var = tmp_lits[i];
    tmp_list.push_back(~var.literal());
  }
  tmp_list.push_back(output_var.literal());
  mSolver.add_clause(tmp_list);

  for (ymuint i = 0; i < n1; ++ i) {
    const GbmLit& var = tmp_lits[i];
    mSolver.add_clause(var.literal(), ~output_var.literal());
  }
  return true;
}

// @brief LUT を表す節を追加する．
// @param[in] input_vars 入力変数のリスト
// @param[in] lut_vars LUT変数のリスト
// @param[in] output_var 出力変数
// @note lut_vars のサイズは input_vars のサイズの指数乗
// @return 割り当てが矛盾を起こしたら false を返す．
bool
GbmSatEngine::make_LUT(const vector<GbmLit>& input_vars,
		       const vector<GbmLit>& lut_vars,
		       GbmLit output_var)
{
  // 実は入力を入れ替えれて MUX で作る．
  return make_MUX(lut_vars, input_vars, output_var);
}

// @brief MUX を表す節を追加する．
// @param[in] d_vars データ入力変数のリスト
// @param[in] s_vars 選択信号変数のリスト
// @param[in] output_var 出力変数
// @note d_vars のサイズは s_vars のサイズの指数乗
// @return 割り当てが矛盾を起こしたら false を返す．
bool
GbmSatEngine::make_MUX(const vector<GbmLit>& d_vars,
		       const vector<GbmLit>& s_vars,
		       GbmLit output_var)
{
  ymuint nd = d_vars.size();
  ymuint ns = s_vars.size();
  assert_cond( (1U << ns) == nd, __FILE__, __LINE__);
  for (ymuint b = 0; b < nd; ++ b) {
    const GbmLit& dvar = d_vars[b];
    vector<Literal> tmp_list;
    tmp_list.reserve(ns + 2);
    bool have_zero = false;
    for (ymuint i = 0; i < ns; ++ i) {
      const GbmLit& svar = s_vars[i];
      if ( (1 << i) & b ) {
	if ( svar.is_zero() ) {
	  have_zero = true;
	  break;
	}
	else if ( svar.is_one() ) {
	  continue;
	}
	else {
	  tmp_list.push_back(~svar.literal());
	}
      }
      else {
	if ( svar.is_zero() ) {
	  continue;
	}
	else if ( svar.is_one() ) {
	  have_zero = true;
	  break;
	}
	else {
	  tmp_list.push_back(svar.literal());
	}
      }
    }
    if ( have_zero ) {
      continue;
    }
    if ( dvar.is_zero() ) {
      if ( output_var.is_zero() ) {
	// 無視
      }
      else if ( output_var.is_one() ) {
	// 矛盾
	return false;
      }
      else {
	tmp_list.push_back(~output_var.literal());
	mSolver.add_clause(tmp_list);
      }
    }
    else if ( dvar.is_one() ) {
      if ( output_var.is_zero() ) {
	// 矛盾
	return false;
      }
      else if ( output_var.is_one() ) {
	// 無視
      }
      else {
	tmp_list.push_back(output_var.literal());
	mSolver.add_clause(tmp_list);
      }
    }
    else {
      if ( output_var.is_zero() ) {
	tmp_list.push_back(~dvar.literal());
	mSolver.add_clause(tmp_list);
      }
      else if ( output_var.is_one() ) {
	tmp_list.push_back(dvar.literal());
	mSolver.add_clause(tmp_list);
      }
      else {
	tmp_list.push_back(~dvar.literal());
	tmp_list.push_back(output_var.literal());
	mSolver.add_clause(tmp_list);
	ymuint n = tmp_list.size();
	tmp_list[n - 2] = dvar.literal();
	tmp_list[n - 1] = ~output_var.literal();
	mSolver.add_clause(tmp_list);
      }
    }
  }
  return true;
}

// @brief RcfNodeHandle から GbmLit を作る．
// @param[in] handle ハンドル
GbmLit
GbmSatEngine::handle_to_lit(RcfNodeHandle handle)
{
  if ( handle.is_zero() ) {
    return GbmLit::make_zero();
  }
  if ( handle.is_one() ) {
    return GbmLit::make_one();
  }
  GbmLit lit = mNodeVarArray[handle.id()];
  if ( handle.inv() ) {
    lit.negate();
  }
  return lit;
}

END_NAMESPACE_YM
