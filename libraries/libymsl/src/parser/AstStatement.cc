
/// @file AstImpl.cc
/// @brief AstImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstAssignment.h"
#include "AstBlockStmt.h"
#include "AstBreak.h"
#include "AstCaseItem.h"
#include "AstContinue.h"
#include "AstDoWhile.h"
#include "AstExpr.h"
#include "AstExprStmt.h"
#include "AstFor.h"
#include "AstFuncDecl.h"
#include "AstGoto.h"
#include "AstIf.h"
#include "AstIfBlock.h"
#include "AstImport.h"
#include "AstLabel.h"
#include "AstList.h"
#include "AstModule.h"
#include "AstPrimary.h"
#include "AstReturn.h"
#include "AstSwitch.h"
#include "AstSymbol.h"
#include "AstVarDecl.h"
#include "AstWhile.h"
#include "YmslCodeList.h"
#include "YmslDriver.h"
#include "YmslLabel.h"
#include "YmslVM.h"


BEGIN_NAMESPACE_YM_YMSL

#include "grammer.hh"

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

// @brief ラベルステートメントの場合に名前を返す．
//
// それ以外では ShString() を返す．
ShString
AstStatement::label() const
{
  return ShString();
}

// @brief インデントする．
// @param[in] s 出力ストリーム
// @param[in] indent インデント量
void
AstStatement::print_indent(ostream& s,
			   ymuint indent)
{
  for (ymuint i = 0; i < indent; ++ i) {
    s << "  ";
  }
}


//////////////////////////////////////////////////////////////////////
// クラス AstImport
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module_list モジュールのリスト
// @param[in] loc ファイル位置
AstImport::AstImport(AstModuleList* module_list,
		     const FileRegion& loc) :
  AstStatement(loc),
  mModuleList(module_list->size())
{
  ymuint pos = 0;
  for (AstModuleList::Iterator p = module_list->begin();
       !p.is_end(); p.next()) {
    mModuleList[pos] = *p;
    ++ pos;
  }
}

// @brief デストラクタ
AstImport::~AstImport()
{
}

