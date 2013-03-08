
/// @file NormalOp.cc
/// @brief NormalOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2013 Yusuke Matsunaga
/// All rights reserved.


#include "NormalOp.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス NormalOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NormalOp::NormalOp(FaultMgr& fault_mgr,
		   TvMgr& tvmgr,
		   vector<TestVector*>& tv_list,
		   Fsim& fsim3,
		   bool drop,
		   bool verify) :
  OpBase(fault_mgr, tvmgr, tv_list, fsim3, drop, verify)
{
}

// @brief デストラクタ
NormalOp::~NormalOp()
{
}

END_NAMESPACE_YM_SATPG
