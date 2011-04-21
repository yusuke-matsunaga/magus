#ifndef LIBYM_CELL_DOTLIB_FFHANDLER_H
#define LIBYM_CELL_DOTLIB_FFHANDLER_H

/// @file libym_cell/dotlib/FfHandler.h
/// @brief FfHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "GroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class FfHandler FfHandler.h "FfHandler.h"
/// @brief ff グループのハンドラ
//////////////////////////////////////////////////////////////////////
class FfHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のハンドラ
  FfHandler(GroupHandler* parent);

  /// @brief デストラクタ
  virtual
  ~FfHandler();

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_FFHANDLER_H
