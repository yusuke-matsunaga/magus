
/// @file libym_cell/dotlib/LibraryHandler.cc
/// @brief LibraryHandler などの実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LibraryHandler.h"

#include "SimpleHandler.h"
#include "ExprHandler.h"
#include "ComplexHandler.h"
#include "DefineHandler.h"
#include "GroupHandler.h"
#include "CellHandler.h"
#include "PtMgr.h"
#include "PtValue.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB


BEGIN_NONAMESPACE

inline
SimpleHandler*
new_simple(GroupHandler* parent)
{
  return new SimpleHandler(parent);
}

inline
ComplexHandler*
new_complex(GroupHandler* parent)
{
  return new ComplexHandler(parent);
}

inline
GroupHandler*
new_group(GroupHandler* parent)
{
  return new GroupHandler(parent);
}

DotlibHandler*
new_input_voltage(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // simple attributes
  DotlibHandler* simple = new ExprHandler(parent);
  handler->reg_handler("vil", simple);
  handler->reg_handler("vih", simple);
  handler->reg_handler("vimin", simple);
  handler->reg_handler("vimax", simple);

  return handler;
}

DotlibHandler*
new_output_voltage(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // simple attributes
  DotlibHandler* simple = new ExprHandler(parent);
  handler->reg_handler("vol", simple);
  handler->reg_handler("voh", simple);
  handler->reg_handler("vomin", simple);
  handler->reg_handler("vomax", simple);

  return handler;
}

DotlibHandler*
new_template(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // simple attributes
  DotlibHandler* simple = new_simple(handler);
  handler->reg_handler("variable_1", simple);
  handler->reg_handler("variable_2", simple);
  handler->reg_handler("variable_3", simple);

  // complex attributes
  DotlibHandler* complex = new_complex(handler);
  handler->reg_handler("index_1", complex);
  handler->reg_handler("index_2", complex);
  handler->reg_handler("index_3", complex);

  // group statements
  handler->reg_handler("domain", new_group(handler));

  return handler;
}

DotlibHandler*
new_operating_conditions(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // simple attributes
  DotlibHandler* simple = new_simple(handler);
  handler->reg_handler("calc_mode", simple);
  handler->reg_handler("parameter1", simple);
  handler->reg_handler("parameter2", simple);
  handler->reg_handler("parameter3", simple);
  handler->reg_handler("parameter4", simple);
  handler->reg_handler("parameter5", simple);
  handler->reg_handler("process", simple);
  handler->reg_handler("temperature", simple);
  handler->reg_handler("tree_type", simple);
  handler->reg_handler("voltage", simple);

  // complex attributes
  DotlibHandler* complex = new_complex(handler);
  handler->reg_handler("power_rail", complex);

  return handler;
}

DotlibHandler*
new_wire_load(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // simple attributes
  DotlibHandler* simple = new_simple(handler);
  handler->reg_handler("area", simple);
  handler->reg_handler("capacitance", simple);
  handler->reg_handler("resistance", simple);
  handler->reg_handler("slope", simple);

  // complex attributes
  DotlibHandler* complex = new_complex(handler);
  handler->reg_handler("fanout_length", complex);

  return handler;
}

DotlibHandler*
new_wire_load_selection(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // complex attributes
  DotlibHandler* complex = new_complex(handler);
  handler->reg_handler("wire_load_from_area", complex);

  return handler;
}

