#ifndef FSIM3_NSDEF_H
#define FSIM3_NSDEF_H

/// @file fsim3_nsdef.h
/// @brief Fsim3 用の名前空間の定義
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: fsim3_nsdef.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.

// 名前空間の定義ファイル
// 必ず先頭でインクルードしなければならない．

// namespace でネストするのがいやなので define マクロでごまかす．

#include "satpg_nsdef.h"

#define BEGIN_NAMESPACE_YM_SATPG_FSIM3 \
BEGIN_NAMESPACE_YM_SATPG \
BEGIN_NAMESPACE(nsFsim3)

#define END_NAMESPACE_YM_SATPG_FSIM3 \
END_NAMESPACE(nsFsim3) \
END_NAMESPACE_YM_SATPG


BEGIN_NAMESPACE_YM_SATPG_FSIM3

class Fsim3;

END_NAMESPACE_YM_SATPG_FSIM3

BEGIN_NAMESPACE_YM_SATPG

using nsFsim3::Fsim3;

extern
Fsim3*
new_Fsim3();

END_NAMESPACE_YM_SATPG

#endif // FSIM3_NSDEF_H
