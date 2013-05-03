
/// @file dump.cc
/// @brief CellLibrary の内容をダンプする関数
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellLibrary.h"
#include "ym_cell/Cell.h"
#include "ym_cell/CellLut.h"
#include "ym_cell/CellPin.h"
#include "ym_cell/CellTiming.h"
#include "ym_cell/CellClass.h"
#include "ym_cell/CellGroup.h"
#include "ym_cell/CellPatGraph.h"
#include "ym_cell/CellCapacitance.h"
#include "ym_cell/CellTime.h"

#include "ym_logic/LogExpr.h"
#include "ym_logic/NpnMapM.h"


BEGIN_NAMESPACE_YM_CELL

// @brief ストリーム出力演算子
// @param[in] s 出力先のストリーム
// @param[in] delay_mode 遅延モード
// @return s を返す．
ostream&
operator<<(ostream& s,
	   tCellDelayModel delay_model)
{
  switch ( delay_model ) {
  case kCellDelayGenericCmos:   s << "generic_cmos"; break;
  case kCellDelayTableLookup:   s << "table_lookup"; break;
  case kCellDelayPiecewiseCmos: s << "piecewise_cmos"; break;
  case kCellDelayCmos2:         s << "cmos2"; break;
  case kCellDelayDcm:           s << "dcm"; break;
  default: assert_not_reached(__FILE__, __LINE__);
  }
  return s;
}

// @brief ストリーム出力演算子
// @param[in] s 出力先のストリーム
// @param[in] timing_sense タイミングセンス
// @return s を返す．
ostream&
operator<<(ostream& s,
	   tCellTimingSense timing_sense)
{
  switch ( timing_sense ) {
  case kCellPosiUnate: s << "posi_unate"; break;
  case kCellNegaUnate: s << "nega_unate"; break;
  case kCellNonUnate:  s << "non_unate"; break;
  default: assert_not_reached(__FILE__, __LINE__);
  }
  return s;
}

// @brief ストリーム出力演算子
// @param[in] s 出力先のストリーム
// @param[in] timing_type タイミング条件
// @return s を返す．
ostream&
operator<<(ostream& s,
	   tCellTimingType timing_type)
{
  switch ( timing_type ) {
  case kCellTimingCombinational:         s << "combinational"; break;
  case kCellTimingCombinationalRise:     s << "combinational_rise"; break;
  case kCellTimingCombinationalFall:     s << "combinational_fall"; break;

  case kCellTimingThreeStateEnable:      s << "three_state_enable"; break;
  case kCellTimingThreeStateDisable:     s << "three_state_disable"; break;
  case kCellTimingThreeStateEnableRise:  s << "three_state_enable_rise"; break;
  case kCellTimingThreeStateEnableFall:  s << "three_state_enable_fall"; break;
  case kCellTimingThreeStateDisableRise: s << "three_state_disable_rise"; break;
  case kCellTimingThreeStateDisableFall: s << "three_state_disable_fall"; break;

  case kCellTimingRisingEdge:            s << "rising_edge"; break;
  case kCellTimingFallingEdge:           s << "falling_edge"; break;

  case kCellTimingPreset:                s << "preset"; break;
  case kCellTimingClear:                 s << "clear"; break;

  case kCellTimingHoldRising:            s << "hold_rising"; break;
  case kCellTimingHoldFalling:           s << "hold_falling"; break;

  case kCellTimingSetupRising:           s << "setup_rising"; break;
  case kCellTimingSetupFalling:          s << "setup_falling"; break;

  case kCellTimingRecoveryRising:        s << "recover_rising"; break;
  case kCellTimingRecoveryFalling:       s << "recover_falling"; break;

  case kCellTimingSkewRising:            s << "skew_rising"; break;
  case kCellTimingSkewFalling:           s << "skew_falling"; break;

  case kCellTimingRemovalRising:         s << "removal_rising"; break;
  case kCellTimingRemovalFalling:        s << "removal_falling"; break;

  case kCellTimingNonSeqSetupRising:     s << "non_seq_setup_rising"; break;
  case kCellTimingNonSeqSetupFalling:    s << "non_seq_setup_falling"; break;
  case kCellTimingNonSeqHoldRising:      s << "non_seq_hold_rising"; break;
  case kCellTimingNonSeqHoldFalling:     s << "non_seq_hold_falling"; break;

  case kCellTimingNochangeHighHigh:      s << "nochange_high_high"; break;
  case kCellTimingNochangeHighLow:       s << "nochange_high_low"; break;
  case kCellTimingNochangeLowHigh:       s << "nochange_low_high"; break;
  case kCellTimingNochangeLowLow:        s << "nochange_low_low"; break;

  default: assert_not_reached(__FILE__, __LINE__);
  }
  return s;
}

