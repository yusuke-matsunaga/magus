
/// @file YmslParser.cc
/// @brief YmslParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslParser.h"
#include "YmslScanner.h"
#include "YmslModule.h"
#include "AstMgr.h"
#include "AstBlock.h"
#include "AstFuncDecl.h"
#include "AstVarDecl.h"

#include "../builtin/YmslPrint.h"

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
#include "YmslLabel.h"

#include "YmUtils/MsgMgr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslParser
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
YmslParser::YmslParser()
{
  mScanner = NULL;
  mModule = NULL;

#if 0
  YmslFunc* func1 = new YmslPrint(ShString("print"), vector<YmslVar*>(0));
  mGlobalDict.add_function(func1);
#endif

  mDebug = false;
}

// @brief デストラクタ
YmslParser::~YmslParser()
{
  // 念の為
  delete mScanner;
}

// @brief トップレベルブロックを返す．
AstBlock*
YmslParser::toplevel_block() const
{
  return mModule->toplevel_block();
}

// @brief 一つのソースファイルを読み込む．
// @param[in] ido 入力データ
// @param[in] module モジュール
// @return 読み込みに成功したら true を返す．
bool
YmslParser::read_source(IDO& ido,
			YmslModule* module)
{
  int yyparser(YmslParser&, AstMgr&);

  mScanner = new YmslScanner(ido);
  mModule = module;

  AstMgr mgr;

  int stat = yyparse(*this, mgr);

  delete mScanner;
  mScanner = NULL;

  mModule = NULL;

  return (stat == 0);
}

// @brief 現在のブロックを返す．
AstBlock*
YmslParser::cur_block() const
{
  return mModule->cur_block();
}

// @brief 新しいブロックを作りスタックに積む．
// @return 新しいブロックを返す．
AstBlock*
YmslParser::push_new_block()
{
  return mModule->push_new_block();
}

// @brief ブロックをスタックから取り去る．
void
YmslParser::pop_block()
{
  mModule->pop_block();
}

// @brief インポートする．
// @param[in] module_name モジュール名
// @param[in] alias_name エイリアス名
void
YmslParser::import(AstSymbol* module_name_sym,
		   AstSymbol* alias_name_sym)
{
#if 0
  ASSERT_COND( module_name_sym->next() == NULL );
  ASSERT_COND( alias_name_sym == NULL || alias_name_sym->next() == NULL );
  ShString module_name = module_name_sym->str_val();
  ShString alias_name = alias_name_sym != NULL ? alias_name_sym->str_val() : module_name;

  //IDO ido;
  IDO* p_ido;
  // module_name を探し．ido に結びつける．
  bool stat = read_source(*p_ido, module_name);
#endif
}

// @brief 関数を追加する．
void
YmslParser::add_function(AstFuncDecl* funcdecl)
{
  mModule->add_function(funcdecl);
}

// @brief グローバル変数を追加する．
// @param[in] vardecl 変数宣言
void
YmslParser::add_global_var(AstVarDecl* vardecl)
{
  mModule->add_global_var(vardecl);
}

// @brief 現在のブロックに変数を追加する．
void
YmslParser::add_local_var(AstVarDecl* vardecl)
{
  mModule->add_local_var(vardecl);
}

// @brief 現在のブロックに statement を追加する．
void
YmslParser::add_statement(AstStatement* stmt)
{
  mModule->add_statement(stmt);
}

// @brief yylex とのインターフェイス
// @param[out] lvalp 値を格納する変数
// @param[out] llocp 位置情報を格納する変数
// @return 読み込んだトークンの id を返す．
int
YmslParser::scan(YYSTYPE& lval,
		 FileRegion& lloc,
		 AstMgr& mgr)
{
  int id = mScanner->read_token(lloc);

  switch ( id ) {
  case SYMBOL:
    lval.symbol_type = mgr.new_Symbol(ShString(mScanner->cur_string()), lloc);
    break;

  case STRING_VAL:
    lval.expr_type = mgr.new_StringConst(mScanner->cur_string(), lloc);
    break;

  case INT_VAL:
    lval.expr_type = mgr.new_IntConst(mScanner->cur_int(), lloc);
    break;

  case FLOAT_VAL:
    lval.expr_type = mgr.new_FloatConst(mScanner->cur_float(), lloc);
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

// @brief 関数のリストを返す．
const vector<AstFuncDecl*>&
YmslParser::function_list() const
{
  return mModule->function_list();
}

// @brief グローバル変数のリストを返す．
const vector<AstVarDecl*>&
YmslParser::global_var_list() const
{
  return mModule->global_var_list();
}

// @brief 関数を探す．
// @param[in] name 関数名
//
// 見つからなければ NULL を返す．
AstFuncDecl*
YmslParser::find_function(ShString name) const
{
  return mModule->find_function(name);
}

// @brief 変数を探す．
// @param[in] name 変数名
//
// 見つからなければ NULL を返す．
AstVarDecl*
YmslParser::find_var(ShString name) const
{
  return mModule->find_var(name);
}

#if 0
// @brief 変数宣言を作る．
// @param[in] name 変数名
// @param[in] type 型
// @param[in] init_expr 初期化式
// @param[in] global グローバル変数の時 true にするフラグ
// @param[in] loc ファイル位置
AstVarDecl*
YmslParser::new_VarDecl(AstSymbol* name,
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
YmslParser::new_FuncDecl(AstSymbol* name,
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
YmslParser::new_Assignment(TokenType token,
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
YmslParser::new_If(AstIfBlock* if_list,
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
YmslParser::new_IfBlock(AstIfBlock* prev,
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
YmslParser::new_For(AstStatement* init,
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
YmslParser::new_While(AstExpr* cond,
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
YmslParser::new_DoWhile(AstBlock* block,
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
YmslParser::new_Switch(AstExpr* expr,
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
YmslParser::new_CaseItem(AstCaseItem* prev,
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
YmslParser::new_Goto(AstSymbol* label,
		     const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstGoto));
  return new (p) AstGoto(label, loc);
}

// @brief ラベルを作る．
// @param[in] label ラベル
// @param[in] loc ファイル位置
AstStatement*
YmslParser::new_Label(AstSymbol* label,
		      const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstLabel));
  return new (p) AstLabel(label, loc);
}

// @brief break 文を作る．
// @param[in] loc ファイル位置
AstStatement*
YmslParser::new_Break(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstBreak));
  return new (p) AstBreak(loc);
}

// @brief continue 文を作る．
// @param[in] loc ファイル位置
AstStatement*
YmslParser::new_Continue(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstContinue));
  return new (p) AstContinue(loc);
}

// @brief return 文を作る．
// @param[in] expr 値
// @param[in] loc ファイル位置
AstStatement*
YmslParser::new_Return(AstExpr* expr,
		       const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstReturn));
  return new (p) AstReturn(expr, loc);
}

// @brief ブロック文を作る．
// @param[in] block 本体
// @param[in] loc ファイル位置
AstStatement*
YmslParser::new_BlockStmt(AstBlock* block,
			  const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstBlockStmt));
  return new (p) AstBlockStmt(block, loc);
}

