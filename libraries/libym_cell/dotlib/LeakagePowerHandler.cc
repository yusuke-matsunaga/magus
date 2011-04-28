
/// @file libym_cell/dotlib/LeakagePowerHandler.cc
/// @brief LeakagePowerHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LeakagePowerHandler.h"

#include "SimpleHandler.h"
#include "ComplexHandler.h"
#include "GroupHandler.h"

#include "PtMgr.h"

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
// クラス LeakagePowerHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
LeakagePowerHandler::LeakagePowerHandler(GroupHandler* parent) :
  EmptyGroupHandler(parent)
{
  // simple attributes
  DotlibHandler* simple = new_simple(this);
  reg_handler("power_level", simple);
  reg_handler("related_pg_pin", simple);
  reg_handler("when", simple);
  reg_handler("value", simple);
}

// @brief デストラクタ
LeakagePowerHandler::~LeakagePowerHandler()
{
}

END_NAMESPACE_YM_CELL_DOTLIB
