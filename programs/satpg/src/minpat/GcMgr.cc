
/// @file GcMgr.cc
/// @brief GcMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "GcMgr.h"
#include "GcNode.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

// GcNode のソート用の比較ファンクタ
struct Lt
{
  bool
  operator()(GcNode* node1,
	     GcNode* node2)
  {
    if ( node1->adj_dgree() > node2->adj_dgree() ) {
      return true;
    }
    return false;
  }

};

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス GcMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GcMgr::GcMgr()
{
  mNodeNum = 0;
  mNodeArray = NULL;
}

// @brief デストラクタ
GcMgr::~GcMgr()
{
  init(0);
}

// @brief ノード数を指定して初期化する．
void
GcMgr::init(ymuint num)
{
  delete [] mNodeArray;

  mNodeNum = num;
  if ( mNodeNum > 0 ) {
    mNodeArray = new GcNode[mNodeNum];
    for (ymuint i = 0; i < mNodeNum; ++ i) {
      mNodeArray[i].mId = i;
    }
  }
  else {
    mNodeArray = NULL;
  }
}

// @brief ノード数を得る．
ymuint
GcMgr::node_num() const
{
  return mNodeNum;
}

// @brief 2つのノードを接続する．
// @param[in] id1, id2 2つのノードの番号 ( 0 <= id1, id2 < node_num() )
void
GcMgr::connect(ymuint id1,
	       ymuint id2)
{
  assert_cond( id1 < node_num(), __FILE__, __LINE__);
  assert_cond( id2 < node_num(), __FILE__, __LINE__);

  GcNode* node1 = &mNodeArray[id1];
  GcNode* node2 = &mNodeArray[id2];

  node1->mLinkList.push_back(node2);
  node2->mLinkList.push_back(node1);
}

// @brief 彩色する．
// @param[in] color_group 同じ色のノード番号のリストの配列
// @return 彩色数を返す．
ymuint
GcMgr::coloring(vector<vector<ymuint> >& color_group)
{
  // dsatur アルゴリズムを用いる．

  // ソート用のノード配列
  vector<GcNode*> tmp_list(mNodeNum);

  // mNodeArray を隣接ノード数の降順にソートする．
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    tmp_list[i] = &mNodeArray[i];
  }
  sort(tmp_list.begin(), tmp_list.end(), Lt());

  // 1: 隣接するノード数が最大のノードを選び彩色する．
  //    ソートしているので先頭のノードを選べば良い．
  GcNode* max_node = tmp_list[0];
  max_node->mColor = 1;

  ymuint color_num = 1;

  // 2: saturation dgree が最大の未彩色ノードを選び最小の色番号で彩色する．
  for ( ; ; ) {
    ymint max_dgree = 0;
    GcNode* max_node = NULL;
    for (ymuint i = 1; i < mNodeNum; ++ i) {
      GcNode* node = tmp_list[i];
      if ( node->color() > 0 ) {
	continue;
      }
      if ( node->mLinkList.size() < max_dgree ) {
	break;
      }
      ymuint d = node->sat_dgree(color_num) + 1;
      if ( max_dgree < d ) {
	max_dgree = d;
	max_node = node;
      }
    }
    if ( max_node == NULL ) {
      break;
    }

    // max_node につけることのできる最小の色番号を求める．
    vector<bool> colored(color_num, false);
    const vector<GcNode*>& node_list = max_node->mLinkList;
    for (vector<GcNode*>::const_iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      GcNode* node1 = *p;
      ymuint c = node1->color();
      if ( c == 0 ) {
	continue;
      }
      colored[c - 1] = true;
    }
    ymuint min_col = 0;
    for (ymuint i = 0; i < color_num; ++ i) {
      if ( !colored[i] ) {
	min_col = i + 1;
	break;
      }
    }
    if ( min_col == 0 ) {
      ++ color_num;
      min_col = color_num;
    }
    max_node->mColor = min_col;
  }

  // 検証
  {
    for (ymuint i = 0; i < mNodeNum; ++ i) {
      GcNode* node = &mNodeArray[i];
      ymuint c0 = node->color();
      assert_cond( c0 > 0, __FILE__, __LINE__);
      const vector<GcNode*>& node_list = node->link_list();
      for (vector<GcNode*>::const_iterator p = node_list.begin();
	   p != node_list.end(); ++ p) {
	GcNode* node1 = *p;
	ymuint c1 = node1->color();
	assert_cond( c1 > 0, __FILE__, __LINE__);
	assert_cond( c0 != c1, __FILE__, __LINE__);
      }
    }
  }

  color_group.clear();
  color_group.resize(color_num);
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    GcNode* node = &mNodeArray[i];
    ymuint c = node->color();
    assert_cond( c > 0, __FILE__, __LINE__);
    assert_cond( c <= color_num, __FILE__, __LINE__);
    color_group[c - 1].push_back(i);
  }

  return color_num;
}

// @brief グラフの内容を出力する．
void
GcMgr::dump(ostream& s) const
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


//////////////////////////////////////////////////////////////////////
// クラス GcNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GcNode::GcNode()
{
  mColor = 0;
}

// @brief デストラクタ
GcNode::~GcNode()
{
}

// @brief saturation degree を返す．
// @param[in] max_color 現時点の彩色数
ymuint
GcNode::sat_dgree(ymuint max_color) const
{
  vector<bool> count_array(max_color, false);
  for (vector<GcNode*>::const_iterator p = mLinkList.begin();
       p != mLinkList.end(); ++ p) {
    GcNode* node = *p;
    ymuint c = node->color();
    if ( c > 0 ) {
      count_array[c - 1] = true;
    }
  }

  ymuint ans = 0;
  for (ymuint i = 0; i < max_color; ++ i) {
    if ( count_array[i] ) {
      ++ ans;
    }
  }
  return ans;
}

END_NAMESPACE_YM_SATPG
