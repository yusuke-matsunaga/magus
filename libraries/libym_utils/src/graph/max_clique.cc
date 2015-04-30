
/// @file MisSolver.cc
/// @brief MisSolver の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmUtils/Graph.h"
#include "Node.h"
#include "NodeHeap.h"
#include "YmUtils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// @brief max_clique 用の NodeHeap
class MaxCliqueComp
{
public:

  /// @brief 比較関数
  /// @param[in] node1, node2 対象のノード
  /// @retval 負の値 node1 が node2 より前にある．
  /// @retval 0 node1 と node2 は同じ
  /// @retval 正の値 node1 が node2 より後ろにある．
  int
  operator()(Node* node1,
	     Node* node2);

};

// @brief 比較関数
// @param[in] node1, node2 対象のノード
// @retval 負の値 node1 が node2 より前にある．
// @retval 0 node1 と node2 は同じ
// @retval 正の値 node1 が node2 より後ろにある．
int
MaxCliqueComp::operator()(Node* node1,
			  Node* node2)
{
  return node2->adj_num() - node1->adj_num();
}

END_NONAMESPACE

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

  // 各ノードに対応する Node というオブジェクトを作る．
  // ndoe_array[row_pos] に row_pos の行の Node が入る．
  // top から Node::mNext を使ってリンクトリストを作る．
  Node* node_array = new Node[node_num];
  for (ymuint i = 0; i < node_num; ++ i) {
    Node* node = &node_array[i];
    node->set(i);
  }

  NodeHeap<Node, MaxCliqueComp> node_heap(node_num);

  // Node のメモリ確保用のアロケータ
  SimpleAlloc alloc;
  // 処理対象のノードを収めるリスト
  vector<Node*> node_list;
  {
    // 隣接するノードの情報を link_array に入れる．
    vector<vector<ymuint> > link_array(node_num);
    for (ymuint i = 0; i < graph.edge_num(); ++ i) {
      pair<ymuint, ymuint> p = graph.edge(i);
      ymuint id1 = p.first;
      ymuint id2 = p.second;
      link_array[id1].push_back(id2);
      link_array[id2].push_back(id1);
    }
    // link_array の情報を Node に移す．
    for (ymuint i = 0; i < node_num; ++ i) {
      Node* node1 = &node_array[i];
      const vector<ymuint>& link_list = link_array[i];
      ymuint link_num = link_list.size();
      void* p = alloc.get_memory(sizeof(Node*) * link_num);
      Node** adj_link = new (p) Node*[link_num];
      for (ymuint j = 0; j < link_num; ++ j) {
	Node* node2 = &node_array[link_list[j]];
	adj_link[j] = node2;
      }
      node1->set_adj_link(link_num, adj_link);

      node_heap.put_node(node1);
      node_list.push_back(node1);
    }
  }

  // 作業用のフラグ配列
  void* p = alloc.get_memory(sizeof(bool) * node_num);
  bool* tmp_mark = new (p) bool[node_num];
  for (ymuint i = 0; i < node_num; ++ i) {
    tmp_mark[i] = false;
  }

  // 未処理の Node のうち Node::adj_num() が最大のものを取り出し，解に加える．
  while ( !node_heap.empty() ) {
    Node* best_node = node_heap.get_min();
    node_set.push_back(best_node->id());

    for (ymuint i = 0; i < best_node->adj_size(); ++ i) {
      // best_node に隣接しているノードにマークをつける．
      Node* node2 = best_node->adj_node(i);
      tmp_mark[node2->id()] = true;
    }

    // マークのついていないノードを削除する．
    // node_list を直接操作するのは面倒なので
    // 一旦 tmp_list にコピーする．
    vector<Node*> tmp_list;
    tmp_list.reserve(node_list.size());
    for (vector<Node*>::iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      Node* node = *p;
      if ( node != best_node ) {
	ASSERT_COND( !node->deleted() );
	tmp_list.push_back(node);
      }
    }
    node_list.clear();
    for (vector<Node*>::iterator p = tmp_list.begin();
	 p != tmp_list.end(); ++ p) {
      Node* node = *p;
      if ( tmp_mark[node->id()] ) {
	node_list.push_back(node);
      }
      else {
	// このノードを削除する．
	node_heap.delete_node(node);
	// さらにこのノードに隣接しているノードの隣接数を減らす．
	for (ymuint i = 0; i < node->adj_size(); ++ i) {
	  Node* node1 = node->adj_node(i);
	  if ( !node1->deleted() ) {
	    node1->dec_adj_num();
	    node_heap.update(node1);
	  }
	}
      }
    }
    // マークを消す．
    for (ymuint i = 0; i < best_node->adj_size(); ++ i) {
      // best_node に隣接しているノードのマークを消す．
      Node* node2 = best_node->adj_node(i);
      tmp_mark[node2->id()] = false;
    }
  }

  delete [] node_array;

  return node_set.size();
}

END_NAMESPACE_YM
