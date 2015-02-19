
/// @file AstMgr.cc
/// @brief AstMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstMgr.h"

#include "YmslScanner.h"
#include "AstList.h"
#include "AstSymbol.h"

#include "expr/AstArrayRef.h"
#include "expr/AstBinOp.h"
#include "expr/AstFalse.h"
#include "expr/AstFloatConst.h"
#include "expr/AstFuncCall.h"
#include "expr/AstIntConst.h"
#include "expr/AstIteOp.h"
#include "expr/AstMemberRef.h"
#include "expr/AstStringConst.h"
#include "expr/AstSymbolExpr.h"
#include "expr/AstTrue.h"
#include "expr/AstUniOp.h"

#include "stmt/AstAssignment.h"
#include "stmt/AstBlockStmt.h"
#include "stmt/AstBreak.h"
#include "stmt/AstCaseItem.h"
#include "stmt/AstConstDecl.h"
#include "stmt/AstContinue.h"
#include "stmt/AstDecr.h"
#include "stmt/AstDoWhile.h"
#include "stmt/AstEnumConst.h"
#include "stmt/AstEnumDecl.h"
#include "stmt/AstExprStmt.h"
#include "stmt/AstFor.h"
#include "stmt/AstFuncDecl.h"
#include "stmt/AstGoto.h"
#include "stmt/AstIf.h"
#include "stmt/AstImport.h"
#include "stmt/AstIncr.h"
#include "stmt/AstInplaceOp.h"
#include "stmt/AstLabel.h"
#include "stmt/AstNullStmt.h"
#include "stmt/AstParam.h"
#include "stmt/AstReturn.h"
#include "stmt/AstSwitch.h"
#include "stmt/AstToplevel.h"
#include "stmt/AstVarDecl.h"
#include "stmt/AstWhile.h"

#include "type/AstArrayType.h"
#include "type/AstMapType.h"
#include "type/AstNamedType.h"
#include "type/AstPrimType.h"
#include "type/AstSetType.h"

#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] debug デバッグフラグ
AstMgr::AstMgr(bool debug)
{
  mScanner = NULL;
  mDebug = debug;
}

// @brief デストラクタ
AstMgr::~AstMgr()
{
}

// @brief ソースファイルを読み込む．
// @param[in] ido 入力データ
// @return 読み込みに成功したら true を返す．
bool
AstMgr::read_source(IDO& ido)
{
  int yyparser(AstMgr&);

  mScanner = new YmslScanner(ido);
  mToplevel = NULL;

  int stat = yyparse(*this);

  delete mScanner;
  mScanner = NULL;

  return (stat == 0);
}

// @brief トップレベルのASTを返す．
AstStatement*
AstMgr::toplevel() const
{
  return mToplevel;
}

// @brief 根のノードをセットする．
// @param[in] stmt_list ステートメントリスト
// @param[in] loc ファイル位置
void
AstMgr::set_root(AstStmtList* stmt_list,
		 const FileRegion& loc)
{
  ASSERT_COND ( mToplevel == NULL );

  ymuint num = stmt_list->size();
  void* q = mAlloc.get_memory(sizeof(AstStatement*) * num);
  AstStatement** stmt_array = new (q) AstStatement*[num];
  ymuint pos = 0;
  for (AstStmtList::Iterator p = stmt_list->begin();
       !p.is_end(); p.next()) {
    stmt_array[pos] = *p;
    ++ pos;
  }

  void* p = mAlloc.get_memory(sizeof(AstToplevel));
  mToplevel = new (p) AstToplevel(num, stmt_array, loc);
}

// @brief yylex とのインターフェイス
// @param[out] lval 値を格納する変数
// @param[out] lloc 位置情報を格納する変数
// @return 読み込んだトークンの id を返す．
TokenType
AstMgr::scan(YYSTYPE& lval,
	     FileRegion& lloc)
{
  TokenType id = mScanner->read_token(lloc);

  switch ( id ) {
  case SYMBOL:
    lval.symbol_type = new_Symbol(mScanner->cur_string(), lloc);
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
  if ( mDebug ) {
    mScanner->print_token(id, cout);
    cout << endl;
  }
  return id;
}

// @brief import 文を作る．
// @param[in] module モジュール名
// @param[in] alias エイリアス
// @param[in] loc ファイル位置
//
// alias は NULL の場合もある．
AstStatement*
AstMgr::new_Import(AstExpr* module,
		   AstSymbol* alias,
		   const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstImport));
  return new (p) AstImport(module, alias, loc);
}

