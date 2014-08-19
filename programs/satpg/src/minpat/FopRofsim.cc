
/// @file FopRofsim.cc
/// @brief FopRofsim の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "FopRofsim.h"
#include "Fsim.h"
#include "FaultMgr.h"
#include "TpgFault.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス FopRofsim
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fsim 故障シミュレータ
// @param[in] f_list 故障のリスト
// @param[in] max_fault_id 故障IDの最大値+1
FopRofsim::FopRofsim(Fsim& fsim,
		     const vector<TpgFault*>& f_list,
		     ymuint max_fault_id) :
  mFsim(fsim),
  mDetArray(max_fault_id)
{
  mDetBits = 0UL;
  mFsim.set_faults(f_list);
}

// @brief デストラクタ
FopRofsim::~FopRofsim()
{
}

// @brief 故障を検出したときの処理
// @param[in] f 故障
// @param[in] dpat 検出したパタンを表すビットベクタ
void
FopRofsim::operator()(TpgFault* f,
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
FopRofsim::clear_count()
{
  ymuint n = mDetArray.size();
  for (ymuint i = 0; i < n; ++ i) {
    mDetArray[i] = false;
  }
}

// @brief 今のパタンで新たに故障検出が行えたかを表すビットベクタを返す．
PackedVal
FopRofsim::det_bits()
{
  PackedVal ans = mDetBits;
  mDetBits = 0UL;
  return ans;
}

END_NAMESPACE_YM_SATPG
