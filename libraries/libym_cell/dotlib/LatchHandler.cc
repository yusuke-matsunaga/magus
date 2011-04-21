
/// @file libym_cell/dotlib/LatchHandler.cc
/// @brief LatchHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LatchHandler.h"

#include "SimpleHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス LatchHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
LatchHandler::LatchHandler(GroupHandler* parent) :
  GroupHandler(parent->parser())
{
  DotLibHandler* simple = new SimpleHandler(parser(), this);

  // simple attributes
  reg_handler("clear", simple);
  reg_handler("clear_preset_var1", simple);
  reg_handler("clear_preset_var2", simple);
  reg_handler("data_in", simple);
  reg_handler("enable", simple);
  reg_handler("enable_also", simple);
  reg_handler("preset", simple);
}

// @brief デストラクタ
LatchHandler::~LatchHandler()
{
}

END_NAMESPACE_YM_CELL_DOTLIB