// @brief enum 定義を作る．
// @param[in] name 型名
// @param[in] const_list 定数リスト
// @param[in] loc ファイル位置
AstStatement*
AstMgr::new_EnumDecl(AstSymbol* name,
		     AstEnumConstList* const_list,
		     const FileRegion& loc)
{
  ymuint num = const_list->size();
  void* q = mAlloc.get_memory(sizeof(AstEnumConst*) * num);
  AstEnumConst** const_array = new (q) AstEnumConst*[num];
  ymuint pos = 0;
  for (AstEnumConstList::Iterator p = const_list->begin();
       !p.is_end(); p.next()) {
    const_array[pos] = *p;
    ++ pos;
  }

  void* p = mAlloc.get_memory(sizeof(AstEnumDecl));
  return new (p) AstEnumDecl(name, num, const_array, loc);
}

// @brief enum 定数を作る．
// @param[in] name 定数名
// @param[in] expr 値を表す式
// @param[in] loc ファイル位置
AstEnumConst*
AstMgr::new_EnumConst(AstSymbol* name,
		      AstExpr* expr,
		      const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstEnumConst));
  return new (p) AstEnumConst(name, expr, loc);
}

// @brief 変数宣言を作る．
// @param[in] name 変数名
// @param[in] type 型
// @param[in] init_expr 初期化式
// @param[in] loc ファイル位置
AstStatement*
AstMgr::new_VarDecl(AstSymbol* name,
		    AstType* type,
		    AstExpr* init_expr,
		    const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstVarDecl));
  return new (p) AstVarDecl(name, type, init_expr, loc);
}

// @brief 定数宣言を作る．
// @param[in] name 定数名
// @param[in] type 型
// @param[in] init_expr 初期化式
// @param[in] loc ファイル位置
AstStatement*
AstMgr::new_ConstDecl(AstSymbol* name,
		      AstType* type,
		      AstExpr* init_expr,
		      const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstConstDecl));
  return new (p) AstConstDecl(name, type, init_expr, loc);
}

// @brief パラメータ宣言を作る．
// @param[in] name 変数名
// @param[in] type 型
// @param[in] init_expr 初期化式
// @param[in] loc ファイル位置
AstParam*
AstMgr::new_Param(AstSymbol* name,
		  AstType* type,
		  AstExpr* init_expr,
		  const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstParam));
  return new (p) AstParam(name, type, init_expr, loc);
}

// @brief 関数宣言を作る．
// @param[in] name 変数名
// @param[in] type 型
// @param[in] param_list パラメータリスト
// @param[in] stmt 本体の文
// @param[in] loc ファイル位置
AstStatement*
AstMgr::new_FuncDecl(AstSymbol* name,
		     AstType* type,
		     AstParamList* param_list,
		     AstStatement* stmt,
		     const FileRegion& loc)
{
  ymuint param_num = param_list->size();
  void* q = mAlloc.get_memory(sizeof(AstParam*) * param_num);
  AstParam** param_array = new (q) AstParam*[param_num];
  ymuint pos = 0;
  for (AstParamList::Iterator p = param_list->begin();
       !p.is_end(); p.next()) {
    param_array[pos] = *p;
    ++ pos;
  }

  void* p = mAlloc.get_memory(sizeof(AstFuncDecl));
  return  new (p) AstFuncDecl(name, type, param_num, param_array, stmt, loc);
}

// @brief 代入文を作る．
// @param[in] left 左辺
// @param[in] right 右辺
// @param[in] loc ファイル位置
AstStatement*
AstMgr::new_Assignment(AstExpr* left,
		       AstExpr* right,
		       const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstAssignment));
  return new (p) AstAssignment(left, right, loc);
}

// @brief 演算付き代入文を作る．
// @param[in] opcode オペコード
// @param[in] left 左辺
// @param[in] right 右辺
// @param[in] loc ファイル位置
AstStatement*
AstMgr::new_InplaceOp(OpCode opcode,
		      AstExpr* left,
		      AstExpr* right,
		      const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstInplaceOp));
  return new (p) AstInplaceOp(opcode, left, right, loc);
}

// @brief 増加文を作る．
// @param[in] expr 対象の式
// @param[in] loc ファイル位置
AstStatement*
AstMgr::new_Incr(AstExpr* expr,
		 const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstIncr));
  return new (p) AstIncr(expr, loc);
}

