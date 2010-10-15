
/// @file libym_techmap/PatGraph.cc
/// @brief PatGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "PatGraph.h"


BEGIN_NAMESPACE_YM_TECHMAP

// @brief コンストラクタ
// @param[in] data 内容を表すエンコードされたデータ
PatGraph::PatGraph(const ymuint32* data)
{
  ymuint pos = 0;

  // 最初の1ワードがノード数
  mNodeNum = data[pos];
  ++ pos;
  mNodeTypeArray = new tType[mNodeNum];

  // 次の1ワードが入力数
  mInputNum = data[pos];
  ++ pos;

  // 次の1ワードが根の ID
  mRootId = data[pos];
  ++ pos;

  // 続く mNodeNum ワードがノードの種類
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    mNodeTypeArray[i] = static_cast<tType>(data[pos]);
    ++ pos;
  }

  // 次のワードが枝の数
  mEdgeNum = data[pos];
  ++ pos;
  mEdgeArray = new PatEdge[mEdgeNum];

  // 続く mEdgeNum ワードが枝の情報
  for (ymuint i = 0; i < mEdgeNum; ++ i) {
    mEdgeArray[i].mData = data[pos];
    ++ pos;
  }
}

// @brief デストラクタ
PatGraph::~PatGraph()
{
  delete [] mNodeTypeArray;
  delete [] mEdgeArray;
}

// @relates PatGraph
// @brief PatGraph の内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] patgraph パタングラフ
void
dump(ostream& s,
     const PatGraph& patgraph)
{
  s << "==== PatGraph dump start ====" << endl;

  // ノードの種類の出力
  ymuint nn = patgraph.node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    s << "Node#" << i << ": ";
    switch ( patgraph.node_type(i) ) {
    case PatGraph::kInput: s << "INPUT"; break;
    case PatGraph::kAnd:   s << "AND"; break;
    case PatGraph::kXor:   s << "XOR"; break;
    default: assert_not_reached(__FILE__, __LINE__);
    }
    s << endl;
  }
  s << endl;

  // 枝の情報の出力
  ymuint ne = patgraph.edge_num();
  for (ymuint i = 0; i < ne; ++ i) {
    const PatEdge& e = patgraph.edge(i);
    s << "Edge#" << i << ": " << e.from_id()
      << " -> " << e.to_id() << "(" << e.fanin_pos() << ")";
    if ( e.inverted() ) {
      s << " ***";
    }
    s << endl;
  }

  s << "==== PatGraph dump end ====" << endl;
}

END_NAMESPACE_YM_TECHMAP
