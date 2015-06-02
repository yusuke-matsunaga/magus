#ifndef NLSOLVERFR_H
#define NLSOLVERFR_H

/// @file NlSolverFr.h
/// @brief NlSolverFr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlSolver.h"
#include "YmLogic/SatSolver.h"


BEGIN_NAMESPACE_YM_NLINK

class FrontierInfo;

//////////////////////////////////////////////////////////////////////
/// @class NlSolverFr NlSolverFr.h "NlSolverFr.h"
/// @brief number link を解くためのクラス
//////////////////////////////////////////////////////////////////////
class NlSolverFr :
  public NlSolver
{
public:

  /// @brief コンストラクタ
  NlSolverFr();

  /// @brief デストラクタ
  virtual
  ~NlSolverFr();


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

  /// @brief 枝の順序付けを行う．
  void
  ordering(const NlGraph& graph,
	   vector<const NlEdge*>& edge_list);

  /// @brief フロンティアの更新を行う．
  /// @param[inout] frontier_nodes フロンティアノードのリスト
  /// @param[in] edge 新しく加わる枝
  /// @param[in] del_list 削除されるノードのフロンティアリスト上の位置
  /// @param[out] pos1, pos2 枝の両端のノードの位置
  ///
  /// pos1, pos2 は該当のノードがない場合には -1 を返す．
  void
  calc_frontier(vector<const NlNode*>& frontier_nodes,
		const NlEdge* edge,
		ymuint max_node_id,
		vector<ymuint>& del_list,
		int& pos1,
		int& pos2);

  /// @brief フロンティアの更新を行う．
  bool
  update_frontier(FrontierInfo& fr,
		  const NlEdge* edge,
		  bool selected,
		  const NlGraph& graph);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 枝の順序を記憶する配列
  vector<ymuint> mEdgeOrder;

};

END_NAMESPACE_YM_NLINK

#endif // NLSOLVERFR_H