// @brief 減少文を作る．
// @param[in] expr 対象の式
// @param[in] loc ファイル位置
AstStatement*
AstMgr::new_Decr(AstExpr* expr,
		 const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstDecr));
  return new (p) AstDecr(expr, loc);
}

// @brief if 文を作る．
// @param[in] expr 条件式
// @param[in] then_stmt 条件が成り立った時実行される文のリスト
// @param[in] else_stmt 条件が成り立たなかった時実行される文
// @param[in] loc ファイル位置
AstStatement*
AstMgr::new_If(AstExpr* expr,
	       AstStatement* then_stmt,
	       AstStatement* else_stmt,
	       const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstIf));
  return new (p) AstIf(expr, then_stmt, else_stmt, loc);
}

// @brief for 文を作る．
// @param[in] init 初期化文
// @param[in] cond 条件式
// @param[in] next 増加文
// @param[in] stmt 本体の文
// @param[in] loc ファイル位置
AstStatement*
AstMgr::new_For(AstStatement* init,
		AstExpr* cond,
		AstStatement* next,
		AstStatement* stmt,
		const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstFor));
  return new (p) AstFor(init, cond, next, stmt, loc);
}

// @brief while 文を作る．
// @param[in] cond 条件式
// @param[in] stmt 本体の文
// @param[in] loc ファイル位置
AstStatement*
AstMgr::new_While(AstExpr* cond,
		  AstStatement* stmt,
		  const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstWhile));
  return new (p) AstWhile(cond, stmt, loc);
}

// @brief do-while 文を作る．
// @param[in] stmt 本体の文
// @param[in] cond 条件式
// @param[in] loc ファイル位置
AstStatement*
AstMgr::new_DoWhile(AstStatement* stmt,
		    AstExpr* cond,
		    const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstDoWhile));
  return new (p) AstDoWhile(stmt, cond, loc);
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
  ymuint num = case_list->size();
  void* q = mAlloc.get_memory(sizeof(AstCaseItem*) * num);
  AstCaseItem** case_array = new (q) AstCaseItem*[num];

  ymuint pos = 0;
  for (AstCaseList::Iterator p = case_list->begin();
       !p.is_end(); p.next()) {
    case_array[pos] = *p;
    ++ pos;
  }

  void* p = mAlloc.get_memory(sizeof(AstSwitch));
  return new (p) AstSwitch(expr, num, case_array, loc);
}

// @brief case-item を作る．
// @param[in] label ラベル
// @param[in] stmt 本体の文
// @param[in] loc ファイル位置
AstCaseItem*
AstMgr::new_CaseItem(AstExpr* label,
		     AstStatement* stmt,
		     const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstCaseItem));
  return new (p) AstCaseItem(label, stmt, loc);
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
  ymuint num = stmt_list->size();
  void* q = mAlloc.get_memory(sizeof(AstStatement*) * num);
  AstStatement** stmt_array = new (q) AstStatement*[num];
  ymuint pos = 0;
  for (AstStmtList::Iterator p = stmt_list->begin();
       !p.is_end(); p.next()) {
    stmt_array[pos] = *p;
    ++ pos;
  }

  void* p = mAlloc.get_memory(sizeof(AstBlockStmt));
  return new (p) AstBlockStmt(num, stmt_array, loc);
}

// @brief 式文を作る．
// @param[in] expr 式
// @param[in] loc ファイル位置
AstStatement*
AstMgr::new_ExprStmt(AstExpr* expr,
		     const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstExprStmt));
  return new (p) AstExprStmt(expr, loc);
}

// @brief 空文を作る．
// @param[in] loc ファイル位置
AstStatement*
AstMgr::new_NullStmt(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstNullStmt));
  return new (p) AstNullStmt(loc);
}

// @brief 単項演算式を作る．
// @param[in] opcode オペコード
// @param[in] left オペランド
// @param[in] loc ファイル位置
AstExpr*
AstMgr::new_UniOp(OpCode opcode,
		  AstExpr* left,
		  const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstUniOp));
  return new (p) AstUniOp(opcode, left, loc);
}

