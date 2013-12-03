
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
  delete [] mNode1Array;
  delete [] mNode2Array;

  for (vector<BtgEdge*>::iterator p = mEdgeList.begin();
       p != mEdgeList.end(); ++ p) {
    delete *p;
  }
}

// @brief 節点グループ1 の要素数を返す．
ymuint
BtgMatch::node1_num()
{
  return mNode1Num;
}

// @brief 節点グループ2 の要素数を返す．
ymuint
BtgMatch::node2_num()
{
  return mNode2Num;
}

// @brief 枝をすべて取り除く
void
BtgMatch::clear_edge()
{
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
  BtgEdge* edge = new BtgEdge(node1_id, node2_id, weight);
  mEdgeList.push_back(edge);
  mNode1Array[node1_id].mEdgeList.push_back(edge);
  mNode2Array[node2_id].mEdgeList.push_back(edge);
}

// @brief 枝数を返す．
ymuint
BtgMatch::edge_num()
{
  return mEdgeList.size();
}

// @brief 枝の情報を返す．
// @param[in] pos 位置番号 ( 0 <= pos < edge_num() )
// @param[out] node1_id 節点グループ1のノード番号
// @param[out] node2_id 節点グループ2のノード番号
// @param[out] weight 枝の重み
void
BtgMatch::nedge_info(ymuint pos,
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


BEGIN_NONAMESPACE

// @brief 増加パスを見つける．
bool
find_alt_path(BtgNode* v1,
	      vector<bool>& mark)
{
  if ( mark[v1->id()] ) {
    return false;
  }

  mark[v1->id()] = true;

  ymuint ne = v1->edge_num();
  for (ymuint i = 0; i < ne; ++ i) {
    BtgEdge* edge = v1->edge(i);
    BtgNode* v2 = edge->v2();
    BtgEdge* edge1 = v2->cur_edge();
    if ( edge1 == NULL ) {
      // 見つけた．
      v1->set_cur_edge(edge);
      v2->set_cur_edge(edge);
      return true;
    }

    if ( find_alt_path(edge1->v1(), mark) ) {
      // 見つけた．
      v1->mCurEdge = edge;
      v2->mCurEdge = edge;
      return true;
    }
  }

  return false;
}

END_NONAMESPACE

// @brief 最大マッチングを求める．
// @param[in] edge_list マッチング結果の枝を格納するリスト
// @return マッチング結果の重みの総和を返す．
ymuint
BtgMatch::calc_match(vector<ymuint>& edge_list)
{
  for (ymuint i = 0; i < mNode1Num; ++ i) {
    BtgNode* v1 = &mNode1Array[i];
    ymuint ne = v1->edge_num();
    bool found = false;
    // 空いているノードを探す．
    for (ymuint j = 0; j < ne; ++ j) {
      BtgEdge* edge = v1->edge(j);
      BtgNode* v2 = edge->v2();
      BtgEdge* edge1 = v2->cur_edge();
      if ( edge1 == NULL ) {
	v1->set_cur_edge(edge);
	v2->set_cur_edge(edge);
	found = true;
	break;
      }
    }
    if ( !found ) {
      // なかったので既に割り当てられている枝を取り替える．
      vector<bool> mark(mNode1Num, false);
      find_alt_path(v1, mark);
    }
  }

  for (ymuint i = 0; i < mNode1Num; ++ i) {
    BtgNode* v1 = &mNode1Array[i];
    BtgEdge* edge = v1->cur_edge();
    edge_list.push_back(edge);
  }

}

END_NAMESPACE_YM_IGF
