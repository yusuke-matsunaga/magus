#ifndef LUTMAP3_NSDEF_H
#define LUTMAP3_NSDEF_H

/// @file lutmap2_nsdef.h
/// @brief luttmap2 パッケージに共通な定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "ym_config.h"


/// @brief lutmap 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_LUTMAP3 \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsLutmap3)

/// @brief lutmap 用の名前空間の終了
#define END_NAMESPACE_YM_LUTMAP3 \
END_NAMESPACE(nsLutmap3) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_LUTMAP3

class LutMap3;

END_NAMESPACE_YM_LUTMAP3

BEGIN_NAMESPACE_YM

using nsLutmap3::LutMap3;

END_NAMESPACE_YM

#endif // LUTMAP3_NSDEF_H
