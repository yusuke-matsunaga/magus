
/// @file SatSolver.sip
/// @brief SatSolver の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

class SatSolver
{
%TypeHeaderCode
#include "YmLogic/SatSolver."
using nsYm::SatSolver;
%End

public:

  /// @brief コンストラクタ
  /// @param[in] type 実装タイプを表す文字列
  /// @param[in] option オプション文字列
  SatSolver(const char* type = NULL,
	    const char* option = NULL);

  /// @brief 正しい状態のときに true を返す．
  bool
  sane() const;

  /// @brief 変数を追加する．
  /// @return 新しい変数番号を返す．
  /// @note 変数番号は 0 から始まる．
  VarId
  new_var();

  /// @brief 節を追加する．
  /// @param[in] lits リテラルのベクタ
  void
  add_clause(const vector<Literal>& lits);

  /// @brief 1項の節(リテラル)を追加する．
  void
  add_clause(Literal lit1);

  /// @brief 2項の節を追加する．
  void
  add_clause(Literal lit1,
	     Literal lit2);

  /// @brief 3項の節を追加する．
  void
  add_clause(Literal lit1,
	     Literal lit2,
	     Literal lit3);

  /// @brief 4項の節を追加する．
  void
  add_clause(Literal lit1,
	     Literal lit2,
	     Literal lit3,
	     Literal lit4);

  /// @brief 5項の節を追加する．
  void
  add_clause(Literal lit1,
	     Literal lit2,
	     Literal lit3,
	     Literal lit4,
	     Literal lit5);

  /// @brief SAT 問題を解く．
  /// @param[out] model 充足するときの値の割り当てを格納する配列．
  /// @retval kB3True  充足した．
  /// @retval kB3False 充足不能が判明した．
  /// @retval kB3X     わからなかった．
  /// @note i 番めの変数の割り当て結果は model[i] に入る．
  /// 通常は complete な割り当てなので値は true/false だけになるはず
  Bool3
  solve(vector<Bool3>& model);

  /// @brief assumption 付きの SAT 問題を解く．
  /// @param[in] assumptions あらかじめ仮定する変数の値割り当てリスト
  /// @param[out] model 充足するときの値の割り当てを格納する配列．
  /// @retval kSat 充足した．
  /// @retval kUnsat 充足不能が判明した．
  /// @retval kUndet わからなかった．
  /// @note i 番めの変数の割り当て結果は model[i] に入る．
  Bool3
  solve(const vector<Literal>& assumptions,
	vector<Bool3>& model);

  /// @brief 学習節の整理を行なう．
  void
  reduce_learnt_clause();

  /// @brief 学習節をすべて削除する．
  virtual
  void
  forget_learnt_clause();

  /// @brief 現在の内部状態を得る．
  /// @param[out] stats 状態を格納する構造体
  void
  get_stats(SatStats& stats) const;

  /// @brief 変数の数を得る．
  unsigned int
  variable_num() const;

  /// @brief 制約節の数を得る．
  unsiged int
  clause_num() const;

  /// @brief 制約節のリテラルの総数を得る．
  unsigned int
  literal_num() const;

  /// @brief conflict_limit の最大値
  /// @param[in] val 設定する値
  /// @return 以前の設定値を返す．
  unsigned long
  set_max_conflict(unsigned long val);

  /// @brief solve() 中のリスタートのたびに呼び出されるメッセージハンドラの登録
  /// @param[in] msg_handler 登録するメッセージハンドラ
  void
  reg_msg_handler(SatMsgHandler* msg_handler);

  /// @brief 時間計測機能を制御する
  void
  timer_on(bool enable);

};
