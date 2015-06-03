#ifndef NLSOLVERDFS_H
#define NLSOLVERDFS_H

/// @file NlSolverDfs.h
/// @brief NlSolverDfs のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlSolver.h"
#include "NlSolution.h"


BEGIN_NAMESPACE_YM_NLINK

struct Ban
{
  Ban(const NlGraph& graph);

  bool
  select(ymuint edge_id);

  bool
  unselect(ymuint edge_id);

  const NlGraph& mGraph;

  vector<ymuint8> mEdgeArray;

  vector<ymint8> mNodeArray;

};


//////////////////////////////////////////////////////////////////////
/// @class NlSolverDfs NlSolverDfs.h "NlSolverDfs.h"
/// @brief number link を解くためのクラス
//////////////////////////////////////////////////////////////////////
class NlSolverDfs :
  public NlSolver
{
public:

  /// @brief コンストラクタ
  NlSolverDfs();

  /// @brief デストラクタ
  virtual
  ~NlSolverDfs();


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
	NlSolution& solution);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 探索を行う．
  bool
  dfs_recur(const Ban& ban);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  NlSolution mSolution;

};

END_NAMESPACE_YM_NLINK

#endif // NLSOLVERDFS_H
