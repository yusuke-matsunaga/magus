#ifndef LIBYM_CELL_DOTLIB_PTLIBRARY_H
#define LIBYM_CELL_DOTLIB_PTLIBRARY_H

/// @file libym_cell/dotlib/PtLibrary.h
/// @brief PtLibrary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "dotlib_nsdef.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class PtLibrary PtLibrary.h "PtLibrary.h"
/// @brief library グループを表すパース木の要素
//////////////////////////////////////////////////////////////////////
class PtLibrary
{
public:

  /// @brief コンストラクタ
  /// @param[in] name ライブラリ名
  PtLibrary(const string& name);

  /// @brief デストラクタ
  ~PtLibrary();


public:

  /// @brief セル数を得る．
  ymuint
  cell_num() const;

  /// @brief セルを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < cell_num() )
  const PtCell*
  cell(ymuint pos) const;


};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_PTLIBRARY_H
