
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
dfs(PhfNode* node,
    PhfEdge* from,
    vector<bool>& v_mark,
    vector<PhfEdge*>& edge_list)
{
  ymuint ne = node->edge_num();
  for (ymuint i = 0; i < ne; ++ i) {
    PhfEdge* edge = node->edge(i);
    if ( edge == from ) {
      continue;
    }

    edge_list.push_back(edge);

    ymuint nn = edge->node_num();
    for (ymuint i = 0; i < nn; ++ i) {
      PhfNode* node1 = edge->node(i);
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
  clear();
}

// @brief マッピングを求める．
bool
PhfGen::mapping(const vector<RegVect*>& vector_list,
		const vector<const InputFunc*>& f_list,
		vector<vector<ymuint32>* >& g_list)
{
  clear();

  ymuint nv = vector_list.size();

  ymuint nf = f_list.size();
  assert_cond( g_list.size() == nf, __FILE__, __LINE__);

  hash_map<ymuint32, PhfNode*>* v_hash_array = new hash_map<ymuint32, PhfNode*>[nf];

  for (ymuint v_pos = 0; v_pos < nv; ++ v_pos) {
    RegVect* rv = vector_list[v_pos];

    vector<PhfNode*> node_list(nf);
    for (ymuint f_pos = 0; f_pos < nf; ++ f_pos) {
      const InputFunc& f1 = *f_list[f_pos];
      hash_map<ymuint32, PhfNode*>& v1_hash = v_hash_array[f_pos];

      ymuint32 pat1 = f1.eval(rv);
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

  ymuint nn = mNodeList.size();

  { // simple check
    bool not_simple = false;
    for (ymuint n_pos = 0; n_pos < nn; ++ n_pos) {
      PhfNode* node = mNodeList[n_pos];
      if ( node->edge(0)->node(0) != node ) {
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

  vector<PhfEdge*> edge_list;
  {
    vector<bool> v_mark(nn, false);

    for ( ; ; ) {
      PhfNode* node0 = NULL;
      for (ymuint i = 0; i < nn; ++ i) {
	PhfNode* node = mNodeList[i];
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
    PhfEdge* edge = edge_list[i];
    ymuint nn = edge->node_num();
    for (ymuint j = 0; j < nn; ++ j) {
      PhfNode* node1 = edge->node(j);
      if ( !node1->is_assigned() ) {
	ymuint32 val = edge->val();
	for (ymuint k = 0; k < nn; ++ k) {
	  if ( k != j ) {
	    PhfNode* node2 = edge->node(k);
	    val ^= node2->val();
	  }
	}
	node1->set_val(val);
	vector<ymuint32>& g1 = *g_list[j];
	g1[node1->pat()] = val;
      }
    }
  }

  return true;
}

// @brief ノードを枝を開放する．
void
PhfGen::clear()
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
PhfGen::new_node(ymuint32 pat)
{
  ymuint id = mNodeList.size();
  PhfNode* node = new PhfNode(id, pat);
  mNodeList.push_back(node);
  return node;
}

// @brief 枝を生成する．
PhfEdge*
PhfGen::new_edge(ymuint id,
		 const vector<PhfNode*>& node_list,
		 ymuint32 val)
{
  PhfEdge* edge = new PhfEdge(id, node_list, val);
  mEdgeList.push_back(edge);
  return edge;
}

END_NAMESPACE_YM_IGF
