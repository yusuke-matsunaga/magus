
/// @file NlBan.cc
/// @brief NlBan の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlBan.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
// クラス NlBan
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] graph 問題を表すグラフ
NlBan::NlBan(const Nlgraph& graph) :
  mGraph(graph)
{
  mWidth = graph.width();
  mHeight = graph.height();
  mNum = graph.num();

  mGridArray.resize((mWidth + 2) * (mHeight * 2));
  for (ymuint i = 0; i < mGridArray.size(); ++ i) {
    mGridArray[i] = 1;
  }

  for (ymuint x = 0; x < mWidth; ++ x) {
    for (ymuint y = 0; y < mHeight; ++ y) {
      grid(x, y) = 0;
    }
  }

  for (ymuint i = 0; i < mNum; ++ i) {
    const NlNode* node1 = graph.start_node(i);
    {
      ymuint x = node1->x();
      ymuint y = node1->y();
      grid(x, y) = ((i + 1) << 2) | 2;
    }
    const NlNode* node2 = graph.end_node(i);
    {
      ymuint x = node2->x();
      ymuint y = node2->y();
      grid(x, y) = ((i + 1) << 2) | 2;
    }
  }
}

// @brief デストラクタ
NlBan::~NlBan()
{
}

// @brief 外周部の線分を確定させる．
void
NlBan::phase1()
{
  // 最初の外周を求める．
  vector<const NlNode*> fringe_list;
  for (ymuint x = 0; x < mWidth; ++ x) {
    const NlNode* node = mGraph.node(x, 0);
    fringe_list.push_back(node);
  }
  for (ymuint y = 1; y < mHeight - 1; ++ y) {
    const NlNode* node = mGraph.node(mWidth - 1, y);
    fringe_list.push_back(node);
  }
  for (ymuint x = mWidth; x > 0; -- x) {
    const NlNode* node = mGraph.node(x - 1, mHeight - 1);
    fringe_list.push_back(node);
  }
  for (ymuint y = mHeight - 1; y > 0; -- y) {
    const NlNode* node = mGraph.node(0, y);
    fringe_list.push_back(node);
  }

}

// @brief 外周に隣接しる格子のリストを求める．
void
NlBan::get_fringe_list(vector<const NlNode*>& node_list)
{

}

ymuint&
NlBan::grid(ymuint x,
	    ymuint y)
{
  return mGridArray[(x + 1) * (mHeight + 2) + (y + 1)];
}

END_NAMESPACE_YM_NLINK
