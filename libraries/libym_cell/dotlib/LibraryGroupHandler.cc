
/// @file libym_cell/dotlib/LibraryGroupHandler.cc
/// @brief LibraryGroupHandler などの実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LibraryGroupHandler.h"

#include "SimpleHandler.h"
#include "ComplexHandler.h"

#include "DefineHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB


BEGIN_NONAMESPACE

inline
SimpleHandler*
new_simple(GroupHandler* parent,
	   bool symbol_mode)
{
  return new SimpleHandler(parent->parser(), parent, symbol_mode);
}

inline
ComplexHandler*
new_complex(GroupHandler* parent)
{
  return new ComplexHandler(parent->parser(), parent);
}

inline
GroupHandler*
new_group(GroupHandler* parent)
{
  return new GroupHandler(parent->parser(), parent);
}

DotLibHandler*
new_ff(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);


  // simple attributes
  DotLibHandler* simple = new_simple(handler, false);
  handler->reg_handler("clear", simple);
  handler->reg_handler("clear_preset_var1", simple);
  handler->reg_handler("clear_preset_var2", simple);
  handler->reg_handler("clocked_on", simple);
  handler->reg_handler("clocked_on_also", simple);
  handler->reg_handler("next_state", simple);
  handler->reg_handler("preset", simple);

  return handler;
}

DotLibHandler*
new_latch(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // simple attributes
  DotLibHandler* simple = new_simple(handler, false);
  handler->reg_handler("clear", simple);
  handler->reg_handler("clear_preset_var1", simple);
  handler->reg_handler("clear_preset_var2", simple);
  handler->reg_handler("data_in", simple);
  handler->reg_handler("enable", simple);
  handler->reg_handler("enable_also", simple);
  handler->reg_handler("preset", simple);

  return handler;
}

DotLibHandler*
new_leakage_power(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // simple attributes
  DotLibHandler* simple = new_simple(handler, false);
  handler->reg_handler("power_level", simple);
  handler->reg_handler("related_pg_pin", simple);
  handler->reg_handler("when", simple);
  handler->reg_handler("value", simple);

  return handler;
}

DotLibHandler*
new_power(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // simple attributes

  // complex attribute
  DotLibHandler* complex = new_complex(handler);
  handler->reg_handler("index_1", complex);
  handler->reg_handler("index_2", complex);
  handler->reg_handler("index_3", complex);
  handler->reg_handler("values", complex);
  handler->reg_handler("orders", complex);
  handler->reg_handler("coefs", complex);

  // group statements
  handler->reg_handler("domain", new_group(handler));

  return handler;
}

DotLibHandler*
new_internal_power(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // simple attributes
  DotLibHandler* simple = new_simple(handler, false);
  handler->reg_handler("equal_or_opposite_output", simple);
  handler->reg_handler("falling_together_group", simple);
  handler->reg_handler("power_level", simple);
  handler->reg_handler("related_pin", simple);
  handler->reg_handler("rising_together_group", simple);
  handler->reg_handler("switching_interval", simple);
  handler->reg_handler("switching_together_group", simple);
  handler->reg_handler("when", simple);

  // complex attribute

  // group statements
  handler->reg_handler("power", new_power(handler));
  handler->reg_handler("fall_power", new_power(handler));
  handler->reg_handler("rise_power", new_power(handler));
  handler->reg_handler("domain", new_group(handler));

  return handler;
}

DotLibHandler*
new_tbl(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // simple attributes

  // complex attribute
  DotLibHandler* complex = new_complex(handler);
  handler->reg_handler("index_1", complex);
  handler->reg_handler("index_2", complex);
  handler->reg_handler("index_3", complex);
  handler->reg_handler("values", complex);

  // group statements
  handler->reg_handler("domain", new_group(handler));

  return handler;
}

