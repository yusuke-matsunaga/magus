
/// @file SymDict.cc
/// @brief SymDict の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrHandleImpl.h"
#include "IrType.h"
#include "IrFunction.h"
//#include "IrLabel.h"
#include "IrScope.h"
#include "IrVar.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
IrHandle::IrHandle() :
  mLink(NULL)
{
}

// @brief デストラクタ
IrHandle::~IrHandle()
{
}

// @brief 変数を返す．
//
// 他の要素の場合には NULL を返す．
IrVar*
IrHandle::var() const
{
  return NULL;
}

// @brief 関数を返す．
//
// 他の要素の場合には NULL を返す．
IrFunction*
IrHandle::function() const
{
  return NULL;
}

// @brief ラベルを返す．
//
// 他の要素の場合には NULL を返す．
IrLabel*
IrHandle::label() const
{
  return NULL;
}

// @brief スコープを返す．
//
// 他の要素の場合には NULL を返す．
IrScope*
IrHandle::scope() const
{
  return NULL;
}

// @brief 型を返す．
//
// 他の要素の場合には NULL を返す．
IrType*
IrHandle::named_type() const
{
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス VarHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vardecl 変数宣言
VarHandle::VarHandle(IrVar* var) :
  mVar(var)
{
}

// @brief デストラクタ
VarHandle::~VarHandle()
{
}

// @brief 名前を返す．
ShString
VarHandle::name() const
{
  return mVar->name();
}

// @brief 変数を返す．
//
// 他の要素の場合には NULL を返す．
IrVar*
VarHandle::var() const
{
  return mVar;
}


//////////////////////////////////////////////////////////////////////
// クラス FuncHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] func 関数
FuncHandle::FuncHandle(IrFunction* func) :
  mFunc(func)
{
}

// @brief デストラクタ
FuncHandle::~FuncHandle()
{
}

// @brief 名前を返す．
ShString
FuncHandle::name() const
{
  return mFunc->name();
}

// @brief 関数を返す．
//
// 他の要素の場合には NULL を返す．
IrFunction*
FuncHandle::function() const
{
  return mFunc;
}


//////////////////////////////////////////////////////////////////////
// クラス LabelHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
LabelHandle::LabelHandle(IrLabel* label) :
  mLabel(label)
{
}

// @brief デストラクタ
LabelHandle::~LabelHandle()
{
}

// @brief 名前を返す．
ShString
LabelHandle::name() const
{
  //return mLabel->name();
}

// @brief ラベルを返す．
//
// 他の要素の場合には NULL を返す．
IrLabel*
LabelHandle::label() const
{
  return mLabel;
}


//////////////////////////////////////////////////////////////////////
// クラス ScopeHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] scope スコープ
ScopeHandle::ScopeHandle(IrScope* scope) :
  mScope(scope)
{
}

// @brief デストラクタ
ScopeHandle::~ScopeHandle()
{
}

// @brief 名前を返す．
ShString
ScopeHandle::name() const
{
  return mScope->name();
}

// @brief スコープを返す．
//
// 他の要素の場合には NULL を返す．
IrScope*
ScopeHandle::scope() const
{
  return mScope;
}


//////////////////////////////////////////////////////////////////////
// クラス TypeHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] type 型
TypeHandle::TypeHandle(IrType* type) :
  mType(type)
{
}

// @brief デストラクタ
TypeHandle::~TypeHandle()
{
}

// @brief 名前を返す．
ShString
TypeHandle::name() const
{
  return mType->type_name();
}

// @brief 型を返す．
//
// 他の要素の場合には NULL を返す．
IrType*
TypeHandle::named_type() const
{
  return mType;
}

END_NAMESPACE_YM_YMSL
