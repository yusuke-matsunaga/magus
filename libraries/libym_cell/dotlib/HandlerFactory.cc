
/// @file libym_dotlib/HandlerFactory.cc
/// @brief HandlerFactory の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "HandlerFactory.h"

#include "SimpleHandler.h"
#include "ComplexHandler.h"
#include "DefineHandler.h"
#include "GroupHandler.h"
#include "ExprHandler.h"
#include "FuncHandler.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス HandlerFactory
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
HandlerFactory::HandlerFactory()
{
}

// @brief デストラクタ
HandlerFactory::~HandlerFactory()
{
}

// @brief library group 用のハンドラを作る．
// @param[in] parser パーサー
DotlibHandler*
HandlerFactory::new_library(DotlibParserImpl& parser)
{
  GroupHandler* handler = new Str1GroupHandler(parser);

  // simple attributes
  DotlibHandler* simple = new SimpleHandler(handler, false);
  DotlibHandler* str_simple = new StrSimpleHandler(handler, false);
  DotlibHandler* symstr_simple = new StrSimpleHandler(handler, true);
  DotlibHandler* flt_simple = new FloatSimpleHandler(handler);

  handler->reg_handler("bus_naming_style",                       str_simple);
  handler->reg_handler("comment",                                str_simple);
  handler->reg_handler("current_unit",                           symstr_simple);
  handler->reg_handler("date",                                   str_simple);
  handler->reg_handler("delay_model",                            str_simple);
  handler->reg_handler("em_temp_degradation_factor",             flt_simple);
  handler->reg_handler("fpga_technology",                        str_simple);
  handler->reg_handler("in_place_swap_mode",                     str_simple);
  handler->reg_handler("input_threshold_pct_fall",               simple);
  handler->reg_handler("input_threshold_pct_rise",               simple);
  handler->reg_handler("leakage_power_unit",                     symstr_simple);
  handler->reg_handler("nom_calc_mode",                          str_simple);
  handler->reg_handler("nom_process",                            flt_simple);
  handler->reg_handler("nom_temperature",                        flt_simple);
  handler->reg_handler("nom_voltage",                            flt_simple);
  handler->reg_handler("output_threshold_pct_fall",              simple);
  handler->reg_handler("output_threshold_pct_rise",              simple);
  handler->reg_handler("piece_type",                             str_simple);
  handler->reg_handler("power_model",                            str_simple);
  handler->reg_handler("preferred_output_pad_slew_rate_control", str_simple);
  handler->reg_handler("preferred_input_pad_voltage",            str_simple);
  handler->reg_handler("preferred_output_pad_voltage",           str_simple);
  handler->reg_handler("pulling_resistance_unit",                symstr_simple);
  handler->reg_handler("revision",                               symstr_simple);
  handler->reg_handler("simulation",                             str_simple);
  handler->reg_handler("slew_derate_from_library",               simple);
  handler->reg_handler("slew_lower_threshold_pct_fall",          simple);
  handler->reg_handler("slew_lower_threshold_pct_rise",          simple);
  handler->reg_handler("slew_upper_threshold_pct_fall",          simple);
  handler->reg_handler("slew_upper_threshold_pct_rise",          simple);
  handler->reg_handler("time_unit",                              symstr_simple);
  handler->reg_handler("voltage_unit",                           symstr_simple);

  // default attributes
  handler->reg_handler("default_cell_leakage_power",             flt_simple);
  handler->reg_handler("default_connection_class",               str_simple);
  handler->reg_handler("default_fall_delay_intercept",           flt_simple);
  handler->reg_handler("default_fall_pin_resistance",            flt_simple);
  handler->reg_handler("default_fanout_load",                    flt_simple);
  handler->reg_handler("default_inout_pin_cap",                  flt_simple);
  handler->reg_handler("default_inout_pin_fall_res",             flt_simple);
  handler->reg_handler("default_inout_pin_rise_res",             flt_simple);
  handler->reg_handler("default_input_pin_cap",                  flt_simple);
  handler->reg_handler("default_intrinsic_fall",                 flt_simple);
  handler->reg_handler("default_intrinsic_rise",                 flt_simple);
  handler->reg_handler("default_leakage_power_density",          flt_simple);
  handler->reg_handler("default_max_capacitance",                flt_simple);
  handler->reg_handler("default_max_fanout",                     flt_simple);
  handler->reg_handler("default_max_transition",                 flt_simple);
  handler->reg_handler("default_max_utilization",                flt_simple);
  handler->reg_handler("default_min_porosity",                   flt_simple);
  handler->reg_handler("default_operating_conditions",           str_simple);
  handler->reg_handler("default_output_pin_cap",                 flt_simple);
  handler->reg_handler("default_output_pin_fall_res",            flt_simple);
  handler->reg_handler("default_output_pin_rise_res",            flt_simple);
  handler->reg_handler("default_rise_delay_intercept",           flt_simple);
  handler->reg_handler("default_rise_pin_resistance",            flt_simple);
  handler->reg_handler("default_slope_fall",                     flt_simple);
  handler->reg_handler("default_slope_rise",                     flt_simple);
  handler->reg_handler("default_wire_load",                      str_simple);
  handler->reg_handler("default_wire_load_area",                 flt_simple);
  handler->reg_handler("default_wire_load_capacitance",          flt_simple);
  handler->reg_handler("default_wire_load_mode",                 str_simple);
  handler->reg_handler("default_wire_load_resistance",           flt_simple);
  handler->reg_handler("default_wire_load_selection",            str_simple);

  // scaling attributes
  handler->reg_handler("k_process_cell_fall",                    flt_simple);
  handler->reg_handler("k_process_cell_leakage_power",           flt_simple);
  handler->reg_handler("k_process_cell_rise",                    flt_simple);
  handler->reg_handler("k_process_drive_current",                flt_simple);
  handler->reg_handler("k_process_drive_fall",                   flt_simple);
  handler->reg_handler("k_process_drive_rise",                   flt_simple);
  handler->reg_handler("k_process_fall_delay_intercept",         flt_simple);
  handler->reg_handler("k_process_fall_pin_resistance",          flt_simple);
  handler->reg_handler("k_process_fall_propagation",             flt_simple);
  handler->reg_handler("k_process_fall_transition",              flt_simple);
  handler->reg_handler("k_process_hold_fall",                    flt_simple);
  handler->reg_handler("k_process_hold_rise",                    flt_simple);
  handler->reg_handler("k_process_internal_power",               flt_simple);
  handler->reg_handler("k_process_intrinsic_fall",               flt_simple);
  handler->reg_handler("k_process_intrinsic_rise",               flt_simple);
  handler->reg_handler("k_process_min_period",                   flt_simple);
  handler->reg_handler("k_process_min_pulse_width_high",         flt_simple);
  handler->reg_handler("k_process_min_pulse_width_low",          flt_simple);
  handler->reg_handler("k_process_nochange_fall",                flt_simple);
  handler->reg_handler("k_process_nochange_rise",                flt_simple);
  handler->reg_handler("k_process_pin_cap",                      flt_simple);
  handler->reg_handler("k_process_recovery_fall",                flt_simple);
  handler->reg_handler("k_process_recovery_rise",                flt_simple);
  handler->reg_handler("k_process_removal_fall",                 flt_simple);
  handler->reg_handler("k_process_removal_rise",                 flt_simple);
  handler->reg_handler("k_process_rise_delay_intercept",         flt_simple);
  handler->reg_handler("k_process_rise_pin_resistance",          flt_simple);
  handler->reg_handler("k_process_rise_propagation",             flt_simple);
  handler->reg_handler("k_process_rise_transition",              flt_simple);
  handler->reg_handler("k_process_setup_fall",                   flt_simple);
  handler->reg_handler("k_process_setup_rise",                   flt_simple);
  handler->reg_handler("k_process_skew_fall",                    flt_simple);
  handler->reg_handler("k_process_skew_rise",                    flt_simple);
  handler->reg_handler("k_process_slope_fall",                   flt_simple);
  handler->reg_handler("k_process_slope_rise",                   flt_simple);
  handler->reg_handler("k_process_wire_cap",                     flt_simple);
  handler->reg_handler("k_process_wire_res",                     flt_simple);
  handler->reg_handler("k_temp_cell_fall",                       flt_simple);
  handler->reg_handler("k_temp_cell_leakage_power",              flt_simple);
  handler->reg_handler("k_temp_cell_rise",                       flt_simple);
  handler->reg_handler("k_temp_drive_current",                   flt_simple);
  handler->reg_handler("k_temp_drive_fall",                      flt_simple);
  handler->reg_handler("k_temp_drive_rise",                      flt_simple);
  handler->reg_handler("k_temp_fall_delay_intercept",            flt_simple);
  handler->reg_handler("k_temp_fall_pin_resistance",             flt_simple);
  handler->reg_handler("k_temp_fall_propagation",                flt_simple);
  handler->reg_handler("k_temp_fall_transition",                 flt_simple);
  handler->reg_handler("k_temp_hold_fall",                       flt_simple);
  handler->reg_handler("k_temp_hold_rise",                       flt_simple);
  handler->reg_handler("k_temp_internal_power",                  flt_simple);
  handler->reg_handler("k_temp_intrinsic_fall",                  flt_simple);
  handler->reg_handler("k_temp_intrinsic_rise",                  flt_simple);
  handler->reg_handler("k_temp_min_period",                      flt_simple);
  handler->reg_handler("k_temp_min_pulse_width_high",            flt_simple);
  handler->reg_handler("k_temp_min_pulse_width_low",             flt_simple);
  handler->reg_handler("k_temp_nochange_fall",                   flt_simple);
  handler->reg_handler("k_temp_nochange_rise",                   flt_simple);
  handler->reg_handler("k_temp_pin_cap",                         flt_simple);
  handler->reg_handler("k_temp_recovery_fall",                   flt_simple);
  handler->reg_handler("k_temp_recovery_rise",                   flt_simple);
  handler->reg_handler("k_temp_removal_fall",                    flt_simple);
  handler->reg_handler("k_temp_removal_rise",                    flt_simple);
  handler->reg_handler("k_temp_rise_delay_intercept",            flt_simple);
  handler->reg_handler("k_temp_rise_pin_resistance",             flt_simple);
  handler->reg_handler("k_temp_rise_propagation",                flt_simple);
  handler->reg_handler("k_temp_rise_transition",                 flt_simple);
  handler->reg_handler("k_temp_setup_fall",                      flt_simple);
  handler->reg_handler("k_temp_setup_rise",                      flt_simple);
  handler->reg_handler("k_temp_skew_fall",                       flt_simple);
  handler->reg_handler("k_temp_skew_rise",                       flt_simple);
  handler->reg_handler("k_temp_slope_fall",                      flt_simple);
  handler->reg_handler("k_temp_slope_rise",                      flt_simple);
  handler->reg_handler("k_temp_wire_cap",                        flt_simple);
  handler->reg_handler("k_temp_wire_res",                        flt_simple);
  handler->reg_handler("k_volt_cell_fall",                       flt_simple);
  handler->reg_handler("k_volt_cell_leakage_power",              flt_simple);
  handler->reg_handler("k_volt_cell_rise",                       flt_simple);
  handler->reg_handler("k_volt_drive_current",                   flt_simple);
  handler->reg_handler("k_volt_drive_fall",                      flt_simple);
  handler->reg_handler("k_volt_drive_rise",                      flt_simple);
  handler->reg_handler("k_volt_fall_delay_intercept",            flt_simple);
  handler->reg_handler("k_volt_fall_pin_resistance",             flt_simple);
  handler->reg_handler("k_volt_fall_propagation",                flt_simple);
  handler->reg_handler("k_volt_fall_transition",                 flt_simple);
  handler->reg_handler("k_volt_hold_fall",                       flt_simple);
  handler->reg_handler("k_volt_hold_rise",                       flt_simple);
  handler->reg_handler("k_volt_internal_power",                  flt_simple);
  handler->reg_handler("k_volt_intrinsic_fall",                  flt_simple);
  handler->reg_handler("k_volt_intrinsic_rise",                  flt_simple);
  handler->reg_handler("k_volt_min_period",                      flt_simple);
  handler->reg_handler("k_volt_min_pulse_width_high",            flt_simple);
  handler->reg_handler("k_volt_min_pulse_width_low",             flt_simple);
  handler->reg_handler("k_volt_nochange_fall",                   flt_simple);
  handler->reg_handler("k_volt_nochange_rise",                   flt_simple);
  handler->reg_handler("k_volt_pin_cap",                         flt_simple);
  handler->reg_handler("k_volt_recovery_fall",                   flt_simple);
  handler->reg_handler("k_volt_recovery_rise",                   flt_simple);
  handler->reg_handler("k_volt_removal_fall",                    flt_simple);
  handler->reg_handler("k_volt_removal_rise",                    flt_simple);
  handler->reg_handler("k_volt_rise_delay_intercept",            flt_simple);
  handler->reg_handler("k_volt_rise_pin_resistance",             flt_simple);
  handler->reg_handler("k_volt_rise_propagation",                flt_simple);
  handler->reg_handler("k_volt_rise_transition",                 flt_simple);
  handler->reg_handler("k_volt_setup_fall",                      flt_simple);
  handler->reg_handler("k_volt_setup_rise",                      flt_simple);
  handler->reg_handler("k_volt_skew_fall",                       flt_simple);
  handler->reg_handler("k_volt_skew_rise",                       flt_simple);
  handler->reg_handler("k_volt_slope_fall",                      flt_simple);
  handler->reg_handler("k_volt_slope_rise",                      flt_simple);
  handler->reg_handler("k_volt_wire_cap",                        flt_simple);
  handler->reg_handler("k_volt_wire_res",                        flt_simple);

  // complex attributes
  DotlibHandler* complex = new ComplexHandler(handler);
  DotlibHandler* str1_complex = new Str1ComplexHandler(handler);
  DotlibHandler* unit_complex = new UnitComplexHandler(handler);
  DotlibHandler* define = new DefineHandler(handler);
  handler->reg_handler("capacitive_load_unit",                   unit_complex);
  handler->reg_handler("default_part",                           complex);
  handler->reg_handler("define",                                 define);
  handler->reg_handler("define_cell_area",                       complex);
  handler->reg_handler("define_group",                           complex);
  handler->reg_handler("library_features",                       str1_complex);
  handler->reg_handler("piece_define",                           str1_complex);
  handler->reg_handler("routing_layers",                         complex);
  handler->reg_handler("technology",                             str1_complex);

  //fall_net_delay : name ;
  //rise_net_delay : name ;

  // group statements
  handler->reg_handler("cell",
		       new_cell(handler));
  handler->reg_handler("dc_current_template",
		       new_template(handler));
  handler->reg_handler("em_lut_template",
		       new_template(handler));
  handler->reg_handler("fall_transition_degradation",
		       new_group(handler));
  handler->reg_handler("faults_lut_template",
		       new_template(handler));
  handler->reg_handler("input_voltage",
		       new_input_voltage(handler));
  handler->reg_handler("iv_lut_template",
		       new_template(handler));
  handler->reg_handler("lu_table_template",
		       new_template(handler));
  handler->reg_handler("noise_lut_template",
		       new_template(handler));
  handler->reg_handler("operating_conditions",
		       new_operating_conditions(handler));
  handler->reg_handler("output_voltage",
		       new_output_voltage(handler));
  handler->reg_handler("output_current_template",
		       new_template(handler));
  handler->reg_handler("part",
		       new_group(handler));
  handler->reg_handler("poly_template",
		       new_template(handler));
  handler->reg_handler("power_lut_template",
		       new_template(handler));
  handler->reg_handler("power_poly_template",
		       new_template(handler));
  handler->reg_handler("power_supply",
		       new_group(handler));
  handler->reg_handler("propagation_lut_template",
		       new_template(handler));
  handler->reg_handler("rise_transition_degradation",
		       new_group(handler));
  handler->reg_handler("scaled_cell",
		       new_group(handler));
  handler->reg_handler("scaling_factors",
		       new_group(handler));
  handler->reg_handler("timing",
		       new_group(handler));
  handler->reg_handler("timing_range",
		       new_group(handler));
  handler->reg_handler("type",
		       new_group(handler));
  handler->reg_handler("wire_load",
		       new_wire_load(handler));
  handler->reg_handler("wire_load_selection",
		       new_wire_load_selection(handler));
  handler->reg_handler("wire_load_table",
		       new_wire_load_table(handler));

  return handler;
}