DotLibHandler*
new_timing(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // simple attributes
  DotLibHandler* simple = new_simple(handler, false);
  handler->reg_handler("edge_rate_sensitivity_f0", simple);
  handler->reg_handler("edge_rate_sensitivity_f1", simple);
  handler->reg_handler("edge_rate_sensitivity_r0", simple);
  handler->reg_handler("edge_rate_sensitivity_r1", simple);
  handler->reg_handler("fall_resistance", simple);
  handler->reg_handler("intrinsic_fall", simple);
  handler->reg_handler("intrinsic_rise", simple);
  handler->reg_handler("related_bus_equivalent", simple);
  handler->reg_handler("related_bus_pins", simple);
  handler->reg_handler("related_output_pin", simple);
  handler->reg_handler("related_pin", simple);
  handler->reg_handler("rise_resistance", simple);
  handler->reg_handler("sdf_cond", simple);
  handler->reg_handler("sdf_cond_end", simple);
  handler->reg_handler("sdf_cond_start", simple);
  handler->reg_handler("sdf_edges", simple);
  handler->reg_handler("slope_fall", simple);
  handler->reg_handler("slope_rise", simple);
  handler->reg_handler("timing_sense", simple);
  handler->reg_handler("timing_type", simple);
  handler->reg_handler("when", simple);
  handler->reg_handler("when_end", simple);
  handler->reg_handler("when_start", simple);

  // complex attribute
  DotLibHandler* complex = new_complex(handler);
  handler->reg_handler("fall_delay_intercept", complex);
  handler->reg_handler("fall_pin_resistance", complex);
  handler->reg_handler("rise_delay_intercept", complex);
  handler->reg_handler("rise_pin_resistance", complex);
  handler->reg_handler("orders", complex);
  handler->reg_handler("coefs", complex);

  // group statements
  handler->reg_handler("cell_degradation", new_group(handler));
  handler->reg_handler("cell_fall", new_tbl(handler));
  handler->reg_handler("cell_rise", new_tbl(handler));
  handler->reg_handler("fall_constraint", new_tbl(handler));
  handler->reg_handler("fall_propagation", new_tbl(handler));
  handler->reg_handler("fall_transition", new_tbl(handler));
  handler->reg_handler("noise_immunity_above_high", new_group(handler));
  handler->reg_handler("noise_immunity_below_low", new_group(handler));
  handler->reg_handler("noise_immunity_high", new_group(handler));
  handler->reg_handler("noise_immunity_low", new_group(handler));
  handler->reg_handler("propogated_noise_height_above_high", new_group(handler));
  handler->reg_handler("propogated_noise_height_below_low", new_group(handler));
  handler->reg_handler("propogated_noise_height_high", new_group(handler));
  handler->reg_handler("propogated_noise_height_low", new_group(handler));
  handler->reg_handler("propogated_noise_peak_time_ratio_above_high", new_group(handler));
  handler->reg_handler("propogated_noise_peak_time_ratio__below_low", new_group(handler));
  handler->reg_handler("propogated_noise_peak_time_ratio_high", new_group(handler));
  handler->reg_handler("propogated_noise_peak_time_ratio_low", new_group(handler));
  handler->reg_handler("propogated_noise_width_above_high", new_group(handler));
  handler->reg_handler("propogated_noise_width_below_low", new_group(handler));
  handler->reg_handler("propogated_noise_width_high", new_group(handler));
  handler->reg_handler("propogated_noise_width_low", new_group(handler));
  handler->reg_handler("retaining_fall", new_group(handler));
  handler->reg_handler("retaining_rise", new_group(handler));
  handler->reg_handler("retain_fall_slew", new_group(handler));
  handler->reg_handler("retain_rise_slew", new_group(handler));
  handler->reg_handler("rise_constraint", new_tbl(handler));
  handler->reg_handler("rise_propagation", new_tbl(handler));
  handler->reg_handler("rise_transition", new_tbl(handler));
  handler->reg_handler("steady_state_current_high", new_group(handler));
  handler->reg_handler("steady_state_current_low", new_group(handler));
  handler->reg_handler("steady_state_current_tristate", new_group(handler));

  return handler;
}

