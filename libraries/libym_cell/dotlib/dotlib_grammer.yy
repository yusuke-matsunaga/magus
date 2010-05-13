%{

/// @file libym_cell/dotlib/dotlib_grammer.yy
/// @brief .lib ファイル用パーザ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: dotlib_grammer.yy 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.

#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include <ym_cell/cell_nsdef.h>
#include <ym_utils/ShString.h>
#include <ym_utils/MsgHandler.h>

#include "DotLibParser.h"
#include "DotLibLex.h"
  

BEGIN_NAMESPACE_YM_CELL

// より詳細なエラー情報を出力させる．
#define YYERROR_VERBOSE 1

int
yylex();
 
int
yyerror(const char* msg);

// 字句解析器
DotLibLex* theLex = NULL;

#define YYSTYPE DotLibPt*

%}

// 位置のトラッキングを行う．
%locations

// トークンの定義
%token STR
%token NUM

// 予約語
%token LIBRARY
%token DELAY_MODEL
%token BUS_NAMING_STYLE
%token ROUTING_LAYERS
%token DATE
%token REVISION
%token COMMENT
%token TIME_UNIT
%token VOLTAGE_UNIT
%token CURRENT_UNIT
%token PULLING_RESISTANCE_UNIT
%token CAPACITIVE_LOAD_UNIT
%token LEAKAGE_POWER_UNIT
%token PIECE_TYPE
%token PIECE_DEFINE
%token DEFINE_CELL_AREA
%token IN_PLACE_SWAP_MODE
%token LIBRARY_FEATURES
%token SIMULATION
%token DEFAULT
%token OPERATING_CONDITIONS
%token TIMING_RANGE
%token WIRE_LOAD
%token WIRE_LOAD_SELECTION
%token POWER_LUT_TEMPLATE
%token CELL
%token SCALED_CELL
%token TYPE
%token INPUT_VOLTAGE
%token OUTPUT_VOLTAGE
%token GENERIC_CMOS
%token TABLE_LOOKUP
%token CMOS2
%token PIECEWISE_CMOS
%token DCM
%token TRUE
%token FALSE
%token MATCH_FOOTPRINT
%token NO_SWAPPING
%token CMOS
%token FPGA

%token ERROR


%start file

%%

// ファイルは1つの library グループから構成される．
file
: library_group
;

// library グループ
library_group
: LIBRARY '(' STR ')' '{' library_body '}'
;

// library 記述の本体
library_body
: library_attr
| library_body library_attr
;

// library グループ・レベル属性
library_attr
: TECHNOLOGY '(' technology ')' ';'
| DELAY_MODEL ':' delay_model ';'
| BUS_NAMING_STYLE ':' : STR ';'
| ROUTING_LAYERS '(' STR ')' ';'
| DATE ':' STR ';'
| REVISION ':' NUM ';'
| REVISION ':' STR ';'
| COMMENT ':' STR ';'
| TIME_UNIT ':' unit ';'
| VOLTAGE_UNIT ':' STR ';'
| CURRENT_UNIT ':' STR ';'
| PULLING_RESISTANCE_UNIT ':' STR ';'
| CAPACITIVE_LOAD_UNIT '(' NUM ',' STR ')' ';'
| LEAKAGE_POWER_UNIT ':' STR ';'
| PIECE_TYPE ':' piece_type ';'
| PIECE_DEFINE '(' STR ')' ';'
| DEFINE_CELL_AREA '(' STR ',' resource_type ')' ';'
| IN_PLACE_SWAP_MODE ':' swap_mode ';'
| LIBRARY_FEATURES '(' STR ')' ';'
| SIMULATION ':' boolean_value ';'
| DEFAULT values
| OPERATING_CONDITIONS '(' STR ')' '{' operating_conditions '}'
| TIMING_RANGE '(' STR ')' '{' timing_information '}'
| WIRE_LOAD '(' STR ')' '{' wire_load_information '}'
| WIRE_LOAD_SELECTION '(' ')' '{' area_group_selections '}'
| LU_TABLE_TEMPLATE '(' STR ')' '{' lut_template_description '}'
| POWER_LUT_TEMPLATE '(' STR ')' '{' lut_template_description '}'
| CELL '(' STR ')' '{' cell_description '}'
| SCALED_CELL '(' STR ',' STR ')' '{' cell_description '}'
| TYPE '(' STR ')' '{' bus_type_name '}'
| INPUT_VOLTAGE '(' STR ')' '{' input_voltage_description '}'
| OUTPUT_VOLTAGE '(' STR ')' '{' output_voltage_description '}'
| INPUT_THRESHOLD_PCT_FALL ':' NUM ';'
| INPUT_THRESHOLD_PCT_RISE ':' NUM ';'
| OUTPUT_THRESHOLD_PCT_FALL ':' NUM ';'
| OUTPUT_THRESHOLD_PCT_RISE ':' NUM ';'
| SLEW_DERATE_FROM_LIBRARY ':' NUM ';'
| SLEW_LOWER_THRESHOLD_PCT_FALL ':' NUM ';'
| SLEW_LOWER_THRESHOLD_PCT_RISE ':' NUM ';'
| SLEW_UPPER_THRESHOLD_PCT_FALL ':' NUM ';'
| SLEW_UPPER_THRESHOLD_PCT_RISE ':' NUM ';'
| PREFERRED_OUTPUT_PAD_SLEW_RATE_CONTROL ':' STR ';'
| PREFERRED_OUTPUT_PAD_VOLTAGE ':' STR ';'
| PREFERRED_INPUT_PAD_VOLTAGE ':' STR ';'
;

