
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
#include "RegVect.h"


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
  clear();
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
  clear();

  ymuint nv = vector_list.size();

  hash_map<ymuint32, Phf3Node*> v1_hash;
  hash_map<ymuint32, Phf3Node*> v2_hash;
  hash_map<ymuint32, Phf3Node*> v3_hash;
  vector<Phf3Node*> v1_array(nv);
  vector<Phf3Node*> v2_array(nv);
  vector<Phf3Node*> v3_array(nv);

  for (ymuint i = 0; i < nv; ++ i) {
    RegVect* rv = vector_list[i];
    {
      ymuint32 pat = f1.eval(rv);
      hash_map<ymuint32, Phf3Node*>& node_hash = v1_hash;
      vector<Phf3Node*>& node_array = v1_array;

      hash_map<ymuint32, Phf3Node*>::iterator p = node_hash.find(pat);
      Phf3Node* node = NULL;
      if ( p == node_hash.end() ) {
	node = new_node(pat);
	node_hash.insert(make_pair(pat, node));
      }
      else {
	node = p->second;
      }
      node_array[i] = node;
    }
    {
      ymuint32 pat = f2.eval(rv);
      hash_map<ymuint32, Phf3Node*>& node_hash = v2_hash;
      vector<Phf3Node*>& node_array = v2_array;

      hash_map<ymuint32, Phf3Node*>::iterator p = node_hash.find(pat);
      Phf3Node* node = NULL;
      if ( p == node_hash.end() ) {
	node = new_node(pat);
	node_hash.insert(make_pair(pat, node));
      }
      else {
	node = p->second;
      }
      node_array[i] = node;
    }
    {
      ymuint32 pat = f3.eval(rv);
      hash_map<ymuint32, Phf3Node*>& node_hash = v3_hash;
      vector<Phf3Node*>& node_array = v3_array;

      hash_map<ymuint32, Phf3Node*>::iterator p = node_hash.find(pat);
      Phf3Node* node = NULL;
      if ( p == node_hash.end() ) {
	node = new_node(pat);
	node_hash.insert(make_pair(pat, node));
      }
      else {
	node = p->second;
      }
      node_array[i] = node;
    }
  }

  for (ymuint i = 0; i < nv; ++ i) {
    Phf3Node* node1 = v1_array[i];
    Phf3Node* node2 = v2_array[i];
    Phf3Node* node3 = v3_array[i];
    Phf3Edge* edge = new Phf3Edge(i, node1, node2, node3, i);
    node1->add_edge(edge);
    node2->add_edge(edge);
    node3->add_edge(edge);
    mEdgeList.push_back(edge);
  }

  ymuint nn = mNodeList.size();

  { // simple check
    bool not_simple = false;
    for (ymuint v_pos = 0; v_pos < nv; ++ v_pos) {
      Phf3Node* node = v1_array[v_pos];
      ymuint ne = node->edge_num();
      for (ymuint i1 = 0; i1 < ne; ++ i1) {
	Phf3Edge* edge1 = node->edge(i1);
	for (ymuint i2 = i1 + 1; i2 < ne; ++ i2) {
	  Phf3Edge* edge2 = node->edge(i2);
	  if ( edge1->node2()->id() == edge2->node2()->id() &&
	       edge1->node3()->id() == edge2->node3()->id() ) {
	    not_simple = true;
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
	Phf3Node* node = mNodeList[i];
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

// @brief ノードを枝を開放する．
void
Phf3Gen::clear()
{
  for (vector<Phf3Node*>::iterator p = mNodeList.begin();
       p != mNodeList.end(); ++ p) {
    delete *p;
  }
  mNodeList.clear();

  for (vector<Phf3Edge*>::iterator p = mEdgeList.begin();
       p != mEdgeList.end(); ++ p) {
    delete *p;
  }
  mEdgeList.clear();
}

// @brief ノードを生成する．
Phf3Node*
Phf3Gen::new_node(ymuint32 pat)
{
  ymuint id = mNodeList.size();
  Phf3Node* node = new Phf3Node(id, pat);
  mNodeList.push_back(node);
  return node;
}

END_NAMESPACE_YM_IGF
