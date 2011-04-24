
/// @file libym_cell/dotlib/PtValue.cc
/// @brief PtValue の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtValue.h"


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


//////////////////////////////////////////////////////////////////////
/// クラス PtInt
//////////////////////////////////////////////////////////////////////
class PtInt :
  public PtValue
{

  /// @brief コンストラクタ
  /// @param[in] value 値
  /// @param[in] loc ファイル上の位置
  PtInt(int value,
	const FileRegion& loc);

  /// @brief デストラクタ
  ~PtInt();


public:

  /// @brief 型を得る．
  virtual
  tTokenType
  type() const;

  /// @brief 式全体のファイル上の位置を返す．
  virtual
  FileRegion
  loc() const;

  /// @brief 整数値を返す．
  /// @note type() が INT_NUM の時のみ意味を持つ．
  virtual
  int
  int_value() const;

  /// @brief 数値を返す．
  /// @note type() が FLOAT_NUM の時のみ意味を持つ．
  virtual
  double
  float_value() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  int mValue;

  // ファイル上の位置
  FileRegion mLoc;

};

// @brief コンストラクタ
// @param[in] value 値
// @param[in] loc ファイル上の位置
PtInt::PtInt(double value,
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
  return FLOAT_NUM;
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
  return stati_cast<double>(mValue);
}


//////////////////////////////////////////////////////////////////////
/// クラス PtFloat
//////////////////////////////////////////////////////////////////////
class PtFloat :
  public PtValue
{

  /// @brief コンストラクタ
  /// @param[in] value 値
  /// @param[in] loc ファイル上の位置
  PtFloat(double value,
	  const FileRegion& loc);

  /// @brief デストラクタ
  ~PtFloat();


public:

  /// @brief 型を得る．
  virtual
  tTokenType
  type() const;

  /// @brief 式全体のファイル上の位置を返す．
  virtual
  FileRegion
  loc() const;

  /// @brief 数値を返す．
  /// @note type() が FLOAT_NUM の時のみ意味を持つ．
  virtual
  double
  float_value() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  double mValue;

  // ファイル上の位置
  FileRegion mLoc;

};

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
class PtString :
  public PtValue
{

  /// @brief コンストラクタ
  /// @param[in] value 値
  /// @param[in] loc ファイル上の位置
  PtString(const string& value,
	   const FileRegion& loc);

  /// @brief デストラクタ
  ~PtString();


public:

  /// @brief 型を得る．
  virtual
  tTokenType
  type() const;

  /// @brief 式全体のファイル上の位置を返す．
  virtual
  FileRegion
  loc() const;

  /// @brief 定数シンボルを返す．
  /// @note type() が SYMBOL の時のみ意味を持つ．
  virtual
  string
  string_value() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  string mValue;

  // ファイル上の位置
  FileRegion mLoc;

};

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
class PtOpr :
  public PtValue
{
public:

  /// @brief 表すコンストラクタ
  /// @param[in] type 演算子の型
  /// @param[in] opr1, opr2 オペランド
  PtOpr(tTokenType type,
	PtValue* opr1,
	PtValue* opr2);

  /// @brief デストラクタ
  virtual
  ~PtOpr();


public:

  /// @brief 型を得る．
  virtual
  tTokenType
  type() const;

  /// @brief 式全体のファイル上の位置を返す．
  virtual
  FileRegion
  loc() const;

  /// @brief 第一オペランドを返す．
  /// @note type() が演算子の型の時のみ意味を持つ．
  virtual
  const PtValue*
  opr1() const;

  /// @brief 第二オペランドを返す．
  /// @note type() が演算子の型の時のみ意味を持つ．
  virtual
  const PtValue*
  opr2() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 型
  tTokenType mType;

  // 第一オペランド
  const PtValue* mOpr1;

  // 第二オペランド
  const PtValue* mOpr2;

};

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
