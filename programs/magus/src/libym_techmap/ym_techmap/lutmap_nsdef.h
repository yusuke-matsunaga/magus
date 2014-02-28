#ifndef YM_TECHMAP_LUTMAP_NSDEF_H
#define YM_TECHMAP_LUTMAP_NSDEF_H

/// @file ym_techmap/lutmap_nsdef.h
/// @brief luttmap パッケージに共通な定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: lutmap.h 555 2007-04-08 07:20:30Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"

#include "ym_utils/DlList.h"


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

typedef DlList<LnEdge> LnEdgeList;
typedef DlList<LnNode> LnNodeList;

END_NAMESPACE_YM_LUTMAP

BEGIN_NAMESPACE_YM

using nsLutmap::LutMap;

using nsLutmap::LnGraph;
using nsLutmap::LnPort;
using nsLutmap::LnEdge;
using nsLutmap::LnNode;

using nsLutmap::LnEdgeList;
using nsLutmap::LnNodeList;

END_NAMESPACE_YM

#endif // YM_TECHMAP_LUTMAP_NSDEF_H