// @brief input_voltage group 用のハンドラを作る．
// @param[in] parent 親のハンドラ
DotlibHandler*
HandlerFactory::new_input_voltage(GroupHandler* parent)
{
  GroupHandler* handler = new Str1GroupHandler(parent);

  // simple attributes
  DotlibHandler* expr_handler = new ExprHandler(handler);
  handler->reg_handler("vil",   expr_handler);
  handler->reg_handler("vih",   expr_handler);
  handler->reg_handler("vimin", expr_handler);
  handler->reg_handler("vimax", expr_handler);

  return handler;
}

// @brief output_voltage group 用のハンドラを作る．
// @param[in] parent 親のハンドラ
DotlibHandler*
HandlerFactory::new_output_voltage(GroupHandler* parent)
{
  GroupHandler* handler = new Str1GroupHandler(parent);

  // simple attributes
  DotlibHandler* expr_handler = new ExprHandler(handler);
  handler->reg_handler("vol",   expr_handler);
  handler->reg_handler("voh",   expr_handler);
  handler->reg_handler("vomin", expr_handler);
  handler->reg_handler("vomax", expr_handler);

  return handler;
}

// @brief operating_conditions group 用のハンドラを作る．
// @param[in] parent 親のハンドラ
DotlibHandler*
HandlerFactory::new_operating_conditions(GroupHandler* parent)
{
  GroupHandler* handler = new Str1GroupHandler(parent);

  // simple attributes
  DotlibHandler* simple = new SimpleHandler(handler, false);
  DotlibHandler* str_simple = new StrSimpleHandler(handler, false);
  DotlibHandler* flt_simple = new FloatSimpleHandler(handler);
  handler->reg_handler("calc_mode",   simple);
  handler->reg_handler("parameter1",  simple);
  handler->reg_handler("parameter2",  simple);
  handler->reg_handler("parameter3",  simple);
  handler->reg_handler("parameter4",  simple);
  handler->reg_handler("parameter5",  simple);
  handler->reg_handler("process",     flt_simple);
  handler->reg_handler("temperature", flt_simple);
  handler->reg_handler("tree_type",   str_simple);
  handler->reg_handler("voltage",     flt_simple);

  // complex attributes
  DotlibHandler* complex = new ComplexHandler(handler);
  handler->reg_handler("power_rail", complex);

  return handler;
}

