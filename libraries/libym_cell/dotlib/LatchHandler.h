#ifndef LIBYM_CELL_DOTLIB_LATCHHANDLER_H
#define LIBYM_CELL_DOTLIB_LATCHHANDLER_H

/// @file libym_cell/dotlib/LatchHandler.h
/// @brief LatchHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "GroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class LatchHandler LatchHandler.h "LatchHandler.h"
/// @brief latch グループのハンドラ
//////////////////////////////////////////////////////////////////////
class LatchHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のハンドラ
  LatchHandler(GroupHandler* parent);

  /// @brief デストラクタ
  virtual
  ~LatchHandler();

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_LATCHHANDLER_H
