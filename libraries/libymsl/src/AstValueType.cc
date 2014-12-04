
/// @file AstValueType.cc
/// @brief AstValueType の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstValueType.h"
#include "AstIntType.h"
#include "AstFloatType.h"
#include "AstStringType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstValueType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AstValueType::AstValueType(const FileRegion& loc) :
  Ast(loc)
{
}

// @brief デストラクタ
AstValueType::~AstValueType()
{
}


//////////////////////////////////////////////////////////////////////
// クラス AstIntType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル位置
AstIntType::AstIntType(const FileRegion& loc) :
  AstValueType(loc)
{
}

// @brief デストラクタ
AstIntType::~AstIntType()
{
}

// @brief 型を得る．
ValueType
AstIntType::value_type() const
{
  return kIntType;
}

// @brief 値を表す文字列を返す．
string
AstIntType::str() const
{
  return string("int");
}


//////////////////////////////////////////////////////////////////////
// クラス AstFloatType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル位置
AstFloatType::AstFloatType(const FileRegion& loc) :
  AstValueType(loc)
{
}

// @brief デストラクタ
AstFloatType::~AstFloatType()
{
}

// @brief 型を得る．
ValueType
AstFloatType::value_type() const
{
  return kFloatType;
}

// @brief 値を表す文字列を返す．
string
AstFloatType::str() const
{
  return string("float");
}


//////////////////////////////////////////////////////////////////////
// クラス AstStringType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル位置
AstStringType::AstStringType(const FileRegion& loc) :
  AstValueType(loc)
{
}

// @brief デストラクタ
AstStringType::~AstStringType()
{
}

// @brief 型を得る．
ValueType
AstStringType::value_type() const
{
  return kStringType;
}

// @brief 値を表す文字列を返す．
string
AstStringType::str() const
{
  return string("string");
}

END_NAMESPACE_YM_YMSL
