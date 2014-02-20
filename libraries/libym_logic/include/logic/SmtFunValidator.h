#ifndef LOGIC_SMTFUNVALIDATOR_H
#define LOGIC_SMTFUNVALIDATOR_H

/// @file logic/SmtFunValidator.h
/// @brief SmtFunValidator のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "logic/smt_nsdef.h"


BEGIN_NAMESPACE_YM_SMT

//////////////////////////////////////////////////////////////////////
/// @class SmtFunValidator SmtFunValidator.h "logic/SmtFunValidator.h"
/// @brief 組み込み関数の型をチェックするクラス
//////////////////////////////////////////////////////////////////////
class SmtFunValidator
{
public:

  /// @brief デストラクタ
  virtual
  ~SmtFunValidator() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力の型を返す．
  /// @param[in] pos 位置番号
  virtual
  tSmtSortId
  input_sort(ymuint pos) const = 0;

  /// @brief 出力の型を返す．
  virtual
  tSmtSortId
  outout_sort() const = 0;

  /// @brief 入力数が適正かチェックする．
  /// @param[in] input_num 入力数
  /// @retval true 適正だった．
  /// @retval false 入力数が合わなかった．
  virtual
  bool
  check_input_num(ymuint input_num) const = 0;

  /// @brief 入力の型が適正かチェックする．
  /// @param[in] input_sort_list 入力の型のリスト
  /// @retval true 適正だった．
  /// @retval false 入力数もしくは型が合わなかった．
  virtual
  bool
  check_inputs(const vector<tSmtSortId>& input_sort_list) const = 0;

  /// @brief 入力と出力の型が適正かチェックする．
  /// @param[in] input_sort_list 入力の型のリスト
  /// @param[in] output_sort 出力の型
  /// @retval true 適正だった．
  /// @retval false 入力数もしくは型が合わなかった．
  virtual
  bool
  check_all(const vector<tSmtSortId>& input_sort_list,
	    tSmtSortId output_sort) const = 0;

};

END_NAMESPACE_YM_SMT

#endif // LOGIC_SMTFUNVALIDATOR_H
