
/// @file MisSolver.cc
/// @brief MisSolver の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmUtils/Graph.h"
#include "McNode.h"
#include "McNodeHeap.h"
#include "YmUtils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM

// @brief 最大クリークを求める．
// @param[in] graph 対象のグラフ
// @param[out] node_set クリークの要素(ノード番号)を収める配列
// @return 要素数を返す．
ymuint
max_clique(const Graph& graph,
	   vector<ymuint>& node_set)
{
  node_set.clear();

  ymuint node_num = graph.node_num();
  if ( node_num == 0 ) {
    return 0;
  }

  // 各ノードに対応する McNode というオブジェクトを作る．
  // ndoe_array[row_pos] に row_pos の行の Node が入る．
  // top から Node::mNext を使ってリンクとリストを作る．
  SimpleAlloc alloc;
  McNodeHeap node_heap(node_num);

  void* p = alloc.get_memory(sizeof(McNode*) * node_num);
  McNode** node_array = new (p) McNode*[node_num];
  for (ymuint i = 0; i < node_num; ++ i) {
    McNode* node = node_heap.node(i);
    node->set(i);
    node_array[i] = node;
  }

  // 同じ列を共有する行の関係を Node::mAdjLink で表す．
  // node1 と列を共有する行の Node が node1->mAdjLink[0:node1->mAdjNum -1]
  // に入る．
  // node1->mNum も node1->mAdjNum で初期化される．
  vector<vector<ymuint> > link_array(node_num);
  for (ymuint i = 0; i < graph.edge_num(); ++ i) {
    pair<ymuint, ymuint> p = graph.edge(i);
    ymuint id1 = p.first;
    ymuint id2 = p.second;
    link_array[id1].push_back(id2);
    link_array[id2].push_back(id1);
  }
  for (ymuint i = 0; i < node_num; ++ i) {
    McNode* node1 = node_array[i];
    const vector<ymuint>& link_list = link_array[i];
    ymuint link_num = link_list.size();
    void* p = alloc.get_memory(sizeof(McNode*) * link_num);
    McNode** adj_link = new (p) McNode*[link_num];
    for (ymuint j = 0; j < link_num; ++ j) {
      McNode* node2 = node_array[link_list[j]];
      adj_link[j] = node2;
    }
    node1->set_adj_link(link_num, adj_link);

    node_heap.put(node1);
  }

  // 未処理の McNode のうち Node::mNum が最小のものを取り出し，解に加える．
  while ( !node_heap.empty() ) {
    McNode* best_node = node_heap.get_min();
    node_set.push_back(best_node->id());

    for (ymuint i = 0; i < best_node->adj_size(); ++ i) {
      // best_node に隣接しているノードも処理済みとする．
      McNode* node2 = best_node->adj_node(i);
      if ( !node2->deleted() ) {
	node_heap.delete_node(node2);
	// さらにこのノードに隣接しているノードの mNum を減らす．
	for (ymuint j = 0; j < node2->adj_size(); ++ j) {
	  McNode* node3 = node2->adj_node(j);
	  if ( !node3->deleted() ) {
	    node3->dec_adj_num();
	    node_heap.update(node3);
	  }
	}
      }
    }
  }

  return node_set.size();
}

END_NAMESPACE_YM
