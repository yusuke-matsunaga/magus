
/// @file YmslAstImpl.cc
/// @brief YmslAstImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstImpl.h"
#include "YmslAstList.h"
#include "YmslAstVarDecl.h"
#include "YmslAstFuncDecl.h"
#include "YmslAstAssignment.h"
#include "YmslAstIf.h"
#include "YmslAstElif.h"
#include "YmslAstFor.h"
#include "YmslAstWhile.h"
#include "YmslAstDoWhile.h"
#include "YmslAstSwitch.h"
#include "YmslAstCaseItem.h"
#include "YmslAstGoto.h"
#include "YmslAstLabel.h"
#include "YmslAstBreak.h"
#include "YmslAstContinue.h"
#include "YmslAstReturn.h"
#include "YmslAstBlock.h"
#include "YmslAstIntType.h"
#include "YmslAstFloatType.h"
#include "YmslAstStringType.h"
#include "YmslAstUserType.h"


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
void
YmslAstList::print(ostream& s) const
{
  s << "[List begin]" << endl;
  for (vector<YmslAst*>::const_iterator p = mList.begin();
       p != mList.end(); ++ p) {
    YmslAst* child = *p;
    child->print(s);
    s << endl;
  }
  s << "[List end]" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstVarDecl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id 変数名
// @param[in] type 型
// @param[in] init_expr 初期化式
// @param[in] loc ファイル位置
YmslAstVarDecl::YmslAstVarDecl(YmslAst* id,
			       YmslAst* type,
			       YmslAst* init_expr,
			       const FileRegion& loc) :
  YmslAstImpl(loc)
{
  mChildList[0] = id;
  mChildList[1] = type;
  mChildList[2] = init_expr;
}

// @brief デストラクタ
YmslAstVarDecl::~YmslAstVarDecl()
{
}

// @brief 型を得る．
AstType
YmslAstVarDecl::type() const
{
  return kAstVarDecl;
}

// @brief 子供の数を返す．
ymuint
YmslAstVarDecl::child_num() const
{
  return 3;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
YmslAst*
YmslAstVarDecl::child(ymuint pos) const
{
  ASSERT_COND( pos < child_num() );
  return mChildList[pos];
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
YmslAstVarDecl::print(ostream& s) const
{
  s << "var ";
  mChildList[0]->print(s);
  s << ": ";
  mChildList[1]->print(s);
  if ( mChildList[2] != NULL ) {
    s << " = ";
    mChildList[2]->print(s);
  }
  s << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstFuncDecl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id 変数名
// @param[in] type 型
// @param[in] param_list パラメータリスト
// @param[in] statement_list 本文のリスト
// @param[in] loc ファイル位置
YmslAstFuncDecl::YmslAstFuncDecl(YmslAst* id,
				 YmslAst* type,
				 YmslAst* param_list,
				 YmslAst* statement_list,
				 const FileRegion& loc) :
  YmslAstImpl(loc)
{
  mChildList[0] = id;
  mChildList[1] = type;
  mChildList[2] = param_list;
  mChildList[3] = statement_list;
}

// @brief デストラクタ
YmslAstFuncDecl::~YmslAstFuncDecl()
{
}

// @brief 型を得る．
AstType
YmslAstFuncDecl::type() const
{
  return kAstFuncDecl;
}

// @brief 子供の数を返す．
ymuint
YmslAstFuncDecl::child_num() const
{
  return 4;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
YmslAst*
YmslAstFuncDecl::child(ymuint pos) const
{
  ASSERT_COND( pos < child_num() );
  return mChildList[pos];
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
YmslAstFuncDecl::print(ostream& s) const
{
  s << "function ";
  mChildList[0]->print(s);
  s << ": ";
  mChildList[1]->print(s);
  s << "(";
  {
    ymuint n = mChildList[2]->child_num();
    const char* comma = "";
    for (ymuint i = 0; i < n; ++ i) {
      YmslAst* param = mChildList[2]->child(i);
      s << comma;
      comma = ", ";
      param->print(s);
    }
  }
  s << ") {" << endl;
  {
    ymuint n = mChildList[3]->child_num();
    for (ymuint i = 0; i < n; ++ i) {
      YmslAst* statement = mChildList[3]->child(i);
      statement->print(s);
    }
  }
  s << "}" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstAssignment
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left 左辺
// @param[in] right 右辺
YmslAstAssignment::YmslAstAssignment(YmslAst* left,
				     YmslAst* right) :
  YmslAstImpl(FileRegion(left->file_region(), right->file_region()))
{
  mChildList[0] = left;
  mChildList[1] = right;
}

// @brief デストラクタ
YmslAstAssignment::~YmslAstAssignment()
{
}

// @brief 型を得る．
AstType
YmslAstAssignment::type() const
{
  return kAstAssignment;
}

// @brief 子供の数を返す．
ymuint
YmslAstAssignment::child_num() const
{
  return 2;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
YmslAst*
YmslAstAssignment::child(ymuint pos) const
{
  ASSERT_COND( pos < child_num() );
  return mChildList[pos];
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
YmslAstAssignment::print(ostream& s) const
{
  mChildList[0]->print(s);
  s << " = " << endl;
  mChildList[1]->print(s);
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstIf
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cond 条件
// @param[in] then_list then 文のリスト
// @param[in] elif_list elif ブロックのリスト
// @param[in] else_list else 文のリスト
// @param[in] loc ファイル位置
YmslAstIf::YmslAstIf(YmslAst* cond,
		     YmslAst* then_list,
		     YmslAst* elif_list,
		     YmslAst* else_list,
		     const FileRegion& loc) :
  YmslAstImpl(loc)
{
  mChildList[0] = cond;
  mChildList[1] = then_list;
  mChildList[2] = elif_list;
  mChildList[3] = else_list;
}

// @brief デストラクタ
YmslAstIf::~YmslAstIf()
{
}

// @brief 型を得る．
AstType
YmslAstIf::type() const
{
  return kAstIf;
}

// @brief 子供の数を返す．
ymuint
YmslAstIf::child_num() const
{
  return 4;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
YmslAst*
YmslAstIf::child(ymuint pos) const
{
  ASSERT_COND( pos < child_num() );
  return mChildList[pos];
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
YmslAstIf::print(ostream& s) const
{
  s << "if ";
  mChildList[0]->print(s);
  s << " {" << endl;
  {
    ymuint n = mChildList[1]->child_num();
    for (ymuint i = 0; i < n; ++ i) {
      YmslAst* statement = mChildList[1]->child(i);
      statement->print(s);
    }
  }
  s << "}" << endl;
  {
    ymuint n = mChildList[2]->child_num();
    for (ymuint i = 0; i < n; ++ i) {
      YmslAst* elif = mChildList[2]->child(i);
      elif->print(s);
    }
  }
  if ( mChildList[3]->child_num() > 0 ) {
    s << "else {" << endl;
    ymuint n = mChildList[3]->child_num();
    for (ymuint i = 0; i < n; ++ i) {
      YmslAst* statement = mChildList[3]->child(i);
      statement->print(s);
    }
    s << "}" << endl;
  }
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstElif
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cond 条件
// @param[in] then_list then 文のリスト
// @param[in] loc ファイル位置
YmslAstElif::YmslAstElif(YmslAst* cond,
			 YmslAst* then_list,
			 const FileRegion& loc) :
  YmslAstImpl(loc)
{
  mChildList[0] = cond;
  mChildList[1] = then_list;
}

// @brief デストラクタ
YmslAstElif::~YmslAstElif()
{
}

// @brief 型を得る．
AstType
YmslAstElif::type() const
{
  return kAstElif;
}

// @brief 子供の数を返す．
ymuint
YmslAstElif::child_num() const
{
  return 2;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
YmslAst*
YmslAstElif::child(ymuint pos) const
{
  ASSERT_COND( pos < child_num() );
  return mChildList[pos];
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
YmslAstElif::print(ostream& s) const
{
  s << "elif ";
  mChildList[0]->print(s);
  s << " {" << endl;
  {
    ymuint n = mChildList[1]->child_num();
    for (ymuint i = 0; i < n; ++ i) {
      YmslAst* statement = mChildList[1]->child(i);
      statement->print(s);
    }
  }
  s << "}" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstFor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] init 初期化文
// @param[in] cond 条件式
// @param[in] next 増加文
// @param[in] body 本文
// @param[in] loc ファイル位置
YmslAstFor::YmslAstFor(YmslAst* init,
		       YmslAst* cond,
		       YmslAst* next,
		       YmslAst* body,
		       const FileRegion& loc) :
  YmslAstImpl(loc)
{
  mChildList[0] = init;
  mChildList[1] = cond;
  mChildList[2] = next;
  mChildList[3] = body;
}

// @brief デストラクタ
YmslAstFor::~YmslAstFor()
{
}

// @brief 型を得る．
AstType
YmslAstFor::type() const
{
  return kAstFor;
}

// @brief 子供の数を返す．
ymuint
YmslAstFor::child_num() const
{
  return 4;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
YmslAst*
YmslAstFor::child(ymuint pos) const
{
  ASSERT_COND( pos < child_num() );
  return mChildList[pos];
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
YmslAstFor::print(ostream& s) const
{
  s << "for (";
  mChildList[0]->print(s);
  s << "; ";
  mChildList[1]->print(s);
  s << "; ";
  mChildList[2]->print(s);
  s << ") {" << endl;
  {
    ymuint n = mChildList[3]->child_num();
    for (ymuint i = 0; i < n; ++ i) {
      YmslAst* statement = mChildList[3]->child(i);
      statement->print(s);
    }
  }
  s << "}" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstWhile
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cond 条件式
// @param[in] body 本文
// @param[in] loc ファイル位置
YmslAstWhile::YmslAstWhile(YmslAst* cond,
			   YmslAst* body,
			   const FileRegion& loc) :
  YmslAstImpl(loc)
{
  mChildList[0] = cond;
  mChildList[1] = body;
}

// @brief デストラクタ
YmslAstWhile::~YmslAstWhile()
{
}

// @brief 型を得る．
AstType
YmslAstWhile::type() const
{
  return kAstWhile;
}

// @brief 子供の数を返す．
ymuint
YmslAstWhile::child_num() const
{
  return 2;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
YmslAst*
YmslAstWhile::child(ymuint pos) const
{
  ASSERT_COND( pos < child_num() );
  return mChildList[pos];
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
YmslAstWhile::print(ostream& s) const
{
  s << "while ";
  mChildList[0]->print(s);
  s << " {" << endl;
  {
    ymuint n = mChildList[1]->child_num();
    for (ymuint i = 0; i < n; ++ i) {
      YmslAst* statement = mChildList[1]->child(i);
      statement->print(s);
    }
  }
  s << "}" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstDoWhile
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] body 本文
// @param[in] cond 条件式
// @param[in] loc ファイル位置
YmslAstDoWhile::YmslAstDoWhile(YmslAst* body,
			       YmslAst* cond,
			       const FileRegion& loc) :
  YmslAstImpl(loc)
{
  mChildList[0] = body;
  mChildList[1] = cond;
}

// @brief デストラクタ
YmslAstDoWhile::~YmslAstDoWhile()
{
}

// @brief 型を得る．
AstType
YmslAstDoWhile::type() const
{
  return kAstDoWhile;
}

// @brief 子供の数を返す．
ymuint
YmslAstDoWhile::child_num() const
{
  return 2;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
YmslAst*
YmslAstDoWhile::child(ymuint pos) const
{
  ASSERT_COND( pos < child_num() );
  return mChildList[pos];
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
YmslAstDoWhile::print(ostream& s) const
{
  s << "do {";
  {
    ymuint n = mChildList[0]->child_num();
    for (ymuint i = 0; i < n; ++ i) {
      YmslAst* statement = mChildList[0]->child(i);
      statement->print(s);
    }
  }
  s << "} while ";
  mChildList[1]->print(s);
  s << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstSwitch
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cond 条件式
// @param[in] case_list case リスト
// @param[in] loc ファイル位置
YmslAstSwitch::YmslAstSwitch(YmslAst* cond,
			     YmslAst* case_list,
			     const FileRegion& loc) :
  YmslAstImpl(loc)
{
  mChildList[0] = cond;
  mChildList[1] = case_list;
}

// @brief デストラクタ
YmslAstSwitch::~YmslAstSwitch()
{
}

// @brief 型を得る．
AstType
YmslAstSwitch::type() const
{
  return kAstSwitch;
}

// @brief 子供の数を返す．
ymuint
YmslAstSwitch::child_num() const
{
  return 2;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
YmslAst*
YmslAstSwitch::child(ymuint pos) const
{
  ASSERT_COND( pos < child_num() );
  return mChildList[pos];
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
YmslAstSwitch::print(ostream& s) const
{
  s << "switch ";
  mChildList[0]->print(s);
  s << "{" << endl;
  {
    ymuint n = mChildList[1]->child_num();
    for (ymuint i = 0; i < n; ++ i) {
      YmslAst* caseitem = mChildList[1]->child(i);
      caseitem->print(s);
    }
  }
  s << "}" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstCaseItem
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] statement_list 本文のリスト
// @param[in] loc ファイル位置
YmslAstCaseItem::YmslAstCaseItem(YmslAst* label,
				 YmslAst* statement_list,
				 const FileRegion& loc) :
  YmslAstImpl(loc)
{
  mChildList[0] = label;
  mChildList[1] = statement_list;
}

// @brief デストラクタ
YmslAstCaseItem::~YmslAstCaseItem()
{
}

// @brief 型を得る．
AstType
YmslAstCaseItem::type() const
{
  return kAstCaseItem;
}

// @brief 子供の数を返す．
ymuint
YmslAstCaseItem::child_num() const
{
  return 2;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
YmslAst*
YmslAstCaseItem::child(ymuint pos) const
{
  ASSERT_COND( pos < child_num() );
  return mChildList[pos];
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
YmslAstCaseItem::print(ostream& s) const
{
  if ( mChildList[0] != NULL ) {
    s << "case ";
    mChildList[0]->print(s);
  }
  else {
    s << "default ";
  }
  s << ": " << endl;
  {
    ymuint n = mChildList[1]->child_num();
    for (ymuint i = 0; i < n; ++ i) {
      YmslAst* statement = mChildList[1]->child(i);
      statement->print(s);
    }
  }
  s << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstGoto
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] loc ファイル位置
YmslAstGoto::YmslAstGoto(YmslAst* label,
			 const FileRegion& loc) :
  YmslAstImpl(loc)
{
  mLabel = label;
}

// @brief デストラクタ
YmslAstGoto::~YmslAstGoto()
{
}

// @brief 型を得る．
AstType
YmslAstGoto::type() const
{
  return kAstGoto;
}

// @brief 子供の数を返す．
ymuint
YmslAstGoto::child_num() const
{
  return 1;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
YmslAst*
YmslAstGoto::child(ymuint pos) const
{
  ASSERT_COND( pos < child_num() );
  return mLabel;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
YmslAstGoto::print(ostream& s) const
{
  s << "goto ";
  mLabel->print(s);
  s << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstLabel
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] loc ファイル位置
YmslAstLabel::YmslAstLabel(YmslAst* label,
			   const FileRegion& loc) :
  YmslAstImpl(loc)
{
  mLabel = label;
}

// @brief デストラクタ
YmslAstLabel::~YmslAstLabel()
{
}

// @brief 型を得る．
AstType
YmslAstLabel::type() const
{
  return kAstLabel;
}

// @brief 子供の数を返す．
ymuint
YmslAstLabel::child_num() const
{
  return 1;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
YmslAst*
YmslAstLabel::child(ymuint pos) const
{
  ASSERT_COND( pos < child_num() );
  return mLabel;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
YmslAstLabel::print(ostream& s) const
{
  mLabel->print(s);
  s << ":" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstBreak
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル位置
YmslAstBreak::YmslAstBreak(const FileRegion& loc) :
  YmslAstImpl(loc)
{
}

// @brief デストラクタ
YmslAstBreak::~YmslAstBreak()
{
}

// @brief 型を得る．
AstType
YmslAstBreak::type() const
{
  return kAstBreak;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
YmslAstBreak::print(ostream& s) const
{
  s << "break" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstContinue
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル位置
YmslAstContinue::YmslAstContinue(const FileRegion& loc) :
  YmslAstImpl(loc)
{
}

// @brief デストラクタ
YmslAstContinue::~YmslAstContinue()
{
}

// @brief 型を得る．
AstType
YmslAstContinue::type() const
{
  return kAstContinue;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
YmslAstContinue::print(ostream& s) const
{
  s << "continue" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstReturn
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] expr 値
// @param[in] loc ファイル位置
YmslAstReturn::YmslAstReturn(YmslAst* expr,
			     const FileRegion& loc) :
  YmslAstImpl(loc)
{
  mExpr = expr;
}

// @brief デストラクタ
YmslAstReturn::~YmslAstReturn()
{
}

// @brief 型を得る．
AstType
YmslAstReturn::type() const
{
  return kAstReturn;
}

// @brief 子供の数を返す．
ymuint
YmslAstReturn::child_num() const
{
  return 1;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
YmslAst*
YmslAstReturn::child(ymuint pos) const
{
  ASSERT_COND( pos < child_num() );
  return mExpr;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
YmslAstReturn::print(ostream& s) const
{
  s << "return ";
  mExpr->print(s);
  s << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstBlock
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] statement_list 文のリスト
// @param[in] loc ファイル位置
YmslAstBlock::YmslAstBlock(YmslAst* statement_list,
			   const FileRegion& loc) :
  YmslAstImpl(loc)
{
  mStatementList = statement_list;
}

// @brief デストラクタ
YmslAstBlock::~YmslAstBlock()
{
}

// @brief 型を得る．
AstType
YmslAstBlock::type() const
{
  return kAstBlock;
}

// @brief 子供の数を返す．
ymuint
YmslAstBlock::child_num() const
{
  return 1;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
YmslAst*
YmslAstBlock::child(ymuint pos) const
{
  ASSERT_COND( pos < child_num() );
  return mStatementList;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
YmslAstBlock::print(ostream& s) const
{
  s << "{" << endl;
  ymuint n = mStatementList->child_num();
  for (ymuint i = 0; i < n; ++ i) {
    YmslAst* statement = mStatementList->child(i);
    statement->print(s);
  }
  s << "}" << endl;
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
void
YmslAstIntType::print(ostream& s) const
{
  s << "int" << endl;
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
void
YmslAstFloatType::print(ostream& s) const
{
  s << "float" << endl;
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
void
YmslAstStringType::print(ostream& s) const
{
  s << "string" << endl;
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
void
YmslAstUserType::print(ostream& s) const
{
  mTypeName->print(s);
}

END_NAMESPACE_YM_YMSL
