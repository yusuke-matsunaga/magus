#ifndef LIBDUMP_NSDEF_H
#define LIBDUMP_NSDEF_H

/// @file libdump_nsdef.h
/// @brief libdump 用の名前空間の定義
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"

/// @brief libdump 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_CELL_LIBDUMP \
BEGIN_NAMESPACE_YM_CELL \
BEGIN_NAMESPACE(nsLibdump)

/// @brief libdump 用の名前空間の終了
#define END_NAMESPACE_YM_CELL_LIBDUMP \
END_NAMESPACE(nsLibdump) \
END_NAMESPACE_YM_CELL

#endif // LIBDUMP_NSDEF_H
