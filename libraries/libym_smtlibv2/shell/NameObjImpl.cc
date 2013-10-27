
/// @file NameObjImpl.cc
/// @brief NameObjImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "NameObjImpl.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス NameObjImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name_id 名前を表す識別子
NameObjImpl::NameObjImpl(const SmtId* name_id) :
  mName(name_id),
  mLink(NULL)
{
}

// @brief デストラクタ
NameObjImpl::~NameObjImpl()
{
}

// @brief 名前を表す識別子を返す．
const SmtId*
NameObjImpl::name() const
{
  return mName;
}

// @brief 組み込み関数の時 true を返す．
bool
NameObjImpl::is_builtin_fun() const
{
  return false;
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

// @brief tSumFun を返す．
// @note is_builtin_fun() == true の時のみ意味がある．
tSmtFun
NameObjImpl::fun_type() const
{
  assert_not_reached(__FILE__, __LINE__);
  return kSmtFun_UserDef;
}

// @brief SmtFun を返す．
// @note is_fun() == true の時のみ意味がある．
const SmtFun*
NameObjImpl::fun() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief SmtVar を返す．
// @note is_var() == true の時のみ意味がある．
const SmtVar*
NameObjImpl::var() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス BuiltinFunObj
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name_id 名前を表す識別子
// @param[in] fun_type 関数の型
BuiltinFunObj::BuiltinFunObj(const SmtId* name_id,
			     tSmtFun fun_type) :
  NameObjImpl(name_id),
  mFunType(fun_type)
{
}

// @brief デストラクタ
BuiltinFunObj::~BuiltinFunObj()
{
}

// @brief 組み込み関数の時 true を返す．
bool
BuiltinFunObj::is_builtin_fun() const
{
  return true;
}

// @brief tSumFun を返す．
// @note is_builtin_fun() == true の時のみ意味がある．
tSmtFun
BuiltinFunObj::fun_type() const
{
  return mFunType;
}


//////////////////////////////////////////////////////////////////////
// クラス FunObj
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name_id 名前を表す識別子
// @param[in] fun 関数
FunObj::FunObj(const SmtId* name_id,
	       const SmtFun* fun) :
  NameObjImpl(name_id),
  mFun(fun)
{
}

// @brief デストラクタ
FunObj::~FunObj()
{
}

// @brief SmtFun を持っているとき true を返す．
bool
FunObj::is_fun() const
{
  return true;
}

// @brief SmtFun を返す．
// @note is_fun() == true の時のみ意味がある．
const SmtFun*
FunObj::fun() const
{
  return mFun;
}


//////////////////////////////////////////////////////////////////////
// クラス VarObj
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name_id 名前を表す識別子
// @param[in] var 変数
VarObj::VarObj(const SmtId* name_id,
	       const SmtVar* var) :
  NameObjImpl(name_id),
  mVar(var)
{
}

// @brief デストラクタ
VarObj::~VarObj()
{
}

// @brief SmtVar を持っているとき true を返す．
bool
VarObj::is_var() const
{
  return true;
}

// @brief SmtVar を返す．
const SmtVar*
VarObj::var() const
{
  return mVar;
}

END_NAMESPACE_YM_SMTLIBV2
