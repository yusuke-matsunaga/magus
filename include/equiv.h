#ifndef EQUIV_H
#define EQUIV_H

/// @file equiv.h
/// @brief equiv の名前空間の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym_config.h"


// 名前空間の定義
#define BEGIN_NAMESPACE_EQUIV \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsEquiv)

#define END_NAMESPACE_EQUIV \
END_NAMESPACE(nsEquiv) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_EQUIV

//////////////////////////////////////////////////////////////////////
// クラス名の前方宣言
//////////////////////////////////////////////////////////////////////

class EquivMgr;

END_NAMESPACE_EQUIV

BEGIN_NAMESPACE_YM

using nsEquiv::EquivMgr;

END_NAMESPACE_YM

#endif // EQUIV_H
