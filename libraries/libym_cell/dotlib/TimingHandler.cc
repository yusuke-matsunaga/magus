
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

#include "PtMgr.h"
#include "PtNode.h"
#include "PtPin.h"
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
  return new GenGroupHandler(parent);
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

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス TimingHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
TimingHandler::TimingHandler(PinHandler* parent) :
  GroupHandler(parent->parser(), parent->ptmgr()),
  mParent(parent),
  mTiming(NULL)
{
}

// @brief デストラクタ
TimingHandler::~TimingHandler()
{
}

// @brief 親のハンドラを得る．
GroupHandler*
TimingHandler::parent()
{
  return mParent;
}

// @brief 対応する PtNode を返す．
PtNode*
TimingHandler::pt_node()
{
  return mTiming;
}

// @brief group statement の最初に呼ばれる関数
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value_list 値を表すトークンのリスト
bool
TimingHandler::begin_group(const ShString& attr_name,
			   const FileRegion& attr_loc,
			   const vector<const PtValue*>& value_list)
{
  if ( value_list.size() != 0 ) {
    FileRegion loc = value_list[0]->loc();
    put_msg(__FILE__, __LINE__, loc,
	    kMsgError,
	    "DOTLIB_PARSER",
	    "timing group does not have a parameter.");
    return false;
  }

  mTiming = ptmgr().new_pttiming();
  mParent->add_timing(mTiming);

  return true;
}

// @brief group statement の最後に呼ばれる関数
bool
TimingHandler::end_group()
{
  mTiming = NULL;
  return true;
}

END_NAMESPACE_YM_CELL_DOTLIB
