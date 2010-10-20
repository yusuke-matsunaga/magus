
/// @file libym_techmap/patgen/PgNode.cc
/// @brief PgNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "PgNode.h"


BEGIN_NAMESPACE_YM_TECHMAP_PATGEN


//////////////////////////////////////////////////////////////////////
// クラス PgNode
//////////////////////////////////////////////////////////////////////


// @brief コンストラクタ
PgNode::PgNode() :
  mType(0U)
{
  mFanin[0] = NULL;
  mFanin[1] = NULL;
}

// @brief デストラクタ
PgNode::~PgNode()
{
}

END_NAMESPACE_YM_TECHMAP_PATGEN
