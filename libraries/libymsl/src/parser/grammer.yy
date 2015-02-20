%{

/// @file grammer.yy
/// @brief grammer の構文解析器
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

// C++ 用の定義

#include "ymsl_int.h"

#include "YmUtils/FileRegion.h"
#include "YmUtils/MsgMgr.h"

#include "AstMgr.h"
#include "AstList.h"


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
  AstCaseList*      caselist_type;
  AstEnumConstList* eclist_type;
  AstExpr*          expr_type;
  AstExprList*      exprlist_type;
  AstParam*         param_type;
  AstParamList*     paramlist_type;
  AstStatement*     statement_type;
  AstStmtList*      stmtlist_type;
  AstSymbol*        symbol_type;
  AstType*          type_type;
  OpCode            opcode_type;
}


// 終端記号(文字以外の記号)
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

// 終端記号(予約語)
%token ARRAY
%token AS
%token BOOLEAN
%token BREAK
%token CASE
%token CLASS
%token CONST
%token CONTINUE
%token DEFAULT
%token DO
%token ELIF
%token ELSE
%token ENUM
%token FALSE
%token FLOAT
%token FOR
%token FUNCTION
%token GOTO
%token IF
%token IMPORT
%token INT
%token MAP
%token RETURN
%token SET
%token STRING
%token SWITCH
%token TRUE
%token VAR
%token VOID
%token WHILE

// 値を持つ終端記号
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
%type <eclist_type>     enumconst_list
%type <expr_type>       expr
%type <expr_type>       init_expr
%type <expr_type>       primary
%type <exprlist_type>   expr_list
%type <opcode_type>     eqop
%type <param_type>      param
%type <paramlist_type>  param_list
%type <statement_type>  block_stmt
%type <statement_type>  complex_stmt
%type <statement_type>  import
%type <statement_type>  single_stmt
%type <statement_type>  statement
%type <statement_type>  else_stmt
%type <stmtlist_type>   module_head
%type <stmtlist_type>   statement_list
%type <type_type>       type

%%

// 本体

%start module;

module
: module_head statement_list
{
  mgr.set_root($1, $2, @$);
  delete $1;
  delete $2;
}
;

// モジュールの先頭要素
module_head
: // 空
{
  $$ = new AstStmtList;
}
| module_head import
{
  $$ = $1;
  $$->add($2);
}
;

// import 文
import
: IMPORT primary SEMI
{
  $$ = mgr.new_Import($2, NULL, @$);
}
| IMPORT primary AS SYMBOL SEMI
{
  $$ = mgr.new_Import($2, $4, @$);
}
;

// ステートメントのリスト
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

// ステートメント
statement
// 単一のステートメント
: single_stmt SEMI
{
  $$ = $1;
}
// 複合文
| complex_stmt
{
  $$ = $1;
}
// enum 定義
| ENUM SYMBOL LCB enumconst_list RCB
{
  $$ = mgr.new_EnumDecl($2, $4, @$);
  delete $4;
}
// 関数定義
| FUNCTION SYMBOL LP param_list RP COLON type block_stmt
{
  $$ = mgr.new_FuncDecl($2, $7, $4, $8, @$);
  delete $4;
}
// 変数定義
| VAR SYMBOL COLON type init_expr SEMI
{
  $$ = mgr.new_VarDecl($2, $4, $5, @$);
}
// 定数定義
| CONST SYMBOL COLON type EQ expr SEMI
{
  $$ = mgr.new_ConstDecl($2, $4, $6, @$);
}
// ラベル文
| SYMBOL COLON
{
  $$ = mgr.new_Label($1, @$);
}
// 空文
| SEMI
{
  $$ = mgr.new_NullStmt(@$);
}
// エラー回復用のルール
| error SEMI
{
  yyerrok;
}
;

