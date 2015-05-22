#ifndef NLSOLVER1_H
#define NLSOLVER1_H

/// @file NlSolver1.h
/// @brief NlSolver1 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlSolver.h"
#include "YmLogic/SatSolver.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
/// @class NlSolver1 NlSolver1.h "NlSolver1.h"
/// @brief number link を解くためのクラス
//////////////////////////////////////////////////////////////////////
class NlSolver1 :
  public NlSolver
{
public:

  /// @brief コンストラクタ
  NlSolver1();

  /// @brief デストラクタ
  virtual
  ~NlSolver1();


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

  /// @brief 基本的な制約を作る．
  void
  make_base_cnf(SatSolver& solver,
		const NlGraph& graph);

  /// @brief 解を出力する．
  /// @param[in] graph 問題を表すグラフ
  /// @param[in] model SATの解
  /// @param[in] solution 解
  void
  setup_solution(const NlGraph& graph,
		 const vector<Bool3>& model,
		 NlSolution& solution);

  /// @brief 枝の変数番号をセットする．
  /// @param[in] edge 枝番号 ( 1 〜 )
  /// @param[in] var 変数番号
  void
  set_edge_var(ymuint edge,
	       VarId var);

  /// @brief 節点の変数番号を得る．
  /// @param[in] node 節点番号
  /// @param[in] idx 線分番号
  VarId
  node_var(ymuint edge,
	   ymuint idx);

  /// @brief 枝の変数番号を得る．
  /// @param[in] edge 枝番号 ( 1 〜 )
  VarId
  edge_var(ymuint edge);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 線分数
  ymuint mNum;

  // 節点の変数番号の配列
  // サイズは節点数 x 線分数
  vector<VarId> mNodeVarArray;

  // 枝の変数番号の配列
  // サイズは枝数
  vector<VarId> mEdgeVarArray;

};

END_NAMESPACE_YM_NLINK

#endif // NLSOLVER1_H
