
/// @file FuncVect.sip
/// @brief FuncVect の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class FuncVect
{
%TypeHeaderCode
#include "FuncVect.h"
using nsYm::nsIgf::FuncVect;
%End

  /// @brief コンストラクタ
  /// @param[in] max_val 値の最大値
  /// @param[in] input_size 入力の要素数
  FuncVect(unsigned int max_val,
	   unsigned int input_size);

  /// @brief 値の最大値を得る．
  unsigned int
  max_val() const;

  /// @brief 入力の要素数を得る．
  unsigned int
  input_size() const;

  /// @brief 値を得る．
  /// @param[in] id 入力番号 ( 0 <= id < input_size() )
  /// @note 出力値の範囲は [0, max_val())
  unsigned int
  val(unsigned int id) const;

  /// @brief 値を設定する．
  /// @param[in] id 入力番号 ( 0 <= id < input_size() )
  /// @param[in] val 値 ( 0 <= val < ma_val() )
  void
  set_val(unsigned int id,
	  unsigned int val);

};
