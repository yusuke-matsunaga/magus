#ifndef SMTFUNVALIDATORIMPL_H
#define SMTFUNVALIDATORIMPL_H

/// @file SmtFunValidatorImpl.h
/// @brief SmtFunValidatorImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/SmtFunValidator.h"


BEGIN_NAMESPACE_YM_SMT

//////////////////////////////////////////////////////////////////////
/// @class SmtBool0Validator SmtFunValidatorImpl.h "SmtFunValidatorImpl.h"
/// @brief 引数をとらない Bool 型関数の validator
//////////////////////////////////////////////////////////////////////
class SmtBool0Validator :
  public SmtFunValidator
{

public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力の型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < max_input_num() )
  virtual
  tSmtSortId
  input_sort(ymuint pos) const;

  /// @brief 出力の型を返す．
  virtual
  tSmtSortId
  outout_sort() const;

  /// @brief 入力数が適正かチェックする．
  /// @param[in] input_num 入力数
  /// @retval true 適正だった．
  /// @retval false 入力数が合わなかった．
  virtual
  bool
  check_input_num(ymuint input_num) const;

  /// @brief 入力の型が適正かチェックする．
  /// @param[in] input_sort_list 入力の型のリスト
  /// @retval true 適正だった．
  /// @retval false 入力数もしくは型が合わなかった．
  virtual
  bool
  check_inputs(const vector<tSmtSortId>& input_sort_list) const;

  /// @brief 入力と出力の型が適正かチェックする．
  /// @param[in] input_sort_list 入力の型のリスト
  /// @param[in] output_sort 出力の型
  /// @retval true 適正だった．
  /// @retval false 入力数もしくは型が合わなかった．
  virtual
  bool
  check_all(const vector<tSmtSortId>& input_sort_list,
	    tSmtSortId output_sort) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtBool1Validator SmtFunValidatorImpl.h "SmtFunValidatorImpl.h"
/// @brief 引数が1つの Bool 型関数
//////////////////////////////////////////////////////////////////////
class SmtBool1Validator :
  public SmtFunValidator
{

public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力の型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < max_input_num() )
  virtual
  tSmtSortId
  input_sort(ymuint pos) const;

  /// @brief 出力の型を返す．
  virtual
  tSmtSortId
  outout_sort() const;

  /// @brief 入力数が適正かチェックする．
  /// @param[in] input_num 入力数
  /// @retval true 適正だった．
  /// @retval false 入力数が合わなかった．
  virtual
  bool
  check_input_num(ymuint input_num) const;

  /// @brief 入力の型が適正かチェックする．
  /// @param[in] input_sort_list 入力の型のリスト
  /// @retval true 適正だった．
  /// @retval false 入力数もしくは型が合わなかった．
  virtual
  bool
  check_inputs(const vector<tSmtSortId>& input_sort_list) const;

  /// @brief 入力と出力の型が適正かチェックする．
  /// @param[in] input_sort_list 入力の型のリスト
  /// @param[in] output_sort 出力の型
  /// @retval true 適正だった．
  /// @retval false 入力数もしくは型が合わなかった．
  virtual
  bool
  check_all(const vector<tSmtSortId>& input_sort_list,
	    tSmtSortId output_sort) const;
};


//////////////////////////////////////////////////////////////////////
/// @class SmtBool2Validator SmtFunValidatorImpl.h "SmtFunValidatorImpl.h"
/// @brief 引数が2つ(以上)の Bool 型関数
//////////////////////////////////////////////////////////////////////
class SmtBool2Validator :
  public SmtFunValidator
{

public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力の型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < max_input_num() )
  virtual
  tSmtSortId
  input_sort(ymuint pos) const;

  /// @brief 出力の型を返す．
  virtual
  tSmtSortId
  outout_sort() const;

  /// @brief 入力数が適正かチェックする．
  /// @param[in] input_num 入力数
  /// @retval true 適正だった．
  /// @retval false 入力数が合わなかった．
  virtual
  bool
  check_input_num(ymuint input_num) const;

  /// @brief 入力の型が適正かチェックする．
  /// @param[in] input_sort_list 入力の型のリスト
  /// @retval true 適正だった．
  /// @retval false 入力数もしくは型が合わなかった．
  virtual
  bool
  check_inputs(const vector<tSmtSortId>& input_sort_list) const;

  /// @brief 入力と出力の型が適正かチェックする．
  /// @param[in] input_sort_list 入力の型のリスト
  /// @param[in] output_sort 出力の型
  /// @retval true 適正だった．
  /// @retval false 入力数もしくは型が合わなかった．
  virtual
  bool
  check_all(const vector<tSmtSortId>& input_sort_list,
	    tSmtSortId output_sort) const;
};


