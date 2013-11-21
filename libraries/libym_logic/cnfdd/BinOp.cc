
/// @file BinOp.cc
/// @brief BinOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "BinOp.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
// クラス BinOpBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
BinOpBase::BinOpBase(CNFddMgrImpl& mgr) :
  Op(mgr)
{
}

// @brief デストラクタ
BinOpBase::~BinOpBase()
{
}


//////////////////////////////////////////////////////////////////////
// クラス BinOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
// @param[in] name テーブル名
BinOp::BinOp(CNFddMgrImpl& mgr,
	     const char* name) :
  BinOpBase(mgr),
  mCompTbl(mgr, name)
{
}

// @brief デストラクタ
BinOp::~BinOp()
{
}

// @brief 次の GC で回収されるノードに関連した情報を削除する．
void
BinOp::sweep()
{
  if ( mCompTbl.used_num() > 0 ) {
    mCompTbl.sweep();
  }
}

END_NAMESPACE_YM_CNFDD
