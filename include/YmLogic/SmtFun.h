#ifndef YMYMLOGIC_SMTFUN_H
#define YMYMLOGIC_SMTFUN_H


/// @file YmLogic/SmtFun.h
/// @brief SmtFun のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/smt_nsdef.h"


BEGIN_NAMESPACE_YM_SMT

//////////////////////////////////////////////////////////////////////
/// @class SmtFun SmtFun.h "YmLogic/SmtFun.h"
/// @brief 関数を表すクラス
///
/// 関数は以下のプロパティを持つ．
///  - 入力の型
///  - 出力の型
///  - 本体の式 (オプショナル)
///  - 属性
//////////////////////////////////////////////////////////////////////
class SmtFun
{
public:
  /// @brief デストラクタ
  virtual
  ~SmtFun() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を返す．
  virtual
  ymuint
  input_num() const = 0;

  /// @brief 入力の型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  virtual
  tSmtSortId
  input_sort(ymuint pos) const = 0;

  /// @brief 入力変数を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  /// @note uninterpreted function の場合は NULL を返す．
  virtual
  const SmtVar*
  input_var(ymuint pos) const = 0;

  /// @brief 出力の型を返す．
  virtual
  tSmtSortId
  output_sort() const = 0;

  /// @brief 本体の式を返す．
  /// @note uninterpreted function の場合は NULL を返す．
  virtual
  const SmtTerm*
  body() const = 0;

};

END_NAMESPACE_YM_SMT

#endif // YMYMLOGIC_SMTFUN_H
