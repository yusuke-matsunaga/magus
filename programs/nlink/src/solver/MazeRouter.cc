
/// @file MazeRouter.cc
/// @brief MazeRouter の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "MazeRouter.h"
#include "NlGraph.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
// クラス MazeRouter
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MazeRouter::MazeRouter()
{
}

// @brief デストラクタ
MazeRouter::~MazeRouter()
{
}

// @brief ラベル付を行う．
// @param[in] graph グラフ
// @param[in] idx 線分番号
// @param[in] dir 向き
// @param[out] edge_list ラベル付された枝番号のリスト
// @param[out] index_list 各ラベルごとの末尾のインデックスリスト
// @return 最短経路長を返す．
ymuint
MazeRouter::labeling(const NlGraph& graph,
		     ymuint idx,
		     bool dir,
		     vector<ymuint>& edge_list,
		     vector<ymuint>& index_list)
{
  ASSERT_COND( idx < graph.num() );

  mCellArray.clear();
  mCellArray.resize(graph.max_node_id());
  for (ymuint i = 0; i < mCellArray.size(); ++ i) {
    mCellArray[i] = 0;
  }

  for (ymuint i = 0; i < graph.num(); ++ i) {
    if ( i == idx ) {
      continue;
    }

    const NlNode* start_node = graph.start_node(i);
    ymuint index1 = start_node->id();
    mCellArray[index1] = -1;

    const NlNode* end_node = graph.end_node(i);
    ymuint index2 = end_node->id();
    mCellArray[index2] = -1;
  }

  // 始点
  const NlNode* s_node = graph.start_node(idx);

  // 終点
  const NlNode* t_node = graph.end_node(idx);

  if ( dir ) {
    const NlNode* tmp = s_node;
    s_node = t_node;
    t_node = tmp;
  }

  // 始点のセルを cell_list に積む．
  vector<const NlNode*> cell_list;
  vector<ymuint> end_list;
  index_list.clear();

  ymuint label = 0;

  cell_list.push_back(s_node);
  end_list.push_back(cell_list.size());

  bool found = false;
  ymuint rpos0 = 0;
  for ( ; ; ) {
    ymuint epos = end_list[label];
    ++ label;
    for (ymuint i = rpos0; i < epos; ++ i) {
      const NlNode* node = cell_list[i];
      if ( node == t_node ) {
	found = true;
	break;
      }
      label1(graph, node, DirUp,    label, cell_list, edge_list);
      label1(graph, node, DirDown,  label, cell_list, edge_list);
      label1(graph, node, DirLeft,  label, cell_list, edge_list);
      label1(graph, node, DirRight, label, cell_list, edge_list);
    }
    end_list.push_back(cell_list.size());
    index_list.push_back(edge_list.size());
    rpos0 = epos;
    if ( found ) {
      break;
    }
  }

  return label;
}

// @brief ラベル付けの基本処理
void
MazeRouter::label1(const NlGraph& graph,
		   const NlNode* node,
		   Dir dir,
		   ymuint label,
		   vector<const NlNode*>& cell_list,
		   vector<ymuint>& edge_list)
{
  ymuint x = node->x();
  ymuint y = node->y();
  ymuint edge;
  switch ( dir ) {
  case DirUp:
    edge = node->upper_edge();
    -- y;
    break;

  case DirDown:
    edge = node->lower_edge();
    ++ y;
    break;

  case DirLeft:
    edge = node->left_edge();
    -- x;
    break;

  case DirRight:
    edge = node->right_edge();
    ++ x;
    break;
  }
  if ( edge == 0 ) {
    // 範囲外
    return;
  }

  const NlNode* node1 = graph.node(x, y);
  ymuint index1 = node1->id();
  if ( mCellArray[index1] == 0 ) {
    mCellArray[index1] = label;
    cell_list.push_back(node1);
  }
  if ( mCellArray[index1] == label ) {
    edge_list.push_back(edge);
  }
}

END_NAMESPACE_YM_NLINK
