
/// @file libym_cell/dotlib/PinHandler.cc
/// @brief PinHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PinHandler.h"

#include "DummySimpleHandler.h"
#include "DummyComplexHandler.h"
#include "DummyGroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス PinHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
PinHandler::PinHandler(DotLibParser& parser) :
  GroupHandler(parser)
{
  DotLibHandler* dummy_simple = new DummySimpleHandler(parser);
  DotLibHandler* dummy_complex = new DummyComplexHandler(parser);
  DotLibHandler* dummy_group = new DummyGroupHandler(parser);

  // simple attributes
  reg_handler("bit_width", dummy_simple);
  reg_handler("capacitance", dummy_simple);
  reg_handler("clock", dummy_simple);
  reg_handler("clock_gate_clock_pin", dummy_simple);
  reg_handler("clock_gate_enable_pin", dummy_simple);
  reg_handler("clock_gate_test_pin", dummy_simple);
  reg_handler("clock_gate_obs_pin", dummy_simple);
  reg_handler("clock_gate_out_pin", dummy_simple);
  reg_handler("complementary_pin", dummy_simple);
  reg_handler("connection_class", dummy_simple);
  reg_handler("direction", dummy_simple);
  reg_handler("dont_fault", dummy_simple);
  reg_handler("drive_current", dummy_simple);
  reg_handler("driver_type", dummy_simple);
  reg_handler("fall_capacitance", dummy_simple);
  reg_handler("fall_current_slope_after_threshold", dummy_simple);
  reg_handler("fall_current_slope_before_threshold", dummy_simple);
  reg_handler("fall_time_after_threshold", dummy_simple);
  reg_handler("fall_time_before_threshold", dummy_simple);
  reg_handler("fanout_load", dummy_simple);
  reg_handler("fault_model", dummy_simple);
  reg_handler("function", dummy_simple);
  reg_handler("has_builtin_pad", dummy_simple);
  reg_handler("hysteresis", dummy_simple);
  reg_handler("input_map", dummy_simple);
  reg_handler("input_signal_level", dummy_simple);
  reg_handler("input_voltage", dummy_simple);
  reg_handler("internal_node", dummy_simple);
  reg_handler("inverted_output", dummy_simple);
  reg_handler("is_pad", dummy_simple);
  reg_handler("max_capacitance", dummy_simple);
  reg_handler("max_fanout", dummy_simple);
  reg_handler("max_input_noise_width", dummy_simple);
  reg_handler("max_transition", dummy_simple);
  reg_handler("min_capacitance", dummy_simple);
  reg_handler("min_fanout", dummy_simple);
  reg_handler("min_input_noise_width", dummy_simple);
  reg_handler("min_period", dummy_simple);
  reg_handler("min_pulse_width_high", dummy_simple);
  reg_handler("min_pulse_width_low", dummy_simple);
  reg_handler("min_transition", dummy_simple);
  reg_handler("multicell_pad_pin", dummy_simple);
  reg_handler("nextstate_type", dummy_simple);
  reg_handler("output_signal_level", dummy_simple);
  reg_handler("output_voltage", dummy_simple);
  reg_handler("pin_func_type", dummy_simple);
  reg_handler("prefer_tied", dummy_simple);
  reg_handler("primary_output", dummy_simple);
  reg_handler("pulling_current", dummy_simple);
  reg_handler("pulling_resistance", dummy_simple);
  reg_handler("rise_capacitance", dummy_simple);
  reg_handler("rise_current_slope_after_threshold", dummy_simple);
  reg_handler("rise_current_slope_before_threshold", dummy_simple);
  reg_handler("rise_time_after_threshold", dummy_simple);
  reg_handler("rise_time_before_threshold", dummy_simple);
  reg_handler("signal_type", dummy_simple);
  reg_handler("slew_control", dummy_simple);
  reg_handler("state_function", dummy_simple);
  reg_handler("test_output_only", dummy_simple);
  reg_handler("three_state", dummy_simple);
  reg_handler("vhdl_name", dummy_simple);
  reg_handler("x_function", dummy_simple);

  // complex attributes
  reg_handler("fall_capacitance_range", dummy_complex);
  reg_handler("rise_capacitance_range", dummy_complex);

  // group statements
  reg_handler("electromigration", dummy_group);
  reg_handler("hyperbolic_noise_above_high", dummy_group);
  reg_handler("hyperbolic_noise_below_low", dummy_group);
  reg_handler("hyperbolic_noise_high", dummy_group);
  reg_handler("hyperbolic_noise_low", dummy_group);
  reg_handler("internal_power", dummy_group);
  reg_handler("max_trans", dummy_group);
  reg_handler("min_pulse_width", dummy_group);
  reg_handler("minimum_period", dummy_group);
  reg_handler("timing", dummy_group);
  reg_handler("tlatch", dummy_group);
}

// @brief デストラクタ
PinHandler::~PinHandler()
{
}

// @brief グループ名を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] group_name グループ名
bool
PinHandler::read_group_name(const string& attr_name,
			    const string& group_name)
{
  cout << attr_name << "( " << group_name << " ) {" << endl;
  return true;
}

// @brief グループ内のステートメントをすべて処理したときに呼ばれる関数
bool
PinHandler::end_group()
{
  cout << "}" << endl;
  return true;
}


END_NAMESPACE_YM_CELL_DOTLIB