single_stmt
// 代入文
: primary EQ expr
{
  $$ = mgr.new_Assignment($1, $3, @$);
}
// 演算付き代入文
| primary eqop expr
{
  $$ = mgr.new_InplaceOp($2, $1, $3, @$);
}
// 式文
| expr
{
  $$ = mgr.new_ExprStmt($1, @$);
}
// インクリメント文
| primary PLUSPLUS
{
  $$ = mgr.new_Incr($1, @$);
}
// デクリメント文
| primary MINUSMINUS
{
  $$ = mgr.new_Decr($1, @$);
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

enumconst_list
: SYMBOL
{
  AstEnumConst* ec = mgr.new_EnumConst($1, NULL, @$);
  $$ = new AstEnumConstList;
  $$->add(ec);
}
| SYMBOL EQ expr
{
  AstEnumConst* ec = mgr.new_EnumConst($1, $3, @$);
  $$ = new AstEnumConstList;
  $$->add(ec);
}
| enumconst_list COMMA SYMBOL
{
  AstEnumConst* ec = mgr.new_EnumConst($3, NULL, @3);
  $$ = $1;
  $$->add(ec);
}
| enumconst_list COMMA SYMBOL EQ expr
{
  AstEnumConst* ec = mgr.new_EnumConst($3, $5, FileRegion(@3, @5));
  $$ = $1;
  $$->add(ec);
}
;

// 代入文の演算子
// なんかバカみたいなアクション定義
eqop
: EQPLUS   { $$ = kOpAdd; }
| EQMINUS  { $$ = kOpSub; }
| EQMULT   { $$ = kOpMul; }
| EQDIV    { $$ = kOpDiv; }
| EQMOD    { $$ = kOpMod; }
| EQLSHIFT { $$ = kOpLshift; }
| EQRSHIFT { $$ = kOpRshift; }
| EQBITAND { $$ = kOpBitAnd; }
| EQBITOR  { $$ = kOpBitOr; }
| EQBITXOR { $$ = kOpBitXor; }
;

// 複合文
// というか末尾にセミコロンがこない文
complex_stmt
// IF 文
: IF expr block_stmt else_stmt
{
  $$ = mgr.new_If($2, $3, $4, @$);
}
// FOR 文
| FOR LP single_stmt SEMI expr SEMI single_stmt RP block_stmt
{
  $$ = mgr.new_For($3, $5, $7, $9, @$);
}
// WHILE 文
| WHILE LP expr RP block_stmt
{
  $$ = mgr.new_While($3, $5, @$);
}
// DO-WHILE 文
| DO block_stmt WHILE LP expr RP
{
  $$ = mgr.new_DoWhile($2, $5, @$);
}
// SWITCH 文
| SWITCH expr LCB case_list RCB
{
  $$ = mgr.new_Switch($2, $4, @$);
  delete $4;
}
// ブロック文
| block_stmt
{
  $$ = $1;
}
;

// ブロック文
block_stmt
: LCB statement_list RCB
{
  $$ = mgr.new_BlockStmt($2, @$);
  delete $2;
}
;

// elif/else 節のリスト
else_stmt
: // 空
{
  $$ = NULL;
}
| ELSE block_stmt
{
  $$ = $2;
}
| ELIF expr block_stmt else_stmt
{
  $$ = mgr.new_If($2, $3, $4, @$);
}
;

case_list
: // 空
{
  $$ = new AstCaseList;
}
| case_list CASE expr COLON block_stmt
{
  AstCaseItem* item = mgr.new_CaseItem($3, $5, FileRegion(@2, @5));
  $$ = $1;
  $$->add(item);
}
| case_list DEFAULT COLON block_stmt
{
  AstCaseItem* item = mgr.new_CaseItem(NULL, $4, FileRegion(@2, @4));
  $$ = $1;
  $$->add(item);
}
;

// 関数定義の引数リスト
param_list
: // 空
{
  $$ = new AstParamList;
}
| param
{
  $$ = new AstParamList;
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
  $$ = mgr.new_Param($1, $3, $4, @$);
}
;

// データ型
type
: VOID
{
  $$ = mgr.new_PrimType(kVoidType, @$);
}
| BOOLEAN
{
  $$ = mgr.new_PrimType(kBooleanType, @$);
}
| INT
{
  $$ = mgr.new_PrimType(kIntType, @$);
}
| FLOAT
{
  $$ = mgr.new_PrimType(kFloatType, @$);
}
| STRING
{
  $$ = mgr.new_PrimType(kStringType, @$);
}
| primary
{
  $$ = mgr.new_NamedType($1, @$);
}
| ARRAY LP type RP
{
  $$ = mgr.new_ArrayType($3, @$);
}
| SET LP type RP
{
  $$ = mgr.new_SetType($3, @$);
}
| MAP LP type COMMA type RP
{
  $$ = mgr.new_MapType($3, $5, @$);
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
// 定数
: TRUE
{
  $$ = mgr.new_TrueConst(@$);
}
| FALSE
{
  $$ = mgr.new_FalseConst(@$);
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
// プライマリ
| primary
{
  $$ = $1;
}
// 単項演算
| MINUS expr %prec UOP
{
  $$ = mgr.new_UniOp(kOpUniMinus, $2, @$);
}
| BITNEG expr
{
  $$ = mgr.new_UniOp(kOpBitNeg, $2, @$);
}
| LOGNOT expr
{
  $$ = mgr.new_UniOp(kOpLogNot, $2, @$);
}
| INT LP expr RP
{
  $$ = mgr.new_UniOp(kOpCastInt, $3, @$);
}
| BOOLEAN LP expr RP
{
  $$ = mgr.new_UniOp(kOpCastBoolean, $3, @$);
}
| FLOAT LP expr RP
{
  $$ = mgr.new_UniOp(kOpCastFloat, $3, @$);
}
// 二項演算
| expr PLUS expr
{
  $$ = mgr.new_BinOp(kOpAdd, $1, $3);
}
| expr MINUS expr
{
  $$ = mgr.new_BinOp(kOpSub, $1, $3);
}
| expr MULT expr
{
  $$ = mgr.new_BinOp(kOpMul, $1, $3);
}
| expr DIV expr
{
  $$ = mgr.new_BinOp(kOpDiv, $1, $3);
}
| expr MOD expr
{
  $$ = mgr.new_BinOp(kOpMod, $1, $3);
}
| expr LSHIFT expr
{
  $$ = mgr.new_BinOp(kOpLshift, $1, $3);
}
| expr RSHIFT expr
{
  $$ = mgr.new_BinOp(kOpRshift, $1, $3);
}
| expr BITAND expr
{
  $$ = mgr.new_BinOp(kOpBitAnd, $1, $3);
}
| expr BITOR expr
{
  $$ = mgr.new_BinOp(kOpBitOr, $1, $3);
}
| expr BITXOR expr
{
  $$ = mgr.new_BinOp(kOpBitXor, $1, $3);
}
| expr LOGAND expr
{
  $$ = mgr.new_BinOp(kOpLogAnd, $1, $3);
}
| expr LOGOR expr
{
  $$ = mgr.new_BinOp(kOpLogOr, $1, $3);
}
| expr EQEQ expr
{
  $$ = mgr.new_BinOp(kOpEqual, $1, $3);
}
| expr NOTEQ expr
{
  $$ = mgr.new_BinOp(kOpNotEq, $1, $3);
}
| expr LT expr
{
  $$ = mgr.new_BinOp(kOpLt, $1, $3);
}
| expr GT expr
{
  // LT にしてオペランドを入れ替える．
  $$ = mgr.new_BinOp(kOpLt, $3, $1);
}
| expr LE expr
{
  $$ = mgr.new_BinOp(kOpLe, $1, $3);
}
| expr GE expr
{
  // LE にしてオペランドを入れ替える．
  $$ = mgr.new_BinOp(kOpLe, $3, $1);
}
| expr QST expr COLON expr %prec ITE
{
  $$ = mgr.new_IteOp($1, $3, $5);
}
| LP expr RP
{
  $$ = $2;
}
// 関数呼び出し
| primary LP expr_list RP
{
  $$ = mgr.new_FuncCall($1, $3, @$);
  delete $3;
}
;

// プライマリ式
primary
: SYMBOL
{
  $$ = mgr.new_SymbolExpr($1);
}
// メンバ参照
| primary DOT SYMBOL
{
  $$ = mgr.new_MemberRef($1, $3, @$);
}
// 配列参照
| primary LBK expr RBK
{
  $$ = mgr.new_ArrayRef($1, $3, @$);
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
