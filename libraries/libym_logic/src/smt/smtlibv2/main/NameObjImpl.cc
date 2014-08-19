
/// @file NameObjImpl.cc
/// @brief NameObjImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "NameObjImpol.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス NameObjImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NameObjImpl::NameObjImpl()
{
  mLink = NULL;
}

// @brief デストラクタ
NameObjImpl::~NameObjImpl()
{
}

// @brief SmtFun を持っているとき true を返す．
bool
NameObjImpl::is_fun() const
{
  return false;
}

// @brief SmtVar を持っているとき true を返す．
bool
NameObjImpl::is_var() const
{
  return false;
}

// @brief SmtFun を返す．
const SmtFun*
NameObImpl::fun() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief SmtVar を返す．
const SmtVar*
NameObjImpl::var() const
{
  assert_not_reached(__FILE__, __LINE_);
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス FunObj
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fun 関数
FunObj::FunObj(const SmtFun* fun) :
  mFun(fun)
{
}

// @brief デストラクタ
FunObj::~FunObj()
{
}

// @brief 名前を表す識別子を返す．
const SmtId*
FunObj::name() const
{
  return mFun->name();
}

// @brief SmtFun を持っているとき true を返す．
bool
FunObj::is_fun() const
{
  return true;
}

// @brief SmtFun を返す．
const SmtFun*
FunObj::fun() const
{
  return mFun;
}


//////////////////////////////////////////////////////////////////////
// クラス VarObj
//////////////////////////////////////////////////////////////////////



END_NAMESPACE_YM_SMTLIBV2
