#ifndef LIBYM_CELL_DOTLIB_TIMINGHANDLER_H
#define LIBYM_CELL_DOTLIB_TIMINGHANDLER_H

/// @file libym_cell/dotlib/TimingHandler.h
/// @brief TimingHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "GroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class TimingHandler TimingHandler.h "TimingHandler.h"
//////////////////////////////////////////////////////////////////////
class TimingHandler :
  public EmptyGroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のハンドラ
  TimingHandler(GroupHandler* parent);

  /// @brief デストラクタ
  ~TimingHandler();

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_TIMINGHANDLER_H
