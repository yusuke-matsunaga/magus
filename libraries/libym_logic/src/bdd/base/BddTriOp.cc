
/// @file BddTriOp.cc
/// @brief BddTriOp
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "BddTriOp.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// クラス BddTriOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
// @param[in] name テーブル名
BddTriOp::BddTriOp(BddMgrImpl* mgr,
		   const char* name) :
  BddOp(mgr),
  mCompTbl(mgr, name)
{
}

// @brief デストラクタ
BddTriOp::~BddTriOp()
{
}

// @brief 次の GC で回収されるノードに関連した情報を削除する．
void
BddTriOp::sweep()
{
  mCompTbl.sweep();
}

END_NAMESPACE_YM_BDD
