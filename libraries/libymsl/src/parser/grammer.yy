%{

/// @file grammer.yy
/// @brief grammer の構文解析器
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

// C++ 用の定義

#include "YmUtils/FileRegion.h"
#include "YmUtils/MsgMgr.h"

#include "YmslDriver.h"

#include "AstBlock.h"
#include "AstCaseItem.h"
#include "AstExpr.h"
#include "AstIfBlock.h"
#include "AstVarDecl.h"


// より詳細なエラー情報を出力させる．
#define YYERROR_VERBOSE 1

// 位置を表す型
// (yylloc の型)
#define YYLTYPE FileRegion

// YYLTYPE を書き換えたので以下のマクロも書き換えが必要
#define YYLLOC_DEFAULT(Current, Rhs, N) Current = fr_merge(Rhs, N);


BEGIN_NAMESPACE_YM_YMSL

// 字句解析関数
int
yylex(YYSTYPE*,
      YYLTYPE*,
      YmslDriver&);

// エラー報告関数
int
yyerror(YYLTYPE*,
	YmslDriver&,
	const char*);


// fr_array 全体のファイル領域を求める．
// 直感的には FileRegion(fr_array[1], fr_array[n])
// だが(先頭が 0 でなく 1 であることに注意),
// 場合によっては空のトークンで位置がないばあいがあるので
// それをスキップしなければならない．
inline
FileRegion
fr_merge(const FileRegion fr_array[],
	 ymuint n)
{
  if ( n == 0 ) {
    // なんでこんなことがあるのか不明
    return FileRegion();
  }

  // 真の先頭を求める．
  ymuint i;
  for (i = 1; i <= n && !fr_array[i].is_valid(); ++ i) ;
  const FileRegion& first = fr_array[i];

  // 真の末尾を求める．
  ymuint j;
  for (j = n; j >= i && !fr_array[j].is_valid(); -- j) ;
  const FileRegion& last = fr_array[j];

  return FileRegion(first, last);
}

%}

// Yacc 用の定義

// "pure" driver にする．
%define api.pure

// 位置のトラッキングを行う．
%locations

// yyparse の引数
%parse-param {YmslDriver& driver}

// yylex の引数
%lex-param {YmslDriver& driver}

// 値を表す型
%union {
  AstBlock* block_type;
  AstCaseItem* caseitem_type;
  AstExpr* expr_type;
  AstIfBlock* ifblock_type;
  AstStatement* statement_type;
  AstSymbol* symbol_type;
  AstVarDecl* vardecl_type;
  AstValueType* valuetype_type;
}


// 終端記号
%token COLON
%token SEMI
%token COMMA
%token DOT
%token PLUSPLUS
%token MINUSMINUS
%token EQ
%token LP
%token RP
%token LCB
%token RCB
%token LBK
%token RBK
%token IF
%token ELSE
%token ELIF
%token FOR
%token WHILE
%token SWITCH
%token CASE
%token DEFAULT
%token DO
%token GOTO
%token BREAK
%token CONTINUE
%token FUNCTION
%token RETURN
%token VAR
%token GLOBAL
%token INT
%token FLOAT
%token STRING
%token <symbol_type> SYMBOL
%token <expr_type> INT_VAL
%token <expr_type> FLOAT_VAL
%token <expr_type> STRING_VAL

%token DUMMY
%token ERROR

// 演算子の定義
%left LOGOR LOGXOR
%left LOGAND
%left BITOR BITXOR
%left BITAND
%left EQEQ NOTEQ
%left LT LE GT GE
%left PLUS MINUS
%left MULT DIV MOD
%left LOGNOT BITNEG UOP

// 非終端の型
%type <block_type> statement_block
%type <caseitem_type> case_list
%type <expr_type> expr
%type <expr_type> expr_list
%type <expr_type> init_expr
%type <expr_type> lvalue
%type <ifblock_type> elif_list
%type <ifblock_type> else_block
%type <statement_type> block_statement
%type <statement_type> single_statement
%type <statement_type> statement
%type <valuetype_type> type
%type <vardecl_type> var_decl param_list

%%

// 本体

%start item_list;

// トップレベルの要素リスト
item_list
: // 空
| item_list item
;

