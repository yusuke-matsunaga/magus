
/// @file ConstVal.cc
/// @brief ConstVal の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ConstVal.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス ConstVal
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] value_type 値の型
ConstVal::ConstVal(const Type* value_type) :
  mValueType(value_type)
{
}

// @brief デストラクタ
ConstVal::~ConstVal()
{
}

// @brief 値の型を返す．
const Type*
ConstVal::value_type() const
{
  return mValueType;
}

// @brief ブール値を返す．
//
// value_type() が boolean_type の時有効
Ymsl_BOOLEAN
ConstVal::boolean_val() const
{
  ASSERT_NOT_REACHED;
  return false;
}

// @brief 整数値を返す．
//
// value_type() が int_type の時有効
Ymsl_INT
ConstVal::int_val() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 実数値を返す．
//
// value_type() が float_type の時有効
Ymsl_FLOAT
ConstVal::float_val() const
{
  ASSERT_NOT_REACHED;
  return 0.0;
}

// @brief 文字列を返す．
//
// value_type() が string_type の時有効
const char*
ConstVal::string_val() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

END_NAMESPACE_YM_YMSL
