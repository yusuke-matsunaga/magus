
/// @file GcSolver.cc
/// @brief GcSolver の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "GcSolver.h"
#include "GcNode.h"
#include "GcNodeHeap.h"
#include "NodeValList.h"
#include "TpgFault.h"
#include "TpgCnf1.h"
#include "YmUtils/Graph.h"


BEGIN_NAMESPACE_YM_SATPG

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
    if ( node1->is_deleted() ) {
      continue;
    }
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
// @param[in] node_num ノード数
GcSolver::GcSolver(ymuint node_num,
		   ymuint max_id)
{
  mNodeNum = node_num;
  if ( mNodeNum > 0 ) {
    ymuint vectlen = (mNodeNum + 63) / 64;
    mNodeArray = new GcNode[mNodeNum];
    for (ymuint i = 0; i < mNodeNum; ++ i) {
      GcNode* node = &mNodeArray[i];
      node->init(i, vectlen);
    }
  }
  else {
    mNodeArray = NULL;
  }
  mMaxId = max_id;
}

// @brief デストラクタ
GcSolver::~GcSolver()
{
  delete [] mNodeArray;
}

// @brief ノード数を得る．
ymuint
GcSolver::node_num() const
{
  return mNodeNum;
}

// @brief ノードを取り出す．
// @param[in] id ノード番号 ( 0 <= id < node_num() )
GcNode*
GcSolver::node(ymuint id)
{
  ASSERT_COND( id < node_num() );
  return &mNodeArray[id];
}

// @brief 彩色する．
// @param[in] color_group 同じ色のノード番号のリストの配列
// @return 彩色数を返す．
ymuint
GcSolver::coloring(vector<vector<ymuint> >& color_group)
{
  // dsatur アルゴリズムを用いる．

  GcNodeHeap node_heap(mNodeNum);

  for (ymuint i = 0; i < mNodeNum; ++ i) {
    GcNode* node = &mNodeArray[i];
    node_heap.put_node(node);
  }

  mMaxColor = 1;

  vector<NodeValList> suf_list_array;

  // 1: 隣接するノード数が最大のノードを選び彩色する．
  //    ソートしているので先頭のノードを選べば良い．
  GcNode* max_node = node_heap.get_min();
  {
    TpgFault* fault = max_node->fault();
    TpgCnf1 tpg_cnf(string(), string(), NULL);

    tpg_cnf.make_fval_cnf(max_node->fault(), mMaxId);
    NodeValList suf_list;
    bool stat = tpg_cnf.get_suf_list(suf_list);
    ASSERT_COND( stat );
    suf_list_array.push_back(suf_list);
  }
  color_node(max_node, mMaxColor, node_heap);

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
    color_list.reserve(mMaxColor);
    for (ymuint c = 1; c <= mMaxColor; ++ c) {
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
    TpgCnf1 tpg_cnf(string(), string(), NULL);

    tpg_cnf.make_fval_cnf(max_node->fault(), mMaxId);

    // max_node につけることのできる最小の色番号を求める．
    ymuint min_col = 0;
    if ( color_list.empty() ) {
      // 新しい色を割り当てる．
      ++ mMaxColor;
      min_col = mMaxColor;
      NodeValList suf_list;
      bool stat = tpg_cnf.get_suf_list(suf_list);
      ASSERT_COND( stat );
      suf_list_array.push_back(suf_list);
    }
    else {
      sort(color_list.begin(), color_list.end(), ColLt());
      for (ymuint i = 0; i < color_list.size(); ++ i) {
	ymuint col = color_list[i].first;
	NodeValList& suf_list0 = suf_list_array[col - 1];
	if ( tpg_cnf.get_suf_list(suf_list0) ) {
	  min_col = col;
	  break;
	}
      }
      if ( min_col == 0 ) {
	// 新しい色を割り当てる．
	++ mMaxColor;
	min_col = mMaxColor;
	NodeValList suf_list;
	bool stat = tpg_cnf.get_suf_list(suf_list);
	ASSERT_COND( stat );
	suf_list_array.push_back(suf_list);
      }
    }

    color_node(max_node, min_col, node_heap);
  }

  // 検証
  // もちろん最小色数ではないが，同じ色が隣接していないことを確認する．
  // また，未彩色のノードがないことも確認する．
  // 違反が見つかったら例外を送出する．
  if ( true ) {
    for (ymuint i = 0; i < mNodeNum; ++ i) {
      GcNode* node = &mNodeArray[i];
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

  // 結果を color_group に入れる．
  color_group.clear();
  color_group.resize(mMaxColor);
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    GcNode* node = &mNodeArray[i];
    if ( node->is_deleted() ) {
      continue;
    }

    ymuint c = node->color();

    ASSERT_COND( c > 0 );
    ASSERT_COND( c <= mMaxColor );

    color_group[c - 1].push_back(i);
  }

  return mMaxColor;
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
  mSelected = false;
  mDeleted = false;
}

// @brief デストラクタ
GcNode::~GcNode()
{
  delete [] mColorSet;
}

// @brief 初期化する．
void
GcNode::init(ymuint id,
	     ymuint vectlen)
{
  mId = id;
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
