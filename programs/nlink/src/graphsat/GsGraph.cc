
/// @file GsGraph.cc
/// @brief GsGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "GsGraphBuilder.h"
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

// @brief 始点を指定する．
// @param[in] node_id 始点のノード番号
void
GsGraphBuilder::set_start_node(ymuint node_id)
{
  ASSERT_COND( node_id < node_num() );
  mStartId = node_id;
}

// @brief 終点を指定する．
// @param[in] node_id 終点のノード番号
void
GsGraphBuilder::set_end_node(ymuint node_id)
{
  ASSERT_COND( node_id < node_num() );
  mEndId = node_id;
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

// @brief 始点のノード番号を返す．
ymuint
GsGraphBuilder::start_node() const
{
  return mStartId;
}

// @brief 終点のノード番号を返す．
ymuint
GsGraphBuilder::end_node() const
{
  return mEndId;
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
  set_size(builder.node_num(), builder.edge_num());

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

    edge1->mGraph = this;
    edge1->mVar = builder.var(i);
    edge1->mSelected = false;
  }

  ymuint start_id = builder.start_node();
  GsNode* start_node = node(start_id);
  start_node->mTermMark = 1;

  ymuint end_id = builder.end_node();
  GsNode* end_node = node(end_id);
  end_node->mTermMark = 2;

  mUpdate = false;
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
GsGraph::GsGraph(const GsGraph& src)
{
  set_size(src.node_num(), src.edge_num());

  for (ymuint i = 0; i < mNodeNum; ++ i) {
    GsNode* node = &mNodeArray[i];
    const GsNode* src_node = &src.mNodeArray[i];
    ymuint n = src_node->edge_num();
    node->mEdgeNum = n;
    node->mEdgeList = new GsEdge*[n];
    for (ymuint j = 0; j < n; ++ j) {
      ymuint edge_id = src_node->edge(j)->id();
      node->mEdgeList[j] = &mEdgeArray[edge_id];
    }
  }

  for (ymuint i = 0; i < mEdgeNum; ++ i) {
    GsEdge* edge1 = edge(i);
    GsEdge* src_edge = &src.mEdgeArray[i];

    ymuint node1_id = src_edge->node1()->id();
    edge1->mNode1 = node(node1_id);

    ymuint node2_id = src_edge->node2()->id();
    edge1->mNode2 = node(node2_id);

    edge1->mGraph = this;
    edge1->mVar = src_edge->var();
    edge1->mSelected = false;
  }

  ymuint start_id = src.start_node()->id();
  GsNode* start_node = node(start_id);
  start_node->mTermMark = 1;

  ymuint end_id = src.end_node()->id();
  GsNode* end_node = node(end_id);
  end_node->mTermMark = 2;

  mUpdate = false;
}

// @brief ノード数と枝数を設定する．
// @param[in] node_num ノード数
// @param[in] edge_num
void
GsGraph::set_size(ymuint node_num,
		  ymuint edge_num)
{
  mNodeNum = node_num;
  mNodeArray = new GsNode[mNodeNum];
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    mNodeArray[i].mId = i;
    mNodeArray[i].mTermMark = 0;
  }

  mEdgeNum = edge_num;
  mEdgeArray = new GsEdge[mEdgeNum];
  for (ymuint i = 0; i < mEdgeNum; ++ i) {
    mEdgeArray[i].mId = i;
  }
}

// @brief デストラクタ
GsGraph::~GsGraph()
{
  delete [] mEdgeArray;
  delete [] mNodeArray;
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

// @relates GsGraph
// @brief デバッグ用にGsGraph の内容を出力する．
// @param[in] s 出力ストリーム
// @param[in] graph グラフ
void
print_graph(ostream& s,
	    GsGraph& graph)
{
  s << "===<GsGraph>===" << endl
    << "# of nodes: " << graph.node_num() << endl
    << "# of edges: " << graph.edge_num() << endl;
  for (ymuint i = 0; i < graph.node_num(); ++ i) {
    const GsNode* node = graph.node(i);
    s << " Node#" << node->id() << ": # of edges: " << node->edge_num() << endl;
    s << "  ";
    for (ymuint j = 0; j < node->edge_num(); ++ j) {
      const GsEdge* edge = node->edge(j);
      s << " Edge#" << edge->id();
    }
    s << endl;
  }
  s << endl;
  s << " StartNode = Node#" << graph.start_node()->id() << endl
    << " EndNode   = Node#" << graph.end_node()->id() << endl
    << endl;
  for (ymuint i = 0; i < graph.edge_num(); ++ i) {
    const GsEdge* edge = graph.edge(i);
    s << " Edge#" << edge->id() << ": VarId = " << edge->var()
      << "(Node#" << edge->node1()->id()
      << ", Node#" << edge->node2()->id() << ")" << endl;
  }
  s << endl;
}

END_NAMESPACE_YM_NLINK
