
/// @file libym_cell/dotlib/FfHandler.cc
/// @brief FfHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "FfHandler.h"

#include "SimpleHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス FfHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
FfHandler::FfHandler(GroupHandler* parent) :
  GroupHandler(parent->parser(), parent)
{
  DotLibHandler* simple = new SimpleHandler(parser(), this);

  // simple attributes
  reg_handler("clear", simple);
  reg_handler("clear_preset_var1", simple);
  reg_handler("clear_preset_var2", simple);
  reg_handler("clocked_on", simple);
  reg_handler("clocked_on_also", simple);
  reg_handler("next_state", simple);
  reg_handler("preset", simple);
}

// @brief デストラクタ
FfHandler::~FfHandler()
{
}

END_NAMESPACE_YM_CELL_DOTLIB
