
/// @file FopRtpg.cc
/// @brief FopRtpg の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "FopRtpg.h"
#include "Fsim.h"
#include "FaultMgr.h"
#include "TpgFault.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス Fop2Rtpg
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fsim 故障シミュレータ
FopRtpg::FopRtpg(Fsim& fsim) :
  mFsim(fsim)
{
  clear_count();
}

// @brief デストラクタ
FopRtpg::~FopRtpg()
{
}

// @brief 故障を検出したときの処理
// @param[in] f 故障
// @param[in] dpat 検出したパタンを表すビットベクタ
void
FopRtpg::operator()(const TpgFault* f,
		    PackedVal dpat)
{
  mFaultList.push_back(f);
  mFsim.set_skip(f);
  for (ymuint i = 0; i < kPvBitLen; ++ i) {
    if ( dpat & (1UL << i) ) {
      ++ mCount[i];
      break;
    }
  }
}

// @brief 初期化する．
void
FopRtpg::init()
{
  clear_count();
  mFaultList.clear();
}

// @brief 検出回数をクリアする．
void
FopRtpg::clear_count()
{
  for (ymuint i = 0; i < kPvBitLen; ++ i) {
    mCount[i] = 0;
  }
}

/// @brief 検出回数を得る．
/// @param[in] bitpos ビット位置
ymuint
FopRtpg::count(ymuint bitpos)
{
  return mCount[bitpos];
}

// @brief 検出された故障のリストを得る．
const vector<const TpgFault*>&
FopRtpg::fault_list() const
{
  return mFaultList;
}

END_NAMESPACE_YM_SATPG
