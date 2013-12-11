
/// @file BtgMatch.cc
/// @brief BtgMatch の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/BtgMatch.h"
#include "BtgNode.h"
#include "BtgEdge.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス BtgMatch
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
BtgMatch::BtgMatch()
{
  mNode1Num = 0;
  mNode1Array = NULL;
  mNode2Num = 0;
  mNode2Array = NULL;
}

// @brief コンストラクタ
// @param[in] node1_num 節点グループ1 の要素数
// @param[in] node2_num 節点グループ2 の要素数
BtgMatch::BtgMatch(ymuint node1_num,
		   ymuint node2_num)
{
  mNode1Num = node1_num;
  mNode1Array = new BtgNode[mNode1Num];
  for (ymuint i = 0; i < mNode1Num; ++ i) {
    mNode1Array[i].set(0, i);
  }

  mNode2Num = node2_num;
  mNode2Array = new BtgNode[mNode2Num];
  for (ymuint i = 0; i < mNode2Num; ++ i) {
    mNode2Array[i].set(1, i);
  }
}

// @brief デストラクタ
BtgMatch::~BtgMatch()
{
  clear_edge();

  delete [] mNode1Array;
  delete [] mNode2Array;
}

// @brief 節点数を設定する．
// @param[in] node1_num 節点グループ1の要素数
// @param[in] node2_num 節点グループ2の要素数
// @note 以前の内容はクリアされる．
void
BtgMatch::resize(ymuint node_num1,
		 ymuint node_num2)
{
  clear_edge();

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

// @brief 節点グループ1 の要素数を返す．
ymuint
BtgMatch::node1_num() const
{
  return mNode1Num;
}

// @brief 節点グループ2 の要素数を返す．
ymuint
BtgMatch::node2_num() const
{
  return mNode2Num;
}

// @brief 枝をすべて取り除く
void
BtgMatch::clear_edge()
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
BtgMatch::add_edge(ymuint node1_id,
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

// @brief 枝数を返す．
ymuint
BtgMatch::edge_num() const
{
  return mEdgeList.size();
}

// @brief 枝の情報を返す．
// @param[in] pos 位置番号 ( 0 <= pos < edge_num() )
// @param[out] node1_id 節点グループ1のノード番号
// @param[out] node2_id 節点グループ2のノード番号
// @param[out] weight 枝の重み
void
BtgMatch::edge_info(ymuint pos,
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
BtgMatch::calc_match(vector<ymuint>& edge_list)
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

  if ( uniform ) {
    // 全ての同じ重みだった
    //  -> 最大マッチングを求める．

    // 節点数の少ない方から探した方が速い．
    if ( mNode1Num <= mNode2Num ) {
      calc_match1(edge_list);
    }
    else {
      calc_match2(edge_list);
    }
  }
  else {
    // 重み最大マッチングを求める．

    // 節点数の少ない方から探した方が速い．
    if ( mNode1Num <= mNode2Num ) {
      calc_wmatch1(edge_list);
    }
    else {
      calc_wmatch2(edge_list);
    }
  }

  ymuint weight_sum = 0;
  for (vector<ymuint>::iterator p = edge_list.begin();
       p != edge_list.end(); ++ p) {
    BtgEdge* edge = mEdgeArray[*p];
    weight_sum += edge->weight();
  }

  return weight_sum;
}

// @brief 節点グループ1でまわす calc_match()
// @param[in] edge_list マッチング結果の枝を格納するリスト
void
BtgMatch::calc_match1(vector<ymuint>& edge_list)
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
	    edge_list.push_back(edge->id());
	    if ( old_edge == NULL ) {
	      break;
	    }
	    v2 = old_edge->node2();
	    edge = parent[v2->id()];
	    v1 = edge->node1();
	  }
	  break;
	}
	else {
	  // すでにマッチしていた．
	  // マッチ先のノードから探索する．
	  BtgNode* v3 = cur_edge->node1();
	  if ( !qmark[v3->id()] ) {
	    qmark[v3->id()] = true;
	    queue.push_back(v3);
	    parent[v2->id()] = edge;
	  }
	}
      }
    }
  }
}

// @brief 節点グループ2でまわす calc_match()
// @param[in] edge_list マッチング結果の枝を格納するリスト
void
BtgMatch::calc_match2(vector<ymuint>& edge_list)
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
	    edge_list.push_back(edge->id());
	    if ( old_edge == NULL ) {
	      break;
	    }
	    v1 = old_edge->node1();
	    edge = parent[v1->id()];
	    v2 = edge->node2();
	  }
	  break;
	}
	else {
	  // すでにマッチしていた．
	  // マッチ先のノードから探索する．
	  BtgNode* v3 = cur_edge->node2();
	  if ( !qmark[v3->id()] ) {
	    qmark[v3->id()] = true;
	    queue.push_back(v3);
	    parent[v1->id()] = edge;
	  }
	}
      }
    }
  }
}

// @brief 節点グループ1でまわす calc_wmatch()
// @param[in] edge_list マッチング結果の枝を格納するリスト
void
BtgMatch::calc_wmatch1(vector<ymuint>& edge_list)
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
	    edge_list.push_back(edge->id());
	    if ( old_edge == NULL ) {
	      break;
	    }
	    v2 = old_edge->node2();
	    edge = parent[v2->id()];
	    v1 = edge->node1();
	  }
	  break;
	}
	else {
	  // すでにマッチしていた．
	  // マッチ先のノードから探索する．
	  BtgNode* v3 = cur_edge->node1();
	  if ( !qmark[v3->id()] ) {
	    qmark[v3->id()] = true;
	    queue.push_back(v3);
	    parent[v2->id()] = edge;
	  }
	}
      }
    }
  }
}

END_NAMESPACE_YM
