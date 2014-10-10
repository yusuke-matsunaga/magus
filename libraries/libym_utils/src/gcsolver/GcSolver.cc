
/// @file GcSolver.cc
/// @brief GcSolver の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "YmUtils/GcSolver.h"
#include "GcNode.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// GcNode のソート用の比較ファンクタ
struct Lt
{
  bool
  operator()(GcNode* node1,
	     GcNode* node2)
  {
    if ( node1->adj_degree() > node2->adj_degree() ) {
      return true;
    }
    return false;
  }

};

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス GcSolver
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GcSolver::GcSolver()
{
  mNodeNum = 0;
  mNodeArray = NULL;
  mNodeHeap = NULL;
}

// @brief デストラクタ
GcSolver::~GcSolver()
{
  init(0);
}

// @brief ノード数を指定して初期化する．
void
GcSolver::init(ymuint num)
{
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    delete [] mNodeArray[i].mColorSet;
  }
  delete [] mNodeArray;
  delete [] mNodeHeap;

  mNodeNum = num;
  if ( mNodeNum > 0 ) {
    ymuint vectlen = (mNodeNum + 63) / 64;
    mNodeArray = new GcNode[mNodeNum];
    for (ymuint i = 0; i < mNodeNum; ++ i) {
      GcNode* node = &mNodeArray[i];
      node->mId = i;
      node->mColorSet = new ymuint64[vectlen];
      for (ymuint j = 0; j < vectlen; ++ j) {
	node->mColorSet[j] = 0UL;
      }
    }
    mNodeHeap = new GcNode*[mNodeNum];
  }
  else {
    mNodeArray = NULL;
  }
}

// @brief ノード数を得る．
ymuint
GcSolver::node_num() const
{
  return mNodeNum;
}

// @brief 2つのノードを接続する．
// @param[in] id1, id2 2つのノードの番号 ( 0 <= id1, id2 < node_num() )
void
GcSolver::connect(ymuint id1,
		  ymuint id2)
{
  ASSERT_COND( id1 < node_num() );
  ASSERT_COND( id2 < node_num() );

  GcNode* node1 = &mNodeArray[id1];
  GcNode* node2 = &mNodeArray[id2];

  node1->mLinkList.push_back(node2);
  node2->mLinkList.push_back(node1);
}

// @brief 彩色する．
// @param[in] color_group 同じ色のノード番号のリストの配列
// @return 彩色数を返す．
ymuint
GcSolver::coloring(vector<vector<ymuint> >& color_group)
{
  // dsatur アルゴリズムを用いる．

  mHeapNum = 0;

  // mNodeArray を隣接ノード数の降順にソートする．
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    push_node(&mNodeArray[i]);
  }

  mMaxColor = 1;

  // 1: 隣接するノード数が最大のノードを選び彩色する．
  //    ソートしているので先頭のノードを選べば良い．
  GcNode* max_node = pop_node();
  color_node(max_node, mMaxColor);

  // 2: saturation degree が最大の未彩色ノードを選び最小の色番号で彩色する．
  while ( mHeapNum > 0 ) {
    GcNode* max_node = pop_node();
    // max_node につけることのできる最小の色番号を求める．
    clear_count();
    ymuint cnum = 0;
    const vector<GcNode*>& node_list = max_node->mLinkList;
    for (vector<GcNode*>::const_iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      GcNode* node1 = *p;
      ymuint c = node1->color();
      if ( c > 0 ) {
	if ( !mCountArray[c - 1] ) {
	  mCountArray[c - 1] = true;
	  ++ cnum;
	  if ( cnum == mMaxColor ) {
	    break;
	  }
	}
      }
    }
    ymuint min_col = 0;
    for (ymuint c = 0; c < mMaxColor; ++ c) {
      if ( !mCountArray[c] ) {
	min_col = c + 1;
	break;
      }
    }
    if ( min_col == 0 ) {
      ++ mMaxColor;
      min_col = mMaxColor;
    }
    color_node(max_node, min_col);
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
    ymuint c = node->color();

    ASSERT_COND( c > 0 );
    ASSERT_COND( c <= mMaxColor );

    color_group[c - 1].push_back(i);
  }

  return mMaxColor;
}

// @brief グラフの内容を出力する．
void
GcSolver::dump(ostream& s) const
{
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    GcNode* node = &mNodeArray[i];
    const vector<GcNode*>& node_list = node->link_list();
    cout << i << ": ";
    for (vector<GcNode*>::const_iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      GcNode* node1 = *p;
      cout << " " << node1->id();
    }
    cout << endl;
  }
  cout << endl;
}

