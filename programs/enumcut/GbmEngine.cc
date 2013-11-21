
/// @file GbmEngine.cc
/// @brief GbmEngine の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "GbmEngine.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス GbmEngine
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GbmEngine::GbmEngine()
{
}

// @brief デストラクタ
GbmEngine::~GbmEngine()
{
}

// @brief AND ゲートを表す節を追加する．
// @param[in] solver SATソルバ
// @param[in] input_vars 入力変数のリスト
// @param[in] output_var 出力変数
bool
GbmEngine::make_AND(SatSolver& solver,
		    const vector<GbmLit>& input_vars,
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
    solver.add_clause(Literal(output_var.var_id(), kPolNega));
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
    solver.add_clause(tmp_list);
    return true;
  }
  if ( output_var.is_one() ) {
    // 全ての入力が1でなければならない．
    for (ymuint i = 0; i < n1; ++ i) {
      const GbmLit& var = tmp_lits[i];
      solver.add_clause(var.literal());
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
  solver.add_clause(tmp_list);

  for (ymuint i = 0; i < n1; ++ i) {
    const GbmLit& var = tmp_lits[i];
    solver.add_clause(var.literal(), ~output_var.literal());
  }
  return true;
}

// @brief LUT を表す節を追加する．
// @param[in] solver SATソルバ
// @param[in] input_vars 入力変数のリスト
// @param[in] lut_vars LUT変数のリスト
// @param[in] output_var 出力変数
// @note lut_vars のサイズは input_vars のサイズの指数乗
bool
GbmEngine::make_LUT(SatSolver& solver,
		    const vector<GbmLit>& input_vars,
		    const vector<GbmLit>& lut_vars,
		    GbmLit output_var)
{
  return make_MUX(solver, lut_vars, input_vars, output_var);
}

// @brief MUX を表す節を追加する．
// @param[in] solver SATソルバ
// @param[in] d_vars データ入力変数のリスト
// @param[in] s_vars 選択信号変数のリスト
// @param[in] output_var 出力変数
// @note d_vars のサイズは s_vars のサイズの指数乗
// @note 実は make_LUT() と同じことをやっている．
bool
GbmEngine::make_MUX(SatSolver& solver,
		    const vector<GbmLit>& d_vars,
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
	solver.add_clause(tmp_list);
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
	solver.add_clause(tmp_list);
      }
    }
    else {
      if ( output_var.is_zero() ) {
	tmp_list.push_back(~dvar.literal());
	solver.add_clause(tmp_list);
      }
      else if ( output_var.is_one() ) {
	tmp_list.push_back(dvar.literal());
	solver.add_clause(tmp_list);
      }
      else {
	tmp_list.push_back(~dvar.literal());
	tmp_list.push_back(output_var.literal());
	solver.add_clause(tmp_list);
	ymuint n = tmp_list.size();
	tmp_list[n - 2] = dvar.literal();
	tmp_list[n - 1] = ~output_var.literal();
	solver.add_clause(tmp_list);
      }
    }
  }
  return true;
}

END_NAMESPACE_YM
