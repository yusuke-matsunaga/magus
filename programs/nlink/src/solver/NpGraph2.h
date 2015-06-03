#ifndef NPGRAPH2_H
#define NPGRAPH2_H

/// @file NpGraph2.h
/// @brief NpGraph2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "nlink_nsdef.h"
#include "YmLogic/VarId.h"
#include "YmLogic/SatSolver.h"


BEGIN_NAMESPACE_YM_NLINK

class NpNode;
class NpEdge;

//////////////////////////////////////////////////////////////////////
/// @class NpGraph2 NpGraph2.h "NpGraph2.h"
/// @brief nonpolar equality graph を表すクラス
//////////////////////////////////////////////////////////////////////
class NpGraph2
{
public:

  /// @brief コンストラクタ
  /// @param[in] solver SATソルバ
  /// @param[in] src_graph もとになるグラフ
  /// @param[in] edge_map 枝の変数番号の配列
  NpGraph2(SatSolver& solver,
	   const NlGraph& src_graph,
	   ymuint group_num,
	   const vector<ymuint>& group_map,
	   const vector<VarId>& edge_map);

  /// @brief デストラクタ
  ~NpGraph2();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief chordal graph に変形する．
  /// @param[in] solver SAT ソルバ
  void
  make_chordal(SatSolver& solver);

  /// @brief short-cut 枝を追加する．
  /// @param[in] solver SATソルバ
  /// @param[in] src_graph もとになるグラフ
  /// @param[in] x1, y1 ノード1の座標
  /// @param[in] x1, y2 ノード2の座標
  void
  add_shortcut(SatSolver& solver,
	       const NlGraph& src_graph,
	       ymuint x1,
	       ymuint y1,
	       ymuint x2,
	       ymuint y2,
	       ymuint g);

  /// @brief 等価制約を追加する．
  /// @param[in] solver SATソルバ
  /// @param[in] node1, node2 両端のノード
  /// @param[in] disequality 非等価制約の時 true にするフラグ
  void
  add_equality(SatSolver& solver,
	       NpNode* node1,
	       NpNode* node2,
	       bool disequality);

  /// @brief node1 と node2 を結ぶ枝を探す．
  /// @param[in] node1, node2 両端のノード
  ///
  /// なければ NULL を返す．
  NpEdge*
  find_edge(NpNode* node1,
	    NpNode* node2) const;

  /// @brief node1 と node2 を結ぶ枝を作る．
  /// @param[in] node1, node2 両端のノード
  /// @param[in] var 変数番号
  /// @param[in] aux 追加枝の場合に true にするフラグ
  NpEdge*
  connect(NpNode* node1,
	  NpNode* node2,
	  VarId var,
	  bool aux);

  /// @brief ノードを取り出す．
  /// @param[in] node_id ノード番号
  /// @param[in] group グループ番号
  NpNode*
  _node(ymuint node_id,
	ymuint group);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 配線グループ数
  ymuint mGroupNum;

  // ノードの配列
  vector<NpNode*> mNodeArray;

  // オリジナルの枝のリスト
  vector<NpEdge*> mOrigEdgeList;

  // 追加した枝のリスト
  vector<NpEdge*> mAuxEdgeList;

};

END_NAMESPACE_YM_NLINK

#endif // NPGRAPH2_H
