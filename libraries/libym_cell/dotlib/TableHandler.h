#ifndef LIBYM_CELL_DOTLIB_TABLEHANDLER_H
#define LIBYM_CELL_DOTLIB_TABLEHANDLER_H

/// @file libym_cell/dotlib/TableHandler.h
/// @brief TableHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "GroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

class TimingHandler;

//////////////////////////////////////////////////////////////////////
/// @class TableHandler TableHandler.h "TableHandler.h"
//////////////////////////////////////////////////////////////////////
class TableHandler :
  public Str1GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のハンドラ
  TableHandler(GroupHandler* parent);

  /// @brief デストラクタ
  ~TableHandler();

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_TABLEHANDLER_H
