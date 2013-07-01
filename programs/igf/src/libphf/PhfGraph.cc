
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
PhfGraph::PhfGraph(const vector<const FuncVect*>& func_list) :
  mAlloc(4096)
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
  for (ymuint n_pos = 0; n_pos < mNodeArraySize; ++ n_pos) {
    PhfNode* node = mNodeArray[n_pos];
    if ( node == NULL ) {
      continue;
    }
    if ( node->edge(0)->node(0) != node ) {
      // 最初のハッシュ値のノードだけを対象にする．
      continue;
    }

    ymuint ne = node->edge_num();
    for (ymuint e_pos1 = 0; e_pos1 < ne; ++ e_pos1) {
      PhfEdge* edge1 = node->edge(e_pos1);
      for (ymuint e_pos2 = e_pos1 + 1; e_pos2 < ne; ++ e_pos2) {
	PhfEdge* edge2 = node->edge(e_pos2);
	bool diff = false;
	for (ymuint l = 1; l < mDegree; ++ l) {
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
  edge_list.resize(mEdgeListSize);

  vector<pair<PhfNode*, PhfEdge*> > node_queue(mNodeArraySize);
  ymuint wpos = 0;
  for (ymuint n_pos = 0; n_pos < mNodeArraySize; ++ n_pos) {
    PhfNode* node = mNodeArray[n_pos];
    if ( node != NULL ) {
      ymuint ne = node->edge_num();
      if ( ne == 1 ) {
	PhfEdge* edge = node->edge(0);
	node_queue[wpos].first = node;
	node_queue[wpos].second = edge;
	++ wpos;
      }
    }
  }

  ymuint epos = mEdgeListSize;
  for (ymuint rpos = 0; rpos < wpos; ++ rpos) {
    PhfNode* node0 = node_queue[rpos].first;
    PhfEdge* edge0 = node_queue[rpos].second;

    if ( !node0->mActive ) {
      continue;
    }
    if ( !edge0->mActive ) {
      continue;
    }

    // node0 と edge0 を処理済みにする．
    node0->mActive = false;
    edge0->mActive = false;

    // edge0 をリストに加える．
    -- epos;
    edge_list[epos] = edge0;

    // edge0 に接続している未処理のノードを探す．
    for (ymuint i = 0; i < mDegree; ++ i) {
      PhfNode* node1 = edge0->node(i);
      if ( !node1->mActive ) {
	continue;
      }

      // node1 に接続している未処理の枝を探す．
      ymuint ne = node1->edge_num();
      ymuint d = 0;
      PhfEdge* edge1 = NULL;
      for (ymuint j = 0; j < ne; ++ j) {
	PhfEdge* edge = node1->edge(j);
	if ( edge->mActive ) {
	  ++ d;
	  edge1 = edge;
	}
      }
      if ( d == 1 ) {
	// 次数が1ならキューに積む．
	node_queue[wpos].first = node1;
	node_queue[wpos].second = edge1;
	++ wpos;
      }
    }
  }

  if ( epos != 0 ) {
    return false;
  }

  if ( VERIFY_ACYCLIC_CHECK ) {
    vector<bool> v_mark(mMaxId, false);
    bool error = false;
    for (vector<PhfEdge*>::iterator p = edge_list.begin();
	 p != edge_list.end(); ++ p) {
      PhfEdge* edge = *p;
      bool has_node = false;
      for (ymuint i = 0; i < mDegree; ++ i) {
	PhfNode* node = edge->node(i);
	if ( !v_mark[node->id()] ) {
	  has_node = true;
	  v_mark[node->id()] = true;
	}
      }
      if ( !has_node ) {
	error = true;
      }
    }
    if ( error ) {
      for (ymuint i = 0; i < mNodeArraySize; ++ i) {
	PhfNode* node = mNodeArray[i];
	if ( node != NULL ) {
	  cout << "Node#" << i << ": " << node->pat() << endl;
	}
      }
      cout << endl;
      for (ymuint i = 0; i < mEdgeListSize; ++ i) {
	cout << "Edge#" << i << ": ";
	PhfEdge* edge = mEdgeList[i];
	for (ymuint j = 0; j < mDegree; ++ j) {
	  cout << " Node#" << edge->node(j)->id();
	}
	cout << endl;
      }
      cout << endl;
      cout << "EdgeList = ";
      for (ymuint i = 0; i < mEdgeListSize; ++ i) {
	cout << " " << edge_list[i]->id();
      }
      cout << endl;
    }
    assert_cond( !error, __FILE__, __LINE__);
  }

  return true;
}

// @brief 関数のリストからグラフを作る．
// @param[in] func_list 関数のリスト
void
PhfGraph::gen_graph(const vector<const FuncVect*>& func_list)
{
  ymuint nf = func_list.size();
  assert_cond( nf > 1, __FILE__, __LINE__);

  mDegree = nf;

  ymuint nv = func_list[0]->input_size();

  mEdgeListSize = nv;
  mEdgeList = new PhfEdge*[mEdgeListSize];

  mMaxId = 0;
  ymuint orange = func_list[0]->max_val();
  mNodeArraySize = nf * orange;
  mNodeArray = new PhfNode*[mNodeArraySize];
  for (ymuint i = 0; i < mNodeArraySize; ++ i) {
    mNodeArray[i] = NULL;
  }

  for (ymuint v_pos = 0; v_pos < nv; ++ v_pos) {
    PhfEdge* edge = new_edge(v_pos, v_pos);
    for (ymuint f_pos = 0; f_pos < nf; ++ f_pos) {
      const FuncVect& f1 = *func_list[f_pos];
      ymuint32 pat1 = f1.val(v_pos);

      PhfNode* node1 = mNodeArray[pat1 + f_pos * orange];
      if ( node1 == NULL ) {
	node1 = new_node(pat1);
	mNodeArray[pat1 + f_pos * orange] = node1;
      }

      // edge の f_pos 番めの要素に node1 を設定する．
      edge->mNodeList[f_pos] = node1;
      node1->add_edge(edge);
    }
  }

  mNodeList = new PhfNode*[mMaxId];
  for (ymuint i = 0; i < mNodeArraySize; ++ i) {
    PhfNode* node = mNodeArray[i];
    if ( node != NULL ) {
      mNodeList[node->id()] = node;
    }
  }
}

// @brief ノードを枝を開放する．
void
PhfGraph::clear()
{
  for (ymuint i = 0; i < mNodeArraySize; ++ i) {
    PhfNode* node = mNodeArray[i];
    delete node;
  }

  mNodeArraySize = 0;
  delete [] mNodeArray;

  mEdgeListSize = 0;
  delete [] mEdgeList;

  mMaxId = 0;
  delete [] mNodeList;

  mAlloc.destroy();
}

// @brief ノードを生成する．
PhfNode*
PhfGraph::new_node(ymuint32 pat)
{
  ymuint id = mMaxId;
  ++ mMaxId;
  PhfNode* node = new PhfNode(id, pat);
  node->mActive = true;

  return node;
}

// @brief 枝を生成する．
PhfEdge*
PhfGraph::new_edge(ymuint id,
		   ymuint32 val)
{
  void* p = mAlloc.get_memory(sizeof(PhfEdge) + sizeof(PhfNode*) * (mDegree - 1));
  PhfEdge* edge = new (p) PhfEdge(id, val);
  mEdgeList[id] = edge;
  edge->mActive = true;

  return edge;
}

END_NAMESPACE_YM_IGF
