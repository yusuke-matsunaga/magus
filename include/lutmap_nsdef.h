#ifndef LUTMAP_NSDEF_H
#define LUTMAP_NSDEF_H

/// @file lutmap_nsdef.h
/// @brief luttmap パッケージに共通な定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "ym_config.h"


/// @brief lutmap 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_LUTMAP \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsLutmap)

/// @brief lutmap 用の名前空間の終了
#define END_NAMESPACE_YM_LUTMAP \
END_NAMESPACE(nsLutmap) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_LUTMAP

class LutMap;

class LnGraph;
class LnPort;
class LnEdge;
class LnNode;

END_NAMESPACE_YM_LUTMAP

BEGIN_NAMESPACE_YM

using nsLutmap::LutMap;
using nsLutmap::LnGraph;
using nsLutmap::LnPort;
using nsLutmap::LnEdge;
using nsLutmap::LnNode;

END_NAMESPACE_YM

#endif // LUTMAP_NSDEF_H