// テクノロジ
technology
: CMOS
| FPGA
;

// 遅延モデル
delay_model
: GENERIC_CMOS
| TABLE_LOOKUP
| CMOS2
| PIECEWISE_CMOS
| DCM
;

// ブール値
boolean_value
: TRUE
| FALSE
;

// 折れ線タイプ
piece_type
: PIECE_LENGTH
| PIECE_WIRE_CAP
| PIECE_PIN_CAP
| PIECE_TOTAL_CAP
;

// 消費電力計算用テンプレート
lut_template_description
: VARIABLE_1 ':' STR ';'
  INDEX_1 '(' STR ')' ';'
| VARIABLE_1 ':' STR ';' VARIABLE_2 ':' STR ';'
  INDEX_1 '(' STR ')' ';' INDEX_2 '(' STR ')' ';'
| VARIABLE_1 ':' STR ';' VARIABLE_2 ':' STR ';' VARIABLE_3 ':' STR ';'
  INDEX_1 '(' STR ')' ';' INDEX_2 '(' STR ')' ';' INDEX_3 '(' STR ')' ';'
;

// インプレースモード
swap_mode
: MATCH_FOOTPRINT
| NO_SWAPPING
;

// セルの情報
cell_description
: cell_attr
| cell_description cell_attr
;

// セルの属性
cell_attr
: AREA ':' NUM ';'
| CELL_FOOTPRINT ':' STR ';'
| CONTENTION_CONDITION ':' STR ';'
| DONT_FAULT ':' STR ';'
| DONT_TOUCH ':' boolean_value ';'
| DONT_USE ':' boolean_value ';'
| HANDLE_NEGATIVE_CONSTRAINT ':' boolean_value ';'
| IS_CLOCK_GATING_CELL ':' boolean_value ';'
| MAP_ONLY ':' boolean_value ';'
| PAD_CELL ':' boolean_value ';'
| AUXILIARY_PAD_CELL ':' boolean_value ';'
| PAD_TYPE ':' STR ';'
| PIN_EQUAL ':' '(' STR ')' ';'
| PIN_OPPOSITE ':' '(' STR ',' STR ')' ';'
| PREFERRED ':' boolean_value ';'
| SCALING_FACTORS ':' STR ';'
| SCAN_GROUP ':' STR ';'
| USE_FOR_SIZE_ONLY ':' boolean_value ';'
| VHDL_NAME ':' STR ';'
| MODE_DEFINITION '(' STR ')' '{' mode_values '}'
| ROUTING_TRACK '(' STR ')' '{'
    TRACKS ':' NUM ';'
    TOTAL_TRACK_AREA ':' NUM ';'
  '}'
