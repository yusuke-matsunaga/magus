
/// @file AstValueType.cc
/// @brief AstValueType の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstValueType.h"
#include "AstVoidType.h"
#include "AstBooleanType.h"
#include "AstIntType.h"
#include "AstFloatType.h"
#include "AstStringType.h"
#include "AstUserType.h"
#include "AstSymbol.h"


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
// クラス AstVoidType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル位置
AstVoidType::AstVoidType(const FileRegion& loc) :
  AstValueType(loc)
{
}

// @brief デストラクタ
AstVoidType::~AstVoidType()
{
}

// @brief 単純型の場合に true を返す．
bool
AstVoidType::simple_type() const
{
  return true;
}

// @brief 型を得る．
ValueType
AstVoidType::value_type() const
{
  return kVoidType;
}

// @brief 値を表す文字列を返す．
string
AstVoidType::str() const
{
  return string("int");
}


//////////////////////////////////////////////////////////////////////
// クラス AstBooleanType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル位置
AstBooleanType::AstBooleanType(const FileRegion& loc) :
  AstValueType(loc)
{
}

// @brief デストラクタ
AstBooleanType::~AstBooleanType()
{
}

// @brief 単純型の場合に true を返す．
bool
AstBooleanType::simple_type() const
{
  return true;
}

// @brief 型を得る．
ValueType
AstBooleanType::value_type() const
{
  return kBooleanType;
}

// @brief 値を表す文字列を返す．
string
AstBooleanType::str() const
{
  return string("int");
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

// @brief 単純型の場合に true を返す．
bool
AstIntType::simple_type() const
{
  return true;
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

// @brief 単純型の場合に true を返す．
bool
AstFloatType::simple_type() const
{
  return true;
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

// @brief 単純型の場合に true を返す．
bool
AstStringType::simple_type() const
{
  return true;
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


//////////////////////////////////////////////////////////////////////
// クラス AstUserType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] type_name 型名
// @param[in] loc ファイル位置
AstUserType::AstUserType(AstSymbol* type_name) :
  AstValueType(type_name->file_region()),
  mTypeName(type_name)
{
}

// @brief デストラクタ
AstUserType::~AstUserType()
{
}

// @brief 単純型の場合に true を返す．
bool
AstUserType::simple_type() const
{
  return false;
}

// @brief 型を得る．
ValueType
AstUserType::value_type() const
{
  return kUserType;
}

// @brief 値を表す文字列を返す．
string
AstUserType::str() const
{
  return mTypeName->str_val();
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstUserType::print(ostream& s,
		   ymuint indent) const
{
}

END_NAMESPACE_YM_YMSL
