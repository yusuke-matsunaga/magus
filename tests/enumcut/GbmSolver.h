#ifndef GBMSOLVER_H
#define GBMSOLVER_H

/// @file GbmSolver.h
/// @brief GbmSolver のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "GbmMgr.h"
#include "GbmNodeHandle.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GbmSolver GbmSolver.h "GbmSolver.h"
/// @brief GBM を解くクラス
//////////////////////////////////////////////////////////////////////
class GbmSolver
{
public:

  /// @brief デストラクタ
  virtual
  ~GbmSolver() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 問題を解く
  virtual
  bool
  solve(const GbmMgr& mgr,
	GbmNodeHandle output1,
	GbmNodeHandle output2) = 0;

};

END_NAMESPACE_YM

#endif // GBMSOLVER_H
