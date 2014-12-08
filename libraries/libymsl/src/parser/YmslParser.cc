
/// @file YmslParser.cc
/// @brief YmslParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslParser.h"
#include "YmslScanner.h"

#include "AstSymbol.h"
#include "AstBlock.h"
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
#include "AstFuncCall.h"
#include "AstUniOp.h"
#include "AstBinOp.h"
#include "AstVarExpr.h"
#include "AstIntConst.h"
#include "AstFloatConst.h"
#include "AstStringConst.h"
#include "AstIntType.h"
#include "AstFloatType.h"
#include "AstStringType.h"
#include "AstUserType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslParser
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
YmslParser::YmslParser()
{
  mScanner = NULL;
  mToplevelBlock = NULL;
}

// @brief デストラクタ
YmslParser::~YmslParser()
{
  // 念の為
  delete mScanner;
}

// @brief 読み込む．
// @param[in] ido 入力データ
// @return 成功したら true を返す．
bool
YmslParser::read(IDO& ido)
{
  int yyparser(YmslParser&);

  mScanner = new YmslScanner(ido);

  mToplevelBlock = new AstBlock(NULL);
  mBlockStack.push_back(mToplevelBlock);

  int stat = yyparse(*this);

  delete mScanner;
  mScanner = NULL;

  return (stat == 0);
}

// @brief トップレベルブロックを返す．
AstBlock*
YmslParser::toplevel_block() const
{
  return mToplevelBlock;
}

// @brief yylex とのインターフェイス
// @param[out] lvalp 値を格納する変数
// @param[out] llocp 位置情報を格納する変数
// @return 読み込んだトークンの id を返す．
int
YmslParser::yylex(YYSTYPE& lval,
		  FileRegion& lloc)
{
  int id = mScanner->read_token(lloc);
  RsrvWordDic dic;

  switch ( id ) {
  case SYMBOL:
    lval.symbol_type = new AstSymbol(ShString(mScanner->cur_string()), lloc);
    break;

  case STRING_VAL:
    lval.expr_type = new_AstStringConst(mScanner->cur_string(), lloc);
    break;

  case INT_NUM:
    lval.int_type = mScanner->cur_int();
    break;

  case FLOAT_NUM:
    lval.float_type = mScanner->cur_float();
    break;

  default:
    break;
  }

  return id;
}

// @brief 現在のブロックを返す．
AstBlock*
YmslParser::cur_block()
{
  ASSERT_COND( !mBlockStack.empty() );
  return mBlockStack.back();
}

// @brief 新しいブロックを作りスタックに積む．
// @return 新しいブロックを返す．
AstBlock*
YmslParser::push_new_block()
{
  AstBlock* parent = cur_block();
  AstBlock* block = new AstBlock(parent);
  mBlockStack.push_back(block);
  return block;
}

// @brief ブロックをスタックから取り去る．
void
YmslParser::pop_block()
{
  mBlockStack.pop_back();
}

// @brief 変数宣言を作る．
// @param[in] name 変数名
// @param[in] type 型
// @param[in] init_expr 初期化式
// @param[in] loc ファイル位置
AstVarDecl*
YmslParser::new_AstVarDecl(AstSymbol* name,
			   AstValueType* type,
			   AstExpr* init_expr,
			   const FileRegion& loc)
{
  return new AstVarDecl(name->str_val(), type, init_expr, loc);
}

// @brief 関数宣言を作る．
// @param[in] name 変数名
// @param[in] type 型
// @param[in] param_list パラメータリスト
// @param[in] block 本体のブロック
// @param[in] loc ファイル位置
AstFuncDecl*
YmslParser::new_AstFuncDecl(AstSymbol* name,
			    AstValueType* type,
			    AstVarDecl* param_list,
			    const FileRegion& loc)
{
  AstBlock* block = cur_block();
  return new AstFuncDecl(name->str_val(), type, param_list, block, loc);
}

// @brief 代入文を作る．
// @param[in] left 左辺
// @param[in] right 右辺
AstStatement*
YmslParser::new_AstAssignment(AstExpr* left,
			      AstExpr* right)
{
  return new AstAssignment(left, right);
}

