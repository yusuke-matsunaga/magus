#ifndef YM_MISLIB_MISLIB_NSDEF_H
#define YM_MISLIB_MISLIB_NSDEF_H

/// @file libym_mislib/mislib_nsdef.h
/// @brief mislib サブモジュールの定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"


//////////////////////////////////////////////////////////////////////
// 名前空間を定義するためのマクロ
//////////////////////////////////////////////////////////////////////

/// @brief mislib の名前空間の開始
#define BEGIN_NAMESPACE_YM_MISLIB \
BEGIN_NAMESPACE_YM_CELL \
BEGIN_NAMESPACE(nsMislib)

/// @brief mislib の名前空間の終了
#define END_NAMESPACE_YM_MISLIB \
END_NAMESPACE(nsMislib) \
END_NAMESPACE_YM_CELL


BEGIN_NAMESPACE_YM_MISLIB

// クラス名の前方宣言
class MislibMgr;
class MislibNode;
class MislibParser;

END_NAMESPACE_YM_MISLIB

#endif // LIBYM_MISLIB_MISLIB_NSDEF_H
