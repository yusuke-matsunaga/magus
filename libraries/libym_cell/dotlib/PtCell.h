#ifndef LIBYM_CELL_DOTLIB_PTCELL_H
#define LIBYM_CELL_DOTLIB_PTCELL_H

/// @file libym_cell/dotlib/PtCell.h
/// @brief PtCell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "dotlib_nsdef.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class PtCell PtCell.h "PtCell.h"
/// @brief cell グループを表すパース木の要素
//////////////////////////////////////////////////////////////////////
class PtCell
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  PtCell(const string& name);

  /// @brief デストラクタ
  ~PtCell();


public:

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_PTCELL_H
