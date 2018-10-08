#ifndef CELLMAP_NSDEF_H
#define CELLMAP_NSDEF_H

/// @file cellmap_nsdef.h
/// @brief cellmap パッケージに共通な定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "magus.h"


/// @brief cellmap 用の名前空間の開始
#define BEGIN_NAMESPACE_CELLMAP \
BEGIN_NAMESPACE_MAGUS \
BEGIN_NAMESPACE(nsCellmap)

/// @brief cellmap 用の名前空間の終了
#define END_NAMESPACE_CELLMAP \
END_NAMESPACE(nsCellmap) \
END_NAMESPACE_MAGUS


BEGIN_NAMESPACE_CELLMAP

class CellMap;

END_NAMESPACE_CELLMAP

BEGIN_NAMESPACE_MAGUS

using nsCellmap::CellMap;

END_NAMESPACE_MAGUS

#endif // YM_TECHMAP_CELLMAP_NSDEF_H
