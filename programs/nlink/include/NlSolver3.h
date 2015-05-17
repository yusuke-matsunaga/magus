#ifndef NLSOLVER3_H
#define NLSOLVER3_H

/// @file NlSolver3.h
/// @brief NlSolver3 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "nlink_nsdef.h"
#include "YmLogic/SatSolver.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
/// @class NlSolver3 NlSolver3.h "NlSolver3.h"
/// @brief number link を解くためのクラス
//////////////////////////////////////////////////////////////////////
class NlSolver3
{
public:

  /// @brief コンストラクタ
  NlSolver3();

  /// @brief デストラクタ
  ~NlSolver3();


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

  // 辺を表すデータ構造
  struct Edge
  {

    // 枝を表す文字列を返す．
    string
    str() const;

    // ID番号
    ymuint mId;

    // 縦の辺の時 true
    bool mVertical;

    // 左上の座標
    ymuint mX0;
    ymuint mY0;

    // 変数番号
    VarId mVar;

  };

  // ノードを表すデータ構造
  struct Node
  {

    // 端点番号
    // 0 で無印
    ymuint mEndMark;

    // 接続する辺のリスト
    vector<Edge*> mEdgeList;

    // 変数番号の配列
    vector<VarId> mVarArray;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief 内容を初期化する．
  /// @param[in] solver SAT ソルバ
  /// @param[in] width 幅
  /// @param[in] height 高さ
  /// @param[in] num 線分数
  void
  init(SatSolver& solver,
       ymuint width,
       ymuint height,
       ymuint num);

  /// @brief ノードを得る．
  Node*
  _node(ymuint x,
	ymuint y);

  /// @brief 左の辺を得る．
  /// @param[in] x, y 辺の右端の座標
  Edge*
  left_edge(ymuint x,
	    ymuint y);

  /// @brief 右の辺を得る．
  /// @param[in] x, y 辺の左端の座標
  Edge*
  right_edge(ymuint x,
	     ymuint y);

  /// @brief 上の辺を得る．
  /// @param[in] x, y 辺の下端の座標
  Edge*
  upper_edge(ymuint x,
	     ymuint y);

  /// @brief 下の辺を得る．
  /// @param[in] x, y 辺の上端の座標
  Edge*
  lower_edge(ymuint x,
	     ymuint y);

  /// @brief 部分解リストに追加する．
  void
  add_sol_list(const vector<ymuint>& cur_sol,
	       const vector<bool>& cut_mark,
	       vector<vector<ymuint> >& sol_list,
	       vector<vector<ymuint> >& sig_list);

  /// @brief 解を出力する．
  /// @param[in] model SATの解
  /// @param[in] solution 解
  void
  setup_solution(const vector<Bool3>& model,
		 NlSolution& solution);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 幅
  ymuint mWidth;

  // 高さ
  ymuint mHeight;

  // 線分数
  ymuint mNum;

  // 枝の数．
  ymuint mEdgeNum;

  // 枝の配列
  vector<Edge*> mEdgeArray;

  // 横の辺の配列
  vector<Edge*> mHarray;

  // 縦の辺の配列
  vector<Edge*> mVarray;

  // ノードの配列
  vector<Node*> mNodeArray;


};

END_NAMESPACE_YM_NLINK

#endif // NLSOLVER3_H