// @brief ストリーム出力演算子
// @param[in] s 出力先のストリーム
// @param[in] var_type 変数の型
// @return s を返す．
ostream&
operator<<(ostream& s,
	   tCellVarType var_type)
{
  switch ( var_type ) {
  case kVarInputNetTransition: s << "input_net_transition"; break;
  case kVarTotalOutputNetCapacitance: s << "total_output_net_capacitance"; break;
  case kVarOutputNetLength: s << "output_net_length"; break;
  case kVarOutputNetWireCap: s << "output_net_wire_cap"; break;
  case kVarOutputNetPinCap: s << "output_net_pin_cap"; break;
  case kVarRelatedOutTotalOutputNetCapacitance: s << "related_out_total_output_net_capacitance"; break;
  case kVarRelatedOutOutputNetLength: s << "related_out_output_net_length"; break;
  case kVarRelatedOutOutputNetWireCap: s << "related_out_output_net_wire_cap"; break;
  case kVarRelatedOutOutputNetPinCap: s << "related_out_output_net_pin_cap"; break;
  case kVarConstrainedPinTransition: s << "constrained_pin_transition"; break;
  case kVarRelatedPinTransition: s << "related_pin_transition"; break;
  case kVarNone: s << "none"; break;

  default: assert_not_reached(__FILE__, __LINE__);
  }
  return s;
}

BEGIN_NONAMESPACE

// LUT の情報を出力する．
void
display_lut(ostream& s,
	    const char* label,
	    const CellLut* lut)
{
  s << "    " << label << endl;
  ymuint d = lut->dimension();
  for (ymuint i = 0; i < d; ++ i) {
    s << "      Variable_" << (i + 1) << " = " << lut->variable_type(i) << endl;
  }
  for (ymuint i = 0; i < d; ++ i) {
    s << "      Index_" << (i + 1) << "    = ";
    ymuint n = lut->index_num(i);
    const char* comma = "";
    s << "(";
    for (ymuint j = 0; j < n; ++ j) {
      s << comma << lut->index(i, j);
      comma = ", ";
    }
    s << ")" << endl;
  }

  if ( d == 1) {
    s << "      Values = (";
    ymuint n1 = lut->index_num(0);
    vector<ymuint32> pos_array(1);
    const char* comma = "";
    for (ymuint i1 = 0; i1 < n1; ++ i1) {
      pos_array[0] = i1;
      s << comma << lut->grid_value(pos_array);
      comma = ", ";
    }
    s << ")" << endl;
  }
  else if ( d == 2 ) {
    s << "      Values = (" << endl;
    ymuint n1 = lut->index_num(0);
    ymuint n2 = lut->index_num(1);
    vector<ymuint32> pos_array(2);
    for (ymuint i1 = 0; i1 < n1; ++ i1) {
      s << "                (";
      pos_array[0] = i1;
      const char* comma = "";
      for (ymuint i2 = 0; i2 < n2; ++ i2) {
	pos_array[1] = i2;
	s << comma << lut->grid_value(pos_array);
	comma = ", ";
      }
      s << ")" << endl;
    }
    s << "               )" << endl;
  }
  else if ( d == 3 ) {
    s << "      Values = (" << endl;
    ymuint n1 = lut->index_num(0);
    ymuint n2 = lut->index_num(1);
    ymuint n3 = lut->index_num(2);
    vector<ymuint32> pos_array(3);
    for (ymuint i1 = 0; i1 < n1; ++ i1) {
      s << "                (";
      pos_array[0] = i1;
      const char* comma2 = "";
      for (ymuint i2 = 0; i2 < n2; ++ i2) {
	pos_array[1] = i2;
	s << comma2 << "(";
	const char* comma3 = "";
	for (ymuint i3 = 0; i3 < n3; ++ i3) {
	  pos_array[2] = i3;
	  s << comma3 << lut->grid_value(pos_array);
	  comma3 = ", ";
	}
	s << ")";
	comma2 = ", ";
      }
      s << ")" << endl;
    }
    s << "                )" << endl;
  }
}

