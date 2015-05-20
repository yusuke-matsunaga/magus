#ifndef GSGRAPH_H
#define GSGRAPH_H

/// @file GsGraph.h
/// @brief GsGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "nlink_nsdef.h"
#include "YmLogic/VarId.h"


BEGIN_NAMESPACE_YM_NLINK

class GsNode;
class GsEdge;

//////////////////////////////////////////////////////////////////////
/// @class GsGraphBuilder GsGraphBuilder.h "GsGraphBuilder.h"
/// @brief GsGraph を作るためのビルダクラス
//////////////////////////////////////////////////////////////////////
class GsGraphBuilder
{
public:

  /// @brief コンストラクタ
  /// @param[in] node_num ノード数
  GsGraphBuilder(ymuint node_num);

  /// @brief デストラクタ
  ~GsGraphBuilder();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 枝を追加する．
  /// @param[in] node1_id, node2_id 両端のノード番号
  /// @param[in] var 枝の変数番号
  void
  add_edge(ymuint node1_id,
	   ymuint node2_id,
	   VarId var);

  /// @brief ノード数を返す．
  ymuint
  node_num() const;

  /// @brief ノードの枝番号リストを返す．
  /// @param[in] pos ノードの番号 ( 0 <= pos < node_num() )
  const vector<ymuint>&
  edge_list(ymuint pos) const;

  /// @brief 枝数を返す．
  ymuint
  edge_num() const;

  /// @brief 枝の一方のノード番号を返す．
  /// @param[in] pos 枝の番号 ( 0 <= pos < edge_num() )
  ymuint
  node1(ymuint pos) const;

  /// @brief 枝の他方のノード番号を返す．
  /// @param[in] pos 枝の番号 ( 0 <= pos < edge_num() )
  ymuint
  node2(ymuint pos) const;

  /// @brief 枝の変数番号を返す．
  VarId
  var(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード数
  ymuint mNodeNum;

  // ノードごとの枝番号リスト
  vector<vector<ymuint> > mEdgeListArray;

  // 枝の一方のノード番号のリスト
  vector<ymuint> mNode1List;

  // 枝の他方のノード番号のリスト
  vector<ymuint> mNode2List;

  // 枝の変数番号のリスト
  vector<VarId> mVarIdList;

};


//////////////////////////////////////////////////////////////////////
/// @class GsGraph GsGraph.h "GsGraph.h"
/// @brief GraphSat のグラフを表すクラス
//////////////////////////////////////////////////////////////////////
class GsGraph
{
public:

  /// @brief コンストラクタ
  /// @param[in] builder ビルダオブジェクト
  GsGraph(const GsGraphBuilder& builder);

  /// @brief デストラクタ
  ~GsGraph();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を返す．
  ymuint
  node_num() const;

  /// @brief ノードを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < node_num() )
  GsNode*
  node(ymuint pos);

  /// @brief 枝数を返す．
  ymuint
  edge_num() const;

  /// @brief 枝を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos edge_num() )
  GsEdge*
  edge(ymuint pos);

  /// @brief 始点を返す．
  GsNode*
  start_node();

  /// @brief 終点を返す．
  GsNode*
  end_node();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード数
  ymuint mNodeNum;

  // ノード本体の配列
  GsNode* mNodeArray;

  // 枝数
  ymuint mEdgeNum;

  // 枝の配列
  GsEdge* mEdgeArray;

  // 始点のノード
  GsNode* mStartNode;

  // 終点のノード
  GsNode* mEndNode;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ノード数を返す．
inline
ymuint
GsGraph::node_num() const
{
  return mNodeNum;
}

// @brief 枝数を返す．
inline
ymuint
GsGraph::edge_num() const
{
  return mEdgeNum;
}

// @brief 始点を返す．
inline
GsNode*
GsGraph::start_node()
{
  return mStartNode;
}

// @brief 終点を返す．
inline
GsNode*
GsGraph::end_node()
{
  return mEndNode;
}

END_NAMESPACE_YM_NLINK

#endif // GSGRAPH_H
