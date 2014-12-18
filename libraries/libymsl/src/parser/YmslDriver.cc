
/// @file YmslDriver.cc
/// @brief YmslDriver の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslDriver.h"
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

#include "../builtin/YmslPrint.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslDriver
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
YmslDriver::YmslDriver()
{
  mScanner = NULL;
  mToplevelBlock = NULL;

  YmslFunc* func1 = new YmslPrint(ShString("print"), vector<YmslVar*>(0));
  mGlobalDict.add_function(func1);
}

// @brief デストラクタ
YmslDriver::~YmslDriver()
{
  // 念の為
  delete mScanner;
}

// @brief 読み込む．
// @param[in] ido 入力データ
// @return 成功したら true を返す．
bool
YmslDriver::read(IDO& ido)
{
  int yyparser(YmslDriver&);

  mScanner = new YmslScanner(ido);

  mToplevelBlock = new AstBlock(&mGlobalDict);
  mBlockStack.push_back(mToplevelBlock);

  int stat = yyparse(*this);

  delete mScanner;
  mScanner = NULL;

  return (stat == 0);
}

// @brief トップレベルブロックを返す．
AstBlock*
YmslDriver::toplevel_block() const
{
  return mToplevelBlock;
}

// @brief yylex とのインターフェイス
// @param[out] lvalp 値を格納する変数
// @param[out] llocp 位置情報を格納する変数
// @return 読み込んだトークンの id を返す．
int
YmslDriver::yylex(YYSTYPE& lval,
		  FileRegion& lloc)
{
  int id = mScanner->read_token(lloc);

  switch ( id ) {
  case SYMBOL:
    lval.symbol_type = new AstSymbol(ShString(mScanner->cur_string()), lloc);
    break;

  case STRING_VAL:
    lval.expr_type = new_StringConst(mScanner->cur_string(), lloc);
    break;

  case INT_VAL:
    lval.expr_type = new_IntConst(mScanner->cur_int(), lloc);
    break;

  case FLOAT_VAL:
    lval.expr_type = new_FloatConst(mScanner->cur_float(), lloc);
    break;

  default:
    break;
  }

  return id;
}

// @brief 現在のブロックを返す．
AstBlock*
YmslDriver::cur_block()
{
  ASSERT_COND( !mBlockStack.empty() );
  return mBlockStack.back();
}

// @brief 新しいブロックを作りスタックに積む．
// @return 新しいブロックを返す．
AstBlock*
YmslDriver::push_new_block()
{
  AstBlock* parent = cur_block();
  AstBlock* block = new AstBlock(parent);
  mBlockStack.push_back(block);
  return block;
}

// @brief ブロックをスタックから取り去る．
void
YmslDriver::pop_block()
{
  mBlockStack.pop_back();
}

// @brief 関数を追加する．
void
YmslDriver::add_function(AstFuncDecl* funcdecl)
{
  //mGlobalDict.add_func(func);
}

// @brief グローバル変数を追加する．
// @param[in] vardecl 変数宣言
void
YmslDriver::add_global_var(AstVarDecl* vardecl)
{
  mGlobalDict.add_vardecl(vardecl);
}

// @brief 現在のブロックに変数を追加する．
void
YmslDriver::add_local_var(AstVarDecl* vardecl)
{
  mBlockStack.back()->add_vardecl(vardecl);
}

// @brief 現在のブロックに statement を追加する．
void
YmslDriver::add_statement(AstStatement* stmt)
{
  mBlockStack.back()->add_statement(stmt);
}

// @brief 変数宣言を作る．
// @param[in] name 変数名
// @param[in] type 型
// @param[in] init_expr 初期化式
// @param[in] loc ファイル位置
AstVarDecl*
YmslDriver::new_VarDecl(AstSymbol* name,
			AstValueType* type,
			AstExpr* init_expr,
			const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstVarDecl));
  return new (p) AstVarDecl(name->str_val(), type, init_expr, loc);
}

// @brief 関数宣言を作る．
// @param[in] name 変数名
// @param[in] type 型
// @param[in] param_list パラメータリスト
// @param[in] block 本体のブロック
// @param[in] loc ファイル位置
AstFuncDecl*
YmslDriver::new_FuncDecl(AstSymbol* name,
			 AstValueType* type,
			 AstVarDecl* param_list,
			 const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstFuncDecl));
  AstBlock* block = cur_block();
  return  new (p) AstFuncDecl(name->str_val(), type, param_list, block, loc);
}

