#ifndef LIBYM_TECHMAP_CELLMAP_LIBDUMP_LIBDUMP_NSDEF_H
#define LIBYM_TECHMAP_CELLMAP_LIBDUMP_LIBDUMP_NSDEF_H

/// @file libym_techmap/cellmap/libdump/libdump_nsdef.h
/// @brief libdump 用の名前空間の定義
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_techmap/cellmap_nsdef.h"

/// @brief libdump 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_CELLMAP_LIBDUMP \
BEGIN_NAMESPACE_YM_CELLMAP \
BEGIN_NAMESPACE(nsLibDump)

/// @brief libdump 用の名前空間の終了
#define END_NAMESPACE_YM_CELLMAP_LIBDUMP \
END_NAMESPACE(nsLibDump) \
END_NAMESPACE_YM_CELLMAP

#endif // LIBYM_TECHMAP_CELLMAP_LIBDUMP_LIBDUMP_NSDEF_H
