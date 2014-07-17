
/// @file DopVerify.cc
/// @brief DopVerify の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DopVerify.h"
#include "AtpgMgr.h"
#include "Fsim.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief 'verify' タイプを生成する．
// @param[in] mgr AtpgMgr
DetectOp*
new_DopVerify(AtpgMgr& mgr)
{
  return new DopVerify(mgr._fsim3());
}


//////////////////////////////////////////////////////////////////////
// クラス DopVerify
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fsim 故障シミュレータ
DopVerify::DopVerify(Fsim& fsim) :
  mFsim(fsim)
{
}

// @brief デストラクタ
DopVerify::~DopVerify()
{
}

// @brief 故障リストを設定する．
// @param[in] fault_list 故障リスト
void
DopVerify::set_faults(const vector<TpgFault*>& fault_list)
{
}

// @brief テストパタンが見つかった時の処理
// @param[in] f 故障
// @param[in] tv テストパタン
void
DopVerify::operator()(TpgFault* f,
		      TestVector* tv)
{
  bool detect = mFsim.spsfp(tv, f);
  assert_cond( detect, __FILE__, __LINE__);
}

END_NAMESPACE_YM_SATPG
