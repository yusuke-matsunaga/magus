
/// @file Type.cc
/// @brief Type の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Type.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス Type
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Type::Type()
{
}

// @brief デストラクタ
Type::~Type()
{
}

// @brief 型名を得る．
//
// enum のみ有効
ShString
Type::type_name() const
{
  ASSERT_NOT_REACHED;
  return ShString();
}

// @brief キーの型を得る．
//
// map のみ有効
const Type*
Type::key_type() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 要素の型を得る．
//
// array/set/map のみ有効
const Type*
Type::elem_type() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 関数の出力の型を返す．
//
// function のみ有効
const Type*
Type::function_type() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 関数の入力数を返す．
//
// function のみ有効
ymuint
Type::function_input_num() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 関数の入力の型を返す．
// @param[in] pos 入力番号 ( 0 <= pos < function_input_num() )
//
// function のみ有効
const Type*
Type::function_input_type(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 列挙型の数を得る．
//
// enum のみ有効
ymuint
Type::enum_num() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 列挙型の定数値を得る．
// @param[in] index インデックス ( 0 <= index < enum_num() )
//
// enum のみ有効
int
Type::enum_elem_val(ymuint index) const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 列挙型の定数名を得る．
// @param[in] index インデックス ( 0 <= index < enum_num() )
//
// enum のみ有効
ShString
Type::enum_elem_name(ymuint index) const
{
  ASSERT_NOT_REACHED;
  return ShString();
}

// @brief 列挙型のインデックスを得る．
// @param[in] name 列挙型の名前
//
// enum のみ有効
// 該当する名前がなければ -1 を返す．
int
Type::enum_index(ShString name) const
{
  ASSERT_NOT_REACHED;
  return -1;
}


//////////////////////////////////////////////////////////////////////
// misc
//////////////////////////////////////////////////////////////////////

// @brief ValueType を出力する．
ostream&
operator<<(ostream& s,
	   TypeId vt)
{
  switch ( vt ) {
  case kVoidType:    s << "void"; break;
  case kBooleanType: s << "boolean"; break;
  case kIntType:     s << "int"; break;
  case kFloatType:   s << "float"; break;
  case kStringType:  s << "string"; break;
  case kArrayType:   s << "__array__"; break;
  case kSetType:     s << "__set__"; break;
  case kMapType:     s << "__map__"; break;
  case kFuncType:    s << "__function__"; break;
  case kClassType:   s << "__class__"; break;
  case kEnumType:    s << "__enum__"; break;
  case kUserDefType: s << "__user__"; break;
  }
  return s;
}

END_NAMESPACE_YM_YMSL
