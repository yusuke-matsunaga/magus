#ifndef YM_SMTLIBV2_SMTFUN_H
#define YM_SMTLIBV2_SMTFUN_H

/// @file ym_smtlibv2/SmtFun.h
/// @brief SmtFun のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"
#include "ym_utils/ShString.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtFun SmtFun.h "ym_smtlibv2/SmtFun.h"
/// @brief 関数を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtFun
{
  friend class SmtFunMgr;

protected:

  /// @brief デストラクタ
  virtual
  ~SmtFun() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  virtual
  ymuint
  id() const = 0;

  /// @brief 名前を返す．
  virtual
  const SmtId*
  name() const = 0;

  /// @brief 出力の型を返す．
  virtual
  const SmtSort*
  sort() const = 0;

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
  const SmtId*
  input_var(ymuint pos) const = 0;

  /// @brief 本体の式を返す．
  /// @note uninterpreted function の場合は NULL を返す．
  virtual
  const SmtTerm*
  body() const = 0;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // YM_SMTLIBV2_SMTFUN_H