DotLibHandler*
new_pin(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // simple attributes
  DotLibHandler* simple = new_simple(handler, false);
  handler->reg_handler("bit_width", simple);
  handler->reg_handler("capacitance", simple);
  handler->reg_handler("clock", simple);
  handler->reg_handler("clock_gate_clock_pin", simple);
  handler->reg_handler("clock_gate_enable_pin", simple);
  handler->reg_handler("clock_gate_test_pin", simple);
  handler->reg_handler("clock_gate_obs_pin", simple);
  handler->reg_handler("clock_gate_out_pin", simple);
  handler->reg_handler("complementary_pin", simple);
  handler->reg_handler("connection_class", simple);
  handler->reg_handler("direction", simple);
  handler->reg_handler("dont_fault", simple);
  handler->reg_handler("drive_current", simple);
  handler->reg_handler("driver_type", simple);
  handler->reg_handler("fall_capacitance", simple);
  handler->reg_handler("fall_current_slope_after_threshold", simple);
  handler->reg_handler("fall_current_slope_before_threshold", simple);
  handler->reg_handler("fall_time_after_threshold", simple);
  handler->reg_handler("fall_time_before_threshold", simple);
  handler->reg_handler("fanout_load", simple);
  handler->reg_handler("fault_model", simple);
  handler->reg_handler("function", simple);
  handler->reg_handler("has_builtin_pad", simple);
  handler->reg_handler("hysteresis", simple);
  handler->reg_handler("input_map", simple);
  handler->reg_handler("input_signal_level", simple);
  handler->reg_handler("input_voltage", simple);
  handler->reg_handler("internal_node", simple);
  handler->reg_handler("inverted_output", simple);
  handler->reg_handler("is_pad", simple);
  handler->reg_handler("max_capacitance", simple);
  handler->reg_handler("max_fanout", simple);
  handler->reg_handler("max_input_noise_width", simple);
  handler->reg_handler("max_transition", simple);
  handler->reg_handler("min_capacitance", simple);
  handler->reg_handler("min_fanout", simple);
  handler->reg_handler("min_input_noise_width", simple);
  handler->reg_handler("min_period", simple);
  handler->reg_handler("min_pulse_width_high", simple);
  handler->reg_handler("min_pulse_width_low", simple);
  handler->reg_handler("min_transition", simple);
  handler->reg_handler("multicell_pad_pin", simple);
  handler->reg_handler("nextstate_type", simple);
  handler->reg_handler("output_signal_level", simple);
  handler->reg_handler("output_voltage", simple);
  handler->reg_handler("pin_func_type", simple);
  handler->reg_handler("prefer_tied", simple);
  handler->reg_handler("primary_output", simple);
  handler->reg_handler("pulling_current", simple);
  handler->reg_handler("pulling_resistance", simple);
  handler->reg_handler("rise_capacitance", simple);
  handler->reg_handler("rise_current_slope_after_threshold", simple);
  handler->reg_handler("rise_current_slope_before_threshold", simple);
  handler->reg_handler("rise_time_after_threshold", simple);
  handler->reg_handler("rise_time_before_threshold", simple);
  handler->reg_handler("signal_type", simple);
  handler->reg_handler("slew_control", simple);
  handler->reg_handler("state_function", simple);
  handler->reg_handler("test_output_only", simple);
  handler->reg_handler("three_state", simple);
  handler->reg_handler("vhdl_name", simple);
  handler->reg_handler("x_function", simple);

  // complex attributes
  DotLibHandler* complex = new_complex(handler);
  handler->reg_handler("fall_capacitance_range", complex);
  handler->reg_handler("rise_capacitance_range", complex);

  // group statements
  handler->reg_handler("electromigration", new_group(handler));
  handler->reg_handler("hyperbolic_noise_above_high", new_group(handler));
  handler->reg_handler("hyperbolic_noise_below_low", new_group(handler));
  handler->reg_handler("hyperbolic_noise_high", new_group(handler));
  handler->reg_handler("hyperbolic_noise_low", new_group(handler));
  handler->reg_handler("internal_power", new_internal_power(handler));
  handler->reg_handler("max_trans", new_group(handler));
  handler->reg_handler("min_pulse_width", new_group(handler));
  handler->reg_handler("minimum_period", new_group(handler));
  handler->reg_handler("timing", new_timing(handler));
  handler->reg_handler("tlatch", new_group(handler));

  return handler;
}

