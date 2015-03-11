#ifndef YMUTILS_GRAPH_H
#define YMUTILS_GRAPH_H

/// @file YmUtils/Graph.h
/// @brief Graph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class Graph Graph.h "YmUtils/Graph.h"
/// @brief 一般的な無向グラフを表すクラス
//////////////////////////////////////////////////////////////////////
class Graph
{
public:

  /// @brief コンストラクタ
  /// @param[in] node_num ノード数
  ///
  /// この状態では枝はない
  Graph(ymuint node_num);

  /// @brief デストラクタ
  ~Graph();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を得る．
  ymuint
  node_num() const;

  /// @brief 2つのノードを接続する．
  /// @param[in] id1, id2 2つのノードの番号 ( 0 <= id1, id2 < node_num() )
  void
  connect(ymuint id1,
	  ymuint id2);

  /// @brief 枝の総数を返す．
  ymuint
  edge_num() const;

  /// @brief 枝の情報を返す．
  /// @param[in] idx 枝番号 ( 0 <= idx < edge_num() )
  /// @return ノード番号の pair を返す．
  ///
  /// pair の値はノード番号の昇順に整列している．
  pair<ymuint, ymuint>
  edge(ymuint idx) const;

  /// @brief グラフの内容を出力する．
  /// @param[in] s 出力先のストリーム
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード数
  ymuint32 mNodeNum;

  // 枝の実体の配列
  // ただし，実際には2つで1つの枝を表す．
  vector<ymuint> mEdgeArray;

};

/// @brief 彩色問題を解く
/// @param[in] graph 対象のグラフ
/// @param[out] color_group 同じ色のノード番号のリストの配列
/// @return 彩色数を返す．
extern
ymuint
coloring(const Graph& graph,
	 vector<vector<ymuint> >& color_group);

END_NAMESPACE_YM

#endif // YMUTILS_GRAPH_H
