
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

#include "BusHandler.h"
#include "FfHandler.h"
#include "LatchHandler.h"
#include "PinHandler.h"
#include "StateTableHandler.h"
#include "LeakagePowerHandler.h"

#include "InternalPowerHandler.h"

#include "DummySimpleHandler.h"
#include "DummyComplexHandler.h"
#include "DummyGroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス CellHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
CellHandler::CellHandler(GroupHandler* parent) :
  GroupHandler(parent->parser(), parent)
{
  DotLibHandler* simple = new SimpleHandler(parser(), this);
  DotLibHandler* complex = new ComplexHandler(parser(), this);
  DotLibHandler* group = new GroupHandler(parser(), this);

  // simple attributes
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
  reg_handler("pin_opposite", complex);
  reg_handler("rail_connection", complex);
  reg_handler("power_supply_namestring", complex);
  reg_handler("resource_usage", complex);

  // group statements
  reg_handler("bus", new BusHandler(this));
  reg_handler("dynamic_current", group);
  reg_handler("ff", new FfHandler(this));
  reg_handler("ff_bank", group);
  reg_handler("functional_yield_metric", group);
  reg_handler("generated_clock", group);
  reg_handler("intrinsic_parasitic", group);
  reg_handler("latch", new LatchHandler(this));
  reg_handler("latch_bank", group);
  reg_handler("leakage_current", group);
  reg_handler("leakage_power", new LeakagePowerHandler(this));
  reg_handler("lut", group);
  reg_handler("mode_definition", group);
  reg_handler("pin", new PinHandler(this));
  reg_handler("routing_track", group);
  reg_handler("statetable", new StateTableHandler(this));

  reg_handler("internal_power", new CellInternalPowerHandler(this));
}

// @brief デストラクタ
CellHandler::~CellHandler()
{
}

END_NAMESPACE_YM_CELL_DOTLIB
