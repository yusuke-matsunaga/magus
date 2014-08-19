%{

/// @file gds_grammer.yy
/// @brief GDS-II のパーサ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "gds/Msg.h"
#include "gds/GdsParser.h"
#include "GdsRecTable.h"


// より詳細なエラー情報を出力させる．
#define YYERROR_VERBOSE 1


BEGIN_NAMESPACE_YM_GDS_PARSER

// yacc/bison が生成したヘッダファイル
#include "gds_grammer.hh"

// このファイルで定義されている関数
int
yylex(YYSTYPE* lvalp,
      GdsParser& parser);

int
yyerror(GdsParser& parser,
	const char* msg);

%}

// "pure" parser にする．
%define api.pure

// yyparse の引数
%parse-param {GdsParser& parser}

// yylex の引数
%lex-param {GdsParser& parser}

// トークンの型
%union {
  ymint16     int2_type;
  ymint32     int4_type;
  double      real_type;
  ymuint16    bitarray_type;
  ymuint32    colrow_type;

  GdsACL*     acl_type;
  GdsDate*    date_type;
  GdsStrans*  strans_type;
  GdsString*  string_type;
  GdsUnits*   units_type;
  GdsXY*      xy_type;
}

// トークンの定義
%token ERROR

%token <int2_type> HEADER
%token <date_type> BGNLIB
%token <string_type> LIBNAME
%token <units_type> UNITS
%token ENDLIB
%token <date_type> BGNSTR
%token <string_type> STRNAME
%token ENDSTR
%token BOUNDARY
%token PATH
%token SREF
%token AREF
%token TEXT
%token <int2_type> LAYER
%token <int2_type> DATATYPE
%token <int4_type> WIDTH
%token <xy_type> XY
%token ENDEL
%token <string_type> SNAME
%token <colrow_type> COLROW
%token TEXTNODE
%token NODE
%token <int2_type> TEXTTYPE
%token <bitarray_type> PRESENTATION
%token SPACING
%token <string_type> STRING
%token <bitarray_type> STRANS
%token <real_type> MAG
%token <real_type> ANGLE
%token UINTEGER
%token USTRING
%token <string_type> REFLIBS
%token <string_type> FONTS
%token <int2_type> PATHTYPE
%token <int2_type> GENERATIONS
%token <string_type> ATTRTABLE
%token <string_type> STYPTABLE
%token <int2_type> STRTYPE
%token <bitarray_type> ELFLAGS
%token <int4_type> ELKEY
%token <int2_type> LINKTYPE
%token <int4_type> LINKKEYS
%token <int2_type> NODETYPE
%token <int2_type> PROPATTR
%token <string_type> PROPVALUE
%token BOX
%token <int2_type> BOXTYPE
%token <int4_type> PLEX
%token <int4_type> BGNEXTN
%token <int4_type> ENDEXTN
%token <int2_type> TAPENUM
%token TAPECODE
%token <int2_type> STRCLASS
%token RESERVED
%token <int2_type> FORMAT
%token <string_type> MASK
%token ENDMASKS
%token <int2_type> LIBDIRSIZE
%token <string_type> SRFNAME
%token <acl_type> LIBSECUR
%token BORDER
%token SOFTFENCE
%token HARDFENCE
%token SOFTWIRE
%token HARDWIRE
%token PATHPORT
%token NODEPORT
%token USERCONSTRAINT
%token SPACER_ERROR
%token CONTACT


// 非終端ノードの型定義
%type <int2_type> opt_LIBDIRSIZE
%type <string_type> opt_SRFNAME
%type <acl_type> opt_LIBSECUR
%type <string_type> opt_REFLIBS
%type <string_type> opt_FONTS
%type <string_type> opt_ATTRTABLE
%type <int2_type> opt_GENERATIONS
%type <bitarray_type> opt_ELFLAGS
%type <int4_type> opt_PLEX
%type <int2_type> opt_PATHTYPE
%type <int4_type> opt_WIDTH
%type <int4_type> opt_BGNEXTN
%type <int4_type> opt_ENDEXTN
%type <bitarray_type> opt_PRESENTATION
%type <strans_type> opt_strans

%%

stream_format
: header star_structure ENDLIB
;

// ヘッダ部分
header
: HEADER BGNLIB opt_LIBDIRSIZE opt_SRFNAME opt_LIBSECUR
  LIBNAME opt_REFLIBS opt_FONTS opt_ATTRTABLE opt_GENERATIONS
  opt_FormatType UNITS
{
  parser.new_header($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $12);
}
;

opt_LIBDIRSIZE
: // null
{
  $$ = 0;
}
| LIBDIRSIZE
{
  $$ = $1;
}
;

opt_SRFNAME
: // null
{
  $$ = NULL;
}
| SRFNAME
{
  $$ = $1;
}
;