// @brief 代入文を作る．
// @param[in] left 左辺
// @param[in] right 右辺
AstStatement*
YmslDriver::new_Assignment(AstExpr* left,
			   AstExpr* right)
{
  void* p = mAlloc.get_memory(sizeof(AstAssignment));
  return new (p) AstAssignment(left, right);
}

// @brief if 文を作る．
// @param[in] cond 条件式
// @param[in] then_block then ブロック
// @param[in] elif_list elif ブロックリスト
// @param[in] else_block else ブロック
// @param[in] loc ファイル位置
AstStatement*
YmslDriver::new_If(AstIfBlock* top,
		   AstIfBlock* elif_list,
		   AstIfBlock* else_block,
		   const FileRegion& loc)
{
  if ( else_block != NULL ) {
    else_block->set_prev(elif_list);
    elif_list = else_block;
  }
  void* p = mAlloc.get_memory(sizeof(AstIf));
  return new (p) AstIf(top, elif_list, loc);
}

// @brief if blockを作る．
// @param[in] cond 条件式
// @param[in] block 本体
// @param[in] loc ファイル位置
AstIfBlock*
YmslDriver::new_IfBlock(AstExpr* cond,
			AstBlock* block,
			const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstIfBlock));
  return new (p) AstIfBlock(cond, block, loc);
}

// @brief for 文を作る．
// @param[in] init 初期化文
// @param[in] cond 条件式
// @param[in] next 増加文
// @param[in] block 本体
// @param[in] loc ファイル位置
AstStatement*
YmslDriver::new_For(AstStatement* init,
		    AstExpr* cond,
		    AstStatement* next,
		    AstBlock* block,
		    const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstFor));
  return new (p) AstFor(init, cond, next, block, loc);
}

// @brief while 文を作る．
// @param[in] cond 条件式
// @param[in] block 本体
// @param[in] loc ファイル位置
AstStatement*
YmslDriver::new_While(AstExpr* cond,
		      AstBlock* block,
		      const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstWhile));
  return new (p) AstWhile(cond, block, loc);
}

// @brief do-while 文を作る．
// @param[in] block 本体
// @param[in] cond 条件式
// @param[in] loc ファイル位置
AstStatement*
YmslDriver::new_DoWhile(AstBlock* block,
			AstExpr* cond,
			const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstDoWhile));
  return new (p) AstDoWhile(block, cond, loc);
}

// @brief switch 文を作る．
// @param[in] expr 条件式
// @param[in] case_list caseリスト
// @param[in] loc ファイル位置
AstStatement*
YmslDriver::new_Switch(AstExpr* expr,
		       AstCaseItem* case_list,
		       const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstSwitch));
  return new (p) AstSwitch(expr, case_list, loc);
}

// @brief case-item を作る．
// @param[in] label ラベル
// @param[in] block 本体
// @param[in] loc ファイル位置
AstCaseItem*
YmslDriver::new_CaseItem(AstExpr* label,
			 AstBlock* block,
			 const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstCaseItem));
  return new (p) AstCaseItem(label, block, loc);
}

// @brief goto 文を作る．
// @param[in] label ラベル
// @param[in] loc ファイル位置
AstStatement*
YmslDriver::new_Goto(AstSymbol* label,
		     const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstGoto));
  return new (p) AstGoto(label, loc);
}

// @brief ラベルを作る．
// @param[in] label ラベル
// @param[in] loc ファイル位置
AstStatement*
YmslDriver::new_Label(AstSymbol* label,
		      const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstLabel));
  return new (p) AstLabel(label, loc);
}

// @brief break 文を作る．
// @param[in] loc ファイル位置
AstStatement*
YmslDriver::new_Break(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstBreak));
  return new (p) AstBreak(loc);
}

// @brief continue 文を作る．
// @param[in] loc ファイル位置
AstStatement*
YmslDriver::new_Continue(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstContinue));
  return new (p) AstContinue(loc);
}

// @brief return 文を作る．
// @param[in] expr 値
// @param[in] loc ファイル位置
AstStatement*
YmslDriver::new_Return(AstExpr* expr,
		       const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstReturn));
  return new (p) AstReturn(expr, loc);
}

// @brief ブロック文を作る．
// @param[in] block 本体
// @param[in] loc ファイル位置
AstStatement*
YmslDriver::new_BlockStmt(AstBlock* block,
			  const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstBlockStmt));
  return new (p) AstBlockStmt(block, loc);
}

