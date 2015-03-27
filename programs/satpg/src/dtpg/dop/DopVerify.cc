
/// @file DopVerify.cc
/// @brief DopVerify の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DopVerify.h"
#include "Fsim.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief 'verify' タイプを生成する．
// @param[in] fsim 故障シミュレータ
DetectOp*
new_DopVerify(Fsim& fsim)
{
  return new DopVerify(fsim);
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

// @brief テストパタンが見つかった時の処理
// @param[in] f 故障
// @param[in] assign_list 値割当のリスト
void
DopVerify::operator()(TpgFault* f,
		      const AssignList& assign_list)
{
  bool detect = mFsim.spsfp(assign_list, f);
  ASSERT_COND( detect );
}

END_NAMESPACE_YM_SATPG
