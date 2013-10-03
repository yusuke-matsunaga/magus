#ifndef YM_SMTLIBV2_SMTATTR_H
#define YM_SMTLIBV2_SMTATTR_H

/// @file ym_smtlibv2/SmtAttr.h
/// @brief SmtAttr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtAttr SmtAttr.h "ym_smtlibv2/SmtAttr.h"
/// @brief attribute を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtAttr
{
public:

  /// @brief コンストラクタ
  explicit
  SmtAttr(const ShString& keyword,
	  const SmtExpr* value = NULL);

  /// @brief デストラクタ
  ~SmtAttr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief キーワードを表す文字列を返す．
  ShString
  keyword() const;

  /// @brief 属性値を表す式を返す．
  const SmtExpr*
  value();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // キーワードを表す文字列
  ShString mKeyword;

  // attribute value
  const SmtExpr* mValue;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
SmtAttr::SmtAttr(const ShString& keyword,
		 const SmtExpr* value) :
  mKeyword(keyword),
  mValue(value)
{
}

// @brief デストラクタ
inline
SmtAttr::~SmtAttr()
{
}

// @brief キーワードを表す文字列を返す．
inline
ShString
SmtAttr::keyword() const
{
  return mKeyword;
}

// @brief 属性値を表す式を返す．
inline
const SmtExpr*
SmtAttr::value()
{
  return mValue;
}

END_NAMESPACE_YM_SMTLIBV2

#endif // YM_SMTLIBV2_SMTATTR_H
