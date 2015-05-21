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

  /// @brief 始点を指定する．
  /// @param[in] node_id 始点のノード番号
  void
  set_start_node(ymuint node_id);

  /// @brief 終点を指定する．
  /// @param[in] node_id 終点のノード番号
  void
  set_end_node(ymuint node_id);

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

  /// @brief 始点のノード番号を返す．
  ymuint
  start_node() const;

  /// @brief 終点のノード番号を返す．
  ymuint
  end_node() const;

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

  // 始点のノード番号
  ymuint mStartId;

  // 終点のノード番号
  ymuint mEndId;

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

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  GsGraph(const GsGraph& src);

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
  start_node() const;

  /// @brief 終点を返す．
  GsNode*
  end_node() const;

  /// @brief 'update' フラグを得る．
  bool
  needs_update() const;

  /// @brief 'update' フラグを消す．
  void
  clear_update();

  /// @brief 'update' フラグを立てる．
  void
  set_update();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数と枝数を設定する．
  /// @param[in] node_num ノード数
  /// @param[in] edge_num
  void
  set_size(ymuint node_num,
	   ymuint edge_num);


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

  // 更新が必要なことを示すフラグ
  bool mUpdate;

};


/// @relates GsGraph
/// @brief デバッグ用にGsGraph の内容を出力する．
/// @param[in] s 出力ストリーム
/// @param[in] graph グラフ
void
print_graph(ostream& s,
	    GsGraph& graph);


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
GsGraph::start_node() const
{
  return mStartNode;
}

// @brief 終点を返す．
inline
GsNode*
GsGraph::end_node() const
{
  return mEndNode;
}

// @brief 'update' フラグを得る．
inline
bool
GsGraph::needs_update() const
{
  return mUpdate;
}

// @brief 'update' フラグを消す．
inline
void
GsGraph::clear_update()
{
  mUpdate = false;
}

// @brief 'update' フラグを立てる．
inline
void
GsGraph::set_update()
{
  mUpdate = true;
}

END_NAMESPACE_YM_NLINK

#endif // GSGRAPH_H
