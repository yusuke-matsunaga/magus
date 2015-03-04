
/// @file VsmVar.cc
/// @brief VsmVar の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "VsmVar.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス VsmVar
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 変数名
// @param[in] type 型
VsmVar::VsmVar(ShString name,
	       const Type* type) :
  mName(name),
  mType(type)
{
}

// @brief デストラクタ
VsmVar::~VsmVar()
{
}

// @brief 関数名を返す．
ShString
VsmVar::name() const
{
  return mName;
}

// @brief 関数の型を返す．
const Type*
VsmVar::type() const
{
  return mType;
}

END_NAMESPACE_YM_YMSL
