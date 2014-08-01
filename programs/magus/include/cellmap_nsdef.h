#ifndef YM_TECHMAP_CELLMAP_NSDEF_H
#define YM_TECHMAP_CELLMAP_NSDEF_H

/// @file ym_techmap/cellmap_nsdef.h
/// @brief cellmap パッケージに共通な定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"


/// @brief cellmap 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_CELLMAP \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsCellmap)

/// @brief cellmap 用の名前空間の終了
#define END_NAMESPACE_YM_CELLMAP \
END_NAMESPACE(nsCellmap) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_CELLMAP

class CellMap;

END_NAMESPACE_YM_CELLMAP

BEGIN_NAMESPACE_YM

using nsCellmap::CellMap;

END_NAMESPACE_YM

#endif // YM_TECHMAP_CELLMAP_NSDEF_H