| PIN '(' pin_list ')' '{' pingroup_description '}'
| BUS '(' STR ')' '{' bus_description '}'
| BUNDLE '(' STR ')' '{' bundle_description '}'
| SINGLE_BIT_DEGENERATE ':' STR ';'
;

mode_values
: mode_value
| mode_values mode_value
;

mode_value
: MODE_VALUE '(' STR ')' '{'
    WHEN ':' STR ';'
    SDF_COND ':' STR ';'
  '}'
;

pin_list
: STR
| pin_list ',' STR
;

pingroup_description
: pingroup_attr
| pingroup_description pingroup_attr
;

pingroup_attr
: CAPACITANCE ':' NUM ';'
| FALL_CAPACITANCE ':' NUM ';'
| RISE_CAPACITANCE ':' NUM ';'
| CLOCK_GATE_CLOCK_PIN ':' boolean_value ';'
| CLOCK_GATE_ENABLE_PIN ':' boolean_value ';'
| CLOCK_GATE_OBS_PIN ':' boolean_value ';'
| CLOCK_GATE_OUT_PIN ':' boolean_value ';'
| CLOCK_GATE_TEST_PIN ':' boolean_value ';'
| COMPLEMENTARY_PIN ':' STR ';'
| CONNECTION_CLASS ':' STR ';'
| DIRECTION ':' STR ';'
| DONT_FAULT ':' STR ';'
| DRIVER_TYPE ':' STR ';'
| FAULT_MODEL ':' STR ';'
| INVERTED_OUTPUT ':' boolean_value ';'
| PIN_FUNC_TYPE ':' STR ';'
| STEADY_STATE_RESISTANCE_FLOAT_MAX ':' NUM ';'
| STEADY_STATE_RESISTANCE_FLOAT_MIN ':' NUM ';'
| STEADY_STATE_RESISTANCE_HIGH_MAX ':' NUM ';'
| STEADY_STATE_RESISTANCE_HIGH_MIN ':' NUM ';'
| STEADY_STATE_RESISTANCE_LOW_MAX ':' NUM ';'
| STEADY_STATE_RESISTANCE_LOW_MIN ':' NUM ';'
| TEST_OUTPUT_ONLY ':' boolean_value ';'
| FANOUT_LOAD ':' NUM ';'
| MAX_FANOUT ':' NUM ';'
| MIN_FANOUT ':' NUM ';'
| MAX_TRANSITION ':' NUM ';'
| MIN_TRANSITION ':' NUM ';'
| MAX_CAPACITANCE ':' NUM ';'
| MIN_CAPACITANCE ':' NUM ';'
| CELL_DEGRADATION '(' STR ')' '{'
    index_1 '(' STR ')' ';'
    values '(' STR ')' ';'
  '}'
| CLOCK ':' boolean_value ';'
| MIN_PERIOD ':' NUM ';'
| MIN_PULSE_WIDTH_HIGH ':' NUM ';'
| MIN_PULSE_WIDTH_LOW ':' NUM ';'
| FUNCTION ':' STR ';'
| THREE_STATE ':' STR ';'
| X_FUNCTION ':' STR ';'
| STATE_FUNCTION ':' STR ';'
| INTERNAL_NODE ':' STR ';'
| PREFER_TIED ':' STR ';'
| IS_PAD ':' boolean_value ';'
| MULTICELL_PAD_PIN ':' boolean_value ';'
| INPUT_VOLTAGE ':' STR ';'
| PULLING_RESISTANCE ':' NUM ';'
| PULLING_CURRENT ':' NUM ';'
| HYSTERESIS ':' boolean_value ';'
| DRIVE_CURRENT ':' NUM ';'
| SLEW_CONTROL ':' STR ';'
| RISE_CURRENT_SLOPE_BEFORE_THRESHOLD ':' NUM ';'
| RISE_CURRENT_SLOPE_AFTER_THRESHOLD ':' NUM ';'
| FALL_CURRENT_SLOPE_BEFORE_THRESHOLD ':' NUM ';'
| FALL_CURRENT_SLOPE_AFTER_THRESHOLD ':' NUM ';'
| RISE_TIME_BEFORE_THRESHOLD ':' NUM ';'
| RISE_TIME_AFTER_THRESHOLD ':' NUM ';'
| FALL_TIME_BEFORE_THRESHOLD ':' NUM ';'
| FALL_TIME_AFTER_THRESHOLD ':' NUM ';'
| TIMING '(' timing_label ')' '{' timing_description '}'
;

