#ifndef CELLMAP_NSDEF_H
#define CELLMAP_NSDEF_H

/// @file cellmap_nsdef.h
/// @brief cellmap パッケージに共通な定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.

#include "magus.h"


/// @brief cellmap 用の名前空間名
#define CELLMAP_NSNAME nsCellmap

/// @brief cellmap 用の名前空間の開始
#define BEGIN_NAMESPACE_CELLMAP \
BEGIN_NAMESPACE_MAGUS \
BEGIN_NAMESPACE(nsCellmap)

/// @brief cellmap 用の名前空間の終了
#define END_NAMESPACE_CELLMAP \
END_NAMESPACE(nsCellmap) \
END_NAMESPACE_MAGUS

/// @brief cellmap 用の名前空間
#define CELLMAP_NAMESPACE MAGUS_NAMESPACE::CELLMAP_NSNAME

/// @brief cellmap 用の名前空間を表す文字列
#define CELLMAP_NAMESPACE_STR MAGUS_NAMESPACE_STR "::" CELLMAP_NSNAME

BEGIN_NAMESPACE_CELLMAP

END_NAMESPACE_CELLMAP

BEGIN_NAMESPACE_MAGUS

END_NAMESPACE_MAGUS

#endif // YM_TECHMAP_CELLMAP_NSDEF_H
