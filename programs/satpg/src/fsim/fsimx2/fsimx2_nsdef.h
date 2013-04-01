#ifndef FSIMX2_NSDEF_H
#define FSIMX2_NSDEF_H

/// @file fsimx2_nsdef.h
/// @brief FsimX2 用の名前空間の定義
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2013 Yusuke Matsunaga
/// All rights reserved.

// 名前空間の定義ファイル
// 必ず先頭でインクルードしなければならない．

// namespace でネストするのがいやなので define マクロでごまかす．

#include "satpg_nsdef.h"

#define BEGIN_NAMESPACE_YM_SATPG_FSIMX2 \
BEGIN_NAMESPACE_YM_SATPG \
BEGIN_NAMESPACE(nsFsimX2)

#define END_NAMESPACE_YM_SATPG_FSIMX2 \
END_NAMESPACE(nsFsimX2) \
END_NAMESPACE_YM_SATPG

#endif // FSIMX2_NSDEF_H
