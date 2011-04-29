
/// @file libym_dotlib/PtNodeImpl.cc
/// @brief PtNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtNodeImpl.h"
#include "ym_dotlib/PtAttr.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス PtNodeImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PtNodeImpl::PtNodeImpl() :
  mNext(NULL)
{
}

// @brief デストラクタ
PtNodeImpl::~PtNodeImpl()
{
}

// @brief 整数型(kInt)の時に true を返す．
bool
PtNodeImpl::is_int() const
{
  return false;
}

// @brief 数値型(kInt か kFloat)の時に true を返す．
bool
PtNodeImpl::is_float() const
{
  return false;
}

// @brief 文字列型(kString)の時に true を返す．
bool
PtNodeImpl::is_string() const
{
  return false;
}

// @brief 演算子型(kPlus, kMinsu, kMult, kDiv)の時に true を返す．
bool
PtNodeImpl::is_opr() const
{
  return false;
}

// @brief リスト型(kList)の時に true を返す．
bool
PtNodeImpl::is_list() const
{
  return false;
}

// @brief グループ型(kGroup)の時に true を返す．
bool
PtNodeImpl::is_group() const
{
  return false;
}

// @brief 整数値を返す．
// @note is_int() = true の時のみ意味を持つ．
int
PtNodeImpl::int_value() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief 数値を返す．
// @note is_float() = true の時のみ意味を持つ．
double
PtNodeImpl::float_value() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0.0;
}

// @brief 定数シンボルを返す．
// @note is_string() = true の時のみ意味を持つ．
ShString
PtNodeImpl::string_value() const
{
  assert_not_reached(__FILE__, __LINE__);
  return ShString();
}

// @brief 第一オペランドを返す．
// @note is_opr() = true の時のみ意味を持つ．
const PtNode*
PtNodeImpl::opr1() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 第二オペランドを返す．
// @note is_opr() = true の時のみ意味を持つ．
const PtNode*
PtNodeImpl::opr2() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief リストの先頭の要素を返す．
// @note is_list() = true の時のみ意味を持つ．
const PtNode*
PtNodeImpl::top() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief リストの要素数を返す．
// @note is_list() = true の時のみ意味を持つ．
ymuint
PtNodeImpl::list_size() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief 次の要素を得る．
const PtNode*
PtNodeImpl::next() const
{
  return mNext;
}

// @brief 値を得る．
// @note is_group() = true の時のみ意味を持つ．
const PtNode*
PtNodeImpl::value() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 先頭の属性を得る．
// @note is_group() = true の時のみ意味を持つ．
const PtAttr*
PtNodeImpl::attr_top() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 要素を追加する．
// @param[in] node 追加する要素
// @note type() が kList の時のみ意味を持つ．
void
PtNodeImpl::add_node(PtNodeImpl* node)
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief attribute を設定する．
// @param[in] attr 属性
// @note type() が kGroup の時のみ意味を持つ．
void
PtNodeImpl::add_attr(PtAttr* attr)
{
  assert_not_reached(__FILE__, __LINE__);
}


//////////////////////////////////////////////////////////////////////
// クラス PtNodeBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル上の位置
PtNodeBase::PtNodeBase(const FileRegion& loc) :
  mLoc(loc)
{
}

// @brief デストラクタ
PtNodeBase::~PtNodeBase()
{
}

// @brief ファイル上の位置を返す．
FileRegion
PtNodeBase::loc() const
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
  PtNodeBase(loc),
  mValue(value)
{
}

// @brief デストラクタ
PtInt::~PtInt()
{
}

// @brief 型を得る．
PtNode::tType
PtInt::type() const
{
  return kInt;
}

// @brief 整数型(kInt)の時に true を返す．
bool
PtInt::is_int() const
{
  return true;
}