// トップレベルの要素
// ステートメントか関数定義
item
: statement
{
  driver.add_statement($1);
}
// 関数宣言
| func_head SYMBOL LP param_list RP COLON type SEMI
{
  AstFuncDecl* funcdecl = driver.new_FuncDecl($2, $7, $4, @$);
  driver.pop_block();
  driver.add_function(funcdecl);
}
// 関数定義
| func_head SYMBOL LP param_list RP COLON type LCB statement_list RCB
{
  AstFuncDecl* funcdecl = driver.new_FuncDecl($2, $7, $4, @$);
  driver.pop_block();
  driver.add_function(funcdecl);
}
// ローカル変数定義
| var_decl SEMI
{
  driver.add_local_var($1);
}
// グローバル変数定義
| GLOBAL SYMBOL COLON type init_expr SEMI
{
  AstVarDecl* vardecl = driver.new_VarDecl($2, $4, $5, true, @$);
  driver.add_global_var(vardecl);
}
;

// 関数宣言のヘッダ
func_head
: FUNCTION
{
  driver.push_new_block();
}
;

// ステートメント
statement
: single_statement SEMI
{
  $$ = $1;
}
| block_statement
{
  $$ = $1;
}
// ラベル文
| SYMBOL COLON
{
  $$ = driver.new_Label($1, @$);
}
;

single_statement
// 代入文
: lvalue EQ expr
{
  $$ = driver.new_Assignment($1, $3);
}
// 式文
| expr
{
  $$ = driver.new_ExprStmt($1);
}
// GOTO 文
| GOTO SYMBOL
{
  $$ = driver.new_Goto($2, @$);
}
// BREAK 文
| BREAK
{
  $$ = driver.new_Break(@$);
}
// CONTINUE 文
| CONTINUE
{
  $$ = driver.new_Continue(@$);
}
// RETURN 文(引数なし)
| RETURN
{
  $$ = driver.new_Return(NULL, @$);
}
// RETURN 文(引数あり)
| RETURN expr
{
  $$ = driver.new_Return($2, @$);
}
;

// 複合文
block_statement
// IF 文
: IF expr statement_block elif_list else_block
{
  AstIfBlock* top = driver.new_IfBlock($2, $3, FileRegion(@1, @3));
  $$ = driver.new_If(top, $4, $5, @$);
}
// FOR 文
| FOR LP single_statement SEMI expr SEMI single_statement RP statement_block
{
  $$ = driver.new_For($3, $5, $7, $9, @$);
}
// WHILE 文
| WHILE LP expr RP statement_block
{
  $$ = driver.new_While($3, $5, @$);
}
// DO-WHILE 文
| DO statement_block WHILE LP expr RP
{
  $$ = driver.new_DoWhile($2, $5, @$);
}
// SWITCH 文
| SWITCH expr LCB case_list RCB
{
  $$ = driver.new_Switch($2, $4, @$);
}
// ブロック文
| statement_block
{
  $$ = driver.new_BlockStmt($1, @$);
}
;

// ブロック
statement_block
: stmt_begin statement_list RCB
{
  $$ = driver.cur_block();
  driver.pop_block();
}
;

// ブロック文の開始
// わざと非終端ノードを導入したのは
// 本体よりも先にアクションを起こすため．
stmt_begin
: LCB
{
  driver.push_new_block();
}
;

statement_list
: // 空
| statement_list statement
{
  driver.add_statement($2);
}
;

elif_list
: // 空
{
  $$ = NULL;
}
| elif_list ELIF expr statement_block
{
  $$ = driver.new_IfBlock($3, $4, FileRegion(@2,@4));
  $$->set_prev($1);
}
;

else_block
: // 空
{
  $$ = NULL;
}
| ELSE statement_block
{
  $$ = driver.new_IfBlock(NULL, $2, @$);
}
;

case_list
: // 空
{
  $$ = NULL;
}
| case_list CASE expr COLON statement_block
{
  $$ = driver.new_CaseItem($3, $5, FileRegion(@2, @5));
  $$->set_prev($1);
}
| case_list DEFAULT COLON statement_block
{
  $$ = driver.new_CaseItem(NULL, $4, FileRegion(@2, @4));
  $$->set_prev($1);
}
;

lvalue
: SYMBOL
{
  $$ = driver.new_VarExpr($1);
}
| SYMBOL LBK expr RBK
{
  $$ = driver.new_ArrayRef($1, $3, @$);
}
;

param_list
: // 空
{
  $$ = NULL;
}
| var_decl
{
  $$ = $1;
}
| param_list COMMA var_decl
{
  $$ = $3;
  $$->set_prev($1);
}
;

