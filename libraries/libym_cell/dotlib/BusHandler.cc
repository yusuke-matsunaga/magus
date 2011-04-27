
/// @file libym_cell/dotlib/BusHandler.cc
/// @brief BusHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BusHandler.h"

#include "SimpleHandler.h"
#include "ComplexHandler.h"
#include "GroupHandler.h"
#include "CellHandler.h"
#include "PinHandler.h"

#include "PtMgr.h"
#include "PtNode.h"
#include "PtBus.h"
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

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス BusHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
BusHandler::BusHandler(GroupHandler* parent) :
  GroupHandler(parent),
  mBus(NULL)
{
  // simple attributes
  DotlibHandler* simple = new_simple(this);
  reg_handler("bus_type", simple);

  // group statements
  reg_handler("pin", new PinHandler(this));
}

// @brief デストラクタ
BusHandler::~BusHandler()
{
}

// @brief attribute を設定する．
// @param[in] attr_name 属性名
// @param[in] value 値
// @return 設定が失敗したら false を返す．
bool
BusHandler::add_attr(const ShString& attr_name,
		     PtValue* value)
{
  if ( attr_name == "bus_type" && value->type() == PtValue::kString ) {
    mBus->mBusType = value->string_value();
    return true;
  }
#if 0
  return PinHandler::add_attr(attr_name, value);
#endif
  return true;
}

// @brief ピンを追加する．
bool
BusHandler::add_pin(PtPin* pin)
{
  return mBus->add_pin(pin);
}

// @brief group statement の最初に呼ばれる関数
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value_list 値を表すトークンのリスト
bool
BusHandler::begin_group(const ShString& attr_name,
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
	    "DOTLIB_PARSER",
	    "cell group requires just one string as a parameter.");
    return false;
  }

  if ( value_list[0]->type() != PtValue::kString ) {
    put_msg(__FILE__, __LINE__, value_list[0]->loc(),
	    kMsgError,
	    "DOTLIB_PARSER",
	    "string value is exprected.");
    return false;
  }
  mBus = ptmgr().new_ptbus(value_list[0]->string_value());
  parent()->add_bus(mBus);

  return true;
}

// @brief group statement の最後に呼ばれる関数
bool
BusHandler::end_group()
{
  mBus = NULL;
  return true;
}


END_NAMESPACE_YM_CELL_DOTLIB
