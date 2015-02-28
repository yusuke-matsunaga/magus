
/// @file VsmFunction.cc
/// @brief VsmFunction の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "VsmFunction.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス VsmFunction
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 関数名
// @param[in] type 型
VsmFunction::VsmFunction(ShString name,
			 const Type* type) :
  mName(name),
  mType(type)
{
}

// @brief デストラクタ
VsmFunction::~VsmFunction()
{
}

// @brief 関数名を返す．
ShString
VsmFunction::name() const
{
  return mName;
}

// @brief 関数の型を返す．
const Type*
VsmFunction::type() const
{
  return mType;
}

END_NAMESPACE_YM_YMSL