// @brief if 文を作る．
// @param[in] cond 条件式
// @param[in] then_block then ブロック
// @param[in] elif_list elif ブロックリスト
// @param[in] else_block else ブロック
// @param[in] loc ファイル位置
AstStatement*
YmslParser::new_AstIf(AstIfBlock* top,
		      AstIfBlock* elif_list,
		      AstIfBlock* else_block,
		      const FileRegion& loc)
{
  if ( else_block != NULL ) {
    else_block->set_prev(elif_list);
    elif_list = else_block;
  }
  return new AstIf(top, elif_list, loc);
}

// @brief if blockを作る．
// @param[in] cond 条件式
// @param[in] block 本体
// @param[in] loc ファイル位置
AstIfBlock*
YmslParser::new_AstIfBlock(AstExpr* cond,
			   AstBlock* block,
			   const FileRegion& loc)
{
  return new AstIfBlock(cond, block, loc);
}

// @brief for 文を作る．
// @param[in] init 初期化文
// @param[in] cond 条件式
// @param[in] next 増加文
// @param[in] block 本体
// @param[in] loc ファイル位置
AstStatement*
YmslParser::new_AstFor(AstStatement* init,
		       AstExpr* cond,
		       AstStatement* next,
		       AstBlock* block,
		       const FileRegion& loc)
{
  return new AstFor(init, cond, next, block, loc);
}

// @brief while 文を作る．
// @param[in] cond 条件式
// @param[in] block 本体
// @param[in] loc ファイル位置
AstStatement*
YmslParser::new_AstWhile(AstExpr* cond,
			 AstBlock* block,
			 const FileRegion& loc)
{
  return new AstWhile(cond, block, loc);
}

// @brief do-while 文を作る．
// @param[in] block 本体
// @param[in] cond 条件式
// @param[in] loc ファイル位置
AstStatement*
YmslParser::new_AstDoWhile(AstBlock* block,
			   AstExpr* cond,
			   const FileRegion& loc)
{
  return new AstDoWhile(block, cond, loc);
}

// @brief switch 文を作る．
// @param[in] expr 条件式
// @param[in] case_list caseリスト
// @param[in] loc ファイル位置
AstStatement*
YmslParser::new_AstSwitch(AstExpr* expr,
			  AstCaseItem* case_list,
			  const FileRegion& loc)
{
  return new AstSwitch(expr, case_list, loc);
}

// @brief case-item を作る．
// @param[in] label ラベル
// @param[in] block 本体
// @param[in] loc ファイル位置
AstCaseItem*
YmslParser::new_AstCaseItem(AstExpr* label,
			    AstBlock* block,
			    const FileRegion& loc)
{
  return new AstCaseItem(label, block, loc);
}

// @brief goto 文を作る．
// @param[in] label ラベル
// @param[in] loc ファイル位置
AstStatement*
YmslParser::new_AstGoto(AstSymbol* label,
			const FileRegion& loc)
{
  return new AstGoto(label, loc);
}

// @brief ラベルを作る．
// @param[in] label ラベル
// @param[in] loc ファイル位置
AstStatement*
YmslParser::new_AstLabel(AstSymbol* label,
			 const FileRegion& loc)
{
  return new AstLabel(label, loc);
}

// @brief break 文を作る．
// @param[in] loc ファイル位置
AstStatement*
YmslParser::new_AstBreak(const FileRegion& loc)
{
  return new AstBreak(loc);
}

// @brief continue 文を作る．
// @param[in] loc ファイル位置
AstStatement*
YmslParser::new_AstContinue(const FileRegion& loc)
{
  return new AstContinue(loc);
}

// @brief return 文を作る．
// @param[in] expr 値
// @param[in] loc ファイル位置
AstStatement*
YmslParser::new_AstReturn(AstExpr* expr,
			  const FileRegion& loc)
{
  return new AstReturn(expr, loc);
}

// @brief ブロック文を作る．
// @param[in] block 本体
// @param[in] loc ファイル位置
AstStatement*
YmslParser::new_AstBlockStmt(AstBlock* block,
			     const FileRegion& loc)
{
  return new AstBlockStmt(block, loc);
}