DotLibHandler*
new_bus(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // simple attributes
  DotLibHandler* simple = new_simple(handler, false);
  handler->reg_handler("bus_type", simple);

  // group statements
  handler->reg_handler("pin", new_pin(handler));

  return handler;
}

DotLibHandler*
new_bundle(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // simple attributes
  DotLibHandler* simple = new_simple(handler, false);
  handler->reg_handler("capacitance", simple);
  handler->reg_handler("direction", simple);
  handler->reg_handler("function", simple);

  // complex attributes
  DotLibHandler* complex = new_complex(handler);
  handler->reg_handler("members", complex);

  // group statements
  handler->reg_handler("pin", new_pin(handler));
  handler->reg_handler("electromigration", new_group(handler));
  handler->reg_handler("hyperbolic_noise_above_high", new_group(handler));
  handler->reg_handler("hyperbolic_noise_below_low", new_group(handler));
  handler->reg_handler("hyperbolic_noise_high", new_group(handler));
  handler->reg_handler("hyperbolic_noise_low", new_group(handler));
  handler->reg_handler("internal_power", new_internal_power(handler));
  handler->reg_handler("max_trans", new_group(handler));
  handler->reg_handler("min_pulse_width", new_group(handler));
  handler->reg_handler("minimum_period", new_group(handler));
  handler->reg_handler("timing", new_timing(handler));
  handler->reg_handler("tlatch", new_group(handler));

  return handler;
}

DotLibHandler*
new_cell_internal_power(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // simple attributes
  DotLibHandler* simple = new_simple(handler, false);
  handler->reg_handler("related_inputs", simple);
  handler->reg_handler("related_outputs", simple);

  // complex attribute
  DotLibHandler* complex = new_complex(handler);
  handler->reg_handler("values", complex);

  return handler;
}

DotLibHandler*
new_statetable(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);


  // simple attributes
  DotLibHandler* simple = new_simple(handler, false);
  handler->reg_handler("table", simple);

  return handler;
}

DotLibHandler*
new_test_cell(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);
  // group statements
  handler->reg_handler("bus", new_bus(handler));
  handler->reg_handler("dynamic_current", new_group(handler));
  handler->reg_handler("ff", new_ff(handler));
  handler->reg_handler("ff_bank", new_ff(handler));
  handler->reg_handler("functional_yield_metric", new_group(handler));
  handler->reg_handler("generated_clock", new_group(handler));
  handler->reg_handler("intrinsic_parasitic", new_group(handler));
  handler->reg_handler("latch", new_latch(handler));
  handler->reg_handler("latch_bank", new_latch(handler));
  handler->reg_handler("leakage_current", new_group(handler));
  handler->reg_handler("leakage_power", new_leakage_power(handler));
  handler->reg_handler("lut", new_group(handler));
  handler->reg_handler("mode_definition", new_group(handler));
  handler->reg_handler("pin", new_pin(handler));
  handler->reg_handler("statetable", new_statetable(handler));

  return handler;
}

