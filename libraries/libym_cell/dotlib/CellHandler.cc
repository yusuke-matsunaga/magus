
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
#include "LibraryHandler.h"

#include "LeakagePowerHandler.h"
#include "BusHandler.h"
#include "BundleHandler.h"
#include "PinHandler.h"

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
  return new GenGroupHandler(parent);
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
new_test_cell(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);
  // group statements
  handler->reg_handler("bus", new BusHandler(handler));
  handler->reg_handler("dynamic_current", new_group(handler));
  handler->reg_handler("ff", new_ff(handler));
  handler->reg_handler("ff_bank", new_ff(handler));
  handler->reg_handler("functional_yield_metric", new_group(handler));
  handler->reg_handler("generated_clock", new_group(handler));
  handler->reg_handler("intrinsic_parasitic", new_group(handler));
  handler->reg_handler("latch", new_latch(handler));
  handler->reg_handler("latch_bank", new_latch(handler));
  handler->reg_handler("leakage_current", new_group(handler));
  handler->reg_handler("leakage_power", new LeakagePowerHandler(handler));
  handler->reg_handler("lut", new_group(handler));
  handler->reg_handler("mode_definition", new_group(handler));
  handler->reg_handler("pin", new PinHandler(handler));
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
  reg_handler("bus", new BusHandler(this));
  reg_handler("bundle", new BundleHandler(this));
  reg_handler("dynamic_current", new_group(this));
  reg_handler("ff", new_ff(this));
  reg_handler("ff_bank", new_ff(this));
  reg_handler("functional_yield_metric", new_group(this));
  reg_handler("generated_clock", new_group(this));
  reg_handler("intrinsic_parasitic", new_group(this));
  reg_handler("latch", new_latch(this));
  reg_handler("latch_bank", new_latch(this));
  reg_handler("leakage_current", new_group(this));
  reg_handler("leakage_power", new LeakagePowerHandler(this));
  reg_handler("lut", new_group(this));
  reg_handler("mode_definition", new_group(this));
  reg_handler("pin", new PinHandler(this));
  reg_handler("routing_track", new_group(this));
  reg_handler("statetable", new_statetable(this));

  reg_handler("internal_power", new_cell_internal_power(this));

  reg_handler("test_cell", new_test_cell(this));
}

// @brief デストラクタ
CellHandler::~CellHandler()
{
}

// @brief leakage_power を追加する．
bool
CellHandler::add_leakage_power(PtLeakagePower* lp)
{
  return mCell->add_leakage_power(lp);
}

// @brief ピンを追加する．
bool
CellHandler::add_pin(PtPin* pin)
{
  return mCell->add_pin(pin);
}

// @brief バスを追加する．
bool
CellHandler::add_bus(PtBus* bus)
{
  return mCell->add_bus(bus);
}

// @brief バンドルを追加する．
bool
CellHandler::add_bundle(PtBundle* bundle)
{
  return mCell->add_bundle(bundle);
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
  mCell = ptmgr().new_ptcell(value_list[0]->string_value());
  return parent()->add_cell(mCell);
}

// @brief group statement の最後に呼ばれる関数
bool
CellHandler::end_group()
{
  mCell = NULL;
  return true;
}


END_NAMESPACE_YM_CELL_DOTLIB
