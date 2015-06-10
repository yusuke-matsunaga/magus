
/// @file SatSolver.cc
/// @brief SatSolver の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/SatSolver.h"

#include "ymsat/YmSat.h"
#include "MiniSat/SatSolverMiniSat.h"
#include "MiniSat2/SatSolverMiniSat2.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
// SatSolver
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] type 実装タイプを表す文字列
// @param[in] option オプション文字列
// @param[in] rec_out ログを記録するストリームへのポインタ
SatSolver::SatSolver(const string& type,
		     const string& option,
		     ostream* rec_out)
{
  if ( type == "minisat" ) {
    // minisat-1.4
    mImpl = new SatSolverMiniSat(option);
  }
  else if ( type == "minisat2" ) {
    // minisat-2.2
    mImpl = new SatSolverMiniSat2(option);
  }
  else {
    mImpl = new YmSat(option);
  }
  mRecOut = rec_out;
}

// @brief デストラクタ
SatSolver::~SatSolver()
{
  delete mImpl;
}

// @brief 変数を追加する．
// @param[in] decision 決定変数の時に true とする．
// @return 新しい変数番号を返す．
// @note 変数番号は 0 から始まる．
VarId
SatSolver::new_var(bool decision)
{
  VarId id = mImpl->new_var(decision);

  if ( mRecOut ) {
    *mRecOut << "N" << endl
	     << "# varid = " << id << endl;
  }

  return id;
}

// @brief 節を追加する．
// @param[in] lits リテラルのベクタ
void
SatSolver::add_clause(const vector<Literal>& lits)
{
  if ( mRecOut ) {
    *mRecOut << "A";
    for (vector<Literal>::const_iterator p = lits.begin();
	 p != lits.end(); ++ p) {
      Literal l = *p;
      put_lit(l);
    }
    *mRecOut << endl;
  }

  mImpl->add_clause(lits);
}

// @brief 1項の節を追加する．
void
SatSolver::add_clause(Literal lit1)
{
  if ( mRecOut ) {
    *mRecOut << "A";
    put_lit(lit1);
    *mRecOut << endl;
  }

  mImpl->add_clause(1, &lit1);
}

// @brief 2項の節を追加する．
void
SatSolver::add_clause(Literal lit1,
		      Literal lit2)
{
  if ( mRecOut ) {
    *mRecOut << "A";
    put_lit(lit1);
    put_lit(lit2);
    *mRecOut << endl;
  }

  mImpl->add_clause(lit1, lit2);
}

// @brief 3項の節を追加する．
void
SatSolver::add_clause(Literal lit1,
		      Literal lit2,
		      Literal lit3)
{
  if ( mRecOut ) {
    *mRecOut << "A";
    put_lit(lit1);
    put_lit(lit2);
    put_lit(lit3);
    *mRecOut << endl;
  }

  mImpl->add_clause(lit1, lit2, lit3);
}

// @brief 4項の節を追加する．
void
SatSolver::add_clause(Literal lit1,
		      Literal lit2,
		      Literal lit3,
		      Literal lit4)
{
  if ( mRecOut ) {
    *mRecOut << "A";
    put_lit(lit1);
    put_lit(lit2);
    put_lit(lit3);
    put_lit(lit4);
    *mRecOut << endl;
  }

  mImpl->add_clause(lit1, lit2, lit3, lit4);
}

// @brief 5項の節を追加する．
void
SatSolver::add_clause(Literal lit1,
		      Literal lit2,
		      Literal lit3,
		      Literal lit4,
		      Literal lit5)
{
  if ( mRecOut ) {
    *mRecOut << "A";
    put_lit(lit1);
    put_lit(lit2);
    put_lit(lit3);
    put_lit(lit4);
    put_lit(lit5);
    *mRecOut << endl;
  }

  mImpl->add_clause(lit1, lit2, lit3, lit4, lit5);
}

// @brief SAT 問題を解く．
// @param[out] model 充足するときの値の割り当てを格納する配列．
// @retval kB3True 充足した．
// @retval kB3False 充足不能が判明した．
// @retval kB3X わからなかった．
// @note i 番めの変数の割り当て結果は model[i] に入る．
Bool3
SatSolver::solve(vector<Bool3>& model)
{
  // 空の assumptions を付けて solve() を呼ぶだけ
  return solve(vector<Literal>(), model);
}

