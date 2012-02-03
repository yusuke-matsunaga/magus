
/// @file CNFddUniVOp.cc
/// @brief CNFddUniVOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "CNFddUniVOp.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
// クラス CNFddUniVOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
CNFddUniVOp::CNFddUniVOp(CNFddMgrImpl& mgr) :
  CNFddOp(mgr)
{
}

// @brief デストラクタ
CNFddUniVOp::~CNFddUniVOp()
{
}

// @brief 次の GC で回収されるノードに関連した情報を削除する．
void
CNFddUniVOp::sweep()
{
  // なにもしない．
}

END_NAMESPACE_YM_CNFDD
