
/// @file libym_cell/dotlib/LibraryGroupHandler.cc
/// @brief LibraryGroupHandler などの実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LibraryGroupHandler.h"

#include "LibSimpleHandler.h"
#include "LibComplexHandler.h"

#include "CellHandler.h"
#include "DefineHandler.h"
#include "OperatingConditionsHandler.h"
#include "WireLoadHandler.h"
#include "LuTableTemplateHandler.h"
#include "OutputCurrentTemplateHandler.h"
#include "PowerLutTemplateHandler.h"
#include "InputVoltageHandler.h"
#include "OutputVoltageHandler.h"

#include "DummyComplexHandler.h"
#include "DummyGroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス LibraryGroupHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
LibraryGroupHandler::LibraryGroupHandler(DotLibParser& parser) :
  GroupHandler(parser),
  mLibrary(NULL)
{
  DotLibHandler* dummy_complex = new DummyComplexHandler(parser);
  DotLibHandler* dummy_group = new DummyGroupHandler(parser);

  // simple attributes
  LibSimpleHandler* simple = new LibSimpleHandler(this);
  reg_handler("bus_naming_style", simple);
  reg_handler("comment", simple);
  reg_handler("current_unit", simple);
  reg_handler("date", simple);
  reg_handler("delay_model", simple);
  reg_handler("em_temp_degradation_factor", simple);
  reg_handler("fpga_technology", simple);
  reg_handler("in_place_swap_mode", simple);
  reg_handler("input_threshold_pct_fall", simple);
  reg_handler("input_threshold_pct_rise", simple);
  reg_handler("leakage_power_unit", simple);
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
  reg_handler("pulling_resistance_unit", simple);
  reg_handler("revision", simple);
  reg_handler("simulation", simple);
  reg_handler("slew_derate_from_library", simple);
  reg_handler("slew_lower_threshold_pct_fall", simple);
  reg_handler("slew_lower_threshold_pct_rise", simple);
  reg_handler("slew_upper_threshold_pct_fall", simple);
  reg_handler("slew_upper_threshold_pct_rise", simple);
  reg_handler("time_unit", simple);
  reg_handler("voltage_unit", simple);

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
  reg_handler("capacitive_load_unit", new CapacitiveLoadUnitHandler(parser));
  reg_handler("default_part", dummy_complex);
  reg_handler("define", new DefineHandler(this));
  reg_handler("define_cell_area", new DefineCellAreaHandler(parser));
  reg_handler("define_group", dummy_complex);
  reg_handler("library_features", new LibraryFeaturesHandler(parser));
  reg_handler("piece_define", new PieceDefineHandler(parser));
  reg_handler("routing_layers", new RoutingLayersHandler(parser));
  reg_handler("technology", new TechnologyHandler(parser));

  //fall_net_delay : name ;
  //rise_net_delay : name ;

  // group statements
  reg_handler("cell", new CellHandler(this));
  reg_handler("dc_current_template", dummy_group);
  reg_handler("em_lut_template", dummy_group);
  reg_handler("fall_transition_degradation", dummy_group);
  reg_handler("faults_lut_template", dummy_group);
  reg_handler("input_voltage", new InputVoltageHandler(parser));
  reg_handler("iv_lut_template", dummy_group);
  reg_handler("lu_table_template",
	      new LuTableTemplateHandler(parser));
  reg_handler("noise_lut_template", dummy_group);
  reg_handler("operating_conditions",
	      new OperatingConditionsHandler(parser));
  reg_handler("output_voltage", new OutputVoltageHandler(parser));
  reg_handler("output_current_template",
	      new OutputCurrentTemplateHandler(parser));
  reg_handler("part", dummy_group);
  reg_handler("poly_template", dummy_group);
  reg_handler("power_lut_template",
	      new PowerLutTemplateHandler(parser));
  reg_handler("power_poly_template", dummy_group);
  reg_handler("power_supply", dummy_group);
  reg_handler("propagation_lut_template", dummy_group);
  reg_handler("rise_transition_degradation", dummy_group);
  reg_handler("scaled_cell", dummy_group);
  reg_handler("scaling_factors", dummy_group);
  reg_handler("timing", dummy_group);
  reg_handler("timing_range", dummy_group);
  reg_handler("type", dummy_group);
  reg_handler("wire_load", new WireLoadHandler(parser));
  reg_handler("wire_load_selection", new WireLoadSelectionHandler(parser));
  reg_handler("wire_load_table", new WireLoadTableHandler(parser));
}

// @brief デストラクタ
LibraryGroupHandler::~LibraryGroupHandler()
{
}

// @brief 対象のセルライブラリを返す．
CiLibrary*
LibraryGroupHandler::library()
{
  return mLibrary;
}

// @brief グループ名を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] token_list トークンのリスト
bool
LibraryGroupHandler::begin_group(const string& attr_name,
				 const vector<Token>& token_list)
{
  assert_cond( attr_name == "library", __FILE__, __LINE__);

  if ( token_list.size() != 1 ) {
    FileRegion loc;
    if ( token_list.size() > 1 ) {
      loc = token_list[2].loc();
    }
    msg_mgr().put_msg(__FILE__, __LINE__, loc,
		      kMsgError,
		      "DOTLIB_PARSER",
		      "library statement require only one token");
    return false;
  }

  if ( token_list[0].type() != STR ) {
    msg_mgr().put_msg(__FILE__, __LINE__, token_list[0].loc(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      "string type is expected.");
    return false;
  }

  string name = token_list[0].value();
  mLibrary = new CiLibrary(name);

  return true;
}

// @brief グループ内のステートメントをすべて処理したときに呼ばれる関数
bool
LibraryGroupHandler::end_group()
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス LibAttrHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] lib_handler 親のハンドラ
LibAttrHandler::LibAttrHandler(LibraryGroupHandler* lib_handler) :
  SimpleHandler(lib_handler->parser()),
  mLibHandler(lib_handler)
{
}

// @brief デストラクタ
LibAttrHandler::~LibAttrHandler()
{
}

// 親のハンドラを返す．
LibraryGroupHandler*
LibAttrHandler::lib_handler()
{
  return mLibHandler;
}


//////////////////////////////////////////////////////////////////////
// クラス LibSimpleHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] lib_handler 親のハンドラ
LibSimpleHandler::LibSimpleHandler(LibraryGroupHandler* lib_handler) :
  LibAttrHandler(lib_handler)
{
}

// @brief デストラクタ
LibSimpleHandler::~LibSimpleHandler()
{
}

// @brief 属性値の読み込み処理を行う．
// @param[in] attr_name 属性名
// @param[in] value 値を表すトークン
// @return エラーが起きたら false を返す．
bool
LibSimpleHandler::read_value(const string& attr_name,
			     Token token)
{
  lib_handler()->library()->set_attr(attr_name, token.value());
  return true;
}

END_NAMESPACE_YM_CELL_DOTLIB
