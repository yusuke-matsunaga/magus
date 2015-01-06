
/// @file AstMgr.cc
/// @brief AstMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstMgr.h"
#include "AstFuncDecl.h"
#include "AstVarDecl.h"
#include "AstSymbol.h"
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
#include "AstIteOp.h"
#include "AstVarExpr.h"
#include "AstIntConst.h"
#include "AstFloatConst.h"
#include "AstStringConst.h"
#include "AstPrimary.h"
#include "AstVoidType.h"
#include "AstBooleanType.h"
#include "AstIntType.h"
#include "AstFloatType.h"
#include "AstStringType.h"
#include "AstUserType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AstMgr::AstMgr()
{
}

// @brief デストラクタ
AstMgr::~AstMgr()
{
}

// @brief 変数宣言を作る．
// @param[in] name 変数名
// @param[in] type 型
// @param[in] init_expr 初期化式
// @param[in] global グローバル変数の時 true にするフラグ
// @param[in] loc ファイル位置
AstVarDecl*
AstMgr::new_VarDecl(AstSymbol* name,
		    AstValueType* type,
		    AstExpr* init_expr,
		    bool global,
		    const FileRegion& loc)
{
  if ( type->simple_type() ) {
    void* p = mAlloc.get_memory(sizeof(AstVarDecl));
    return new (p) AstVarDecl(name->str_val(), type->value_type(), init_expr, global, loc);
  }
  else {
  }
}

// @brief 関数宣言を作る．
// @param[in] name 変数名
// @param[in] type 型
// @param[in] param_list パラメータリスト
// @param[in] stmt_list 本体の文
// @param[in] loc ファイル位置
AstFuncDecl*
AstMgr::new_FuncDecl(AstSymbol* name,
		     AstValueType* type,
		     AstVarList* param_list,
		     AstStmtList* stmt_list,
		     const FileRegion& loc)
{
  if ( type->simple_type() ) {
    void* p = mAlloc.get_memory(sizeof(AstFuncDecl));
    return  new (p) AstFuncDecl(name->str_val(), type->value_type(), param_list, stmt_list, loc);
  }
  else {
  }
}

// @brief 代入文を作る．
// @param[in] token トークン
// @param[in] left 左辺
// @param[in] right 右辺
AstStatement*
AstMgr::new_Assignment(TokenType token,
		       AstPrimary* left,
		       AstExpr* right)
{
  void* p = mAlloc.get_memory(sizeof(AstAssignment));
  return new (p) AstAssignment(token, left, right);
}

// @brief if 文を作る．
// @param[in] if_list IfBlock のリスト
// @param[in] loc ファイル位置
AstStatement*
AstMgr::new_If(AstIfList* if_list,
	       const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstIf));
  return new (p) AstIf(if_list, loc);
}

// @brief if blockを作る
// @param[in] cond 条件式
// @param[in] stmt_list 本体の文
// @param[in] loc ファイル位置
AstIfBlock*
AstMgr::new_IfBlock(AstExpr* cond,
		    AstStmtList* stmt_list,
		    const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstIfBlock));
  return new (p) AstIfBlock(cond, stmt_list, loc);
}

// @brief for 文を作る．
// @param[in] init 初期化文
// @param[in] cond 条件式
// @param[in] next 増加文
// @param[in] stmt_list 本体の文
// @param[in] loc ファイル位置
AstStatement*
AstMgr::new_For(AstStatement* init,
		AstExpr* cond,
		AstStatement* next,
		AstStmtList* stmt_list,
		const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstFor));
  return new (p) AstFor(init, cond, next, stmt_list, loc);
}

// @brief while 文を作る．
// @param[in] cond 条件式
// @param[in] stmt_list 本体の文
// @param[in] loc ファイル位置
AstStatement*
AstMgr::new_While(AstExpr* cond,
		  AstStmtList* stmt_list,
		  const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstWhile));
  return new (p) AstWhile(cond, stmt_list, loc);
}

// @brief do-while 文を作る．
// @param[in] stmt_list 本体の文
// @param[in] cond 条件式
// @param[in] loc ファイル位置
AstStatement*
AstMgr::new_DoWhile(AstStmtList* stmt_list,
		    AstExpr* cond,
		    const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstDoWhile));
  return new (p) AstDoWhile(stmt_list, cond, loc);
}

// @brief switch 文を作る．
// @param[in] expr 条件式
// @param[in] case_list caseリスト
// @param[in] loc ファイル位置
AstStatement*
AstMgr::new_Switch(AstExpr* expr,
		   AstCaseList* case_list,
		   const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstSwitch));
  return new (p) AstSwitch(expr, case_list, loc);
}

// @brief case-item を作る．
// @param[in] label ラベル
// @param[in] stmt_list 本体の文
// @param[in] loc ファイル位置
AstCaseItem*
AstMgr::new_CaseItem(AstExpr* label,
		     AstStmtList* stmt_list,
		     const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstCaseItem));
  return new (p) AstCaseItem(label, stmt_list, loc);
}

// @brief goto 文を作る．
// @param[in] label ラベル
// @param[in] loc ファイル位置
AstStatement*
AstMgr::new_Goto(AstSymbol* label,
		 const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstGoto));
  return new (p) AstGoto(label, loc);
}

// @brief ラベルを作る．
// @param[in] label ラベル
// @param[in] loc ファイル位置
AstStatement*
AstMgr::new_Label(AstSymbol* label,
		  const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstLabel));
  return new (p) AstLabel(label, loc);
}

