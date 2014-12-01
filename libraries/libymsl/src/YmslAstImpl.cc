
/// @file YmslAstImpl.cc
/// @brief YmslAstImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstImpl.h"
#include "YmslAstList.h"
#include "YmslAstIntType.h"
#include "YmslAstFloatType.h"
#include "YmslAstStringType.h"
#include "YmslAstUserType.h"
#include "YmslAstInt.h"
#include "YmslAstFloat.h"
#include "YmslAstString.h"
#include "YmslAstSymbol.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslAstImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル位置
YmslAstImpl::YmslAstImpl(const FileRegion& loc) :
  mLoc(loc)
{
}

// @brief デストラクタ
YmslAstImpl::~YmslAstImpl()
{
}

// @brief ファイル位置を得る．
const FileRegion&
YmslAstImpl::file_region() const
{
  return mLoc;
}

// @brief 文字列型の値を返す．
const char*
YmslAstImpl::str_val() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 整数型の値を返す．
int
YmslAstImpl::int_val() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 浮動小数点型の値を返す．
double
YmslAstImpl::float_val() const
{
  ASSERT_NOT_REACHED;
  return 0.0;
}

// @brief 子供の数を返す．
ymuint
YmslAstImpl::child_num() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
YmslAst*
YmslAstImpl::child(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 子供を追加する．
void
YmslAstImpl::add_child(YmslAst* child)
{
  ASSERT_NOT_REACHED;
}

// @brief インデント用の空白を出力する
// @param[in] s 出力ストリーム
// @param[in] indent インデント量
void
YmslAstImpl::print_indent(ostream& s,
			  ymuint indent)
{
  for (ymuint i = 0; i < indent; ++ i) {
    s << "    ";
  }
}

// @brief ステートメントリストを出力する
// @param[in] s 出力ストリーム
// @param[in] statement_list ステートメントリスト
// @param[in] indent インデント量
void
YmslAstImpl::print_statement_list(ostream& s,
				  YmslAst* statement_list,
				  ymuint indent)
{
  ASSERT_COND( statement_list->type() == kAstList );
  ymuint n = statement_list->child_num();
  for (ymuint i = 0; i < n; ++ i) {
    YmslAst* statement = statement_list->child(i);
    statement->print(s, indent);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstList
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
YmslAstList::YmslAstList()
{
}

// @brief デストラクタ
YmslAstList::~YmslAstList()
{
}

// @brief ファイル位置を得る．
const FileRegion&
YmslAstList::file_region() const
{
  return mLoc;
}

// @brief 型を得る．
AstType
YmslAstList::type() const
{
  return kAstList;
}

// @brief 文字列型の値を返す．
const char*
YmslAstList::str_val() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 整数型の値を返す．
int
YmslAstList::int_val() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 浮動小数点型の値を返す．
double
YmslAstList::float_val() const
{
  ASSERT_NOT_REACHED;
  return 0.0;
}

// @brief 子供の数を返す．
ymuint
YmslAstList::child_num() const
{
  return mList.size();
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
YmslAst*
YmslAstList::child(ymuint pos) const
{
  ASSERT_COND( pos < child_num() );
  return mList[pos];
}

// @brief 子供を追加する．
void
YmslAstList::add_child(YmslAst* child)
{
  if ( child == NULL ) {
    return;
  }
  if ( mList.empty() ) {
    mLoc = child->file_region();
  }
  mLoc = FileRegion(mLoc, child->file_region());
  mList.push_back(child);
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
YmslAstList::print(ostream& s,
		   ymuint indent) const
{
  YmslAstImpl::print_indent(s, indent);
  s << "[List begin]" << endl;

  for (vector<YmslAst*>::const_iterator p = mList.begin();
       p != mList.end(); ++ p) {
    YmslAst* child = *p;
    child->print(s, indent + 1);
    s << endl;
  }

  YmslAstImpl::print_indent(s, indent);
  s << "[List end]" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstIntType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル位置
YmslAstIntType::YmslAstIntType(const FileRegion& loc) :
  YmslAstImpl(loc)
{
}

// @brief デストラクタ
YmslAstIntType::~YmslAstIntType()
{
}

// @brief 型を得る．
AstType
YmslAstIntType::type() const
{
  return kAstIntType;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
YmslAstIntType::print(ostream& s,
		      ymuint indent) const
{
  print_indent(s, indent);
  s << "int";
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstFloatType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル位置
YmslAstFloatType::YmslAstFloatType(const FileRegion& loc) :
  YmslAstImpl(loc)
{
}

// @brief デストラクタ
YmslAstFloatType::~YmslAstFloatType()
{
}

// @brief 型を得る．
AstType
YmslAstFloatType::type() const
{
  return kAstFloatType;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
YmslAstFloatType::print(ostream& s,
			ymuint indent) const
{
  print_indent(s, indent);
  s << "float";
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstStringType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル位置
YmslAstStringType::YmslAstStringType(const FileRegion& loc) :
  YmslAstImpl(loc)
{
}

// @brief デストラクタ
YmslAstStringType::~YmslAstStringType()
{
}

// @brief 型を得る．
AstType
YmslAstStringType::type() const
{
  return kAstStringType;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
YmslAstStringType::print(ostream& s,
			 ymuint indent) const
{
  print_indent(s, indent);
  s << "string";
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstUserType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] type_name 型名
YmslAstUserType::YmslAstUserType(YmslAst* type_name) :
  YmslAstImpl(type_name->file_region())
{
  mTypeName = type_name;
}

// @brief デストラクタ
YmslAstUserType::~YmslAstUserType()
{
}

// @brief 型を得る．
AstType
YmslAstUserType::type() const
{
  return kAstUserType;
}

// @brief 子供の数を返す．
ymuint
YmslAstUserType::child_num() const
{
  return 1;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
YmslAst*
YmslAstUserType::child(ymuint pos) const
{
  ASSERT_COND( pos < child_num() );
  return mTypeName;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
YmslAstUserType::print(ostream& s,
		       ymuint indent) const
{
  print_indent(s, indent);
  mTypeName->print(s);
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstInt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
// @param[in] loc ファイル位置
YmslAstInt::YmslAstInt(int val,
		       const FileRegion& loc) :
  YmslAstImpl(loc),
  mVal(val)
{
}

// @brief デストラクタ
YmslAstInt::~YmslAstInt()
{
}

// @brief 型を得る．
AstType
YmslAstInt::type() const
{
  return kAstInt;
}

// @brief 整数型の値を返す．
int
YmslAstInt::int_val() const
{
  return mVal;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
YmslAstInt::print(ostream& s,
		  ymuint indent) const
{
  print_indent(s, indent);
  s << int_val();
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstFloat
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
// @param[in] loc ファイル位置
YmslAstFloat::YmslAstFloat(double val,
			   const FileRegion& loc) :
  YmslAstImpl(loc),
  mVal(val)
{
}

// @brief デストラクタ
YmslAstFloat::~YmslAstFloat()
{
}

// @brief 型を得る．
AstType
YmslAstFloat::type() const
{
  return kAstFloat;
}

// @brief 浮動小数点型の値を返す．
double
YmslAstFloat::float_val() const
{
  return mVal;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
YmslAstFloat::print(ostream& s,
		    ymuint indent) const
{
  print_indent(s, indent);
  s << float_val();
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstString
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
// @param[in] loc ファイル位置
YmslAstString::YmslAstString(const char* val,
			     const FileRegion& loc) :
  YmslAstImpl(loc),
  mVal(val)
{
}

// @brief デストラクタ
YmslAstString::~YmslAstString()
{
}

// @brief 型を得る．
AstType
YmslAstString::type() const
{
  return kAstString;
}

// @brief 文字列型の値を返す．
const char*
YmslAstString::str_val() const
{
  return mVal;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
YmslAstString::print(ostream& s,
		     ymuint indent) const
{
  print_indent(s, indent);
  s << str_val();
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstSymbol
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
// @param[in] loc ファイル位置
YmslAstSymbol::YmslAstSymbol(ShString val,
			     const FileRegion& loc) :
  YmslAstImpl(loc),
  mVal(val)
{
}

// @brief デストラクタ
YmslAstSymbol::~YmslAstSymbol()
{
}

// @brief 型を得る．
AstType
YmslAstSymbol::type() const
{
  return kAstSymbol;
}

// @brief 文字列型の値を返す．
const char*
YmslAstSymbol::str_val() const
{
  return mVal;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
YmslAstSymbol::print(ostream& s,
		     ymuint indent) const
{
  print_indent(s, indent);
  s << str_val();
}

END_NAMESPACE_YM_YMSL
