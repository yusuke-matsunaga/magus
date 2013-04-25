
/// @file LcPatNode.cc
/// @brief LcPatMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LcPatNode.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

//////////////////////////////////////////////////////////////////////
// クラス LcPatNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LcPatNode::LcPatNode() :
  mType(0U),
  mSignature("0"),
  mLocked(false)
{
  mFanin[0] = NULL;
  mFanin[1] = NULL;
}

// @brief デストラクタ
LcPatNode::~LcPatNode()
{
}

// @brief 'lock' する．
// @note ファンインに再帰する．
void
LcPatNode::set_locked()
{
  if ( !mLocked ) {
    mLocked = true;
    if ( !is_input() ) {
      fanin(0)->set_locked();
      fanin(1)->set_locked();
    }
  }
}

END_NAMESPACE_YM_CELL_LIBCOMP
