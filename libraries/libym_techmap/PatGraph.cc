
/// @file libym_techmap/PatGraph.cc
/// @brief PatGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "PatGraph.h"


BEGIN_NAMESPACE_YM_TECHMAP

// @brief コンストラクタ
PatGraph::PatGraph()
{
  mNodeNum = 0;
  mNodeTypeArray = NULL;
  mEdgeNum = 0;
  mEdgeArray = NULL;
}

// @brief デストラクタ
PatGraph::~PatGraph()
{
  delete [] mNodeTypeArray;
  delete [] mEdgeArray;
}

// @brief ノード数を設定し，mNodeTypeArray を確保する．
// @param[in] n ノード数
void
PatGraph::set_node_num(ymuint n)
{
  assert_cond( mNodeNum == 0, __FILE__, __LINE__);
  mNodeNum = n;
  mNodeTypeArray = new tType[n];
}

// @brief ノードの種類を設定する．
// @param[in] id ノード番号 ( 0 <= id < node_num() )
// @param[in] type 設定するノードの種類
void
PatGraph::set_node_type(ymuint id,
			tType type)
{
  assert_cond( id < mNodeNum, __FILE__, __LINE__);
  mNodeTypeArray[id] = type;
}

// @brief 枝の数を設定し，mEdgeArray を確保する．
// @param[in] n 枝の数
void
PatGraph::set_edge_num(ymuint n)
{
  assert_cond( mEdgeNum == 0, __FILE__, __LINE__);
  mEdgeNum = n;
  mEdgeArray = new PatEdge[n];
}

// @brief 枝を設定する．
// @param[in] id 枝の番号 ( 0 <= id < edge_num() )
// @param[in] from_id 入力元のノード番号
// @param[in] to_id 出力先のノード番号
// @param[in] fanin_pos ファンインの位置 ( 0  or 1 )
// @param[in] inverted 極性
void
PatGraph::set_edge(ymuint id,
		   ymuint from_id,
		   ymuint to_id,
		   ymuint fanin_pos,
		   bool inverted)
{
  assert_cond( id < mEdgeNum, __FILE__, __LINE__);
  ymuint32 data = 0U;
  if ( inverted ) {
    data |= 1U;
  }
  if ( fanin_pos == 1 ) {
    data |= 2U;
  }
  data |= (from_id << 2);
  data |= (to_id << 17);
  mEdgeArray[id].mData = data;
}

END_NAMESPACE_YM_TECHMAP
