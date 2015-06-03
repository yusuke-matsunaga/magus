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
  /// @param[in] sat_type SATソルバ名
  NlSolver1(const string& sat_type);

  /// @brief デストラクタ
  virtual
  ~NlSolver1();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 問題を解く
  /// @param[in] graph 問題のグラフ
  /// @param[in] verbose verbose フラグ
  /// @param[out] solution 解
  virtual
  void
  solve(const NlGraph& graph,
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

  /// @brief 節点の変数番号を得る．
  /// @param[in] node 節点
  /// @param[in] idx 線分番号
  VarId
  node_var(const NlNode* node,
	   ymuint idx) const;

  /// @brief 枝の変数番号を得る．
  /// @param[in] edge 枝
  VarId
  edge_var(const NlEdge* edge) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SATソルバ名
  string mSatType;

  // 線分数
  ymuint mNum;

  // mNum の log
  ymuint mLogN;

  // 節点の変数番号の配列
  // サイズは節点数 x log(線分数)
  vector<VarId> mNodeVarArray;

  // 枝の変数番号の配列
  // サイズは枝数
  vector<VarId> mEdgeVarArray;

};

END_NAMESPACE_YM_NLINK

#endif // NLSOLVER1_H
