
/// @file CNFddBinOp.cc
/// @brief CNFddBinOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "CNFddBinOp.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
// クラス CNFddBinOpBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
CNFddBinOpBase::CNFddBinOpBase(CNFddMgrImpl& mgr) :
  CNFddOp(mgr)
{
}

// @brief デストラクタ
CNFddBinOpBase::~CNFddBinOpBase()
{
}


//////////////////////////////////////////////////////////////////////
// クラス CNFddBinOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
// @param[in] name テーブル名
CNFddBinOp::CNFddBinOp(CNFddMgrImpl& mgr,
		       const char* name) :
  CNFddBinOpBase(mgr),
  mCompTbl(mgr, name)
{
}

// @brief デストラクタ
CNFddBinOp::~CNFddBinOp()
{
}

// @brief 次の GC で回収されるノードに関連した情報を削除する．
void
CNFddBinOp::sweep()
{
  if ( mCompTbl.used_num() > 0 ) {
    mCompTbl.sweep();
  }
}

END_NAMESPACE_YM_CNFDD
