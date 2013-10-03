
/// @file SmtBodyTerm.cc
/// @brief SmtBodyTerm の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtBodyTerm.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtBodyTerm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] body 本体
SmtBodyTerm::SmtBodyTerm(const SmtTerm* body) :
  mBody(body)
{
}

// @brief デストラクタ
SmtBodyTerm::~SmtBodyTerm()
{
}

// @brief kLet/kForall/kExists/kAttr 型の場合に本体の項を返す．
const SmtTerm*
SmtBodyTerm::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtLetTerm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] bind_list バインドリスト
// @param[in] body 本体
SmtLetTerm::SmtLetTerm(const vector<pair<ShString, const SmtTerm*> >& bind_list,
		       const SmtTerm* body) :
  SmtBodyTerm(body),
  mBindList(bind_list)
{
}

// @brief デストラクタ
SmtLetTerm::~SmtLetTerm()
{
}

// @brief 型を返す．
SmtTerm::tType
SmtLetTerm::type() const
{
  return kLet;
}

// @brief kLet 型の場合に変数バインディングリストの要素数を返す．
ymuint
SmtLetTerm::let_binding_num() const
{
  return mBindList.size();
}

// @brief kLet 型の場合に変数バインディングリストの要素を返す．
// @param[in] pos 位置番号 ( 0 <= pos < let_binding_num() )
pair<ShString, const SmtTerm*>
SmtLetTerm::let_binding(ymuint pos) const
{
  assert_cond( pos < let_binding_num(), __FILE__, __LINE__);
  return mBindList[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス SmtQualTerm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] var_list 変数リスト
// @param[in] body 本体
SmtQualTerm::SmtQualTerm(const vector<pair<ShString, const SmtSort*> >& var_list,
			 const SmtTerm* body) :
  SmtBodyTerm(body),
  mVarList(var_list)
{
}

// @brief デストラクタ
SmtQualTerm::~SmtQualTerm()
{
}

// @brief kForall/kExists 型の場合に変数リストの要素数を返す．
ymuint
SmtQualTerm::var_num() const
{
  return mVarList.size();
}

// @brief kForall/kExists 型の場合に変数を返す．
// @param[in] pos 位置番号 ( 0 <= pos < forall_var_num() )
pair<ShString, const SmtSort*>
SmtQualTerm::sorted_var(ymuint pos) const
{
  assert_cond( pos < var_num(), __FILE__, __LINE__);
  return mVarList[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス SmtForallTerm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] var_list 変数リスト
// @param[in] body 本体
SmtForallTerm::SmtForallTerm(const vector<pair<ShString, const SmtSort*> >& var_list,
			     const SmtTerm* body) :
  SmtQualTerm(var_list, body)
{
}

// @brief デストラクタ
SmtForallTerm::~SmtForallTerm()
{
}

// @brief 型を返す．
SmtTerm::tType
SmtForallTerm::type() const
{
  return kForall;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtExistsTerm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] var_list 変数リスト
// @param[in] body 本体
SmtExistsTerm::SmtExistsTerm(const vector<pair<ShString, const SmtSort*> >& var_list,
			     const SmtTerm* body) :
  SmtQualTerm(var_list, body)
{
}

// @brief デストラクタ
SmtExistsTerm::~SmtExistsTerm()
{
}

// @brief 型を返す．
SmtTerm::tType
SmtExistsTerm::type() const
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
			 const vector<const SmtAttr*>& attr_list) :
  SmtBodyTerm(body),
  mAttrList(attr_list)
{
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
  return mAttrList.size();
}

// @brief attr 型の場合に属性を返す．
// @param[in] pos 位置番号 ( 0 <= pos < attr_attr_num() )
const SmtAttr*
SmtAttrTerm::attr(ymuint pos) const
{
  assert_cond( pos < attr_num(), __FILE__, __LINE__);
  return mAttrList[pos];
}

END_NAMESPACE_YM_SMTLIBV2
