#ifndef NLSOLVER0_H
#define NLSOLVER0_H

/// @file NlSolver0.h
/// @brief NlSolver0 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlSolver.h"
#include "YmLogic/SatSolver.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
/// @class NlSolver0 NlSolver0.h "NlSolver0.h"
/// @brief number link を解くためのクラス
//////////////////////////////////////////////////////////////////////
class NlSolver0 :
  public NlSolver
{
public:

  /// @brief コンストラクタ
  /// @param[in] sat_type SATソルバ名
  NlSolver0(const string& sat_type);

  /// @brief デストラクタ
  virtual
  ~NlSolver0();


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

  /// @brief 経路を求める．
  /// @param[in] node 始点のノード
  /// @param[in] model SATの解
  /// @param[in] path_list 経路上の枝を納めるリスト
  const NlNode*
  search_path(const NlNode* node,
	      const vector<Bool3>& model,
	      vector<const NlEdge*>& path_list);

  /// @brief 解を出力する．
  /// @param[in] graph 問題を表すグラフ
  /// @param[in] model SATの解
  /// @param[in] solution 解
  void
  setup_solution(const NlGraph& graph,
		 const vector<Bool3>& model,
		 NlSolution& solution);

  /// @brief 枝の変数番号を得る．
  /// @param[in] edge 枝
  VarId
  edge_var(const NlEdge* edge);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SATソルバ名
  string mSatType;

  // 幅
  ymuint mWidth;

  // 高さ
  ymuint mHeight;

  // 線分数
  ymuint mNum;

  // 枝の変数番号の配列
  // サイズは枝数
  vector<VarId> mEdgeVarArray;

  // ノードの番号の配列
  vector<ymuint> mNodeArray;

};

END_NAMESPACE_YM_NLINK

#endif // NLSOLVER0_H
