
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
// @param[in] input_type_list 入力の型のリスト
// @param[in] index インデックス
Function::Function(ShString name,
		   const Type* type,
		   int index) :
  mName(name),
  mType(type),
  mIndex(index)
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

// @brief 関数インデックスを返す．
int
Function::index() const
{
  return mIndex;
}

END_NAMESPACE_YM_YMSL
