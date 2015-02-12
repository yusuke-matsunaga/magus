
/// @file Function.cc
/// @brief Function の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Function.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス Function
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 関数名
// @param[in] type 関数の型
Function::Function(ShString name,
		   const Type* type) :
  mName(name),
  mType(type)
{
}

// @brief デストラクタ
Function::~Function()
{
}

// @brief 関数名を返す．
ShString
Function::name() const
{
  return mName;
}

// @brief 関数の型を返す．
const Type*
Function::type() const
{
  return mType;
}

END_NAMESPACE_YM_YMSL
