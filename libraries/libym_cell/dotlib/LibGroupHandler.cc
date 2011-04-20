
/// @file libym_cell/dotlib/LibGroupHandler.cc
/// @brief LibGroupHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LibGroupHandler.h"
#include "LibraryGroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス LibGroupHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] lib_handler 親のハンドラ
LibGroupHandler::LibGroupHandler(LibraryGroupHandler* lib_handler) :
  GroupHandler(lib_handler->parser()),
  mLibHandler(lib_handler)
{
}

// @brief デストラクタ
LibGroupHandler::~LibGroupHandler()
{
}

// 親のハンドラを返す．
LibraryGroupHandler*
LibGroupHandler::lib_handler()
{
  return mLibHandler;
}

END_NAMESPACE_YM_CELL_DOTLIB
