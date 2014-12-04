
/// @file AstImpl.cc
/// @brief AstImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstVarDecl.h"
#include "AstFuncDecl.h"
#include "AstAssignment.h"
#include "AstIf.h"
#include "AstElif.h"
#include "AstFor.h"
#include "AstWhile.h"
#include "AstDoWhile.h"
#include "AstSwitch.h"
#include "AstCaseItem.h"
#include "AstGoto.h"
#include "AstLabel.h"
#include "AstBreak.h"
#include "AstContinue.h"
#include "AstReturn.h"
#include "AstBlockStmt.h"
#include "AstBlock.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstVarDecl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 変数名
// @param[in] type 型
// @param[in] init_expr 初期化式
// @param[in] loc ファイル位置
AstVarDecl::AstVarDecl(Ast* name,
			       Ast* type,
			       Ast* init_expr,
			       const FileRegion& loc) :
  AstImpl(loc)
{
  ASSERT_COND( name->type() == kAstSymbol );
  mName = name->str_val();

  mType = type;

  mInitExpr = init_expr;
}

// @brief デストラクタ
AstVarDecl::~AstVarDecl()
{
}

// @brief 型を得る．
AstType
AstVarDecl::type() const
{
  return kAstVarDecl;
}

// @brief 名前を返す．
ShString
AstVarDecl::name() const
{
  return mName;
}

