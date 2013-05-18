
/// @file Phf3Gen.cc
/// @brief Phf3Gen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "Phf3Gen.h"
#include "Phf3Node.h"
#include "Phf3Edge.h"
#include "InputFunc.h"


BEGIN_NAMESPACE_YM_IGF

BEGIN_NONAMESPACE

bool
dfs(Phf3Node* node,
    Phf3Edge* from,
    vector<bool>& v_mark,
    vector<Phf3Edge*>& edge_list)
{
  ymuint ne = node->edge_num();
  for (ymuint i = 0; i < ne; ++ i) {
    Phf3Edge* edge = node->edge(i);
    if ( edge == from ) {
      continue;
    }

    edge_list.push_back(edge);

    Phf3Node* node1 = edge->node1();
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

    Phf3Node* node2 = edge->node2();
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

    Phf3Node* node3 = edge->node3();
    if ( node3 != node ) {
      if ( v_mark[node3->id()] ) {
	return false;
      }
      v_mark[node3->id()] = true;
      bool stat = dfs(node3, edge, v_mark, edge_list);
      if ( !stat ) {
	return false;
      }
    }
  }
  return true;
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス Phf3Gen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Phf3Gen::Phf3Gen()
{
}

// @brief デストラクタ
Phf3Gen::~Phf3Gen()
{
}

// @brief マッピングを求める．
bool
Phf3Gen::mapping(const vector<RegVect*>& vector_list,
		 const InputFunc& f1,
		 const InputFunc& f2,
		 const InputFunc& f3,
		 vector<ymuint32>& g1,
		 vector<ymuint32>& g2,
		 vector<ymuint32>& g3)
{
  ymuint nv = vector_list.size();

  hash_map<ymuint32, Phf3Node*> v1_hash;
  hash_map<ymuint32, Phf3Node*> v2_hash;
  hash_map<ymuint32, Phf3Node*> v3_hash;
  vector<Phf3Node*> v1_array(nv);
  vector<Phf3Node*> v2_array(nv);
  vector<Phf3Node*> v3_array(nv);
  vector<Phf3Node*> v_array;
  for (ymuint i = 0; i < nv; ++ i) {
    RegVect* rv = vector_list[i];
    ymuint32 v1 = f1.eval(rv);
    hash_map<ymuint32, Phf3Node*>::iterator p1 = v1_hash.find(v1);
    Phf3Node* node1 = NULL;
    if ( p1 == v1_hash.end() ) {
      ymuint id1 = v_array.size();
      node1 = new Phf3Node(id1, v1);
      v_array.push_back(node1);
      v1_hash.insert(make_pair(v1, node1));
    }
    else {
      node1 = p1->second;
    }
    v1_array[i] = node1;
  }

  for (ymuint i = 0; i < nv; ++ i) {
    RegVect* rv = vector_list[i];
    ymuint32 v2 = f2.eval(rv);
    hash_map<ymuint32, Phf3Node*>::iterator p2 = v2_hash.find(v2);
    Phf3Node* node2 = NULL;
    if ( p2 == v2_hash.end() ) {
      ymuint id2 = v_array.size();
      node2 = new Phf3Node(id2, v2);
      v_array.push_back(node2);
      v2_hash.insert(make_pair(v2, node2));
    }
    else {
      node2 = p2->second;
    }
    v2_array[i] = node2;
  }

  for (ymuint i = 0; i < nv; ++ i) {
    RegVect* rv = vector_list[i];
    ymuint32 v3 = f3.eval(rv);
    hash_map<ymuint32, Phf3Node*>::iterator p3 = v3_hash.find(v3);
    Phf3Node* node3 = NULL;
    if ( p3 == v3_hash.end() ) {
      ymuint id3 = v_array.size();
      node3 = new Phf3Node(id3, v3);
      v_array.push_back(node3);
      v3_hash.insert(make_pair(v3, node3));
    }
    else {
      node3 = p3->second;
    }
    v3_array[i] = node3;
  }

  for (ymuint i = 0; i < nv; ++ i) {
    Phf3Node* node1 = v1_array[i];
    Phf3Node* node2 = v2_array[i];
    Phf3Node* node3 = v3_array[i];
    Phf3Edge* edge = new Phf3Edge(i, node1, node2, node3, i);
    node1->add_edge(edge);
    node2->add_edge(edge);
    node3->add_edge(edge);
  }

  ymuint nn = v_array.size();

  { // simple check
    bool not_simple = false;
    for (ymuint i = 0; i < nv; ++ i) {
      Phf3Node* node = v1_array[i];
      ymuint ne = node->edge_num();
      for (ymuint j = 0; j < ne; ++ j) {
	Phf3Edge* edge1 = node->edge(j);
	for (ymuint k = j + 1; k < ne; ++ k) {
	  Phf3Edge* edge2 = node->edge(k);
	  if ( edge1->node2()->id() == edge2->node2()->id() &&
	       edge1->node3()->id() == edge2->node3()->id() ) {
	    not_simple = true;
	    cout << "Edge#" << edge1->id()
		 << " and Edge#" << edge2->id()
		 << " collaids" << endl;
	    cout << "#" << edge1->id()
		 << ": " << edge1->node1()->id()
		 << ", " << edge1->node2()->id()
		 << ", " << edge1->node3()->id()
		 << endl
		 << "#" << edge2->id()
		 << ": " << edge2->node1()->id()
		 << ", " << edge2->node2()->id()
		 << ", " << edge2->node3()->id()
		 << endl;
	    break;
	  }
	}
	if ( not_simple ) {
	  break;
	}
      }
      if ( not_simple ) {
	break;
      }
    }
    if ( not_simple ) {
      cout << " not simple" << endl;
      return false;
    }
  }

  vector<Phf3Edge*> edge_list;
  {
    vector<bool> v_mark(nn, false);

    for ( ; ; ) {
      Phf3Node* node0 = NULL;
      for (ymuint i = 0; i < nn; ++ i) {
	Phf3Node* node = v_array[i];
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
	cout << " cyclic" << endl;
	return false;
      }
    }
  }

  ymuint ne = edge_list.size();
  for (ymuint i = 0; i < ne; ++ i) {
    Phf3Edge* edge = edge_list[i];
    Phf3Node* node1 = edge->node1();
    Phf3Node* node2 = edge->node2();
    Phf3Node* node3 = edge->node3();

    if ( !node1->is_assigned() ) {
      ymuint32 val = node2->val() ^ node3->val() ^ edge->val();
      node1->set_val(val);
      g1[node1->pat()] = val;
    }
    if ( !node2->is_assigned() ) {
      ymuint32 val = node1->val() ^ node3->val() ^ edge->val();
      node2->set_val(val);
      g2[node2->pat()] = val;
    }
    if ( !node3->is_assigned() ) {
      ymuint32 val = node1->val() ^ node2->val() ^ edge->val();
      node3->set_val(val);
      g3[node2->pat()] = val;
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
