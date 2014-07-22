#ifndef MAGUS_NSDEF_H
#define MAGUS_NSDEF_H

/// @file magus_nsdef.h
/// @brief magus用名前空間の定義ファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup MagusGroup Magus 関連クラス

#include "ymtools.h"

#define BEGIN_NAMESPACE_MAGUS \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsMagus)

#define END_NAMESPACE_MAGUS \
END_NAMESPACE(nsMagus) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_MAGUS

// 前方参照のためのクラス宣言
class MagMgr;
class NetHandle;

END_NAMESPACE_MAGUS

#endif // MAGUS_NSDEF_H
