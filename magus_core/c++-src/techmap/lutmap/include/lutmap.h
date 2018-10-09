#ifndef LUTMAP_H
#define LUTMAP_H

/// @file lutmap.h
/// @brief lutmap パッケージ用の名前空間の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "magus.h"


/// @brief lutmap 用の名前空間の開始
#define BEGIN_NAMESPACE_LUTMAP \
BEGIN_NAMESPACE_MAGUS \
BEGIN_NAMESPACE(nsLutmap)

/// @brief lutmap 用の名前空間の終了
#define END_NAMESPACE_LUTMAP \
END_NAMESPACE(nsLutmap) \
END_NAMESPACE_MAGUS

#endif // LUTMAP_H