// @brief assumption 付きの SAT 問題を解く．
// @param[in] assumptions あらかじめ仮定する変数の値割り当てリスト
// @param[out] model 充足するときの値の割り当てを格納する配列．
// @retval kSat 充足した．
// @retval kUnsat 充足不能が判明した．
// @retval kUndet わからなかった．
// @note i 番めの変数の割り当て結果は model[i] に入る．
Bool3
SatSolver::solve(const vector<Literal>& assumptions,
		 vector<Bool3>& model)
{
  if ( mRecOut ) {
    *mRecOut << "S";
    for (vector<Literal>::const_iterator p = assumptions.begin();
	 p != assumptions.end(); ++ p) {
      Literal l = *p;
      put_lit(l);
    }
    *mRecOut << endl;
  }

  return mImpl->solve(assumptions, model);
}

// @brief リテラルを出力する．
void
SatSolver::put_lit(Literal lit) const
{
  ASSERT_COND( mRecOut != NULL );

  *mRecOut << " " << lit.varid();
  if ( lit.is_positive() ) {
    *mRecOut << "P";
  }
  else {
    *mRecOut << "N";
  }
}

// @brief 学習節の整理を行なう．
void
SatSolver::reduce_learnt_clause()
{
  if ( mRecOut ) {
    *mRecOut << "R" << endl;
  }

  mImpl->reduce_learnt_clause();
}

// @brief 学習節をすべて削除する．
void
SatSolver::forget_learnt_clause()
{
  if ( mRecOut ) {
    *mRecOut << "F" << endl;
  }

  mImpl->forget_learnt_clause();
}

// @brief 正しい状態のときに true を返す．
bool
SatSolver::sane() const
{
  return mImpl->sane();
}

// @brief 現在の内部状態を得る．
// @param[out] stats 状態を格納する構造体
void
SatSolver::get_stats(SatStats& stats) const
{
  mImpl->get_stats(stats);
}

// @brief 変数の数を得る．
ymuint
SatSolver::variable_num() const
{
  return mImpl->variable_num();
}

// @brief 制約節の数を得る．
ymuint
SatSolver::clause_num() const
{
  return mImpl->clause_num();
}

// @brief 制約節のリテラルの総数を得る．
ymuint
SatSolver::literal_num() const
{
  return mImpl->literal_num();
}

// @brief DIMACS 形式で制約節を出力する．
// @param[in] s 出力先のストリーム
void
SatSolver::write_DIMACS(ostream& s) const
{
  mImpl->write_DIMACS(s);
}

// @brief conflict_limit の最大値
// @param[in] val 設定する値
// @return 以前の設定値を返す．
ymuint64
SatSolver::set_max_conflict(ymuint64 val)
{
  return mImpl->set_max_conflict(val);
}

// @brief solve() 中のリスタートのたびに呼び出されるメッセージハンドラの登録
// @param[in] msg_handler 登録するメッセージハンドラ
void
SatSolver::reg_msg_handler(SatMsgHandler* msg_handler)
{
  mImpl->reg_msg_handler(msg_handler);
}

// @brief 時間計測機能を制御する
void
SatSolver::timer_on(bool enable)
{
  mImpl->timer_on(enable);
}


//////////////////////////////////////////////////////////////////////
// クラス SatSolverImpl
//
// ここでは add_clause() のバリエーションのデフォルト実装を
// 提供している．
//////////////////////////////////////////////////////////////////////

// @brief 1項の節を追加する．
void
SatSolverImpl::add_clause(Literal lit1)
{
  add_clause(1, &lit1);
}

// @brief 2項の節を追加する．
void
SatSolverImpl::add_clause(Literal lit1,
			  Literal lit2)
{
  Literal tmp_lits[2] = {lit1, lit2};
  add_clause(2, tmp_lits);
}

// @brief 3項の節を追加する．
void
SatSolverImpl::add_clause(Literal lit1,
			  Literal lit2,
			  Literal lit3)
{
  Literal tmp_lits[3] = {lit1, lit2, lit3};
  add_clause(3, tmp_lits);
}

// @brief 4項の節を追加する．
void
SatSolverImpl::add_clause(Literal lit1,
			  Literal lit2,
			  Literal lit3,
			  Literal lit4)
{
  Literal tmp_lits[4] = {lit1, lit2, lit3, lit4};
  add_clause(4, tmp_lits);
}

// @brief 5項の節を追加する．
void
SatSolverImpl::add_clause(Literal lit1,
			  Literal lit2,
			  Literal lit3,
			  Literal lit4,
			  Literal lit5)
{
  Literal tmp_lits[5] = {lit1, lit2, lit3, lit4, lit5};
  add_clause(5, tmp_lits);
}

END_NAMESPACE_YM_SAT