// 変数宣言
var_decl
: SYMBOL COLON type init_expr
{
  $$ = driver.new_VarDecl($1, $3, $4, false, @$);
}
;

// データ型
type
: INT
{
  $$ = driver.new_IntType(@$);
}
| FLOAT
{
  $$ = driver.new_FloatType(@$);
}
| STRING
{
  $$ = driver.new_StringType(@$);
}
| SYMBOL
{
  $$ = driver.new_UserType($1);
}
;

init_expr
: // 空
{
  $$ = NULL;
}
| EQ expr
{
  $$ = $2;
}
;

expr_list
: // 空
{
  $$ = NULL;
}
| expr
{
  $$ = $1;
}
| expr_list COMMA expr
{
  $$ = $3;
  $$->set_prev($1);
}
;

expr
: expr PLUS expr
{
  $$ = driver.new_BinOp(PLUS, $1, $3);
}
| expr MINUS expr
{
  $$ = driver.new_BinOp(MINUS, $1, $3);
}
| expr MULT expr
{
  $$ = driver.new_BinOp(MULT, $1, $3);
}
| expr DIV expr
{
  $$ = driver.new_BinOp(DIV, $1, $3);
}
| expr MOD expr
{
  $$ = driver.new_BinOp(MOD, $1, $3);
}
| expr LOGOR expr
{
  $$ = driver.new_BinOp(LOGOR, $1, $3);
}
| expr LOGAND expr
{
  $$ = driver.new_BinOp(LOGAND, $1, $3);
}
| LOGNOT expr
{
  $$ = driver.new_UniOp(LOGNOT, $2, @$);
}
| expr BITAND expr
{
  $$ = driver.new_BinOp(BITAND, $1, $3);
}
| expr BITOR expr
{
  $$ = driver.new_BinOp(BITOR, $1, $3);
}
| BITNEG expr
{
  $$ = driver.new_UniOp(BITNEG, $2, @$);
}
| expr EQEQ expr
{
  $$ = driver.new_BinOp(EQEQ, $1, $3);
}
| expr NOTEQ expr
{
  $$ = driver.new_BinOp(NOTEQ, $1, $3);
}
| expr LT expr
{
  $$ = driver.new_BinOp(LT, $1, $3);
}
| expr GT expr
{
  $$ = driver.new_BinOp(GT, $1, $3);
}
| expr LE expr
{
  $$ = driver.new_BinOp(LE, $1, $3);
}
| expr GE expr
{
  $$ = driver.new_BinOp(GE, $1, $3);
}
| PLUS expr %prec UOP
{
  $$ = driver.new_UniOp(PLUS, $2, @$);
}
| MINUS expr %prec UOP
{
  $$ = driver.new_UniOp(MINUS, $2, @$);
}
| LP expr RP
{
  $$ = $2;
}
| SYMBOL
{
  $$ = driver.new_VarExpr($1);
  if ( $$ == NULL ) {
    // 変数が見つからない
  }
}
| SYMBOL LBK expr RBK
{
  $$ = driver.new_ArrayRef($1, $3, @$);
}
| SYMBOL LP expr_list RP
{
  $$ = driver.new_FuncCall($1, $3, @$);
  if ( $$ == NULL ) {
    // 関数が見つからない
  }
}
| INT_VAL
{
  $$ = $1;
}
| FLOAT_VAL
{
  $$ = $1;
}
| STRING_VAL
{
  $$ = $1;
}
;

%%

// C++ の記述

// yacc パーサーが内部で呼び出す関数
// トークンを一つ読み込みトークン番号を返す．
// 同時に意味値(semantic value)を yylval にセットする．
// また位置を yylloc にセットする．
int
yylex(YYSTYPE* lvalp,
      YYLTYPE* llocp,
      YmslDriver& driver)
{
  return driver.yylex(*lvalp, *llocp);
}

// yacc パーサーが内部で呼び出す関数
// エラーメッセージを出力する．
int
yyerror(YYLTYPE* llocp,
	YmslDriver& driver,
	const char* s)
{
  string s2;
  // 好みの問題だけど "parse error" よりは "syntax error" の方が好き．
  if ( !strncmp(s, "parse error", 11) ) {
    s2 ="syntax error";
    s2 += (s + 11);
  }
  else {
    s2 = s;
  }

  MsgMgr::put_msg(__FILE__, __LINE__,
		  *llocp,
		  kMsgError,
		  "PARS",
		  s2);

  return 1;
}

END_NAMESPACE_YM_YMSL
