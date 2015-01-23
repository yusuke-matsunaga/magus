
/// @file YmslSetType.cc
/// @brief YmslSetType の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslSetType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslSetType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elem_type 要素の型
YmslSetType::YmslSetType(const YmslType* elem_type) :
  mElemType(elem_type)
{
}

// @brief デストラクタ
YmslSetType::~YmslSetType()
{
}

// @brief 型を得る．
TypeId
YmslSetType::type_id() const
{
  return kSetType;
}

// @brief 要素の型を得る．
//
// array/set/map のみ有効
const YmslType*
YmslSetType::elem_type() const
{
  return mElemType;
}

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
void
YmslSetType::print(ostream& s) const
{
  s << "set(";
  mElemType->print(s);
  s << ")";
}

END_NAMESPACE_YM_YMSL
