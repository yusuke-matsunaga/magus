
/// @file libym_cell/dotlib/PinHandler.cc
/// @brief PinHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PinHandler.h"

#include "SimpleHandler.h"
#include "ComplexHandler.h"
#include "GroupHandler.h"

#include "TimingHandler.h"

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

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス PinHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
PinHandler::PinHandler(GroupHandler* parent) :
  GroupHandler(parent),
  mPin(NULL)
{
  // simple attributes
  DotlibHandler* simple = new_simple(this);
  reg_handler("bit_width", simple);
  reg_handler("capacitance", simple);
  reg_handler("clock", simple);
  reg_handler("clock_gate_clock_pin", simple);
  reg_handler("clock_gate_enable_pin", simple);
  reg_handler("clock_gate_test_pin", simple);
  reg_handler("clock_gate_obs_pin", simple);
  reg_handler("clock_gate_out_pin", simple);
  reg_handler("complementary_pin", simple);
  reg_handler("connection_class", simple);
  reg_handler("direction", simple);
  reg_handler("dont_fault", simple);
  reg_handler("drive_current", simple);
  reg_handler("driver_type", simple);
  reg_handler("fall_capacitance", simple);
  reg_handler("fall_current_slope_after_threshold", simple);
  reg_handler("fall_current_slope_before_threshold", simple);
  reg_handler("fall_time_after_threshold", simple);
  reg_handler("fall_time_before_threshold", simple);
  reg_handler("fanout_load", simple);
  reg_handler("fault_model", simple);
  reg_handler("function", simple);
  reg_handler("has_builtin_pad", simple);
  reg_handler("hysteresis", simple);
  reg_handler("input_map", simple);
  reg_handler("input_signal_level", simple);
  reg_handler("input_voltage", simple);
  reg_handler("internal_node", simple);
  reg_handler("inverted_output", simple);
  reg_handler("is_pad", simple);
  reg_handler("max_capacitance", simple);
  reg_handler("max_fanout", simple);
  reg_handler("max_input_noise_width", simple);
  reg_handler("max_transition", simple);
  reg_handler("min_capacitance", simple);
  reg_handler("min_fanout", simple);
  reg_handler("min_input_noise_width", simple);
  reg_handler("min_period", simple);
  reg_handler("min_pulse_width_high", simple);
  reg_handler("min_pulse_width_low", simple);
  reg_handler("min_transition", simple);
  reg_handler("multicell_pad_pin", simple);
  reg_handler("nextstate_type", simple);
  reg_handler("output_signal_level", simple);
  reg_handler("output_voltage", simple);
  reg_handler("pin_func_type", simple);
  reg_handler("prefer_tied", simple);
  reg_handler("primary_output", simple);
  reg_handler("pulling_current", simple);
  reg_handler("pulling_resistance", simple);
  reg_handler("rise_capacitance", simple);
  reg_handler("rise_current_slope_after_threshold", simple);
  reg_handler("rise_current_slope_before_threshold", simple);
  reg_handler("rise_time_after_threshold", simple);
  reg_handler("rise_time_before_threshold", simple);
  reg_handler("signal_type", simple);
  reg_handler("slew_control", simple);
  reg_handler("state_function", simple);
  reg_handler("test_output_only", simple);
  reg_handler("three_state", simple);
  reg_handler("vhdl_name", simple);
  reg_handler("x_function", simple);

  // complex attributes
  DotlibHandler* complex = new_complex(this);
  reg_handler("fall_capacitance_range", complex);
  reg_handler("rise_capacitance_range", complex);

  // group statements
  reg_handler("electromigration", new_group(this));
  reg_handler("hyperbolic_noise_above_high", new_group(this));
  reg_handler("hyperbolic_noise_below_low", new_group(this));
  reg_handler("hyperbolic_noise_high", new_group(this));
  reg_handler("hyperbolic_noise_low", new_group(this));
  reg_handler("internal_power", new_internal_power(this));
  reg_handler("max_trans", new_group(this));
  reg_handler("min_pulse_width", new_group(this));
  reg_handler("minimum_period", new_group(this));
  reg_handler("timing", new TimingHandler(this));
  reg_handler("tlatch", new_group(this));
}

// @brief デストラクタ
PinHandler::~PinHandler()
{
}

// @brief 対応する PtNode を返す．
PtNode*
PinHandler::pt_node()
{
  return mPin;
}

// @brief ピンを追加する．
void
PinHandler::add_timing(PtTiming* timing)
{
  mPin->add_timing(timing);
}

// @brief group statement の最初に呼ばれる関数
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value_list 値を表すトークンのリスト
bool
PinHandler::begin_group(const ShString& attr_name,
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
	    "pin group requires just one string as a parameter.");
    return false;
  }

  if ( value_list[0]->type() != SYMBOL ) {
    put_msg(__FILE__, __LINE__, value_list[0]->loc(),
	    kMsgError,
	    "DOTLIBPARSER",
	    "string value is exprected.");
    return false;
  }
  mPin = ptmgr().new_ptpin(value_list[0]);
  parent()->add_pin(mPin);

  return true;
}

// @brief group statement の最後に呼ばれる関数
bool
PinHandler::end_group()
{
  mPin = NULL;
  return true;
}

END_NAMESPACE_YM_CELL_DOTLIB
