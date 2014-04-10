
/// @file BtgMatch.cc
/// @brief BtgMatch の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "BtgMatchImpl.h"
#include "BtgNode.h"
#include "BtgEdge.h"
#include "BtgHeapTree.h"


BEGIN_NAMESPACE_YM_BTG

//////////////////////////////////////////////////////////////////////
// クラス BtgMatchImpl
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
BtgMatchImpl::BtgMatchImpl()
{
  mNode1Num = 0;
  mNode1Array = NULL;
  mNode2Num = 0;
  mNode2Array = NULL;
}

// @brief デストラクタ
BtgMatchImpl::~BtgMatchImpl()
{
  clear();

  delete [] mNode1Array;
  delete [] mNode2Array;
}

// @brief 節点数を設定する．
// @param[in] node1_num 節点グループ1の要素数
// @param[in] node2_num 節点グループ2の要素数
// @note 以前の内容はクリアされる．
void
BtgMatchImpl::set_size(ymuint node_num1,
		       ymuint node_num2)
{
  clear();

  if ( mNode1Num != node_num1 ) {
    delete [] mNode1Array;
    mNode1Num = node_num1;
    mNode1Array = new BtgNode[mNode1Num];
  }
  for (ymuint i = 0; i < mNode1Num; ++ i) {
    mNode1Array[i].set(0, i);
  }

  if ( mNode2Num != node_num2 ) {
    delete [] mNode2Array;
    mNode2Num = node_num2;
    mNode2Array = new BtgNode[mNode2Num];
  }
  for (ymuint i = 0; i < mNode2Num; ++ i) {
    mNode2Array[i].set(1, i);
  }
}

// @brief 枝をすべて取り除く
void
BtgMatchImpl::clear()
{
  for (ymuint i = 0; i < mNode1Num; ++ i) {
    mNode1Array[i].mEdgeTop = NULL;
  }
  for (ymuint i = 0; i < mNode2Num; ++ i) {
    mNode2Array[i].mEdgeTop = NULL;
  }
  for (vector<BtgEdge*>::iterator p = mEdgeList.begin();
       p != mEdgeList.end(); ++ p) {
    delete *p;
  }
  mEdgeList.clear();
}

// @brief 枝を追加する．
// @param[in] node1_id 節点グループ1のノード番号 ( 0 <= node1_id < node1_num() )
// @param[in] node2_id 節点グループ2のノード番号 ( 0 <= node2_id < node2_num() )
// @param[in] weight 枝の重み (省略されたらデフォルトは1)
// @note 既に同じ枝があったら上書きされる．
void
BtgMatchImpl::add_edge(ymuint node1_id,
		       ymuint node2_id,
		       ymuint weight)
{
  assert_cond( node1_id < node1_num(), __FILE__, __LINE__);
  assert_cond( node2_id < node2_num(), __FILE__, __LINE__);
  BtgNode* node1 = &mNode1Array[node1_id];
  BtgNode* node2 = &mNode2Array[node2_id];
  BtgEdge* edge = new BtgEdge(mEdgeList.size(), node1, node2, weight);
  mEdgeList.push_back(edge);

  edge->mLink1 = node1->mEdgeTop;
  node1->mEdgeTop = edge;

  edge->mLink2 = node2->mEdgeTop;
  node2->mEdgeTop = edge;
}

// @brief 増加パスを見つける．
bool
find_alt_path(BtgNode* v1,
	      vector<bool>& mark)
{
  if ( mark[v1->id()] ) {
    return false;
  }
  mark[v1->id()] = true;
  for (BtgEdge* edge = v1->edge_top(); edge; edge = edge->link1()) {
    BtgNode* v2 = edge->node2();
    BtgEdge* edge1 = v2->cur_edge();
    if ( edge1 == NULL ) {
      // 見つけた．
      v1->set_cur_edge(edge);
      v2->set_cur_edge(edge);
      return true;
    }
    if ( find_alt_path(edge1->node1(), mark) ) {
      // 見つけた．
      v1->set_cur_edge(edge);
      v2->set_cur_edge(edge);
      return true;
    }
  }
  return false;
}

