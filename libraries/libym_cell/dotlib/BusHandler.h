#ifndef LIBYM_CELL_DOTLIB_BUSHANDLER_H
#define LIBYM_CELL_DOTLIB_BUSHANDLER_H

/// @file libym_cell/dotlib/BusHandler.h
/// @brief BusHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "GroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class BusHandler BusHandler.h "BusHandler.h"
/// @brief bus グループのハンドラ
//////////////////////////////////////////////////////////////////////
class BusHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のハンドラ
  BusHandler(GroupHandler* parent);

  /// @brief デストラクタ
  virtual
  ~BusHandler();

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_BUSHANDLER_H
