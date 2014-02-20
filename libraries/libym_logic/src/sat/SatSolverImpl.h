#ifndef SATSOLVERIMPL_H
#define SATSOLVERIMPL_H

/// @file SatSolverImpl.h
/// @brief SatSolverImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "logic/sat_nsdef.h"
#include "logic/Bool3.h"
#include "logic/lexp_nsdef.h"
#include "logic/Literal.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
/// @class SatSolverImpl SatSolverImpl.h "SatSolverImpl.h"
/// @brief SatSolver の実装クラス
//////////////////////////////////////////////////////////////////////
class SatSolverImpl
{
public:

  /// @brief デストラクタ
  virtual
  ~SatSolverImpl() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 正しい状態のときに true を返す．
  virtual
  bool
  sane() const = 0;

  /// @brief 変数を追加する．
  /// @return 新しい変数番号を返す．
  /// @note 変数番号は 0 から始まる．
  virtual
  VarId
  new_var() = 0;

  /// @brief 節を追加する．
  /// @param[in] lits リテラルのベクタ
  virtual
  void
  add_clause(const vector<Literal>& lits) = 0;

  /// @brief 節を追加する．
  /// @param[in] lit_num リテラル数
  /// @param[in] lits リテラルの配列
  virtual
  void
  add_clause(ymuint lit_num,
	     Literal* lits) = 0;

  /// @brief SAT 問題を解く．
  /// @param[in] assumptions あらかじめ仮定する変数の値割り当てリスト
  /// @param[out] model 充足するときの値の割り当てを格納する配列．
  /// @retval kB3True 充足した．
  /// @retval kB3False 充足不能が判明した．
  /// @retval kB3X わからなかった．
  /// @note i 番めの変数の割り当て結果は model[i] に入る．
  virtual
  Bool3
  solve(const vector<Literal>& assumptions,
	vector<Bool3>& model) = 0;

  /// @brief 学習節の整理を行なう．
  virtual
  void
  reduce_learnt_clause() = 0;

  /// @brief 現在の内部状態を得る．
  /// @param[out] stats 状態を格納する構造体
  virtual
  void
  get_stats(SatStats& stats) const = 0;

  /// @brief 変数の数を得る．
  virtual
  ymuint
  variable_num() const = 0;

  /// @brief 制約節の数を得る．
  virtual
  ymuint
  clause_num() const = 0;

  /// @brief 制約節のリテラルの総数を得る．
  virtual
  ymuint
  literal_num() const = 0;

  /// @brief conflict_limit の最大値
  /// @param[in] val 設定する値
  /// @return 以前の設定値を返す．
  virtual
  ymuint64
  set_max_conflict(ymuint64 val) = 0;

  /// @brief solve() 中のリスタートのたびに呼び出されるメッセージハンドラの登録
  /// @param[in] msg_handler 登録するメッセージハンドラ
  virtual
  void
  reg_msg_handler(SatMsgHandler* msg_handler) = 0;

  /// @brief 時間計測機能を制御する
  virtual
  void
  timer_on(bool enable) = 0;

};

END_NAMESPACE_YM_SAT

#endif // LIBYM_LOGIC_SAT_INCLUDE_SATSOLVERIMPL_H