// @brief 枝の情報を返す．
// @param[in] pos 位置番号 ( 0 <= pos < edge_num() )
// @param[out] node1_id 節点グループ1のノード番号
// @param[out] node2_id 節点グループ2のノード番号
// @param[out] weight 枝の重み
void
BtgMatchImpl::edge_info(ymuint pos,
			ymuint& node1_id,
			ymuint& node2_id,
			ymuint& weight) const
{
  assert_cond( pos < edge_num(), __FILE__, __LINE__);
  BtgEdge* edge = mEdgeList[pos];
  node1_id = edge->node1()->id();
  node2_id = edge->node2()->id();
  weight = edge->weight();
}

// @brief 最大マッチングを求める．
// @param[in] edge_list マッチング結果の枝を格納するリスト
// @return マッチング結果の重みの総和を返す．
ymuint
BtgMatchImpl::calc_match(vector<ymuint>& edge_list)
{
  ymuint ne = mEdgeList.size();
  if ( ne == 0 ) {
    return 0;
  }

  ymuint w0 = mEdgeList[0]->weight();
  bool uniform = true;
  for (ymuint i = 1; i < ne; ++ i) {
    if ( mEdgeList[i]->weight() != w0 ) {
      uniform = false;
      break;
    }
  }

  ymuint weight_sum = 0;

  // 節点数の少ない方から探した方が速い．
  if ( mNode1Num <= mNode2Num ) {
    if ( uniform ) {
      // 全ての同じ重みだった
      //  -> 最大マッチングを求める．
      calc_match1();
    }
    else {
      //  -> 重み最大マッチングを求める．
      calc_wmatch1();
    }

    for (ymuint i = 0; i < mNode1Num; ++ i) {
      BtgNode* v1 = &mNode1Array[i];
      BtgEdge* edge = v1->cur_edge();
      if ( edge == NULL ) {
	return 0;
      }
      assert_cond( edge->node2()->cur_edge() == edge, __FILE__, __LINE__);
      edge_list.push_back(edge->id());
      weight_sum += edge->weight();
    }
  }
  else {
    if ( uniform ) {
      // 全ての同じ重みだった
      //  -> 最大マッチングを求める．
      calc_match2();
    }
    else {
      //  -> 重み最大マッチングを求める．
      calc_wmatch2();
    }

    for (ymuint i = 0; i < mNode2Num; ++ i) {
      BtgNode* v2 = &mNode2Array[i];
      BtgEdge* edge = v2->cur_edge();
      if ( edge == NULL ) {
	return 0;
      }
      assert_cond( edge->node1()->cur_edge() == edge, __FILE__, __LINE__);
      edge_list.push_back(edge->id());
      weight_sum += edge->weight();
    }
  }

  return weight_sum;
}

// @brief 節点グループ1でまわす calc_match()
void
BtgMatchImpl::calc_match1()
{
  for (ymuint i = 0; i < mNode1Num; ++ i) {
    BtgNode* v1 = &mNode1Array[i];

    // 次に処理すべき節点グループを保持するキュー
    vector<bool> qmark(mNode1Num);
    vector<BtgNode*> queue;
    queue.reserve(mNode1Num);

    // たどる元の枝を保持する配列
    vector<BtgEdge*> parent(mNode2Num, NULL);

    // v1 をキューに積む．
    queue.push_back(v1);
    qmark[v1->id()] = true;

    ymuint rpos = 0;
    while ( rpos < queue.size() ) {
      // キューから節点を取り出す．
      BtgNode* v1 = queue[rpos];
      ++ rpos;

      // v1 に隣接している節点でマッチしていないものを探す．
      bool found = false;
      for (BtgEdge* edge = v1->edge_top();
	   edge != NULL; edge = edge->link1()) {
	BtgNode* v2 = edge->node2();
	BtgEdge* cur_edge = v2->cur_edge();
	if ( cur_edge == NULL ) {
	  // 見つけた．
	  // 後ろ向きにたどる．
	  for ( ; ; ) {
	    BtgEdge* old_edge = v1->cur_edge();
	    v1->set_cur_edge(edge);
	    v2->set_cur_edge(edge);
	    if ( old_edge == NULL ) {
	      break;
	    }
	    v2 = old_edge->node2();
	    edge = parent[v2->id()];
	    v1 = edge->node1();
	  }
	  found = true;
	  break;
	}
      }
      if ( !found ) {
	// すでにマッチしていた．
	// マッチ先のノードから探索する．
	for (BtgEdge* edge = v1->edge_top(); edge; edge = edge->link1()) {
	  BtgNode* v2 = edge->node2();
	  BtgEdge* cur_edge = v2->cur_edge();
	  assert_cond( cur_edge != NULL, __FILE__, __LINE__);
	  BtgNode* v3 = cur_edge->node1();
	  if ( !qmark[v3->id()] ) {
	    qmark[v3->id()] = true;
	    queue.push_back(v3);
	    parent[v2->id()] = edge;
	    found = true;
	    break;
	  }
	}
	if ( !found ) {
	  return;
	}
      }
    }
  }
}

