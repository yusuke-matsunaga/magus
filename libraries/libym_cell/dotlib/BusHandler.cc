
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

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス BusHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
BusHandler::BusHandler(GroupHandler* parent) :
  Str1GroupHandler(parent)
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

END_NAMESPACE_YM_CELL_DOTLIB
