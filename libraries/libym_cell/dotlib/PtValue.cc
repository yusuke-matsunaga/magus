
/// @file libym_cell/dotlib/PtValue.cc
/// @brief PtValue の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtValue.h"
#include "PtValueImpl.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス PtValue
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PtValue::PtValue()
{
}

// @brief デストラクタ
PtValue::~PtValue()
{
}

// @brief 整数値を返す．
// @note type() が INT_NUM の時のみ意味を持つ．
int
PtValue::int_value() const
{
  return 0;
}

// @brief 数値を返す．
// @note type() が FLOAT_NUM の時のみ意味を持つ．
double
PtValue::float_value() const
{
  return 0.0;
}

// @brief 定数シンボルを返す．
// @note type() が SYMBOL の時のみ意味を持つ．
string
PtValue::string_value() const
{
  return string();
}

// @brief 第一オペランドを返す．
// @note type() が演算子の型の時のみ意味を持つ．
const PtValue*
PtValue::opr1() const
{
  return NULL;
}

// @brief 第二オペランドを返す．
// @note type() が演算子の型の時のみ意味を持つ．
const PtValue*
PtValue::opr2() const
{
  return NULL;
}


// @relates PtValue
// @brief PtValue の内容をストリームに出力する．
// @param[in] s 出力先のストリーム
// @param[in] value 値
ostream&
operator<<(ostream& s,
	   const PtValue* value)
{
  switch ( value->type() ) {
  case INT_NUM:
    s << "int(" << value->int_value() << ")";
    break;

  case FLOAT_NUM:
    s << "float(" << value->float_value() << ")";
    break;

  case SYMBOL:
    s << "string(" << value->string_value() << ")";
    break;

  case PLUS:
    s << "(" << value->opr1() << " + " << value->opr2() << ")";
    break;

  case MINUS:
    s << "(" << value->opr1() << " - " << value->opr2() << ")";
    break;

  case MULT:
    s << "(" << value->opr1() << " * " << value->opr2() << ")";
    break;

  case DIV:
    s << "(" << value->opr1() << " / " << value->opr2() << ")";
    break;

  default:
    s << "unknown type";
    break;
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
/// クラス PtInt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] value 値
// @param[in] loc ファイル上の位置
PtInt::PtInt(int value,
	     const FileRegion& loc) :
  mValue(value),
  mLoc(loc)
{
}

// @brief デストラクタ
PtInt::~PtInt()
{
}

// @brief 型を得る．
tTokenType
PtInt::type() const
{
  return INT_NUM;
}

// @brief 式全体のファイル上の位置を返す．
FileRegion
PtInt::loc() const
{
  return mLoc;
}

// @brief 整数値を返す．
// @note type() が INT_NUM の時のみ意味を持つ．
int
PtInt::int_value() const
{
  return mValue;
}

// @brief 数値を返す．
// @note type() が FLOAT_NUM の時のみ意味を持つ．
double
PtInt::float_value() const
{
  return static_cast<double>(mValue);
}


//////////////////////////////////////////////////////////////////////
/// クラス PtFloat
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] value 値
// @param[in] loc ファイル上の位置
PtFloat::PtFloat(double value,
		 const FileRegion& loc) :
  mValue(value),
  mLoc(loc)
{
}

// @brief デストラクタ
PtFloat::~PtFloat()
{
}

// @brief 型を得る．
tTokenType
PtFloat::type() const
{
  return FLOAT_NUM;
}

// @brief 式全体のファイル上の位置を返す．
FileRegion
PtFloat::loc() const
{
  return mLoc;
}

// @brief 数値を返す．
// @note type() が FLOAT_NUM の時のみ意味を持つ．
double
PtFloat::float_value() const
{
  return mValue;
}


//////////////////////////////////////////////////////////////////////
/// クラス PtString
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] value 値
// @param[in] loc ファイル上の位置
PtString::PtString(const string& value,
		   const FileRegion& loc) :
  mValue(value),
  mLoc(loc)
{
}

// @brief デストラクタ
PtString::~PtString()
{
}

// @brief 型を得る．
tTokenType
PtString::type() const
{
  return SYMBOL;
}

// @brief 式全体のファイル上の位置を返す．
FileRegion
PtString::loc() const
{
  return mLoc;
}

// @brief 定数シンボルを返す．
// @note type() が SYMBOL の時のみ意味を持つ．
string
PtString::string_value() const
{
  return mValue;
}


//////////////////////////////////////////////////////////////////////
// クラス PtOpr
//////////////////////////////////////////////////////////////////////

// @brief 表すコンストラクタ
// @param[in] type 演算子の型
// @param[in] opr1, opr2 オペランド
PtOpr::PtOpr(tTokenType type,
	     PtValue* opr1,
	     PtValue* opr2) :
  mType(type),
  mOpr1(opr1),
  mOpr2(opr2)

{
}

// @brief デストラクタ
PtOpr::~PtOpr()
{
}

// @brief 型を得る．
tTokenType
PtOpr::type() const
{
  return mType;
}

// @brief 式全体のファイル上の位置を返す．
FileRegion
PtOpr::loc() const
{
  return FileRegion(mOpr1->loc(), mOpr2->loc());
}

// @brief 第一オペランドを返す．
// @note type() が演算子の型の時のみ意味を持つ．
const PtValue*
PtOpr::opr1() const
{
  return mOpr1;
}

// @brief 第二オペランドを返す．
// @note type() が演算子の型の時のみ意味を持つ．
const PtValue*
PtOpr::opr2() const
{
  return mOpr2;
}

END_NAMESPACE_YM_CELL_DOTLIB