// @brief template group 用のハンドラを作る．
// @param[in] parent 親のハンドラ
DotlibHandler*
HandlerFactory::new_template(GroupHandler* parent)
{
  GroupHandler* handler = new Str1GroupHandler(parent);

  // simple attributes
  DotlibHandler* str_simple = new StrSimpleHandler(handler, false);
  handler->reg_handler("variable_1", str_simple);
  handler->reg_handler("variable_2", str_simple);
  handler->reg_handler("variable_3", str_simple);

  // complex attributes
  DotlibHandler* index_handler = new VectorComplexHandler(handler);
  handler->reg_handler("index_1",    index_handler);
  handler->reg_handler("index_2",    index_handler);
  handler->reg_handler("index_3",    index_handler);

  // group statements
  handler->reg_handler("domain", new_group(handler));

  return handler;
}

// @brief wire_load group 用のハンドラを作る．
// @param[in] parent 親のハンドラ
DotlibHandler*
HandlerFactory::new_wire_load(GroupHandler* parent)
{
  GroupHandler* handler = new Str1GroupHandler(parent);

  // simple attributes
  DotlibHandler* flt_simple = new FloatSimpleHandler(handler);
  handler->reg_handler("area",        flt_simple);
  handler->reg_handler("capacitance", flt_simple);
  handler->reg_handler("resistance",  flt_simple);
  handler->reg_handler("slope",       flt_simple);

  // complex attributes
  DotlibHandler* complex = new ComplexHandler(handler);
  handler->reg_handler("fanout_length", complex);

  return handler;
}

