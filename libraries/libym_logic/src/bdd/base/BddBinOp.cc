
/// @file BddBinOp.cc
/// @brief BddBinOp
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "BddBinOp.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// クラス BddBinOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
// @param[in] name テーブル名
BddBinOp::BddBinOp(BddMgrImpl* mgr,
		   const char* name) :
  BddOp(mgr),
  mCompTbl(mgr, name)
{
}

// @brief デストラクタ
BddBinOp::~BddBinOp()
{
}

// @brief 次の GC で回収されるノードに関連した情報を削除する．
void
BddBinOp::sweep()
{
  mCompTbl.sweep();
}

END_NAMESPACE_YM_BDD
