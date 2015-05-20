
/// @file GsGraph.cc
/// @brief GsGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "GsGraph.h"
#include "GsNode.h"
#include "GsEdge.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
// クラス GsGraphBuilder
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] node_num ノード数
GsGraphBuilder::GsGraphBuilder(ymuint node_num) :
  mNodeNum(node_num),
  mEdgeListArray(node_num)
{
}

// @brief デストラクタ
GsGraphBuilder::~GsGraphBuilder()
{
}

// @brief 枝を追加する．
// @param[in] node1_id, node2_id 両端のノード番号
// @param[in] var 枝の変数番号
void
GsGraphBuilder::add_edge(ymuint node1_id,
			 ymuint node2_id,
			 VarId var)
{
  ASSERT_COND( node1_id < node_num() );
  ASSERT_COND( node2_id < node_num() );

  ymuint edge_id = mNode1List.size();
  mEdgeListArray[node1_id].push_back(edge_id);
  mEdgeListArray[node2_id].push_back(edge_id);

  mNode1List.push_back(node1_id);
  mNode2List.push_back(node2_id);
  mVarIdList.push_back(var);
}

// @brief ノード数を返す．
ymuint
GsGraphBuilder::node_num() const
{
  return mNodeNum;
}

// @brief ノードの枝番号リストを返す．
// @param[in] pos ノードの番号 ( 0 <= pos < node_num() )
const vector<ymuint>&
GsGraphBuilder::edge_list(ymuint pos) const
{
  ASSERT_COND( pos < node_num() );
  return mEdgeListArray[pos];
}

// @brief 枝数を返す．
ymuint
GsGraphBuilder::edge_num() const
{
  return mNode1List.size();
}

// @brief 枝の一方のノード番号を返す．
// @param[in] pos 枝の番号 ( 0 <= pos < edge_num() )
ymuint
GsGraphBuilder::node1(ymuint pos) const
{
  ASSERT_COND( pos < edge_num() );
  return mNode1List[pos];
}

// @brief 枝の他方のノード番号を返す．
// @param[in] pos 枝の番号 ( 0 <= pos < edge_num() )
ymuint
GsGraphBuilder::node2(ymuint pos) const
{
  ASSERT_COND( pos < edge_num() );
  return mNode2List[pos];
}

// @brief 枝の変数番号を返す．
VarId
GsGraphBuilder::var(ymuint pos) const
{
  ASSERT_COND( pos < edge_num() );
  return mVarIdList[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス GsGraph
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] builder ビルダオブジェクト
GsGraph::GsGraph(const GsGraphBuilder& builder)
{
  mNodeNum = builder.node_num();
  mNodeArray = new GsNode[mNodeNum];

  mEdgeNum = builder.edge_num();
  mEdgeArray = new GsEdge[mEdgeNum];

  for (ymuint i = 0; i < mNodeNum; ++ i) {
    GsNode* node = &mNodeArray[i];
    const vector<ymuint>& edge_list = builder.edge_list(i);
    ymuint n = edge_list.size();
    node->mEdgeNum = n;
    node->mEdgeList = new GsEdge*[n];
    for (ymuint j = 0; j < n; ++ j) {
      ymuint edge_id = edge_list[j];
      node->mEdgeList[j] = &mEdgeArray[edge_id];
    }
  }

  for (ymuint i = 0; i < mEdgeNum; ++ i) {
    GsEdge* edge1 = edge(i);

    ymuint node1_id = builder.node1(i);
    edge1->mNode1 = node(node1_id);

    ymuint node2_id = builder.node2(i);
    edge1->mNode2 = node(node2_id);

    edge1->mVar = builder.var(i);

    edge1->mSelected = false;
  }
}

// @brief デストラクタ
GsGraph::~GsGraph()
{
}

// @brief ノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < node_num() )
GsNode*
GsGraph::node(ymuint pos)
{
  ASSERT_COND( pos < node_num() );
  return &mNodeArray[pos];
}

// @brief 枝を返す．
// @param[in] pos 位置番号 ( 0 <= pos edge_num() )
GsEdge*
GsGraph::edge(ymuint pos)
{
  ASSERT_COND( pos < edge_num() );
  return &mEdgeArray[pos];
}

END_NAMESPACE_YM_NLINK
