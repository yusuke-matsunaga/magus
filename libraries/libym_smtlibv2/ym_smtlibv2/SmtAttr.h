#ifndef YM_SMTLIBV2_SMTATTR_H
#define YM_SMTLIBV2_SMTATTR_H

/// @file ym_smtlibv2/SmtAttr.h
/// @brief SmtAttr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"
#include "ym_utils/ShString.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtAttr SmtAttr.h "ym_smtlibv2/SmtAttr.h"
/// @brief attribute を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtAttr
{
protected:

  /// @brief デストラクタ
  virtual
  ~SmtAttr() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief キーワードを表す文字列を返す．
  virtual
  ShString
  keyword() const = 0;

  /// @brief 属性値を表す式を返す．
  virtual
  const SmtTerm*
  value() const = 0;

};


/// @relates SmtAttr
/// @brief 内容を表す文字列を返す．
string
attr_str(const SmtAttr* attr);

END_NAMESPACE_YM_SMTLIBV2

#endif // YM_SMTLIBV2_SMTATTR_H
