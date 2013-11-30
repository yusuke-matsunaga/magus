#ifndef BB_NSDEF_H
#define BB_NSDEF_H

/// @file bb_nsdef.h
/// @brief BB 用の名前空間の定義
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: atpg_nsdef.h 2128 2009-03-12 07:25:33Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.

// 名前空間の定義ファイル
// 必ず先頭でインクルードしなければならない．

// namespace でネストするのがいやなので define マクロでごまかす．

#include "ymtools.h"

#define BEGIN_NAMESPACE_YM_BB \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsBb)

#define END_NAMESPACE_YM_BB \
END_NAMESPACE(nsBb) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_BB



END_NAMESPACE_YM_BB

#endif // BB_NSDEF_H
