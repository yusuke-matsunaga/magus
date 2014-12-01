%{

/// @file grammer.yy
/// @brief grammer の構文解析器
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

// C++ 用の定義

#include "YmslParser.h"
#include "YmslAst.h"
#include "YmUtils/FileRegion.h"
#include "YmUtils/MsgMgr.h"

// より詳細なエラー情報を出力させる．
#define YYERROR_VERBOSE 1

// 値を表す型
#define YYSTYPE YmslAst*

// 位置を表す型
// (yylloc の型)
#define YYLTYPE FileRegion

// YYLTYPE を書き換えたので以下のマクロも書き換えが必要
#define YYLLOC_DEFAULT(Current, Rhs, N) Current = fr_merge(Rhs, N);


BEGIN_NAMESPACE_YM_YMSL

#include "grammer.hh"

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
%token SYMBOL
%token INT_NUM
%token FLOAT_NUM
%token STRING_VAL

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

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

// 本体

// トップレベルの要素リスト
item_list
: // 空
{
  $$ = parser.new_AstList();
  parser.set_root($$);
}
| item_list item
{
  $$ = $1;
  $$->add_child($2);
}
;

// トップレベルの要素
// ステートメントか関数定義
item
: statement
{
  $$ = $1;
}
| func_decl
{
  $$ = $1;
}
;

// ステートメント
statement
: var_decl
{
  $$ = $1;
}
| lvalue EQ expr
{
  $$ = parser.new_AstAssignment($1, $3);
}
| SYMBOL LP expr_list RP
{
  $$ = parser.new_AstFuncCall($1, $3, @$);
}
| IF expr LCB statement_list RCB elif_list else_block
{
  $$ = parser.new_AstIf($2, $4, $6, $7, @$);
}
| FOR LP statement SEMI expr SEMI statement RP LCB statement_list RCB
{
  $$ = parser.new_AstFor($3, $5, $7, $10, @$);
}
| WHILE LP expr RP LCB statement_list RCB
{
  $$ = parser.new_AstWhile($2, $4, @$);
}
| DO LCB statement_list RCB WHILE LP expr RP
{
  $$ = parser.new_AstDoWhile($3, $6, @$);
}
| SWITCH expr LCB case_list RCB
{
  $$ = parser.new_AstSwitch($2, $4, @$);
}
| GOTO SYMBOL
{
  $$ = parser.new_AstGoto($2, @$);
}
| SYMBOL COLON
{
  $$ = parser.new_AstLabel($1, @$);
}
| BREAK
{
  $$ = parser.new_AstBreak(@$);
}
| CONTINUE
{
  $$ = parser.new_AstContinue(@$);
}
| RETURN
{
  $$ = parser.new_AstReturn(NULL, @$);
}
| RETURN LP expr RP
{
  $$ = parser.new_AstReturn($2, @$);
}
| LCB statement_list RCB
{
  $$ = parser.new_AstBlock($2, @$);
}
;

statement_list
: // 空
{
  $$ = parser.new_AstList();
}
| statement_list statement
{
  $$ = $1;
  $$->add_child($2);
}
;

lvalue
: SYMBOL
{
  $$ = $1;
}
| SYMBOL LBK expr RBK
{
  $$ = parser.new_AstArrayRef($1, $3, @$);
}
;

// 変数定義
var_decl
: VAR SYMBOL COLON type init_expr
{
  $$ = parser.new_AstVarDecl($2, $4, $5, @$);
}
| VAR SYMBOL LBK INT_NUM RBK COLON type
{
}
;

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
  $$ = $1;
}
;

elif_list
: // 空
{
  $$ = parser.new_AstList();
}
| elif_list ELIF expr LCB statement_list RCB
{
  $$ = $1;
  YmslAst* elif = parser.new_AstElif($3, $5, FileRegion(@2, @6));
  $$->add_child(elif);
}
;

else_block
: // 空
{
  $$ = NULL;
}
| ELSE LCB statement_list RCB
{
  $$ = $3;
}
;

case_list
: // 空
{
  $$ = parser.new_AstList();
}
| case_list CASE expr COLON statement_list
{
  $$ = $1;
  YmslAst* case_item = parser.new_AstCaseItem($3, $5, FileRegion(@2, @5));
  $$->add_child(case_item);
}
| case_list DEFAULT COLON statement_list
{
  $$ = $1;
  YmslAst* case_item = parser.new_AstCaseItem(NULL, $4, FileRegion(@2, @4));
  $$->add_child(case_item);
}
;

func_decl
: FUNCTION SYMBOL LP param_list RP COLON type LCB statement_list RCB
{
  $$ = parser.new_AstFuncDecl($2, $4, $7, $8, @$);
}
;

param_list
: // 空
{
  $$ = parser.new_AstList();
}
| param_list1
{
  $$ = $1;
}
;

param_list1
: param_decl
{
  $$ = parser.new_AstList();
  $$->add_child($1);
}
| param_list1 COMMA param_decl
{
  $$ = $1;
  $$->add_child($3);
}
;

param_decl
: SYMBOL COLON type
{
  $$ = parser.new_AstVarDecl($1, $3, NULL, @$);
}
| SYMBOL COLON type EQ expr
{
  $$ = parser.new_AstVarDecl($1, $3, $5, @$);
}
;

expr_list
: // 空
{
  $$ = parser.new_AstList();
}
| expr
{
  $$ = parser.new_AstList();
  $$->add_child($1);
}
| expr_list COMMA expr
{
  $$ = $1;
  $$->add_child($3);
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
  $$ = $1;
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
  $$ = $1;
}
| FLOAT_NUM
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
