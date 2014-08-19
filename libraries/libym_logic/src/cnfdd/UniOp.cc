
/// @file UniOp.cc
/// @brief UniOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "UniOp.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
// クラス UniOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
// @param[in] name テーブル名
UniOp::UniOp(CNFddMgrImpl& mgr,
	     const char* name) :
  Op(mgr),
  mCompTbl(mgr, name)
{
}

// @brief デストラクタ
UniOp::~UniOp()
{
}

// @brief 次の GC で回収されるノードに関連した情報を削除する．
void
UniOp::sweep()
{
  if ( mCompTbl.used_num() > 0 ) {
    mCompTbl.sweep();
  }
}

END_NAMESPACE_YM_CNFDD
