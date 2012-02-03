
/// @file CNFddUniOp.cc
/// @brief CNFddUniOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "CNFddUniOp.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
// クラス CNFddUniOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
CNFddUniOp::CNFddUniOp(CNFddMgrImpl& mgr) :
  CNFddOp(mgr)
{
}

// @brief デストラクタ
CNFddUniOp::~CNFddUniOp()
{
}

// @brief 次の GC で回収されるノードに関連した情報を削除する．
void
CNFddUniOp::sweep()
{
  // なにもしない．
}

END_NAMESPACE_YM_CNFDD
