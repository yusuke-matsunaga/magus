
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
PtValue::PtValue() :
  mNext(NULL)
{
}

// @brief デストラクタ
PtValue::~PtValue()
{
}

// @brief 整数値を返す．
// @note type() が kInt の時のみ意味を持つ．
int
PtValue::int_value() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief 数値を返す．
// @note type() が kFloat の時のみ意味を持つ．
double
PtValue::float_value() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0.0;
}

// @brief 定数シンボルを返す．
// @note type() が kString の時のみ意味を持つ．
ShString
PtValue::string_value() const
{
  assert_not_reached(__FILE__, __LINE__);
  return ShString();
}

// @brief 第一オペランドを返す．
// @note type() が演算子の型(kPlus, kMinus, kMult, kDiv)の時のみ意味を持つ．
const PtValue*
PtValue::opr1() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 第二オペランドを返す．
// @note type() が演算子の型(kPlus, kMinus, kMult, kDiv)の時のみ意味を持つ．
const PtValue*
PtValue::opr2() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief リストの先頭の要素を返す．
// @note type() が kList の時のみ意味をもつ．
const PtValue*
PtValue::top() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 次の要素を得る．
const PtValue*
PtValue::next() const
{
  return mNext;
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
  case PtValue::kInt:
    s << "INT(" << value->int_value() << ")";
    break;

  case PtValue::kFloat:
    s << "FLOAT(" << value->float_value() << ")";
    break;

  case PtValue::kString:
    s << "STRING(" << value->string_value() << ")";
    break;

  case PtValue::kPlus:
    s << "(" << value->opr1() << " + " << value->opr2() << ")";
    break;

  case PtValue::kMinus:
    s << "(" << value->opr1() << " - " << value->opr2() << ")";
    break;

  case PtValue::kMult:
    s << "(" << value->opr1() << " * " << value->opr2() << ")";
    break;

  case PtValue::kDiv:
    s << "(" << value->opr1() << " / " << value->opr2() << ")";
    break;

  case PtValue::kList:
    {
      const char* comma = "";
      s << "{ ";
      for (const PtValue* v1 = value->top(); v1; v1 = v1->next()) {
	s << comma << v1;
	comma = ", ";
      }
      s << " }";
    }
    break;

  default:
    s << "unknown type";
    break;
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
// クラス PtValueBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル上の位置
PtValueBase::PtValueBase(const FileRegion& loc) :
  mLoc(loc)
{
}

// @brief デストラクタ
PtValueBase::~PtValueBase()
{
}

// @brief ファイル上の位置を返す．
FileRegion
PtValueBase::loc() const
{
  return mLoc;
}


//////////////////////////////////////////////////////////////////////
/// クラス PtInt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] value 値
// @param[in] loc ファイル上の位置
PtInt::PtInt(int value,
	     const FileRegion& loc) :
  PtValueBase(loc),
  mValue(value)
{
}

// @brief デストラクタ
PtInt::~PtInt()
{
}

// @brief 型を得る．
PtValue::tType
PtInt::type() const
{
  return kInt;
}

// @brief 整数値を返す．
// @note type() が kInt の時のみ意味を持つ．
int
PtInt::int_value() const
{
  return mValue;
}

// @brief 数値を返す．
// @note type() が kFloat の時のみ意味を持つ．
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
  PtValueBase(loc),
  mValue(value)
{
}

// @brief デストラクタ
PtFloat::~PtFloat()
{
}

// @brief 型を得る．
PtValue::tType
PtFloat::type() const
{
  return kFloat;
}

// @brief 数値を返す．
// @note type() が kFloat の時のみ意味を持つ．
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
PtString::PtString(ShString value,
		   const FileRegion& loc) :
  PtValueBase(loc),
  mValue(value)
{
}

// @brief デストラクタ
PtString::~PtString()
{
}

// @brief 型を得る．
PtValue::tType
PtString::type() const
{
  return kString;
}

// @brief 定数シンボルを返す．
// @note type() が SYMBOL の時のみ意味を持つ．
ShString
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
PtOpr::PtOpr(tType type,
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
PtValue::tType
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


//////////////////////////////////////////////////////////////////////
// クラス PtList
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] top 先頭要素
PtList::PtList(PtValue* top) :
  mTop(top)
{
}

// @brief デストラクタ
PtList::~PtList()
{
}

// @brief 型を得る．
PtValue::tType
PtList::type() const
{
  return kList;
}

// @brief 式全体のファイル上の位置を返す．
FileRegion
PtList::loc() const
{
  const PtValue* top = mTop;
  if ( top == NULL ) {
    return FileRegion();
  }
  const PtValue* end = top;
  for ( ; end->next() != NULL; end = end->next() ) ;
  return FileRegion(top->loc(), end->loc());
}

// @brief リストの先頭の要素を返す．
// @note type() が kList の時のみ意味をもつ．
const PtValue*
PtList::top() const
{
  return mTop;
}


//////////////////////////////////////////////////////////////////////
// クラス PtValueList
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @note 空のリストとなる．
PtValueList::PtValueList()
{
  mTop = NULL;
  mTail = NULL;
}

// @brief デストラクタ
PtValueList::~PtValueList()
{
}

// @brief 要素を追加する．
void
PtValueList::add(PtValue* value)
{
  if ( mTop == NULL ) {
    mTop = mTail = value;
  }
  else {
    mTail->mNext = value;
    mTail = value;
  }
  // 念のため
  assert_cond( value->mNext == NULL, __FILE__, __LINE__);
}

// @brief 先頭の要素を取り出す．
PtValue*
PtValueList::top()
{
  return mTop;
}

END_NAMESPACE_YM_CELL_DOTLIB
