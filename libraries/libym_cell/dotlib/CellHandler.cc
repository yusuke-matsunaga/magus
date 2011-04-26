
/// @file libym_cell/dotlib/CellHandler.cc
/// @brief CellHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CellHandler.h"

#include "SimpleHandler.h"
#include "ComplexHandler.h"
#include "GroupHandler.h"

#include "PtMgr.h"
#include "PtNode.h"
#include "PtCell.h"
#include "PtValue.h"

#include "ym_utils/FileRegion.h"


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
new_ff(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);


  // simple attributes
  DotlibHandler* simple = new_simple(handler);
  handler->reg_handler("clear", simple);
  handler->reg_handler("clear_preset_var1", simple);
  handler->reg_handler("clear_preset_var2", simple);
  handler->reg_handler("clocked_on", simple);
  handler->reg_handler("clocked_on_also", simple);
  handler->reg_handler("next_state", simple);
  handler->reg_handler("preset", simple);

  return handler;
}

DotlibHandler*
new_latch(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // simple attributes
  DotlibHandler* simple = new_simple(handler);
  handler->reg_handler("clear", simple);
  handler->reg_handler("clear_preset_var1", simple);
  handler->reg_handler("clear_preset_var2", simple);
  handler->reg_handler("data_in", simple);
  handler->reg_handler("enable", simple);
  handler->reg_handler("enable_also", simple);
  handler->reg_handler("preset", simple);

  return handler;
}

