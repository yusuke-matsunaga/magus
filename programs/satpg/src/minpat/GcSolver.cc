
/// @file GcSolver.cc
/// @brief GcSolver の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "GcSolver.h"
#include "GcNode.h"
#include "GcNodeHeap.h"
#include "TpgFault.h"
#include "TpgCnf1.h"

#include "YmUtils/Graph.h"
#include "YmUtils/BtgMatch.h"
#include "YmUtils/HashSet.h"


BEGIN_NAMESPACE_YM_SATPG

extern
void
verify_suf_list(TpgFault* fault,
		ymuint max_id,
		const NodeValList& suf_list);

BEGIN_NONAMESPACE

// @brief ノードに彩色して情報を更新する．
void
color_node(GcNode* node,
	   ymuint color,
	   GcNodeHeap& node_heap)
{
  // node に color の色を割り当てる．
  node->set_color(color);

  // node に隣接するノードの SAT degree を更新する．
  const vector<GcNode*>& node_list = node->link_list();
  for (vector<GcNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    GcNode* node1 = *p;
    if ( node1->color() == 0 ) {
      // node1 が未着色の場合
      if ( !node1->check_adj_color(color) ) {
	// node1 にとって color は新規の隣り合う色だった．
	node1->add_adj_color(color);

	// SAT degree が変わったのでヒープ上の位置も更新する．
	node_heap.update(node1);
      }
    }
  }
}

