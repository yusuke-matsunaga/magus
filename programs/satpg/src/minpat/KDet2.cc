
/// @file KDet2.cc
/// @brief KDet2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "KDet2.h"
#include "Fsim.h"
#include "TpgFault.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス KDet2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fsm 故障シミュレータ
// @param[in] f_list 故障のリスト
KDet2::KDet2(Fsim& fsim,
	     const vector<TpgFault*>& f_list) :
  mFsim(fsim),
  mOp(fsim, f_list)
{
  mMaxFaultId = 0;
  for (vector<TpgFault*>::const_iterator p = f_list.begin();
       p != f_list.end(); ++ p) {
    TpgFault* f = *p;
    ymuint f_id = f->id();
    if ( mMaxFaultId < f_id ) {
      mMaxFaultId = f_id;
    }
  }
  ++ mMaxFaultId;
}

// @brief デストラクタ
KDet2::~KDet2()
{
}

// @brief 各故障に対して k 回検出するまでは故障シミュレーションを行う．
// @param[in] pat_list パタンのリスト
// @param[in] fault_info_array 故障ごとの情報を収める配列
void
KDet2::run(const vector<TestVector*>& pat_list,
	   vector<FaultInfo>& fault_info_array)
{
  vector<TestVector*> cur_array;
  cur_array.reserve(kPvBitLen);
  ymuint np = pat_list.size();
  ymuint base = 0;
  for (ymuint i = 0; i < np; ++ i) {
    TestVector* tv = pat_list[i];
    cur_array.push_back(tv);
    if ( cur_array.size() == kPvBitLen ) {
      mFsim.ppsfp(cur_array, mOp);
      for (ymuint j = 0; j < kPvBitLen; ++ j) {
	const vector<ymuint>& det_list = mOp.det_list(j);
	for (ymuint k = 0; k < det_list.size(); ++ k) {
	  ymuint f_id = det_list[k];
	  FaultInfo& fi = fault_info_array[f_id];
	  fi.add_pat(base + j);
	  fi.add_fnum(det_list.size());
	}
      }
      cur_array.clear();
      mOp.clear_det_list();
      base += kPvBitLen;
    }
  }
  if ( !cur_array.empty() ) {
    mFsim.ppsfp(cur_array, mOp);
    for (ymuint j = 0; j < cur_array.size(); ++ j) {
      const vector<ymuint>& det_list = mOp.det_list(j);
      for (ymuint k = 0; k < det_list.size(); ++ k) {
	ymuint f_id = det_list[k];
	FaultInfo& fi = fault_info_array[f_id];
	fi.add_pat(base + j);
	fi.add_fnum(det_list.size());
      }
    }
    mOp.clear_det_list();
  }

  for (ymuint i = 0; i < mMaxFaultId; ++ i) {
    fault_info_array[i].sort_pat_list();
  }
}

END_NAMESPACE_YM_SATPG
