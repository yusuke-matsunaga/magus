#ifndef NLSOLVER_H
#define NLSOLVER_H

/// @file NlSolver.h
/// @brief NlSolver のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "nlink_nsdef.h"
#include "YmLogic/SatSolver.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
/// @class NlSolver NlSolver.h "NlSolver.h"
/// @brief number link を解くためのクラス
//////////////////////////////////////////////////////////////////////
class NlSolver
{
public:

  /// @brief コンストラクタ
  NlSolver();

  /// @brief デストラクタ
  ~NlSolver();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 問題を解く
  /// @param[in] problem 問題
  /// @param[out] solution 解
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

  /// @brief 基本的な制約を作る．
  void
  make_base_cnf(SatSolver& solver,
		const NlGraph& graph);

  /// @brief 解を出力する．
  /// @param[in] graph 問題のグラフ
  /// @param[in] model SATの解
  /// @param[in] solution 解
  void
  setup_solution(const NlGraph& graph,
		 const vector<Bool3>& model,
		 NlSolution& solution);

  /// @brief ノードの変数を得る．
  VarId
  node_var(const NlNode* node,
	   ymuint idx) const;

  /// @brief 枝の変数を得る．
  VarId
  edge_var(ymuint edge) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 線分数
  ymuint mNum;

  // 枝の変数の配列
  vector<VarId> mEdgeVarArray;

  // ノードの変数の配列
  vector<VarId> mNodeVarArray;

};

END_NAMESPACE_YM_NLINK

#endif // NLSOLVER_H
