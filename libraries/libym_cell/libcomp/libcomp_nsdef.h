#ifndef LIBCOMP_NSDEF_H
#define LIBCOMP_NSDEF_H

/// @file libcomp_nsdef.h
/// @brief libcomp 用の名前空間の定義
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"

/// @brief libcomp 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_CELL_LIBCOMP \
BEGIN_NAMESPACE_YM_CELL \
BEGIN_NAMESPACE(nsLibcump)

/// @brief libcomp 用の名前空間の終了
#define END_NAMESPACE_YM_CELL_LIBCOMP \
END_NAMESPACE(nsLibcump) \
END_NAMESPACE_YM_CELL


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

class LcClass;
class LcGroup;

END_NAMESPACE_YM_CELL_LIBCOMP

#endif // LIBCOMP_NSDEF_H
