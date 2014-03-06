
/// @file Fop2Rofsim.cc
/// @brief Fop2Rofsim の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "Fop2Rofsim.h"
#include "Fsim.h"
#include "FaultMgr.h"
#include "TpgFault.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス Fop2Rofsim
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fsim 故障シミュレータ
// @param[in] fmgr 故障マネージャ
Fop2Rofsim::Fop2Rofsim(Fsim& fsim,
		       FaultMgr& fmgr) :
  mFsim(fsim),
  mDetArray(fmgr.all_num())
{
  mDetBits = 0UL;
}

// @brief デストラクタ
Fop2Rofsim::~Fop2Rofsim()
{
}

// @brief 故障を検出したときの処理
// @param[in] f 故障
// @param[in] dpat 検出したパタンを表すビットベクタ
void
Fop2Rofsim::operator()(TpgFault* f,
		       PackedVal dpat)
{
  ymuint f_id = f->id();
  if ( mDetArray[f_id] ) {
    return;
  }
  mDetArray[f_id] = true;
  mFsim.set_skip(f);
  for (ymuint i = 0; i < kPvBitLen; ++ i) {
    if ( dpat & (1UL << i) ) {
      mDetBits |= (1UL << i);
      break;
    }
  }
}

// @brief 検出回数をクリアする．
void
Fop2Rofsim::clear_count()
{
  ymuint n = mDetArray.size();
  for (ymuint i = 0; i < n; ++ i) {
    mDetArray[i] = false;
  }
  mFsim.clear_skip();
}

// @brief 今のパタンで新たに故障検出が行えたかを表すビットベクタを返す．
PackedVal
Fop2Rofsim::det_bits()
{
  PackedVal ans = mDetBits;
  mDetBits = 0UL;
  return ans;
}

END_NAMESPACE_YM_SATPG
