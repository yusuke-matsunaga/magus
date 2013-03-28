
/// @file VerifyDetectOp.cc
/// @brief VerifyDetectOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "VerifyDetectOp.h"
#include "AtpgMgr.h"
#include "Fsim.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief 'verify' タイプを生成する．
// @param[in] mgr AtpgMgr
DetectOp*
new_VerifyDetectOp(AtpgMgr& mgr)
{
  return new VerifyDetectOp(mgr._fsimx());
}


//////////////////////////////////////////////////////////////////////
// クラス VerifyDetectOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fsim 故障シミュレータ
VerifyDetectOp::VerifyDetectOp(Fsim& fsim) :
  mFsim(fsim)
{
}

// @brief デストラクタ
VerifyDetectOp::~VerifyDetectOp()
{
}

// @brief テストパタンが見つかった時の処理
// @param[in] f 故障
// @param[in] tv テストパタン
void
VerifyDetectOp::operator()(TpgFault* f,
			   TestVector* tv)
{
  bool detect = mFsim.run(tv, f);
  assert_cond( detect, __FILE__, __LINE__);
}

END_NAMESPACE_YM_SATPG