// @brief 数値型(kInt か kFloat)の時に true を返す．
bool
PtInt::is_float() const
{
  return true;
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
  PtNodeBase(loc),
  mValue(value)
{
}

// @brief デストラクタ
PtFloat::~PtFloat()
{
}

// @brief 型を得る．
PtNode::tType
PtFloat::type() const
{
  return kFloat;
}

// @brief 数値型(kInt か kFloat)の時に true を返す．
bool
PtFloat::is_float() const
{
  return true;
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
  PtNodeBase(loc),
  mValue(value)
{
}

// @brief デストラクタ
PtString::~PtString()
{
}

// @brief 型を得る．
PtNode::tType
PtString::type() const
{
  return kString;
}

// @brief 文字列型(kString)の時に true を返す．
bool
PtString::is_string() const
{
  return true;
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
	     PtNode* opr1,
	     PtNode* opr2) :
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
PtNode::tType
PtOpr::type() const
{
  return mType;
}

// @brief 演算子型(kPlus, kMinsu, kMult, kDiv)の時に true を返す．
bool
PtOpr::is_opr() const
{
  return true;
}

// @brief 式全体のファイル上の位置を返す．
FileRegion
PtOpr::loc() const
{
  return FileRegion(mOpr1->loc(), mOpr2->loc());
}

// @brief 第一オペランドを返す．
// @note type() が演算子の型の時のみ意味を持つ．
const PtNode*
PtOpr::opr1() const
{
  return mOpr1;
}

// @brief 第二オペランドを返す．
// @note type() が演算子の型の時のみ意味を持つ．
const PtNode*
PtOpr::opr2() const
{
  return mOpr2;
}


//////////////////////////////////////////////////////////////////////
// クラス PtList
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PtList::PtList() :
  mTop(NULL),
  mTail(NULL)
{
}

// @brief デストラクタ
PtList::~PtList()
{
}

// @brief 型を得る．
PtNode::tType
PtList::type() const
{
  return kList;
}

// @brief リスト型(kList)の時に true を返す．
bool
PtList::is_list() const
{
  return true;
}

// @brief 式全体のファイル上の位置を返す．
FileRegion
PtList::loc() const
{
  if ( mTop == NULL ) {
    return FileRegion();
  }
  return FileRegion(mTop->loc(), mTail->loc());
}

// @brief リストの先頭の要素を返す．
// @note type() が kList の時のみ意味をもつ．
const PtNode*
PtList::top() const
{
  return mTop;
}

// @brief リストの要素数を返す．
// @note is_list() = true の時のみ意味を持つ．
ymuint
PtList::list_size() const
{
  ymuint n = 0;
  for (const PtNodeImpl* v = mTop; v; v = v->mNext) {
    ++ n;
  }
  return n;
}

// @brief 要素を追加する．
// @param[in] node 追加する要素
// @note type() が kList の時のみ意味を持つ．
void
PtList::add_node(PtNodeImpl* node)
{
  if ( mTop != NULL ) {
    mTail->mNext = node;
    mTail = node;
  }
  else {
    mTop = mTail = node;
  }
}


//////////////////////////////////////////////////////////////////////
// クラス PtGroup
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] value 値
// @param[in] loc ファイル上の位置
PtGroup::PtGroup(const PtNode* value,
		 const FileRegion& loc) :
  PtNodeBase(loc),
  mValue(value),
  mAttrTop(NULL),
  mAttrTail(NULL)
{
}

// @brief デストラクタ
PtGroup::~PtGroup()
{
}

// @brief 型を得る．
PtNode::tType
PtGroup::type() const
{
  return kGroup;
}

// @brief グループ型(kGroup)の時に true を返す．
bool
PtGroup::is_group() const
{
  return true;
}

// @brief 値を得る．
// @note type() が kGroup の時のみ意味を持つ．
const PtNode*
PtGroup::value() const
{
  return mValue;
}

// @brief 先頭の属性を得る．
// @note type() が kGroup の時のみ意味を持つ．
const PtAttr*
PtGroup::attr_top() const
{
  return mAttrTop;
}

// @brief attribute を設定する．
// @param[in] attr 属性
// @note type() が kGroup の時のみ意味を持つ．
void
PtGroup::add_attr(PtAttr* attr)
{
  if ( mAttrTop != NULL ) {
    mAttrTail->mNext = attr;
    mAttrTail = attr;
  }
  else {
    mAttrTop = mAttrTail = attr;
  }
}

END_NAMESPACE_YM_DOTLIB
