
/// @file libym_cell/dotlib/TimingHandler.cc
/// @brief TimingHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "TimingHandler.h"

#include "SimpleHandler.h"
#include "ComplexHandler.h"
#include "GroupHandler.h"
#include "PinHandler.h"
#include "TableHandler.h"


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

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス TimingHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
TimingHandler::TimingHandler(GroupHandler* parent) :
  EmptyGroupHandler(parent)
{
  // simple attributes
  DotlibHandler* simple = new_simple(this);
  reg_handler("edge_rate_sensitivity_f0", simple);
  reg_handler("edge_rate_sensitivity_f1", simple);
  reg_handler("edge_rate_sensitivity_r0", simple);
  reg_handler("edge_rate_sensitivity_r1", simple);
  reg_handler("fall_resistance", simple);
  reg_handler("intrinsic_fall", simple);
  reg_handler("intrinsic_rise", simple);
  reg_handler("related_bus_equivalent", simple);
  reg_handler("related_bus_pins", simple);
  reg_handler("related_output_pin", simple);
  reg_handler("related_pin", simple);
  reg_handler("rise_resistance", simple);
  reg_handler("sdf_cond", simple);
  reg_handler("sdf_cond_end", simple);
  reg_handler("sdf_cond_start", simple);
  reg_handler("sdf_edges", simple);
  reg_handler("slope_fall", simple);
  reg_handler("slope_rise", simple);
  reg_handler("timing_sense", simple);
  reg_handler("timing_type", simple);
  reg_handler("when", simple);
  reg_handler("when_end", simple);
  reg_handler("when_start", simple);

  // complex attribute
  DotlibHandler* complex = new_complex(this);
  reg_handler("fall_delay_intercept", complex);
  reg_handler("fall_pin_resistance", complex);
  reg_handler("rise_delay_intercept", complex);
  reg_handler("rise_pin_resistance", complex);
  reg_handler("orders", complex);
  reg_handler("coefs", complex);

  // group statements
  reg_handler("cell_degradation", new_group(this));
  reg_handler("cell_fall", new TableHandler(this));
  reg_handler("cell_rise", new TableHandler(this));
  reg_handler("fall_constraint", new TableHandler(this));
  reg_handler("fall_propagation", new TableHandler(this));
  reg_handler("fall_transition", new TableHandler(this));
  reg_handler("noise_immunity_above_high", new_group(this));
  reg_handler("noise_immunity_below_low", new_group(this));
  reg_handler("noise_immunity_high", new_group(this));
  reg_handler("noise_immunity_low", new_group(this));
  reg_handler("propogated_noise_height_above_high", new_group(this));
  reg_handler("propogated_noise_height_below_low", new_group(this));
  reg_handler("propogated_noise_height_high", new_group(this));
  reg_handler("propogated_noise_height_low", new_group(this));
  reg_handler("propogated_noise_peak_time_ratio_above_high", new_group(this));
  reg_handler("propogated_noise_peak_time_ratio__below_low", new_group(this));
  reg_handler("propogated_noise_peak_time_ratio_high", new_group(this));
  reg_handler("propogated_noise_peak_time_ratio_low", new_group(this));
  reg_handler("propogated_noise_width_above_high", new_group(this));
  reg_handler("propogated_noise_width_below_low", new_group(this));
  reg_handler("propogated_noise_width_high", new_group(this));
  reg_handler("propogated_noise_width_low", new_group(this));
  reg_handler("retaining_fall", new_group(this));
  reg_handler("retaining_rise", new_group(this));
  reg_handler("retain_fall_slew", new_group(this));
  reg_handler("retain_rise_slew", new_group(this));
  reg_handler("rise_constraint", new TableHandler(this));
  reg_handler("rise_propagation", new TableHandler(this));
  reg_handler("rise_transition", new TableHandler(this));
  reg_handler("steady_state_current_high", new_group(this));
  reg_handler("steady_state_current_low", new_group(this));
  reg_handler("steady_state_current_tristate", new_group(this));
}

// @brief デストラクタ
TimingHandler::~TimingHandler()
{
}

END_NAMESPACE_YM_CELL_DOTLIB
