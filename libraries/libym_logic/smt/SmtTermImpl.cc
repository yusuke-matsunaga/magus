
/// @file SmtTermImpl.cc
/// @brief SmtTermImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtTermImpl.h"


BEGIN_NAMESPACE_YM_SMT

//////////////////////////////////////////////////////////////////////
// クラス SmtTermImpl
//////////////////////////////////////////////////////////////////////

// @brief デストラクタ
SmtTermImpl::~SmtTermImpl()
{
}

// @brief kNumConst 型の場合に整数値を返す．
ymuint32
SmtTermImpl::int_value() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief kDecConst/kHexConst/kBinConst/kStrConst 型の場合に文字列を返す．
ShString
SmtTermImpl::str_value() const
{
  assert_not_reached(__FILE__, __LINE__);
  return ShString();
}

// @brief kVarTerm 型の場合に関数を返す．
const SmtVar*
SmtTermImpl::var() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief kFunTerm 型の場合に関数を返す．
const SmtFun*
SmtTermImpl::function() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief kFunTerm 型の場合に入力数を返す．
ymuint
SmtTermImpl::input_num() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief kFunTerm 型の場合に入力を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
const SmtTerm*
SmtTermImpl::input(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief kForall/kExists 型の場合に変数リストの要素数を返す．
ymuint
SmtTermImpl::var_num() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief kForall/kExists 型の場合に変数を返す．
// @param[in] pos 位置番号 ( 0 <= pos < var_num() )
const SmtVar*
SmtTermImpl::bound_var(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief annotated 型の場合に属性リストの要素数を返す．
ymuint
SmtTermImpl::attr_num() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief attr 型の場合に属性キーを返す．
// @param[in] pos 位置番号 ( 0 <= pos < attr_attr_num() )
ShString
SmtTermImpl::attr_keyword(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return ShString();
}

// @brief attr 型の場合に属性値を返す．
// @param[in] pos 位置番号 ( 0 <= pos < attr_attr_num() )
ShString
SmtTermImpl::attr_value(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return ShString();
}

// @brief kForall/kExists/kAnnotated 型の場合に本体の項を返す．
const SmtTerm*
SmtTermImpl::body() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtNumTerm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
SmtNumTerm::SmtNumTerm(ymuint val) :
  mVal(val)
{
}

// @brief デストラクタ
SmtNumTerm::~SmtNumTerm()
{
}

// @brief 型を返す．
SmtTerm::tType
SmtNumTerm::type() const
{
  return kNumConst;
}

// @brief kNumConst 型の場合に整数値を返す．
ymuint32
SmtNumTerm::int_value() const
{
  return mVal;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtStrTerm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
SmtStrTerm::SmtStrTerm(const ShString& val) :
  mVal(val)
{
}

// @brief デストラクタ
SmtStrTerm::~SmtStrTerm()
{
}

// @brief 型を返す．
SmtTerm::tType
SmtStrTerm::type() const
{
  return kStrConst;
}

// @brief kDecConst/kHexConst/kBinConst/kStrConst 型の場合に文字列を返す．
ShString
SmtStrTerm::str_value() const
{
  return mVal;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtDecTerm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
SmtDecTerm::SmtDecTerm(const ShString& val) :
  SmtStrTerm(val)
{
}

// @brief デストラクタ
SmtDecTerm::~SmtDecTerm()
{
}

// @brief 型を返す．
SmtTerm::tType
SmtDecTerm::type() const
{
  return kDecConst;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtHexTerm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
SmtHexTerm::SmtHexTerm(const ShString& val) :
  SmtStrTerm(val)
{
}

// @brief デストラクタ
SmtHexTerm::~SmtHexTerm()
{
}

// @brief 型を返す．
SmtTerm::tType
SmtHexTerm::type() const
{
  return kHexConst;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtBinTerm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
SmtBinTerm::SmtBinTerm(const ShString& val) :
  SmtStrTerm(val)
{
}

// @brief デストラクタ
SmtBinTerm::~SmtBinTerm()
{
}

// @brief 型を返す．
SmtTerm::tType
SmtBinTerm::type() const
{
  return kBinConst;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtVarTerm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] var 変数
SmtVarTerm::SmtVarTerm(const SmtVar* var) :
  mVar(var)
{
}

// @brief デストラクタ
SmtVarTerm::~SmtVarTerm()
{
}

// @brief 型を返す．
SmtTerm::tType
SmtVarTerm::type() const
{
  return kVarTerm;
}

// @brief kVarTerm 型の場合に関数を返す．
const SmtVar*
SmtVarTerm::var() const
{
  return mVar;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtFunTerm1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fun 関数
SmtFunTerm1::SmtFunTerm1(const SmtFun* fun) :
  mFun(fun)
{
}

// @brief デストラクタ
SmtFunTerm1::~SmtFunTerm1()
{
}

// @brief 型を返す．
SmtTerm::tType
SmtFunTerm1::type() const
{
  return kFunTerm;
}

// @brief kFunTerm 型の場合に関数を返す．
const SmtFun*
SmtFunTerm1::function() const
{
  return mFun;
}

// @brief kFunTerm 型の場合に入力数を返す．
ymuint
SmtFunTerm1::input_num() const
{
  return 0;
}

// @brief kFunTerm 型の場合に入力を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
const SmtTerm*
SmtFunTerm1::input(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtFunTerm2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fun 関数
// @param[in] arg_list 引数のリスト
SmtFunTerm2::SmtFunTerm2(const SmtFun* fun,
			 const vector<SmtTerm*>& arg_list) :
  SmtFunTerm1(fun),
  mInputNum(arg_list.size())
{
  for (ymuint i = 0; i < mInputNum; ++ i) {
    mInputList[i] = arg_list[i];
  }
}

// @brief デストラクタ
SmtFunTerm2::~SmtFunTerm2()
{
}

// @brief kFunTerm 型の場合に入力数を返す．
ymuint
SmtFunTerm2::input_num() const
{
  return mInputNum;
}

// @brief kFunTerm 型の場合に入力を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
const SmtTerm*
SmtFunTerm2::input(ymuint pos) const
{
  assert_cond( pos < input_num(), __FILE__, __LINE__);
  return mInputList[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス SmtQualTerm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] var_list 束縛変数のリスト
// @param[in] body 本体の式
SmtQualTerm::SmtQualTerm(const vector<const SmtVar*>& var_list,
			 const SmtTerm* body) :
  mBody(body),
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
const SmtVar*
SmtQualTerm::bound_var(ymuint pos) const
{
  assert_cond( pos < var_num(), __FILE__, __LINE__);
  return mVarList[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス SmtForallTerm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] var_list 束縛変数のリスト
// @param[in] body 本体の式
SmtForallTerm::SmtForallTerm(const vector<const SmtVar*>& var_list,
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
// @param[in] var_list 束縛変数のリスト
// @param[in] body 本体の式
SmtExistsTerm::SmtExistsTerm(const vector<const SmtVar*>& var_list,
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
// クラス SmtAnnotatedTerm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] body 本体の式
// @param[in] attr_list 属性リスト
SmtAnnotatedTerm::SmtAnnotatedTerm(const SmtTerm* body,
				   const vector<SmtAttr>& attr_list) :
  mBody(body),
  mAttrNum(attr_list.size())
{
  for (ymuint i = 0; i < mAttrNum; ++ i) {
    mAttrList[i] = attr_list[i];
  }
}

// @brief デストラクタ
SmtAnnotatedTerm::~SmtAnnotatedTerm()
{
}

// @brief 型を返す．
SmtTerm::tType
SmtAnnotatedTerm::type() const
{
  return kAnnotated;
}

// @brief annotated 型の場合に属性リストの要素数を返す．
ymuint
SmtAnnotatedTerm::attr_num() const
{
  return mAttrNum;
}

// @brief attr 型の場合に属性キーを返す．
// @param[in] pos 位置番号 ( 0 <= pos < attr_attr_num() )
ShString
SmtAnnotatedTerm::attr_keyword(ymuint pos) const
{
  assert_cond( pos < attr_num(), __FILE__, __LINE__);
  return mAttrList[pos].mKeyword;
}

// @brief attr 型の場合に属性値を返す．
// @param[in] pos 位置番号 ( 0 <= pos < attr_attr_num() )
ShString
SmtAnnotatedTerm::attr_value(ymuint pos) const
{
  assert_cond( pos < attr_num(), __FILE__, __LINE__);
  return mAttrList[pos].mValue;
}

END_NAMESPACE_YM_SMT
