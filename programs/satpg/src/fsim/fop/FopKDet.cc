
/// @file FopKDet.cc
/// @brief FopKDet の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "FopKDet.h"
#include "Fsim.h"
#include "FaultMgr.h"
#include "TpgFault.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス FopKDet
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fsim 故障シミュレータ
// @param[in] fmgr 故障マネージャ
FopKDet::FopKDet(Fsim& fsim,
		     FaultMgr& fmgr) :
  mFsim(fsim),
  mLimit(1),
  mPatListArray(fmgr.all_num())
{
  mFsim.set_faults(fmgr.det_list());
}

// @brief デストラクタ
FopKDet::~FopKDet()
{
}

// @brief 故障を検出したときの処理
// @param[in] f 故障
// @param[in] dpat 検出したパタンを表すビットベクタ
void
FopKDet::operator()(TpgFault* f,
		      PackedVal dpat)
{
  ymuint f_id = f->id();
  vector<TestVector*>& pat_list = mPatListArray[f_id];
  for (ymuint i = 0; i < kPvBitLen; ++ i) {
    if ( dpat & (1UL << i) ) {
      pat_list.push_back(mCurPatList[i]);
      if ( pat_list.size() >= mLimit ) {
	// 規定回数以上検出されたので以後のシミュレーションではスキップする．
	mFsim.set_skip(f);
	break;
      }
    }
  }
}

// @brief 検出回数をクリアする．
void
FopKDet::clear_count()
{
  ymuint n = mPatListArray.size();
  for (ymuint i = 0; i < n; ++ i) {
    mPatListArray[i].clear();
  }
}

// @brief 検出回数のしきい値をセットする．
void
FopKDet::set_limit(ymuint limit)
{
  mLimit = limit;
}

// @brief パタンをセットする．
void
FopKDet::set_pattern(const vector<TestVector*>& pat_list)
{
  mCurPatList = pat_list;
}

// @brief 故障に対するパタンのリストを返す．
// @param[in] f_id 故障の ID
const vector<TestVector*>&
FopKDet::pat_list(ymuint f_id)
{
  return mPatListArray[f_id];
}

END_NAMESPACE_YM_SATPG
