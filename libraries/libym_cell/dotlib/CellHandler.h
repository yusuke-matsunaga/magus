#ifndef LIBYM_CELL_DOTLIB_CELLHANDLER_H
#define LIBYM_CELL_DOTLIB_CELLHANDLER_H

/// @file libym_cell/dotlib/CellHandler.h
/// @brief CellHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "GroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class CellHandler CellHandler.h "CellHandler.h"
/// @brief cell group 用のハンドラ
//////////////////////////////////////////////////////////////////////
class CellHandler :
  public Str1GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のハンドラ
  CellHandler(GroupHandler* parent);

  /// @brief デストラクタ
  virtual
  ~CellHandler();

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_CELLHANDLER_H
