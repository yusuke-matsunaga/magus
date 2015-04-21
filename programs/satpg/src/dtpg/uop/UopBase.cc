
/// @file UopBase.cc
/// @brief UopBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "UopBase.h"
#include "FaultMgr.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief 'base' タイプを生成する．
// @param[in] fmgr 故障マネージャ
UntestOp*
new_UopBase(FaultMgr& fmgr)
{
  return new UopBase(fmgr);
}


//////////////////////////////////////////////////////////////////////
// クラス UopBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fmgr FaultMgr
UopBase::UopBase(FaultMgr& fmgr) :
  mMgr(fmgr)
{
}

// @brief デストラクタ
UopBase::~UopBase()
{
}

// @brief テスト不能故障と判定された時の処理
// @param[in] f 故障
void
UopBase::operator()(const TpgFault* f)
{
  mMgr.set_status(f, kFsUntestable);
}


END_NAMESPACE_YM_SATPG
