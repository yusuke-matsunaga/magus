#ifndef LIBYM_CELL_DOTLIB_DOTLIBMGR_H
#define LIBYM_CELL_DOTLIB_DOTLIBMGR_H

/// @file libym_cell/dotlib/DotlibMgr.h
/// @brief DotlibMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "dotlib_nsdef.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class DotlibMgr DotlibMgr.h "DotlibMgr.h"
/// @brief パース木から CellLibrary を生成するクラス
//////////////////////////////////////////////////////////////////////
class DotlibMgr
{
public:

  /// @brief コンストラクタ
  DotlibMgr();

  /// @brief デストラクタ
  ~DotlibMgr();


public:

  /// @brief パース木から CellLibrary を生成する．
  /// @param[in] root パース木の根のノード
  const CellLibrary*
  gen_library(const PtNode* root);

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_DOTLIBMGR_H
