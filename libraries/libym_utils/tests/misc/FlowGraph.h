#ifndef FLOWGRAPH_H
#define FLOWGRAPH_H

/// @file FlowGraph.h
/// @brief FlowGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

class FlowNode;
class FlowEdge;

//////////////////////////////////////////////////////////////////////
/// @class FlowGraph FlowGraph.h "FlowGraph.h"
/// @brief フローグラフを表すクラス
//////////////////////////////////////////////////////////////////////
class FlowGraph
{
public:

  /// @brief コンストラクタ
  FlowGraph();

  /// @brief デストラクタ
  ~FlowGraph();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ソースノードを返す．
  FlowNode*
  source_node();

  /// @brief シンクノードを返す．
  FlowNode*
  sink_node();

  /// @brief ノードを追加する．
  /// @return 作成したノードを返す．
  FlowNode*
  new_node();

  /// @brief 枝を追加する．
  /// @param[in] from 入力元のノード
  /// @param[in] to 出力先のノード
  /// @param[in] cost コスト
  FlowEdge*
  new_edge(FlowNode* from,
	   FlowNode* to,
	   ymuint cost);

  /// @brief 最小コスト最大フローを求める．
  void
  max_flow();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Priority First Search を行なう．
  bool
  pfs(vector<FlowEdge*>& edge_list);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるクラス
  //////////////////////////////////////////////////////////////////////

  struct NodeComp
  {

    int
    operator()(FlowNode* left,
	       FlowNode* right);

  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードリスト
  vector<FlowNode*> mNodeList;

  // 枝のリスト
  vector<FlowEdge*> mEdgeList;

  // ソースノード
  FlowNode* mSource;

  // シンクノード
  FlowNode* mSink;

};

END_NAMESPACE_YM

#endif //FLOWGRAPH_H
