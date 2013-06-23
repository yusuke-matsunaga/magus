#ifndef PHFGRAPH_H
#define PHFGRAPH_H

/// @file PhfGraph.h
/// @brief PhfGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"


BEGIN_NAMESPACE_YM_IGF

class PhfNode;
class PhfEdge;

//////////////////////////////////////////////////////////////////////
/// @class PhfGraph PhfGraph.h "PhfGraph.h"
/// @brief PHF を内部構造を表すグラフ
//////////////////////////////////////////////////////////////////////
class PhfGraph
{
public:

  /// @brief コンストラクタ
  /// @param[in] func_list 関数のリスト
  PhfGraph(const vector<const FuncVect*>& func_list);

  /// @brief デストラクタ
  ~PhfGraph();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief simple check を行なう．
  /// @retval true simple graph だった．
  /// @retval ffalse simple graph ではなかった．
  bool
  simple_check() const;

  /// @brief acyclic check を行なう．
  /// @param[out] edge_list 枝の順番を格納するリスト
  /// @retval true acyclic だった．
  /// @retval false cyclic だった．
  bool
  acyclic_check(vector<PhfEdge*>& edge_list) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief グラフをクリアする．
  void
  clear();

  /// @brief 関数のリストからグラフを作る．
  /// @param[in] func_list 関数のリスト
  void
  gen_graph(const vector<const FuncVect*>& func_list);

  /// @brief ノードを生成する．
  PhfNode*
  new_node(ymuint32 pat);

  /// @brief 枝を生成する．
  PhfEdge*
  new_edge(ymuint id,
	   const vector<PhfNode*>& node_list,
	   ymuint32 val);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 生成したノードを入れておくリスト
  vector<PhfNode*> mNodeList;

  // 生成した枝を入れておくリスト
  vector<PhfEdge*> mEdgeList;

};

END_NAMESPACE_YM_IGF

#endif // PHFGRAPH_H