// @brief 命令コードのサイズを計算する．
ymuint
AstImport::calc_size()
{
  return 0;
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstImport::compile(YmslDriver& driver,
		   YmslCodeList& code_list,
		   Ymsl_INT& addr)
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstImport::print(ostream& s,
		 ymuint indent) const
{
  print_indent(s, indent);
  s << "import ";
  ymuint n = mModuleList.size();
  const char* comma = "";
  for (ymuint i = 0; i < n; ++ i) {
    AstModule* module = mModuleList[i];
    s << comma << module->module_name()->str_val();
    if ( module->alias_name() != NULL ) {
      s << " as " << module->alias_name()->str_val();
    }
    comma = ", ";
  }
  s << ";" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス AstAssignment
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] token トークン
// @param[in] left 左辺
// @param[in] right 右辺
AstAssignment::AstAssignment(TokenType token,
			     AstPrimary* left,
			     AstExpr* right) :
  AstStatement(FileRegion(left->file_region(), right->file_region())),
  mToken(token),
  mLeft(left),
  mRight(right)
{
}

// @brief デストラクタ
AstAssignment::~AstAssignment()
{
}

// @brief 命令コードのサイズを計算する．
ymuint
AstAssignment::calc_size()
{
  ymuint size = mRight->calc_size();
  size += 2;
  return size;
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstAssignment::compile(YmslDriver& driver,
		       YmslCodeList& code_list,
		       Ymsl_INT& addr)
{
  mRight->compile(driver, code_list, addr);

  Ymsl_CODE op = mLeft->opcode();
  code_list.write_opcode(addr, op);
  code_list.write_int(addr, mLeft->var()->index());
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
  switch ( mToken ) {
  case EQ:       s << " = "; break;
  case EQPLUS:   s << " += "; break;
  case EQMINUS:  s << " -= "; break;
  case EQMULT:   s << " *= "; break;
  case EQDIV:    s << " /= "; break;
  case EQMOD:    s << " %= "; break;
  case EQLSHIFT: s << " <<= "; break;
  case EQRSHIFT: s << " >>= "; break;
  case EQBITAND: s << " &= "; break;
  case EQBITOR:  s << " |= "; break;
  case EQBITXOR: s << " ^= "; break;
  default: ASSERT_NOT_REACHED;
  }
  mRight->print(s);
  s << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス AstIf
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] if_list IfBlock のリスト
// @param[in] loc ファイル位置
AstIf::AstIf(AstIfList* if_list,
	     const FileRegion& loc) :
  AstStatement(loc),
  mIfBlockList(if_list->size())
{
  ymuint pos = 0;
  for (AstIfList::Iterator p = if_list->begin();
       !p.is_end(); p.next()) {
    mIfBlockList[pos] = *p;
    ++ pos;
  }
}

// @brief デストラクタ
AstIf::~AstIf()
{
}

// @brief 命令コードのサイズを計算する．
ymuint
AstIf::calc_size()
{
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstIf::compile(YmslDriver& driver,
	       YmslCodeList& code_list,
	       Ymsl_INT& addr)
{
  ymuint n = mIfBlockList.size();
  vector<Ymsl_INT> addr_list;
  for (ymuint i = 0; i < n; ++ i) {
    AstIfBlock* if_block = mIfBlockList[i];
    AstExpr* cond = if_block->cond();
    if ( cond != NULL ) {
      cond->compile(driver, code_list, addr);
      code_list.write_opcode(addr, YMVM_BRANCH_FALSE);
      Ymsl_INT label1_addr = addr;
      code_list.write_int(addr, 0);
      if_block->compile(driver, code_list, addr);
      if ( i < n - 1 ) {
	code_list.write_opcode(addr, YMVM_JUMP);
	addr_list.push_back(addr);
	code_list.write_int(addr, 0);
      }
      code_list.write_int(label1_addr, addr);
    }
    else {
      ASSERT_COND( i == n - 1 );
      if_block->compile(driver, code_list, addr);
    }
  }
  for (vector<Ymsl_INT>::iterator p = addr_list.begin();
       p != addr_list.end(); ++ p) {
    Ymsl_INT label_addr = *p;
    code_list.write_int(label_addr, addr);
  }
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
    if_block->print(s, indent + 1);
    print_indent(s, indent);
    s << "}" << endl;
  }
}


//////////////////////////////////////////////////////////////////////
// クラス AstIfBlock
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cond 条件
// @param[in] stmt_list 本体の文
// @param[in] loc ファイル位置
AstIfBlock::AstIfBlock(AstExpr* cond,
		       AstStmtList* stmt_list,
		       const FileRegion& loc) :
  AstBlockStmt(stmt_list, loc),
  mCond(cond)
{
}

// @brief デストラクタ
AstIfBlock::~AstIfBlock()
{
}

// @brief 条件を返す．
AstExpr*
AstIfBlock::cond() const
{
  return mCond;
}


//////////////////////////////////////////////////////////////////////
// クラス AstFor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] init 初期化文
// @param[in] cond 条件式
// @param[in] next 増加文
// @param[in] stmt_list 本体の文
// @param[in] loc ファイル位置
AstFor::AstFor(AstStatement* init,
	       AstExpr* cond,
	       AstStatement* next,
	       AstStmtList* stmt_list,
	       const FileRegion& loc) :
  AstBlockStmt(stmt_list, loc),
  mInit(init),
  mCond(cond),
  mNext(next)
{
}

// @brief デストラクタ
AstFor::~AstFor()
{
}

// @brief 命令コードのサイズを計算する．
ymuint
AstFor::calc_size()
{
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstFor::compile(YmslDriver& driver,
		YmslCodeList& code_list,
		Ymsl_INT& addr)
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
// @param[in] stmt_list 本体の文
// @param[in] loc ファイル位置
AstWhile::AstWhile(AstExpr* cond,
		   AstStmtList* stmt_list,
		   const FileRegion& loc) :
  AstBlockStmt(stmt_list, loc),
  mCond(cond)
{
}

// @brief デストラクタ
AstWhile::~AstWhile()
{
}

// @brief 命令コードのサイズを計算する．
ymuint
AstWhile::calc_size()
{
  ymuint size = mCond->calc_size();
  size += AstBlockStmt::calc_size();
  size += 4;
  return size;
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstWhile::compile(YmslDriver& driver,
		  YmslCodeList& code_list,
		  Ymsl_INT& addr)
{
  Ymsl_INT label1 = addr;
  mCond->compile(driver, code_list, addr);
  code_list.write_opcode(addr, YMVM_BRANCH_FALSE);
  Ymsl_INT label2_addr = addr;
  code_list.write_int(addr, 0);
  AstBlockStmt::compile(driver, code_list, addr);
  code_list.write_opcode(addr, YMVM_JUMP);
  code_list.write_int(addr, label1);
  code_list.write_int(label2_addr, addr);
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
  AstBlockStmt::print(s, indent + 1);
  s << "}" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス AstDoWhile
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] stmt_list 本体の文
// @param[in] cond 条件式
// @param[in] loc ファイル位置
AstDoWhile::AstDoWhile(AstStmtList* stmt_list,
		       AstExpr* cond,
		       const FileRegion& loc) :
  AstBlockStmt(stmt_list, loc),
  mCond(cond)
{
}

// @brief デストラクタ
AstDoWhile::~AstDoWhile()
{
}

// @brief 命令コードのサイズを計算する．
ymuint
AstDoWhile::calc_size()
{
  ymuint size = AstBlockStmt::calc_size();
  size += mCond->calc_size();
  size += 2;
  return size;
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstDoWhile::compile(YmslDriver& driver,
		    YmslCodeList& code_list,
		    Ymsl_INT& addr)
{
  Ymsl_INT label1 = addr;
  AstBlockStmt::compile(driver, code_list, addr);
  mCond->compile(driver, code_list, addr);
  code_list.write_opcode(addr, YMVM_BRANCH_TRUE);
  code_list.write_int(addr, label1);
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

  AstBlockStmt::print(s, indent + 1);

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
		     AstCaseList* case_list,
		     const FileRegion& loc) :
  AstStatement(loc),
  mExpr(expr),
  mCaseItemList(case_list->size())
{
  ymuint pos = 0;
  for (AstCaseList::Iterator p = case_list->begin();
       !p.is_end(); p.next()) {
    mCaseItemList[pos] = *p;
    ++ pos;
  }
}

// @brief デストラクタ
AstSwitch::~AstSwitch()
{
}

// @brief 命令コードのサイズを計算する．
ymuint
AstSwitch::calc_size()
{
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstSwitch::compile(YmslDriver& driver,
		   YmslCodeList& code_list,
		   Ymsl_INT& addr)
{
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
  mExpr->print(s);
  s << "{" << endl;

  ymuint n = mCaseItemList.size();
  for (ymuint i = 0; i < n; ++ i) {
    AstCaseItem* caseitem = mCaseItemList[i];
    caseitem->print(s, indent + 1);
  }

  print_indent(s, indent);
  s << "}" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス AstCaseItem
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] stmt_list 本体の文
// @param[in] loc ファイル位置
AstCaseItem::AstCaseItem(AstExpr* label,
			 AstStmtList* stmt_list,
			 const FileRegion& loc) :
  AstBlockStmt(stmt_list, loc),
  mLabel(label)
{
}

// @brief デストラクタ
AstCaseItem::~AstCaseItem()
{
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

  AstBlockStmt::print(s, indent);

  s << endl;
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

// @brief 命令コードのサイズを計算する．
ymuint
AstGoto::calc_size()
{
  return 2;
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstGoto::compile(YmslDriver& driver,
		 YmslCodeList& code_list,
		 Ymsl_INT& addr)
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstGoto::print(ostream& s,
	       ymuint indent) const
{
  print_indent(s, indent);
  s << "goto " << mLabel->str_val() << endl;
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

// @brief 命令コードのサイズを計算する．
ymuint
AstLabel::calc_size()
{
  return 0;
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstLabel::compile(YmslDriver& driver,
		  YmslCodeList& code_list,
		  Ymsl_INT& addr)
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstLabel::print(ostream& s,
		ymuint indent) const
{
  print_indent(s, indent);
  s << mLabel->str_val() << ":" << endl;
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

// @brief 命令コードのサイズを計算する．
ymuint
AstBreak::calc_size()
{
  return 2;
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstBreak::compile(YmslDriver& driver,
		  YmslCodeList& code_list,
		  Ymsl_INT& addr)
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

// @brief 命令コードのサイズを計算する．
ymuint
AstContinue::calc_size()
{
  return 2;
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstContinue::compile(YmslDriver& driver,
		     YmslCodeList& code_list,
		     Ymsl_INT& addr)
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

// @brief 命令コードのサイズを計算する．
ymuint
AstReturn::calc_size()
{
  ymuint size = 1;
  if ( mExpr != NULL ) {
    size += mExpr->calc_size();
  }
  return size;
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstReturn::compile(YmslDriver& driver,
		   YmslCodeList& code_list,
		   Ymsl_INT& addr)
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
// @param[in] stmt_list 本体の文
// @param[in] loc ファイル位置
AstBlockStmt::AstBlockStmt(AstStmtList* stmt_list,
			   const FileRegion& loc) :
  AstStatement(loc),
  mStmtList(stmt_list->size())
{
  ymuint pos = 0;
  for (AstStmtList::Iterator p = stmt_list->begin();
       !p.is_end(); p.next()) {
    mStmtList[pos] = *p;
    ++ pos;
  }
}

// @brief デストラクタ
AstBlockStmt::~AstBlockStmt()
{
}

// @brief 命令コードのサイズを計算する．
ymuint
AstBlockStmt::calc_size()
{
  ymuint size = 0;
  for (ymuint pos = 0; pos < mStmtList.size(); ++ pos) {
    size += mStmtList[pos]->calc_size();
  }
  return size;
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstBlockStmt::compile(YmslDriver& driver,
		      YmslCodeList& code_list,
		      Ymsl_INT& addr)
{
  for (ymuint pos = 0; pos < mStmtList.size(); ++ pos) {
    mStmtList[pos]->compile(driver, code_list, addr);
  }
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstBlockStmt::print(ostream& s,
		    ymuint indent) const
{
  ymuint n = mStmtList.size();
  for (ymuint i = 0; i < n; ++ i) {
    AstStatement* stmt = mStmtList[i];
    stmt->print(s, indent);
  }
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

// @brief 命令コードのサイズを計算する．
ymuint
AstExprStmt::calc_size()
{
  return mExpr->calc_size();
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstExprStmt::compile(YmslDriver& driver,
		     YmslCodeList& code_list,
		     Ymsl_INT& addr)
{
  mExpr->compile(driver, code_list, addr);
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