// @brief 節点グループ2でまわす calc_match()
void
BtgMatchImpl::calc_match2()
{
  for (ymuint i = 0; i < mNode2Num; ++ i) {
    BtgNode* v2 = &mNode2Array[i];

    // 次に処理すべき節点グループを保持するキュー
    vector<bool> qmark(mNode2Num);
    vector<BtgNode*> queue;
    queue.reserve(mNode2Num);

    // たどる元の枝を保持する配列
    vector<BtgEdge*> parent(mNode1Num, NULL);

    // v1 をキューに積む．
    queue.push_back(v2);
    qmark[v2->id()] = true;

    ymuint rpos = 0;
    while ( rpos < queue.size() ) {
      // キューから節点を取り出す．
      BtgNode* v2 = queue[rpos];
      ++ rpos;

      // v2 に隣接している節点でマッチしていないものを探す．
      bool found = false;
      for (BtgEdge* edge = v2->edge_top();
	   edge != NULL; edge = edge->link2()) {
	BtgNode* v1 = edge->node1();
	BtgEdge* cur_edge = v1->cur_edge();
	if ( cur_edge == NULL ) {
	  // 見つけた．
	  // 後ろ向きにたどる．
	  for ( ; ; ) {
	    BtgEdge* old_edge = v2->cur_edge();
	    v1->set_cur_edge(edge);
	    v2->set_cur_edge(edge);
	    if ( old_edge == NULL ) {
	      break;
	    }
	    v1 = old_edge->node1();
	    edge = parent[v1->id()];
	    v2 = edge->node2();
	  }
	  found = true;
	  break;
	}
      }
      if ( !found ) {
	// すでにマッチしていた．
	// マッチ先のノードから探索する．
	for (BtgEdge* edge = v2->edge_top(); edge; edge = edge->link2()) {
	  BtgNode* v1 = edge->node1();
	  BtgEdge* cur_edge = v1->cur_edge();
	  assert_cond( cur_edge != NULL, __FILE__, __LINE__);
	  BtgNode* v3 = cur_edge->node2();
	  if ( !qmark[v3->id()] ) {
	    qmark[v3->id()] = true;
	    queue.push_back(v3);
	    parent[v1->id()] = edge;
	    found = true;
	  }
	}
	if ( !found ) {
	  return;
	}
      }
    }
  }
}