//////////////////////////////////////////////////////////////////////
/// @class SmtEqValidator SmtFunValidatorImpl.h "SmtFunValidatorImpl.h"
/// @brief eq/distinct タイプの関数
//////////////////////////////////////////////////////////////////////
class SmtEqValidator :
  public SmtFunValidator
{

public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力の型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < max_input_num() )
  virtual
  tSmtSortId
  input_sort(ymuint pos) const;

  /// @brief 出力の型を返す．
  virtual
  tSmtSortId
  outout_sort() const;

  /// @brief 入力数が適正かチェックする．
  /// @param[in] input_num 入力数
  /// @retval true 適正だった．
  /// @retval false 入力数が合わなかった．
  virtual
  bool
  check_input_num(ymuint input_num) const;

  /// @brief 入力の型が適正かチェックする．
  /// @param[in] input_sort_list 入力の型のリスト
  /// @retval true 適正だった．
  /// @retval false 入力数もしくは型が合わなかった．
  virtual
  bool
  check_inputs(const vector<tSmtSortId>& input_sort_list) const;

  /// @brief 入力と出力の型が適正かチェックする．
  /// @param[in] input_sort_list 入力の型のリスト
  /// @param[in] output_sort 出力の型
  /// @retval true 適正だった．
  /// @retval false 入力数もしくは型が合わなかった．
  virtual
  bool
  check_all(const vector<tSmtSortId>& input_sort_list,
	    tSmtSortId output_sort) const;
};


//////////////////////////////////////////////////////////////////////
/// @class SmtIteValidator SmtFunValidatorImpl.h "SmtFunValidatorImpl.h"
/// @brief ITE関数
//////////////////////////////////////////////////////////////////////
class SmtIteValidator :
  public SmtFunValidator
{

public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力の型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < max_input_num() )
  virtual
  tSmtSortId
  input_sort(ymuint pos) const;

  /// @brief 出力の型を返す．
  virtual
  tSmtSortId
  outout_sort() const;

  /// @brief 入力数が適正かチェックする．
  /// @param[in] input_num 入力数
  /// @retval true 適正だった．
  /// @retval false 入力数が合わなかった．
  virtual
  bool
  check_input_num(ymuint input_num) const;

  /// @brief 入力の型が適正かチェックする．
  /// @param[in] input_sort_list 入力の型のリスト
  /// @retval true 適正だった．
  /// @retval false 入力数もしくは型が合わなかった．
  virtual
  bool
  check_inputs(const vector<tSmtSortId>& input_sort_list) const;

  /// @brief 入力と出力の型が適正かチェックする．
  /// @param[in] input_sort_list 入力の型のリスト
  /// @param[in] output_sort 出力の型
  /// @retval true 適正だった．
  /// @retval false 入力数もしくは型が合わなかった．
  virtual
  bool
  check_all(const vector<tSmtSortId>& input_sort_list,
	    tSmtSortId output_sort) const;
};


//////////////////////////////////////////////////////////////////////
/// @class SmtUminusValidator SmtFunValidatorImpl.h "SmtFunValidatorImpl.h"
/// @brief 単項マイナス演算
//////////////////////////////////////////////////////////////////////
class SmtUminusValidator :
  public SmtFunValidator
{

public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力の型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < max_input_num() )
  virtual
  tSmtSortId
  input_sort(ymuint pos) const;

  /// @brief 出力の型を返す．
  virtual
  tSmtSortId
  outout_sort() const;

  /// @brief 入力数が適正かチェックする．
  /// @param[in] input_num 入力数
  /// @retval true 適正だった．
  /// @retval false 入力数が合わなかった．
  virtual
  bool
  check_input_num(ymuint input_num) const;

  /// @brief 入力の型が適正かチェックする．
  /// @param[in] input_sort_list 入力の型のリスト
  /// @retval true 適正だった．
  /// @retval false 入力数もしくは型が合わなかった．
  virtual
  bool
  check_inputs(const vector<tSmtSortId>& input_sort_list) const;

  /// @brief 入力と出力の型が適正かチェックする．
  /// @param[in] input_sort_list 入力の型のリスト
  /// @param[in] output_sort 出力の型
  /// @retval true 適正だった．
  /// @retval false 入力数もしくは型が合わなかった．
  virtual
  bool
  check_all(const vector<tSmtSortId>& input_sort_list,
	    tSmtSortId output_sort) const;
};


