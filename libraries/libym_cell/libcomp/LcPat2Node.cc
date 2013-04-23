
/// @file LcPat2Node.cc
/// @brief LcPatMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LcPat2Node.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

//////////////////////////////////////////////////////////////////////
// クラス LcPat2Node
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LcPat2Node::LcPat2Node() :
  mType(0U),
  mLocked(false)
{
}

// @brief デストラクタ
LcPat2Node::~LcPat2Node()
{
}

END_NAMESPACE_YM_CELL_LIBCOMP
