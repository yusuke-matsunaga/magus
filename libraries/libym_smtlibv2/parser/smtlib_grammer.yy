%{ // C++ 宣言部

/// @file libym_smtlibv2/parser/smtlib_grammer.yy
/// @brief SmtLibV2のパーザ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: mislib_grammer.yy 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"
#include "ym_utils/FileRegion.h"
#include "SmtLibParser.h"
#include "SmtLibLex.h"


// より詳細なエラー情報を出力させる．
#define YYERROR_VERBOSE 1

// 解析結果の型
#define YYSTYPE SmtLibPt*

// 位置を表す型
// (yylloc の型)
#define YYLTYPE FileRegion

// YYLTYPE を書き換えたので以下のマクロも書き換えが必要
#define YYLLOC_DEFAULT(Current, Rhs, N) Current = loc_merge(Rhs, N);


BEGIN_NAMESPACE_YM_SMTLIBV2

// yacc/bison が生成したヘッダファイル
#include "smtlib_grammer.h"

// 字句解析関数の宣言
int
yylex(YYSTYPE*,
      YYLTYPE*,
      SmtLibParser&);

// エラー報告関数の宣言
int
yyerror(YYLTYPE*,
	SmtLibParser&,
	const char*);

BEGIN_NONAMESPACE

// loc_array 全体のファイル領域を求める．
inline
FileRegion
loc_merge(const FileRegion loc_array[],
	  ymuint n)
{
  if ( n == 0 ) {
    return FileRegion();
  }

  // 真の先頭を求める．
  ymuint i;
  for (i = 1; i <= n && !loc_array[i].is_valid(); ++ i) ;
  const FileRegion& first = loc_array[i];

  // 真の末尾を求める．
  ymuint j;
  for (j = n; i >= i && !loc_array[j].is_valid(); -- j) ;
  const FileRegion& last = loc_array[j];

  return FileRegion(first, last);
}

END_NONAMESPACE

%} // C++ 宣言部おわり


// "pure" parser にする．
%pure-parser

// 位置のトラッキングを行う．
%locations

// yyparse の引数
%parse-param {SmtLibParser& parser}

// yylex の引数
%lex-param {SmtLibParser& parser}

// トークンの定義
%token NUM
%token DEC
%token BIN
%token HEX
%token STRING
%token SYMBOL
%token KEYWORD

%token NL

%token ERROR

%start file


%%

//////////////////////////////////////////////////////////////////////
// YACC 本体定義
//////////////////////////////////////////////////////////////////////

// smtlib ファイルは S式の集まり
file
: s_expr
| file s_expr
;

// S式
s_expr
: spec_constant
| SYMBOL
| KEYWORD
| '(' list_of_s_expr ')'
;

list_of_s_expr
: // 空
| list_of_s_expr s_expr
;

spec_constant
: NUM
| DEC
| HEX
| BIN
| STRING
;


%%

//////////////////////////////////////////////////////////////////////
// 関連する C++ のコード
//////////////////////////////////////////////////////////////////////

// 字句解析器
// 実際には SmtLibLex が仕事をしている．
int
yylex(YYSTYPE* lvalp,
      YYLTYPE* llocp,
      SmtLibParser& parser)
{
  return parser.scan(*lvalp, *llocp);
}

// エラー出力関数
int
yyerror(YYLTYPE* llocp,
	SmtLibParser& parser,
	const char* msg)
{
  parser.error(*llocp, msg);
  return 1;
}

END_NAMESPACE_YM_SMTLIBV2
