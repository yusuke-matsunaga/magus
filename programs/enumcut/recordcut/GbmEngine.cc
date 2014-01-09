
/// @file GbmEngine.cc
/// @brief GbmEngine の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "GbmEngine.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

const bool debug = false;

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス GbmEngine
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] solver SATソルバ
// @param[in] node_num ノード数
// @param[in] conf_num 設定変数の数
GbmEngine::GbmEngine(SatSolver& solver,
		     ymuint node_num,
		     ymuint conf_num) :
  mSolver(solver),
  mNodeVarArray(node_num),
  mConfVarArray(conf_num)
{
}

// @brief デストラクタ
GbmEngine::~GbmEngine()
{
}

// @brief ノードに対応するリテラルを登録する．
// @param[in] id ノード番号
// @param[in] lit リテラル
void
GbmEngine::set_node_var(ymuint id,
			GbmLit lit)
{
  assert_cond( id < mNodeVarArray.size(), __FILE__, __LINE__);
  mNodeVarArray[id] = lit;
}

// @brief 設定変数に対応するリテラルを登録する．
// @param[in] id 変数番号
// @param[in] lit リテラル
void
GbmEngine::set_conf_var(ymuint id,
			GbmLit lit)
{
  assert_cond( id < mConfVarArray.size(), __FILE__, __LINE__);
  mConfVarArray[id] = lit;
}

// @brief 外部入力の順列入れ替えを2値符号化する場合の CNF 式を作る．
// @param[in] input_list 入力ノードのリスト
// @param[in] iorder_vid_array 入力順を表す変数の配列
// @param[in] bw 1つの入力の符号化に用いるビット長
// @param[in] bit_pat 外部入力の割り当てを表すビットパタン
void
GbmEngine::make_inputs_cnf_binary(const vector<const RcfNode*>& input_list,
				  const vector<VarId> iorder_vid_array,
				  ymuint bw,
				  ymuint bit_pat)
{
  ymuint ni = input_list.size();
  for (ymuint i = 0; i < ni; ++ i) {
    const RcfNode* node = input_list[i];
    ymuint id = node->id();
    VarId vid = mSolver.new_var();
    if ( debug ) {
      cout << " lut_input#" << i << ": " << vid << endl;
    }
    set_node_var(id, GbmLit(vid));

      // 入力と外部入力の間の関係式を作る．
    vector<Literal> tmp_lits(bw + 1);
    for (ymuint j = 0; j < ni; ++ j) {
      for (ymuint k = 0; k < bw; ++ k) {
	VarId kvar = iorder_vid_array[i * bw + k];
	// こちらは含意の左辺なので否定する．
	if ( j & (1U << k) ) {
	  tmp_lits[k] = Literal(kvar, kPolNega);
	}
	else {
	  tmp_lits[k] = Literal(kvar, kPolPosi);
	}
      }
      if ( bit_pat & (1U << j) ) {
	tmp_lits[bw] = Literal(vid, kPolPosi);
      }
      else {
	tmp_lits[bw] = Literal(vid, kPolNega);
      }
      if ( debug ) {
	cout << " added clause = ";
	for (ymuint x = 0; x <= bw; ++ x) {
	  cout << " " << tmp_lits[x];
	}
	cout << endl;
      }
      mSolver.add_clause(tmp_lits);
    }
    // 使っていない変数の組み合わせを禁止する．
    vector<Literal> tmp_lits2(bw);
    for (ymuint j = ni; j < (1U << bw); ++ j) {
      for (ymuint k = 0; k < bw; ++ k) {
	VarId kvar = iorder_vid_array[i * bw + k];
	if ( j & (1U << k) ) {
	  tmp_lits2[k] = Literal(kvar, kPolNega);
	}
	else {
	  tmp_lits2[k] = Literal(kvar, kPolPosi);
	}
      }
      if ( debug ) {
	cout << " added clause = ";
	for (ymuint x = 0; x < bw; ++ x) {
	  cout << " " << tmp_lits2[x];
	}
	cout << endl;
      }
      mSolver.add_clause(tmp_lits2);
    }
#if 0
    // 異なる LUT 入力におなじ入力が接続してはいけないというルール
    for (ymuint j = 0; j < ni; ++ j) {
      vector<Literal> tmp_lits3(bw * 2);
      for (ymuint k = 0; k < i; ++ k) {
	for (ymuint l = 0; l < bw; ++ l) {
	  tPol pol = ( j & (1U << l) ) ? kPolNega : kPolPosi;
	  tmp_lits3[l] = Literal(iorder_vid_array[k * bw + l], pol);
	  tmp_lits3[l + bw] = Literal(iorder_vid_array[i * bw + l], pol);
	}
	if ( debug ) {
	  cout << " added clause = ";
	  for (ymuint x = 0; x < bw; ++ x) {
	    cout << " " << tmp_lits3[x];
	  }
	  cout << endl;
	}
	mSolver.add_clause(tmp_lits3);
      }
    }
    // 対称性を考慮したルール
    ymuint pred;
    if ( network.get_pred(i, pred) ) {
      for (ymuint j = 0; j < ni; ++ j) {
	vector<Literal> tmp_lits3(bw * 2);
	for (ymuint l = 0; l < m; ++ l) {
	  tPol pol = ( j & (1U << l) ) ? kPolNega : kPolPosi;
	  tmp_lits3[l] = Literal(iorder_vid_array[i * bw + l], pol);
	}
	for (ymuint k = j + 1; k < ni; ++ k) {
	  for (ymuint l = 0; l < bw; ++ l) {
	    tPol pol = ( j & (1U << l) ) ? kPolNega : kPolPosi;
	    tmp_lits3[l + bw] = Literal(iorder_vid_array[pred * bw + l], pol);
	  }
	  if ( debug ) {
	    cout << " added clause = ";
	    for (ymuint x = 0; x < bw; ++ x) {
	      cout << " " << tmp_lits3[x];
	    }
	    cout << endl;
	  }
	  mSolver.add_clause(tmp_lits3);
	}
      }
    }
#endif
  }
}