// @brief wire_load_selection group 用のハンドラを作る．
// @param[in] parent 親のハンドラ
DotlibHandler*
HandlerFactory::new_wire_load_selection(GroupHandler* parent)
{
  GroupHandler* handler = new GroupHandler(parent);

  // complex attributes
  DotlibHandler* complex = new ComplexHandler(handler);
  handler->reg_handler("wire_load_from_area", complex);

  return handler;
}

// @brief wire_load_table group 用のハンドラを作る．
// @param[in] parent 親のハンドラ
DotlibHandler*
HandlerFactory::new_wire_load_table(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // complex attributes
  DotlibHandler* complex = new ComplexHandler(handler);
  handler->reg_handler("fanout_area",        complex);
  handler->reg_handler("fanout_capacitance", complex);
  handler->reg_handler("fanout_length",      complex);
  handler->reg_handler("fanout_resistance",  complex);

  return handler;
}

// @brief cell group 用のハンドラを作る．
// @param[in] parent 親のハンドラ
DotlibHandler*
HandlerFactory::new_cell(GroupHandler* parent)
{
  GroupHandler* handler = new Str1GroupHandler(parent);

  // simple attributes
  DotlibHandler* simple = new SimpleHandler(handler, false);
  DotlibHandler* str_simple = new StrSimpleHandler(handler, false);
  DotlibHandler* flt_simple = new FloatSimpleHandler(handler);
  handler->reg_handler("area",                         flt_simple);
  handler->reg_handler("auxiliary_pad_cell",           simple);
  handler->reg_handler("base_name",                    str_simple);
  handler->reg_handler("bus_naming_style",             str_simple);
  handler->reg_handler("cell_footprint",               str_simple);
  handler->reg_handler("cell_leakage_power",           simple);
  handler->reg_handler("clock_gating_integrated_cell", simple);
  handler->reg_handler("contention_condition",         simple);
  handler->reg_handler("dont_fault",                   simple);
  handler->reg_handler("dont_touch",                   simple);
  handler->reg_handler("dont_use",                     simple);
  handler->reg_handler("driver_type",                  simple);
  handler->reg_handler("edif_name",                    simple);
  handler->reg_handler("em_temp_degradation_factor",   simple);
  handler->reg_handler("fpga_domain_style",            simple);
  handler->reg_handler("geometry_print",               simple);
  handler->reg_handler("handle_negative_constraint",   simple);
  handler->reg_handler("interface_timing",             simple);
  handler->reg_handler("io_type",                      simple);
  handler->reg_handler("is_clock_gating_cell",         simple);
  handler->reg_handler("map_only",                     simple);
  handler->reg_handler("pad_cell",                     simple);
  handler->reg_handler("pad_type",                     simple);
  handler->reg_handler("power_cell_type",              simple);
  handler->reg_handler("preferred",                    simple);
  handler->reg_handler("scaling_factors",              simple);
  handler->reg_handler("single_bit_degenerate",        simple);
  handler->reg_handler("slew_type",                    simple);
  handler->reg_handler("timing_model_type",            simple);
  handler->reg_handler("use_for_size_only",            simple);
  handler->reg_handler("vhdl_name",                    simple);

  handler->reg_handler("is_filler_cell",               simple);

  // complex attributes
  DotlibHandler* complex = new ComplexHandler(handler);
  handler->reg_handler("pin_opposite",                 complex);
  handler->reg_handler("rail_connection",              complex);
  handler->reg_handler("power_supply_namestring",      complex);
  handler->reg_handler("resource_usage",               complex);

  // group statements
  handler->reg_handler("bus",                          new_bus(handler));
  handler->reg_handler("bundle",                       new_bundle(handler));
  handler->reg_handler("dynamic_current",              new_group(handler));
  handler->reg_handler("ff",                           new_ff(handler));
  handler->reg_handler("ff_bank",                      new_ff_bank(handler));
  handler->reg_handler("functional_yield_metric",      new_group(handler));
  handler->reg_handler("generated_clock",              new_group(handler));
  handler->reg_handler("intrinsic_parasitic",          new_group(handler));
  handler->reg_handler("latch",                        new_latch(handler));
  handler->reg_handler("latch_bank",                   new_latch_bank(handler));
  handler->reg_handler("leakage_current",              new_group(handler));
  handler->reg_handler("leakage_power",                new_leakage_power(handler));
  handler->reg_handler("lut",                          new_group(handler));
  handler->reg_handler("mode_definition",              new_group(handler));
  handler->reg_handler("pin",                          new_pin(handler));
  handler->reg_handler("routing_track",                new_group(handler));
  handler->reg_handler("statetable",                   new_statetable(handler));

  handler->reg_handler("internal_power", new_cell_internal_power(handler));

  handler->reg_handler("test_cell",                    new_test_cell(handler));

  return handler;
}