DotlibHandler*
new_power(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // simple attributes

  // complex attribute
  DotlibHandler* complex = new_complex(handler);
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

DotlibHandler*
new_internal_power(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // simple attributes
  DotlibHandler* simple = new_simple(handler);
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

DotlibHandler*
new_tbl(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // simple attributes

  // complex attribute
  DotlibHandler* complex = new_complex(handler);
  handler->reg_handler("index_1", complex);
  handler->reg_handler("index_2", complex);
  handler->reg_handler("index_3", complex);
  handler->reg_handler("values", complex);

  // group statements
  handler->reg_handler("domain", new_group(handler));

  return handler;
}

DotlibHandler*
new_timing(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // simple attributes
  DotlibHandler* simple = new_simple(handler);
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
  DotlibHandler* complex = new_complex(handler);
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

DotlibHandler*
new_pin(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // simple attributes
  DotlibHandler* simple = new_simple(handler);
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
  DotlibHandler* complex = new_complex(handler);
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

DotlibHandler*
new_bus(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // simple attributes
  DotlibHandler* simple = new_simple(handler);
  handler->reg_handler("bus_type", simple);

  // group statements
  handler->reg_handler("pin", new_pin(handler));

  return handler;
}

DotlibHandler*
new_bundle(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // simple attributes
  DotlibHandler* simple = new_simple(handler);
  handler->reg_handler("capacitance", simple);
  handler->reg_handler("direction", simple);
  handler->reg_handler("function", simple);

  // complex attributes
  DotlibHandler* complex = new_complex(handler);
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

DotlibHandler*
new_cell_internal_power(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // simple attributes
  DotlibHandler* simple = new_simple(handler);
  handler->reg_handler("related_inputs", simple);
  handler->reg_handler("related_outputs", simple);

  // complex attribute
  DotlibHandler* complex = new_complex(handler);
  handler->reg_handler("values", complex);

  return handler;
}

DotlibHandler*
new_statetable(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);


  // simple attributes
  DotlibHandler* simple = new_simple(handler);
  handler->reg_handler("table", simple);

  return handler;
}

DotlibHandler*
new_leakage_power(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);

  // simple attributes
  DotlibHandler* simple = new_simple(handler);
  handler->reg_handler("power_level", simple);
  handler->reg_handler("related_pg_pin", simple);
  handler->reg_handler("when", simple);
  handler->reg_handler("value", simple);

  return handler;
}

DotlibHandler*
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

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス CellHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
CellHandler::CellHandler(GroupHandler* parent) :
  GroupHandler(parent),
  mCell(NULL)
{
  // simple attributes
  DotlibHandler* simple = new SimpleHandler(this);
  reg_handler("area", simple);
  reg_handler("auxiliary_pad_cell", simple);
  reg_handler("base_name", simple);
  reg_handler("bus_naming_style", simple);
  reg_handler("cell_footprint", simple);
  reg_handler("cell_leakage_power", simple);
  reg_handler("clock_gating_integrated_cell", simple);
  reg_handler("contention_condition", simple);
  reg_handler("dont_fault", simple);
  reg_handler("dont_touch", simple);
  reg_handler("dont_use", simple);
  reg_handler("driver_type", simple);
  reg_handler("edif_name", simple);
  reg_handler("em_temp_degradation_factor", simple);
  reg_handler("fpga_domain_style", simple);
  reg_handler("geometry_print", simple);
  reg_handler("handle_negative_constraint", simple);
  reg_handler("interface_timing", simple);
  reg_handler("io_type", simple);
  reg_handler("is_clock_gating_cell", simple);
  reg_handler("map_only", simple);
  reg_handler("pad_cell", simple);
  reg_handler("pad_type", simple);
  reg_handler("power_cell_type", simple);
  reg_handler("preferred", simple);
  reg_handler("scaling_factors", simple);
  reg_handler("single_bit_degenerate", simple);
  reg_handler("slew_type", simple);
  reg_handler("timing_model_type", simple);
  reg_handler("use_for_size_only", simple);
  reg_handler("vhdl_name", simple);

  reg_handler("is_filler_cell", simple);

  // complex attributes
  DotlibHandler* complex = new ComplexHandler(this);
  reg_handler("pin_opposite", complex);
  reg_handler("rail_connection", complex);
  reg_handler("power_supply_namestring", complex);
  reg_handler("resource_usage", complex);

  // group statements
  reg_handler("bus", new_bus(this));
  reg_handler("bundle", new_bundle(this));
  reg_handler("dynamic_current", new_group(this));
  reg_handler("ff", new_ff(this));
  reg_handler("ff_bank", new_ff(this));
  reg_handler("functional_yield_metric", new_group(this));
  reg_handler("generated_clock", new_group(this));
  reg_handler("intrinsic_parasitic", new_group(this));
  reg_handler("latch", new_latch(this));
  reg_handler("latch_bank", new_latch(this));
  reg_handler("leakage_current", new_group(this));
  reg_handler("leakage_power", new_leakage_power(this));
  reg_handler("lut", new_group(this));
  reg_handler("mode_definition", new_group(this));
  reg_handler("pin", new_pin(this));
  reg_handler("routing_track", new_group(this));
  reg_handler("statetable", new_statetable(this));

  reg_handler("internal_power", new_cell_internal_power(this));

  reg_handler("test_cell", new_test_cell(this));
}

// @brief デストラクタ
CellHandler::~CellHandler()
{
}

#if 0
// @brief 対応する PtNode を返す．
PtNode*
CellHandler::pt_node()
{
  return mCell;
}

// @brief group statement の最初に呼ばれる関数
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value_list 値を表すトークンのリスト
bool
CellHandler::begin_group(const ShString& attr_name,
			 const FileRegion& attr_loc,
			 const vector<const PtValue*>& value_list)
{
  if ( value_list.size() != 1 ) {
    FileRegion loc;
    if ( value_list.size() > 1 ) {
      loc = value_list[1]->loc();
    }
    else {
      loc = attr_loc;
    }
    put_msg(__FILE__, __LINE__, loc,
	    kMsgError,
	    "DOTLIBPARSER",
	    "cell group requires just one string as a parameter.");
    return false;
  }

  if ( value_list[0]->type() != SYMBOL ) {
    put_msg(__FILE__, __LINE__, value_list[0]->loc(),
	    kMsgError,
	    "DOTLIBPARSER",
	    "string value is exprected.");
    return false;
  }
  mCell = ptmgr().new_ptcell(attr_name, attr_loc, value_list[0]);
  parent()->pt_node()->add_child(mCell);

  return true;
}

// @brief group statement の最後に呼ばれる関数
bool
CellHandler::end_group()
{
  mCell = NULL;
  return true;
}
#endif


END_NAMESPACE_YM_CELL_DOTLIB
