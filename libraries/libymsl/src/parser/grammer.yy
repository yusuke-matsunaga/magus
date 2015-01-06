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

#include "AstMgr.h"
#include "AstExpr.h"
#include "AstList.h"
#include "AstSymbol.h"
#include "AstFuncDecl.h"
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
      AstMgr& mgr);

// エラー報告関数
int
yyerror(YYLTYPE*,
	AstMgr&,
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
%parse-param {AstMgr& mgr}

// yylex の引数
%lex-param {AstMgr& mgr}

// 値を表す型
%union {
  TokenType      token_type;
  AstCaseList*   caselist_type;
  AstExpr*       expr_type;
  AstExprList*   exprlist_type;
  AstIfList*     iflist_type;
  AstModule*     module_type;
  AstModuleList* modulelist_type;
  AstPrimary*    primary_type;
  AstStatement*  statement_type;
  AstStmtList*   stmtlist_type;
  AstSymbol*     symbol_type;
  AstSymbolList* symbollist_type;
  AstVarDecl*    vardecl_type;
  AstVarList*    varlist_type;
  AstValueType*  valuetype_type;
}


// 終端記号
%token COLON
%token SEMI
%token COMMA
%token DOT
%token QST
%token PLUSPLUS
%token MINUSMINUS
%token EQ
%token EQPLUS
%token EQMINUS
%token EQMULT
%token EQDIV
%token EQMOD
%token EQLSHIFT
%token EQRSHIFT
%token EQBITAND
%token EQBITOR
%token EQBITXOR
%token LP
%token RP
%token LCB
%token RCB
%token LBK
%token RBK

%token AS
%token BOOLEAN
%token BREAK
%token CASE
%token CONTINUE
%token DEFAULT
%token DO
%token ELIF
%token ELSE
%token FLOAT
%token FOR
%token FUNCTION
%token GLOBAL
%token GOTO
%token IF
%token IMPORT
%token INT
%token RETURN
%token STRING
%token SWITCH
%token VAR
%token VOID
%token WHILE

%token <symbol_type> SYMBOL
%token <expr_type>   INT_VAL
%token <expr_type>   FLOAT_VAL
%token <expr_type>   STRING_VAL

%token DUMMY
%token ERROR

// 演算子の定義
%nonassoc QST
%nonassoc ITE
%left     LOGOR
%left     LOGAND
%nonassoc LSHIFT RSHIFT
%left     BITOR BITXOR
%left     BITAND
%nonassoc EQEQ NOTEQ LT LE GT GE
%left     PLUS MINUS
%left     MULT DIV MOD
%left     LOGNOT BITNEG UOP

// 非終端の型
%type <caselist_type>   case_list
%type <expr_type>       expr
%type <expr_type>       init_expr
%type <exprlist_type>   expr_list
%type <primary_type>    lvalue
%type <iflist_type>     if_list
%type <iflist_type>     if_else_list
%type <module_type>     module
%type <modulelist_type> module_list
%type <statement_type>  complex_statement
%type <statement_type>  item
%type <statement_type>  single_statement
%type <statement_type>  statement
%type <stmtlist_type>   item_list
%type <stmtlist_type>   statement_list
%type <symbollist_type> identifier
%type <token_type>      eqop
%type <valuetype_type>  type
%type <vardecl_type>    param
%type <varlist_type>    param_list

%%

// 本体

%start item_top;

item_top
: item_list
{
  mgr.set_root($1, @$);
}

// 要素のリスト
item_list
: // 空
{
  $$ = new AstStmtList;
}
| item_list item
{
  $$ = $1;
  $$->add($2);
}
;

// トップレベルの要素
// ステートメントか関数定義
item
: statement
{
  $$ = $1;
}
// 関数定義
| FUNCTION SYMBOL LP param_list RP COLON type LCB statement_list RCB
{
  $$ = mgr.new_FuncDecl($2, $7, $4, $9, @$);
}
// グローバル変数定義
| GLOBAL SYMBOL COLON type init_expr SEMI
{
  $$ = mgr.new_VarDecl($2, $4, $5, true, @$);
}
// import 文
| IMPORT module_list SEMI
{
  $$ = mgr.new_Import($2, @$);
}
;

// import のモジュールリスト
module_list
: module
{
  $$ = new AstModuleList;
  $$->add($1);
}
| module_list COMMA module
{
  $$ = $1;
  $$->add($3);
}
;

module
: SYMBOL
{
  $$ = mgr.new_Module($1, NULL, @$);
}
| SYMBOL AS SYMBOL
{
  $$ = mgr.new_Module($1, $3, @$);
}
;

// ステートメント
statement
// 単一のステートメント
: single_statement SEMI
{
  $$ = $1;
}
// 複合文
| complex_statement
{
  $$ = $1;
}
// ローカル変数定義
| VAR SYMBOL COLON type init_expr SEMI
{
  $$ = mgr.new_VarDecl($2, $4, $5, false, @$);
}
// ラベル文
| SYMBOL COLON
{
  $$ = mgr.new_Label($1, @$);
}
// エラー回復用のルール
| error SEMI
{
  yyerrok;
}
;

single_statement
// 代入文
: lvalue eqop expr
{
  $$ = mgr.new_Assignment($2, $1, $3);
}
// 式文
| expr
{
  $$ = mgr.new_ExprStmt($1);
}
// インクリメント文
| lvalue PLUSPLUS
{
}
// デクリメント文
| lvalue MINUSMINUS
{
}
// GOTO 文
| GOTO SYMBOL
{
  $$ = mgr.new_Goto($2, @$);
}
// BREAK 文
| BREAK
{
  $$ = mgr.new_Break(@$);
}
// CONTINUE 文
| CONTINUE
{
  $$ = mgr.new_Continue(@$);
}
// RETURN 文(引数なし)
| RETURN
{
  $$ = mgr.new_Return(NULL, @$);
}
// RETURN 文(引数あり)
| RETURN expr
{
  $$ = mgr.new_Return($2, @$);
}
;

// 代入文の演算子
// なんかバカみたいなアクション定義
eqop
: EQ       { $$ = EQ; }
| EQPLUS   { $$ = EQPLUS; }
| EQMINUS  { $$ = EQMINUS; }
| EQMULT   { $$ = EQMULT; }
| EQDIV    { $$ = EQDIV; }
| EQMOD    { $$ = EQMOD; }
| EQLSHIFT { $$ = EQLSHIFT; }
| EQRSHIFT { $$ = EQRSHIFT; }
| EQBITAND { $$ = EQBITAND; }
| EQBITOR  { $$ = EQBITOR; }
| EQBITXOR { $$ = EQBITXOR; }
;

// 複合文
// というか末尾にセミコロンがこない文
complex_statement
// IF 文
: if_else_list
{
  $$ = mgr.new_If($1, @$);
}
// FOR 文
| FOR LP single_statement SEMI expr SEMI single_statement RP LCB statement_list RCB
{
  $$ = mgr.new_For($3, $5, $7, $10, @$);
}
// WHILE 文
| WHILE LP expr RP LCB statement_list RCB
{
  $$ = mgr.new_While($3, $6, @$);
}
// DO-WHILE 文
| DO LCB statement_list RCB WHILE LP expr RP
{
  $$ = mgr.new_DoWhile($3, $7, @$);
}
// SWITCH 文
| SWITCH expr LCB case_list RCB
{
  $$ = mgr.new_Switch($2, $4, @$);
}
// ブロック文
| LCB statement_list RCB
{
  $$ = mgr.new_BlockStmt($2, @$);
}
;

statement_list
: // 空
{
  $$ = new AstStmtList;
}
| statement_list statement
{
  $$ = $1;
  $$->add($2);
}
;

// if 〜 elif 〜 のリスト
if_list
: IF expr LCB statement_list RCB
{
  AstIfBlock* block = mgr.new_IfBlock($2, $4, @$);
  $$ = new AstIfList;
  $$->add(block);
}
| if_list ELIF expr LCB statement_list RCB
{
  AstIfBlock* block = mgr.new_IfBlock($3, $5, FileRegion(@2, @6));
  $$ = $1;
  $$->add(block);
}
;

// if_list の末尾にオプションで else 〜 がくるもの
if_else_list
: if_list
{
  $$ = $1;
}
| if_list ELSE LCB statement_list RCB
{
  AstIfBlock* block = mgr.new_IfBlock(NULL, $4, FileRegion(@2, @5));
  $$ = $1;
  $$->add(block);
}
;

case_list
: // 空
{
  $$ = new AstCaseList;
}
| case_list CASE expr COLON LCB statement_list RCB
{
  AstCaseItem* item = mgr.new_CaseItem($3, $6, FileRegion(@2, @7));
  $$ = $1;
  $$->add(item);
}
| case_list DEFAULT COLON LCB statement_list RCB
{
  AstCaseItem* item = mgr.new_CaseItem(NULL, $5, FileRegion(@2, @6));
  $$ = $1;
  $$->add(item);
}
;

// 左辺値
lvalue
: identifier
{
  $$ = mgr.new_Primary($1, @$);
}
/*
| identifier LBK expr RBK
{
  $$ = mgr.new_ArrayRef($1, $3, @$);
}
*/
;

// 関数定義の引数リスト
param_list
: // 空
{
  $$ = new AstVarList;
}
| param
{
  $$ = new AstVarList;
  $$->add($1);
}
| param_list COMMA param
{
  $$ = $1;
  $$->add($3);
}
;

// パラメータ宣言
param
: SYMBOL COLON type init_expr
{
  $$ = mgr.new_VarDecl($1, $3, $4, false, @$);
}
;

// データ型
type
: VOID
{
  $$ = mgr.new_VoidType(@$);
}
| BOOLEAN
{
  $$ = mgr.new_BooleanType(@$);
}
| INT
{
  $$ = mgr.new_IntType(@$);
}
| FLOAT
{
  $$ = mgr.new_FloatType(@$);
}
| STRING
{
  $$ = mgr.new_StringType(@$);
}
| SYMBOL
{
  $$ = mgr.new_UserType($1);
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

expr
// 単項演算
: PLUS expr %prec UOP
{
  $$ = mgr.new_UniOp(PLUS, $2, @$);
}
| MINUS expr %prec UOP
{
  $$ = mgr.new_UniOp(MINUS, $2, @$);
}
| BITNEG expr
{
  $$ = mgr.new_UniOp(BITNEG, $2, @$);
}
| LOGNOT expr
{
  $$ = mgr.new_UniOp(LOGNOT, $2, @$);
}
| INT LP expr RP
{
  $$ = mgr.new_UniOp(INT, $3, @$);
}
| BOOLEAN LP expr RP
{
  $$ = mgr.new_UniOp(BOOLEAN, $3, @$);
}
| FLOAT LP expr RP
{
  $$ = mgr.new_UniOp(FLOAT, $3, @$);
}
// 二項演算
| expr PLUS expr
{
  $$ = mgr.new_BinOp(PLUS, $1, $3);
}
| expr MINUS expr
{
  $$ = mgr.new_BinOp(MINUS, $1, $3);
}
| expr MULT expr
{
  $$ = mgr.new_BinOp(MULT, $1, $3);
}
| expr DIV expr
{
  $$ = mgr.new_BinOp(DIV, $1, $3);
}
| expr MOD expr
{
  $$ = mgr.new_BinOp(MOD, $1, $3);
}
| expr LSHIFT expr
{
  $$ = mgr.new_BinOp(LSHIFT, $1, $3);
}
| expr RSHIFT expr
{
  $$ = mgr.new_BinOp(RSHIFT, $1, $3);
}
| expr BITAND expr
{
  $$ = mgr.new_BinOp(BITAND, $1, $3);
}
| expr BITOR expr
{
  $$ = mgr.new_BinOp(BITOR, $1, $3);
}
| expr BITXOR expr
{
  $$ = mgr.new_BinOp(BITXOR, $1, $3);
}
| expr LOGAND expr
{
  $$ = mgr.new_BinOp(LOGAND, $1, $3);
}
| expr LOGOR expr
{
  $$ = mgr.new_BinOp(LOGOR, $1, $3);
}
| expr EQEQ expr
{
  $$ = mgr.new_BinOp(EQEQ, $1, $3);
}
| expr NOTEQ expr
{
  $$ = mgr.new_BinOp(NOTEQ, $1, $3);
}
| expr LT expr
{
  $$ = mgr.new_BinOp(LT, $1, $3);
}
| expr GT expr
{
  $$ = mgr.new_BinOp(GT, $1, $3);
}
| expr LE expr
{
  $$ = mgr.new_BinOp(LE, $1, $3);
}
| expr GE expr
{
  $$ = mgr.new_BinOp(GE, $1, $3);
}
| expr QST expr COLON expr %prec ITE
{
  $$ = mgr.new_IteOp($1, $3, $5);
}
| LP expr RP
{
  $$ = $2;
}
| identifier
{
  $$ = mgr.new_VarExpr($1, @$);
}
/*
| identifier LBK expr RBK
{
  $$ = mgr.new_ArrayRef($1, $3, @$);
}
*/
| identifier LP expr_list RP
{
  $$ = mgr.new_FuncCall($1, $3, @$);
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

// 式のリスト
expr_list
: // 空
{
  $$ = new AstExprList;
}
| expr
{
  $$ = new AstExprList;
  $$->add($1);
}
| expr_list COMMA expr
{
  $$ = $1;
  $$->add($3);
}
;

// 識別子
identifier
: SYMBOL
{
  $$ = new AstSymbolList;
  $$->add($1);
}
| identifier DOT SYMBOL
{
  $$ = $1;
  $$->add($3);
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
      AstMgr& mgr)
{
  return mgr.scan(*lvalp, *llocp);
}

// yacc パーサーが内部で呼び出す関数
// エラーメッセージを出力する．
int
yyerror(YYLTYPE* llocp,
	AstMgr& mgr,
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