// @brief leakage_power group 用のハンドラを作る．
// @param[in] parent 親のハンドラ
DotlibHandler*
HandlerFactory::new_leakage_power(GroupHandler* parent)
{
  GroupHandler* handler = new EmptyGroupHandler(parent);

  // simple attributes
  DotlibHandler* simple = new SimpleHandler(handler, false);
  DotlibHandler* expr_handler = new StrSimpleHandler(handler, false);
  DotlibHandler* flt_simple = new FloatSimpleHandler(handler);
  handler->reg_handler("power_level",    simple);
  handler->reg_handler("related_pg_pin", simple);
  handler->reg_handler("when",           expr_handler);
  handler->reg_handler("value",          flt_simple);

  return handler;
}

// @brief test_cell group 用のハンドラを作る．
// @param[in] parent 親のハンドラ
DotlibHandler*
HandlerFactory::new_test_cell(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // group statements
  handler->reg_handler("bus", new_bus(handler));
  handler->reg_handler("dynamic_current", new_group(handler));
  handler->reg_handler("ff", new_ff(handler));
  handler->reg_handler("ff_bank", new_ff_bank(handler));
  handler->reg_handler("functional_yield_metric", new_group(handler));
  handler->reg_handler("generated_clock", new_group(handler));
  handler->reg_handler("intrinsic_parasitic", new_group(handler));
  handler->reg_handler("latch", new_latch(handler));
  handler->reg_handler("latch_bank", new_latch_bank(handler));
  handler->reg_handler("leakage_current", new_group(handler));
  handler->reg_handler("leakage_power", new_leakage_power(handler));
  handler->reg_handler("lut", new_group(handler));
  handler->reg_handler("mode_definition", new_group(handler));
  handler->reg_handler("pin", new_pin(handler));
  handler->reg_handler("statetable", new_statetable(handler));

  return handler;
}

// @brief ff group 用のハンドラを作る．
// @param[in] parent 親のハンドラ
DotlibHandler*
HandlerFactory::new_ff(GroupHandler* parent)
{
  GroupHandler* handler = new Str2GroupHandler(parent);

  // simple attributes
  DotlibHandler* str_simple = new StrSimpleHandler(handler, false);
  DotlibHandler* fhandler = new FuncHandler(handler);
  handler->reg_handler("clear",             fhandler);
  handler->reg_handler("clear_preset_var1", str_simple);
  handler->reg_handler("clear_preset_var2", str_simple);
  handler->reg_handler("clocked_on",        fhandler);
  handler->reg_handler("clocked_on_also",   fhandler);
  handler->reg_handler("next_state",        fhandler);
  handler->reg_handler("preset",            fhandler);

  return handler;
}

