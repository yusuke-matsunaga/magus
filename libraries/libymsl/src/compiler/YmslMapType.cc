
/// @file YmslMapType.cc
/// @brief YmslMapType の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslMapType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslMapType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] key_type キーの型
// @param[in] elem_type 要素の型
YmslMapType::YmslMapType(const YmslType* key_type,
			     const YmslType* elem_type) :
  mKeyType(key_type),
  mElemType(elem_type)
{
}

// @brief デストラクタ
YmslMapType::~YmslMapType()
{
}

// @brief 型を得る．
TypeId
YmslMapType::type_id() const
{
  return kMapType;
}

// @brief キーの型を得る．
//
// map のみ有効
const YmslType*
YmslMapType::key_type() const
{
  return mKeyType;
}

// @brief 要素の型を得る．
//
// array/set/map のみ有効
const YmslType*
YmslMapType::elem_type() const
{
  return mElemType;
}

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
void
YmslMapType::print(ostream& s) const
{
  s << "map(";
  mKeyType->print(s);
  s << ", ";
  mElemType->print(s);
  s << ")";
}

END_NAMESPACE_YM_YMSL
