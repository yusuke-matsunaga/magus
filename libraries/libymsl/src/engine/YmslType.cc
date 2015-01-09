
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
// クラス YmslIntType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
YmslIntType::YmslIntType()
{
}

// @brief デストラクタ
YmslIntType::~YmslIntType()
{
}

// @brief 型を得る．
TypeId
YmslIntType::type() const
{
  return kIntType;
}

// @brief 値を表す文字列を返す．
string
YmslIntType::str() const
{
  return string("int");
}


//////////////////////////////////////////////////////////////////////
// クラス YmslFloatType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
YmslFloatType::YmslFloatType()
{
}

// @brief デストラクタ
YmslFloatType::~YmslFloatType()
{
}

// @brief 型を得る．
TypeId
YmslFloatType::type() const
{
  return kFloatType;
}

// @brief 値を表す文字列を返す．
string
YmslFloatType::str() const
{
  return string("float");
}


//////////////////////////////////////////////////////////////////////
// クラス YmslStringType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
YmslStringType::YmslStringType()
{
}

// @brief デストラクタ
YmslStringType::~YmslStringType()
{
}

// @brief 型を得る．
TypeId
YmslStringType::type() const
{
  return kStringType;
}

// @brief 値を表す文字列を返す．
string
YmslStringType::str() const
{
  return string("string");
}


//////////////////////////////////////////////////////////////////////
// クラス YmslUserType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] type_name 型名
YmslUserType::YmslUserType(string type_name) :
  mTypeName(type_name)
{
}

// @brief デストラクタ
YmslUserType::~YmslUserType()
{
}

// @brief 型を得る．
TypeId
YmslUserType::type() const
{
  return kUserType;
}

// @brief 値を表す文字列を返す．
string
YmslUserType::str() const
{
  return mTypeName;
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
  case kStructType:  s << "__struct__"; break;
  case kClassType:   s << "__class__"; break;
  case kFuncType:    s << "__function__"; break;
  case kUserType:    s << "__user__"; break;
  }
  return s;
}

END_NAMESPACE_YM_YMSL