bus_type_name
: BASE_TYPE ':' STR ';' // STR は array のみ
| DATA_TYPE ':' STR ';' // STR は bit のみ
| BIT_WIDTH ':' NUM ';'
| BIT_FROM ':' NUM ';'
| BIT_TO ':' NUM ';'
| DOWNTO ':' boolean_value ';'
;

bus_description
: bus_attr
| bus_description bus_attr
;

bus_attr
: BUS_TYPE ':' STR ';'
| pingroup_attr
| PIN '(' STR ')' '{' pingroup_description '}'
;

bundle_description
: bundle_attr
| bundle_description bundle_attr
;

bundle_attr
: MEMBERS '(' pin_list ')'
| pingroup_attr
;

timing_label
: /* 空 */
| timing_label ',' STR
;

timing_description
: timing_attr
| timing_description timing_attr
;

timing_attr
: RELATED_PIN ':' STR ';'
| RELATED_BUS_PIN ':' STR ';'
| TIMING_SENSE ':' STR ';'
| TIMING_TYPE ':' STR ';'
| MODE '(' STR ',' STR ')' ';'
| INTRINSIC_RISE ':' NUM ';'
| INTRINSIC_FALL ':' NUM ';'
| RISE_RESISTANCE ':' NUM ';'
| FALL_RESISTANCE ':' NUM ';'
| RISE_DELAY_INTERCEPT '(' NUM ',' STR ')' ';'
| FALL_DELAY_INTERCEPT '(' NUM ',' STR ')' ';'
| RISE_PIN_RESISTANCE '(' NUM ',' STR ')' ';'
| FALL_PIN_RESISTANCE '(' NUM ',' STR ')' ';'
| RISE_TRANSITION '(' STR ')' '{' values_attr '}'
| FALL_TRANSITION '(' STR ')' '{' values_attr '}'
| CELL_RISE '(' STR ')' '{' values_attr '}'
| CELL_FALL '(' STR ')' '{' values_attr '}'
| RISE_PROPAGATION '(' STR ')' '{' values_attr '}'
| FALL_PROPAGATION '(' STR ')' '{' values_attr '}'
| SLOPE_RISE ':' NUM ';'
| SLOPE_FALL ':' NUM ';'
| WHEN ':' STR ';'
| SDF_COND ':' STR ';'
| RISE_CONSTRAINT '(' STR ')' '{' values_attr '}'
| FALL_CONSTRAINT '(' STR ')' '{' values_attr '}'

values_attr
: VALUES '(' values ')' ';'
| INTERMEDIATE_VALUES '(' values ')' ';'
  VALUES '(' values ')' ';'
| VALUES '(' values ')' ';'
  INTERMEDIATE_VALUES '(' values ')' ';'
;

values
: STR
| values ',' STR
;

// ゲート
gate
: GATE STR NUM STR EQ expr ';' pin_list
{
  theMgr->new_gate(@$.first_line, @$.first_column,
		   @$.last_line, @$.last_column,
		   $2, $3, $4, $6, $8);
}
| GATE STR NUM STR EQ expr ';'
{
  theMgr->new_gate(@$.first_line, @$.first_column,
		   @$.last_line, @$.last_column,
		   $2, $3, $4, $6, NULL);
}
| GATE error
{
  yyclearin;
  yyerrok;
}
;

// 出力を表す論理式
expr
: LP expr RP
{
  $$ = $2;
}
| STR
{
  $$ = $1;
}
| NOT expr
{
  $$ = theMgr->new_not(@$.first_line, @$.first_column,
		       @$.last_line, @$.last_column,
		       $2);
} 
| expr STAR expr
{
  $$ = theMgr->new_and(@$.first_line, @$.first_column,
		       @$.last_line, @$.last_column,
		       $1, $3);
}
| expr PLUS expr
{
  $$ = theMgr->new_or(@$.first_line, @$.first_column,
		      @$.last_line, @$.last_column,
		      $1, $3);
}
| CONST0
{
  $$ = $1;
}
| CONST1
{
  $$ = $1;
}
;

