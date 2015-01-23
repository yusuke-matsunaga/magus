
/// @file YmslArrayType.cc
/// @brief YmslArrayType の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslArrayType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslArrayType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elem_type 要素の型
YmslArrayType::YmslArrayType(const YmslType* elem_type) :
  mElemType(elem_type)
{
}

// @brief デストラクタ
YmslArrayType::~YmslArrayType()
{
}

// @brief 型を得る．
TypeId
YmslArrayType::type_id() const
{
  return kArrayType;
}

// @brief 要素の型を得る．
//
// array/set/map のみ有効
const YmslType*
YmslArrayType::elem_type() const
{
  return mElemType;
}

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
void
YmslArrayType::print(ostream& s) const
{
  s << "array(";
  mElemType->print(s);
  s << ")";
}

END_NAMESPACE_YM_YMSL