// タイミング情報を出力する．
void
display_timing(ostream& s,
	       const Cell* cell,
	       ymuint ipos,
	       ymuint opos,
	       tCellTimingSense sense,
	       tCellDelayModel delay_model)
{
  ymuint n = cell->timing_num(ipos, opos, sense);
  for (ymuint i = 0; i < n; ++ i) {
    const CellTiming* timing = cell->timing(ipos, opos, sense, i);
    s << "  Timing:" << endl
      << "    Type             = " << timing->type() << endl
      << "    Input Pin        = " << cell->input(ipos)->name() << endl
      << "    Output Pin       = " << cell->output(opos)->name() << endl
      << "    Sense            = ";
    if ( sense == kCellPosiUnate ) {
      s << "positive unate";
    }
    else if ( sense == kCellNegaUnate ) {
      s << "negative unate";
    }
    else {
      assert_not_reached(__FILE__, __LINE__);
    }
    s << endl;
    if ( !timing->timing_cond().is_one() ) {
      s << "    When             = " << timing->timing_cond() << endl;
    }
    switch ( delay_model ) {
    case kCellDelayGenericCmos:
      s << "    Rise Intrinsic   = " << timing->intrinsic_rise() << endl
	<< "    Rise Resistance  = " << timing->rise_resistance() << endl
	<< "    Fall Intrinsic   = " << timing->intrinsic_fall() << endl
	<< "    Fall Resistance  = " << timing->fall_resistance() << endl;
      break;

    case kCellDelayTableLookup:
      if ( timing->cell_rise() ) {
	display_lut(s, "Cell Rise", timing->cell_rise());
      }
      if ( timing->rise_transition() ) {
	display_lut(s, "Rise Transition", timing->rise_transition());
      }
      if ( timing->rise_propagation() ) {
	display_lut(s, "Rise Propagation", timing->rise_propagation());
      }

      if ( timing->cell_fall() ) {
	display_lut(s, "Cell Fall", timing->cell_fall());
      }
      if ( timing->fall_transition() ) {
	display_lut(s, "Fall Transition", timing->fall_transition());
      }
      if ( timing->fall_propagation() ) {
	display_lut(s, "Fall Propagation", timing->fall_propagation());
      }
      break;

    case kCellDelayPiecewiseCmos:
      break;

    case kCellDelayCmos2:
      break;

    case kCellDelayDcm:
      break;
    }
  }
}

// セルクラスの情報を出力する．
void
display_class(ostream& s,
	      const char* title,
	      const CellClass* cclass)
{
  s << title << endl;
  ymuint n = cclass->idmap_num();
  if ( n > 0 ) {
    s << "  Idmap List = " << endl;
    for (ymuint i = 0; i < n; ++ i) {
      s << cclass->idmap(i) << endl;
    }
    s << endl;
  }
  for (ymuint i = 0; i < cclass->group_num(); ++ i) {
    const CellGroup* group = cclass->cell_group(i);
    s << "  Group: Map = " << group->map() << endl
      << "         Cell = ";
    for (ymuint j = 0; j < group->cell_num(); ++ j) {
      const Cell* cell = group->cell(j);
      s << " " << cell->name();
    }
    s << endl;
  }
  s << endl;
}

void
display_pos(ostream& s,
	    const char* title,
	    ymuint pos,
	    ymuint sense)
{
  if ( sense > 0 ) {
    s << " " << title << " = ";
    if ( sense == 2 ) {
      s << "~";
    }
    s << pos;
  }
}

// FFセルクラスの情報を出力する．
void
display_ff_class(ostream& s,
		 const char* title,
		 const CellClass* cclass)
{
  s << title << endl;
  ymuint n = cclass->idmap_num();
  if ( n > 0 ) {
    s << "  Idmap List = " << endl;
    for (ymuint i = 0; i < n; ++ i) {
      s << cclass->idmap(i) << endl;
    }
    s << endl;
  }
  for (ymuint i = 0; i < cclass->group_num(); ++ i) {
    const CellGroup* group = cclass->cell_group(i);
    s << "  Group:";
    s << " data-pin = " << group->data_pos();
    display_pos(s, "clock-pin", group->clock_pos(), group->clock_sense());
    display_pos(s, "clear-pin", group->clear_pos(), group->clear_sense());
    display_pos(s, "preset-pin", group->preset_pos(), group->preset_sense());
    s << " q-pin = " << group->q_pos()
      << " xq-pin = " << group->xq_pos()
      << endl;
    s << "         Cell = ";
    for (ymuint j = 0; j < group->cell_num(); ++ j) {
      const Cell* cell = group->cell(j);
      s << " " << cell->name();
    }
    s << endl;
  }
  s << endl;
}