// ピンリスト
pin_list
: pin
{
  $$ = theMgr->new_pinlist();
  $$->push_back($1);
}
| pin_list pin
{
  ($$ = $1)->push_back($2);
}
;

// 入力ピン
pin
: PIN STR phase NUM NUM NUM NUM NUM NUM
{
  $$ = theMgr->new_pin(@$.first_line, @$.first_column,
		       @$.last_line, @$.last_column,
		       $2, $3, $4, $5, $6, $7, $8, $9);
}
| PIN STAR phase NUM NUM NUM NUM NUM NUM
{
  // STAR(*) はワイルドカード
  $$ = theMgr->new_pin(@$.first_line, @$.first_column,
		       @$.last_line, @$.last_column,
		       NULL, $3, $4, $5, $6, $7, $8, $9);
}
;

// 極性情報
phase
: NONINV
{
  $$ = $1;
}
| INV
{
  $$ = $1;
}
| UNKNOWN
{
  $$ = $1;
}
;

%%

// テクノロジ・ライブラリを読み込む
// 読み込みが成功した場合は true を返す。
bool
DotLibPtMgr::read(const string& filename)
{
  DotLibLex lex;
  if ( !lex.open_file(filename) ) {
    fail_header(__FILE__, __LINE__, "PARS", 0, 0, 0, 0)
      << filename << " : cannot open";
    msg_end();
  }
  else {
    theMgr = this;
    theLex = &lex;
    yyparse();
    theMgr = NULL;
    theLex = NULL;
  }
  return MsgMgr::the_mgr().error_num() == 0;
}

// エラー出力関数
int
yyerror(const char* msg)
{
  string s2;
  // 好みの問題だけど "parse error" よりは "syntax error" の方が好き．
  if ( !strncmp(msg, "parse error", 11) ) {
    s2 ="syntax error";
    s2 += (msg + 11);
  }
  else {
    s2 = msg;
  }

  error_header(__FILE__, __LINE__, "MISLIB_PARSE",
	       yylloc.first_line, yylloc.first_column,
	       yylloc.last_line, yylloc.last_column,
	       s2);
  msg_end();

  return 1;
}

// 字句解析器
// 実際には Lex が仕事をしている．
int
yylex()
{
  int tok = theLex->read_token();
  yylloc.first_line = yylloc.last_line = theLex->cur_line();
  yylloc.first_column = theLex->first_column();
  yylloc.last_column = theLex->cur_column();
  
  switch ( tok ) {
  case STR:
    yylval = theMgr->new_str(yylloc.first_line, yylloc.first_column,
			     yylloc.last_line, yylloc.last_column,
			     ShString(theLex->cur_string()).id());
    break;
    
  case NUM:
    yylval = theMgr->new_num(yylloc.first_line, yylloc.first_column,
			     yylloc.last_line, yylloc.last_column,
			     strtod(theLex->cur_string(),
				    static_cast<char**>(NULL)));
    break;
    
  case NONINV:
    yylval = theMgr->new_noninv(yylloc.first_line, yylloc.first_column,
				yylloc.last_line, yylloc.last_column);
    break;
    
  case INV:
    yylval = theMgr->new_inv(yylloc.first_line, yylloc.first_column,
			     yylloc.last_line, yylloc.last_column);
    break;
    
  case UNKNOWN:
    yylval = theMgr->new_unknown(yylloc.first_line, yylloc.first_column,
				 yylloc.last_line, yylloc.last_column);
    break;
    
  case CONST0:
    yylval = theMgr->new_const0(yylloc.first_line, yylloc.first_column,
				yylloc.last_line, yylloc.last_column);
    break;
    
  case CONST1:
    yylval = theMgr->new_const1(yylloc.first_line, yylloc.first_column,
				yylloc.last_line, yylloc.last_column);
    break;
    
  }
  return tok;
}

END_NAMESPACE_YM_CELL