// @brief 式文を作る．
// @param[in] expr 式
AstStatement*
YmslDriver::new_ExprStmt(AstExpr* expr)
{
  void* p = mAlloc.get_memory(sizeof(AstExprStmt));
  return new (p) AstExprStmt(expr);
}

// @brief 単項演算式を作る．
// @param[in] op 演算子のトークン
// @param[in] left オペランド
// @param[in] loc ファイル位置
AstExpr*
YmslDriver::new_UniOp(TokenType op,
		      AstExpr* left,
		      const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstUniOp));
  return new (p) AstUniOp(op, left, loc);
}

// @brief 二項演算式を作る．
// @param[in] op 演算子のトークン
// @param[in] left, right オペランド
AstExpr*
YmslDriver::new_BinOp(TokenType op,
		      AstExpr* left,
		      AstExpr* right)
{
  void* p = mAlloc.get_memory(sizeof(AstBinOp));
  return new (p) AstBinOp(op, left, right);
}

// @brief 配列参照を作る．
// @param[in] id 配列名
// @param[in] index インデックス
// @param[in] loc ファイル位置
AstExpr*
YmslDriver::new_ArrayRef(AstSymbol* id,
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
YmslDriver::new_FuncCall(AstSymbol* symbol,
			 AstExpr* expr_list,
			 const FileRegion& loc)
{
  YmslFunc* func = mGlobalDict.find_function(symbol->str_val());
  if ( func == NULL ) {
    return NULL;
  }
  void* p = mAlloc.get_memory(sizeof(AstFuncCall));
  return new (p) AstFuncCall(func, expr_list, loc);
}

// @brief 識別子式を作る．
// @param[in] symbol 値
// @param[in] loc ファイル位置
AstExpr*
YmslDriver::new_VarExpr(AstSymbol* symbol)
{
  AstBlock* block = cur_block();
  AstVarDecl* var_decl = block->find_vardecl(symbol->str_val());
  if ( var_decl == NULL ) {
    return NULL;
  }
  void* p = mAlloc.get_memory(sizeof(AstVarExpr));
  return new (p) AstVarExpr(var_decl, symbol->file_region());
}

// @brief 整数定数式を作る．
// @param[in] val 値
// @param[in] loc ファイル位置
AstExpr*
YmslDriver::new_IntConst(int val,
			 const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstIntConst));
  return new (p) AstIntConst(val, loc);
}

// @brief 浮動小数点定数式を作る．
// @param[in] val 値
// @param[in] loc ファイル位置
AstExpr*
YmslDriver::new_FloatConst(double val,
			   const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstFloatConst));
  return new (p) AstFloatConst(val, loc);
}

// @brief 文字列定数式を作る．
// @param[in] val 値
// @param[in] loc ファイル位置
AstExpr*
YmslDriver::new_StringConst(const char* val,
			    const FileRegion& loc)
{
  ymuint n = 0;
  if ( val != NULL ) {
    n = strlen(val);
  }
  void* q = mAlloc.get_memory(n + 1);
  char* dup_str = new (q) char[n + 1];
  for (ymuint i = 0; i < n; ++ i) {
    dup_str[i] = val[i];
  }
  dup_str[n] = '\0';

  void* p = mAlloc.get_memory(sizeof(AstStringConst));
  return new (p) AstStringConst(dup_str, loc);
}

// @brief 文字列型を作る．
// @param[in] loc ファイル位置
AstValueType*
YmslDriver::new_StringType(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstStringType));
  return new (p) AstStringType(loc);
}

// @brief 整数型を作る．
// @param[in] loc ファイル位置
AstValueType*
YmslDriver::new_IntType(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstIntType));
  return new (p) AstIntType(loc);
}

// @brief 浮動小数点型を作る．
// @param[in] loc ファイル位置
AstValueType*
YmslDriver::new_FloatType(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstFloatType));
  return new (p) AstFloatType(loc);
}

// @brief ユーザー定義型を作る．
// @param[in] type_name 型名
// @param[in] loc ファイル位置
AstValueType*
YmslDriver::new_UserType(AstSymbol* type_name)
{
  void* p = mAlloc.get_memory(sizeof(AstUserType));
  return new (p) AstUserType(type_name);
}

END_NAMESPACE_YM_YMSL