
/// @file UniVOp.cc
/// @brief UniVOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "UniVOp.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
// クラス UniVOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
UniVOp::UniVOp(CNFddMgrImpl& mgr) :
  Op(mgr)
{
}

// @brief デストラクタ
UniVOp::~UniVOp()
{
}

// @brief 次の GC で回収されるノードに関連した情報を削除する．
void
UniVOp::sweep()
{
  // なにもしない．
}

END_NAMESPACE_YM_CNFDD
