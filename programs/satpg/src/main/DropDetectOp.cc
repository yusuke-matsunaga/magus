
/// @file DropDetectOp.cc
/// @brief DropDetectOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "DropDetectOp.h"
#include "AtpgMgr.h"
#include "Fsim.h"
#include "FaultMgr.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief 'drop' タイプを生成する．
// @param[in] mgr AtpgMgr
DetectOp*
new_DropDetectOp(AtpgMgr& mgr)
{
  return new DropDetectOp(mgr._fault_mgr(), mgr._fsimx());
}


//////////////////////////////////////////////////////////////////////
// クラス DropDetectOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fmgr 故障マネージャ
// @param[in] fsim 故障シミュレータ
DropDetectOp::DropDetectOp(FaultMgr& fmgr,
			   Fsim& fsim) :
  mMgr(fmgr),
  mFsim(fsim)
{
}

// @brief デストラクタ
DropDetectOp::~DropDetectOp()
{
}

// @brief テストパタンが見つかった時の処理
// @param[in] f 故障
// @param[in] tv テストパタン
void
DropDetectOp::operator()(TpgFault* f,
			 TestVector* tv)
{
  mFsim.run(tv, mDetFaults);
  for (vector<TpgFault*>::iterator p = mDetFaults.begin();
       p != mDetFaults.end(); ++ p) {
    TpgFault* f = *p;
    mMgr.set_status(f, kFsDetected);
  }
}

END_NAMESPACE_YM_SATPG
