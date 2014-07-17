
/// @file SatSolverMiniSat2.cc
/// @brief SatSolverMiniSat2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SatSolverMiniSat2.h"
#include "logic/SatStats.h"


BEGIN_NAMESPACE_YM_SAT

using namespace Minisat;

BEGIN_NONAMESPACE

inline
Lit
literal2lit(Literal l)
{
  return mkLit(static_cast<Var>(l.varid().val()), l.is_negative());
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// SatSolverMiniSat2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] option オプション文字列
SatSolverMiniSat2::SatSolverMiniSat2(const string& option)
{
  if ( option == "verbose" ) {
    mSolver.verbosity = 1;
  }
}

// @brief デストラクタ
SatSolverMiniSat2::~SatSolverMiniSat2()
{
}

// @brief 正しい状態のときに true を返す．
bool
SatSolverMiniSat2::sane() const
{
  return mSolver.okay();
}

// @brief 変数を追加する．
// @return 新しい変数番号を返す．
// @note 変数番号は 0 から始まる．
VarId
SatSolverMiniSat2::new_var()
{
  return VarId(mSolver.newVar(true));
}

// @brief 節を追加する．
// @param[in] lits リテラルのベクタ
void
SatSolverMiniSat2::add_clause(const vector<Literal>& lits)
{
  vec<Lit> tmp;
  for (vector<Literal>::const_iterator p = lits.begin();
       p != lits.end(); ++ p) {
    Literal l = *p;
    Lit lit = literal2lit(l);
    tmp.push(lit);
  }
  mSolver.addClause_(tmp);
}

// @brief 節を追加する．
// @param[in] lit_num リテラル数
// @param[in] lits リテラルの配列
void
SatSolverMiniSat2::add_clause(ymuint lit_num,
			      const Literal* lits)
{
  vec<Lit> tmp;
  for (ymuint i = 0; i < lit_num; ++ i) {
    Literal l = lits[i];
    Lit lit = literal2lit(l);
    tmp.push(lit);
  }
  mSolver.addClause_(tmp);
}

// @brief 1項の節(リテラル)を追加する．
void
SatSolverMiniSat2::add_clause(Literal lit1)
{
  mSolver.addClause(literal2lit(lit1));
}

// @brief 2項の節を追加する．
void
SatSolverMiniSat2::add_clause(Literal lit1,
			      Literal lit2)
{
  mSolver.addClause(literal2lit(lit1), literal2lit(lit2));
}

// @brief 3項の節を追加する．
void
SatSolverMiniSat2::add_clause(Literal lit1,
			      Literal lit2,
			      Literal lit3)
{
  mSolver.addClause(literal2lit(lit1), literal2lit(lit2), literal2lit(lit3));
}

// @brief SAT 問題を解く．
// @param[in] assumptions あらかじめ仮定する変数の値割り当てリスト
// @param[out] model 充足するときの値の割り当てを格納する配列．
// @retval kB3True 充足した．
// @retval kB3False 充足不能が判明した．
// @retval kB3X わからなかった．
// @note i 番めの変数の割り当て結果は model[i] に入る．
Bool3
SatSolverMiniSat2::solve(const vector<Literal>& assumptions,
			 vector<Bool3>& model)
{
  vec<Lit> tmp;
  for (vector<Literal>::const_iterator p = assumptions.begin();
       p != assumptions.end(); ++ p) {
    Literal l = *p;
    Lit lit = literal2lit(l);
    tmp.push(lit);
  }
  bool ans = mSolver.solve(tmp);
  if ( ans ) {
    ymuint n = mSolver.model.size();
    model.resize(n);
    for (ymuint i = 0; i < n; ++ i) {
      lbool lb = mSolver.model[i];
      if ( lb == l_True ) {
	model[i] = kB3True;
      }
      else if ( lb == l_False ) {
	model[i] = kB3False;
      }
      else {
	model[i] = kB3X;
      }
    }
    return kB3True;
  }
  return kB3False;
}

// @brief 学習節の整理を行なう．
void
SatSolverMiniSat2::reduce_learnt_clause()
{
  // 未実装
}

// @brief 学習節をすべて削除する．
void
SatSolverMiniSat2::forget_learnt_clause()
{
  // 未実装
}

// @brief conflict_limit の最大値
// @param[in] val 設定する値
// @return 以前の設定値を返す．
ymuint64
SatSolverMiniSat2::set_max_conflict(ymuint64 val)
{
  // 無効
  return 0;
}

// @brief 現在の内部状態を得る．
// @param[out] stats 状態を格納する構造体
void
SatSolverMiniSat2::get_stats(SatStats& stats) const
{
  stats.mRestart = mSolver.starts;
  stats.mVarNum = mSolver.nVars();
  stats.mConstrClauseNum = mSolver.nClauses();
  stats.mConstrLitNum = mSolver.clauses_literals;
  stats.mLearntClauseNum = mSolver.nLearnts();
  stats.mLearntLitNum = mSolver.learnts_literals;
  stats.mConflictNum = mSolver.conflicts;
  stats.mDecisionNum = mSolver.decisions;
  stats.mPropagationNum = mSolver.propagations;
  stats.mConflictLimit = 0;
  stats.mLearntLimit = 0;
}

// @brief 変数の数を得る．
ymuint
SatSolverMiniSat2::variable_num() const
{
  return mSolver.nVars();
}

// @brief 制約節の数を得る．
ymuint
SatSolverMiniSat2::clause_num() const
{
  return mSolver.nClauses();
}

// @brief 制約節のリテラルの総数を得る．
ymuint
SatSolverMiniSat2::literal_num() const
{
  return mSolver.clauses_literals;
}

// @brief solve() 中のリスタートのたびに呼び出されるメッセージハンドラの登録
// @param[in] msg_handler 登録するメッセージハンドラ
void
SatSolverMiniSat2::reg_msg_handler(SatMsgHandler* msg_handler)
{
}

// @brief 時間計測機能を制御する
void
SatSolverMiniSat2::timer_on(bool enable)
{
}

END_NAMESPACE_YM_SAT
