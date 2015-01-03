%{

/// @file test.yy
/// @brief test の構文解析器
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

// C++ 用の定義

%}

// Yacc 用の定義

%left '-'
%left '*' '/'
%nonassoc UMINUS

%token id

%nonassoc '+'

%%

// 本体

expr : '(' expr ')'
     | expr '+' expr '+' expr
     | expr '-' expr
     | expr '*' expr
     | expr '/' expr
     | UMINUS expr %prec UMINUS
     | id
     ;

%%

// C++ の記述