// ラッチセルクラスの情報を出力する．
void
display_latch_class(ostream& s,
		    const char* title,
		    const CellClass* cclass)
{
  s << title << endl;
  ymuint n = cclass->idmap_num();
  if ( n > 0 ) {
    s << "  Idmap List = " << endl;
    for (ymuint i = 0; i < n; ++ i) {
      s << cclass->idmap(i) << endl;
    }
    s << endl;
  }
  for (ymuint i = 0; i < cclass->group_num(); ++ i) {
    const CellGroup* group = cclass->cell_group(i);
    s << "  Group:";
    display_pos(s, "data-pin",  group->data_pos(), group->has_data() ? 1 : 0);
    display_pos(s, "enable-pin", group->enable_pos(), group->enable_sense());
    display_pos(s, "clear-pin", group->clear_pos(), group->clear_sense());
    display_pos(s, "preset-pin", group->preset_pos(), group->preset_sense());
    s << " q-pin = " << group->q_pos()
      << endl;
    s << "         Cell = ";
    for (ymuint j = 0; j < group->cell_num(); ++ j) {
      const Cell* cell = group->cell(j);
      s << " " << cell->name();
    }
    s << endl;
  }
  s << endl;
}

// セルグループの情報を出力する．
void
display_group(ostream& s,
	      const char* title,
	      const CellGroup* group)
{
  s << title << endl
    << "  Cell =";
  for (ymuint i = 0; i < group->cell_num(); ++ i) {
    const Cell* cell = group->cell(i);
    s << " " << cell->name();
  }
  s << endl
    << endl;
}

void
display_index(ostream& s,
	      const CellLutTemplate* templ,
	      ymuint var)
{
  ymuint n = templ->index_num(var);
  s << "(";
  const char* comma = "";
  for (ymuint i = 0; i < n; ++ i) {
    s << comma << templ->index(var, i);
    comma = ", ";
  }
  s << ")";
}

END_NONAMESPACE