opt_LIBSECUR
: // null
{
  $$ = NULL;
}
| LIBSECUR
{
  $$ = $1;
}
;

opt_REFLIBS
: // null
{
  $$ = NULL;
}
| REFLIBS
{
  $$ = $1;
}
;

opt_FONTS
: // null
{
  $$ = NULL;
}
| FONTS
{
  $$ = $1;
}
;

opt_ATTRTABLE
: // null
{
  $$ = NULL;
}
| ATTRTABLE
{
  $$ = $1;
}
;

opt_GENERATIONS
: // null
{
  $$ = NULL;
}
| GENERATIONS
{
  $$ = $1;
}
;

opt_FormatType
: // null
| FORMAT
{
  parser.set_format($1);
}
| FORMAT plus_MASK ENDMASKS
{
  parser.set_format($1);
}
;

plus_MASK
: MASK
{
  parser.add_mask($1);
}
| plus_MASK MASK
{
  parser.add_mask($2);
}
;

star_structure
: // null
| star_structure structure
;

structure
: struct_head star_element ENDSTR
;

struct_head
: BGNSTR STRNAME
{
  parser.add_struct($1, $2);
}
| BGNSTR STRNAME STRCLASS
{
  parser.add_struct($1, $2);
}
;

star_element
: // null
| star_element element
;

element
: elem_header star_property ENDEL
;

elem_header
: boundary
| path
| sref
| aref
| text
| node
| box
;

boundary
: BOUNDARY opt_ELFLAGS opt_PLEX LAYER DATATYPE XY
{
  parser.add_boundary($2, $3, $4, $5, $6);
}
;

path
: PATH opt_ELFLAGS opt_PLEX LAYER DATATYPE
  opt_PATHTYPE opt_WIDTH opt_BGNEXTN opt_ENDEXTN XY
{
  parser.add_path($2, $3, $4, $5, $6, $7, $8, $9, $10);
}
;

sref
: SREF opt_ELFLAGS opt_PLEX SNAME opt_strans XY
{
  parser.add_sref($2, $3, $4, $5, $6);
}
;

aref
: AREF opt_ELFLAGS opt_PLEX SNAME opt_strans COLROW XY
{
  parser.add_aref($2, $3, $4, $5, $6, $7);
}
;

text
: TEXT opt_ELFLAGS opt_PLEX LAYER TEXTTYPE
  opt_PRESENTATION opt_PATHTYPE opt_WIDTH opt_strans
  XY STRING
{
  parser.add_text($2, $3, $4, $5, $6, $7, $8, $9, $10, $11);
}
;


node
: NODE opt_ELFLAGS opt_PLEX LAYER NODETYPE XY
{
  parser.add_node($2, $3, $4, $5, $6);
}
;

box
: BOX opt_ELFLAGS opt_PLEX LAYER BOXTYPE XY
{
  parser.add_box($2, $3, $4, $5, $6);
}
;

opt_ELFLAGS
: // null
{
  $$ = 0;
}
| ELFLAGS
{
  $$ = $1;
}
;

opt_PLEX
: // null
{
  $$ = 0;
}
| PLEX
{
  $$ = $1;
}
;

opt_PATHTYPE
: // null
{
  $$ = 0;
}
| PATHTYPE
{
  $$ = $1;
}
;

opt_WIDTH
: // null
{
  $$ = 0;
}
| WIDTH
{
  $$ = $1;
}
;

opt_BGNEXTN
: // null
{
  $$ = 0;
}
| BGNEXTN
{
  $$ = $1;
}
;

opt_ENDEXTN
: // null
{
  $$ = 0;
}
| ENDEXTN
{
  $$ = $1;
}
;

opt_strans
: // null
{
  $$ = NULL;
}
| STRANS
{
  $$ = parser.new_strans($1, 0.0, 0.0);
}
| STRANS     ANGLE
{
  $$ = parser.new_strans($1, 0.0, $2);
}
| STRANS MAG
{
  $$ = parser.new_strans($1, $2, 0.0);
}
| STRANS MAG ANGLE
{
  $$ = parser.new_strans($1, $2, $3);
}
;

opt_PRESENTATION
: // null
{
  $$ = NULL;
}
| PRESENTATION
{
  $$ = $1;
}
;

star_property
: // null
| star_property PROPATTR PROPVALUE
{
  parser.add_property($2, $3);
}
;

%%

// 一文字(ここでは 1 record)をとってくる関数
int
yylex(YYSTYPE* lvalp,
      GdsParser& parser)
{
  return parser.yylex(*lvalp);
}

// yacc パーサが内部で呼び出す関数
// エラーメッセージを出力する．
int
yyerror(GdsParser& parser,
	const char* s)
{
  return parser.yyerror(s);
}

END_NAMESPACE_YM_GDS_PARSER
