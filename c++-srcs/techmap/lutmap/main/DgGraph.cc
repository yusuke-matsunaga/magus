
/// @file DgGraph.cc
/// @brief DgGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016, 2018, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "DgGraph.h"
#include "DgNode.h"


BEGIN_NAMESPACE_LUTMAP

//////////////////////////////////////////////////////////////////////
// クラス DgGraph
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DgGraph::DgGraph(
  SizeType node_num
) : mNodeNum{node_num}
{
  mNodeArray = new DgNode[mNodeNum];
  for ( SizeType i = 0; i < mNodeNum; ++ i ) {
    mNodeArray[i].mId = i;
  }
}

// @brief デストラクタ
DgGraph::~DgGraph()
{
  delete [] mNodeArray;
}

// @brief ノードを得る．
DgNode*
DgGraph::node(
  SizeType pos
)
{
  ASSERT_COND( pos >= 0 && pos < node_num() );

  return &mNodeArray[pos];
}

// @brief 枝を張nる．
void
DgGraph::connect(
  SizeType id1,
  SizeType id2
)
{
  ASSERT_COND( id1 >= 0 && id1 < mNodeNum );
  ASSERT_COND( id2 >= 0 && id2 < mNodeNum );

  // 正規化する．
  if ( id1 > id2 ) {
    std::swap(id1, id2);
  }

  auto node1 = &mNodeArray[id1];
  auto node2 = &mNodeArray[id2];

  // 2つの ID からインデックスを作る．
  // 実はオーバーフローするかもしれない．
  SizeType index = id1 * mNodeNum + id2;
  SizeType pos = 0;
  bool exists = false;
  if ( mEdgeHash.count(index) == 0 ) {
    // 本当に重複しているか調べる．
    vector<DgNode*>& adj_link = node1->mAdjLink;
    if ( node1->mAdjLink[pos] == node2 ) {
      // 重複していた．
      exists = true;
    }
  }
  else {
    pos = mEdgeHash.at(index);
  }
  if ( !exists ) {
    pos = node1->mAdjLink.size();
    node1->mAdjLink.push_back(node2);
    node2->mAdjLink.push_back(node1);
    mEdgeHash.emplace(index, pos);
  }
}

BEGIN_NONAMESPACE

struct Lt
{
  bool
  operator()(
    DgNode* left,
    DgNode* right
  )
  {
    return left->adj_link().size() < right->adj_link().size();
  }
};

END_NONAMESPACE

// @brief maximal independent set のサイズを求める．
SizeType
DgGraph::get_mis_size()
{
  // DgNode を隣接数の昇順に並べる．
  vector<DgNode*> node_list(mNodeNum);
  for ( SizeType i = 0; i < mNodeNum; ++ i ) {
    node_list[i] = &mNodeArray[i];
  }
  sort(node_list.begin(), node_list.end(), Lt{});

  // 先頭から取り出して MIS を求める．
  SizeType n = 0;
  for ( SizeType i = 0; i < mNodeNum; ++ i ) {
    auto node = node_list[i];
    if ( !node->is_active() ) {
      continue;
    }
    ++ n;

    // node に隣接しているノードを inactive にする．
    for ( auto node1: node->adj_link() ) {
      node1->inactivate();
    }
  }
  return n;
}

END_NAMESPACE_LUTMAP
