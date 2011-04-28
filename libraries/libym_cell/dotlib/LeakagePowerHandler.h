#ifndef LIBYM_CELL_DOTLIB_LEAKAGEPOWERHANDLER_H
#define LIBYM_CELL_DOTLIB_LEAKAGEPOWERHANDLER_H

/// @file libym_cell/dotlib/LeakagePowerHandler.h
/// @brief LeakagePowerHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "GroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class LeakagePowerHandler LeakagePowerHandler.h "LeakagePowerHandler.h"
/// @brief leakage_power group 用のハンドラ
//////////////////////////////////////////////////////////////////////
class LeakagePowerHandler :
  public EmptyGroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のハンドラ
  LeakagePowerHandler(GroupHandler* parent);

  /// @brief デストラクタ
  virtual
  ~LeakagePowerHandler();

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_LEAKAGEPOWERHANDLER_H
