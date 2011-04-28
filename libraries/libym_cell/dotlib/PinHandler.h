#ifndef LIBYM_CELL_DOTLIB_PINHANDLER_H
#define LIBYM_CELL_DOTLIB_PINHANDLER_H

/// @file libym_cell/dotlib/PinHandler.h
/// @brief PinHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "GroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class PinHandler PinHandler.h "PinHandler.h"
//////////////////////////////////////////////////////////////////////
class PinHandler :
  public Str1GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のハンドラ
  PinHandler(GroupHandler* parent);

  /// @brief デストラクタ
  ~PinHandler();

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_PINHANDLER_H
