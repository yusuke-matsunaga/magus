
/// @file SmtAttrImpl.cc
/// @brief SmtAttrImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtAttrImpl.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtAttrImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SmtAttrImpl::SmtAttrImpl(const ShString& keyword) :
  mKeyword(keyword)
{
}

// @brief デストラクタ
SmtAttrImpl::~SmtAttrImpl()
{
}

// @brief キーワードを表す文字列を返す．
ShString
SmtAttrImpl::keyword() const
{
  return mKeyword;
}

// @brief 属性値を表す式を返す．
const SmtTerm*
SmtAttrImpl::value() const
{
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtAttrImpl2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SmtAttrImpl2::SmtAttrImpl2(const ShString& keyword,
			   const SmtTerm* expr) :
  SmtAttrImpl(keyword),
  mValue(expr)
{
}

// @brief デストラクタ
SmtAttrImpl2::~SmtAttrImpl2()
{
}

// @brief 属性値を表す式を返す．
const SmtTerm*
SmtAttrImpl2::value() const
{
  return mValue;
}

END_NAMESPACE_YM_SMTLIBV2
