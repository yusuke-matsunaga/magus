
/// @file ZddUniOp.cc
/// @brief ZddUniOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ZddUniOp.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
// クラス ZddUniOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
ZddUniOp::ZddUniOp(ZddMgrImpl* mgr) :
  ZddOp(mgr)
{
}

// @brief デストラクタ
ZddUniOp::~ZddUniOp()
{
}

// @brief 次の GC で回収されるノードに関連した情報を削除する．
void
ZddUniOp::sweep()
{
  // なにもしない．
}

END_NAMESPACE_YM_ZDD
