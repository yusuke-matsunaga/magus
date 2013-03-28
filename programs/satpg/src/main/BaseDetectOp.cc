
/// @file BaseDetectOp.cc
/// @brief BaseDetectOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "BaseDetectOp.h"
#include "AtpgMgr.h"
#include "FaultMgr.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief 'base' タイプを生成する．
// @param[in] mgr AtpgMgr
DetectOp*
new_BaseDetectOp(AtpgMgr& mgr)
{
  return new BaseDetectOp(mgr._fault_mgr());
}


//////////////////////////////////////////////////////////////////////
// クラス BaseDetectOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fmgr FaultMgr
BaseDetectOp::BaseDetectOp(FaultMgr& fmgr) :
  mMgr(fmgr)
{
}

// @brief デストラクタ
BaseDetectOp::~BaseDetectOp()
{
}

// @brief テストパタンが見つかった時の処理
// @param[in] f 故障
// @param[in] tv テストパタン
void
BaseDetectOp::operator()(TpgFault* f,
			 TestVector* tv)
{
  mMgr.set_status(f, kFsDetected);
}

END_NAMESPACE_YM_SATPG
