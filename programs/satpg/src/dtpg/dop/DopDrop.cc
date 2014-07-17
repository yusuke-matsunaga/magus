
/// @file DopDrop.cc
/// @brief DopDrop の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DopDrop.h"
#include "AtpgMgr.h"
#include "Fsim.h"
#include "FaultMgr.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief 'drop' タイプを生成する．
// @param[in] mgr AtpgMgr
DetectOp*
new_DopDrop(AtpgMgr& mgr)
{
  return new DopDrop(mgr._fault_mgr(), mgr._fsim3());
}


//////////////////////////////////////////////////////////////////////
// クラス DopDrop
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fmgr 故障マネージャ
// @param[in] fsim 故障シミュレータ
DopDrop::DopDrop(FaultMgr& fmgr,
		 Fsim& fsim) :
  mMgr(fmgr),
  mFsim(fsim),
  mOp(fmgr, fsim)
{
}

// @brief デストラクタ
DopDrop::~DopDrop()
{
}

// @brief テストパタンが見つかった時の処理
// @param[in] f 故障
// @param[in] tv テストパタン
void
DopDrop::operator()(TpgFault* f,
		    TestVector* tv)
{
  mFsim.sppfp(tv, mOp);
}

END_NAMESPACE_YM_SATPG
