#ifndef NLSOLVER2_5_H
#define NLSOLVER2_5_H

/// @file NlSolver2_5.h
/// @brief NlSolver2_5 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlSolver.h"
#include "YmLogic/SatSolver.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
/// @class NlSolver2_5 NlSolver2_5.h "NlSolver2_5.h"
/// @brief number link を解くためのクラス
//////////////////////////////////////////////////////////////////////
class NlSolver2_5 :
  public NlSolver
{
public:

  /// @brief コンストラクタ
  /// @param[in] sat_type SATソルバ名
  NlSolver2_5(const string& sat_type);

  /// @brief デストラクタ
  virtual
  ~NlSolver2_5();


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

  /// @brief 枝の変数番号を得る．
  /// @param[in] edge 枝
  /// @param[in] idx 線分番号
  VarId
  edge_var(const NlEdge* edge,
	   ymuint idx) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SATソルバ名
  string mSatType;

  // 線分数
  ymuint mNum;

  // 線分数の log
  ymuint mLogN;

  // 枝の選択状態を表す変数番号の配列
  vector<VarId> mSelVarArray;

  // 枝の変数番号の配列
  // サイズは枝数 x log(線分数)
  vector<VarId> mVarArray;

};

END_NAMESPACE_YM_NLINK

#endif // NLSOLVER2_5_H
