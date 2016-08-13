#ifndef MCT2_NSDEF_H
#define MCT2_NSDEF_H

/// @file mct2_nsdef.h
/// @brief mct2 サブパッケージに共通な定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap_nsdef.h"


/// @brief mct2 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_LUTMAP_MCT2 \
BEGIN_NAMESPACE_YM_LUTMAP \
BEGIN_NAMESPACE(nsMct2)

/// @brief mct2 用の名前空間の終了
#define END_NAMESPACE_YM_LUTMAP_MCT2 \
END_NAMESPACE(nsMct2) \
END_NAMESPACE_YM_LUTMAP

#endif // MCT2_NSDEF_H
