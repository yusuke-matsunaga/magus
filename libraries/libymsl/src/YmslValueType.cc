
/// @file YmslValueType.cc
/// @brief YmslValueType の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslValueType.h"
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
ValueType
YmslIntType::value_type() const
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
ValueType
YmslFloatType::value_type() const
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
ValueType
YmslStringType::value_type() const
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
ValueType
YmslUserType::value_type() const
{
  return kUserType;
}

// @brief 値を表す文字列を返す．
string
YmslUserType::str() const
{
  return mTypeName;
}

END_NAMESPACE_YM_YMSL
