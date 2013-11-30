#ifndef SAT_ENGINE_NSDEF_H
#define SAT_ENGINE_NSDEF_H

/// @file sat_engine_nsdef.h
/// @brief SatEngine 用の名前空間の定義
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2013 Yusuke Matsunaga
/// All rights reserved.

// 名前空間の定義ファイル
// 必ず先頭でインクルードしなければならない．

// namespace でネストするのがいやなので define マクロでごまかす．

#include "satpg_nsdef.h"

#define BEGIN_NAMESPACE_YM_SATPG_SAT_ENGINE \
BEGIN_NAMESPACE_YM_SATPG		    \
BEGIN_NAMESPACE(nsSatEngine)

#define END_NAMESPACE_YM_SATPG_SAT_ENGINE \
 END_NAMESPACE(nsSatEngine)		  \
END_NAMESPACE_YM_SATPG


BEGIN_NAMESPACE_YM_SATPG_SAT_ENGINE

END_NAMESPACE_YM_SATPG_SAT_ENGINE

#endif // SAT_ENGINE_NSDEF_H
