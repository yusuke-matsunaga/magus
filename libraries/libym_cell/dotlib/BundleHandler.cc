
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

#include "PtMgr.h"
#include "PtNode.h"
#include "PtBundle.h"
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
  GroupHandler(parent),
  mBundle(NULL)
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

// @brief simple attribute を設定する．
// @param[in] attr_name 属性名
// @param[in] value 値
// @return 設定が失敗したら false を返す．
bool
BundleHandler::add_simple_attr(const ShString& attr_name,
			       const PtValue* value)
{
#warning "未完"
  return true;
}

// @brief complex attribute を設定する．
// @param[in] attr_name 属性名
// @param[in] value_list 値のリスト
// @return 設定が失敗したら false を返す．
bool
BundleHandler::::add_complex_attr(const ShString& attr_name,
				  const vector<const PtValue*>& value_list)
{
#warning "未完"
  return true;
}

// @brief ピンを追加する．
bool
BundleHandler::add_pin(PtPin* pin)
{
  mBundle->add_pin(pin);
  return false;
}

// @brief group statement の最初に呼ばれる関数
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value_list 値を表すトークンのリスト
bool
BundleHandler::begin_group(const ShString& attr_name,
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
  mBundle = ptmgr().new_ptbundle(value_list[0]->string_value());
  parent()->add_bundle(mBundle);

  return true;
}

// @brief group statement の最後に呼ばれる関数
bool
BundleHandler::end_group()
{
  mBundle = NULL;
  return true;
}

END_NAMESPACE_YM_CELL_DOTLIB