// @brief 式文を作る．
// @param[in] expr 式
AstStatement*
YmslParser::new_AstExprStmt(AstExpr* expr)
{
  return new AstExprStmt(expr);
}

// @brief 現在のブロックに statement を追加する．
void
YmslParser::add_statement(AstStatement* stmt)
{
  mBlockStack.back()->add_statement(stmt);
}

// @brief 単項演算式を作る．
// @param[in] op 演算子のトークン
// @param[in] left オペランド
// @param[in] loc ファイル位置
AstExpr*
YmslParser::new_AstUniOp(TokenType op,
			 AstExpr* left,
			 const FileRegion& loc)
{
  return new AstUniOp(op, left, loc);
}

// @brief 二項演算式を作る．
// @param[in] op 演算子のトークン
// @param[in] left, right オペランド
AstExpr*
YmslParser::new_AstBinOp(TokenType op,
			 AstExpr* left,
			 AstExpr* right)
{
  return new AstBinOp(op, left, right);
}

// @brief 配列参照を作る．
// @param[in] id 配列名
// @param[in] index インデックス
// @param[in] loc ファイル位置
AstExpr*
YmslParser::new_AstArrayRef(AstSymbol* id,
			    AstExpr* index,
			    const FileRegion& loc)
{
  return NULL;
}

// @brief 関数呼び出しを作る．
// @param[in] symbol 関数名
// @param[in] expr_list 引数のリスト
// @param[in] loc ファイル位置
AstExpr*
YmslParser::new_AstFuncCall(AstSymbol* symbol,
			    AstExpr* expr_list,
			    const FileRegion& loc)
{
  AstBlock* block = cur_block();
  AstFuncDecl* func_decl = block->find_funcdecl(symbol->str_val());
  if ( func_decl == NULL ) {
    return NULL;
  }
  return new AstFuncCall(func_decl, expr_list, loc);
}

// @brief 識別子式を作る．
// @param[in] symbol 値
// @param[in] loc ファイル位置
AstExpr*
YmslParser::new_AstVarExpr(AstSymbol* symbol)
{
  AstBlock* block = cur_block();
  AstVarDecl* var_decl = block->find_vardecl(symbol->str_val());
  if ( var_decl == NULL ) {
    return NULL;
  }
  return new AstVarExpr(var_decl, symbol->file_region());
}

// @brief 整数定数式を作る．
// @param[in] val 値
// @param[in] loc ファイル位置
AstExpr*
YmslParser::new_AstIntConst(int val,
			    const FileRegion& loc)
{
  return new AstIntConst(val, loc);
}

// @brief 浮動小数点定数式を作る．
// @param[in] val 値
// @param[in] loc ファイル位置
AstExpr*
YmslParser::new_AstFloatConst(double val,
			      const FileRegion& loc)
{
  return new AstFloatConst(val, loc);
}

// @brief 文字列定数式を作る．
// @param[in] val 値
// @param[in] loc ファイル位置
AstExpr*
YmslParser::new_AstStringConst(const char* val,
			       const FileRegion& loc)
{
  return new AstStringConst(val, loc);
}

// @brief 文字列型を作る．
// @param[in] loc ファイル位置
AstValueType*
YmslParser::new_AstStringType(const FileRegion& loc)
{
  return new AstStringType(loc);
}

// @brief 整数型を作る．
// @param[in] loc ファイル位置
AstValueType*
YmslParser::new_AstIntType(const FileRegion& loc)
{
  return new AstIntType(loc);
}

// @brief 浮動小数点型を作る．
// @param[in] loc ファイル位置
AstValueType*
YmslParser::new_AstFloatType(const FileRegion& loc)
{
  return new AstFloatType(loc);
}

// @brief ユーザー定義型を作る．
// @param[in] type_name 型名
// @param[in] loc ファイル位置
AstValueType*
YmslParser::new_AstUserType(AstSymbol* type_name)
{
  return new AstUserType(type_name);
}

END_NAMESPACE_YM_YMSL