DotLibHandler*
new_cell(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // simple attributes
  DotLibHandler* simple = new_simple(handler, false);
  handler->reg_handler("area", simple);
  handler->reg_handler("auxiliary_pad_cell", simple);
  handler->reg_handler("base_name", simple);
  handler->reg_handler("bus_naming_style", simple);
  handler->reg_handler("cell_footprint", simple);
  handler->reg_handler("cell_leakage_power", simple);
  handler->reg_handler("clock_gating_integrated_cell", simple);
  handler->reg_handler("contention_condition", simple);
  handler->reg_handler("dont_fault", simple);
  handler->reg_handler("dont_touch", simple);
  handler->reg_handler("dont_use", simple);
  handler->reg_handler("driver_type", simple);
  handler->reg_handler("edif_name", simple);
  handler->reg_handler("em_temp_degradation_factor", simple);
  handler->reg_handler("fpga_domain_style", simple);
  handler->reg_handler("geometry_print", simple);
  handler->reg_handler("handle_negative_constraint", simple);
  handler->reg_handler("interface_timing", simple);
  handler->reg_handler("io_type", simple);
  handler->reg_handler("is_clock_gating_cell", simple);
  handler->reg_handler("map_only", simple);
  handler->reg_handler("pad_cell", simple);
  handler->reg_handler("pad_type", simple);
  handler->reg_handler("power_cell_type", simple);
  handler->reg_handler("preferred", simple);
  handler->reg_handler("scaling_factors", simple);
  handler->reg_handler("single_bit_degenerate", simple);
  handler->reg_handler("slew_type", simple);
  handler->reg_handler("timing_model_type", simple);
  handler->reg_handler("use_for_size_only", simple);
  handler->reg_handler("vhdl_name", simple);

  handler->reg_handler("is_filler_cell", simple);

  // complex attributes
  DotLibHandler* complex = new_complex(handler);
  handler->reg_handler("pin_opposite", complex);
  handler->reg_handler("rail_connection", complex);
  handler->reg_handler("power_supply_namestring", complex);
  handler->reg_handler("resource_usage", complex);

  // group statements
  handler->reg_handler("bus", new_bus(handler));
  handler->reg_handler("bundle", new_bundle(handler));
  handler->reg_handler("dynamic_current", new_group(handler));
  handler->reg_handler("ff", new_ff(handler));
  handler->reg_handler("ff_bank", new_ff(handler));
  handler->reg_handler("functional_yield_metric", new_group(handler));
  handler->reg_handler("generated_clock", new_group(handler));
  handler->reg_handler("intrinsic_parasitic", new_group(handler));
  handler->reg_handler("latch", new_latch(handler));
  handler->reg_handler("latch_bank", new_latch(handler));
  handler->reg_handler("leakage_current", new_group(handler));
  handler->reg_handler("leakage_power", new_leakage_power(handler));
  handler->reg_handler("lut", new_group(handler));
  handler->reg_handler("mode_definition", new_group(handler));
  handler->reg_handler("pin", new_pin(handler));
  handler->reg_handler("routing_track", new_group(handler));
  handler->reg_handler("statetable", new_statetable(handler));

  handler->reg_handler("internal_power", new_cell_internal_power(handler));

  handler->reg_handler("test_cell", new_test_cell(handler));

  return handler;
}

DotLibHandler*
new_input_voltage(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // simple attributes
  DotLibHandler* simple = new_simple(handler, false);
  handler->reg_handler("vil", simple);
  handler->reg_handler("vih", simple);
  handler->reg_handler("vimin", simple);
  handler->reg_handler("vimax", simple);

  return handler;
}

DotLibHandler*
new_output_voltage(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // simple attributes
  DotLibHandler* simple = new_simple(handler, false);
  handler->reg_handler("vol", simple);
  handler->reg_handler("voh", simple);
  handler->reg_handler("vomin", simple);
  handler->reg_handler("vomax", simple);

  return handler;
}

