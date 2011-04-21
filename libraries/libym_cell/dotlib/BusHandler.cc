
/// @file libym_cell/dotlib/BusHandler.cc
/// @brief BusHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BusHandler.h"

#include "SimpleHandler.h"

#include "PinHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス BusHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
BusHandler::BusHandler(GroupHandler* parent) :
  GroupHandler(parent->parser(), parent)
{
  DotLibHandler* simple = new SimpleHandler(parser(), this);

  // simple attributes
  reg_handler("bus_type", simple);

  // group statements
  reg_handler("pin", new PinHandler(this));

}

// @brief デストラクタ
BusHandler::~BusHandler()
{
}

END_NAMESPACE_YM_CELL_DOTLIB
