#ifndef MCT1_NSDEF_H
#define MCT1_NSDEF_H

/// @file mct1_nsdef.h
/// @brief mct1 サブパッケージに共通な定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap_nsdef.h"


/// @brief lutmap 用の名前空間の開始
#define BEGIN_NAMESPACE_LUTMAP_MCT1 \
BEGIN_NAMESPACE_LUTMAP \
BEGIN_NAMESPACE(nsMct1)

/// @brief lutmap 用の名前空間の終了
#define END_NAMESPACE_LUTMAP_MCT1 \
END_NAMESPACE(nsMct1) \
END_NAMESPACE_LUTMAP

#endif // MCT1_NSDEF_H
