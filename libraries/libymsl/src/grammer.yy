%{

/// @file grammer.yy
/// @brief grammer の構文解析器
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

// C++ 用の定義

#include "YmslParser.h"
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


// トークンの型
//%union {
//}

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
%token DO
%token GOTO
%token BREAK
%token CONTINUE
%token FUNCTION
%token RETURN
%token VAR
%token INT
%token FLOAT
%token SYMBOL
%token INT_NUM
%token FLOAT_NUM
%token STRING
%token END

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
| item_list item
;

// トップレベルの要素
// ステートメントか関数定義
item
: statement
| func_decl
;

// ステートメント
statement
: var_decl
| lvalue EQ expr
| SYMBOL LP expr_list RP
| IF expr LCB statement_list RCB else_block
| FOR LP statement SEMI expr SEMI statement RP LCB statement_list RCB
| WHILE expr LCB statement_list RCB
| DO LCB statement_list RCB WHILE expr
| SWITCH expr LCB case_list RCB
| GOTO SYMBOL
| SYMBOL COLON
| BREAK
| CONTINUE
| RETURN
;

statement_list
: // 空
| statement_list statement
;

lvalue
: SYMBOL
| SYMBOL LBK expr RBK
;

// 変数定義
var_decl
: VAR SYMBOL COLON type init_expr
| VAR SYMBOL LBK INT_NUM RBK COLON type
;

type
: INT
| FLOAT
| SYMBOL
;

init_expr
: // 空
| EQ expr
;

else_block
: // 空
| ELSE LCB statement_list RCB
| elif_list ELSE LCB statement_list RCB
;

elif_list
: ELIF expr LCB statement_list RCB
| elif_list ELIF expr LCB statement_list RCB
;

case_list
: // 空
| case_list CASE expr COLON statement_list
;

func_decl
: FUNCTION SYMBOL LP param_list RP COLON type LCB statement_list RCB
;

param_list
: // 空
| param_list1
;

param_list1
: param_decl
| param_list1 COMMA param_decl
;

param_decl
: SYMBOL COLON type
| SYMBOL COLON type EQ expr
;

expr_list
: // 空
| expr_list1
;

expr_list1
: expr
| expr_list COMMA expr
;

expr
: expr PLUS expr
| expr MINUS expr
| expr MULT expr
| expr DIV expr
| expr MOD expr
| expr LOGOR expr
| expr LOGAND expr
| LOGNOT expr
| expr BITAND expr
| expr BITOR expr
| BITNEG expr
| expr EQEQ expr
| expr NOTEQ expr
| expr LT expr
| expr GT expr
| expr LE expr
| expr GE expr
| PLUS expr %prec UOP
| MINUS expr %prec UOP
| LP expr RP
| SYMBOL
| SYMBOL LBK expr RBK
| SYMBOL LP expr_list RP
| INT_NUM
| FLOAT_NUM
| STRING
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
