
/// @file BddUniOp.cc
/// @brief BddUniOp
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "BddUniOp.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// クラス BddUniOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
BddUniOp::BddUniOp(BddMgrImpl* mgr) :
  BddOp(mgr)
{
}

// @brief デストラクタ
BddUniOp::~BddUniOp()
{
}

// @brief 次の GC で回収されるノードに関連した情報を削除する．
void
BddUniOp::sweep()
{
  mCompTbl.clear();
}

END_NAMESPACE_YM_BDD
