%{

/// @file bddsh_parser.yy
/// @brief Bdd のテスト用shellの構文解析用ソース
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "logic/Bdd.h"
#include "logic/BddMgr.h"
#include "logic/BddVarSet.h"
#include "logic/BddLitSet.h"

using namespace std;

extern
int yylex();
int yyerror(char*);

extern string cur_text;
extern int cur_num;


BEGIN_NAMESPACE_YM_BDD

#define YYSTYPE Bdd

hash_map<string, Bdd> id_table;
string lname;

BddMgr mgr("bmc", "Bdd Manager");
bool interactive = false;

void prompt();

%}


// トークンの定義
%token NUMBER
%token TRUE
%token FALSE
%token ID
%token SEMI
%token EQ
%token LP
%token RP
%token LB
%token RB
%token QMARK
%token COLON
%token ID
%token ITE_OP
%token ESM_OP
%token ASM_OP
%token COMMA
%token END

%left COND
%left OROR
%left ANDAND
%left BOR
%left BAND BXOR
%left EQEQ NOTEQ
%left LT LTEQ GT GTEQ
%left ADD SUB
%left MUL DIV
%nonassoc UMINUS TILDE NOT

%%

s
: list
;

list
:
| list line
;

line
: expr SEMI
{
  $1.print(cout);
  prompt();
}
| ID { lname = cur_text; } EQ expr SEMI
{
  id_table[lname] = $4;
  $4.print(cout);
  cout << " is stored in " << lname << endl;
  prompt();
}
| END
{
  cout << "bye-bye" << endl;
  YYACCEPT;
}
| error SEMI
{
  yyerrok;
  prompt();
}
;

expr
: TRUE
{
  $$ = mgr.make_one();
}
| FALSE
{
  $$ = mgr.make_zero();
}
| NUMBER
{
  VarId id(cur_num);
  $$ = mgr.make_posiliteral(id);
}
| expr BAND expr
{
  $$ = $1 & $3;
}
| expr BOR expr
{
  $$ = $1 | $3;
}
| expr BXOR expr
{
  $$ = $1 ^ $3;
}
| TILDE expr
{
  $$ = ~$2;
}
| LP expr RP
{
  $$ = $2;
}
| ITE_OP expr QMARK expr COLON expr COLON
{
  $$ = ite_op($2, $4, $6);
}
| variable
{
  $$ = $1;
}
;

pred
: expr EQEQ expr
{
  if ( $1 == $3 ) {
    $$ = mgr.make_one();
  }
  else {
    $$ = mgr.make_zero();
  }
}
| expr NOTEQ expr
{
  if ( $1 != $3 ) {
    $$ = mgr.make_one();
  }
  else {
    $$ = mgr.make_zero();
  }
}
| expr LT expr
{
  if ( $1 < $3 ) {
    $$ = mgr.make_one();
  }
  else {
    $$ = mgr.make_zero();
  }
}
| expr GT expr
{
  if ( $1 > $3 ) {
    $$ = mgr.make_one();
  }
  else {
    $$ = mgr.make_zero();
  }
}
| expr LTEQ expr
{
  if ( $1 <= $3 ) {
    $$ = mgr.make_one();
  }
  else {
    $$ = mgr.make_zero();
  }
}
| expr GTEQ expr
{
  if ( $1 >= $3 ) {
    $$ = mgr.make_one();
  }
  else {
    $$ = mgr.make_zero();
  }
}
| expr ANDAND expr
{
  if ( $1 && $3 ) {
    $$ = mgr.make_one();
  }
  else {
    $$ = mgr.make_zero();
  }
}
;

variable
: ID
{
  hash_map<string, Bdd>::iterator p = id_table.find(cur_text);
  if ( p == id_table.end() ) {
    cerr << cur_text << " : not defined" << endl;
    YYERROR;
  }
  $$ = p->second;
}
;

%%

void
prompt()
{
  if ( interactive ) {
    cout << "$ " << flush;
  }
}

END_NAMESPACE_YM_BDD

int
yyerror(char* s)
{
  using namespace std;

  cerr << s << endl;
  return 1;
}


int
main(int argc,
     char** argv)
{
  using namespace nsYm::nsBdd;

  extern FILE* yyin;
  if ( argc > 1 ) {
    yyin = fopen(argv[1], "r");
  }
  else {
    interactive = true;
    yyin = stdin;
  }
  prompt();
  yyparse();
}
