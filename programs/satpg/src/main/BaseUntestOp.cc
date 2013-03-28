
/// @file BaseUntestOp.cc
/// @brief BaseUntestOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "BaseUntestOp.h"
#include "AtpgMgr.h"
#include "FaultMgr.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief 'base' タイプを生成する．
// @param[in] mgr AtpgMgr
UntestOp*
new_BaseUntestOp(AtpgMgr& mgr)
{
  return new BaseUntestOp(mgr._fault_mgr());
}


//////////////////////////////////////////////////////////////////////
// クラス BaseUntestOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fmgr FaultMgr
BaseUntestOp::BaseUntestOp(FaultMgr& fmgr) :
  mMgr(fmgr)
{
}

// @brief デストラクタ
BaseUntestOp::~BaseUntestOp()
{
}

// @brief テスト不能故障と判定された時の処理
// @param[in] f 故障
void
BaseUntestOp::operator()(TpgFault* f)
{
  mMgr.set_status(f, kFsUntestable);
}


END_NAMESPACE_YM_SATPG
