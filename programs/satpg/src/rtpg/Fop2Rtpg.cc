
/// @file Fop2Rtpg.cc
/// @brief Fop2Rtpg の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "Fop2Rtpg.h"
#include "Fsim.h"
#include "FaultMgr.h"
#include "TpgFault.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス Fop2Rtpg
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fsim 故障シミュレータ
// @param[in] fmgr 故障マネージャ
Fop2Rtpg::Fop2Rtpg(Fsim& fsim,
		   FaultMgr& fmgr) :
  mFsim(fsim),
  mFaultMgr(fmgr)
{
  clear_count();
}

// @brief デストラクタ
Fop2Rtpg::~Fop2Rtpg()
{
}

// @brief 故障を検出したときの処理
// @param[in] f 故障
// @param[in] dpat 検出したパタンを表すビットベクタ
void
Fop2Rtpg::operator()(TpgFault* f,
		     PackedVal dpat)
{
  if ( f->status() == kFsUndetected ) {
    mFaultMgr.set_status(f, kFsDetected);
  }
  mFsim.set_skip(f);
  for (ymuint i = 0; i < kPvBitLen; ++ i) {
    if ( dpat & (1UL << i) ) {
      ++ mCount[i];
      break;
    }
  }
}

// @brief 検出回数をクリアする．
void
Fop2Rtpg::clear_count()
{
  for (ymuint i = 0; i < kPvBitLen; ++ i) {
    mCount[i] = 0;
  }
}

/// @brief 検出回数を得る．
/// @param[in] bitpos ビット位置
ymuint
Fop2Rtpg::count(ymuint bitpos)
{
  return mCount[bitpos];
}

END_NAMESPACE_YM_SATPG
