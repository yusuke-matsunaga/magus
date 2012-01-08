
/// @file ZddBinOp.cc
/// @brief ZddBinOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ZddBinOp.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
// クラス ZddBinOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
// @param[in] name テーブル名
ZddBinOp::ZddBinOp(ZddMgrImpl* mgr,
		   const char* name) :
  ZddOp(mgr),
  mCompTbl(mgr, name)
{
}

// @brief デストラクタ
ZddBinOp::~ZddBinOp()
{
}

// @brief 次の GC で回収されるノードに関連した情報を削除する．
void
ZddBinOp::sweep()
{
  if ( mCompTbl.used_num() > 0 ) {
    mCompTbl.sweep();
  }
}

END_NAMESPACE_YM_ZDD