// @brief ff_bank group 用のハンドラを作る．
// @param[in] parent 親のハンドラ
DotlibHandler*
HandlerFactory::new_ff_bank(GroupHandler* parent)
{
  GroupHandler* handler = new Str2IntGroupHandler(parent);

  // simple attributes
  DotlibHandler* str_simple = new StrSimpleHandler(handler, false);
  DotlibHandler* fhandler = new FuncHandler(handler);
  handler->reg_handler("clear",             fhandler);
  handler->reg_handler("clear_preset_var1", str_simple);
  handler->reg_handler("clear_preset_var2", str_simple);
  handler->reg_handler("clocked_on",        fhandler);
  handler->reg_handler("clocked_on_also",   fhandler);
  handler->reg_handler("next_state",        fhandler);
  handler->reg_handler("preset",            fhandler);

  return handler;
}

// @brief latch group 用のハンドラを作る．
// @param[in] parent 親のハンドラ
DotlibHandler*
HandlerFactory::new_latch(GroupHandler* parent)
{
  GroupHandler* handler = new Str2GroupHandler(parent);

  // simple attributes
  DotlibHandler* str_simple = new StrSimpleHandler(handler, false);
  DotlibHandler* fhandler = new FuncHandler(handler);
  handler->reg_handler("clear",             fhandler);
  handler->reg_handler("clear_preset_var1", str_simple);
  handler->reg_handler("clear_preset_var2", str_simple);
  handler->reg_handler("data_in",           fhandler);
  handler->reg_handler("enable",            fhandler);
  handler->reg_handler("enable_also",       fhandler);
  handler->reg_handler("preset",            fhandler);

  return handler;
}

// @brief latch_bank group 用のハンドラを作る．
// @param[in] parent 親のハンドラ
DotlibHandler*
HandlerFactory::new_latch_bank(GroupHandler* parent)
{
  GroupHandler* handler = new Str2IntGroupHandler(parent);

  // simple attributes
  DotlibHandler* str_simple = new StrSimpleHandler(handler, false);
  DotlibHandler* fhandler = new FuncHandler(handler);
  handler->reg_handler("clear",             fhandler);
  handler->reg_handler("clear_preset_var1", str_simple);
  handler->reg_handler("clear_preset_var2", str_simple);
  handler->reg_handler("data_in",           fhandler);
  handler->reg_handler("enable",            fhandler);
  handler->reg_handler("enable_also",       fhandler);
  handler->reg_handler("preset",            fhandler);

  return handler;
}

// @brief cell 内の internal_power group 用のハンドラを作る．
DotlibHandler*
HandlerFactory::new_cell_internal_power(GroupHandler* parent)
{
  GroupHandler* handler = new Str1GroupHandler(parent);

  // simple attributes
  DotlibHandler* simple = new SimpleHandler(handler, false);
  handler->reg_handler("related_inputs", simple);
  handler->reg_handler("related_outputs", simple);

  // complex attribute
  DotlibHandler* complex = new ComplexHandler(handler);
  handler->reg_handler("values", complex);

  return handler;
}

// @brief power group 用のハンドラを作る．
// @param[in] parent 親のハンドラ
DotlibHandler*
HandlerFactory::new_power(GroupHandler* parent)
{
  GroupHandler* handler = new Str1GroupHandler(parent);

  // simple attributes

  // complex attribute
  DotlibHandler* index_handler = new VectorComplexHandler(handler);
  DotlibHandler* value_handler = new VectorListComplexHandler(handler);
  DotlibHandler* complex = new ComplexHandler(handler);
  handler->reg_handler("index_1", index_handler);
  handler->reg_handler("index_2", index_handler);
  handler->reg_handler("index_3", index_handler);
  handler->reg_handler("values",  value_handler);

  handler->reg_handler("orders",  complex);
  handler->reg_handler("coefs",   complex);

  // group statements
  handler->reg_handler("domain", new_group(handler));

  return handler;
}

// @brief statetable group 用のハンドラを作る．
// @param[in] parent 親のハンドラ
DotlibHandler*
HandlerFactory::new_statetable(GroupHandler* parent)
{
  GroupHandler* handler = new Str2GroupHandler(parent);

  // simple attributes
  DotlibHandler* simple = new StrSimpleHandler(handler, false);
  handler->reg_handler("table", simple);

  return handler;
}

// @brief bus group 用のハンドラを作る．
// @param[in] parent 親のハンドラ
DotlibHandler*
HandlerFactory::new_bus(GroupHandler* parent)
{
  GroupHandler* handler = new Str1GroupHandler(parent);

  // simple attributes
  DotlibHandler* simple = new StrSimpleHandler(handler, false);
  handler->reg_handler("bus_type", simple);

  // group statements
  handler->reg_handler("pin", new_pin(handler));

  return handler;
}

