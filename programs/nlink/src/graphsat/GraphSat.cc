
/// @file GraphSat.cc
/// @brief GraphSat の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "GraphSat.h"

#include "GraphSatImpl.h"
#include "GraphSatR.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
// GraphSat
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] type 実装タイプを表す文字列
// @param[in] option オプション文字列
// @param[in] outp ログを記録するストリームへのポインタ
GraphSat::GraphSat(const string& type,
		   const string& option,
		   ostream* outp)
{
  if ( outp ) {
    mImpl = new GraphSatR(*outp, option);
  }
  else {
    mImpl = new GraphSatImpl(option);
  }
}

// @brief デストラクタ
GraphSat::~GraphSat()
{
  delete mImpl;
}

// @brief 正しい状態のときに true を返す．
bool
GraphSat::sane() const
{
  return mImpl->sane();
}

// @brief 変数を追加する．
// @return 新しい変数番号を返す．
// @note 変数番号は 0 から始まる．
VarId
GraphSat::new_var()
{
  return mImpl->new_var();
}

// @brief 節を追加する．
// @param[in] lits リテラルのベクタ
void
GraphSat::add_clause(const vector<Literal>& lits)
{
  mImpl->add_clause(lits);
}

// @brief 1項の節を追加する．
void
GraphSat::add_clause(Literal lit1)
{
  mImpl->add_clause(1, &lit1);
}

// @brief 2項の節を追加する．
void
GraphSat::add_clause(Literal lit1,
		     Literal lit2)
{
  mImpl->add_clause(lit1, lit2);
}

// @brief 3項の節を追加する．
void
GraphSat::add_clause(Literal lit1,
		     Literal lit2,
		     Literal lit3)
{
  mImpl->add_clause(lit1, lit2, lit3);
}

// @brief 4項の節を追加する．
void
GraphSat::add_clause(Literal lit1,
		     Literal lit2,
		     Literal lit3,
		     Literal lit4)
{
  mImpl->add_clause(lit1, lit2, lit3, lit4);
}

// @brief 5項の節を追加する．
void
GraphSat::add_clause(Literal lit1,
		     Literal lit2,
		     Literal lit3,
		     Literal lit4,
		     Literal lit5)
{
  mImpl->add_clause(lit1, lit2, lit3, lit4, lit5);
}

// @brief グラフを追加する．
void
GraphSat::add_graph(const GsGraphBuilder& graph_src)
{
  mImpl->add_graph(graph_src);
}

// @brief SAT 問題を解く．
// @param[out] model 充足するときの値の割り当てを格納する配列．
// @retval kB3True 充足した．
// @retval kB3False 充足不能が判明した．
// @retval kB3X わからなかった．
// @note i 番めの変数の割り当て結果は model[i] に入る．
Bool3
GraphSat::solve(vector<Bool3>& model)
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
GraphSat::solve(const vector<Literal>& assumptions,
		vector<Bool3>& model)
{
  return mImpl->solve(assumptions, model);
}

// @brief 学習節の整理を行なう．
void
GraphSat::reduce_learnt_clause()
{
  mImpl->reduce_learnt_clause();
}

// @brief 学習節をすべて削除する．
void
GraphSat::forget_learnt_clause()
{
  mImpl->forget_learnt_clause();
}

// @brief 現在の内部状態を得る．
// @param[out] stats 状態を格納する構造体
void
GraphSat::get_stats(SatStats& stats) const
{
  mImpl->get_stats(stats);
}

// @brief 変数の数を得る．
ymuint
GraphSat::variable_num() const
{
  return mImpl->variable_num();
}

// @brief 制約節の数を得る．
ymuint
GraphSat::clause_num() const
{
  return mImpl->clause_num();
}

// @brief 制約節のリテラルの総数を得る．
ymuint
GraphSat::literal_num() const
{
  return mImpl->literal_num();
}

// @brief conflict_limit の最大値
// @param[in] val 設定する値
// @return 以前の設定値を返す．
ymuint64
GraphSat::set_max_conflict(ymuint64 val)
{
  return mImpl->set_max_conflict(val);
}

// @brief solve() 中のリスタートのたびに呼び出されるメッセージハンドラの登録
// @param[in] msg_handler 登録するメッセージハンドラ
void
GraphSat::reg_msg_handler(SatMsgHandler* msg_handler)
{
  mImpl->reg_msg_handler(msg_handler);
}

// @brief 時間計測機能を制御する
void
GraphSat::timer_on(bool enable)
{
  mImpl->timer_on(enable);
}

END_NAMESPACE_YM_SAT
