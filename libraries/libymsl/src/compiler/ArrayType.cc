
/// @file ArrayType.cc
/// @brief ArrayType の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ArrayType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス ArrayType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elem_type 要素の型
ArrayType::ArrayType(const Type* elem_type) :
  Type(kArrayType),
  mElemType(elem_type)
{
}

// @brief デストラクタ
ArrayType::~ArrayType()
{
}

// @brief 指定された型にキャスト可能な場合に true を返す．
// @param[in] type 指定された型
bool
ArrayType::castable_to(const Type* type) const
{
  return type == this;
}

// @brief 要素の型を得る．
//
// array/set/map のみ有効
const Type*
ArrayType::elem_type() const
{
  return mElemType;
}

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
void
ArrayType::print(ostream& s) const
{
  s << "array(";
  mElemType->print(s);
  s << ")";
}

END_NAMESPACE_YM_YMSL
