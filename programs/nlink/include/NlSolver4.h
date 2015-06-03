#ifndef NLSOLVER4_H
#define NLSOLVER4_H

/// @file NlSolver4.h
/// @brief NlSolver4 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlSolver.h"
#include "YmLogic/SatSolver.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
/// @class NlSolver4 NlSolver4.h "NlSolver4.h"
/// @brief number link を解くためのクラス
//////////////////////////////////////////////////////////////////////
class NlSolver4 :
  public NlSolver
{
public:

  /// @brief コンストラクタ
  /// @param[in] sat_type SATソルバ名
  NlSolver4(const string& sat_type);

  /// @brief デストラクタ
  virtual
  ~NlSolver4();


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

  /// @brief 経路を求める．
  /// @param[in] node 始点のノード
  /// @param[in] model SATの解
  const NlNode*
  search_path(const NlNode* node,
	      const vector<Bool3>& model);

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

  // グループ数
  ymuint mGroupNum;

  // 線分のグループ番号
  vector<ymuint> mGroupMap;

  // 枝の変数番号の配列
  // サイズは枝数 x 線分数
  vector<VarId> mVarArray;

  // ノードの番号の配列
  vector<ymuint> mNodeArray;

};

END_NAMESPACE_YM_NLINK

#endif // NLSOLVER4_H
