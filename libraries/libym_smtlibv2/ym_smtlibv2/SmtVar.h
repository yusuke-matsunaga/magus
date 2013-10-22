#ifndef YM_SMTLIBV2_SMTVAR_H
#define YM_SMTLIBV2_SMTVAR_H

/// @file ym_smtlibv2/SmtVar.h
/// @brief SmtVar のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtVar SmtVar.h "ym_smtlibv2/SmtVar.h"
/// @brief 変数を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtVar
{
  friend class SmtNameMgr;

protected:

  /// @brief デストラクタ
  virtual
  ~SmtVar() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  const SmtId*
  name() const = 0;

  /// @brief 型を返す．
  /// @note 型がついていない場合は NULL を返す．
  virtual
  const SmtSort*
  sort() const = 0;

  /// @brief global 変数の場合に true を返す．
  virtual
  bool
  is_global() const = 0;

  /// @brief forall 変数の場合に true を返す．
  virtual
  bool
  is_forall() const = 0;

  /// @brief exists 変数の場合に true を返す．
  virtual
  bool
  is_exists() const = 0;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // YM_SMTLIBV2_SMTVAR_H
