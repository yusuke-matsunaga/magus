
/// @file Graph.cc
/// @brief Graph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmAlgo/Graph.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス Graph
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] node_num ノード数
Graph::Graph(ymuint node_num)
{
  mNodeNum = node_num;
}

// @brief デストラクタ
Graph::~Graph()
{
}

// @brief ノード数を得る．
ymuint
Graph::node_num() const
{
  return mNodeNum;
}

// @brief 2つのノードを接続する．
// @param[in] id1, id2 2つのノードの番号 ( 0 <= id1, id2 < node_num() )
void
Graph::connect(ymuint id1,
	       ymuint id2)
{
  ASSERT_COND( id1 < node_num() );
  ASSERT_COND( id2 < node_num() );
  ASSERT_COND( id1 != id2 );

  if ( id1 < id2 ) {
    mEdgeArray.push_back(id1);
    mEdgeArray.push_back(id2);
  }
  else {
    mEdgeArray.push_back(id2);
    mEdgeArray.push_back(id1);
  }
}

// @brief 枝の総数を返す．
ymuint
Graph::edge_num() const
{
  ymuint n = mEdgeArray.size();
  ASSERT_COND( (n % 2) == 0 );
  return n / 2;
}

// @brief 枝の情報を返す．
// @param[in] idx 枝番号 ( 0 <= idx < edge_num() )
// @return ノード番号の pair を返す．
//
// pair の値はノード番号の昇順に整列している．
pair<ymuint, ymuint>
Graph::edge(ymuint idx) const
{
  ymuint id1 = mEdgeArray[idx * 2 + 0];
  ymuint id2 = mEdgeArray[idx * 2 + 1];
  return make_pair(id1, id2);
}

// @brief グラフの内容を出力する．
// @param[in] s 出力先のストリーム
void
Graph::dump(ostream& s) const
{
  vector<vector<ymuint> > link_list(node_num());
  for (ymuint i = 0; i < edge_num(); ++ i) {
    pair<ymuint, ymuint> p = edge(i);
    ymuint id1 = p.first;
    ymuint id2 = p.second;
    link_list[id1].push_back(id2);
    link_list[id2].push_back(id1);
  }

  for (ymuint i = 0; i < node_num(); ++ i) {
    cout << i << ": ";
    const vector<ymuint>& list1 = link_list[i];
    for (vector<ymuint>::const_iterator p = list1.begin();
	 p != list1.end(); ++ p) {
      ymuint id2 = *p;
      cout << " " << id2;
    }
    cout << endl;
  }
  cout << endl;
}

END_NAMESPACE_YM
