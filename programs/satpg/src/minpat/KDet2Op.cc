
/// @file KDet2Op.cc
/// @brief KDet2Op の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "KDet2Op.h"
#include "Fsim.h"
#include "TpgFault.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス KDet2Op
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fsim 故障シミュレータ
// @param[in] f_list 故障のリスト
KDet2Op::KDet2Op(Fsim& fsim,
		 const vector<TpgFault*>& f_list) :
  mFsim(fsim)
{
  mFsim.set_faults(f_list);
}

// @brief デストラクタ
KDet2Op::~KDet2Op()
{
}

// @brief 故障を検出したときの処理
// @param[in] f 故障
// @param[in] dpat 検出したパタンを表すビットベクタ
void
KDet2Op::operator()(TpgFault* f,
		    PackedVal dpat)
{
  ymuint f_id = f->id();
  for (ymuint i = 0; i < kPvBitLen; ++ i) {
    if ( dpat & (1UL << i) ) {
      mDetListArray[i].push_back(f_id);
    }
  }
}

// @brief 検出された故障のID番号のリストを返す．
const vector<ymuint>&
KDet2Op::det_list(ymuint pos)
{
  return mDetListArray[pos % kPvBitLen];
}

// @brief det_list をクリアする．
void
KDet2Op::clear_det_list()
{
  for (ymuint i = 0; i < kPvBitLen; ++ i) {
    mDetListArray[i].clear();
  }
}

END_NAMESPACE_YM_SATPG
