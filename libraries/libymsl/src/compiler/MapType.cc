
/// @file MapType.cc
/// @brief MapType の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "MapType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス MapType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] key_type キーの型
// @param[in] elem_type 要素の型
MapType::MapType(const Type* key_type,
		 const Type* elem_type) :
  Type(kMapType),
  mKeyType(key_type),
  mElemType(elem_type)
{
}

// @brief デストラクタ
MapType::~MapType()
{
}

// @brief 指定された型にキャスト可能な場合に true を返す．
// @param[in] type 指定された型
bool
MapType::castable_to(const Type* type) const
{
  return type == this;
}

// @brief キーの型を得る．
//
// map のみ有効
const Type*
MapType::key_type() const
{
  return mKeyType;
}

// @brief 要素の型を得る．
//
// array/set/map のみ有効
const Type*
MapType::elem_type() const
{
  return mElemType;
}

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
void
MapType::print(ostream& s) const
{
  s << "map(";
  mKeyType->print(s);
  s << ", ";
  mElemType->print(s);
  s << ")";
}

END_NAMESPACE_YM_YMSL