// @brief 子供の数を返す．
ymuint
AstVarDecl::child_num() const
{
  return 2;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
Ast*
AstVarDecl::child(ymuint pos) const
{
  ASSERT_COND( pos < child_num() );
  switch ( pos ) {
  case 0: return mType;
  case 1: return mInitExpr;
  default: ASSERT_NOT_REACHED;
  }
  return NULL;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstVarDecl::print(ostream& s,
		      ymuint indent) const
{
  print_indent(s, indent);
  s << "var " << name() << ": ";
  mType->print(s);
  if ( mInitExpr != NULL ) {
    s << " = ";
    mInitExpr->print(s);
  }
  s << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス AstFuncDecl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 関数名
// @param[in] type 型
// @param[in] param_list パラメータリスト
// @param[in] block 本体のブロック
// @param[in] loc ファイル位置
AstFuncDecl::AstFuncDecl(Ast* name,
				 Ast* type,
				 Ast* param_list,
				 YmslBlock* block,
				 const FileRegion& loc) :
  AstImpl(loc)
{
  ASSERT_COND( name->type() == kAstSymbol );
  mName = name->str_val();

  mType = type;

  mParamList = param_list;

  mBlock = block;
}

// @brief デストラクタ
AstFuncDecl::~AstFuncDecl()
{
}

// @brief 型を得る．
AstType
AstFuncDecl::type() const
{
  return kAstFuncDecl;
}

// @brief 名前を返す．
ShString
AstFuncDecl::name() const
{
  return mName;
}

// @brief ブロックを返す．
//
// ブロックを持たない要素の場合 NULL を返す．
YmslBlock*
AstFuncDecl::block() const
{
  return mBlock;
}

// @brief 子供の数を返す．
ymuint
AstFuncDecl::child_num() const
{
  return 2;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
Ast*
AstFuncDecl::child(ymuint pos) const
{
  switch ( pos ) {
  case 0: return mType;
  case 1: return mParamList;
  default: ASSERT_NOT_REACHED;
  }
  return NULL;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstFuncDecl::print(ostream& s,
		       ymuint indent) const
{
  // 関数宣言はトップレベルなので indent は無視

  s << "function " << name() << ": ";
  mType->print(s);
  s << "(";
  {
    ymuint n = mParamList->child_num();
    const char* comma = "";
    for (ymuint i = 0; i < n; ++ i) {
      Ast* param = mParamList->child(i);
      s << comma;
      comma = ", ";
      param->print(s);
    }
  }
  s << ")";

  mBlock->print(s, 0);
}


//////////////////////////////////////////////////////////////////////
// クラス AstAssignment
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left 左辺
// @param[in] right 右辺
AstAssignment::AstAssignment(Ast* left,
				     Ast* right) :
  AstImpl(FileRegion(left->file_region(), right->file_region()))
{
  mChildList[0] = left;
  mChildList[1] = right;
}

// @brief デストラクタ
AstAssignment::~AstAssignment()
{
}

// @brief 型を得る．
AstType
AstAssignment::type() const
{
  return kAstAssignment;
}

// @brief 子供の数を返す．
ymuint
AstAssignment::child_num() const
{
  return 2;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
Ast*
AstAssignment::child(ymuint pos) const
{
  ASSERT_COND( pos < child_num() );
  return mChildList[pos];
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstAssignment::print(ostream& s,
			 ymuint indent) const
{
  print_indent(s, indent);
  mChildList[0]->print(s);
  s << " = ";
  mChildList[1]->print(s);
  s << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス AstIf
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cond 条件
// @param[in] then_block then ブロック
// @param[in] elif_list elif ブロックのリスト
// @param[in] else_block else ブロック
// @param[in] loc ファイル位置
AstIf::AstIf(Ast* cond,
		     YmslBlock* then_block,
		     Ast* elif_list,
		     YmslBlock* else_block,
		     const FileRegion& loc) :
  AstBlock(then_block, loc)
{
  mCond = cond;
}

// @brief デストラクタ
AstIf::~AstIf()
{
}

// @brief 型を得る．
AstType
AstIf::type() const
{
  return kAstIf;
}

// @brief 子供の数を返す．
ymuint
AstIf::child_num() const
{
  return 1;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
Ast*
AstIf::child(ymuint pos) const
{
  ASSERT_COND( pos < child_num() );
  return mCond;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstIf::print(ostream& s,
		 ymuint indent) const
{
#if 0
  print_indent(s, indent);
  s << "if ";
  mChildList[0]->print(s);
  s << " {" << endl;

  print_statement_list(s, mChildList[1], indent + 1);

  print_indent(s, indent);
  s << "}" << endl;

  {
    ymuint n = mChildList[2]->child_num();
    for (ymuint i = 0; i < n; ++ i) {
      Ast* elif = mChildList[2]->child(i);
      elif->print(s, indent);
    }
  }

  if ( mChildList[3] != NULL ) {
    print_indent(s, indent);
    s << "else {" << endl;

    print_statement_list(s, mChildList[3], indent + 1);

    print_indent(s, indent);
    s << "}" << endl;
  }
#endif
}


//////////////////////////////////////////////////////////////////////
// クラス AstElif
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cond 条件
// @param[in] block 本体
// @param[in] loc ファイル位置
AstElif::AstElif(Ast* cond,
			 YmslBlock* block,
			 const FileRegion& loc) :
  AstBlock(block, loc)
{
  mCond = cond;
}

// @brief デストラクタ
AstElif::~AstElif()
{
}

// @brief 型を得る．
AstType
AstElif::type() const
{
  return kAstElif;
}

// @brief 子供の数を返す．
ymuint
AstElif::child_num() const
{
  return 1;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
Ast*
AstElif::child(ymuint pos) const
{
  ASSERT_COND( pos < child_num() );
  return mCond;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstElif::print(ostream& s,
		   ymuint indent) const
{
#if 0
  print_indent(s, indent);
  s << "elif ";
  mChildList[0]->print(s);
  s << " {" << endl;

  print_statement_list(s, mChildList[1], indent + 1);

  print_indent(s, indent);
  s << "}" << endl;
#endif
}


//////////////////////////////////////////////////////////////////////
// クラス AstFor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] init 初期化文
// @param[in] cond 条件式
// @param[in] next 増加文
// @param[in] body 本文
// @param[in] loc ファイル位置
AstFor::AstFor(Ast* init,
		       Ast* cond,
		       Ast* next,
		       YmslBlock* block,
		       const FileRegion& loc) :
  AstBlock(block, loc)
{
  mInit = init;
  mCond = cond;
  mNext = next;
}

// @brief デストラクタ
AstFor::~AstFor()
{
}

// @brief 型を得る．
AstType
AstFor::type() const
{
  return kAstFor;
}

// @brief 子供の数を返す．
ymuint
AstFor::child_num() const
{
  return 3;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
Ast*
AstFor::child(ymuint pos) const
{
  switch ( pos ) {
  case 0: return mInit;
  case 1: return mCond;
  case 2: return mNext;
  default: ASSERT_NOT_REACHED;
  }
  return NULL;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstFor::print(ostream& s,
		  ymuint indent) const
{
  print_indent(s, indent);
  s << "for (";
  mInit->print(s);
  s << "; ";
  mCond->print(s);
  s << "; ";
  mNext->print(s);
  s << ")" << endl;

  block()->print(s, indent);
}


//////////////////////////////////////////////////////////////////////
// クラス AstWhile
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cond 条件式
// @param[in] body 本文
// @param[in] loc ファイル位置
AstWhile::AstWhile(Ast* cond,
			   YmslBlock* block,
			   const FileRegion& loc) :
  AstBlock(block, loc)
{
  mCond = cond;
}

// @brief デストラクタ
AstWhile::~AstWhile()
{
}

// @brief 型を得る．
AstType
AstWhile::type() const
{
  return kAstWhile;
}

// @brief 子供の数を返す．
ymuint
AstWhile::child_num() const
{
  return 1;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
Ast*
AstWhile::child(ymuint pos) const
{
  ASSERT_COND( pos < child_num() );
  return mCond;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstWhile::print(ostream& s,
		    ymuint indent) const
{
  print_indent(s, indent);
  s << "while ";
  mCond->print(s);

  block()->print(s, indent);
}


//////////////////////////////////////////////////////////////////////
// クラス AstDoWhile
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] block 本体
// @param[in] cond 条件式
// @param[in] loc ファイル位置
AstDoWhile::AstDoWhile(YmslBlock* block,
			       Ast* cond,
			       const FileRegion& loc) :
  AstBlock(block, loc)
{
  mCond = cond;
}

// @brief デストラクタ
AstDoWhile::~AstDoWhile()
{
}

// @brief 型を得る．
AstType
AstDoWhile::type() const
{
  return kAstDoWhile;
}

// @brief 子供の数を返す．
ymuint
AstDoWhile::child_num() const
{
  return 1;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
Ast*
AstDoWhile::child(ymuint pos) const
{
  ASSERT_COND( pos < child_num() );
  return mCond;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstDoWhile::print(ostream& s,
		      ymuint indent) const
{
  print_indent(s, indent);
  s << "do" << endl;

  block()->print(s, indent);

  print_indent(s, indent);
  s << "while ";
  mCond->print(s);
  s << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス AstSwitch
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cond 条件式
// @param[in] case_list case リスト
// @param[in] loc ファイル位置
AstSwitch::AstSwitch(Ast* cond,
			     Ast* case_list,
			     const FileRegion& loc) :
  AstImpl(loc)
{
  mChildList[0] = cond;
  mChildList[1] = case_list;
}

// @brief デストラクタ
AstSwitch::~AstSwitch()
{
}

// @brief 型を得る．
AstType
AstSwitch::type() const
{
  return kAstSwitch;
}

// @brief 子供の数を返す．
ymuint
AstSwitch::child_num() const
{
  return 2;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
Ast*
AstSwitch::child(ymuint pos) const
{
  ASSERT_COND( pos < child_num() );
  return mChildList[pos];
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstSwitch::print(ostream& s,
		     ymuint indent) const
{
  print_indent(s, indent);
  s << "switch ";
  mChildList[0]->print(s);
  s << "{" << endl;

  {
    ymuint n = mChildList[1]->child_num();
    for (ymuint i = 0; i < n; ++ i) {
      Ast* caseitem = mChildList[1]->child(i);
      caseitem->print(s, indent + 1);
    }
  }

  print_indent(s, indent);
  s << "}" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス AstCaseItem
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] block 本体
// @param[in] loc ファイル位置
AstCaseItem::AstCaseItem(Ast* label,
				 YmslBlock* block,
				 const FileRegion& loc) :
  AstBlock(block, loc)
{
  mLabel = label;
}

// @brief デストラクタ
AstCaseItem::~AstCaseItem()
{
}

// @brief 型を得る．
AstType
AstCaseItem::type() const
{
  return kAstCaseItem;
}

// @brief 子供の数を返す．
ymuint
AstCaseItem::child_num() const
{
  return 1;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
Ast*
AstCaseItem::child(ymuint pos) const
{
  ASSERT_COND( pos < child_num() );
  return mLabel;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstCaseItem::print(ostream& s,
		       ymuint indent) const
{
  print_indent(s, indent);
  if ( mLabel != NULL ) {
    s << "case ";
    mLabel->print(s);
  }
  else {
    s << "default ";
  }
  s << ": " << endl;

  block()->print(s, indent);

  s << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス AstGoto
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] loc ファイル位置
AstGoto::AstGoto(Ast* label,
			 const FileRegion& loc) :
  AstImpl(loc)
{
  mLabel = label;
}

// @brief デストラクタ
AstGoto::~AstGoto()
{
}

// @brief 型を得る．
AstType
AstGoto::type() const
{
  return kAstGoto;
}

// @brief 子供の数を返す．
ymuint
AstGoto::child_num() const
{
  return 1;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
Ast*
AstGoto::child(ymuint pos) const
{
  ASSERT_COND( pos < child_num() );
  return mLabel;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstGoto::print(ostream& s,
		   ymuint indent) const
{
  print_indent(s, indent);
  s << "goto ";
  mLabel->print(s);
  s << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス AstLabel
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] loc ファイル位置
AstLabel::AstLabel(Ast* label,
			   const FileRegion& loc) :
  AstImpl(loc)
{
  mLabel = label;
}

// @brief デストラクタ
AstLabel::~AstLabel()
{
}

// @brief 型を得る．
AstType
AstLabel::type() const
{
  return kAstLabel;
}

// @brief 子供の数を返す．
ymuint
AstLabel::child_num() const
{
  return 1;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
Ast*
AstLabel::child(ymuint pos) const
{
  ASSERT_COND( pos < child_num() );
  return mLabel;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstLabel::print(ostream& s,
		    ymuint indent) const
{
  print_indent(s, indent);
  mLabel->print(s);
  s << ":" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス AstBreak
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル位置
AstBreak::AstBreak(const FileRegion& loc) :
  AstImpl(loc)
{
}

// @brief デストラクタ
AstBreak::~AstBreak()
{
}

// @brief 型を得る．
AstType
AstBreak::type() const
{
  return kAstBreak;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstBreak::print(ostream& s,
		    ymuint indent) const
{
  print_indent(s, indent);
  s << "break" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス AstContinue
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル位置
AstContinue::AstContinue(const FileRegion& loc) :
  AstImpl(loc)
{
}

// @brief デストラクタ
AstContinue::~AstContinue()
{
}

// @brief 型を得る．
AstType
AstContinue::type() const
{
  return kAstContinue;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstContinue::print(ostream& s,
		       ymuint indent) const
{
  print_indent(s, indent);
  s << "continue" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス AstReturn
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] expr 値
// @param[in] loc ファイル位置
AstReturn::AstReturn(Ast* expr,
			     const FileRegion& loc) :
  AstImpl(loc)
{
  mExpr = expr;
}

// @brief デストラクタ
AstReturn::~AstReturn()
{
}

// @brief 型を得る．
AstType
AstReturn::type() const
{
  return kAstReturn;
}

// @brief 子供の数を返す．
ymuint
AstReturn::child_num() const
{
  return 1;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
Ast*
AstReturn::child(ymuint pos) const
{
  ASSERT_COND( pos < child_num() );
  return mExpr;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstReturn::print(ostream& s,
		     ymuint indent) const
{
  print_indent(s, indent);
  s << "return ";
  mExpr->print(s);
  s << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス AstBlock
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] block 本体
// @param[in] loc ファイル位置
AstBlock::AstBlock(YmslBlock* block,
			   const FileRegion& loc) :
  AstImpl(loc),
  mBlock(block)
{
}

// @brief デストラクタ
AstBlock::~AstBlock()
{
}

// @brief 型を得る．
AstType
AstBlock::type() const
{
  return kAstBlock;
}

// @brief ブロックを返す．
//
// ブロックを持たない要素の場合 NULL を返す．
YmslBlock*
AstBlock::block() const
{
  return mBlock;
}

// @brief 子供の数を返す．
ymuint
AstBlock::child_num() const
{
  return 0;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
Ast*
AstBlock::child(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstBlock::print(ostream& s,
		    ymuint indent) const
{
  block()->print(s, indent);
}

END_NAMESPACE_YM_YMSL
