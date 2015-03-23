
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
#include "YmUtils/BtgMatch.h"


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
  for (ymuint i = 0; i < mMaxId; ++ i) {
    mNodeList[i]->mActive = true;
  }
  for (ymuint i = 0; i < mEdgeListSize; ++ i) {
    mEdgeList[i]->mActive = true;
  }

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
    ASSERT_COND( !error );
  }

  return true;
}

// @brief 分割を行う．
bool
PhfGraph::cf_partition(vector<ymuint>& block_map)
{
  block_map.clear();
  block_map.resize(mEdgeListSize);

  // 実は acyclic_check() と似たことをやる．
  for (ymuint i = 0; i < mMaxId; ++ i) {
    mNodeList[i]->mActive = true;
  }
  for (ymuint i = 0; i < mEdgeListSize; ++ i) {
    mEdgeList[i]->mActive = true;
  }

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

  ymuint epos = 0;
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

    for (ymuint i = 0; i < mDegree; ++ i) {
      if ( edge0->node(i) == node0 ) {
	block_map[edge0->id()] = i;
	break;
      }
    }
    ++ epos;

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

  if ( epos == mEdgeListSize ) {
    // 全ての枝を処理した．
    return true;
  }

  // 残った枝に関係するノード数を数える．
  ymuint ne = 0;
  vector<ymuint> edge_map(mEdgeListSize);
  vector<PhfEdge*> redge_map;
  vector<PhfNode*> node_list;
  vector<ymuint> node_map(mMaxId);
  {
    vector<bool> node_mark(mMaxId, false);
    for (ymuint i = 0; i < mEdgeListSize; ++ i) {
      PhfEdge* edge = mEdgeList[i];
      if ( !edge->mActive ) {
	continue;
      }
      edge_map[edge->id()] = ne;
      redge_map.push_back(edge);
      ++ ne;
      for (ymuint j = 0; j < mDegree; ++ j) {
	PhfNode* node = edge->node(j);
	if ( !node_mark[node->id()] ) {
	  node_mark[node->id()] = true;
	  ymuint node_id = node_list.size();
	  node_map[node->id()] = node_id;
	  node_list.push_back(node);
	}
      }
    }
  }
  ymuint nn = node_list.size();

  // ノード数が枝数より少なければマッチングしない．
  if ( nn < ne ) {
    return false;
  }

  cout << "  begin btg match";
  cout.flush();

  // 残ったグラフを別の2部グラフに変換してマッチングを求める．
  BtgMatch bm;

  bm.set_size(ne, nn);

  for (ymuint i = 0; i < mEdgeListSize; ++ i) {
    PhfEdge* edge = mEdgeList[i];
    if ( !edge->mActive ) {
      continue;
    }
    for (ymuint j = 0; j < mDegree; ++ j) {
      PhfNode* node = edge->node(j);
      bm.add_edge(edge_map[edge->id()], node_map[node->id()]);
    }
  }

  vector<ymuint> edge_list;
  bool stat = bm.calc_match(edge_list);

  cout << " end" << endl;

  if ( !stat ) {
    return false;
  }

  ASSERT_COND( edge_list.size() == ne );
  for (ymuint i = 0; i < ne; ++ i) {
    ymuint eid = edge_list[i];
    ymuint v1;
    ymuint v2;
    ymuint w;
    bm.edge_info(eid, v1, v2, w);
    PhfEdge* phf_edge = redge_map[v1];
    PhfNode* phf_node = node_list[v2];

    for (ymuint j = 0; j < mDegree; ++ j) {
      PhfNode* node1 = phf_edge->node(j);
      if ( node1 == phf_node ) {
	block_map[phf_edge->id()] = j;
	break;
      }
    }
  }

  // 検証用のコード
  vector<bool> v_array(mMaxId, false);
  for (ymuint i = 0; i < mEdgeListSize; ++ i) {
    PhfEdge* edge = mEdgeList[i];
    ymuint pos = block_map[edge->id()];
    PhfNode* node = edge->node(pos);
    ASSERT_COND( !v_array[node->id()] );
    v_array[node->id()] = true;
  }

  return true;
}

// @brief 関数のリストからグラフを作る．
// @param[in] func_list 関数のリスト
void
PhfGraph::gen_graph(const vector<const FuncVect*>& func_list)
{
  ymuint nf = func_list.size();
  ASSERT_COND( nf > 1 );

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

BEGIN_NONAMESPACE

struct Lt
{
  bool
  operator()(const pair<ymuint, PhfNode*>& left,
	     const pair<ymuint, PhfNode*>& right)
  {
    return left.first < right.first;
  }

};

struct Gt
{
  bool
  operator()(const pair<ymuint, PhfNode*>& left,
	     const pair<ymuint, PhfNode*>& right)
  {
    return left.first > right.first;
  }

};

END_NONAMESPACE

// @brief displace_decomposition を行う．
bool
PhfGraph::displace_decomposition(vector<ymuint>& displace_map,
				 bool use_xor)
{
  ASSERT_COND( mDegree == 2 );

  ymuint orange = mNodeArraySize / 2;
  vector<pair<ymuint, PhfNode*> > node1_list;
  vector<pair<ymuint, PhfNode*> > node2_list;
  node1_list.reserve(orange);
  node2_list.reserve(orange);
  vector<bool> used(orange, false);
  for (ymuint i = 0; i < mNodeArraySize; ++ i) {
    PhfNode* node = mNodeArray[i];
    if ( node == NULL ) {
      continue;
    }
    ASSERT_COND( node->edge_num() > 0 );
    PhfEdge* edge0 = node->edge(0);
    if ( edge0->node(0) == node ) {
      node1_list.push_back(make_pair(node->edge_num(), node));
    }
    else {
      ASSERT_COND( edge0->node(1) == node );
      node2_list.push_back(make_pair(node->edge_num(), node));
    }
  }

  displace_map.clear();
  displace_map.resize(orange, 0);

  sort(node2_list.begin(), node2_list.end(), Gt());

  for (vector<pair<ymuint, PhfNode*> >::iterator p = node2_list.begin();
       p != node2_list.end(); ++ p) {
    PhfNode* node = p->second;
    ymuint pat = node->pat();
    ymuint ne = node->edge_num();
    bool found = false;
    for (ymuint d = 0; d < orange; ++ d) {
      bool conflict = false;
      for (ymuint i = 0; i < ne; ++ i) {
	PhfEdge* edge = node->edge(i);
	PhfNode* alt_node = edge->node(0);
	ymuint pos = alt_node->pat();
	if ( use_xor ) {
	  pos ^= d;
	}
	else {
	  pos = (pos + d) % orange;
	}
	if ( used[pos] ) {
	  conflict = true;
	  break;
	}
      }
      if ( conflict ) {
	continue;
      }

      for (ymuint i = 0; i < ne; ++ i) {
	PhfEdge* edge = node->edge(i);
	PhfNode* alt_node = edge->node(0);
	ymuint pos = alt_node->pat();
	if ( use_xor ) {
	  pos ^= d;
	}
	else {
	  pos = (pos + d) % orange;
	}
	used[pos] = true;
      }

      displace_map[pat] = d;
      found = true;
      break;
    }
    if ( !found ) {
      return false;
    }
  }
  return true;
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