// @brief 式文を作る．
// @param[in] expr 式
AstStatement*
YmslParser::new_ExprStmt(AstExpr* expr)
{
  void* p = mAlloc.get_memory(sizeof(AstExprStmt));
  return new (p) AstExprStmt(expr);
}

// @brief 単項演算式を作る．
// @param[in] op 演算子のトークン
// @param[in] left オペランド
// @param[in] loc ファイル位置
AstExpr*
YmslParser::new_UniOp(TokenType op,
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
YmslParser::new_BinOp(TokenType op,
		      AstExpr* left,
		      AstExpr* right)
{
  void* p = mAlloc.get_memory(sizeof(AstBinOp));
  return new (p) AstBinOp(op, left, right);
}

// @brief ITE演算式を作る．
// @param[in] opr1, opr2, opr3 オペランド
AstExpr*
YmslParser::new_IteOp(AstExpr* opr1,
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
YmslParser::new_ArrayRef(AstSymbol* id,
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
YmslParser::new_FuncCall(AstSymbol* symbol,
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
YmslParser::new_VarExpr(AstSymbol* symbol,
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
YmslParser::new_IntConst(int val,
			 const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstIntConst));
  return new (p) AstIntConst(val, loc);
}

// @brief 浮動小数点定数式を作る．
// @param[in] val 値
// @param[in] loc ファイル位置
AstExpr*
YmslParser::new_FloatConst(double val,
			   const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstFloatConst));
  return new (p) AstFloatConst(val, loc);
}

// @brief 文字列定数式を作る．
// @param[in] val 値
// @param[in] loc ファイル位置
AstExpr*
YmslParser::new_StringConst(const char* val,
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
YmslParser::new_Primary(AstSymbol* symbol)
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
  void* p = mAlloc.get_memory(sizeof(AstPrimary));
  return new (p) AstPrimary(var_decl, symbol->file_region());
#else
  void* p = mAlloc.get_memory(sizeof(AstPrimary));
  return new (p) AstPrimary(symbol, symbol->file_region());
#endif
}

// @brief 文字列型を作る．
// @param[in] loc ファイル位置
AstValueType*
YmslParser::new_StringType(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstStringType));
  return new (p) AstStringType(loc);
}

// @brief void型を作る．
// @param[in] loc ファイル位置
AstValueType*
YmslParser::new_VoidType(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstVoidType));
  return new (p) AstVoidType(loc);
}

// @brief boolean型を作る．
// @param[in] loc ファイル位置
AstValueType*
YmslParser::new_BooleanType(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstBooleanType));
  return new (p) AstBooleanType(loc);
}

// @brief 整数型を作る．
// @param[in] loc ファイル位置
AstValueType*
YmslParser::new_IntType(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstIntType));
  return new (p) AstIntType(loc);
}

// @brief 浮動小数点型を作る．
// @param[in] loc ファイル位置
AstValueType*
YmslParser::new_FloatType(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(AstFloatType));
  return new (p) AstFloatType(loc);
}

// @brief ユーザー定義型を作る．
// @param[in] type_name 型名
// @param[in] loc ファイル位置
AstValueType*
YmslParser::new_UserType(AstSymbol* type_name)
{
  void* p = mAlloc.get_memory(sizeof(AstUserType));
  return new (p) AstUserType(type_name);
}

// @brief ラベルを作る．
// @param[in] name ラベル名
YmslLabel*
YmslParser::new_label(YmslCodeList& code_list,
		      ShString name)
{
  void* p = mAlloc.get_memory(sizeof(YmslLabel));
  return new (p) YmslLabel(code_list, name);
}
#endif

END_NAMESPACE_YM_YMSL
