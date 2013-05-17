
/// @file PhfGen.cc
/// @brief PhfGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "PhfGen.h"
#include "PhfNode.h"
#include "PhfEdge.h"
#include "InputFunc.h"


BEGIN_NAMESPACE_YM_IGF

BEGIN_NONAMESPACE

bool
dfs2(PhfNode* node2,
     PhfNode* from,
     vector<bool>& v_mark,
     vector<PhfEdge*>& edge_list);

bool
dfs1(PhfNode* node1,
     PhfNode* from,
     vector<bool>& v_mark,
     vector<PhfEdge*>& edge_list)
{
  ymuint ne = node1->edge_num();
  for (ymuint i = 0; i < ne; ++ i) {
    PhfEdge* edge = node1->edge(i);
    PhfNode* node2 = edge->node2();
    if ( node2 == from ) {
      continue;
    }
    edge_list.push_back(edge);
    edge->set_dir(0);
    if ( v_mark[node2->id()] ) {
      return false;
    }
    v_mark[node2->id()] = true;
    bool stat = dfs2(node2, node1, v_mark, edge_list);
    if ( !stat ) {
      return false;
    }
  }
  return true;
}

bool
dfs2(PhfNode* node2,
     PhfNode* from,
     vector<bool>& v_mark,
     vector<PhfEdge*>& edge_list)
{
  ymuint ne = node2->edge_num();
  for (ymuint i = 0; i < ne; ++ i) {
    PhfEdge* edge = node2->edge(i);
    PhfNode* node1 = edge->node1();
    if ( node1 == from ) {
      continue;
    }
    if ( v_mark[node1->id()] ) {
      return false;
    }
    edge_list.push_back(edge);
    edge->set_dir(1);
    v_mark[node1->id()] = true;
    bool stat = dfs1(node1, node2, v_mark, edge_list);
    if ( !stat ) {
      return false;
    }
  }
  return true;
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス PhfGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PhfGen::PhfGen()
{
}

// @brief デストラクタ
PhfGen::~PhfGen()
{
}

// @brief マッピングを求める．
bool
PhfGen::mapping(const vector<RegVect*>& vector_list,
		const InputFunc& f1,
		const InputFunc& f2,
		vector<ymuint32>& g1,
		vector<ymuint32>& g2)
{
  ymuint nv = vector_list.size();

  hash_map<ymuint32, PhfNode*> v1_hash;
  hash_map<ymuint32, PhfNode*> v2_hash;
  vector<PhfNode*> v1_array(nv);
  vector<PhfNode*> v2_array(nv);
  vector<PhfNode*> v_array;
  ymuint e_id = 0;
  for (ymuint i = 0; i < nv; ++ i) {
    RegVect* rv = vector_list[i];
    ymuint32 v1 = f1.eval(rv);
    hash_map<ymuint32, PhfNode*>::iterator p1 = v1_hash.find(v1);
    PhfNode* node1 = NULL;
    if ( p1 == v1_hash.end() ) {
      ymuint id1 = v1_hash.size();
      node1 = new PhfNode(id1, v1);
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
    hash_map<ymuint32, PhfNode*>::iterator p2 = v2_hash.find(v2);
    PhfNode* node2 = NULL;
    if ( p2 == v2_hash.end() ) {
      ymuint id2 = v2_hash.size() + offset;
      node2 = new PhfNode(id2, v2);
      v_array.push_back(node2);
      v2_hash.insert(make_pair(v2, node2));
    }
    else {
      node2 = p2->second;
    }
    v2_array[i] = node2;

    PhfNode* node1 = v1_array[i];
    PhfEdge* edge = new PhfEdge(e_id, node1, node2, i);
    node1->add_edge(edge);
    node2->add_edge(edge);
    ++ e_id;
  }

  ymuint nn = v_array.size();

  { // simple check
    bool not_simple = false;
    for (ymuint i = 0; i < nv; ++ i) {
      PhfNode* node = v1_array[i];
      ymuint ne = node->edge_num();
      vector<ymuint> mark(nn, false);
      for (ymuint j = 0; j < ne; ++ j) {
	PhfEdge* edge = node->edge(j);
	PhfNode* node2 = edge->node2();
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

  vector<PhfEdge*> edge_list;
  {
    vector<bool> v_mark(nn, false);

    for ( ; ; ) {
      PhfNode* node0 = NULL;
      for (ymuint i = 0; i < nn; ++ i) {
	PhfNode* node = v_array[i];
	if ( !v_mark[node->id()] ) {
	  node0 = node;
	  break;
	}
      }
      if ( node0 == NULL ) {
	break;
      }
      v_mark[node0->id()] = true;
      bool stat = dfs1(node0, NULL, v_mark, edge_list);
      if ( !stat ) {
	return false;
      }
    }
  }

  ymuint ne = edge_list.size();
  for (ymuint i = 0; i < ne; ++ i) {
    PhfEdge* edge = edge_list[i];
    PhfNode* node1 = edge->node1();
    PhfNode* node2 = edge->node2();
    if ( edge->dir() ) {
      ymuint32 val = node2->val() ^ edge->val();
      node1->set_val(val);
      g1[node1->pat()] = val;
    }
    else {
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
