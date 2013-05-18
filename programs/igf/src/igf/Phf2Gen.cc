
/// @file Phf2Gen.cc
/// @brief Phf2Gen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "Phf2Gen.h"
#include "Phf2Node.h"
#include "Phf2Edge.h"
#include "InputFunc.h"


BEGIN_NAMESPACE_YM_IGF

BEGIN_NONAMESPACE

bool
dfs(Phf2Node* node,
    Phf2Edge* from,
    vector<bool>& v_mark,
    vector<Phf2Edge*>& edge_list)
{
  ymuint ne = node->edge_num();
  for (ymuint i = 0; i < ne; ++ i) {
    Phf2Edge* edge = node->edge(i);
    if ( edge == from ) {
      continue;
    }

    edge_list.push_back(edge);

    Phf2Node* node1 = edge->node1();
    if ( node1 != node ) {
      if ( v_mark[node1->id()] ) {
	return false;
      }
      v_mark[node1->id()] = true;
      bool stat = dfs(node1, edge, v_mark, edge_list);
      if ( !stat ) {
	return false;
      }
    }

    Phf2Node* node2 = edge->node2();
    if ( node2 != node ) {
      if ( v_mark[node2->id()] ) {
	return false;
      }
      v_mark[node2->id()] = true;
      bool stat = dfs(node2, edge, v_mark, edge_list);
      if ( !stat ) {
	return false;
      }
    }
  }
  return true;
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス Phf2Gen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Phf2Gen::Phf2Gen()
{
}

// @brief デストラクタ
Phf2Gen::~Phf2Gen()
{
}

// @brief マッピングを求める．
bool
Phf2Gen::mapping(const vector<RegVect*>& vector_list,
		 const InputFunc& f1,
		 const InputFunc& f2,
		 vector<ymuint32>& g1,
		 vector<ymuint32>& g2)
{
  ymuint nv = vector_list.size();

  hash_map<ymuint32, Phf2Node*> v1_hash;
  hash_map<ymuint32, Phf2Node*> v2_hash;
  vector<Phf2Node*> v1_array(nv);
  vector<Phf2Node*> v2_array(nv);
  vector<Phf2Node*> v_array;
  ymuint e_id = 0;
  for (ymuint i = 0; i < nv; ++ i) {
    RegVect* rv = vector_list[i];
    ymuint32 v1 = f1.eval(rv);
    hash_map<ymuint32, Phf2Node*>::iterator p1 = v1_hash.find(v1);
    Phf2Node* node1 = NULL;
    if ( p1 == v1_hash.end() ) {
      ymuint id1 = v1_hash.size();
      node1 = new Phf2Node(id1, v1);
      v_array.push_back(node1);
      v1_hash.insert(make_pair(v1, node1));
    }
    else {
      node1 = p1->second;
    }
    v1_array[i] = node1;
  }

  ymuint offset = v1_hash.size();
  for (ymuint i = 0; i < nv; ++ i) {
    RegVect* rv = vector_list[i];
    ymuint32 v2 = f2.eval(rv);
    hash_map<ymuint32, Phf2Node*>::iterator p2 = v2_hash.find(v2);
    Phf2Node* node2 = NULL;
    if ( p2 == v2_hash.end() ) {
      ymuint id2 = v2_hash.size() + offset;
      node2 = new Phf2Node(id2, v2);
      v_array.push_back(node2);
      v2_hash.insert(make_pair(v2, node2));
    }
    else {
      node2 = p2->second;
    }
    v2_array[i] = node2;

    Phf2Node* node1 = v1_array[i];
    Phf2Edge* edge = new Phf2Edge(e_id, node1, node2, i);
    node1->add_edge(edge);
    node2->add_edge(edge);
    ++ e_id;
  }

  ymuint nn = v_array.size();

  { // simple check
    bool not_simple = false;
    for (ymuint i = 0; i < nv; ++ i) {
      Phf2Node* node = v1_array[i];
      ymuint ne = node->edge_num();
      vector<ymuint> mark(nn, false);
      for (ymuint j = 0; j < ne; ++ j) {
	Phf2Edge* edge = node->edge(j);
	Phf2Node* node2 = edge->node2();
	if ( mark[node2->id()] ) {
	  not_simple = true;
	  break;
	}
	mark[node2->id()] = true;
      }
    }
    if ( not_simple ) {
      return false;
    }
  }

  vector<Phf2Edge*> edge_list;
  {
    vector<bool> v_mark(nn, false);

    for ( ; ; ) {
      Phf2Node* node0 = NULL;
      for (ymuint i = 0; i < nn; ++ i) {
	Phf2Node* node = v_array[i];
	if ( !v_mark[node->id()] ) {
	  node0 = node;
	  break;
	}
      }
      if ( node0 == NULL ) {
	break;
      }
      v_mark[node0->id()] = true;
      bool stat = dfs(node0, NULL, v_mark, edge_list);
      if ( !stat ) {
	return false;
      }
    }
  }

  ymuint ne = edge_list.size();
  for (ymuint i = 0; i < ne; ++ i) {
    Phf2Edge* edge = edge_list[i];
    Phf2Node* node1 = edge->node1();
    Phf2Node* node2 = edge->node2();

    if ( !node1->is_assigned() ) {
      ymuint32 val = node2->val() ^ edge->val();
      node1->set_val(val);
      g1[node1->pat()] = val;
    }
    if ( !node2->is_assigned() ) {
      ymuint32 val = node1->val() ^ edge->val();
      node2->set_val(val);
      g2[node2->pat()] = val;
    }
    if ( 0 ) {
      cout << "Edge#" << setw(6) << edge->id() << ": " << edge->val() << endl
	   << "Node#" << setw(6) << node1->id() << ": " << node1->val() << endl
	   << "Node#" << setw(6) << node2->id() << ": " << node2->val() << endl
	   << endl;
    }
  }

  return true;
}

END_NAMESPACE_YM_IGF
