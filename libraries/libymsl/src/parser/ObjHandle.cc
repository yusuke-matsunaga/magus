
/// @file SymDict.cc
/// @brief SymDict の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ObjHandleImpl.h"
#include "YmslEnum.h"
#include "YmslFunction.h"
#include "YmslLabel.h"
#include "YmslScope.h"
#include "YmslVar.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス ObjHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ObjHandle::ObjHandle() :
  mLink(NULL)
{
}

// @brief デストラクタ
ObjHandle::~ObjHandle()
{
}

// @brief 変数を返す．
//
// 他の要素の場合には NULL を返す．
YmslVar*
ObjHandle::var() const
{
  return NULL;
}

// @brief 関数を返す．
//
// 他の要素の場合には NULL を返す．
YmslFunction*
ObjHandle::function() const
{
  return NULL;
}

// @brief ラベルを返す．
//
// 他の要素の場合には NULL を返す．
YmslLabel*
ObjHandle::label() const
{
  return NULL;
}

// @brief スコープを返す．
//
// 他の要素の場合には NULL を返す．
YmslScope*
ObjHandle::scope() const
{
  return NULL;
}

// @brief enum 型を返す．
//
// 他の要素の場合には NULL を返す．
YmslEnum*
ObjHandle::enum_type() const
{
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス VarHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vardecl 変数宣言
VarHandle::VarHandle(YmslVar* var) :
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
YmslVar*
VarHandle::var() const
{
  return mVar;
}


//////////////////////////////////////////////////////////////////////
// クラス FuncHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] func 関数
FuncHandle::FuncHandle(YmslFunction* func) :
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
YmslFunction*
FuncHandle::function() const
{
  return mFunc;
}


//////////////////////////////////////////////////////////////////////
// クラス LabelHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
LabelHandle::LabelHandle(YmslLabel* label) :
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
  return mLabel->name();
}

// @brief ラベルを返す．
//
// 他の要素の場合には NULL を返す．
YmslLabel*
LabelHandle::label() const
{
  return mLabel;
}


//////////////////////////////////////////////////////////////////////
// クラス ScopeHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] scope スコープ
ScopeHandle::ScopeHandle(YmslScope* scope) :
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
YmslScope*
ScopeHandle::scope() const
{
  return mScope;
}


//////////////////////////////////////////////////////////////////////
// クラス EnumHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] enum_type enum 型
EnumHandle::EnumHandle(YmslEnum* enum_type) :
  mEnum(enum_type)
{
}

// @brief デストラクタ
EnumHandle::~EnumHandle()
{
}

// @brief 名前を返す．
ShString
EnumHandle::name() const
{
  return mEnum->name();
}

// @brief enum 型を返す．
//
// 他の要素の場合には NULL を返す．
YmslEnum*
EnumHandle::enum_type() const
{
  return mEnum;
}

END_NAMESPACE_YM_YMSL
