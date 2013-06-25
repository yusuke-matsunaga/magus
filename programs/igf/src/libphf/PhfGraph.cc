
/// @file PhfGraph.cc
/// @brief PhfGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "PhfGraph.h"
#include "PhfNode.h"
#include "PhfEdge.h"
#include "FuncVect.h"


#define VERIFY_ACYCLIC_CHECK 0

BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
// クラス PhfGraph
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] func_list 関数のリスト
PhfGraph::PhfGraph(const vector<const FuncVect*>& func_list)
{
  gen_graph(func_list);
}

// @brief デストラクタ
PhfGraph::~PhfGraph()
{
  clear();
}

// @brief simple check を行なう．
// @retval true simple graph だった．
// @retval false simple graph ではなかった．
bool
PhfGraph::simple_check() const
{
  ymuint nn = mNodeList.size();
  for (ymuint n_pos = 0; n_pos < nn; ++ n_pos) {
    PhfNode* node = mNodeList[n_pos];
    if ( node->edge(0)->node(0) != node ) {
      // 最初のハッシュ値のノードだけを対象にする．
      continue;
    }

    ymuint ne = node->edge_num();
    for (ymuint e_pos1 = 0; e_pos1 < ne; ++ e_pos1) {
      PhfEdge* edge1 = node->edge(e_pos1);
      ymuint nn = edge1->node_num();
      for (ymuint e_pos2 = e_pos1 + 1; e_pos2 < ne; ++ e_pos2) {
	PhfEdge* edge2 = node->edge(e_pos2);
	bool diff = false;
	for (ymuint l = 1; l < nn; ++ l) {
	  if ( edge1->node(l)->id() != edge2->node(l)->id() ) {
	    diff = true;
	    break;
	  }
	}
	if ( !diff ) {
	  return false;
	}
      }
    }
  }
  return true;
}

// @brief acyclic check を行なう．
// @param[out] edge_list 枝の順番を格納するリスト
// @retval true acyclic だった．
// @retval false cyclic だった．
bool
PhfGraph::acyclic_check(vector<PhfEdge*>& edge_list) const
{
  ymuint node_num = mNodeList.size();
  ymuint edge_num = mEdgeList.size();
  vector<bool> v_mark(node_num, false);
  vector<bool> e_mark(edge_num, false);
  ymuint count = 0;
  for ( ; ; ) {
    PhfNode* node0 = NULL;
    PhfEdge* edge0 = NULL;
    for (ymuint n_pos = 0; n_pos < node_num; ++ n_pos) {
      PhfNode* node = mNodeList[n_pos];
      if ( v_mark[node->id()] ) {
	continue;
      }
      ymuint ne = node->edge_num();
      ymuint d = 0;
      for (ymuint i = 0; i < ne; ++ i) {
	PhfEdge* edge = node->edge(i);
	if ( !e_mark[edge->id()] ) {
	  ++ d;
	  edge0 = edge;
	}
      }
      if ( d == 1 ) {
	node0 = node;
	break;
      }
    }
    if ( node0 == NULL ) {
      return false;
    }
    v_mark[node0->id()] = true;
    e_mark[edge0->id()] = true;
    edge_list.push_back(edge0);

    ++ count;
    if ( count == edge_num ) {
      if ( VERIFY_ACYCLIC_CHECK ) {
	vector<bool> v_mark(node_num, false);
	for (vector<PhfEdge*>::reverse_iterator p = edge_list.rbegin();
	     p != edge_list.rend(); ++ p) {
	  PhfEdge* edge = *p;
	  ymuint n = edge->node_num();
	  bool has_node = false;
	  for (ymuint i = 0; i < n; ++ i) {
	    PhfNode* node = edge->node(i);
	    if ( !v_mark[node->id()] ) {
	      has_node = true;
	      v_mark[node->id()] = true;
	    }
	  }
	  assert_cond( has_node, __FILE__, __LINE__);
	}
      }

      return true;
    }
  }
}

// @brief 関数のリストからグラフを作る．
// @param[in] func_list 関数のリスト
void
PhfGraph::gen_graph(const vector<const FuncVect*>& func_list)
{
  ymuint nf = func_list.size();
  assert_cond( nf > 1, __FILE__, __LINE__);

  ymuint nv = func_list[0]->input_size();

  hash_map<ymuint32, PhfNode*>* v_hash_array = new hash_map<ymuint32, PhfNode*>[nf];

  for (ymuint v_pos = 0; v_pos < nv; ++ v_pos) {
    vector<PhfNode*> node_list(nf);
    for (ymuint f_pos = 0; f_pos < nf; ++ f_pos) {
      const FuncVect& f1 = *func_list[f_pos];
      hash_map<ymuint32, PhfNode*>& v1_hash = v_hash_array[f_pos];

      ymuint32 pat1 = f1.val(v_pos);
      hash_map<ymuint32, PhfNode*>::iterator p1 = v1_hash.find(pat1);
      PhfNode* node1 = NULL;
      if ( p1 == v1_hash.end() ) {
	node1 = new_node(pat1);
	v1_hash.insert(make_pair(pat1, node1));
      }
      else {
	node1 = p1->second;
      }
      node_list[f_pos] = node1;
    }

    PhfEdge* edge = new_edge(v_pos, node_list, v_pos);
    for (ymuint f_pos = 0; f_pos < nf; ++ f_pos) {
      PhfNode* node = node_list[f_pos];
      node->add_edge(edge);
    }
  }
  delete [] v_hash_array;
}

// @brief ノードを枝を開放する．
void
PhfGraph::clear()
{
  for (vector<PhfNode*>::iterator p = mNodeList.begin();
       p != mNodeList.end(); ++ p) {
    delete *p;
  }
  mNodeList.clear();

  for (vector<PhfEdge*>::iterator p = mEdgeList.begin();
       p != mEdgeList.end(); ++ p) {
    delete *p;
  }
  mEdgeList.clear();
}

// @brief ノードを生成する．
PhfNode*
PhfGraph::new_node(ymuint32 pat)
{
  ymuint id = mNodeList.size();
  PhfNode* node = new PhfNode(id, pat);
  mNodeList.push_back(node);
  return node;
}

// @brief 枝を生成する．
PhfEdge*
PhfGraph::new_edge(ymuint id,
		 const vector<PhfNode*>& node_list,
		 ymuint32 val)
{
  PhfEdge* edge = new PhfEdge(id, node_list, val);
  mEdgeList.push_back(edge);
  return edge;
}

END_NAMESPACE_YM_IGF
