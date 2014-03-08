
/// @file DopDropNew.cc
/// @brief DopDropNew の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DopDropNew.h"
#include "AtpgMgr.h"
#include "FsimNew.h"
#include "FaultMgr.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief 'drop' タイプを生成する．
// @param[in] mgr AtpgMgr
DetectOp*
new_DopDropNew(AtpgMgr& mgr)
{
  return new DopDropNew(mgr._fault_mgr(), mgr._fsimnew3());
}


//////////////////////////////////////////////////////////////////////
// クラス DopDropNew
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fmgr 故障マネージャ
// @param[in] fsim 故障シミュレータ
DopDropNew::DopDropNew(FaultMgr& fmgr,
		       FsimNew& fsim) :
  mMgr(fmgr),
  mFsim(fsim)
{
}

// @brief デストラクタ
DopDropNew::~DopDropNew()
{
}

// @brief テストパタンが見つかった時の処理
// @param[in] f 故障
// @param[in] tv テストパタン
void
DopDropNew::operator()(TpgFault* f,
		       TestVector* tv)
{
  mFsim.sppfp(tv, mMgr.remain_list(), mDetFaults);
  for (vector<TpgFault*>::iterator p = mDetFaults.begin();
       p != mDetFaults.end(); ++ p) {
    TpgFault* f = *p;
    mMgr.set_status(f, kFsDetected);
  }
}

END_NAMESPACE_YM_SATPG