// @brief ノードに彩色して情報を更新する．
void
GcSolver::color_node(GcNode* node,
		     ymuint col)
{
  node->mColor = col;

  const vector<GcNode*>& node_list = node->link_list();
  for (vector<GcNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    GcNode* node1 = *p;
    if ( node1->mColor == 0 ) {
      update_sat_degree(node1, col);
    }
  }
}

// @brief saturation degree を再計算する．
void
GcSolver::update_sat_degree(GcNode* node,
			    ymuint color)
{
  if ( !node->check_color(color) ) {
    node->set_color(color);
    ++ node->mSatDegree;

    // SAT degree が変わったのでヒープ上の位置も更新する．
    ymuint pos = node->mHeapPos;
    while ( pos > 0 ) {
      GcNode* node = mNodeHeap[pos];
      ymuint parent = (pos - 1) >> 1;
      GcNode* node_p = mNodeHeap[parent];
      if ( compare(node_p, node) > 0 ) {
	set_node(parent, node);
	set_node(pos, node_p);
	pos = parent;
      }
      else {
	pos = 0;
      }
    }
  }
}

// @brief mCountArray をクリアする．
void
GcSolver::clear_count()
{
  mCountArray.clear();
  mCountArray.resize(mMaxColor, false);
}

// @brief ヒープにノードを追加する．
void
GcSolver::push_node(GcNode* node)
{
  set_node(mHeapNum, node);
  ++ mHeapNum;

  ymuint pos = mHeapNum - 1;
  while ( pos > 0 ) {
    GcNode* node = mNodeHeap[pos];
    ymuint parent = (pos - 1) >> 1;
    GcNode* node_p = mNodeHeap[parent];
    if ( compare(node_p, node) > 0 ) {
      set_node(parent, node);
      set_node(pos, node_p);
      pos = parent;
    }
    else {
      pos = 0;
    }
  }
}

// @brief ヒープからノードを取り出す．
GcNode*
GcSolver::pop_node()
{
  ASSERT_COND( mHeapNum > 0 );

  GcNode* node = mNodeHeap[0];

  -- mHeapNum;
  GcNode* last = mNodeHeap[mHeapNum];
  set_node(0, last);

  ymuint parent = 0;
  for ( ; ; ) {
    ymuint left = parent + parent + 1;
    ymuint right = left + 1;
    if ( right > mHeapNum ) {
      // 左右の子供を持たない場合．
      break;
    }
    GcNode* node_p = mNodeHeap[parent];
    GcNode* node_l = mNodeHeap[left];
    if ( right == mHeapNum ) {
      // 右の子供を持たない場合
      if ( compare(node_p, node_l) > 0 ) {
	// 左の子供と取り替える．
	set_node(parent, node_l);
	set_node(left, node_p);
      }
      break;
    }
    else {
      GcNode* node_r = mNodeHeap[right];
      if ( compare(node_p, node_l) > 0 &&
	   compare(node_l, node_r) <= 0 ) {
	// 左の子供と取り替える．
	// 次は左の子で同じ処理をする．
	set_node(parent, node_l);
	set_node(left, node_p);
	parent = left;
      }
      else if ( compare(node_p, node_r) > 0 &&
		compare(node_r, node_l) < 0 ) {
	// 右の子供と取り替える．
	// 次は右の子で同じ処理をする．
	set_node(parent, node_r);
	set_node(right, node_p);
	parent = right;
      }
      else {
	break;
      }
    }
  }

  return node;
}

// @brief ヒープ中の配列にノードをセットする．
void
GcSolver::set_node(ymuint pos,
		GcNode* node)
{
  mNodeHeap[pos] = node;
  node->mHeapPos = pos;
}

// @brief ノードの比較関数
int
GcSolver::compare(GcNode* node1,
	       GcNode* node2)
{
  if ( node1->sat_degree() < node2->sat_degree() ) {
    return 1;
  }
  if ( node1->sat_degree() > node2->sat_degree() ) {
    return -1;
  }
  if ( node1->adj_degree() < node2->adj_degree() ) {
    return 1;
  }
  if ( node1->adj_degree() > node2->adj_degree() ) {
    return -1;
  }
  return 0;
}


//////////////////////////////////////////////////////////////////////
// クラス GcNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GcNode::GcNode()
{
  mSatDegree = 0;
  mColor = 0;
}

// @brief デストラクタ
GcNode::~GcNode()
{
}

END_NAMESPACE_YM
