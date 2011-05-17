#ifndef LIBYM_TECHMAP_CELLMAP_CELLMAP_NSDEF_H
#define LIBYM_TECHMAP_CELLMAP_CELLMAP_NSDEF_H

/// @file libym_techmap/cellmap/cell_nsdef.h
/// @brief cellhmap パッケージに共通な定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: lutmap.h 555 2007-04-08 07:20:30Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_techmap/techmap_nsdef.h"


/// @brief cellmap 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_TECHMAP_CELLMAP \
BEGIN_NAMESPACE_YM_TECHMAP \
BEGIN_NAMESPACE(nsCellmap)

/// @brief cellmap 用の名前空間の終了
#define END_NAMESPACE_YM_TECHMAP_CELLMAP \
END_NAMESPACE(nsCellmap) \
END_NAMESPACE_YM_TECHMAP

BEGIN_NAMESPACE_YM_TECHMAP_CELLMAP


END_NAMESPACE_YM_TECHMAP_CELLMAP

BEGIN_NAMESPACE_YM_TECHMAP

class CellMapImpl;

END_NAMESPACE_YM_TECHMAP

#endif // LIBYM_TECHMAP_CELLMAP_CELLMAP_NSDEF_H
