#ifndef FSIMX_NSDEF_H
#define FSIMX_NSDEF_H

/// @file fsimx_nsdef.h
/// @brief FsimX 用の名前空間の定義
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.

// 名前空間の定義ファイル
// 必ず先頭でインクルードしなければならない．

// namespace でネストするのがいやなので define マクロでごまかす．

#include "satpg_nsdef.h"

#define BEGIN_NAMESPACE_YM_SATPG_FSIMX \
BEGIN_NAMESPACE_YM_SATPG \
BEGIN_NAMESPACE(nsFsimX)

#define END_NAMESPACE_YM_SATPG_FSIMX \
END_NAMESPACE(nsFsimX) \
END_NAMESPACE_YM_SATPG

#endif // FSIMX_NSDEF_H
