
/// @file libym_techmap/PatGraph.cc
/// @brief PatGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "PatGraph.h"


BEGIN_NAMESPACE_YM_TECHMAP

// @brief コンストラクタ
PatGraph::PatGraph()
{
  mNodeNum = 0;
  mNodeTypeArray = NULL;
  mEdgeNum = 0;
  mEdgeArray = NULL;
}

// @brief デストラクタ
PatGraph::~PatGraph()
{
  delete [] mNodeTypeArray;
  delete [] mEdgeArray;
}

// @brief ノード数を設定し，mNodeTypeArray を確保する．
// @param[in] n ノード数
void
PatGraph::set_node_num(ymuint n)
{
  assert_cond( mNodeNum == 0, __FILE__, __LINE__);
  mNodeNum = n;
  mNodeTypeArray = new tType[n];
}

// @brief ノードの種類を設定する．
// @param[in] id ノード番号 ( 0 <= id < node_num() )
// @param[in] type 設定するノードの種類
void
PatGraph::set_node_type(ymuint id,
			tType type)
{
  assert_cond( id < mNodeNum, __FILE__, __LINE__);
  mNodeTypeArray[id] = type;
}

// @brief 枝の数を設定し，mEdgeArray を確保する．
// @param[in] n 枝の数
void
PatGraph::set_edge_num(ymuint n)
{
  assert_cond( mEdgeNum == 0, __FILE__, __LINE__);
  mEdgeNum = n;
  mEdgeArray = new PatEdge[n];
}

// @brief 枝を設定する．
// @param[in] id 枝の番号 ( 0 <= id < edge_num() )
// @param[in] from_id 入力元のノード番号
// @param[in] to_id 出力先のノード番号
// @param[in] fanin_pos ファンインの位置 ( 0  or 1 )
// @param[in] inverted 極性
void
PatGraph::set_edge(ymuint id,
		   ymuint from_id,
		   ymuint to_id,
		   ymuint fanin_pos,
		   bool inverted)
{
  assert_cond( id < mEdgeNum, __FILE__, __LINE__);
  ymuint32 data = 0U;
  if ( inverted ) {
    data |= 1U;
  }
  if ( fanin_pos == 1 ) {
    data |= 2U;
  }
  data |= (from_id << 2);
  data |= (to_id << 17);
  mEdgeArray[id].mData = data;
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
