
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
#include "AstIfBlock.h"
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
#include "AstExprStmt.h"
#include "AstBlock.h"
#include "AstExpr.h"


BEGIN_NAMESPACE_YM_YMSL

BEGIN_NONAMESPACE

void
print_indent(ostream& s,
	    ymuint indent)
{
  for (ymuint i = 0; i < indent; ++ i) {
    s << "  ";
  }
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス AstStatement
//////////////////////////////////////////////////////////////////////


// @brief コンストラクタ
// @param[in] loc ファイル位置
AstStatement::AstStatement(const FileRegion& loc) :
  Ast(loc)
{
}

// @brief デストラクタ
AstStatement::~AstStatement()
{
}

// @brief ブロックを返す．
//
// ブロックを持たない要素の場合 NULL を返す．
AstBlock*
AstStatement::block() const
{
  return NULL;
}

// @brief ラベルステートメントの場合に名前を返す．
//
// それ以外では ShString() を返す．
ShString
AstStatement::label() const
{
  return ShString();
}


//////////////////////////////////////////////////////////////////////
// クラス AstAssignment
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left 左辺
// @param[in] right 右辺
AstAssignment::AstAssignment(AstExpr* left,
			     AstExpr* right) :
  AstStatement(FileRegion(left->file_region(), right->file_region())),
  mLeft(left),
  mRight(right)
{
}

// @brief デストラクタ
AstAssignment::~AstAssignment()
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstAssignment::print(ostream& s,
		     ymuint indent) const
{
  print_indent(s, indent);
  mLeft->print(s);
  s << " = ";
  mRight->print(s);
  s << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス AstIf
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

ymuint
count_size(AstIfBlock* block_list)
{
  ymuint n = 0;
  for (AstIfBlock* ib = block_list; ib != NULL; ib = ib->next()) {
    ++ n;
  }
  return n;
}

END_NONAMESPACE

// @brief コンストラクタ
// @param[in] top 先頭の if ブロック
// @param[in] elif_list elif ブロックリスト
// @param[in] loc ファイル位置
AstIf::AstIf(AstIfBlock* top,
	     AstIfBlock* elif_list,
	     const FileRegion& loc) :
  AstStatement(loc),
  mIfBlockList(count_size(elif_list) + 1)
{
  ASSERT_COND( top != NULL );
  mIfBlockList[0] = top;
  ymuint i = mIfBlockList.size();
  for (AstIfBlock* ib = elif_list; ib != NULL; ib = ib->next()) {
    -- i;
    mIfBlockList[i] = ib;
  }
}

// @brief デストラクタ
AstIf::~AstIf()
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstIf::print(ostream& s,
	     ymuint indent) const
{
  ymuint n = mIfBlockList.size();
  for (ymuint i = 0; i < n; ++ i) {
    AstIfBlock* if_block = mIfBlockList[i];
    AstExpr* cond = if_block->cond();
    print_indent(s, indent);
    if ( cond != NULL ) {
      if ( i == 0 ) {
	s << "if ";
      }
      else {
	s << "elif ";
      }
      cond->print(s);
    }
    else {
      s << "else";
    }
    s << " {" << endl;
    AstBlock* block = if_block->block();
    block->print(s, indent + 1);
    print_indent(s, indent);
    s << "}" << endl;
  }
}


//////////////////////////////////////////////////////////////////////
// クラス AstIfBlock
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cond 条件
// @param[in] block 本体
// @param[in] loc ファイル位置
AstIfBlock::AstIfBlock(AstExpr* cond,
		       AstBlock* block,
		       const FileRegion& loc) :
  AstBlockStmt(block, loc),
  mCond(cond),
  mNext(NULL)
{
}

// @brief デストラクタ
AstIfBlock::~AstIfBlock()
{
}

// @brief 次の要素を得る．
AstIfBlock*
AstIfBlock::next() const
{
  return mNext;
}

// @brief 次の要素をセットする．
void
AstIfBlock::set_prev(AstIfBlock* prev)
{
  mNext = prev;
}

// @brief 条件を返す．
AstExpr*
AstIfBlock::cond() const
{
  return mCond;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstIfBlock::print(ostream& s,
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
AstFor::AstFor(AstStatement* init,
	       AstExpr* cond,
	       AstStatement* next,
	       AstBlock* block,
	       const FileRegion& loc) :
  AstBlockStmt(block, loc),
  mInit(init),
  mCond(cond),
  mNext(next)
{
}

// @brief デストラクタ
AstFor::~AstFor()
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstFor::print(ostream& s,
	      ymuint indent) const
{
#if 0
  print_indent(s, indent);
  s << "for (";
  mInit->print(s);
  s << "; ";
  mCond->print(s);
  s << "; ";
  mNext->print(s);
  s << ")" << endl;

  block()->print(s, indent);
#endif
}


//////////////////////////////////////////////////////////////////////
// クラス AstWhile
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cond 条件式
// @param[in] body 本文
// @param[in] loc ファイル位置
AstWhile::AstWhile(AstExpr* cond,
		   AstBlock* block,
		   const FileRegion& loc) :
  AstBlockStmt(block, loc),
  mCond(cond)
{
}

// @brief デストラクタ
AstWhile::~AstWhile()
{
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
  s << " {" << endl;
  block()->print(s, indent + 1);
  s << "}" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス AstDoWhile
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] block 本体
// @param[in] cond 条件式
// @param[in] loc ファイル位置
AstDoWhile::AstDoWhile(AstBlock* block,
		       AstExpr* cond,
		       const FileRegion& loc) :
  AstBlockStmt(block, loc),
  mCond(cond)
{
}

// @brief デストラクタ
AstDoWhile::~AstDoWhile()
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstDoWhile::print(ostream& s,
		  ymuint indent) const
{
  print_indent(s, indent);
  s << "do {" << endl;

  block()->print(s, indent + 1);

  print_indent(s, indent);
  s << "while ";
  mCond->print(s);
  s << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス AstSwitch
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] expr 条件式
// @param[in] case_list case リスト
// @param[in] loc ファイル位置
AstSwitch::AstSwitch(AstExpr* expr,
		     AstCaseItem* case_list,
		     const FileRegion& loc) :
  AstStatement(loc),
  mExpr(expr)
{
}

// @brief デストラクタ
AstSwitch::~AstSwitch()
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstSwitch::print(ostream& s,
		 ymuint indent) const
{
#if 0
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
#endif
}


//////////////////////////////////////////////////////////////////////
// クラス AstCaseItem
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] block 本体
// @param[in] loc ファイル位置
AstCaseItem::AstCaseItem(AstExpr* label,
			 AstBlock* block,
			 const FileRegion& loc) :
  AstBlockStmt(block, loc),
  mLabel(label),
  mNext(NULL)
{
}

// @brief デストラクタ
AstCaseItem::~AstCaseItem()
{
}

// @brief 次の要素を得る．
AstCaseItem*
AstCaseItem::next() const
{
  return mNext;
}

// @brief 前の要素をセットする．
void
AstCaseItem::set_prev(AstCaseItem* prev)
{
  if ( prev != NULL ) {
    prev->mNext = this;
  }
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstCaseItem::print(ostream& s,
		   ymuint indent) const
{
#if 0
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
#endif
}


//////////////////////////////////////////////////////////////////////
// クラス AstGoto
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] loc ファイル位置
AstGoto::AstGoto(AstSymbol* label,
		 const FileRegion& loc) :
  AstStatement(loc),
  mLabel(label)
{
}

// @brief デストラクタ
AstGoto::~AstGoto()
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstGoto::print(ostream& s,
	       ymuint indent) const
{
#if 0
  print_indent(s, indent);
  s << "goto ";
  mLabel->print(s);
  s << endl;
#endif
}


//////////////////////////////////////////////////////////////////////
// クラス AstLabel
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] loc ファイル位置
AstLabel::AstLabel(AstSymbol* label,
		   const FileRegion& loc) :
  AstStatement(loc),
  mLabel(label)
{
}

// @brief デストラクタ
AstLabel::~AstLabel()
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstLabel::print(ostream& s,
		ymuint indent) const
{
#if 0
  print_indent(s, indent);
  mLabel->print(s);
  s << ":" << endl;
#endif
}


//////////////////////////////////////////////////////////////////////
// クラス AstBreak
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル位置
AstBreak::AstBreak(const FileRegion& loc) :
  AstStatement(loc)
{
}

// @brief デストラクタ
AstBreak::~AstBreak()
{
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
  AstStatement(loc)
{
}

// @brief デストラクタ
AstContinue::~AstContinue()
{
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
AstReturn::AstReturn(AstExpr* expr,
		     const FileRegion& loc) :
  AstStatement(loc),
  mExpr(expr)
{
}

// @brief デストラクタ
AstReturn::~AstReturn()
{
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
  if ( mExpr != NULL ) {
    mExpr->print(s);
  }
  s << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス AstBlockStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] block 本体
// @param[in] loc ファイル位置
AstBlockStmt::AstBlockStmt(AstBlock* block,
			   const FileRegion& loc) :
  AstStatement(loc),
  mBlock(block)
{
}

// @brief デストラクタ
AstBlockStmt::~AstBlockStmt()
{
}

// @brief ブロックを返す．
//
// ブロックを持たない要素の場合 NULL を返す．
AstBlock*
AstBlockStmt::block() const
{
  return mBlock;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstBlockStmt::print(ostream& s,
		    ymuint indent) const
{
  print_indent(s, indent);
  s << "{" << endl;
  block()->print(s, indent + 1);
  print_indent(s, indent);
  s << "}" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス AstExprStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] expr 式
AstExprStmt::AstExprStmt(AstExpr* expr) :
  AstStatement(expr->file_region()),
  mExpr(expr)
{
}

// @brief デストラクタ
AstExprStmt::~AstExprStmt()
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstExprStmt::print(ostream& s,
		   ymuint indent) const
{
  print_indent(s, indent);
  mExpr->print(s);
  s << endl;
}

END_NAMESPACE_YM_YMSL