// @brief break 文を作る．
// @param[in] loc ファイル位置
AstStatement*
AstMgr::new_Break(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstBreak));
  return new (p) AstBreak(loc);
}

// @brief continue 文を作る．
// @param[in] loc ファイル位置
AstStatement*
AstMgr::new_Continue(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstContinue));
  return new (p) AstContinue(loc);
}

// @brief return 文を作る．
// @param[in] expr 値
// @param[in] loc ファイル位置
AstStatement*
AstMgr::new_Return(AstExpr* expr,
		   const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstReturn));
  return new (p) AstReturn(expr, loc);
}

// @brief ブロック文を作る．
// @param[in] stmt_list 本体の文
// @param[in] loc ファイル位置
AstStatement*
AstMgr::new_BlockStmt(AstStmtList* stmt_list,
		      const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstBlockStmt));
  return new (p) AstBlockStmt(stmt_list, loc);
}

// @brief 式文を作る．
// @param[in] expr 式
AstStatement*
AstMgr::new_ExprStmt(AstExpr* expr)
{
  void* p = mAlloc.get_memory(sizeof(AstExprStmt));
  return new (p) AstExprStmt(expr);
}

// @brief 単項演算式を作る．
// @param[in] op 演算子のトークン
// @param[in] left オペランド
// @param[in] loc ファイル位置
AstExpr*
AstMgr::new_UniOp(TokenType op,
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
AstMgr::new_BinOp(TokenType op,
		  AstExpr* left,
		  AstExpr* right)
{
  void* p = mAlloc.get_memory(sizeof(AstBinOp));
  return new (p) AstBinOp(op, left, right);
}

// @brief ITE演算式を作る．
// @param[in] opr1, opr2, opr3 オペランド
AstExpr*
AstMgr::new_IteOp(AstExpr* opr1,
		  AstExpr* opr2,
		  AstExpr* opr3)
{
  void* p = mAlloc.get_memory(sizeof(AstIteOp));
  return new (p) AstIteOp(opr1, opr2, opr3);
}

// @brief 配列参照を作る．
// @param[in] id 配列名
// @param[in] index インデックス
// @param[in] loc ファイル位置
AstExpr*
AstMgr::new_ArrayRef(AstSymbol* id,
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
AstMgr::new_FuncCall(AstSymbolList* symbol,
		     AstExprList* expr_list,
		     const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstFuncCall));
  return new (p) AstFuncCall(symbol, expr_list, loc);
}

// @brief 識別子式を作る．
// @param[in] symbol 値
// @param[in] loc ファイル位置
AstExpr*
AstMgr::new_VarExpr(AstSymbolList* symbol,
		    const FileRegion& loc)
{
#if 0
  AstVarDecl* var_decl = find_var(symbol->str_val());
  if ( var_decl == NULL ) {
    ostringstream buf;
    buf << symbol->str_val() << ": Undefined";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    symbol->file_region(),
		    kMsgError,
		    "PARS",
		    buf.str());
    return NULL;
  }
#endif
  void* p = mAlloc.get_memory(sizeof(AstVarExpr));
  return new (p) AstVarExpr(symbol, loc);
}

// @brief 整数定数式を作る．
// @param[in] val 値
// @param[in] loc ファイル位置
AstExpr*
AstMgr::new_IntConst(int val,
		     const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstIntConst));
  return new (p) AstIntConst(val, loc);
}

// @brief 浮動小数点定数式を作る．
// @param[in] val 値
// @param[in] loc ファイル位置
AstExpr*
AstMgr::new_FloatConst(double val,
		       const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstFloatConst));
  return new (p) AstFloatConst(val, loc);
}

// @brief 文字列定数式を作る．
// @param[in] val 値
// @param[in] loc ファイル位置
AstExpr*
AstMgr::new_StringConst(const char* val,
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

// @brief 左辺のプライマリを作る．
// @param[in] symbol 変数名
// @param[in] loc ファイル位置
AstPrimary*
AstMgr::new_Primary(AstSymbolList* symbol,
		    const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstPrimary));
  return new (p) AstPrimary(symbol, loc);
}

// @brief 文字列型を作る．
// @param[in] loc ファイル位置
AstValueType*
AstMgr::new_StringType(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstStringType));
  return new (p) AstStringType(loc);
}

// @brief void型を作る．
// @param[in] loc ファイル位置
AstValueType*
AstMgr::new_VoidType(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstVoidType));
  return new (p) AstVoidType(loc);
}

// @brief boolean型を作る．
// @param[in] loc ファイル位置
AstValueType*
AstMgr::new_BooleanType(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstBooleanType));
  return new (p) AstBooleanType(loc);
}

// @brief 整数型を作る．
// @param[in] loc ファイル位置
AstValueType*
AstMgr::new_IntType(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstIntType));
  return new (p) AstIntType(loc);
}

// @brief 浮動小数点型を作る．
// @param[in] loc ファイル位置
AstValueType*
AstMgr::new_FloatType(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstFloatType));
  return new (p) AstFloatType(loc);
}

// @brief ユーザー定義型を作る．
// @param[in] type_name 型名
AstValueType*
AstMgr::new_UserType(AstSymbol* type_name)
{
  void* p = mAlloc.get_memory(sizeof(AstUserType));
  return new (p) AstUserType(type_name);
}

// @brief シンボルを作る．
// @param[in] str シンボル名
// @param[in] loc ファイル位置
AstSymbol*
AstMgr::new_Symbol(ShString str,
		   const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstSymbol));
  return new (p) AstSymbol(str, loc);
}

END_NAMESPACE_YM_YMSL
