#ifndef FSIM2_NSDEF_H
#define FSIM2_NSDEF_H

/// @file fsim_nsdef.h
/// @brief Fsim 用の名前空間の定義
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.

// 名前空間の定義ファイル
// 必ず先頭でインクルードしなければならない．

// namespace でネストするのがいやなので define マクロでごまかす．

#include "satpg_nsdef.h"

#define BEGIN_NAMESPACE_YM_SATPG_FSIM2 \
BEGIN_NAMESPACE_YM_SATPG \
BEGIN_NAMESPACE(nsFsim2)

#define END_NAMESPACE_YM_SATPG_FSIM2 \
END_NAMESPACE(nsFsim2) \
END_NAMESPACE_YM_SATPG

#endif // FSIM2_NSDEF_H