DotLibHandler*
new_template(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // simple attributes
  DotLibHandler* simple = new_simple(handler, false);
  handler->reg_handler("variable_1", simple);
  handler->reg_handler("variable_2", simple);
  handler->reg_handler("variable_3", simple);

  // complex attributes
  DotLibHandler* complex = new_complex(handler);
  handler->reg_handler("index_1", complex);
  handler->reg_handler("index_2", complex);
  handler->reg_handler("index_3", complex);

  // group statements
  handler->reg_handler("domain", new_group(handler));

  return handler;
}

DotLibHandler*
new_operating_conditions(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // simple attributes
  DotLibHandler* simple = new_simple(handler, false);
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
  DotLibHandler* complex = new_complex(handler);
  handler->reg_handler("power_rail", complex);

  return handler;
}

DotLibHandler*
new_wire_load(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // simple attributes
  DotLibHandler* simple = new_simple(handler, false);
  handler->reg_handler("area", simple);
  handler->reg_handler("capacitance", simple);
  handler->reg_handler("resistance", simple);
  handler->reg_handler("slope", simple);

  // complex attributes
  DotLibHandler* complex = new_complex(handler);
  handler->reg_handler("fanout_length", complex);

  return handler;
}

DotLibHandler*
new_wire_load_selection(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // complex attributes
  DotLibHandler* complex = new_complex(handler);
  handler->reg_handler("wire_load_from_area", complex);

  return handler;
}

DotLibHandler*
new_wire_load_table(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // complex attributes
  DotLibHandler* complex = new_complex(handler);
  handler->reg_handler("fanout_area", complex);
  handler->reg_handler("fanout_capacitance", complex);
  handler->reg_handler("fanout_length", complex);
  handler->reg_handler("fanout_resistance", complex);

  return handler;
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス LibraryGroupHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
LibraryGroupHandler::LibraryGroupHandler(DotLibParser& parser) :
  GroupHandler(parser, NULL)
{

  // simple attributes
  DotLibHandler* simple = new SimpleHandler(parser, this, false);
  DotLibHandler* simple2 = new SimpleHandler(parser, this, true);
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
  DotLibHandler* complex = new ComplexHandler(parser, this);
  reg_handler("capacitive_load_unit", complex);
  reg_handler("default_part", complex);
  reg_handler("define", new DefineHandler(parser, this));
  reg_handler("define_cell_area", complex);
  reg_handler("define_group", complex);
  reg_handler("library_features", complex);
  reg_handler("piece_define", complex);
  reg_handler("routing_layers", complex);
  reg_handler("technology", complex);

  //fall_net_delay : name ;
  //rise_net_delay : name ;

  // group statements
  reg_handler("cell", new_cell(this));
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
LibraryGroupHandler::~LibraryGroupHandler()
{
}

// @brief グループ名を読み込んだ時の処理
// @param[in] attr_token 属性名を表すトークン
// @param[in] value_list 値を表すトークンのリスト
bool
LibraryGroupHandler::begin_group(Token attr_token,
				 const vector<Token>& value_list)
{
  assert_cond( attr_token.value() == "library", __FILE__, __LINE__);

  if ( value_list.size() != 1 ) {
    FileRegion loc;
    if ( value_list.size() > 1 ) {
      loc = value_list[2].loc();
    }
    msg_mgr().put_msg(__FILE__, __LINE__, loc,
		      kMsgError,
		      "DOTLIB_PARSER",
		      "library statement require only one token");
    return false;
  }

  if ( value_list[0].type() != SYMBOL ) {
    msg_mgr().put_msg(__FILE__, __LINE__, value_list[0].loc(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      "string type is expected.");
    return false;
  }

  set_pt_node( new PtNode(attr_token, value_list) );

  return true;
}

// @brief グループ内のステートメントをすべて処理したときに呼ばれる関数
bool
LibraryGroupHandler::end_group()
{
  return true;
}

END_NAMESPACE_YM_CELL_DOTLIB