//////////////////////////////////////////////////////////////////////
/// @class SmtArithValidator SmtFunValidatorImpl.h "SmtFunValidatorImpl.h"
/// @brief 算術演算タイプの関数(除く除算)
//////////////////////////////////////////////////////////////////////
class SmtArithValidator :
  public SmtFunValidator
{

public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力の型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < max_input_num() )
  virtual
  tSmtSortId
  input_sort(ymuint pos) const;

  /// @brief 出力の型を返す．
  virtual
  tSmtSortId
  outout_sort() const;

  /// @brief 入力数が適正かチェックする．
  /// @param[in] input_num 入力数
  /// @retval true 適正だった．
  /// @retval false 入力数が合わなかった．
  virtual
  bool
  check_input_num(ymuint input_num) const;

  /// @brief 入力の型が適正かチェックする．
  /// @param[in] input_sort_list 入力の型のリスト
  /// @retval true 適正だった．
  /// @retval false 入力数もしくは型が合わなかった．
  virtual
  bool
  check_inputs(const vector<tSmtSortId>& input_sort_list) const;

  /// @brief 入力と出力の型が適正かチェックする．
  /// @param[in] input_sort_list 入力の型のリスト
  /// @param[in] output_sort 出力の型
  /// @retval true 適正だった．
  /// @retval false 入力数もしくは型が合わなかった．
  virtual
  bool
  check_all(const vector<tSmtSortId>& input_sort_list,
	    tSmtSortId output_sort) const;
};


//////////////////////////////////////////////////////////////////////
/// @class SmtDivValidator SmtFunValidatorImpl.h "SmtFunValidatorImpl.h"
/// @brief 除算
//////////////////////////////////////////////////////////////////////
class SmtDivValidator :
  public SmtFunValidator
{

public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力の型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < max_input_num() )
  virtual
  tSmtSortId
  input_sort(ymuint pos) const;

  /// @brief 出力の型を返す．
  virtual
  tSmtSortId
  outout_sort() const;

  /// @brief 入力数が適正かチェックする．
  /// @param[in] input_num 入力数
  /// @retval true 適正だった．
  /// @retval false 入力数が合わなかった．
  virtual
  bool
  check_input_num(ymuint input_num) const;

  /// @brief 入力の型が適正かチェックする．
  /// @param[in] input_sort_list 入力の型のリスト
  /// @retval true 適正だった．
  /// @retval false 入力数もしくは型が合わなかった．
  virtual
  bool
  check_inputs(const vector<tSmtSortId>& input_sort_list) const;

  /// @brief 入力と出力の型が適正かチェックする．
  /// @param[in] input_sort_list 入力の型のリスト
  /// @param[in] output_sort 出力の型
  /// @retval true 適正だった．
  /// @retval false 入力数もしくは型が合わなかった．
  virtual
  bool
  check_all(const vector<tSmtSortId>& input_sort_list,
	    tSmtSortId output_sort) const;
};


//////////////////////////////////////////////////////////////////////
/// @class SmtCompValidator SmtFunValidatorImpl.h "SmtFunValidatorImpl.h"
/// @brief 比較演算タイプの関数
//////////////////////////////////////////////////////////////////////
class SmtCompValidator :
  public SmtFunValidator
{

public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力の型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < max_input_num() )
  virtual
  tSmtSortId
  input_sort(ymuint pos) const;

  /// @brief 出力の型を返す．
  virtual
  tSmtSortId
  outout_sort() const;

  /// @brief 入力数が適正かチェックする．
  /// @param[in] input_num 入力数
  /// @retval true 適正だった．
  /// @retval false 入力数が合わなかった．
  virtual
  bool
  check_input_num(ymuint input_num) const;

  /// @brief 入力の型が適正かチェックする．
  /// @param[in] input_sort_list 入力の型のリスト
  /// @retval true 適正だった．
  /// @retval false 入力数もしくは型が合わなかった．
  virtual
  bool
  check_inputs(const vector<tSmtSortId>& input_sort_list) const;

  /// @brief 入力と出力の型が適正かチェックする．
  /// @param[in] input_sort_list 入力の型のリスト
  /// @param[in] output_sort 出力の型
  /// @retval true 適正だった．
  /// @retval false 入力数もしくは型が合わなかった．
  virtual
  bool
  check_all(const vector<tSmtSortId>& input_sort_list,
	    tSmtSortId output_sort) const;
};

END_NAMESPACE_YM_SMT

#endif // SMTFUNVALIDATORIMPL_H
