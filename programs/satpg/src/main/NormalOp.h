#ifndef NORMALOP_H
#define NORMALOP_H

/// @file NormalOp.h
/// @brief NormalOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2013 Yusuke Matsunaga
/// All rights reserved.


#include "OpBase.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class NormalOp NormalOp.h "NormalOp.h"
/// @brief 通常の TpgOperator
//////////////////////////////////////////////////////////////////////
class NormalOp :
  public OpBase
{
public:

  /// @brief コンストラクタ
  NormalOp(FaultMgr& fault_mgr,
	   TvMgr& tvmgr,
	   vector<TestVector*>& tv_list,
	   Fsim& fsim3,
	   bool drop = false,
	   bool verify = false);

  /// @brief デストラクタ
  ~NormalOp();

};

END_NAMESPACE_YM_SATPG

#endif // NORMALOP_H
