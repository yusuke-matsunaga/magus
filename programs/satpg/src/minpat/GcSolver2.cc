
/// @file GcSolver2.cc
/// @brief GcSolver2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "GcSolver2.h"
#include "GcNode.h"
#include "GcNodeHeap.h"
#include "TpgFault.h"
#include "TpgCnf1.h"


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
// クラス GcSolver2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fault_list 故障リスト
// @param[in] max_id ノード番号の最大値
GcSolver2::GcSolver2(const vector<TpgFault*>& fault_list,
		     ymuint max_id) :
  mFaultList(fault_list)
{
  mMaxId = max_id;
}

// @brief デストラクタ
GcSolver2::~GcSolver2()
{
  for (ymuint i = 0; i < mColList.size(); ++ i) {
    delete mColList[i];
  }
}

// @brief 枝を追加する．
// @param[in] id1, id2 ノード番号 ( 0 <= id1, id2 < node_num() )
void
GcSolver2::connect(ymuint id1,
		   ymuint id2)
{
  ASSERT_COND( id1 < mFaultList.size() );
  ASSERT_COND( id2 < mFaultList.size() );
  mEdgeList.push_back(make_pair(id1, id2));
}

// @brief 彩色する．
// @return 彩色数を返す．
ymuint
GcSolver2::coloring()
{
  RandGen rg;
  ymuint min_col = mFaultList.size();
  for (ymuint i = 0; i < 50; ++ i) {
    vector<ColInfo*> col_list;
    int nc = coloring1(rg, mFaultList, col_list);
    cout << "# of colors = " << nc << endl;
    if ( min_col > nc ) {
#if 0
      for (ymuint j = 0; j < mColList.size(); ++ j) {
	delete [] mColList[j];
      }
#endif
      min_col = nc;
      mColList = col_list;
    }
  }
  return mColList.size();
}

// @brief 彩色する．
// @return 彩色数を返す．
ymuint
GcSolver2::coloring1(RandGen& rg,
		     const vector<TpgFault*>& fault_list,
		     vector<ColInfo*>& col_list)
{
  ymuint fault_num = fault_list.size();
  ymuint vectlen = (fault_num + 63) / 64;
  GcNode* node_array = new GcNode[fault_num];
  for (ymuint i = 0; i < fault_num; ++ i) {
    GcNode* node = &node_array[i];
    node->init(i, fault_list[i], vectlen);
  }
  for (ymuint i = 0; i < mEdgeList.size(); ++ i) {
    ymuint id1 = mEdgeList[i].first;
    ymuint id2 = mEdgeList[i].second;
    GcNode* node1 = &node_array[id1];
    GcNode* node2 = &node_array[id2];
    nsYm::nsSatpg::connect(node1, node2);
  }

  // dsatur アルゴリズムを用いる．

  GcNodeHeap node_heap(fault_num);

  for (ymuint i = 0; i < fault_num; ++ i) {
    GcNode* node = &node_array[i];
    node_heap.put_node(node);
  }

  // 1: 隣接するノード数が最大のノードを選び彩色する．
  //    ソートしているので先頭のノードを選べば良い．
  GcNode* max_node = node_heap.get_min();
  new_color(max_node->fault(), col_list);
  color_node(max_node, col_list.size(), node_heap);

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
    color_list.reserve(col_list.size());
    for (ymuint c = 1; c <= col_list.size(); ++ c) {
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
      new_color(fault, col_list);
      min_col = col_list.size();
    }
    else {
      TpgCnf1 tpg_cnf(string(), string(), NULL);
      tpg_cnf.make_fval_cnf(max_node->fault(), mMaxId);

      for (ymuint nc = color_list.size(); nc > 0; ) {
	ymuint pos = rg.int32() % nc;
	ymuint col = color_list[pos].first;
	if ( pos < nc - 1 ) {
	  color_list[pos] = color_list[nc - 1];
	}
	-- nc;
	const NodeValList& suf_list0 = col_list[col - 1]->mSufList;
	NodeValList suf_list;
	if ( tpg_cnf.get_suf_list(suf_list0, suf_list) ) {
	  ColInfo* cip = col_list[col - 1];
	  cip->mFaultList.push_back(max_node->fault());
	  cip->mFaultSufList.push_back(suf_list);
	  cip->mSufList.merge(suf_list);
	  min_col = col;
	  break;
	}
      }

      if ( min_col == 0 ) {
	// 新しい色を割り当てる．
	new_color(fault, col_list);
	min_col = col_list.size();
      }
    }

    color_node(max_node, min_col, node_heap);
  }

  if ( false ) { // 検証
    for (ymuint i = 0; i < col_list.size(); ++ i) {
      ColInfo* cip = col_list[i];
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
  if ( true ) {
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

  return col_list.size();
}

// @brief 故障リストを返す．
// @param[in] col 色番号(1が最初)
const vector<TpgFault*>&
GcSolver2::fault_list(ymuint col) const
{
  ASSERT_COND( col <= mColList.size() );
  return mColList[col - 1]->mFaultList;
}

// @brief 十分割当リストを返す．
// @param[in] col 色番号(1が最初)
const NodeValList&
GcSolver2::suf_list(ymuint col) const
{
  ASSERT_COND( col <= mColList.size() );
  return mColList[col - 1]->mSufList;
}

// @brief 新しい色を割り当てる．
void
GcSolver2::new_color(TpgFault* fault,
		     vector<ColInfo*>& col_list)
{
  TpgCnf1 tpg_cnf(string(), string(), NULL);

  tpg_cnf.make_fval_cnf(fault, mMaxId);

  ColInfo* cip = new ColInfo;
  cip->mColId = col_list.size();
  cip->mFaultList.push_back(fault);
  NodeValList suf_list;
  bool stat = tpg_cnf.get_suf_list(NodeValList(), suf_list);
  ASSERT_COND( stat );
  cip->mFaultSufList.push_back(suf_list);
  cip->mSufList = suf_list;
  col_list.push_back(cip);
}

END_NAMESPACE_YM_SATPG