// @brief 外部入力の順列入れ替えをone-hot符号化する場合の CNF 式を作る．
// @param[in] input_list 入力ノードのリスト
// @param[in] pred_list 各入力に対する先行者の情報を入れた配列
// @param[in] iorder_vid_array 入力順を表す変数の配列
// @param[in] bit_pat 外部入力の割り当てを表すビットパタン
void
GbmEngine::make_inputs_cnf_onehot(const vector<const RcfNode*>& input_list,
				  const vector<int>& pred_list,
				  const vector<VarId> iorder_vid_array,
				  ymuint bit_pat)
{
  ymuint ni = input_list.size();
  for (ymuint i = 0; i < ni; ++ i) {
    const RcfNode* node = input_list[i];
    ymuint id = node->id();
    VarId vid = mSolver.new_var();
    if ( debug ) {
      cout << " lut_input#" << i << ": " << vid << endl;
    }
    set_node_var(id, GbmLit(vid));

    // 入力と外部入力の間の関係式を作る．
    for (ymuint j = 0; j < ni; ++ j) {
      Literal lit0(iorder_vid_array[i * ni + j], kPolNega);
      tPol pol = ( bit_pat & (1U << j) ) ? kPolPosi : kPolNega;
      Literal lit1(vid, pol);
      if ( debug ) {
	cout << " added clause = " << lit0 << " " << lit1 << endl;
      }
      mSolver.add_clause(lit0, lit1);
    }
    // 2つの変数が同時に true になってはいけないというルール
    for (ymuint j = 0; j < ni; ++ j) {
      Literal lit0(iorder_vid_array[i * ni + j], kPolNega);
      for (ymuint k = j + 1; k < ni; ++ k) {
	Literal lit1(iorder_vid_array[i * ni + k], kPolNega);
	if ( debug ) {
	  cout << " added clause = " << lit0 << " " << lit1 << endl;
	}
	mSolver.add_clause(lit0, lit1);
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
    mSolver.add_clause(tmp_lits);
    // 異なる LUT 入力におなじ入力が接続してはいけないというルール
    for (ymuint j = 0; j < ni; ++ j) {
      Literal lit0(iorder_vid_array[i * ni + j], kPolNega);
      for (ymuint k = 0; k < i; ++ k) {
	Literal lit1(iorder_vid_array[k * ni + j], kPolNega);
	if ( debug ) {
	  cout << " added clause = " << lit0 << " " << lit1 << endl;
	}
	mSolver.add_clause(lit0, lit1);
      }
    }

    // 対称性を考慮したルール
    int pred = pred_list[i];
    if ( pred >= 0 ) {
      for (ymuint j = 0; j < ni; ++ j) {
	Literal lit0(iorder_vid_array[i * ni + j], kPolNega);
	for (ymuint k = j + 1; k < ni; ++ k) {
	  Literal lit1(iorder_vid_array[pred * ni + k], kPolNega);
	  if ( debug ) {
	    cout << " added clause = " << lit0 << " " << lit1 << endl;
	  }
	  mSolver.add_clause(lit0, lit1);
	}
      }
    }
  }
}

// @brief 内部ノードに変数番号を割り当て，CNF式を作る．
// @param[in] node_list ノードリスト(入力と出力は含まない)
// @param[in] oid 出力のノード番号
// @param[in] oval 出力値
// @return 割り当てが矛盾を起こしたら false を返す．
bool
GbmEngine::make_nodes_cnf(const vector<const RcfNode*>& node_list,
			  ymuint oid,
			  ymuint oval)
{
  // 内部のノードに変数番号を割り当てる．
  for (vector<const RcfNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    const RcfNode* node = *p;
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
      VarId vid = mSolver.new_var();
      set_node_var(node->id(), GbmLit(vid));
    }
  }
  // 内部のノードに対する CNF 式を作る．
  for (vector<const RcfNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    const RcfNode* node = *p;
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
GbmEngine::make_node_cnf(const RcfNode* node)
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
      lut_vars[i] = mConfVarArray[conf0 + i];
    }
    stat = make_LUT(inputs, lut_vars, output);
  }
  else if ( node->is_mux() ) {
    ymuint ns = node->conf_size();
    vector<GbmLit> s_vars(ns);
    ymuint s0 = node->conf_base();
    for (ymuint i = 0; i < ns; ++ i) {
      s_vars[i] = mConfVarArray[s0 + i];
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
GbmEngine::make_AND(const vector<GbmLit>& input_vars,
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
GbmEngine::make_LUT(const vector<GbmLit>& input_vars,
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
GbmEngine::make_MUX(const vector<GbmLit>& d_vars,
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
GbmEngine::handle_to_lit(RcfNodeHandle handle)
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
