
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

#include "ym_logic/LogExpr.h"
#include "ym_logic/NpnMapM.h"
#include "ym_utils/BinIO.h"


BEGIN_NAMESPACE_YM_CELL

BEGIN_NONAMESPACE

// タイミング情報を出力する．
void
display_timing(ostream& s,
	       const Cell* cell,
	       ymuint ipos,
	       ymuint opos,
	       tCellTimingSense sense)
{
  const CellTiming* timing = cell->timing(ipos, opos, sense);
  if ( timing ) {
    s << "  Timing:" << endl
      << "    Input Pin       = " << cell->input(ipos)->name() << endl
      << "    Output Pin      = " << cell->output(opos)->name() << endl
      << "    Sense           = ";
    if ( sense == kCellPosiUnate ) {
      s << "positive unate";
    }
    else if ( sense == kCellNegaUnate ) {
      s << "negative unate";
    }
    else {
      assert_not_reached(__FILE__, __LINE__);
    }
    s << endl
      << "    Rise Intrinsic  = " << timing->intrinsic_rise() << endl
      << "    Rise Resistance = " << timing->rise_resistance() << endl
      << "    Fall Intrinsic  = " << timing->intrinsic_fall() << endl
      << "    Fall Resistance = " << timing->fall_resistance() << endl;
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
  }
  return s;
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
  s << "  delay_model: ";
  switch ( library.delay_model() ) {
  case CellLibrary::kDelayGenericCmos: s << "generic_cmos"; break;
  case CellLibrary::kDelayTableLookup: s << "table_lookup"; break;
  case CellLibrary::kDelayPiecewiseCmos: s << "piecewise_cmos"; break;
  case CellLibrary::kDelayCmos2: s << "cmos2"; break;
  case CellLibrary::kDelayDcm:  s << "dcm"; break;
  default: assert_not_reached(__FILE__, __LINE__); break;
  }
  s << endl;

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
	display_timing(s, cell, ipos, opos, kCellPosiUnate);
	display_timing(s, cell, ipos, opos, kCellNegaUnate);
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
  ymuint nn = library.node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    s << "Node#" << i << ": ";
    switch ( library.node_type(i) ) {
    case kCellPatInput: s << "INPUT#" << library.input_id(i) ; break;
    case kCellPatAnd:   s << "AND"; break;
    case kCellPatXor:   s << "XOR"; break;
    default: assert_not_reached(__FILE__, __LINE__);
    }
    s << endl;
  }
  s << endl;

  // 枝の情報の出力
  ymuint ne = library.edge_num();
  for (ymuint i = 0; i < ne; ++ i) {
    s << "Edge#" << i << ": " << library.edge_from(i)
      << " -> " << library.edge_to(i) << "(" << library.edge_pos(i) << ")";
    if ( library.edge_inv(i) ) {
      s << " ***";
    }
    s << endl;
  }
  s << endl;

  // パタングラフの情報の出力
  ymuint np = library.pat_num();
  for (ymuint i = 0; i < np; ++ i) {
    const CellPatGraph& pat = library.pat(i);
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
