
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
//#include "LibraryHandler.h"
//#include "BusHandler.h"
//#include "BundleHandler.h"
//#include "PinHandler.h"

#include "PtMgr.h"
#include "PtNode.h"
#include "PtLeakagePower.h"
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
new_leakage_power(GroupHandler* parent)
{
  GroupHandler* handler = new_group(parent);


  return handler;
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス LeakagePowerHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
LeakagePowerHandler::LeakagePowerHandler(GroupHandler* parent) :
  EmptyGroupHandler(parent),
  mLeakagePower(NULL)
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

// @brief group statement の最初に呼ばれる関数
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
bool
LeakagePowerHandler::begin_group(const ShString& attr_name,
				 const FileRegion& attr_loc)
{
  mLeakagePower = ptmgr().new_ptleakage_power();
  return parent()->add_leakage_power(mLeakagePower);
}

// @brief group statement の最後に呼ばれる関数
bool
LeakagePowerHandler::end_group()
{
  mLeakagePower = NULL;
  return true;
}


END_NAMESPACE_YM_CELL_DOTLIB
