
/// @file YmslDriver.cc
/// @brief YmslDriver の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslDriver.h"
#include "YmslScanner.h"
#include "YmslLabel.h"
#include "YmslModule.h"

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

#include "YmUtils/MsgMgr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslDriver
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
YmslDriver::YmslDriver()
{
  mScanner = NULL;
  mDebug = false;
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

  mCurModule = new YmslModule(ShString("__main__"));

  int stat = yyparse(*this);

  delete mScanner;
  mScanner = NULL;

  return (stat == 0);
}

// @brief トップレベルブロックを返す．
AstBlock*
YmslDriver::toplevel_block() const
{
  return mCurModule->toplevel_block();
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
  if ( mDebug ) {
    RsrvWordDic dic;
    switch ( id ) {
    case SYMBOL:     cout << "SYMBOL[" << mScanner->cur_string() << "]"; break;
    case INT_VAL:    cout << "INT[" << mScanner->cur_int() << "]"; break;
    case FLOAT_VAL:  cout << "FLOAT[" << mScanner->cur_float() << "]"; break;
    case STRING_VAL: cout << "STRING[" << mScanner->cur_string() << "]"; break;
    case EOF:        cout << "EOF"; break;
    default:         cout << dic.str(id); break;
    }
    cout << endl;
  }
  return id;
}

// @brief 現在のブロックを返す．
AstBlock*
YmslDriver::cur_block()
{
  return mCurModule->cur_block();
}

// @brief 新しいブロックを作りスタックに積む．
// @return 新しいブロックを返す．
AstBlock*
YmslDriver::push_new_block()
{
  return mCurModule->push_new_block();
}

// @brief ブロックをスタックから取り去る．
void
YmslDriver::pop_block()
{
  mCurModule->pop_block();
}

// @brief 関数を追加する．
void
YmslDriver::add_function(AstFuncDecl* funcdecl)
{
  mCurModule->add_function(funcdecl);
}

// @brief グローバル変数を追加する．
// @param[in] vardecl 変数宣言
void
YmslDriver::add_global_var(AstVarDecl* vardecl)
{
  mCurModule->add_global_var(vardecl);
}

// @brief 現在のブロックに変数を追加する．
void
YmslDriver::add_local_var(AstVarDecl* vardecl)
{
  mCurModule->add_local_var(vardecl);
}

// @brief 現在のブロックに statement を追加する．
void
YmslDriver::add_statement(AstStatement* stmt)
{
  mCurModule->add_statement(stmt);
}

// @brief 変数宣言を作る．
// @param[in] name 変数名
// @param[in] type 型
// @param[in] init_expr 初期化式
// @param[in] global グローバル変数の時 true にするフラグ
// @param[in] loc ファイル位置
AstVarDecl*
YmslDriver::new_VarDecl(AstSymbol* name,
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
// @param[in] block 本体のブロック
// @param[in] loc ファイル位置
AstFuncDecl*
YmslDriver::new_FuncDecl(AstSymbol* name,
			 AstValueType* type,
			 AstVarDecl* param_list,
			 const FileRegion& loc)
{
  if ( type->simple_type() ) {
    void* p = mAlloc.get_memory(sizeof(AstFuncDecl));
    AstBlock* block = cur_block();
    return  new (p) AstFuncDecl(name->str_val(), type->value_type(), param_list, block, loc);
  }
  else {
  }
}

// @brief 代入文を作る．
// @param[in] token トークン
// @param[in] left 左辺
// @param[in] right 右辺
AstStatement*
YmslDriver::new_Assignment(TokenType token,
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
YmslDriver::new_If(AstIfBlock* if_list,
		   const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstIf));
  return new (p) AstIf(if_list, loc);
}

// @brief if blockを作る
// @param[in] prev 前の要素．
// @param[in] cond 条件式
// @param[in] block 本体
// @param[in] loc ファイル位置
AstIfBlock*
YmslDriver::new_IfBlock(AstIfBlock* prev,
			AstExpr* cond,
			AstBlock* block,
			const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstIfBlock));
  return new (p) AstIfBlock(prev, cond, block, loc);
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
// @param[in] prev 直前の要素
// @param[in] label ラベル
// @param[in] block 本体
// @param[in] loc ファイル位置
AstCaseItem*
YmslDriver::new_CaseItem(AstCaseItem* prev,
			 AstExpr* label,
			 AstBlock* block,
			 const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstCaseItem));
  return new (p) AstCaseItem(prev, label, block, loc);
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

// @brief ITE演算式を作る．
// @param[in] opr1, opr2, opr3 オペランド
AstExpr*
YmslDriver::new_IteOp(AstExpr* opr1,
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
  void* p = mAlloc.get_memory(sizeof(AstFuncCall));
  return new (p) AstFuncCall(symbol, expr_list, loc);
}

// @brief 識別子式を作る．
// @param[in] symbol 値
// @param[in] loc ファイル位置
AstExpr*
YmslDriver::new_VarExpr(AstSymbol* symbol)
{
  AstVarDecl* var_decl = mCurModule->find_var(symbol->str_val());
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

// @brief 左辺のプライマリを作る．
// @param[in] symbol 変数名
AstPrimary*
YmslDriver::new_Primary(AstSymbol* symbol)
{
  AstVarDecl* var_decl = mCurModule->find_var(symbol->str_val());
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
  void* p = mAlloc.get_memory(sizeof(AstPrimary));
  return new (p) AstPrimary(var_decl, symbol->file_region());
}

// @brief 文字列型を作る．
// @param[in] loc ファイル位置
AstValueType*
YmslDriver::new_StringType(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstStringType));
  return new (p) AstStringType(loc);
}

// @brief void型を作る．
// @param[in] loc ファイル位置
AstValueType*
YmslDriver::new_VoidType(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstVoidType));
  return new (p) AstVoidType(loc);
}

// @brief boolean型を作る．
// @param[in] loc ファイル位置
AstValueType*
YmslDriver::new_BooleanType(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstBooleanType));
  return new (p) AstBooleanType(loc);
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

// @brief ラベルを作る．
// @param[in] name ラベル名
YmslLabel*
YmslDriver::new_label(YmslCodeList& code_list,
		      ShString name)
{
  void* p = mAlloc.get_memory(sizeof(YmslLabel));
  return new (p) YmslLabel(code_list, name);
}

END_NAMESPACE_YM_YMSL
