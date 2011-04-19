
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

#include "DummySimpleHandler.h"
#include "DummyComplexHandler.h"
#include "DummyGroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス LibraryGroupHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
LibraryGroupHandler::LibraryGroupHandler(DotLibParser& parser) :
  GroupHandler(parser)
{
  DotLibHandler* dummy_simple = new DummySimpleHandler(parser);
  DotLibHandler* dummy_complex = new DummyComplexHandler(parser);
  DotLibHandler* dummy_group = new DummyGroupHandler(parser);

  // simple attributes
  reg_handler("bus_naming_style", new BusNamingStyleHandler(parser));
  reg_handler("comment", new CommentHandler(parser));
  reg_handler("current_unit", new CurrentUnitHandler(parser));
  reg_handler("date", new DateHandler(parser));
  reg_handler("delay_model", new DelayModelHandler(parser));
  reg_handler("em_temp_degradation_factor", dummy_simple);
  reg_handler("fpga_technology", dummy_simple);
  reg_handler("in_place_swap_mode", new InPlaceSwapModeHandler(parser));
  reg_handler("input_threshold_pct_fall", dummy_simple);
  reg_handler("input_threshold_pct_rise", dummy_simple);
  reg_handler("leakage_power_unit", new LeakagePowerUnitHandler(parser));
  reg_handler("nom_calc_mode", dummy_simple);
  reg_handler("nom_process", dummy_simple);
  reg_handler("nom_temperature", dummy_simple);
  reg_handler("nom_voltage", dummy_simple);
  reg_handler("output_threshold_pct_fall", dummy_simple);
  reg_handler("output_threshold_pct_rise", dummy_simple);
  reg_handler("piece_type", new PieceTypeHandler(parser));
  reg_handler("power_model", dummy_simple);
  reg_handler("preferred_output_pad_slew_rate_control", dummy_simple);
  reg_handler("preferred_input_pad_voltage", dummy_simple);
  reg_handler("preferred_output_pad_voltage", dummy_simple);
  reg_handler("pulling_resistance_unit",
	      new PullingResistanceUnitHandler(parser));
  reg_handler("revision", new RevisionHandler(parser));
  reg_handler("simulation", new SimulationHandler(parser));
  reg_handler("slew_derate_from_library", dummy_simple);
  reg_handler("slew_lower_threshold_pct_fall", dummy_simple);
  reg_handler("slew_lower_threshold_pct_rise", dummy_simple);
  reg_handler("slew_upper_threshold_pct_fall", dummy_simple);
  reg_handler("slew_upper_threshold_pct_rise", dummy_simple);
  reg_handler("time_unit", new TimeUnitHandler(parser));
  reg_handler("voltage_unit", new VoltageUnitHandler(parser));

  // default attributes
  reg_handler("default_cell_leakage_power", dummy_simple);
  reg_handler("default_connection_class", dummy_simple);
  reg_handler("default_fall_delay_intercept", dummy_simple);
  reg_handler("default_fall_pin_resistance", dummy_simple);
  reg_handler("default_fanout_load", dummy_simple);
  reg_handler("default_inout_pin_cap", dummy_simple);
  reg_handler("default_inout_pin_fall_res", dummy_simple);
  reg_handler("default_inout_pin_rise_res", dummy_simple);
  reg_handler("default_input_pin_cap", dummy_simple);
  reg_handler("default_intrinsic_fall", dummy_simple);
  reg_handler("default_intrinsic_rise", dummy_simple);
  reg_handler("default_leakage_power_density", dummy_simple);
  reg_handler("default_max_capacitance", dummy_simple);
  reg_handler("default_max_fanout", dummy_simple);
  reg_handler("default_max_transition", dummy_simple);
  reg_handler("default_max_utilization", dummy_simple);
  reg_handler("default_min_porosity", dummy_simple);
  reg_handler("default_operating_conditions", dummy_simple);
  reg_handler("default_output_pin_cap", dummy_simple);
  reg_handler("default_output_pin_fall_res", dummy_simple);
  reg_handler("default_output_pin_rise_res", dummy_simple);
  reg_handler("default_rise_delay_intercept", dummy_simple);
  reg_handler("default_rise_pin_resistance", dummy_simple);
  reg_handler("default_slope_fall", dummy_simple);
  reg_handler("default_slope_rise", dummy_simple);
  reg_handler("default_wire_load", dummy_simple);
  reg_handler("default_wire_load_area", dummy_simple);
  reg_handler("default_wire_load_capacitance", dummy_simple);
  reg_handler("default_wire_load_mode", dummy_simple);
  reg_handler("default_wire_load_resistance", dummy_simple);
  reg_handler("default_wire_load_selection", dummy_simple);

  // scaling attributes
  reg_handler("k_process_cell_fall", dummy_simple);
  reg_handler("k_process_cell_leakage_power", dummy_simple);
  reg_handler("k_process_cell_rise", dummy_simple);
  reg_handler("k_process_drive_current", dummy_simple);
  reg_handler("k_process_drive_fall", dummy_simple);
  reg_handler("k_process_drive_rise", dummy_simple);
  reg_handler("k_process_fall_delay_intercept", dummy_simple);
  reg_handler("k_process_fall_pin_resistance", dummy_simple);
  reg_handler("k_process_fall_propagation", dummy_simple);
  reg_handler("k_process_fall_transition", dummy_simple);
  reg_handler("k_process_hold_fall", dummy_simple);
  reg_handler("k_process_hold_rise", dummy_simple);
  reg_handler("k_process_internal_power", dummy_simple);
  reg_handler("k_process_intrinsic_fall", dummy_simple);
  reg_handler("k_process_intrinsic_rise", dummy_simple);
  reg_handler("k_process_min_period", dummy_simple);
  reg_handler("k_process_min_pulse_width_high", dummy_simple);
  reg_handler("k_process_min_pulse_width_low", dummy_simple);
  reg_handler("k_process_nochange_fall", dummy_simple);
  reg_handler("k_process_nochange_rise", dummy_simple);
  reg_handler("k_process_pin_cap", dummy_simple);
  reg_handler("k_process_recovery_fall", dummy_simple);
  reg_handler("k_process_recovery_rise", dummy_simple);
  reg_handler("k_process_removal_fall", dummy_simple);
  reg_handler("k_process_removal_rise", dummy_simple);
  reg_handler("k_process_rise_delay_intercept", dummy_simple);
  reg_handler("k_process_rise_pin_resistance", dummy_simple);
  reg_handler("k_process_rise_propagation", dummy_simple);
  reg_handler("k_process_rise_transition", dummy_simple);
  reg_handler("k_process_setup_fall", dummy_simple);
  reg_handler("k_process_setup_rise", dummy_simple);
  reg_handler("k_process_skew_fall", dummy_simple);
  reg_handler("k_process_skew_rise", dummy_simple);
  reg_handler("k_process_slope_fall", dummy_simple);
  reg_handler("k_process_slope_rise", dummy_simple);
  reg_handler("k_process_wire_cap", dummy_simple);
  reg_handler("k_process_wire_res", dummy_simple);
  reg_handler("k_temp_cell_fall", dummy_simple);
  reg_handler("k_temp_cell_leakage_power", dummy_simple);
  reg_handler("k_temp_cell_rise", dummy_simple);
  reg_handler("k_temp_drive_current", dummy_simple);
  reg_handler("k_temp_drive_fall", dummy_simple);
  reg_handler("k_temp_drive_rise", dummy_simple);
  reg_handler("k_temp_fall_delay_intercept", dummy_simple);
  reg_handler("k_temp_fall_pin_resistance", dummy_simple);
  reg_handler("k_temp_fall_propagation", dummy_simple);
  reg_handler("k_temp_fall_transition", dummy_simple);
  reg_handler("k_temp_hold_fall", dummy_simple);
  reg_handler("k_temp_hold_rise", dummy_simple);
  reg_handler("k_temp_internal_power", dummy_simple);
  reg_handler("k_temp_intrinsic_fall", dummy_simple);
  reg_handler("k_temp_intrinsic_rise", dummy_simple);
  reg_handler("k_temp_min_period", dummy_simple);
  reg_handler("k_temp_min_pulse_width_high", dummy_simple);
  reg_handler("k_temp_min_pulse_width_low", dummy_simple);
  reg_handler("k_temp_nochange_fall", dummy_simple);
  reg_handler("k_temp_nochange_rise", dummy_simple);
  reg_handler("k_temp_pin_cap", dummy_simple);
  reg_handler("k_temp_recovery_fall", dummy_simple);
  reg_handler("k_temp_recovery_rise", dummy_simple);
  reg_handler("k_temp_removal_fall", dummy_simple);
  reg_handler("k_temp_removal_rise", dummy_simple);
  reg_handler("k_temp_rise_delay_intercept", dummy_simple);
  reg_handler("k_temp_rise_pin_resistance", dummy_simple);
  reg_handler("k_temp_rise_propagation", dummy_simple);
  reg_handler("k_temp_rise_transition", dummy_simple);
  reg_handler("k_temp_setup_fall", dummy_simple);
  reg_handler("k_temp_setup_rise", dummy_simple);
  reg_handler("k_temp_skew_fall", dummy_simple);
  reg_handler("k_temp_skew_rise", dummy_simple);
  reg_handler("k_temp_slope_fall", dummy_simple);
  reg_handler("k_temp_slope_rise", dummy_simple);
  reg_handler("k_temp_wire_cap", dummy_simple);
  reg_handler("k_temp_wire_res", dummy_simple);
  reg_handler("k_vol_cell_fall", dummy_simple);
  reg_handler("k_vol_cell_leakage_power", dummy_simple);
  reg_handler("k_vol_cell_rise", dummy_simple);
  reg_handler("k_vol_drive_current", dummy_simple);
  reg_handler("k_vol_drive_fall", dummy_simple);
  reg_handler("k_vol_drive_rise", dummy_simple);
  reg_handler("k_vol_fall_delay_intercept", dummy_simple);
  reg_handler("k_vol_fall_pin_resistance", dummy_simple);
  reg_handler("k_vol_fall_propagation", dummy_simple);
  reg_handler("k_vol_fall_transition", dummy_simple);
  reg_handler("k_vol_hold_fall", dummy_simple);
  reg_handler("k_vol_hold_rise", dummy_simple);
  reg_handler("k_vol_internal_power", dummy_simple);
  reg_handler("k_vol_intrinsic_fall", dummy_simple);
  reg_handler("k_vol_intrinsic_rise", dummy_simple);
  reg_handler("k_vol_min_period", dummy_simple);
  reg_handler("k_vol_min_pulse_width_high", dummy_simple);
  reg_handler("k_vol_min_pulse_width_low", dummy_simple);
  reg_handler("k_vol_nochange_fall", dummy_simple);
  reg_handler("k_vol_nochange_rise", dummy_simple);
  reg_handler("k_vol_pin_cap", dummy_simple);
  reg_handler("k_vol_recovery_fall", dummy_simple);
  reg_handler("k_vol_recovery_rise", dummy_simple);
  reg_handler("k_vol_removal_fall", dummy_simple);
  reg_handler("k_vol_removal_rise", dummy_simple);
  reg_handler("k_vol_rise_delay_intercept", dummy_simple);
  reg_handler("k_vol_rise_pin_resistance", dummy_simple);
  reg_handler("k_vol_rise_propagation", dummy_simple);
  reg_handler("k_vol_rise_transition", dummy_simple);
  reg_handler("k_vol_setup_fall", dummy_simple);
  reg_handler("k_vol_setup_rise", dummy_simple);
  reg_handler("k_vol_skew_fall", dummy_simple);
  reg_handler("k_vol_skew_rise", dummy_simple);
  reg_handler("k_vol_slope_fall", dummy_simple);
  reg_handler("k_vol_slope_rise", dummy_simple);
  reg_handler("k_vol_wire_cap", dummy_simple);
  reg_handler("k_vol_wire_res", dummy_simple);

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
  reg_handler("cell", new CellHandler(parser));
  reg_handler("dc_current_template", dummy_group);
  reg_handler("em_lut_template", dummy_group);
  reg_handler("fall_transition_degradation", dummy_group);
  reg_handler("faults_lut_template", dummy_group);
  reg_handler("input_voltage", dummy_group);
  reg_handler("iv_lut_template", dummy_group);
  reg_handler("lu_table_template",
	      new LuTableTemplateHandler(parser));
  reg_handler("noise_lut_template", dummy_group);
  reg_handler("operating_conditions",
	      new OperatingConditionsHandler(parser));
  reg_handler("output_voltage", dummy_group);
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
  reg_handler("wire_load_selection", dummy_group);
  reg_handler("wire_load_table", dummy_group);
}

// @brief デストラクタ
LibraryGroupHandler::~LibraryGroupHandler()
{
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

  cout << "library ( " << token_list[0].value() << " )" << endl;

  return true;
}

// @brief グループ内のステートメントをすべて処理したときに呼ばれる関数
bool
LibraryGroupHandler::end_group()
{
  cout << "}" << endl;
  return true;
}

END_NAMESPACE_YM_CELL_DOTLIB
