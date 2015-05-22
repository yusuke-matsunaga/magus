#ifndef NLSOLVERGS_H
#define NLSOLVERGS_H

/// @file NlSolverGs.h
/// @brief NlSolverGs のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlSolver.h"
#include "GraphSat.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
/// @class NlSolverGs NlSolverGs.h "NlSolverGs.h"
/// @brief GraphSat を用いた NlSolver
//////////////////////////////////////////////////////////////////////
class NlSolverGs :
  public NlSolver
{
public:

  /// @brief コンストラクタ
  NlSolverGs();

  /// @brief デストラクタ
  virtual
  ~NlSolverGs();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 問題を解く
  /// @param[in] problem 問題
  /// @param[out] solution 解
  virtual
  void
  solve(const NlProblem& problem,
	NlSolution& solution);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いいられるデータ構造
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @grief GraphSat 用のグラフを作る．
  void
  make_graph(const NlGraph& graph,
	     ymuint wire_idx,
	     GsGraphBuilder& builder);

  /// @brief 基本的な制約を作る．
  void
  make_base_cnf(GraphSat& solver,
		const NlGraph& graph);

  /// @brief 解を出力する．
  /// @param[in] graph 問題のグラフ
  /// @param[in] model SATの解
  /// @param[in] solution 解
  void
  setup_solution(const NlGraph& graph,
		 const vector<Bool3>& model,
		 NlSolution& solution);

  /// @brief 枝の変数番号をセットする．
  /// @param[in] edge 枝番号 ( 1 〜 )
  /// @param[in] idx 線分番号
  /// @param[in] var 変数番号
  void
  set_edge_var(ymuint edge,
	       ymuint idx,
	       VarId var);

  /// @brief 枝の変数番号を得る．
  /// @param[in] edge 枝番号 ( 1 〜 )
  /// @param[in] idx 線分番号
  VarId
  edge_var(ymuint edge,
	   ymuint idx);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 線分数
  ymuint mNum;

  // 枝の変数の配列
  // サイズは枝数 x 線分数
  vector<VarId> mVarArray;

};

END_NAMESPACE_YM_NLINK

#endif // NLSOLVER_H
