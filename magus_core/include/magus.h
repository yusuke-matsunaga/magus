#ifndef MAGUS_H
#define MAGUS_H

/// @file magus.h
/// @brief magus用名前空間の定義ファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2018 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup MagusGroup Magus 関連クラス

#include "ym_config.h"
#include "version_config.h"


#define BEGIN_NAMESPACE_MAGUS \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsMagus)

#define END_NAMESPACE_MAGUS \
END_NAMESPACE(nsMagus) \
END_NAMESPACE_YM

#define MAGUS_NAMESPACE YM_NAMESPACE::MAGUS_NSNAME

#define MAGUS_NAMESPACE_STR YM_NSNAME_STR "::" MAGUS_NSNAME_STR


BEGIN_NAMESPACE_MAGUS

// 前方参照のためのクラス宣言
class MagMgr;
class NetHandle;

END_NAMESPACE_MAGUS

#endif // MAGUS_H
