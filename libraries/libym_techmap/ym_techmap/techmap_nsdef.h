#ifndef YM_TECHMAP_TECHMAP_NSDEF_H
#define YM_TECHMAP_TECHMAP_NSDEF_H

/// @file libym_techmap/ym_sbj/sbj_nsdef.h
/// @brief techmap パッケージに共通な定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: lutmap.h 555 2007-04-08 07:20:30Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "ym_sbj/sbj_nsdef.h"
#include "ym_utils/DlList.h"


/// @brief techmap 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_TECHMAP \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsTechmap)

/// @brief techmap 用の名前空間の終了
#define END_NAMESPACE_YM_TECHMAP \
END_NAMESPACE(nsTechmap) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_TECHMAP

class TechMap;

class CnGraph;
class CnEdge;
class CnNode;

typedef DlList<CnEdge> CnEdgeList;
typedef DlList<CnNode> CnNodeList;

END_NAMESPACE_YM_TECHMAP

BEGIN_NAMESPACE_YM

using nsTechmap::TechMap;

using nsTechmap::CnGraph;
using nsTechmap::CnEdge;
using nsTechmap::CnNode;

using nsTechmap::CnEdgeList;
using nsTechmap::CnNodeList;

END_NAMESPACE_YM

#endif // YM_TECHMAP_TECHMAP_NSDEF_H
