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

#include "YmslParser.h"

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
      YmslParser&);

// エラー報告関数
int
yyerror(YYLTYPE*,
	YmslParser&,
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

// "pure" parser にする．
%define api.pure

// 位置のトラッキングを行う．
%locations

// yyparse の引数
%parse-param {YmslParser& parser}

// yylex の引数
%lex-param {YmslParser& parser}

// 値を表す型
%union {
  int int_type;
  double float_type;
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
%token INT
%token FLOAT
%token STRING
%token <symbol_type> SYMBOL
%token <int_type> INT_NUM
%token <float_type> FLOAT_NUM
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
%type <statement_type> statement
%type <valuetype_type> type
%type <vardecl_type> var_decl param_list

%%

// 本体

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
  parser.add_statement($1);
}
// 関数宣言
| func_head SYMBOL LP param_list RP COLON type statement_block
{
  parser.new_AstFuncDecl($2, $7, $4, $8, @$);
  parser.pop_block();
}
| var_decl
{
  AstBlock* block = parser.cur_block();
  block->add_vardecl($1);
}
;

// 関数宣言のヘッダ
func_head
: FUNCTION
{
  parser.push_new_block();
}
;

// ステートメント
statement
// 代入文
: lvalue EQ expr SEMI
{
  $$ = parser.new_AstAssignment($1, $3);
}
// 式文
| expr SEMI
{
  $$ = parser.new_AstExprStmt($1);
}
// IF 文
| IF expr statement_block elif_list else_block
{
  AstIfBlock* top = parser.new_AstIfBlock($2, $3, FileRegion(@1, @3));
  $$ = parser.new_AstIf(top, $4, $5, @$);
}
// FOR 文
| FOR LP statement SEMI expr SEMI statement RP statement_block
{
  $$ = parser.new_AstFor($3, $5, $7, $9, @$);
}
// WHILE 文
| WHILE LP expr RP statement_block
{
  $$ = parser.new_AstWhile($3, $5, @$);
}
// DO-WHILE 文
| DO statement_block WHILE LP expr RP
{
  $$ = parser.new_AstDoWhile($2, $5, @$);
}
// SWITCH 文
| SWITCH expr LCB case_list RCB
{
  $$ = parser.new_AstSwitch($2, $4, @$);
}
// GOTO 文
| GOTO SYMBOL
{
  $$ = parser.new_AstGoto($2, @$);
}
// ラベル文
| SYMBOL COLON
{
  $$ = parser.new_AstLabel($1, @$);
}
// BREAK 文
| BREAK SEMI
{
  $$ = parser.new_AstBreak(@$);
}
// CONTINUE 文
| CONTINUE SEMI
{
  $$ = parser.new_AstContinue(@$);
}
// RETURN 文(引数なし)
| RETURN SEMI
{
  $$ = parser.new_AstReturn(NULL, @$);
}
// RETURN 文(引数あり)
| RETURN LP expr RP SEMI
{
  $$ = parser.new_AstReturn($3, @$);
}
// ブロック文
| statement_block
{
  $$ = parser.new_AstBlockStmt($1, @$);
}
;

// ブロック
statement_block
: stmt_begin statement_list RCB
{
  $$ = parser.cur_block();
  parser.pop_block();
}
;

// ブロック文の開始
// わざと非終端ノードを導入したのは
// 本体よりも先にアクションを起こすため．
stmt_begin
: LCB
{
  parser.push_new_block();
}
;

statement_list
: // 空
| statement_list statement
{
  parser.add_statement($2);
}
;

elif_list
: // 空
{
  $$ = NULL;
}
| elif_list ELIF expr statement_block
{
  $$ = parser.new_AstIfBlock($3, $4, FileRegion(@2,@4));
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
  $$ = parser.new_AstIfBlock(NULL, $2, @$);
}
;

case_list
: // 空
{
  $$ = NULL;
}
| case_list CASE expr COLON statement_block
{
  $$ = parser.new_AstCaseItem($3, $5, FileRegion(@2, @5));
  $$->set_prev($1);
}
| case_list DEFAULT COLON statement_block
{
  $$ = parser.new_AstCaseItem(NULL, $4, FileRegion(@2, @4));
  $$->set_prev($1);
}
;

lvalue
: SYMBOL
{
  $$ = parser.new_AstVarExpr($1);
}
| SYMBOL LBK expr RBK
{
  $$ = parser.new_AstArrayRef($1, $3, @$);
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
  $$ = parser.new_AstVarDecl($1, $3, $4, @$);
}
;

// データ型
type
: INT
{
  $$ = parser.new_AstIntType(@$);
}
| FLOAT
{
  $$ = parser.new_AstFloatType(@$);
}
| STRING
{
  $$ = parser.new_AstStringType(@$);
}
| SYMBOL
{
  $$ = parser.new_AstUserType($1);
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
  $$ = parser.new_AstBinOp(PLUS, $1, $3);
}
| expr MINUS expr
{
  $$ = parser.new_AstBinOp(MINUS, $1, $3);
}
| expr MULT expr
{
  $$ = parser.new_AstBinOp(MULT, $1, $3);
}
| expr DIV expr
{
  $$ = parser.new_AstBinOp(DIV, $1, $3);
}
| expr MOD expr
{
  $$ = parser.new_AstBinOp(MOD, $1, $3);
}
| expr LOGOR expr
{
  $$ = parser.new_AstBinOp(LOGOR, $1, $3);
}
| expr LOGAND expr
{
  $$ = parser.new_AstBinOp(LOGAND, $1, $3);
}
| LOGNOT expr
{
  $$ = parser.new_AstUniOp(LOGNOT, $2, @$);
}
| expr BITAND expr
{
  $$ = parser.new_AstBinOp(BITAND, $1, $3);
}
| expr BITOR expr
{
  $$ = parser.new_AstBinOp(BITOR, $1, $3);
}
| BITNEG expr
{
  $$ = parser.new_AstUniOp(BITNEG, $2, @$);
}
| expr EQEQ expr
{
  $$ = parser.new_AstBinOp(EQEQ, $1, $3);
}
| expr NOTEQ expr
{
  $$ = parser.new_AstBinOp(NOTEQ, $1, $3);
}
| expr LT expr
{
  $$ = parser.new_AstBinOp(LT, $1, $3);
}
| expr GT expr
{
  $$ = parser.new_AstBinOp(GT, $1, $3);
}
| expr LE expr
{
  $$ = parser.new_AstBinOp(LE, $1, $3);
}
| expr GE expr
{
  $$ = parser.new_AstBinOp(GE, $1, $3);
}
| PLUS expr %prec UOP
{
  $$ = parser.new_AstUniOp(PLUS, $2, @$);
}
| MINUS expr %prec UOP
{
  $$ = parser.new_AstUniOp(MINUS, $2, @$);
}
| LP expr RP
{
  $$ = $2;
}
| SYMBOL
{
  $$ = parser.new_AstVarExpr($1);
  if ( $$ == NULL ) {
  }
}
| SYMBOL LBK expr RBK
{
  $$ = parser.new_AstArrayRef($1, $3, @$);
}
| SYMBOL LP expr_list RP
{
  $$ = parser.new_AstFuncCall($1, $3, @$);
}
| INT_NUM
{
  $$ = parser.new_AstIntConst($1, @$);
}
| FLOAT_NUM
{
  $$ = parser.new_AstFloatConst($1, @$);
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
      YmslParser& parser)
{
  return parser.yylex(*lvalp, *llocp);
}

// yacc パーサーが内部で呼び出す関数
// エラーメッセージを出力する．
int
yyerror(YYLTYPE* llocp,
	YmslParser& parser,
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
