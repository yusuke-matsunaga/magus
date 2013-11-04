#ifndef NAMEOBJ_H
#define NAMEOBJ_H

/// @file NameObj.h
/// @brief NameObj のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"
#include "ym_logic/smt_nsdef.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class NameObj NameObj.h "NameObj.h"
/// @brief SmtFun/SmtVar を持つ擬似ポインタの基底クラス
//////////////////////////////////////////////////////////////////////
class NameObj
{
  friend class SmtNameMgr;

protected:

  /// @brief デストラクタ
  virtual
  ~NameObj() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を表す識別子を返す．
  virtual
  const SmtId*
  name() const = 0;

  /// @brief 組み込み関数の時 true を返す．
  virtual
  bool
  is_builtin_fun() const = 0;

  /// @brief SmtFun を持っているとき true を返す．
  virtual
  bool
  is_fun() const = 0;

  /// @brief SmtVar を持っているとき true を返す．
  virtual
  bool
  is_var() const = 0;

  /// @brief SmtTerm を持っているとき true を返す．
  virtual
  bool
  is_term() const = 0;

  /// @brief tSumFun を返す．
  /// @note is_builtin_fun() == true の時のみ意味がある．
  virtual
  tSmtFunType
  fun_type() const = 0;

  /// @brief SmtFun を返す．
  // @note is_fun() == true の時のみ意味がある．
  virtual
  const SmtFun*
  fun() const = 0;

  /// @brief SmtVar を返す．
  /// @note is_var() == true の時のみ意味がある．
  virtual
  const SmtVar*
  var() const = 0;

  /// @brief SmtTerm を返す．
  /// @note is_term() == true の時のみ意味がある．
  virtual
  const SmtTerm*
  term() const = 0;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // NAMEOBJ_H
