
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
  if ( mNode1Num <= mNode2Num ) {
    return calc_match1(edge_list);
  }
  else {
    return calc_match2(edge_list);
  }
}


BEGIN_NONAMESPACE

// @brief 増加パスを見つける．(calc_match1()用)
bool
find_alt_path1(BtgNode* v2,
	       vector<bool>& mark)
{
  BtgEdge* edge = v2->cur_edge();
  if ( edge == NULL ) {
    return true;
  }

  BtgNode* v1 = edge->node1();

  if ( mark[v1->id()] ) {
    return false;
  }

  mark[v1->id()] = true;

  for (BtgEdge* edge1 = v1->edge_top();
       edge1 != NULL; edge1 = edge1->link1()) {
    BtgNode* v2 = edge1->node2();
    if ( find_alt_path1(v2, mark) ) {
      // 見つけた．
      v1->set_cur_edge(edge);
      v2->set_cur_edge(edge);
      return true;
    }
  }

  return false;
}

END_NONAMESPACE

// @brief 節点グループ1でまわす calc_match()
// @param[in] edge_list マッチング結果の枝を格納するリスト
// @return マッチング結果の重みの総和を返す．
ymuint
BtgMatch::calc_match1(vector<ymuint>& edge_list)
{
  for (ymuint i = 0; i < mNode1Num; ++ i) {
    BtgNode* v1 = &mNode1Array[i];

    // 空いているノードを探す．
    for (BtgEdge* edge = v1->edge_top();
	 edge != NULL; edge = edge->link1()) {
      BtgNode* v2 = edge->node2();
      vector<bool> mark(mNode1Num, false);
      if ( find_alt_path1(v2, mark) ) {
	v1->set_cur_edge(edge);
	v2->set_cur_edge(edge);
	break;
      }
    }
  }

  ymuint weight_sum = 0;
  for (ymuint i = 0; i < mNode1Num; ++ i) {
    BtgNode* v1 = &mNode1Array[i];
    BtgEdge* edge = v1->cur_edge();
    if ( edge != NULL ) {
      edge_list.push_back(edge->id());
      weight_sum += edge->weight();
    }
  }

  return weight_sum;
}


BEGIN_NONAMESPACE

// @brief 増加パスを見つける．(calc_match2()用)
bool
find_alt_path2(BtgNode* v1,
	       vector<bool>& mark)
{
  BtgEdge* edge = v1->cur_edge();
  if ( edge == NULL ) {
    return true;
  }

  BtgNode* v2 = edge->node2();

  if ( mark[v2->id()] ) {
    return false;
  }

  mark[v2->id()] = true;

  for (BtgEdge* edge1 = v2->edge_top();
       edge1 != NULL; edge1 = edge1->link2()) {
    BtgNode* v1 = edge1->node1();
    if ( find_alt_path2(v1, mark) ) {
      // 見つけた．
      v1->set_cur_edge(edge);
      v2->set_cur_edge(edge);
      return true;
    }
  }

  return false;
}

END_NONAMESPACE

// @brief 節点グループ2でまわす calc_match()
// @param[in] edge_list マッチング結果の枝を格納するリスト
// @return マッチング結果の重みの総和を返す．
ymuint
BtgMatch::calc_match2(vector<ymuint>& edge_list)
{
  for (ymuint i = 0; i < mNode2Num; ++ i) {
    BtgNode* v2 = &mNode2Array[i];

    // 空いているノードを探す．
    for (BtgEdge* edge = v2->edge_top();
	 edge != NULL; edge = edge->link2()) {
      BtgNode* v1 = edge->node1();
      vector<bool> mark(mNode2Num, false);
      if ( find_alt_path2(v1, mark) ) {
	v1->set_cur_edge(edge);
	v2->set_cur_edge(edge);
	break;
      }
    }
  }

  ymuint weight_sum = 0;
  for (ymuint i = 0; i < mNode2Num; ++ i) {
    BtgNode* v2 = &mNode2Array[i];
    BtgEdge* edge = v2->cur_edge();
    if ( edge != NULL ) {
      edge_list.push_back(edge->id());
      weight_sum += edge->weight();
    }
  }

  return weight_sum;
}

END_NAMESPACE_YM