DotlibHandler*
new_wire_load_table(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // complex attributes
  DotlibHandler* complex = new_complex(handler);
  handler->reg_handler("fanout_area", complex);
  handler->reg_handler("fanout_capacitance", complex);
  handler->reg_handler("fanout_length", complex);
  handler->reg_handler("fanout_resistance", complex);

  return handler;
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス LibraryHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
// @param[in] ptmgr パース木を管理するオブジェクト
LibraryHandler::LibraryHandler(DotlibParser& parser,
			       PtMgr& ptmgr) :
  GroupHandler(parser, ptmgr)
{
  // simple attributes
  DotlibHandler* simple = new SimpleHandler(this);
  DotlibHandler* simple2 = new SymSimpleHandler(this);
  reg_handler("bus_naming_style", simple);
  reg_handler("comment", simple);
  reg_handler("current_unit", simple2);
  reg_handler("date", simple);
  reg_handler("delay_model", simple);
  reg_handler("em_temp_degradation_factor", simple);
  reg_handler("fpga_technology", simple);
  reg_handler("in_place_swap_mode", simple);
  reg_handler("input_threshold_pct_fall", simple);
  reg_handler("input_threshold_pct_rise", simple);
  reg_handler("leakage_power_unit", simple2);
  reg_handler("nom_calc_mode", simple);
  reg_handler("nom_process", simple);
  reg_handler("nom_temperature", simple);
  reg_handler("nom_voltage", simple);
  reg_handler("output_threshold_pct_fall", simple);
  reg_handler("output_threshold_pct_rise", simple);
  reg_handler("piece_type", simple);
  reg_handler("power_model", simple);
  reg_handler("preferred_output_pad_slew_rate_control", simple);
  reg_handler("preferred_input_pad_voltage", simple);
  reg_handler("preferred_output_pad_voltage", simple);
  reg_handler("pulling_resistance_unit", simple2);
  reg_handler("revision", simple2);
  reg_handler("simulation", simple);
  reg_handler("slew_derate_from_library", simple);
  reg_handler("slew_lower_threshold_pct_fall", simple);
  reg_handler("slew_lower_threshold_pct_rise", simple);
  reg_handler("slew_upper_threshold_pct_fall", simple);
  reg_handler("slew_upper_threshold_pct_rise", simple);
  reg_handler("time_unit", simple2);
  reg_handler("voltage_unit", simple2);

  // default attributes
  reg_handler("default_cell_leakage_power", simple);
  reg_handler("default_connection_class", simple);
  reg_handler("default_fall_delay_intercept", simple);
  reg_handler("default_fall_pin_resistance", simple);
  reg_handler("default_fanout_load", simple);
  reg_handler("default_inout_pin_cap", simple);
  reg_handler("default_inout_pin_fall_res", simple);
  reg_handler("default_inout_pin_rise_res", simple);
  reg_handler("default_input_pin_cap", simple);
  reg_handler("default_intrinsic_fall", simple);
  reg_handler("default_intrinsic_rise", simple);
  reg_handler("default_leakage_power_density", simple);
  reg_handler("default_max_capacitance", simple);
  reg_handler("default_max_fanout", simple);
  reg_handler("default_max_transition", simple);
  reg_handler("default_max_utilization", simple);
  reg_handler("default_min_porosity", simple);
  reg_handler("default_operating_conditions", simple);
  reg_handler("default_output_pin_cap", simple);
  reg_handler("default_output_pin_fall_res", simple);
  reg_handler("default_output_pin_rise_res", simple);
  reg_handler("default_rise_delay_intercept", simple);
  reg_handler("default_rise_pin_resistance", simple);
  reg_handler("default_slope_fall", simple);
  reg_handler("default_slope_rise", simple);
  reg_handler("default_wire_load", simple);
  reg_handler("default_wire_load_area", simple);
  reg_handler("default_wire_load_capacitance", simple);
  reg_handler("default_wire_load_mode", simple);
  reg_handler("default_wire_load_resistance", simple);
  reg_handler("default_wire_load_selection", simple);

  // scaling attributes
  reg_handler("k_process_cell_fall", simple);
  reg_handler("k_process_cell_leakage_power", simple);
  reg_handler("k_process_cell_rise", simple);
  reg_handler("k_process_drive_current", simple);
  reg_handler("k_process_drive_fall", simple);
  reg_handler("k_process_drive_rise", simple);
  reg_handler("k_process_fall_delay_intercept", simple);
  reg_handler("k_process_fall_pin_resistance", simple);
  reg_handler("k_process_fall_propagation", simple);
  reg_handler("k_process_fall_transition", simple);
  reg_handler("k_process_hold_fall", simple);
  reg_handler("k_process_hold_rise", simple);
  reg_handler("k_process_internal_power", simple);
  reg_handler("k_process_intrinsic_fall", simple);
  reg_handler("k_process_intrinsic_rise", simple);
  reg_handler("k_process_min_period", simple);
  reg_handler("k_process_min_pulse_width_high", simple);
  reg_handler("k_process_min_pulse_width_low", simple);
  reg_handler("k_process_nochange_fall", simple);
  reg_handler("k_process_nochange_rise", simple);
  reg_handler("k_process_pin_cap", simple);
  reg_handler("k_process_recovery_fall", simple);
  reg_handler("k_process_recovery_rise", simple);
  reg_handler("k_process_removal_fall", simple);
  reg_handler("k_process_removal_rise", simple);
  reg_handler("k_process_rise_delay_intercept", simple);
  reg_handler("k_process_rise_pin_resistance", simple);
  reg_handler("k_process_rise_propagation", simple);
  reg_handler("k_process_rise_transition", simple);
  reg_handler("k_process_setup_fall", simple);
  reg_handler("k_process_setup_rise", simple);
  reg_handler("k_process_skew_fall", simple);
  reg_handler("k_process_skew_rise", simple);
  reg_handler("k_process_slope_fall", simple);
  reg_handler("k_process_slope_rise", simple);
  reg_handler("k_process_wire_cap", simple);
  reg_handler("k_process_wire_res", simple);
  reg_handler("k_temp_cell_fall", simple);
  reg_handler("k_temp_cell_leakage_power", simple);
  reg_handler("k_temp_cell_rise", simple);
  reg_handler("k_temp_drive_current", simple);
  reg_handler("k_temp_drive_fall", simple);
  reg_handler("k_temp_drive_rise", simple);
  reg_handler("k_temp_fall_delay_intercept", simple);
  reg_handler("k_temp_fall_pin_resistance", simple);
  reg_handler("k_temp_fall_propagation", simple);
  reg_handler("k_temp_fall_transition", simple);
  reg_handler("k_temp_hold_fall", simple);
  reg_handler("k_temp_hold_rise", simple);
  reg_handler("k_temp_internal_power", simple);
  reg_handler("k_temp_intrinsic_fall", simple);
  reg_handler("k_temp_intrinsic_rise", simple);
  reg_handler("k_temp_min_period", simple);
  reg_handler("k_temp_min_pulse_width_high", simple);
  reg_handler("k_temp_min_pulse_width_low", simple);
  reg_handler("k_temp_nochange_fall", simple);
  reg_handler("k_temp_nochange_rise", simple);
  reg_handler("k_temp_pin_cap", simple);
  reg_handler("k_temp_recovery_fall", simple);
  reg_handler("k_temp_recovery_rise", simple);
  reg_handler("k_temp_removal_fall", simple);
  reg_handler("k_temp_removal_rise", simple);
  reg_handler("k_temp_rise_delay_intercept", simple);
  reg_handler("k_temp_rise_pin_resistance", simple);
  reg_handler("k_temp_rise_propagation", simple);
  reg_handler("k_temp_rise_transition", simple);
  reg_handler("k_temp_setup_fall", simple);
  reg_handler("k_temp_setup_rise", simple);
  reg_handler("k_temp_skew_fall", simple);
  reg_handler("k_temp_skew_rise", simple);
  reg_handler("k_temp_slope_fall", simple);
  reg_handler("k_temp_slope_rise", simple);
  reg_handler("k_temp_wire_cap", simple);
  reg_handler("k_temp_wire_res", simple);
  reg_handler("k_volt_cell_fall", simple);
  reg_handler("k_volt_cell_leakage_power", simple);
  reg_handler("k_volt_cell_rise", simple);
  reg_handler("k_volt_drive_current", simple);
  reg_handler("k_volt_drive_fall", simple);
  reg_handler("k_volt_drive_rise", simple);
  reg_handler("k_volt_fall_delay_intercept", simple);
  reg_handler("k_volt_fall_pin_resistance", simple);
  reg_handler("k_volt_fall_propagation", simple);
  reg_handler("k_volt_fall_transition", simple);
  reg_handler("k_volt_hold_fall", simple);
  reg_handler("k_volt_hold_rise", simple);
  reg_handler("k_volt_internal_power", simple);
  reg_handler("k_volt_intrinsic_fall", simple);
  reg_handler("k_volt_intrinsic_rise", simple);
  reg_handler("k_volt_min_period", simple);
  reg_handler("k_volt_min_pulse_width_high", simple);
  reg_handler("k_volt_min_pulse_width_low", simple);
  reg_handler("k_volt_nochange_fall", simple);
  reg_handler("k_volt_nochange_rise", simple);
  reg_handler("k_volt_pin_cap", simple);
  reg_handler("k_volt_recovery_fall", simple);
  reg_handler("k_volt_recovery_rise", simple);
  reg_handler("k_volt_removal_fall", simple);
  reg_handler("k_volt_removal_rise", simple);
  reg_handler("k_volt_rise_delay_intercept", simple);
  reg_handler("k_volt_rise_pin_resistance", simple);
  reg_handler("k_volt_rise_propagation", simple);
  reg_handler("k_volt_rise_transition", simple);
  reg_handler("k_volt_setup_fall", simple);
  reg_handler("k_volt_setup_rise", simple);
  reg_handler("k_volt_skew_fall", simple);
  reg_handler("k_volt_skew_rise", simple);
  reg_handler("k_volt_slope_fall", simple);
  reg_handler("k_volt_slope_rise", simple);
  reg_handler("k_volt_wire_cap", simple);
  reg_handler("k_volt_wire_res", simple);

  // complex attributes
  DotlibHandler* complex = new ComplexHandler(this);
  reg_handler("capacitive_load_unit", complex);
  reg_handler("default_part", complex);
  reg_handler("define", new DefineHandler(this));
  reg_handler("define_cell_area", complex);
  reg_handler("define_group", complex);
  reg_handler("library_features", complex);
  reg_handler("piece_define", complex);
  reg_handler("routing_layers", complex);
  reg_handler("technology", complex);

  //fall_net_delay : name ;
  //rise_net_delay : name ;

  // group statements
  reg_handler("cell", new CellHandler(this));
  reg_handler("dc_current_template", new_template(this));
  reg_handler("em_lut_template", new_template(this));
  reg_handler("fall_transition_degradation", new_group(this));
  reg_handler("faults_lut_template", new_template(this));
  reg_handler("input_voltage", new_input_voltage(this));
  reg_handler("iv_lut_template", new_template(this));
  reg_handler("lu_table_template", new_template(this));
  reg_handler("noise_lut_template", new_template(this));
  reg_handler("operating_conditions", new_operating_conditions(this));
  reg_handler("output_voltage", new_output_voltage(this));
  reg_handler("output_current_template", new_template(this));
  reg_handler("part", new_group(this));
  reg_handler("poly_template", new_template(this));
  reg_handler("power_lut_template", new_template(this));
  reg_handler("power_poly_template", new_template(this));
  reg_handler("power_supply", new_group(this));
  reg_handler("propagation_lut_template", new_template(this));
  reg_handler("rise_transition_degradation", new_group(this));
  reg_handler("scaled_cell", new_group(this));
  reg_handler("scaling_factors", new_group(this));
  reg_handler("timing", new_group(this));
  reg_handler("timing_range", new_group(this));
  reg_handler("type", new_group(this));
  reg_handler("wire_load", new_wire_load(this));
  reg_handler("wire_load_selection", new_wire_load_selection(this));
  reg_handler("wire_load_table", new_wire_load_table(this));
}

// @brief デストラクタ
LibraryHandler::~LibraryHandler()
{
}

// @brief 対応する PtNode を返す．
PtNode*
LibraryHandler::pt_node()
{
  return mLibrary;
}

// @brief グループ名を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value_list 値を表すトークンのリスト
bool
LibraryHandler::begin_group(ShString attr_name,
			    const FileRegion& attr_loc,
			    const vector<const PtValue*>& value_list)
{
  assert_cond( attr_name == "library", __FILE__, __LINE__);

  if ( value_list.size() == 0 ) {
    FileRegion loc = attr_loc;
    put_msg(__FILE__, __LINE__, loc,
	    kMsgError,
	    "DOTLIB_PARSER",
	    "library statement requires library name.");
    return false;
  }
  if ( value_list.size() > 1 ) {
    FileRegion loc = value_list[2]->loc();
    put_msg(__FILE__, __LINE__, loc,
	    kMsgError,
	    "DOTLIB_PARSER",
	    "library statement requires only one token");
    return false;
  }

  if ( value_list[0]->type() != SYMBOL ) {
    put_msg(__FILE__, __LINE__, value_list[0]->loc(),
	    kMsgError,
	    "DOTLIB_PARSER",
	    "string value is expected.");
    return false;
  }

  mLibrary = ptmgr().new_ptgroup(attr_name, attr_loc, value_list);

  return true;
}

// @brief グループ内のステートメントをすべて処理したときに呼ばれる関数
bool
LibraryHandler::end_group()
{
  return true;
}

END_NAMESPACE_YM_CELL_DOTLIB
