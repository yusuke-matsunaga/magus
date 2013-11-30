%{

/// @file bb_grammer.yy
/// @brief bbの構文解析用ソース
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "bb_nsdef.h"
#include "ym_utils/FileRegion.h"
#include "Driver.h"


using namespace nsYm::nsBb;
using namespace std;


// より詳細なエラー情報を出力させる．
#define YYERROR_VERBOSE 1

// 位置を表す型
// (yylloc の型)
#define YYLTYPE FileRegion

// YYLTYPE を書き換えたので以下のマクロも書き換えが必要
#define YYLLOC_DEFAULT(Current, Rhs, N) Current = fr_merge(Rhs, N);


BEGIN_NAMESPACE_YM_BB

// yacc/bison が生成したヘッダファイル
#include "bb_grammer.hh"

class PtNode;

// 字句解析関数
int
yylex(YYSTYPE*,
      YYLTYPE*,
      Driver&);

// エラー報告関数
int
yyerror(YYLTYPE*,
	Driver&,
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
  for (j = n; i >= i && !fr_array[j].is_valid(); -- j) ;
  const FileRegion& last = fr_array[j];

  return FileRegion(first, last);
}

%}


// "pure" parser にする．
%define api.pure

// 位置のトラッキングを行う．
%locations

// yyparse の引数
%parse-param {Driver& driver}

// yylex の引数
%lex-param {Driver& driver}

// トークンの値の型
%union {
  int inttype;
  const char* strtype;
  PtNode* pttype;
}

// トークンの定義
%token <inttype> ID
%token <inttype> NUMBER
%token SEMI
%token LP
%token RP
%token LB
%token RB
%token COMMA
%token BIT_WIDTH
%token VAR
%token COLONEQ

%token ERROR;

%left BOR
%left BAND BXOR
%left EQ
%left LT LE GT GE
%left LTLT GTGT
%left ADD SUB
%left MUL DIV MOD
%nonassoc UMINUS TILDE

%type <pttype> term expr

%%

start
: bw_block var_block constr_block
;

bw_block
: BIT_WIDTH EQ NUMBER SEMI
{
  driver.set_bit_width($3);
}
;

var_block
: var_definition
| var_block var_definition
;

constr_block
: constr
| constr_block constr
;

var_definition
: VAR ID SEMI
{
  driver.new_var(@$, $2);
}
| VAR ID LB NUMBER COMMA NUMBER RB SEMI
{
  driver.new_var(@$, $2, $4, $6);
}
| VAR ID LB NUMBER COMMA NUMBER COMMA NUMBER RB SEMI
{
  driver.new_var(@$, $2, $4, $6, $8);
}
;

constr
: ID COLONEQ expr SEMI
{
  driver.new_assign(@$, $1, $3);
}
| expr EQ expr SEMI
{
  driver.new_eq(@$, $1, $3);
}
| expr LT expr SEMI
{
  driver.new_lt(@$, $1, $3);
}
| expr LE expr SEMI
{
  driver.new_le(@$, $1, $3);
}
| expr GT expr SEMI
{
  driver.new_gt(@$, $1, $3);
}
| expr GE expr SEMI
{
  driver.new_ge(@$, $1, $3);
}
;

expr
: term
{
  $$ = $1;
}
| LP expr RP
{
  $$ = $2;
}
| TILDE expr
{
  $$ = driver.new_neg(@$, $2);
}
| SUB expr %prec UMINUS
{
  $$ = driver.new_uminus(@$, $2);
}
| expr ADD expr
{
  $$ = driver.new_add(@$, $1, $3);
}
| expr SUB expr
{
  $$ = driver.new_sub(@$, $1, $3);
}
| expr MUL expr
{
  $$ = driver.new_mul(@$, $1, $3);
}
| expr DIV expr
{
  $$ = driver.new_div(@$, $1, $3);
}
| expr MOD expr
{
  $$ = driver.new_mod(@$, $1, $3);
}
| expr BAND expr
{
  $$ = driver.new_and(@$, $1, $3);
}
| expr BOR expr
{
  $$ = driver.new_or(@$, $1, $3);
}
| expr BXOR expr
{
  $$ = driver.new_xor(@$, $1, $3);
}
| expr LTLT expr
{
  $$ = driver.new_sll(@$, $1, $3);
}
| expr GTGT expr
{
  $$ = driver.new_srl(@$, $1, $3);
}
;

term
: ID
{
  $$ = driver.new_id(@$, $1);
}
| NUMBER
{
  $$ = driver.new_const(@$, $1);
}
;

//////////////////////////////////////////////////////////////////////
%%

// yacc パーサーが内部で呼び出す関数
// トークンを一つ読み込みトークン番号を返す．
// 同時に意味値(semantic value)を yylval にセットする．
// また位置を yylloc にセットする．
int
yylex(YYSTYPE* lvalp,
      YYLTYPE* llocp,
      Driver& driver)
{
  return driver.yylex(*lvalp, *llocp);
}

int
yyerror(YYLTYPE* llocp,
	Driver& driver,
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

  driver.put_msg(__FILE__, __LINE__,
		 *llocp,
		 kMsgError,
		 "PARS",
		 s2);

  return 1;
}

END_NAMESPACE_YM_BB