// @brief 節点グループ1でまわす calc_wmatch()
void
BtgMatchImpl::calc_wmatch1()
{
  // 次に処理すべき節点グループを保持するキュー
  BtgHeapTree queue(mNode1Num);

  for ( ; ; ) {
    ymint max_weight = 0;
    BtgNode* max_v2 = NULL;

    queue.clear();
    for (ymuint j = 0; j < mNode1Num; ++ j) {
      mNode1Array[j].mPos = 0;
    }

    // たどる元の枝を保持する配列
    vector<BtgEdge*> parent(mNode2Num, NULL);

    for (ymuint i = 0; i < mNode1Num; ++ i) {
      BtgNode* v1 = &mNode1Array[i];
      if ( v1->cur_edge() == NULL ) {
	// v1 をキューに積む．
	v1->mWeight = 0;
	queue.put(v1);
      }
    }

    while ( !queue.empty() ) {
      // キューから節点を取り出す．
      BtgNode* v1 = queue.get();

      // v1 に隣接している節点でマッチしていないものを探す．
      for (BtgEdge* edge = v1->edge_top();
	   edge != NULL; edge = edge->link1()) {
	BtgNode* v2 = edge->node2();
	v2->mWeight = v1->weight() + edge->weight();
	BtgEdge* cur_edge = v2->cur_edge();
	if ( cur_edge == NULL ) {
	  // 見つけた．
	  if ( max_weight < v2->weight() ) {
	    max_weight = v2->weight();
	    max_v2 = v2;
	    parent[v2->id()] = edge;
	  }
	}
	else {
	  // すでにマッチしていた．
	  // マッチ先のノードから探索する．
	  BtgNode* v3 = cur_edge->node1();
	  ymint32 w_new = v2->weight() - cur_edge->weight();
	  if ( v3->mPos > 0 ) {
	    if ( w_new > v3->weight() ) {
	      v3->mWeight = w_new;
	      queue.update(v3);
	    }
	  }
	  else {
	    v3->mWeight = w_new;
	    queue.put(v3);
	    parent[v2->id()] = edge;
	  }
	}
      }
    }

    if ( max_weight == 0 ) {
      // 重みを増加させる経路がなくなったら終わる．
      break;
    }

    // 後ろ向きにたどる．
    for (BtgNode* v2 = max_v2; ; ) {
      BtgEdge* edge = parent[v2->id()];
      BtgNode* v1 = edge->node1();
      BtgEdge* old_edge = v1->cur_edge();
      v1->set_cur_edge(edge);
      v2->set_cur_edge(edge);
      if ( old_edge == NULL ) {
	break;
      }
      v2 = old_edge->node2();
    }
  }
}

// @brief 節点グループ2でまわす calc_wmatch()
void
BtgMatchImpl::calc_wmatch2()
{
  // 次に処理すべき節点グループを保持するキュー
  BtgHeapTree queue(mNode2Num);

  for ( ; ; ) {
    ymint max_weight = 0;
    BtgNode* max_v1 = NULL;

    queue.clear();
    for (ymuint j = 0; j < mNode2Num; ++ j) {
      mNode2Array[j].mPos = 0;
    }

    // たどる元の枝を保持する配列
    vector<BtgEdge*> parent(mNode1Num, NULL);

    for (ymuint i = 0; i < mNode2Num; ++ i) {
      BtgNode* v2 = &mNode2Array[i];

      if ( v2->cur_edge() == NULL ) {
	// v2 をキューに積む．
	v2->mWeight = 0;
	queue.put(v2);
      }
    }

    while ( !queue.empty() ) {
      // キューから節点を取り出す．
      BtgNode* v2 = queue.get();

      // v2 に隣接している節点でマッチしていないものを探す．
      for (BtgEdge* edge = v2->edge_top();
	   edge != NULL; edge = edge->link2()) {
	BtgNode* v1 = edge->node1();
	v1->mWeight = v2->weight() + edge->weight();
	BtgEdge* cur_edge = v1->cur_edge();
	if ( cur_edge == NULL ) {
	  // 見つけた．
	  if ( max_weight < v1->weight() ) {
	    max_weight = v1->weight();
	    max_v1 = v1;
	    parent[v1->id()] = edge;
	  }
	}
	else {
	  // すでにマッチしていた．
	  // マッチ先のノードから探索する．
	  BtgNode* v3 = cur_edge->node2();
	  ymint32 w_new = v1->weight() - cur_edge->weight();
	  if ( v3->mPos > 0 ) {
	    if ( w_new > v3->weight() ) {
	      v3->mWeight = w_new;
	      queue.update(v3);
	    }
	  }
	  else {
	    v3->mWeight = w_new;
	    queue.put(v3);
	    parent[v1->id()] = edge;
	  }
	}
      }
    }

    if ( max_weight == 0 ) {
      // 重みを増加させる経路がなくなったら終わる．
      break;
    }

    // 後ろ向きにたどる．
    for (BtgNode* v1 = max_v1; ; ) {
      BtgEdge* edge = parent[v1->id()];
      BtgNode* v2 = edge->node2();
      BtgEdge* old_edge = v2->cur_edge();
      v2->set_cur_edge(edge);
      v1->set_cur_edge(edge);
      if ( old_edge == NULL ) {
	break;
      }
      v1 = old_edge->node1();
    }
  }
}

END_NAMESPACE_YM_BTG
