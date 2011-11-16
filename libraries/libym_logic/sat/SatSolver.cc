
/// @file libym_logic/sat/SatSolver.cc
/// @brief SatSolver の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SatSolver.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/SatSolver.h"

#include "ymsat/YmSat.h"
#include "ymsat/YmSatR.h"
#include "ymsat/SatAnalyzer.h"
#include "MiniSat/SatSolverMiniSat.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
// SatSolver
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] type 実装タイプを表す文字列
// @param[in] option オプション文字列
// @param[in] outp ログを記録するストリームへのポインタ
SatSolver::SatSolver(const string& type,
		     const string& option,
		     ostream* outp)
{
  if ( type == "minisat" ) {
    mImpl = new SatSolverMiniSat;
  }
  else {
    SatAnalyzer* analyzer = SaFactory::gen_analyzer(option);
    if ( outp ) {
      mImpl = new YmSatR(*outp, analyzer);
    }
    else {
      mImpl = new YmSat(analyzer);
    }
  }
}

// @brief デストラクタ
SatSolver::~SatSolver()
{
  delete mImpl;
}

// @brief 正しい状態のときに true を返す．
bool
SatSolver::sane() const
{
  return mImpl->sane();
}

// @brief 変数を追加する．
// @return 新しい変数番号を返す．
// @note 変数番号は 0 から始まる．
VarId
SatSolver::new_var()
{
  return mImpl->new_var();
}

// @brief 節を追加する．
// @param[in] lits リテラルのベクタ
void
SatSolver::add_clause(const vector<Literal>& lits)
{
  mImpl->add_clause(lits);
}

// @brief 1項の節を追加する．
void
SatSolver::add_clause(Literal lit1)
{
  vector<Literal> tmp(1);
  tmp[0] = lit1;
  mImpl->add_clause(tmp);
}

// @brief 2項の節を追加する．
void
SatSolver::add_clause(Literal lit1,
		      Literal lit2)
{
  vector<Literal> tmp(2);
  tmp[0] = lit1;
  tmp[1] = lit2;
  mImpl->add_clause(tmp);
}

// @brief 3項の節を追加する．
void
SatSolver::add_clause(Literal lit1,
		      Literal lit2,
		      Literal lit3)
{
  vector<Literal> tmp(3);
  tmp[0] = lit1;
  tmp[1] = lit2;
  tmp[2] = lit3;
  mImpl->add_clause(tmp);
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
  return mImpl->solve(vector<Literal>(), model);
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
  return mImpl->solve(assumptions, model);
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

END_NAMESPACE_YM_SAT
