#ifndef YM_SMTLIBV2_SMTSOLVER_H
#define YM_SMTLIBV2_SMTSOLVER_H

/// @file ym_smtlibv2/SmtSolver.h
/// @brief SmtSolver のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtSolver SmtSolver.h "ym_smtlibv2/SmtSolver.h"
/// @brief SMT ソルバの雛形基底クラス
///
/// 実際にはここで定義されている仮想関数を実装したクラスを作る必要がある．
//////////////////////////////////////////////////////////////////////
class SmtSolver
{
public:

  /// @brief デストラクタ
  virtual
  ~SmtSolver() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief set-logic 文の処理を行なう．
  /// @param[in] val 引数の値
  virtual
  tSmtLibResponse
  set_logic(const ShString& val) = 0;

  /// @brief declare-sort 文の処理を行なう．
  /// @param[in] name 型名
  /// @param[in] arg_num 引数の数
  virtual
  tSmtLibResponse
  declare_sort(const ShString& name,
	       ymuint arg_num) = 0;

  /// @brief define-sort 文の処理を行なう．
  /// @param[in] name 型名
  /// @param[in] symbol_list シンボルリスト
  /// @param[in] sort 元の型
  virtual
  tSmtLibResponse
  define_sort(const ShString& name,
	      const vector<ShString>& symbol_list,
	      const SmtSort* sort) = 0;

  /// @brief declare-fun 文の処理を行なう．
  /// @param[in] name 関数名
  /// @param[in] arg_sort_list 引数のタイプのリスト
  /// @param[in] fun_sort 関数のタイプ
  virtual
  tSmtLibResponse
  declare_fun(const ShString& name,
	      const vector<const SmtSort*>& arg_sort_list,
	      const SmtSort* fun_sort) = 0;

  /// @brief define-fun 文の処理を行なう．
  /// @param[in] name 関数名
  /// @param[in] param_list パラメータリスト
  /// @param[in] fun_sort 関数のタイプを表すノード
  /// @param[in] expr 関数本体の式
  virtual
  tSmtLibResponse
  define_fun(const ShString& name,
	     const vector<pair<ShString, const SmtSort*> >& param_list,
	     const SmtSort* fun_sort,
	     const SmtTerm* expr) = 0;

  /// @brief assert 文の処理を行なう．
  /// @param[in] expr 本体の式
  virtual
  tSmtLibResponse
  assert(const SmtTerm* expr) = 0;

  /// @brief check-sat 文の処理を行なう．
  virtual
  tSmtLibResponse
  check_sat() = 0;

  /// @brief get-proof 文の処理を行なう．
  virtual
  tSmtLibResponse
  get_proof() = 0;

  /// @brief get-unsat-core 文の処理を行なう．
  virtual
  tSmtLibResponse
  get_unsat_core() = 0;

  /// @brief get-value 文の処理を行なう．
  /// @param[in] expr_list expr 式のリスト
  virtual
  tSmtLibResponse
  get_value(const vector<const SmtTerm*>& expr_list) = 0;

  /// @brief get-assignment 文の処理を行なう．
  virtual
  tSmtLibResponse
  get_assignment() = 0;

  /// @brief push 文の処理を行なう．
  /// @param[in] level プッシュするレベル
  virtual
  tSmtLibResponse
  push(ymuint level) = 0;

  /// @brief pop 文の処理を行なう．
  virtual
  tSmtLibResponse
  pop(ymuint level) = 0;

  /// @brief get-option 文の処理を行なう．
  /// @param[in] keyword オプションのキーワード
  virtual
  tSmtLibResponse
  get_option(const ShString& keyword) = 0;

  /// @brief set-option 文の処理を行なう．
  /// @param[in] keyword オプションのキーワード
  /// @param[in] attr 属性値を表すノード
  virtual
  tSmtLibResponse
  set_option(const ShString& keyword,
	     const SmtExpr* attr) = 0;

  /// @brief get-info 文の処理を行なう．
  /// @param[in] keyword オプションのキーワード
  virtual
  tSmtLibResponse
  get_info(const ShString& keyword) = 0;

  /// @brief set-info 文の処理を行なう．
  /// @param[in] keyword オプションのキーワード
  /// @param[in] attr 属性値を表すノード
  virtual
  tSmtLibResponse
  set_info(const ShString& keyword,
	   const SmtExpr* attr) = 0;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // YM_SMTLIBV2_SMTSOLVER_H
