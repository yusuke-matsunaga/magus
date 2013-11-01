#ifndef YM_LOGIC_SMTFUN_H
#define YM_LOGIC_SMTFUN_H


/// @file ym_logic/SmtFun.h
/// @brief SmtFun のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/smt_nsdef.h"


BEGIN_NAMESPACE_YM_SMT

//////////////////////////////////////////////////////////////////////
/// @class SmtFun SmtFun.h "ym_logic/SmtFun.h"
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
  friend class SmtSolverImpl;

protected:

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
  const SmtSort*
  input_sort(ymuint pos) const = 0;

  /// @brief 入力変数を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  /// @note uninterpreted function の場合は NULL を返す．
  virtual
  const SmtVar*
  input_var(ymuint pos) const = 0;

  /// @brief 出力の型を返す．
  virtual
  const SmtSort*
  output_sort() const = 0;

  /// @brief 本体の式を返す．
  /// @note uninterpreted function の場合は NULL を返す．
  virtual
  const SmtTerm*
  body() const = 0;

};

END_NAMESPACE_YM_SMT

#endif // YM_LOGIC_SMTFUN_H