struct ColLt
{
  bool
  operator()(const pair<ymuint, ymuint>& left,
	     const pair<ymuint, ymuint>& right)
  {
    return left.first < right.first;
  }
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス GcSolver
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fault_list 故障リスト
// @param[in] max_id ノード番号の最大値
GcSolver::GcSolver(const vector<TpgFault*>& fault_list,
		   ymuint max_id) :
  mFaultList(fault_list)
{
  mMaxId = max_id;
}

// @brief デストラクタ
GcSolver::~GcSolver()
{
  for (ymuint i = 0; i < mColList.size(); ++ i) {
    delete mColList[i];
  }
}

// @brief 枝を追加する．
// @param[in] id1, id2 ノード番号 ( 0 <= id1, id2 < node_num() )
void
GcSolver::connect(ymuint id1,
		  ymuint id2)
{
  ASSERT_COND( id1 < mFaultList.size() );
  ASSERT_COND( id2 < mFaultList.size() );
  mEdgeList.push_back(make_pair(id1, id2));
}

// @brief 彩色する．
// @return 彩色数を返す．
ymuint
GcSolver::coloring()
{
  // データ構造を作る．
  ymuint max_fault_id = 0;
  ymuint fault_num = mFaultList.size();
  ymuint vectlen = (fault_num + 63) / 64;
  GcNode* node_array = new GcNode[fault_num];
  for (ymuint i = 0; i < fault_num; ++ i) {
    GcNode* node = &node_array[i];
    TpgFault* fault = mFaultList[i];
    node->init(i, fault, vectlen);
    if ( max_fault_id < fault->id() ) {
      max_fault_id = fault->id();
    }
  }
  ++ max_fault_id;
  for (ymuint i = 0; i < mEdgeList.size(); ++ i) {
    ymuint id1 = mEdgeList[i].first;
    ymuint id2 = mEdgeList[i].second;
    GcNode* node1 = &node_array[id1];
    GcNode* node2 = &node_array[id2];
    nsYm::nsSatpg::connect(node1, node2);
  }
  if ( false ) {
    Graph graph(fault_num);
    for (ymuint i = 0; i < mEdgeList.size(); ++ i) {
      ymuint id1 = mEdgeList[i].first;
      ymuint id2 = mEdgeList[i].second;
      graph.connect(id1, id2);
    }
    vector<ymuint> node_set;
    ymuint nc = max_clique_exact(graph, node_set);
    cout << " # of independent_set = " << nc << endl;

    vector<vector<ymuint> > color_group;
    ymuint nc2 = nsYm::coloring(graph, color_group);
    cout << " # of colors = " << nc2 << endl;
  }

#if 1
  // dsatur アルゴリズムを用いる．

  GcNodeHeap node_heap(fault_num);

  for (ymuint i = 0; i < fault_num; ++ i) {
    GcNode* node = &node_array[i];
    node_heap.put_node(node);
  }

  // 1: 隣接するノード数が最大のノードを選び彩色する．
  //    ソートしているので先頭のノードを選べば良い．
  GcNode* max_node = node_heap.get_min();
  new_color(max_node->fault());
  color_node(max_node, mColList.size(), node_heap);

  // 2: saturation degree が最大の未彩色ノードを選び最小の色番号で彩色する．
  while ( !node_heap.empty() ) {
    GcNode* max_node = node_heap.get_min();

    const vector<GcNode*>& node_list = max_node->link_list();

    // max_node に隣接しているノードで未彩色のノードを free_list に入れる．
    vector<GcNode*> free_list;
    free_list.reserve(node_list.size());
    for (vector<GcNode*>::const_iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      GcNode* node1 = *p;
      ymuint c = node1->color();
      if ( c == 0 ) {
	free_list.push_back(node1);
      }
    }

    // max_node に隣接しているノードに付いていない色を color_list に入れる．
    vector<pair<ymuint, ymuint> > color_list;
    color_list.reserve(mColList.size());
    for (ymuint c = 1; c <= mColList.size(); ++ c) {
      if ( max_node->check_adj_color(c) ) {
	continue;
      }
      ymuint n = 0;
      for (vector<GcNode*>::const_iterator q = free_list.begin();
	   q != free_list.end(); ++ q) {
	GcNode* node1 = *q;
	if ( !node1->check_adj_color(c) ) {
	  ++ n;
	}
      }
      color_list.push_back(make_pair(c, n));
    }

    TpgFault* fault = max_node->fault();

    // max_node につけることのできる最小の色番号を求める．
    ymuint min_col = 0;
    if ( color_list.empty() ) {
      // 新しい色を割り当てる．
      new_color(fault);
      min_col = mColList.size();
    }
    else {
      TpgCnf1 tpg_cnf(string(), string(), NULL);
      tpg_cnf.make_fval_cnf(max_node->fault(), mMaxId);

      sort(color_list.begin(), color_list.end(), ColLt());

      for (ymuint i = 0; i < color_list.size(); ++ i) {
	ymuint col = color_list[i].first;
	const NodeValList& suf_list0 = mColList[col - 1]->mSufList;
	NodeValList suf_list;
	if ( tpg_cnf.get_suf_list(suf_list0, suf_list) ) {
	  ColInfo* cip = mColList[col - 1];
	  cip->mFaultList.push_back(max_node->fault());
	  cip->mSufList.merge(suf_list);
	  min_col = col;
	  break;
	}
      }
      if ( min_col == 0 ) {
	// 新しい色を割り当てる．
	new_color(fault);
	min_col = mColList.size();
      }
    }

    color_node(max_node, min_col, node_heap);
  }
#else
  HashSet<ymuint> mark;
  { // 核となるクリークを求める．
    Graph graph(fault_num);
    for (ymuint i = 0; i < mEdgeList.size(); ++ i) {
      ymuint id1 = mEdgeList[i].first;
      ymuint id2 = mEdgeList[i].second;
      graph.connect(id1, id2);
    }
    vector<ymuint> node_set;
    ymuint nn = max_clique_exact(graph, node_set);
    for (ymuint i = 0; i < nn; ++ i) {
      ymuint fid = node_set[i];
      TpgFault* fault = mFaultList[fid];
      new_color(fault);
      mark.add(fid);
    }
  }
  for ( ; ; ) {
    vector<ymuint> fault_list;
    vector<ymuint> lid_map(mFaultList.size());
    for (ymuint i = 0; i < mFaultList.size(); ++ i) {
      if ( !mark.check(i) ) {
	ymuint local_id = fault_list.size();
	fault_list.push_back(i);
	lid_map[i] = local_id;
      }
      else {
	lid_map[i] = mFaultList.size();
      }
    }
    if ( fault_list.empty() ) {
      break;
    }
    cout << "remaining faults: " << fault_list.size() << endl;
    {
      Graph graph(fault_list.size());
      for (ymuint i = 0; i < mEdgeList.size(); ++ i) {
	ymuint id1 = mEdgeList[i].first;
	ymuint id2 = mEdgeList[i].second;
	ymuint lid1 = lid_map[id1];
	ymuint lid2 = lid_map[id2];
	if ( lid1 == mFaultList.size() || lid2 == mFaultList.size() ) {
	  continue;
	}
	graph.connect(lid1, lid2);
      }
      vector<ymuint> node_set;
      ymuint nn = max_clique_exact(graph, node_set);
      cout << " select " << nn << " faults" << endl;
      // すでにある mColList と node_set のマッチングを求める．
      ymuint nc = mColList.size();
      vector<NodeValList> suf_list_array(nc * nn);
      vector<bool> edge_array(nc * nn);
      vector<NodeValList> suf0_list_array(nn);
      for (ymuint i = 0; i < nn; ++ i) {
	ymuint lid = node_set[i];
	ymuint id = fault_list[lid];
	mark.add(id);
	TpgFault* fault = mFaultList[id];
	TpgCnf1 tpg_cnf(string(), string(), NULL);
	tpg_cnf.make_fval_cnf(fault, mMaxId);
	for (ymuint j = 0; j < nc; ++ j) {
	  ColInfo* cip = mColList[j];
	  const NodeValList& suf_list0 = cip->mSufList;
	  NodeValList& suf_list = suf_list_array[j * nn + i];
	  edge_array[j * nn + i] = tpg_cnf.get_suf_list(suf_list0, suf_list);
	}
	tpg_cnf.get_suf_list(NodeValList(), suf0_list_array[i]);
      }

      ymuint n1 = nc;
      if ( n1 < nn ) {
	n1 = nn;
      }
      for ( ; ; ) {
	BtgMatch bm;
	bm.set_size(n1, nn);
	for (ymuint i = 0; i < nn; ++ i) {
	  for (ymuint j = 0; j < nc; ++ j) {
	    if ( edge_array[j * nn + i] ) {
	      bm.add_edge(j, i);
	    }
	  }
	  for (ymuint j = nc; j < n1; ++ j) {
	    bm.add_edge(j, i);
	  }
	}
	vector<ymuint> edge_list;
	ymuint n = bm.calc_match(edge_list);
	if ( n == nn ) {
	  // マッチングが見つかった．
	  for (ymuint i = 0; i < edge_list.size(); ++ i) {
	    ymuint edge_id = edge_list[i];
	    ymuint id1;
	    ymuint id2;
	    ymuint dummy;
	    bm.edge_info(edge_id, id1, id2, dummy);
	    ymuint lid = node_set[id2];
	    ymuint fid = fault_list[lid];
	    TpgFault* fault = mFaultList[fid];
	    if ( id1 < nc ) {
	      ColInfo* cip = mColList[id1];
	      cip->mFaultList.push_back(fault);
	      cip->mSufList.merge(suf_list_array[id1 * nn + id2]);
	    }
	    else {
	      while ( mColList.size() <= id1 ) {
		ColInfo* cip = new ColInfo;
		mColList.push_back(cip);
	      }
	      ColInfo* cip = mColList[id1];
	      cip->mFaultList.push_back(fault);
	      cip->mSufList = suf0_list_array[id2];
	    }
	  }
	  break;
	}
	++ n1;
      }
    }
  }

#endif

  if ( true ) { // 検証
    for (ymuint i = 0; i < mColList.size(); ++ i) {
      ColInfo* cip = mColList[i];
      for (ymuint j = 0; j < cip->mFaultList.size(); ++ j) {
	TpgFault* f = cip->mFaultList[j];
	verify_suf_list(f, mMaxId, cip->mSufList);
      }
    }
  }
  // 検証
  // もちろん最小色数ではないが，同じ色が隣接していないことを確認する．
  // また，未彩色のノードがないことも確認する．
  // 違反が見つかったら例外を送出する．
  if ( false ) {
    for (ymuint i = 0; i < fault_num; ++ i) {
      GcNode* node = &node_array[i];
      ymuint c0 = node->color();
      ASSERT_COND( c0 > 0 );

      const vector<GcNode*>& node_list = node->link_list();
      for (vector<GcNode*>::const_iterator p = node_list.begin();
	   p != node_list.end(); ++ p) {
	GcNode* node1 = *p;
	ymuint c1 = node1->color();
	ASSERT_COND( c0 != c1 );
      }
    }
  }

  delete [] node_array;

  return mColList.size();
}

// @brief 故障リストを返す．
// @param[in] col 色番号(1が最初)
const vector<TpgFault*>&
GcSolver::fault_list(ymuint col) const
{
  ASSERT_COND( col <= mColList.size() );
  return mColList[col - 1]->mFaultList;
}

// @brief 十分割当リストを返す．
// @param[in] col 色番号(1が最初)
const NodeValList&
GcSolver::suf_list(ymuint col) const
{
  ASSERT_COND( col <= mColList.size() );
  return mColList[col - 1]->mSufList;
}

// @brief 新しい色を割り当てる．
void
GcSolver::new_color(TpgFault* fault)
{
  TpgCnf1 tpg_cnf(string(), string(), NULL);

  tpg_cnf.make_fval_cnf(fault, mMaxId);

  ColInfo* cip = new ColInfo;
  cip->mColId = mColList.size();
  cip->mFaultList.push_back(fault);
  NodeValList suf_list;
  bool stat = tpg_cnf.get_suf_list(NodeValList(), suf_list);
  ASSERT_COND( stat );
  cip->mSufList = suf_list;
  mColList.push_back(cip);
}


//////////////////////////////////////////////////////////////////////
// クラス GcNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GcNode::GcNode()
{
  mColorSet = NULL;
  mAdjDegree = 0;
  mSatDegree = 0;
  mColor = 0;
}

// @brief デストラクタ
GcNode::~GcNode()
{
  delete [] mColorSet;
}

// @brief 初期化する．
void
GcNode::init(ymuint id,
	     TpgFault* fault,
	     ymuint vectlen)
{
  mId = id;
  mFault = fault;
  mColorSet = new ymuint64[vectlen];
  for (ymuint i = 0; i < vectlen; ++ i) {
    mColorSet[i] = 0UL;
  }
}

// @brief 隣接するノードを削除する．
void
GcNode::delete_link(GcNode* node)
{
  -- mAdjDegree;
}

END_NAMESPACE_YM_SATPG
