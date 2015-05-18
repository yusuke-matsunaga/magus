#ifndef NLSOLVER2_H
#define NLSOLVER2_H

/// @file NlSolver2.h
/// @brief NlSolver2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "nlink_nsdef.h"
#include "YmLogic/SatSolver.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
/// @class NlSolver2 NlSolver2.h "NlSolver2.h"
/// @brief number link を解くためのクラス
//////////////////////////////////////////////////////////////////////
class NlSolver2
{
public:

  /// @brief コンストラクタ
  NlSolver2();

  /// @brief デストラクタ
  ~NlSolver2();


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
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 基本的な制約を作る．
  void
  make_base_cnf(SatSolver& solver,
		const NlGraph& graph,
		vector<VarId>& con_array);

  /// @brief 自明な線分を引いたうえで解を求める．
  void
  trivial_route(ymuint k,
		const NlConnection& con,
		vector<Literal>& assumption);

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

  // 枝の変数番号の配列
  // サイズは枝数 x 線分数
  vector<VarId> mVarArray;

};

END_NAMESPACE_YM_NLINK

#endif // NLSOLVER2_H
