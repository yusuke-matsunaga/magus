
/// @file libym_cell/dotlib/TimingHandler.cc
/// @brief TimingHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "TimingHandler.h"

#include "DummySimpleHandler.h"
#include "DummyComplexHandler.h"
#include "DummyGroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス TimingHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
TimingHandler::TimingHandler(DotLibParser& parser) :
  GroupHandler(parser)
{
  DotLibHandler* dummy_simple = new DummySimpleHandler(parser);
  DotLibHandler* dummy_complex = new DummyComplexHandler(parser);
  DotLibHandler* dummy_group = new DummyGroupHandler(parser);

  // simple attributes
  reg_handler("edge_rate_sensitivity_f0", dummy_simple);
  reg_handler("edge_rate_sensitivity_f1", dummy_simple);
  reg_handler("edge_rate_sensitivity_r0", dummy_simple);
  reg_handler("edge_rate_sensitivity_r1", dummy_simple);
  reg_handler("fall_resistance", dummy_simple);
  reg_handler("intrinsic_fall", dummy_simple);
  reg_handler("intrinsic_rise", dummy_simple);
  reg_handler("related_bus_equivalent", dummy_simple);
  reg_handler("related_bus_pins", dummy_simple);
  reg_handler("related_output_pin", dummy_simple);
  reg_handler("related_pin", dummy_simple);
  reg_handler("rise_resistance", dummy_simple);
  reg_handler("sdf_cond", dummy_simple);
  reg_handler("sdf_cond_end", dummy_simple);
  reg_handler("sdf_cond_start", dummy_simple);
  reg_handler("sdf_edges", dummy_simple);
  reg_handler("slope_fall", dummy_simple);
  reg_handler("slope_rise", dummy_simple);
  reg_handler("timing_sense", dummy_simple);
  reg_handler("timing_type", dummy_simple);
  reg_handler("when", dummy_simple);
  reg_handler("when_end", dummy_simple);
  reg_handler("when_start", dummy_simple);

  // complex attribute
  reg_handler("fall_delay_intercept", dummy_complex);
  reg_handler("fall_pin_resistance", dummy_complex);
  reg_handler("rise_delay_intercept", dummy_complex);
  reg_handler("rise_pin_resistance", dummy_complex);
  reg_handler("orders", dummy_complex);
  reg_handler("coefs", dummy_complex);

  // group statements
  reg_handler("cell_degradation", dummy_group);
  reg_handler("cell_fall", dummy_group);
  reg_handler("cell_rise", dummy_group);
  reg_handler("fall_constraint", dummy_group);
  reg_handler("fall_propagation", dummy_group);
  reg_handler("fall_transition", dummy_group);
  reg_handler("noise_immunity_above_high", dummy_group);
  reg_handler("noise_immunity_below_low", dummy_group);
  reg_handler("noise_immunity_high", dummy_group);
  reg_handler("noise_immunity_low", dummy_group);
  reg_handler("propogated_noise_height_above_high", dummy_group);
  reg_handler("propogated_noise_height_below_low", dummy_group);
  reg_handler("propogated_noise_height_high", dummy_group);
  reg_handler("propogated_noise_height_low", dummy_group);
  reg_handler("propogated_noise_peak_time_ratio_above_high", dummy_group);
  reg_handler("propogated_noise_peak_time_ratio__below_low", dummy_group);
  reg_handler("propogated_noise_peak_time_ratio_high", dummy_group);
  reg_handler("propogated_noise_peak_time_ratio_low", dummy_group);
  reg_handler("propogated_noise_width_above_high", dummy_group);
  reg_handler("propogated_noise_width_below_low", dummy_group);
  reg_handler("propogated_noise_width_high", dummy_group);
  reg_handler("propogated_noise_width_low", dummy_group);
  reg_handler("retaining_fall", dummy_group);
  reg_handler("retaining_rise", dummy_group);
  reg_handler("retain_fall_slew", dummy_group);
  reg_handler("retain_rise_slew", dummy_group);
  reg_handler("rise_constraint", dummy_group);
  reg_handler("rise_propagation", dummy_group);
  reg_handler("rise_transition", dummy_group);
  reg_handler("steady_state_current_high", dummy_group);
  reg_handler("steady_state_current_low", dummy_group);
  reg_handler("steady_state_current_tristate", dummy_group);
}

// @brief デストラクタ
TimingHandler::~TimingHandler()
{
}

// @brief グループ名を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] token_list トークンのリスト
bool
TimingHandler::begin_group(const string& attr_name,
			   const vector<Token>& token_list)
{
  cout << attr_name << "( ";
  ymuint n = token_list.size();
  const char* comma = "";
  for (ymuint i = 0; i < n; ++ i) {
    cout << comma << token_list[i].value();
    comma = ", ";
  }
  cout << " ) {" << endl;
  return true;
}

// @brief グループ内のステートメントをすべて処理したときに呼ばれる関数
bool
TimingHandler::end_group()
{
  cout << "}" << endl;
  return true;
}


END_NAMESPACE_YM_CELL_DOTLIB