void
display_library(ostream& s,
		const CellLibrary& library)
{
  // ライブラリ名
  s << "Library(" << library.name() << ")" << endl;

  // テクノロジ
  s << "  technology: ";
  switch ( library.technology() ) {
  case CellLibrary::kTechCmos: s << "cmos"; break;
  case CellLibrary::kTechFpga: s << "fpga"; break;
  default: assert_not_reached(__FILE__, __LINE__); break;
  }
  s << endl;

  // 遅延モデル
  tCellDelayModel delay_model = library.delay_model();
  s << "  delay_model: " << delay_model << endl;

  // バス命名規則
  s << "  bus_naming_style: " << library.bus_naming_style() << endl;

  // 日付
  s << "  date: " << library.date() << endl;

  // リビジョン
  s << "  revision: " << library.revision() << endl;

  // コメント
  s << "  comment: " << library.comment() << endl;

  // 時間単位
  s << "  time_unit: " << library.time_unit() << endl;

  // 電圧単位
  s << "  voltage_unit: " << library.voltage_unit() << endl;

  // 電流単位
  s << "  current_unit: " << library.current_unit() << endl;

  // 抵抗単位
  s << "  pulling_resistance_unit: " << library.pulling_resistance_unit() << endl;

  // 容量単位
  s << "  capacitive_load_unit: " << library.capacitive_load_unit()
    << library.capacitive_load_unit_str() << endl;

  // 電力単位
  s << "  leakage_power_unit: " << library.leakage_power_unit() << endl;

  s << endl;

  // lu_table_template
  ymuint lut_template_num = library.lu_table_template_num();
  for (ymuint i = 0; i < lut_template_num; ++ i) {
    const CellLutTemplate* templ = library.lu_table_template(i);
    s << "  lu_table_template(" << templ->name() << ")" << endl;
    ymuint d = templ->dimension();
    for (ymuint j = 0; j < d; ++ j) {
      s << "    variable_" << (j + 1) << ": " << templ->variable_type(j) << endl;
    }
    for (ymuint j = 0; j < d; ++ j) {
      s << "    index_" << (j + 1) << "   : ";
      display_index(s, templ, j);
      s << endl;
    }
    s << endl;
  }

  s << endl;

  // セル
  ymuint n = library.cell_num();
  for (ymuint i = 0; i < n; ++ i) {
    const Cell* cell = library.cell(i);
    {
      const Cell* cell1 = library.cell(cell->name());
      assert_cond( cell1 == cell, __FILE__, __LINE__);
    }
    // セル名とセルの種類を出力
    s << "Cell#" << cell->id() << " (" << cell->name() << ") : ";
    if ( cell->is_logic() ) {
      s << "Combinational Logic";
    }
    else if ( cell->is_ff() ) {
      s << "Flip-Flop";
    }
    else if ( cell->is_latch() ) {
      s << "Latch";
    }
    else if ( cell->is_fsm() ) {
      s << "FSM";
    }
    else {
      assert_not_reached(__FILE__, __LINE__);
    }
    s << endl;

    // 面積
    s << "  area = " << cell->area() << endl;

    if ( cell->is_ff() ) {
      // FF の情報
      s << "  Next State         = " << cell->next_state_expr() << endl
	<< "  Clock              = " << cell->clock_expr() << endl;
      if ( !cell->clock2_expr().is_zero() ) {
	s << "  Clock2             = " << cell->clock2_expr() << endl;
      }
      if ( cell->has_clear() ) {
	s << "  Clear              = " << cell->clear_expr() << endl;
      }
      if ( cell->has_preset() ) {
	s << "  Preset             = " << cell->preset_expr() << endl;
      }
      if ( cell->has_clear() && cell->has_preset() ) {
	s << "  Clear Preset Var1  = " << cell->clear_preset_var1() << endl
	  << "  Clear Preset Var2  = " << cell->clear_preset_var2() << endl;
      }
    }
    if ( cell->is_latch() ) {
      // ラッチの情報
      s << "  Data In            = " << cell->data_in_expr() << endl
	<< "  Enable             = " << cell->enable_expr() << endl;
      if ( !cell->enable2_expr().is_zero() ) {
	s << "  Enable2            = " << cell->enable2_expr() << endl;
      }
      if ( cell->has_clear() ) {
	s << "  Clear              = " << cell->clear_expr() << endl;
      }
      if ( cell->has_preset() ) {
	s << "  Preset             = " << cell->preset_expr() << endl;
      }
      if ( cell->has_clear() && cell->has_preset() ) {
	s << "  Clear Preset Var1  = " << cell->clear_preset_var1() << endl
	  << "  Clear Preset Var2  = " << cell->clear_preset_var2() << endl;
      }
    }

    // ピンの情報
    ymuint npin = cell->pin_num();
    for (ymuint pin_id = 0; pin_id < npin; ++ pin_id) {
      const CellPin* pin = cell->pin(pin_id);
      s << "  Pin#" << pin_id << "[ " << pin->name() << " ]: ";
      if ( pin->is_input() ) {
	// 入力ピン
	s << "Input#" << pin->input_id() << endl
	  << "    Capacitance      = " << pin->capacitance() << endl
	  << "    Rise Capacitance = " << pin->rise_capacitance() << endl
	  << "    Fall Capacitance = " << pin->fall_capacitance() << endl;
      }
      else if ( pin->is_output() ) {
	// 出力ピン
	ymuint opos = pin->output_id();
	s << "Output# " << opos << endl;
	if ( cell->has_logic(opos) ) {
	  s << "    Logic            = " << cell->logic_expr(opos) << endl;
	  if ( cell->has_tristate(opos) ) {
	    s << "    Tristate         = " << cell->tristate_expr(opos) << endl;
	  }
	}
	s << "    Max Fanout       = " << pin->max_fanout() << endl
	  << "    Min Fanout       = " << pin->min_fanout() << endl
	  << "    Max Capacitance  = " << pin->max_capacitance() << endl
	  << "    Min Capacitance  = " << pin->min_capacitance() << endl
	  << "    Max Transition   = " << pin->max_transition() << endl
	  << "    Min Transition   = " << pin->min_transition() << endl;
      }
      else if ( pin->is_inout() ) {
	// 入出力ピン
	ymuint opos = pin->output_id();
	s << "Inout#(" << pin->input_id() << ", " << opos << ")" << endl;
	if ( cell->has_logic(opos) ) {
	  s << "    Logic            = " << cell->logic_expr(opos) << endl;
	  if ( cell->has_tristate(opos) ) {
	    s << "    Tristate         = " << cell->tristate_expr(opos) << endl;
	  }
	}
	s << "    Capacitance      = " << pin->capacitance() << endl
	  << "    Rise Capacitance = " << pin->rise_capacitance() << endl
	  << "    Fall Capacitance = " << pin->fall_capacitance() << endl
	  << "    Max Fanout       = " << pin->max_fanout() << endl
	  << "    Min Fanout       = " << pin->min_fanout() << endl
	  << "    Max Capacitance  = " << pin->max_capacitance() << endl
	  << "    Min Capacitance  = " << pin->min_capacitance() << endl
	  << "    Max Transition   = " << pin->max_transition() << endl
	  << "    Min Transition   = " << pin->min_transition() << endl;
      }
      else if ( pin->is_internal() ) {
	// 内部ピン
	ymuint itpos = pin->internal_id();
	s << "Internal#(" << itpos << ")" << endl;
      }
    }

    // タイミング情報
    ymuint ni2 = cell->input_num2();
    ymuint no2 = cell->output_num2();
    for (ymuint ipos = 0; ipos < ni2; ++ ipos) {
      for (ymuint opos = 0; opos < no2; ++ opos) {
	display_timing(s, cell, ipos, opos, kCellPosiUnate, delay_model);
	display_timing(s, cell, ipos, opos, kCellNegaUnate, delay_model);
      }
    }
    s << endl;
  }

  // セルグループの情報
  s << "Cell Group" << endl;
  ymuint nc = library.npn_class_num();
  for (ymuint i = 0; i < nc; ++ i) {
    const CellClass* cclass = library.npn_class(i);
    ostringstream buf;
    buf << "Class#" << i;
    display_class(s, buf.str().c_str(), cclass);
  }

  // 既定セルグループの情報
  display_group(s, "Const0 Group", library.const0_func());
  display_group(s, "Const1 Group", library.const1_func());
  display_group(s, "Buffer Group", library.buf_func());
  display_group(s, "Inverter Group", library.inv_func());

  display_ff_class(s, "DFF Class", library.simple_ff_class(false, false));
  display_ff_class(s, "DFF_R Class", library.simple_ff_class(true, false));
  display_ff_class(s, "DFF_S Class", library.simple_ff_class(false, true));
  display_ff_class(s, "DFF_RS Class", library.simple_ff_class(true, true));

  display_latch_class(s, "Latch Class", library.simple_latch_class(false, false));
  display_latch_class(s, "Latch_R Class", library.simple_latch_class(true, false));
  display_latch_class(s, "Latch_S Class", library.simple_latch_class(false, true));
  display_latch_class(s, "Latch_RS Class", library.simple_latch_class(true, true));

  // パタングラフの情報
  s << "==== PatMgr dump start ====" << endl;

  // ノードの種類の出力
  ymuint nn = library.pg_node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    s << "Node#" << i << ": ";
    switch ( library.pg_node_type(i) ) {
    case kCellPatInput: s << "INPUT#" << library.pg_input_id(i) ; break;
    case kCellPatAnd:   s << "AND"; break;
    case kCellPatXor:   s << "XOR"; break;
    default: assert_not_reached(__FILE__, __LINE__);
    }
    s << endl;
  }
  s << endl;

  // 枝の情報の出力
  ymuint ne = library.pg_edge_num();
  for (ymuint i = 0; i < ne; ++ i) {
    s << "Edge#" << i << ": " << library.pg_edge_from(i)
      << " -> " << library.pg_edge_to(i)
      << "(" << library.pg_edge_pos(i) << ")";
    if ( library.pg_edge_inv(i) ) {
      s << " ***";
    }
    s << endl;
  }
  s << endl;

  // パタングラフの情報の出力
  ymuint np = library.pg_pat_num();
  for (ymuint i = 0; i < np; ++ i) {
    const CellPatGraph& pat = library.pg_pat(i);
    s << "Pat#" << i << ": "
      << "Rep#" << pat.rep_id() << ": ";
    if ( pat.root_inv() ) {
      s << "[inv]";
    }
    s << "(" << pat.input_num() << "), ";
    ymuint n = pat.edge_num();
    for (ymuint i = 0; i < n; ++ i) {
      s << " " << pat.edge(i);
    }
    s << endl;
  }

  s << "==== PatMgr dump end ====" << endl;
}

END_NAMESPACE_YM_CELL
