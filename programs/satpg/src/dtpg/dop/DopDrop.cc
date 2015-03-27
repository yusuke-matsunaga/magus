
/// @file DopDrop.cc
/// @brief DopDrop の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DopDrop.h"
#include "Fsim.h"
#include "FaultMgr.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief 'drop' タイプを生成する．
// @param[in] fmgr 故障マネージャ
// @param[in] fsim 故障シミュレータ
DetectOp*
new_DopDrop(FaultMgr& fmgr,
	    Fsim& fsim)
{
  return new DopDrop(fmgr, fsim);
}


//////////////////////////////////////////////////////////////////////
// クラス DopDrop
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fmgr 故障マネージャ
// @param[in] fsim 故障シミュレータ
DopDrop::DopDrop(FaultMgr& fmgr,
		 Fsim& fsim) :
  mMgr(fmgr),
  mFsim(fsim),
  mOp(fmgr, fsim)
{
}

// @brief デストラクタ
DopDrop::~DopDrop()
{
}

// @brief テストパタンが見つかった時の処理
// @param[in] f 故障
// @param[in] assign_list 値割当のリスト
void
DopDrop::operator()(TpgFault* f,
		    const NodeValList& assign_list)
{
  mFsim.sppfp(assign_list, mOp);
}

END_NAMESPACE_YM_SATPG
