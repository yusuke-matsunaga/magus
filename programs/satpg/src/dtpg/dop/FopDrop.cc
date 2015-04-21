
/// @file FopDrop.cc
/// @brief FopDrop の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "FopDrop.h"
#include "FaultMgr.h"
#include "Fsim.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス FopDrop
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fmgr 故障マネージャ
// @param[in] fsim 故障シミュレータ
FopDrop::FopDrop(FaultMgr& fmgr,
		 Fsim& fsim) :
  mMgr(fmgr),
  mFsim(fsim)
{
}

// @brief デストラクタ
FopDrop::~FopDrop()
{
}

// @brief 故障を検出したときの処理
// @param[in] f 故障
// @param[in] dpat 検出したパタンを表すビットベクタ
void
FopDrop::operator()(const TpgFault* f,
		    PackedVal dpat)
{
  mMgr.set_status(f, kFsDetected);
  mFsim.set_skip(f);
}

END_NAMESPACE_YM_SATPG
