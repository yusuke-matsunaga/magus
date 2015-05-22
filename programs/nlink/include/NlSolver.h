#ifndef NLSOLVER_H
#define NLSOLVER_H

/// @file NlSolver.h
/// @brief NlSolver のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "nlink_nsdef.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
/// @class NlSolver NlSolver.h "NlSolver.h"
/// @brief number link を解くためのクラス
//////////////////////////////////////////////////////////////////////
class NlSolver
{
public:

  /// @brief デストラクタ
  virtual
  ~NlSolver() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 問題を解く
  /// @param[in] problem 問題
  /// @param[in] verbose verbose フラグ
  /// @param[out] solution 解
  virtual
  void
  solve(const NlProblem& problem,
	bool verbose,
	NlSolution& solution) = 0;

};

END_NAMESPACE_YM_NLINK

#endif // NLSOLVER_H
