
/// @file SmtCompTerm.cc
/// @brief SmtCompTerm の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtCompTerm.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtCompTerm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] body 本体
SmtCompTerm::SmtCompTerm(const SmtTerm* body) :
  mBody(body)
{
}

// @brief デストラクタ
SmtCompTerm::~SmtCompTerm()
{
}

// @brief kLet/kForall/kExists/kAttr 型の場合に本体の項を返す．
const SmtTerm*
SmtCompTerm::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtQualTerm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] var_list 変数のリスト
// @param[in] body 本体
SmtQualTerm::SmtQualTerm(const vector<const SmtVarFun*>& var_list,
			 const SmtTerm* body) :
  SmtCompTerm(body),
  mVarNum(var_list.size())
{
  for (ymuint i = 0; i < mVarNum; ++ i) {
    mVarList[i] = var_list[i];
  }
}

// @brief デストラクタ
SmtQualTerm::~SmtQualTerm()
{
}

// @brief kForall/kExists 型の場合に変数リストの要素数を返す．
ymuint
SmtQualTerm::var_num() const
{
  return mVarNum;
}

// @brief kForall/kExists 型の場合に変数を返す．
// @param[in] pos 位置番号 ( 0 <= pos < var_num() )
const SmtVarFun*
SmtQualTerm::bound_var(ymuint pos) const
{
  assert_cond( pos < var_num(), __FILE__, __LINE__);
  return mVarList[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス SmtForall
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] var_list 変数のリスト
// @param[in] body 本体
SmtForall::SmtForall(const vector<const SmtVarFun*>& var_list,
		     const SmtTerm* body) :
  SmtQualTerm(var_list, body)
{
}

// @brief デストラクタ
SmtForall::~SmtForall()
{
}

// @brief 型を返す．
SmtTerm::tType
SmtForall::type() const
{
  return kForall;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtExists
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] var_list 変数のリスト
// @param[in] body 本体
SmtExists::SmtExists(const vector<const SmtVarFun*>& var_list,
		     const SmtTerm* body) :
  SmtQualTerm(var_list, body)
{
}

// @brief デストラクタ
SmtExists::~SmtExists()
{
}

// @brief 型を返す．
SmtTerm::tType
SmtExists::type() const
{
  return kExists;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtAttrTerm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] body 本体
// @param[in] attr_list 属性のリスト
SmtAttrTerm::SmtAttrTerm(const SmtTerm* body,
			 const vector<SmtAttr>& attr_list) :
  SmtCompTerm(body),
  mAttrNum(attr_list.size())
{
  for (ymuint i = 0; i < mAttrNum; ++ i) {
    mAttrList[i] = attr_list[i];
  }
}

// @brief デストラクタ
SmtAttrTerm::~SmtAttrTerm()
{
}

// @brief 型を返す．
SmtTerm::tType
SmtAttrTerm::type() const
{
  return kAttr;
}

// @brief attr 型の場合に属性リストの要素数を返す．
ymuint
SmtAttrTerm::attr_num() const
{
  return mAttrNum;
}

// @brief attr 型の場合に属性キーを返す．
// @param[in] pos 位置番号 ( 0 <= pos < attr_attr_num() )
ShString
SmtAttrTerm::attr_keyword(ymuint pos) const
{
  assert_cond( pos < attr_num(), __FILE__, __LINE__);
  return mAttrList[pos].mKeyword;
}

// @brief attr 型の場合に属性値を返す．
// @param[in] pos 位置番号 ( 0 <= pos < attr_attr_num() )
const SmtTerm*
SmtAttrTerm::attr_value(ymuint pos) const
{
  assert_cond( pos < attr_num(), __FILE__, __LINE__);
  return mAttrList[pos].mValue;
}

END_NAMESPACE_YM_SMTLIBV2
