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
struct SmtAttr
{

  /// @brief コンストラクタ
  explicit
  SmtAttr(const ShString& keyword = ShString(),
	  const SmtTerm* value = NULL) :
    mKeyword(keyword),
    mValue(value)
  {
  }

  /// @brief キーワード
  ShString mKeyword;

  /// @brief 属性値
  const SmtTerm* mValue;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // YM_SMTLIBV2_SMTATTR_H