// @brief 二項演算式を作る．
// @param[in] opcode オペコード
// @param[in] left, right オペランド
AstExpr*
AstMgr::new_BinOp(OpCode opcode,
		  AstExpr* left,
		  AstExpr* right)
{
  void* p = mAlloc.get_memory(sizeof(AstBinOp));
  return new (p) AstBinOp(opcode, left, right);
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

// @brief シンボル式を作る．
// @param[in] symbol シンボル
AstExpr*
AstMgr::new_SymbolExpr(AstSymbol* symbol)
{
  void* p = mAlloc.get_memory(sizeof(AstSymbolExpr));
  return new (p) AstSymbolExpr(symbol);
}

// @brief メンバ参照を作る．
// @param[in] body 本体の式
// @param[in] member メンバ名
// @param[in] loc ファイル位置
AstExpr*
AstMgr::new_MemberRef(AstExpr* body,
		      AstSymbol* member,
		      const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstMemberRef));
  return new (p) AstMemberRef(body, member, loc);
}

// @brief 配列参照を作る．
// @param[in] id 配列名
// @param[in] index インデックス
// @param[in] loc ファイル位置
AstExpr*
AstMgr::new_ArrayRef(AstExpr* id,
		     AstExpr* index,
		     const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstArrayRef));
  return new (p) AstArrayRef(id, index, loc);
}

// @brief 関数呼び出しを作る．
// @param[in] id 関数名
// @param[in] expr_list 引数のリスト
// @param[in] loc ファイル位置
AstExpr*
AstMgr::new_FuncCall(AstExpr* id,
		     AstExprList* expr_list,
		     const FileRegion& loc)
{
  ymuint num = expr_list->size();
  void* q = mAlloc.get_memory(sizeof(AstExpr*) * num);
  AstExpr** expr_array = new (q) AstExpr*[num];

  ymuint pos = 0;
  for (AstExprList::Iterator p = expr_list->begin();
       !p.is_end(); p.next()) {
    expr_array[pos] = *p;
    ++ pos;
  }

  void* p = mAlloc.get_memory(sizeof(AstFuncCall));
  return new (p) AstFuncCall(id, num, expr_array, loc);
}

// @brief true 定数式を作る．
// @param[in] loc ファイル位置
AstExpr*
AstMgr::new_TrueConst(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstTrue));
  return new (p) AstTrue(loc);
}

// @brief false 定数式を作る．
// @param[in] loc ファイル位置
AstExpr*
AstMgr::new_FalseConst(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstFalse));
  return new (p) AstFalse(loc);
}

// @brief 整数定数式を作る．
// @param[in] val 値
// @param[in] loc ファイル位置
AstExpr*
AstMgr::new_IntConst(Ymsl_INT val,
		     const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstIntConst));
  return new (p) AstIntConst(val, loc);
}

// @brief 浮動小数点定数式を作る．
// @param[in] val 値
// @param[in] loc ファイル位置
AstExpr*
AstMgr::new_FloatConst(Ymsl_FLOAT val,
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

// @brief プリミティブ型を作る．
// @param[in] type 型
// @param[in] loc ファイル位置
AstType*
AstMgr::new_PrimType(TypeId type,
		     const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstPrimType));
  return new (p) AstPrimType(type, loc);
}

// @brief 名前付きの型を作る．
// @param[in] type_name 型名
// @param[in] loc ファイル位置
AstType*
AstMgr::new_NamedType(AstExpr* type_name,
		      const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstNamedType));
  return new (p) AstNamedType(type_name, loc);
}

// @brief array 型を作る．
// @param[in] elem_type 要素の型
// @param[in] loc ファイル位置
AstType*
AstMgr::new_ArrayType(AstType* elem_type,
		      const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstArrayType));
  return new (p) AstArrayType(elem_type, loc);
}

// @brief set 型を作る．
// @param[in] elem_type 要素の型
// @param[in] loc ファイル位置
AstType*
AstMgr::new_SetType(AstType* elem_type,
		    const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstSetType));
  return new (p) AstSetType(elem_type, loc);
}

// @brief map 型を作る．
// @param[in] key_type キーの型
// @param[in] elem_type 要素の型
// @param[in] loc ファイル位置
AstType*
AstMgr::new_MapType(AstType* key_type,
		    AstType* elem_type,
		    const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstMapType));
  return new (p) AstMapType(key_type, elem_type, loc);
}

// @brief シンボルを作る．
// @param[in] str シンボル名
// @param[in] loc ファイル位置
AstSymbol*
AstMgr::new_Symbol(const char* str,
		   const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstSymbol));
  return new (p) AstSymbol(ShString(str), loc);
}

END_NAMESPACE_YM_YMSL
