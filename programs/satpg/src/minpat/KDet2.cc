
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

BEGIN_NONAMESPACE

// fault_list 中の故障番号の最大値+1を返す．
ymuint
get_max_id(const vector<TpgFault*>& fault_list)
{
  ymuint max_fault_id = 0;
  ymuint num = fault_list.size();
  for (ymuint i = 0; i < num; ++ i) {
    TpgFault* f = fault_list[i];
    if ( max_fault_id < f->id() ) {
      max_fault_id = f->id();
    }
  }
  ++ max_fault_id;
  return max_fault_id;
}

END_NONAMESPACE

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
// @param[in] pat_list_array 各故障を検出するパタン番号のリストの配列
void
KDet2::run(const vector<TestVector*>& pat_list,
	   vector<vector<ymuint> >& pat_list_array)
{
  vector<TestVector*> cur_array;
  cur_array.reserve(kPvBitLen);
  ymuint np = pat_list.size();
  pat_list_array.clear();
  pat_list_array.resize(mMaxFaultId);
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
	  pat_list_array[f_id].push_back(base + j);
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
	pat_list_array[f_id].push_back(base + j);
      }
    }
    mOp.clear_det_list();
  }

  for (ymuint i = 0; i < mMaxFaultId; ++ i) {
    vector<ymuint>& pat_list = pat_list_array[i];
    sort(pat_list.begin(), pat_list.end());
  }
}

END_NAMESPACE_YM_SATPG
