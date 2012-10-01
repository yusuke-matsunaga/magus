#ifndef GBM_NSDEF_H
#define GBM_NSDEF_H

/// @file gbm_nsdef.h
/// @brief GBM 用の名前空間の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


// 名前空間の定義
#define BEGIN_NAMESPACE_YM_GBM \
BEGIN_NAMESPACE_YM				\
BEGIN_NAMESPACE(nsGbm)

#define END_NAMESPACE_YM_GBM			\
END_NAMESPACE(nsGbm) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_GBM

class Gbm;

END_NAMESPACE_YM_GBM


BEGIN_NAMESPACE_YM

using nsGbm::Gbm;

END_NAMESPACE_YM

#endif // GBM_NSDEF_H
