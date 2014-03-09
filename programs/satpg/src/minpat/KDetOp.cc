
/// @file KDetOp.cc
/// @brief KDetOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "KDetOp.h"
#include "Fsim.h"
#include "FaultMgr.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス KDetOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fsim 故障シミュレータ
// @param[in] fmgr 故障マネージャ
KDetOp::KDetOp(Fsim& fsim,
	       FaultMgr& fmgr) :
  mFsim(fsim),
  mLimit(1),
  mDetCount(fmgr.all_num())
{
  mFsim.set_faults(fmgr.det_list());
}

// @brief デストラクタ
KDetOp::~KDetOp()
{
}

// @brief 故障を検出したときの処理
// @param[in] f 故障
// @param[in] dpat 検出したパタンを表すビットベクタ
void
KDetOp::operator()(TpgFault* f,
		   PackedVal dpat)
{
  ymuint f_id = f->id();
  for (ymuint i = 0; i < kPvBitLen; ++ i) {
    if ( dpat & (1UL << i) ) {
      mDetListArray[i].push_back(f_id);
      ++ mDetCount[f_id];
      if ( mDetCount[f_id] >= mLimit ) {
	mFsim.set_skip(f);
	break;
      }
    }
  }
}

// @brief 検出回数をクリアする．
void
KDetOp::clear_count()
{
  ymuint n = mDetCount.size();
  for (ymuint i = 0; i < n; ++ i) {
    mDetCount[i] = 0;
  }
}

// @brief 検出回数のしきい値をセットする．
void
KDetOp::set_limit(ymuint limit)
{
  mLimit = limit;
}

// @brief 検出された故障のID番号のリストを返す．
const vector<ymuint>&
KDetOp::det_list(ymuint pos)
{
  return mDetListArray[pos % kPvBitLen];
}

// @brief det_list をクリアする．
void
KDetOp::clear_det_list()
{
  for (ymuint i = 0; i < kPvBitLen; ++ i) {
    mDetListArray[i].clear();
  }
}

END_NAMESPACE_YM_SATPG
