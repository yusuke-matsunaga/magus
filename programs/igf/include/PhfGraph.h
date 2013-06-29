#ifndef PHFGRAPH_H
#define PHFGRAPH_H

/// @file PhfGraph.h
/// @brief PhfGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"
#include "ym_utils/SimpleAlloc.h"


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

  /// @brief acyclic_check() の下請け関数
  void
  dfs(PhfNode* node0,
      vector<bool>& v_mark,
      vector<bool>& e_mark,
      vector<PhfEdge*>& edge_list) const;

  /// @brief ノードを生成する．
  PhfNode*
  new_node(ymuint32 pat);

  /// @brief 枝を生成する．
  PhfEdge*
  new_edge(ymuint id,
	   ymuint32 val);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // PhfNode と PhfEdge の生成用アロケータ
  SimpleAlloc mAlloc;

  // 現在のグラフ次数(枝に接続するノード数)
  ymuint32 mDegree;

  // ノードの次の ID 番号
  ymuint32 mMaxId;

  // mNodeArray のサイズ
  ymuint32 mNodeArraySize;

  // パタンをキーにしたノードの配列
  PhfNode** mNodeArray;

  // mEdgeList のサイズ
  ymuint32 mEdgeListSize;

  // 生成した枝を入れておくリスト
  PhfEdge** mEdgeList;

};

END_NAMESPACE_YM_IGF

#endif // PHFGRAPH_H
