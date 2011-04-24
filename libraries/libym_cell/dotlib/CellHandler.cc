
/// @file libym_cell/dotlib/CellHandler.cc
/// @brief CellHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CellHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス CellHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
// @param[in] parent 親のハンドラ
CellHandler::CellHandler(DotlibParser& parser,
			 GroupHandler* parent) :
  GroupHandler(parser, parent),
  mCell(NULL)
{
  // simple attributes
  DotlibHandler* simple = new_simple(handler, false);
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
  DotlibHandler* complex = new_complex(handler);
  reg_handler("pin_opposite", complex);
  reg_handler("rail_connection", complex);
  reg_handler("power_supply_namestring", complex);
  reg_handler("resource_usage", complex);

  // group statements
  reg_handler("bus", new_bus(handler));
  reg_handler("bundle", new_bundle(handler));
  reg_handler("dynamic_current", new_group(handler));
  reg_handler("ff", new_ff(handler));
  reg_handler("ff_bank", new_ff(handler));
  reg_handler("functional_yield_metric", new_group(handler));
  reg_handler("generated_clock", new_group(handler));
  reg_handler("intrinsic_parasitic", new_group(handler));
  reg_handler("latch", new_latch(handler));
  reg_handler("latch_bank", new_latch(handler));
  reg_handler("leakage_current", new_group(handler));
  reg_handler("leakage_power", new_leakage_power(handler));
  reg_handler("lut", new_group(handler));
  reg_handler("mode_definition", new_group(handler));
  reg_handler("pin", new_pin(handler));
  reg_handler("routing_track", new_group(handler));
  reg_handler("statetable", new_statetable(handler));

  reg_handler("internal_power", new_cell_internal_power(handler));

  reg_handler("test_cell", new_test_cell(handler));
}

// @brief デストラクタ
CellHandler::~CellHandler()
{
}

// @brief group statement の最初に呼ばれる関数
// @param[in] attr_token 属性名を表すトークン
// @param[in] value_list 値を表すトークンのリスト
bool
CellHander::begin_group(Token attr_token,
			const vector<Token>& value_list)
{
  if ( value_list.size() != 1 ) {
    FileLoc loc;
    if ( value_list.size() > 1 ) {
      loc = value_list[1].loc();
    }
    else {
      loc = attr_token.loc();
    }
    msg_mgr().put_msg(__FILE__, __LINE__, loc,
		      kMsgError,
		      "DOTLIBPARSER",
		      "cell group requires just one string as a parameter.");
    return false;
  }

  if ( value_list[0].type() != SYMBOL ) {
    msg_mgr().put_msg(__FILE__, __LINE__, value_list[0].loc(),
		      kMsgError,
		      "DOTLIBPARSER",
		      "string value is exprected.");
    return false;
  }
  string name = value_list[0].value();
  mCell = library()->new_cell(name);
}

// @brief group statement の最後に呼ばれる関数
bool
CellHandler::end_group()
{
  mCell = NULL;
}


END_NAMESPACE_YM_CELL_DOTLIB
