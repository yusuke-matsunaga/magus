
/// @file KDet.cc
/// @brief KDet の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "KDet.h"
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
// クラス KDet
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fsm 故障シミュレータ
// @param[in] f_list 故障のリスト
KDet::KDet(Fsim& fsim,
	   const vector<TpgFault*>& f_list) :
  mFsim(fsim),
  mOp(fsim, f_list, get_max_id(f_list))
{
}

// @brief デストラクタ
KDet::~KDet()
{
}

// @brief 各故障に対して k 回検出するまでは故障シミュレーションを行う．
// @param[in] pat_list パタンのリスト
// @param[in] k_limit 検出回数のしきい値
// @param[in] det_list_array 各パタンが検出する故障番号のリストの配列
void
KDet::run(const vector<TestVector*>& pat_list,
	  ymuint k_limit,
	  vector<vector<ymuint> >& det_list_array)
{
  mOp.clear_count();
  mOp.set_limit(k_limit);

  vector<TestVector*> cur_array;
  cur_array.reserve(kPvBitLen);
  ymuint np = pat_list.size();
  det_list_array.clear();
  det_list_array.resize(np);
  ymuint wpos = 0;
  for (ymuint i = 0; i < np; ++ i) {
    TestVector* tv = pat_list[i];
    cur_array.push_back(tv);
    if ( cur_array.size() == kPvBitLen ) {
      mFsim.ppsfp(cur_array, mOp);
      for (ymuint j = 0;j < kPvBitLen; ++ j) {
	det_list_array[wpos] = mOp.det_list(j);
	++ wpos;
      }
      cur_array.clear();
      mOp.clear_det_list();
    }
  }
  if ( !cur_array.empty() ) {
    mFsim.ppsfp(cur_array, mOp);
    for (ymuint j = 0; j < cur_array.size(); ++ j) {
      det_list_array[wpos] = mOp.det_list(j);
      ++ wpos;
    }
    mOp.clear_det_list();
  }
  ASSERT_COND( wpos == np );
}

END_NAMESPACE_YM_SATPG
