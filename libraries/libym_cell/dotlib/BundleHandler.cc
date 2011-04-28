
/// @file libym_cell/dotlib/BundleHandler.cc
/// @brief BundleHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BundleHandler.h"

#include "SimpleHandler.h"
#include "ComplexHandler.h"
#include "GroupHandler.h"
#include "CellHandler.h"
#include "PinHandler.h"
#include "TimingHandler.h"


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
  return new GroupHandler(parent);
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
// クラス BundleHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
BundleHandler::BundleHandler(GroupHandler* parent) :
  Str1GroupHandler(parent)
{
  // simple attributes
  DotlibHandler* simple = new_simple(this);
  reg_handler("capacitance", simple);
  reg_handler("direction", simple);
  reg_handler("function", simple);

  // complex attributes
  DotlibHandler* complex = new_complex(this);
  reg_handler("members", complex);

  // group statements
  reg_handler("pin", new PinHandler(this));
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
BundleHandler::~BundleHandler()
{
}

END_NAMESPACE_YM_CELL_DOTLIB
