
/// @file SmtFunValidatorImpl.cc
/// @brief SmtFunValidator の派生クラスの実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "SmtFunValidatorImpl.h"


BEGIN_NAMESPACE_YM_SMT

//////////////////////////////////////////////////////////////////////
// クラス SmtBool0Validator
//////////////////////////////////////////////////////////////////////

// @brief 入力の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < max_input_num() )
tSmtSortId
SmtBool0Validator::input_sort(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return kSmtSort_None;
}

// @brief 出力の型を返す．
tSmtSortId
SmtBool0Validator::outout_sort() const
{
  return kSmtSort_Bool;
}

// @brief 入力数が適正化チェックする．
// @param[in] input_num 入力数
// @retval true 適正だった．
// @retval false 入力数が合わなかった．
bool
SmtBool0Validator::check_input_num(ymuint input_num) const
{
  return input_num == 0;
}

// @brief 入力の型が適正かチェックする．
// @param[in] input_sort_list 入力の型のリスト
// @retval true 適正だった．
// @retval false 入力数もしくは型が合わなかった．
bool
SmtBool0Validator::check_inputs(const vector<tSmtSortId>& input_sort_list) const
{
  return input_sort_list.empty();
}

// @brief 入力と出力の型が適正かチェックする．
// @param[in] input_sort_list 入力の型のリスト
// @param[in] output_sort 出力の型
// @retval true 適正だった．
// @retval false 入力数もしくは型が合わなかった．
bool
SmtBool0Validator::check_all(const vector<tSmtSortId>& input_sort_list,
			     tSmtSortId output_sort) const
{
  return input_sort_list.empty() && output_sort == kSmtSort_Bool;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtBool1Validator
//////////////////////////////////////////////////////////////////////

// @brief 入力の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < max_input_num() )
tSmtSortId
SmtBool1Validator::input_sort(ymuint pos) const
{
  assert_cond( pos == 0, __FILE__, __LINE__);
  return kSmtSort_Bool;
}

// @brief 出力の型を返す．
tSmtSortId
SmtBool1Validator::outout_sort() const
{
  return kSmtSort_Bool;
}

// @brief 入力数が適正かチェックする．
// @param[in] input_num 入力数
// @retval true 適正だった．
// @retval false 入力数が合わなかった．
bool
SmtBool1Validator::check_input_num(ymuint input_num) const
{
  return input_num == 1;
}

// @brief 入力の型が適正かチェックする．
// @param[in] input_sort_list 入力の型のリスト
// @retval true 適正だった．
// @retval false 入力数もしくは型が合わなかった．
bool
SmtBool1Validator::check_inputs(const vector<tSmtSortId>& input_sort_list) const
{
  if ( input_sort_list.size() != 1 ) {
    return false;
  }
  if ( input_sort_list[0] != kSmtSort_Bool ) {
    return false;
  }
  return true;
}

// @brief 入力と出力の型が適正かチェックする．
// @param[in] input_sort_list 入力の型のリスト
// @param[in] output_sort 出力の型
// @retval true 適正だった．
// @retval false 入力数もしくは型が合わなかった．
bool
SmtBool1Validator::check_all(const vector<tSmtSortId>& input_sort_list,
			     tSmtSortId output_sort) const
{
  if ( !check_inputs(input_sort_list) ) {
    return false;
  }
  if ( output_sort != kSmtSort_Bool ) {
    return false;
  }
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtBool2Validator
//////////////////////////////////////////////////////////////////////

// @brief 入力の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < max_input_num() )
tSmtSortId
SmtBool2Validator::input_sort(ymuint pos) const
{
  return kSmtSort_Bool;
}

// @brief 出力の型を返す．
tSmtSortId
SmtBool2Validator::outout_sort() const
{
  return kSmtSort_Bool;
}

// @brief 入力数が適正かチェックする．
// @param[in] input_num 入力数
// @retval true 適正だった．
// @retval false 入力数が合わなかった．
bool
SmtBool2Validator::check_input_num(ymuint input_num) const
{
  return input_num >= 2;
}

// @brief 入力の型が適正かチェックする．
// @param[in] input_sort_list 入力の型のリスト
// @retval true 適正だった．
// @retval false 入力数もしくは型が合わなかった．
bool
SmtBool2Validator::check_inputs(const vector<tSmtSortId>& input_sort_list) const
{
  ymuint n = input_sort_list.size();
  if ( n < 2 ) {
    return false;
  }
  for (ymuint i = 0; i < n; ++ i) {
    if ( input_sort_list[i] != kSmtSort_Bool ) {
      return false;
    }
  }
  return true;
}

// @brief 入力と出力の型が適正かチェックする．
// @param[in] input_sort_list 入力の型のリスト
// @param[in] output_sort 出力の型
// @retval true 適正だった．
// @retval false 入力数もしくは型が合わなかった．
bool
SmtBool2Validator::check_all(const vector<tSmtSortId>& input_sort_list,
			     tSmtSortId output_sort) const
{
  if ( !check_inputs(input_sort_list) ) {
    return false;
  }
  if ( output_sort != kSmtSort_Bool ) {
    return false;
  }
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtEqValidator
//////////////////////////////////////////////////////////////////////

// @brief 入力の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < max_input_num() )
tSmtSortId
SmtEqValidator::input_sort(ymuint pos) const
{
  return kSmtSort_Any;
}

// @brief 出力の型を返す．
tSmtSortId
SmtEqValidator::outout_sort() const
{
  return kSmtSort_Bool;
}

// @brief 入力数が適正かチェックする．
// @param[in] input_num 入力数
// @retval true 適正だった．
// @retval false 入力数が合わなかった．
bool
SmtEqValidator::check_input_num(ymuint input_num) const
{
  return input_num >= 2;
}

// @brief 入力の型が適正かチェックする．
// @param[in] input_sort_list 入力の型のリスト
// @retval true 適正だった．
// @retval false 入力数もしくは型が合わなかった．
bool
SmtEqValidator::check_inputs(const vector<tSmtSortId>& input_sort_list) const
{
  ymuint n = input_sort_list.size();
  if ( n < 2 ) {
    return false;
  }
  // 入力の型はなんでもよいが全部同じでなければならない．
  for (ymuint i = 1; i < n; ++ i) {
    if ( input_sort_list[i] != input_sort_list[0] ) {
      return false;
    }
  }
}

// @brief 入力と出力の型が適正かチェックする．
// @param[in] input_sort_list 入力の型のリスト
// @param[in] output_sort 出力の型
// @retval true 適正だった．
// @retval false 入力数もしくは型が合わなかった．
bool
SmtEqValidator::check_all(const vector<tSmtSortId>& input_sort_list,
			  tSmtSortId output_sort) const
{
  if ( !check_inputs(input_sort_list) ) {
    return false;
  }
  if ( output_sort != kSmtSort_Bool ) {
    return false;
  }
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtIteValidator
//////////////////////////////////////////////////////////////////////

// @brief 入力の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < max_input_num() )
tSmtSortId
SmtIteValidator::input_sort(ymuint pos) const
{
  assert_cond( pos < 3, __FILE__, __LINE__);
  if ( pos == 0 ) {
    // 最初は Bool 型
    return kSmtSort_Bool;
  }
  // 残りは任意
  return kSmtSort_Any;
}

// @brief 出力の型を返す．
tSmtSortId
SmtIteValidator::outout_sort() const
{
  return kSmtSort_Any;
}

// @brief 入力数が適正かチェックする．
// @param[in] input_num 入力数
// @retval true 適正だった．
// @retval false 入力数が合わなかった．
bool
SmtIteValidator::check_input_num(ymuint input_num) const
{
  return input_num == 3;
}

// @brief 入力の型が適正かチェックする．
// @param[in] input_sort_list 入力の型のリスト
// @retval true 適正だった．
// @retval false 入力数もしくは型が合わなかった．
bool
SmtIteValidator::check_inputs(const vector<tSmtSortId>& input_sort_list) const
{
  if ( input_sort_list.size() != 3 ) {
    return false;
  }
  // 最初の型は Bool 型
  if ( input_sort_list[0] != kSmtSort_Bool ) {
    return false;
  }
  // 残り2つの入力の型は等しくなければならない．
  if ( input_sort_list[1] != input_sort_list[2] ) {
    return false;
  }
  return true;
}

// @brief 入力と出力の型が適正かチェックする．
// @param[in] input_sort_list 入力の型のリスト
// @param[in] output_sort 出力の型
// @retval true 適正だった．
// @retval false 入力数もしくは型が合わなかった．
bool
SmtIteValidator::check_all(const vector<tSmtSortId>& input_sort_list,
			   tSmtSortId output_sort) const
{
  if ( !check_inputs(input_sort_list) ) {
    return false;
  }
  // 残り2つの入力と出力の型は等しくなければならない．
  if ( input_sort_list[1] != output_sort ) {
    return false;
  }
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtUminusValidator
//////////////////////////////////////////////////////////////////////

// @brief 入力の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < max_input_num() )
tSmtSortId
SmtUminusValidator::input_sort(ymuint pos) const
{
  assert_cond( pos == 0, __FILE__, __LINE__);
  return kSmtSort_Num;
}

// @brief 出力の型を返す．
tSmtSortId
SmtUminusValidator::outout_sort() const
{
  return kSmtSort_Num;
}

// @brief 入力数が適正かチェックする．
// @param[in] input_num 入力数
// @retval true 適正だった．
// @retval false 入力数が合わなかった．
bool
SmtUminusValidator::check_input_num(ymuint input_num) const
{
  return input_num == 1;
}

// @brief 入力の型が適正かチェックする．
// @param[in] input_sort_list 入力の型のリスト
// @retval true 適正だった．
// @retval false 入力数もしくは型が合わなかった．
bool
SmtUminusValidator::check_inputs(const vector<tSmtSortId>& input_sort_list) const
{
  if ( input_sort_list.size() != 1 ) {
    return false;
  }
  tSmtSortId sort0 = input_sort_list[0];
  if ( sort0 != kSmtSort_Int && sort0 != kSmtSort_Real ) {
    return false;
  }
  return true;
}

// @brief 入力と出力の型が適正かチェックする．
// @param[in] input_sort_list 入力の型のリスト
// @param[in] output_sort 出力の型
// @retval true 適正だった．
// @retval false 入力数もしくは型が合わなかった．
bool
SmtUminusValidator::check_all(const vector<tSmtSortId>& input_sort_list,
			      tSmtSortId output_sort) const
{
  if ( !check_inputs(input_sort_list) ) {
    return false;
  }
  if ( output_sort != input_sort_list[0] ) {
    return false;
  }
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtArithValidator
//////////////////////////////////////////////////////////////////////

// @brief 入力の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < max_input_num() )
tSmtSortId
SmtArithValidator::input_sort(ymuint pos) const
{
  return kSmtSort_Num;
}

// @brief 出力の型を返す．
tSmtSortId
SmtArithValidator::outout_sort() const
{
  return kSmtSort_Num;
}

// @brief 入力数が適正かチェックする．
// @param[in] input_num 入力数
// @retval true 適正だった．
// @retval false 入力数が合わなかった．
bool
SmtArithValidator::check_input_num(ymuint input_num) const
{
  return input_num >= 2;
}

// @brief 入力の型が適正かチェックする．
// @param[in] input_sort_list 入力の型のリスト
// @retval true 適正だった．
// @retval false 入力数もしくは型が合わなかった．
bool
SmtArithValidator::check_inputs(const vector<tSmtSortId>& input_sort_list) const
{
  ymuint n = input_sort_list.size();
  if ( n < 2 ) {
    return false;
  }
  tSmtSortId sort0 = input_sort_list[0];
  if ( sort0 != kSmtSort_Int && sort0 != kSmtSort_Real ) {
    return false;
  }
  for (ymuint i = 1; i < n; ++ i) {
    if ( input_sort_list[i] != sort0 ) {
      return false;
    }
  }
  return true;
}

// @brief 入力と出力の型が適正かチェックする．
// @param[in] input_sort_list 入力の型のリスト
// @param[in] output_sort 出力の型
// @retval true 適正だった．
// @retval false 入力数もしくは型が合わなかった．
bool
SmtArithValidator::check_all(const vector<tSmtSortId>& input_sort_list,
			     tSmtSortId output_sort) const
{
  if ( !check_inputs(input_sort_list) ) {
    return false;
  }
  if ( output_sort != input_sort_list[0] ) {
    return false;
  }
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtDivValidator
//////////////////////////////////////////////////////////////////////

// @brief 入力の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < max_input_num() )
tSmtSortId
SmtDivValidator::input_sort(ymuint pos) const
{
  return kSmtSort_Real;
}

// @brief 出力の型を返す．
tSmtSortId
SmtDivValidator::outout_sort() const
{
  return kSmtSort_Real;
}

// @brief 入力数が適正かチェックする．
// @param[in] input_num 入力数
// @retval true 適正だった．
// @retval false 入力数が合わなかった．
bool
SmtDivValidator::check_input_num(ymuint input_num) const
{
  return input_num >= 2;
}

// @brief 入力の型が適正かチェックする．
// @param[in] input_sort_list 入力の型のリスト
// @retval true 適正だった．
// @retval false 入力数もしくは型が合わなかった．
bool
SmtDivValidator::check_inputs(const vector<tSmtSortId>& input_sort_list) const
{
  ymuint n = input_sort_list.size();
  if ( n < 2 ) {
    return false;
  }
  for (ymuint i = 0; i < n; ++ i) {
    if ( input_sort_list[i] != kSmtSort_Real ) {
      return false;
    }
  }
  return true;
}

// @brief 入力と出力の型が適正かチェックする．
// @param[in] input_sort_list 入力の型のリスト
// @param[in] output_sort 出力の型
// @retval true 適正だった．
// @retval false 入力数もしくは型が合わなかった．
bool
SmtDivValidator::check_all(const vector<tSmtSortId>& input_sort_list,
			   tSmtSortId output_sort) const
{
  if ( !check_inputs(input_sort_list) ) {
    return false;
  }
  if ( output_sort != kSmtSort_Real ) {
    return false;
  }
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtCompValidator
//////////////////////////////////////////////////////////////////////

// @brief 入力の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < max_input_num() )
tSmtSortId
SmtCompValidator::input_sort(ymuint pos) const
{
  return kSmtSort_Num;
}

// @brief 出力の型を返す．
tSmtSortId
SmtCompValidator::outout_sort() const
{
  return kSmtSort_Bool;
}

// @brief 入力数が適正かチェックする．
// @param[in] input_num 入力数
// @retval true 適正だった．
// @retval false 入力数が合わなかった．
bool
SmtCompValidator::check_input_num(ymuint input_num) const
{
  return input_num >= 2;
}

// @brief 入力の型が適正かチェックする．
// @param[in] input_sort_list 入力の型のリスト
// @retval true 適正だった．
// @retval false 入力数もしくは型が合わなかった．
bool
SmtCompValidator::check_inputs(const vector<tSmtSortId>& input_sort_list) const
{
  ymuint n = input_sort_list.size();
  if ( n < 2 ) {
    return false;
  }
  tSmtSortId sort0 = input_sort_list[0];
  if ( sort0 != kSmtSort_Int && sort0 != kSmtSort_Real ) {
    return false;
  }
  for (ymuint i = 1; i < n; ++ i) {
    if ( input_sort_list[i] != sort0 ) {
      return false;
    }
  }
  return true;
}

// @brief 入力と出力の型が適正かチェックする．
// @param[in] input_sort_list 入力の型のリスト
// @param[in] output_sort 出力の型
// @retval true 適正だった．
// @retval false 入力数もしくは型が合わなかった．
bool
SmtCompValidator::check_all(const vector<tSmtSortId>& input_sort_list,
			    tSmtSortId output_sort) const
{
  if ( !check_inputs(input_sort_list) ) {
    return false;
  }
  if ( output_sort != kSmtSort_Bool ) {
    return false;
  }
  return true;
}

END_NAMESPACE_YM_SMT