// @brief bundle group 用のハンドラを作る．
// @param[in] parent 親のハンドラ
DotlibHandler*
HandlerFactory::new_bundle(GroupHandler* parent)
{
  GroupHandler* handler = new Str1GroupHandler(parent);

  // simple attributes
  DotlibHandler* simple = new SimpleHandler(handler, false);
  DotlibHandler* flt_simple = new FloatSimpleHandler(handler);
  handler->reg_handler("capacitance", flt_simple);
  handler->reg_handler("direction",   simple);
  handler->reg_handler("function",    new FuncHandler(handler));

  // complex attributes
  DotlibHandler* complex = new ComplexHandler(handler);
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

// @brief pin group 用のハンドラを作る．
// @param[in] parent 親のハンドラ
DotlibHandler*
HandlerFactory::new_pin(GroupHandler* parent)
{
  GroupHandler* handler = new Str1GroupHandler(parent);

  // simple attributes
  DotlibHandler* simple = new SimpleHandler(handler, false);
  DotlibHandler* str_simple = new StrSimpleHandler(handler, false);
  DotlibHandler* flt_simple = new FloatSimpleHandler(handler);
  DotlibHandler* func_handler = new FuncHandler(handler);
  handler->reg_handler("bit_width", simple);
  handler->reg_handler("capacitance",                         flt_simple);
  handler->reg_handler("clock",                               simple);
  handler->reg_handler("clock_gate_clock_pin",                simple);
  handler->reg_handler("clock_gate_enable_pin",               simple);
  handler->reg_handler("clock_gate_test_pin",                 simple);
  handler->reg_handler("clock_gate_obs_pin",                  simple);
  handler->reg_handler("clock_gate_out_pin",                  simple);
  handler->reg_handler("complementary_pin",                   simple);
  handler->reg_handler("connection_class",                    simple);
  handler->reg_handler("direction",                           str_simple);
  handler->reg_handler("dont_fault",                          simple);
  handler->reg_handler("drive_current",                       simple);
  handler->reg_handler("driver_type",                         simple);
  handler->reg_handler("fall_capacitance",                    simple);
  handler->reg_handler("fall_current_slope_after_threshold",  simple);
  handler->reg_handler("fall_current_slope_before_threshold", simple);
  handler->reg_handler("fall_time_after_threshold",           simple);
  handler->reg_handler("fall_time_before_threshold",          simple);
  handler->reg_handler("fanout_load",                         flt_simple);
  handler->reg_handler("fault_model",                         simple);
  handler->reg_handler("function",                            func_handler);
  handler->reg_handler("has_builtin_pad",                     simple);
  handler->reg_handler("hysteresis",                          simple);
  handler->reg_handler("input_map",                           simple);
  handler->reg_handler("input_signal_level",                  simple);
  handler->reg_handler("input_voltage",                       simple);
  handler->reg_handler("internal_node",                       simple);
  handler->reg_handler("inverted_output",                     simple);
  handler->reg_handler("is_pad",                              simple);
  handler->reg_handler("max_capacitance",                     flt_simple);
  handler->reg_handler("max_fanout",                          flt_simple);
  handler->reg_handler("max_input_noise_width",               simple);
  handler->reg_handler("max_transition",                      flt_simple);
  handler->reg_handler("min_capacitance",                     flt_simple);
  handler->reg_handler("min_fanout",                          flt_simple);
  handler->reg_handler("min_input_noise_width",               simple);
  handler->reg_handler("min_period",                          simple);
  handler->reg_handler("min_pulse_width_high",                simple);
  handler->reg_handler("min_pulse_width_low",                 simple);
  handler->reg_handler("min_transition",                      flt_simple);
  handler->reg_handler("multicell_pad_pin",                   simple);
  handler->reg_handler("nextstate_type",                      simple);
  handler->reg_handler("output_signal_level",                 simple);
  handler->reg_handler("output_voltage",                      flt_simple);
  handler->reg_handler("pin_func_type",                       simple);
  handler->reg_handler("prefer_tied",                         simple);
  handler->reg_handler("primary_output",                      simple);
  handler->reg_handler("pulling_current",                     flt_simple);
  handler->reg_handler("pulling_resistance",                  flt_simple);
  handler->reg_handler("rise_capacitance",                    flt_simple);
  handler->reg_handler("rise_current_slope_after_threshold",  flt_simple);
  handler->reg_handler("rise_current_slope_before_threshold", flt_simple);
  handler->reg_handler("rise_time_after_threshold",           flt_simple);
  handler->reg_handler("rise_time_before_threshold",          flt_simple);
  handler->reg_handler("signal_type",                         simple);
  handler->reg_handler("slew_control",                        simple);
  handler->reg_handler("state_function",                      simple);
  handler->reg_handler("test_output_only",                    simple);
  handler->reg_handler("three_state",
		       new FuncHandler(handler));
  handler->reg_handler("vhdl_name",                           str_simple);
  handler->reg_handler("x_function",                          simple);

  // complex attributes
  DotlibHandler* complex = new ComplexHandler(handler);
  handler->reg_handler("fall_capacitance_range",              complex);
  handler->reg_handler("rise_capacitance_range",              complex);

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

// @brief internal_power group 用のハンドラを作る．
// @param[in] parent 親のハンドラ
DotlibHandler*
HandlerFactory::new_internal_power(GroupHandler* parent)
{
  GroupHandler* handler = new EmptyGroupHandler(parent);

  // simple attributes
  DotlibHandler* simple = new SimpleHandler(handler, false);
  handler->reg_handler("equal_or_opposite_output", simple);
  handler->reg_handler("falling_together_group",   simple);
  handler->reg_handler("power_level",              simple);
  handler->reg_handler("related_pin",              simple);
  handler->reg_handler("rising_together_group",    simple);
  handler->reg_handler("switching_interval",       simple);
  handler->reg_handler("switching_together_group", simple);
  handler->reg_handler("when",                     simple);

  // complex attribute

  // group statements
  handler->reg_handler("power",                    new_power(handler));
  handler->reg_handler("fall_power",               new_power(handler));
  handler->reg_handler("rise_power",               new_power(handler));
  handler->reg_handler("domain",                   new_group(handler));

  return handler;
}

// @brief timing group 用のハンドラを作る．
// @param[in] parent 親のハンドラ
DotlibHandler*
HandlerFactory::new_timing(GroupHandler* parent)
{
  GroupHandler* handler = new EmptyGroupHandler(parent);

  // simple attributes
  DotlibHandler* simple = new SimpleHandler(handler, false);
  DotlibHandler* str_simple = new StrSimpleHandler(handler, false);
  DotlibHandler* flt_simple = new FloatSimpleHandler(handler);

  handler->reg_handler("related_bus_equivalent",   str_simple);
  handler->reg_handler("related_bus_pins",         str_simple);
  handler->reg_handler("related_output_pin",       str_simple);
  handler->reg_handler("related_pin",              str_simple);

  handler->reg_handler("timing_sense",             str_simple);
  handler->reg_handler("timing_type",              str_simple);

  handler->reg_handler("edge_rate_sensitivity_f0", simple);
  handler->reg_handler("edge_rate_sensitivity_f1", simple);
  handler->reg_handler("edge_rate_sensitivity_r0", simple);
  handler->reg_handler("edge_rate_sensitivity_r1", simple);

  handler->reg_handler("rise_resistance",          flt_simple);
  handler->reg_handler("fall_resistance",          flt_simple);
  handler->reg_handler("intrinsic_rise",           flt_simple);
  handler->reg_handler("intrinsic_fall",           flt_simple);

  handler->reg_handler("sdf_cond",                 simple);
  handler->reg_handler("sdf_cond_end",             simple);
  handler->reg_handler("sdf_cond_start",           simple);
  handler->reg_handler("sdf_edges",                simple);

  handler->reg_handler("slope_fall",               flt_simple);
  handler->reg_handler("slope_rise",               flt_simple);

  handler->reg_handler("when",                     str_simple);
  handler->reg_handler("when_end",                 str_simple);
  handler->reg_handler("when_start",               str_simple);

  // complex attribute
  DotlibHandler* complex = new ComplexHandler(handler);
  DotlibHandler* pw_complex = new PwComplexHandler(handler);
  handler->reg_handler("rise_delay_intercept",     pw_complex);
  handler->reg_handler("fall_delay_intercept",     pw_complex);
  handler->reg_handler("rise_pin_resistance",      pw_complex);
  handler->reg_handler("fall_pin_resistance",      pw_complex);
  handler->reg_handler("orders",                   complex);
  handler->reg_handler("coefs",                    complex);

  // group statements
  handler->reg_handler("cell_degradation",
		       new_group(handler));

  handler->reg_handler("cell_rise",
		       new_table(handler));
  handler->reg_handler("cell_fall",
		       new_table(handler));

  handler->reg_handler("rise_constraint",
		       new_table(handler));
  handler->reg_handler("fall_constraint",
		       new_table(handler));

  handler->reg_handler("rise_propagation",
		       new_table(handler));
  handler->reg_handler("fall_propagation",
		       new_table(handler));

  handler->reg_handler("rise_transition",
		       new_table(handler));
  handler->reg_handler("fall_transition",
		       new_table(handler));

  handler->reg_handler("noise_immunity_above_high",
		       new_group(handler));
  handler->reg_handler("noise_immunity_below_low",
		       new_group(handler));
  handler->reg_handler("noise_immunity_high",
		       new_group(handler));
  handler->reg_handler("noise_immunity_low",
		       new_group(handler));

  handler->reg_handler("propogated_noise_height_above_high",
		       new_group(handler));
  handler->reg_handler("propogated_noise_height_below_low",
		       new_group(handler));
  handler->reg_handler("propogated_noise_height_high",
		       new_group(handler));
  handler->reg_handler("propogated_noise_height_low",
		       new_group(handler));
  handler->reg_handler("propogated_noise_peak_time_ratio_above_high",
		       new_group(handler));
  handler->reg_handler("propogated_noise_peak_time_ratio__below_low",
		       new_group(handler));
  handler->reg_handler("propogated_noise_peak_time_ratio_high",
		       new_group(handler));
  handler->reg_handler("propogated_noise_peak_time_ratio_low",
		       new_group(handler));
  handler->reg_handler("propogated_noise_width_above_high",
		       new_group(handler));
  handler->reg_handler("propogated_noise_width_below_low",
		       new_group(handler));
  handler->reg_handler("propogated_noise_width_high",
		       new_group(handler));
  handler->reg_handler("propogated_noise_width_low",
		       new_group(handler));

  handler->reg_handler("retaining_rise",
		       new_group(handler));
  handler->reg_handler("retaining_fall",
		       new_group(handler));

  handler->reg_handler("retain_fall_slew",
		       new_group(handler));
  handler->reg_handler("retain_rise_slew",
		       new_group(handler));

  handler->reg_handler("steady_state_current_high",
		       new_group(handler));
  handler->reg_handler("steady_state_current_low",
		       new_group(handler));
  handler->reg_handler("steady_state_current_tristate",
		       new_group(handler));

  return handler;
}

// @brief table group 用のハンドラを作る．
// @param[in] parent 親のハンドラ
DotlibHandler*
HandlerFactory::new_table(GroupHandler* parent)
{
  GroupHandler* handler = new Str1GroupHandler(parent);

  // simple attributes

  // complex attribute
  DotlibHandler* index_handler = new VectorComplexHandler(handler);
  DotlibHandler* value_handler = new VectorListComplexHandler(handler);
  handler->reg_handler("index_1", index_handler);
  handler->reg_handler("index_2", index_handler);
  handler->reg_handler("index_3", index_handler);
  handler->reg_handler("values",  value_handler);

  // group statements
  handler->reg_handler("domain", new_group(handler));

  return handler;
}

// @brief 汎用の group 用のハンドラを作る．
GroupHandler*
HandlerFactory::new_group(GroupHandler* parent)
{
  return new GroupHandler(parent);
}

END_NAMESPACE_YM_DOTLIB
