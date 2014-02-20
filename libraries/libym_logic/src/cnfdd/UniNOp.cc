
/// @file UniNOp.cc
/// @brief UninOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "UniNOp.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
// クラス UniNOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
// @param[in] name テーブル名
UniNOp::UniNOp(CNFddMgrImpl& mgr,
	       const char* name) :
  Op(mgr),
  mCompTbl(mgr, name)
{
}

// @brief デストラクタ
UniNOp::~UniNOp()
{
}

// @brief 次の GC で回収されるノードに関連した情報を削除する．
void
UniNOp::sweep()
{
  if ( mCompTbl.used_num() > 0 ) {
    mCompTbl.sweep();
  }
}

END_NAMESPACE_YM_CNFDD
