#ifndef YM_LOGIC_SMTATTR_H
#define YM_LOGIC_SMTATTR_H

/// @file ym_logic/SmtAttr.h
/// @brief SmtAttr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/smt_nsdef.h"
#include "ym_utils/ShString.h"


BEGIN_NAMESPACE_YM_SMT

//////////////////////////////////////////////////////////////////////
/// @class SmtAttr SmtAttr.h "ym_logic/SmtAttr.h"
/// @brief attribute を表すクラス
//////////////////////////////////////////////////////////////////////
struct SmtAttr
{

  /// @brief 空のコンストラクタ
  SmtAttr()
  {
  }

  /// @brief コンストラクタ
  explicit
  SmtAttr(const ShString& keyword) :
    mKeyword(keyword)
  {
  }

  /// @brief コンストラクタ
  SmtAttr(const ShString& keyword,
	  const ShString& value) :
    mKeyword(keyword),
    mValue(value)
  {
  }

  /// @brief キーワード
  ShString mKeyword;

  /// @brief 属性値
  ShString mValue;

};

END_NAMESPACE_YM_SMT

#endif // YM_LOGIC_SMTATTR_H
