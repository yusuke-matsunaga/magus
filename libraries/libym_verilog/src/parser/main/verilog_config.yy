%{

/// @file libym_verilog/parser/verilog_parser.yy
/// @brief Verilog-HDL (IEEE1364-2001) パーサ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: verilog_grammer.yy 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/FileRegion.h"
#include "Parser.h"


// より詳細なエラー情報を出力させる．
#define YYERROR_VERBOSE 1

// 位置を表す型
// (yylloc の型)
#define YYLTYPE FileRegion

// YYLTYPE を書き換えたので以下のマクロも書き換えが必要
#define YYLLOC_DEFAULT(Current, Rhs, N) Current = fr_merge(Rhs, N);


BEGIN_NAMESPACE_YM_VERILOG

// yacc/bison が生成したヘッダファイル
#include "verilog_lib.h"


// 字句解析関数
int
yylex(YYSTYPE*,
      YYLTYPE*,
      Parser&);

// エラー報告関数
int
yyerror(YYLTYPE*,
	Parser&,
	const char*);


// fr_array 全体のファイル領域を求める．
// 直感的には FileRegion(fr_array[1], fr_array[n])
// だが(先頭が 0 でなく 1 であることに注意),
// 場合によっては空のトークンで位置がないばあいがあるので
// それをスキップしなければならない．
inline
FileRegion
fr_merge(const FileRegion fr_array[],
	 size_t n)
{
  if ( n == 0 ) {
    // なんでこんなことがあるのか不明
    return FileRegion();
  }

  // 真の先頭を求める．
  size_t i;
  for (i = 1; i <= n && !fr_array[i].is_valid(); ++ i) ;
  const FileRegion& first = fr_array[i];

  // 真の末尾を求める．
  size_t j;
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
%parse-param {Parser& parser}

// yylex の引数
%lex-param {Parser& parser}


// トークンの型
%union {
  int inttype;
  double dbltype;
  unsigned long uinttype;
  const char* strtype;

  tVpiVarType vartype;
  tVpiNetType nettype;
  tVpiPrimType primtype;
  tVpiOpType optype;
  tVpiConstType consttype;
  tVpiStrength strengthtype;
  tVpiRangeMode rangemode;
  tVpiVsType vstype;
  tVpiUdpVal udpsymbol;

  PuHierName* hiername;

  PtiIOHead* iohead;
  PtiDeclHead* declhead;
  PtItem* item;

  PtGenCaseItem* gencaseitem;
  PtPathDecl* pathdecl;
  PtPathDelay* pathdelay;

  PtStmt* stmt;
  PtCaseItem* caseitem;

  PtExpr* expr;

  PtStrength* strength;
  PtDelay* delay;
  PtControl* control;
  PtConnection* connection;

  PtAttrInst* attrinst;
  PtAttrSpec* attrspec;

  PtrList<PtGenCaseItem>* gcilist;
  PtrList<PtStmt>* stmtlist;
  PtrList<PtCaseItem>* cilist;
  PtrList<PtRange>* rangelist;
  PtrList<PtExpr>* exprlist;
  PtrList<PtConnection>* conlist;
  PtrList<PtAttrInst>* ailist;
  PtrList<PtAttrSpec>* aslist;
}

// Lex 内部のみで用いられるトークン
%token ERROR                // エラーを表す．
%token SPACE                // ' ', '\t', eof
%token NL                   // '\r', '\n' or "\r\n"
%token CD_SYMBOL            // `ではじまる文字列
%token COMMENT1             // "//" 〜 改行
%token COMMENT2             // "/*" 〜 "*/"

// 値を持つ終端記号
%token <strtype> IDENTIFIER          // 識別子
%token <strtype> SYS_IDENTIFIER      // '$'で始まる文字列
%token <strtype> STRING              // 文字列
%token <strtype> UNUMBER             // unsigned number (in string)
%token <strtype> UNUM_BIG            // big unsigned number (in string)
%token <uinttype> UNUM_INT           // unsigned number
%token <dbltype> RNUMBER             // real number

// 値を持たない終端記号
%token BASE_B               // 'b
%token BASE_SB              // 'sb
%token BASE_O               // 'o
%token BASE_SO              // 'so
%token BASE_D               // 'd
%token BASE_SD              // 'sd
%token BASE_H               // 'h
%token BASE_SH              // 'sh
%token TILDEAND             // ~&
%token TILDEOR              // ~|
%token TILDEXOR             // ~^
%token LTLT                 // <<
%token LTLTLT               // <<<
%token GTGT                 // >>
%token GTGTGT               // >>>
%token LTEQ                 // <=
%token GTEQ                 // >=
%token EQEQ                 // ==
%token NOTEQ                // !=
%token EQEQEQ               // ===
%token NOTEQEQ              // !==
%token ANDAND               // &&
%token ANDANDAND            // &&&
%token OROR                 // ||
%token STARSTAR             // **
%token EQGT                 // =>
%token STARGT               // *>
%token MINUSGT              // ->
%token PRSTAR               // (*
%token STARPR               // *)
%token PLUSCOLON            // +:
%token MINUSCOLON           // -:
%token ';'
%token ':'
%token '['
%token ']'
%token '{'
%token '}'
%token '('
%token ')'
%token ','
%token '.'
%token '@'
%token '#'
%token '%'
%token '?'
%token '+'
%token '-'
%token '*'
%token '/'
%token '^'
%token '!'
%token '&'
%token '|'
%token '~'
%token '<'
%token '>'
%token '='
%token '$'
%token '0'
%token '1'
%token 'x'
%token 'b'
%token 'r'
%token 'f'
%token 'n'
%token 'p'

// 予約語
%token ALWAYS               // "always"
%token AND                  // "and"
%token ASSIGN               // "assign"
%token AUTOMATIC            // "automatic"
%token BEGIN                // "begin"
%token BUF                  // "buf"
%token BUFIF0               // "bufif0"
%token BUFIF1               // "bufif1"
%token CASE                 // "case"
%token CASEX                // "casex"
%token CASEZ                // "casez"
%token CELL                 // "cell"
%token CMOS                 // "cmos"
%token CONFIG               // "config"
%token DEASSIGN             // "deassign"
%token DEFAULT              // "default"
%token DEFPARAM             // "defparam"
%token DESIGN               // "design"
%token DISABLE              // "disable"
%token EDGE                 // "edge"
%token ELSE                 // "else"
%token END                  // "end"
%token ENDCASE              // "endcase"
%token ENDCONFIG            // "endconfig"
%token ENDFUNCTION          // "endfunction"
%token ENDGENERATE          // "endgenerate"
%token ENDMODULE            // "endmodule"
%token ENDPRIMITIVE         // "endprimitive"
%token ENDSPECIFY           // "endspecify"
%token ENDTABLE             // "endtable"
%token ENDTASK              // "endtask"
%token EVENT                // "event"
%token FOR                  // "for"
%token FORCE                // "force"
%token FOREVER              // "forever"
%token FORK                 // "fork"
%token FUNCTION             // "function"
%token GENERATE             // "generate"
%token GENVAR               // "genvar"
%token HIGHZ0               // "highz0"
%token HIGHZ1               // "highz1"
%token IF                   // "if"
%token INITIAL              // "initial"
%token IFNONE               // "ifnone"
%token INCDIR               // "incdir"
%token INCLUDE              // "include"
%token INOUT                // "inout"
%token INPUT                // "input"
%token INSTANCE             // "instance"
%token INTEGER              // "integer"
%token JOIN                 // "join"
%token LARGE                // "large"
%token LIBLIST              // "liblist"
%token LIBRARY              // "library"
%token LOCALPARAM           // "localparam"
%token MACROMODULE          // "macromodule"
%token MEDIUM               // "medium"
%token MODULE               // "module"
%token NAND                 // "nand"
%token NEGEDGE              // "negedge"
%token NMOS                 // "nmos"
%token NONE                 // "none" `default_nettype のみで用いる．
%token NOR                  // "nor"
%token NOSHOWCANCELLED      // "noshowcancelled"
%token NOT                  // "not"
%token NOTIF0               // "notif0"
%token NOTIF1               // "notif1"
%token OR                   // "or"
%token OUTPUT               // "output"
%token PARAMETER            // "parameter"
%token PMOS                 // "pmos"
%token POSEDGE              // "posedge"
%token PRIMITIVE            // "primitive"
%token PULL0                // "pull0"
%token PULL1                // "pull1"
%token PULLUP               // "pullup"
%token PULLDOWN             // "pulldown"
%token PULSESTYLE_ONEVENT   // "pulsestyle_onevent"
%token PULSESTYLE_ONDETECT  // "pulsestyle_ondetect"
%token RCMOS                // "rcmos"
%token REAL                 // "real"
%token REALTIME             // "realtime"
%token REG                  // "reg"
%token RELEASE              // "release"
%token REPEAT               // "repeat"
%token RNMOS                // "rnmos"
%token RPMOS                // "rpmos"
%token RTRAN                // "rtran"
%token RTRANIF0             // "rtranif0"
%token RTRANIF1             // "rtranif1"
%token SCALARED             // "scalared"
%token SHOWCANCELLED        // "showcancelled"
%token SIGNED               // "signed"
%token SMALL                // "small"
%token SPECIFY              // "specify"
%token SPECPARAM            // "specparam"
%token STRONG0              // "strong0"
%token STRONG1              // "strong1"
%token SUPPLY0              // "supply0"
%token SUPPLY1              // "supply1"
%token TABLE                // "table"
%token TASK                 // "task"
%token TIME                 // "time"
%token TRAN                 // "tran"
%token TRANIF0              // "tranif0"
%token TRANIF1              // "tranif1"
%token TRI                  // "tri"
%token TRI0                 // "tri0"
%token TRI1                 // "tri1"
%token TRIAND               // "triand"
%token TRIOR                // "trior"
%token TRIREG               // "trireg"
%token UNSIGNED             // "unsigned"
%token USE                  // "use"
%token VECTORED             // "vectored"
%token WAIT                 // "wait"
%token WAND                 // "wand"
%token WEAK0                // "weak0"
%token WEAK1                // "weak1"
%token WHILE                // "while"
%token WIRE                 // "wire"
%token WOR                  // "wor"
%token XOR                  // "xor"
%token XNOR                 // "xnor"

%token SETUP                // "$setup"
%token HOLD                 // "$hold"
%token SETUPHOLD            // "$setuphold"
%token RECOVERY             // "$recovery"
%token REMOVAL              // "$removal"
%token RECREM               // "$recrem"
%token SKEW                 // "$skew"
%token TIMESKEW             // "$timeskew"
%token FULLSKEW             // "$fullskew"
%token PERIOD               // "$period"
%token WIDTH                // "$width"
%token NOCHANGE             // "$nochange"

%token PATHPULSE            // "PATHPULSE$"


// 非終端ノードの型定義

%%

//////////////////////////////////////////////////////////////////////
// A.1.2 Configuration source text
//////////////////////////////////////////////////////////////////////
config_declaration
: CONFIG NAME ';'
design_statement
'{' config_rule_statement '}'
ENDCONFIG
;

design_statement
: DESIGN '{' cell_descriptor '}' ';'
;

config_rule_statement
: DEFAULT liblist_clause
| inst_clause liblist_clause
| inst_clause use_cluase
| cell_clause liblist_clause
| cell_clause use_clause
;

inst_clause
: INSTANCE inst_name
;

inst_name
: NAME
| inst_name '.' NAME
;

cell_clause
: CELL cell_descriptor
;

liblist_clause
: LIBLIST library_list
;

library_list
: // 空もありうる
| library_list NAME
;

use_clause
: USE cell_descriptor
| USE cell_descriptor ':' config
;

cell_descriptor
: NAME
| NAME '.' NAME
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
      Parser& parser)
{
  return parser.yylex(*lvalp, *llocp);
}

// yacc パーサーが内部で呼び出す関数
// エラーメッセージを出力する．
int
yyerror(YYLTYPE* llocp,
	Parser& parser,
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

  parser.put_msg(__FILE__, __LINE__,
		 *llocp,
		 kMsgError,
		 "PARS",
		 s2);

  return 1;
}

END_NAMESPACE_YM_VERILOG
