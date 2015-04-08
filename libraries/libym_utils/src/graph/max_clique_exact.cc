
/// @file MisSolver.cc
/// @brief MisSolver の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmUtils/Graph.h"
#include "Node.h"
#include "YmUtils/HashSet.h"
#include "YmUtils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

struct NodeLt
{
  bool
  operator()(pair<Node*, ymuint>& left,
	     pair<Node*, ymuint>& right)
  {
    return left.second > right.second;
  }
};

ymuint count;

ymuint
mc_recur(const vector<Node*>& selected_node_list,
	 const vector<Node*>& rest_node_list,
	 ymuint best_so_far,
	 vector<ymuint>& node_set)
{
#if 0
  cout << "mc_recur(selected_node_list = " << selected_node_list.size()
       << ", rest_node_list = " << rest_node_list.size()
       << ", best_so_far = " << best_so_far << ")" << endl;
#endif

  if ( selected_node_list.size() + rest_node_list.size() <= best_so_far ) {
    return 0;
  }

  ++ count;
  if ( count >= 10000 ) {
    return 0;
  }

  if ( rest_node_list.empty() ) {
    node_set.clear();
    for (ymuint i = 0; i < selected_node_list.size(); ++ i) {
      Node* node = selected_node_list[i];
      node_set.push_back(node->id());
    }
    return node_set.size();
  }

  HashSet<ymuint> mark;
  vector<pair<Node*, ymuint> > tmp_node_list;
  tmp_node_list.reserve(rest_node_list.size());
  for (ymuint i = 0; i < rest_node_list.size(); ++ i) {
    Node* node1 = rest_node_list[i];
    mark.add(node1->id());
  }

  for (ymuint i = 0; i < rest_node_list.size(); ++ i) {
    Node* node1 = rest_node_list[i];
    ymuint n = 0;
    for (ymuint j = 0; j < node1->adj_size(); ++ j) {
      Node* node2 = node1->adj_node(j);
      if ( mark.check(node2->id()) ) {
	++ n;
      }
    }
    tmp_node_list.push_back(make_pair(node1, n));
  }
  sort(tmp_node_list.begin(), tmp_node_list.end(), NodeLt());

  ymuint max_val = best_so_far;
  for (ymuint i = 0; i < tmp_node_list.size(); ++ i) {
    Node* node1 = tmp_node_list[i].first;

    HashSet<ymuint> mark;
    for (ymuint i = 0; i < node1->adj_size(); ++ i) {
      Node* node2 = node1->adj_node(i);
      mark.add(node2->id());
    }

    vector<Node*> new_node_list;
    new_node_list.reserve(rest_node_list.size());

    for (ymuint i = 0; i < rest_node_list.size(); ++ i) {
      Node* node2 = rest_node_list[i];
      if ( mark.check(node2->id()) ) {
	new_node_list.push_back(node2);
      }
    }

    vector<Node*> new_selected_node_list = selected_node_list;
    new_selected_node_list.push_back(node1);
    vector<ymuint> tmp_node_set;
    mc_recur(new_selected_node_list, new_node_list, max_val, tmp_node_set);
    ymuint val = tmp_node_set.size();
    if ( max_val < val ) {
      max_val = val;
      node_set = tmp_node_set;
    }
  }

#if 0
  cout << "mc_recur(selected_node_list = " << selected_node_list.size()
       << ", rest_node_list = " << rest_node_list.size()
       << ", best_so_far = " << best_so_far << ")" << endl
       << "  ==> " << max_val << endl;
#endif

  return max_val;
}

END_NONAMESPACE

// @brief 最大クリークを求める．
// @param[in] graph 対象のグラフ
// @param[out] node_set クリークの要素(ノード番号)を収める配列
// @return 要素数を返す．
ymuint
max_clique_exact(const Graph& graph,
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

      node_list.push_back(node1);
    }
  }

  count = 0;
  mc_recur(vector<Node*>(0), node_list, 0, node_set);

  delete [] node_array;

  return node_set.size();
}

END_NAMESPACE_YM
