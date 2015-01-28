
/// @file Var.cc
/// @brief Var の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Var.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス Var
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 変数名
// @param[in] value_type 型
// @param[in] index インデックス
Var::Var(ShString name,
	 const Type* value_type,
	 ymuint index) :
  mName(name),
  mValueType(value_type),
  mIndex(index)
{
}

// @brief デストラクタ
Var::~Var()
{
}

// @brief 変数名を得る．
ShString
Var::name() const
{
  return mName;
}

// @brief 型を得る．
const Type*
Var::value_type() const
{
  return mValueType;
}

// @brief 変数インデックスを得る．
ymuint
Var::index() const
{
  return mIndex;
}

END_NAMESPACE_YM_YMSL
