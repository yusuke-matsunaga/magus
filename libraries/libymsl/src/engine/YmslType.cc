
/// @file YmslType.cc
/// @brief YmslType の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslType.h"
#include "YmslIntType.h"
#include "YmslFloatType.h"
#include "YmslStringType.h"
#include "YmslUserType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
YmslType::YmslType(ymuint id) :
  mId(id)
{
}

// @brief デストラクタ
YmslType::~YmslType()
{
}

// @brief 値を表す文字列を返す．
//
// class/enum/userdef のみ有効
string
YmslType::str() const
{
  return string();
}

// @brief 要素の型を得る．
//
// array/set/map のみ有効
const YmslType*
YmslType::elem_type() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief キーの型を得る．
//
// map のみ有効
const YmslType*
YmslType::key_type() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 関数の出力の型を返す．
//
// function のみ有効
const YmslType*
YmslType::function_type() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 関数の入力数を返す．
//
// function のみ有効
ymuint
YmslType::function_input_num() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 関数の入力の型を返す．
// @param[in] pos 入力番号 ( 0 <= pos < function_input_num() )
//
// function のみ有効
const YmslType*
YmslType::function_input_type(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief フィールド(メンバ変数)の数を得る．
//
// class のみ有効
ymuint
YmslType::field_num() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief フィールド(メンバ変数)の型を得る．
// @param[in] index インデックス ( 0 <= index < field_num() )
//
// class のみ有効
const YmslType*
YmslType::field_type(ymuint index) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief フィールド(メンバ変数)の名前を得る．
// @param[in] index インデックス ( 0 <= index < field_num() )
//
// class のみ有効
ShString
YmslType::field_name(ymuint index) const
{
  ASSERT_NOT_REACHED;
  return ShString();
}

// @brief フィールド(メンバ変数)のインデックスを得る．
// @param[in] name フィールド名
//
// class のみ有効
// 該当するフィールドがなければ -1 を返す．
int
YmslType::field_index(ShString name) const
{
  ASSERT_NOT_REACHED;
  return -1;
}

// @brief メソッド(メンバ関数)の数を得る．
//
// class のみ有効
ymuint
YmslType::method_num() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief メソッド(メンバ関数)の型を得る．
// @param[in] index インデックス ( 0 <= index < method_num() )
//
// class のみ有効
const YmslType*
YmslType::method_type(ymuint index) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief メソッド(メンバ関数)の名前を得る．
// @param[in] index インデックス ( 0 <= index < method_num() )
//
// class のみ有効
ShString
YmslType::method_name(ymuint index) const
{
  ASSERT_NOT_REACHED;
  return ShString();
}

// @brief メソッド(メンバ関数)のインデックスを得る．
// @param[in] name メソッド名
//
// class のみ有効
// 該当するメソッドがなければ -1 を返す．
int
YmslType::method_index(ShString name) const
{
  ASSERT_NOT_REACHED;
  return -1;
}

// @brief 列挙型の数を得る．
//
// enum のみ有効
ymuint
YmslType::enum_num() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 列挙型の要素名を得る．
// @param[in] index インデックス ( 0 <= index < enum_num() )
//
// enum のみ有効
ShString
YmslType::enum_name(ymuint index) const
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
YmslType::enum_index(ShString name) const
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
