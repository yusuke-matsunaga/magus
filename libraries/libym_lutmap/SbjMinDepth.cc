
/// @file libym_lutmap/SbjMinDepth.cc
/// @brief SbjMinDepth の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_lutmap/SbjMinDepth.h"
#include "SmdNode.h"
#include "ym_lutmap/SbjGraph.h"


BEGIN_NAMESPACE_YM_LUTMAP

//////////////////////////////////////////////////////////////////////
// クラス SmdNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SmdNode::SmdNode() :
  mFanoutNum(0),
  mFanoutArray(NULL)
{
}

// @brief デストラクタ
SmdNode::~SmdNode()
{
}

// @brief ID番号を設定する．
// @param[in] id ID番号
// @param[in] logic 論理ノードの時 true にセットするフラグ
void
SmdNode::set_id(ymuint id,
		bool logic)
{
  mId = (id << 1) | static_cast<ymuint>(logic);
}

// @brief 1つ目のファンインを設定する．
void
SmdNode::set_fanin0(SmdNode* from)
{
  mFanin0.mFrom = from;
  mFanin0.mTo = this;
  mFanin0.mFlags = 0U;
}

// @brief 2つ目のファンインを設定する．
void
SmdNode::set_fanin1(SmdNode* from)
{
  mFanin1.mFrom = from;
  mFanin1.mTo = this;
  mFanin1.mFlags = 1U;
}

// @brief ファンアウト数を設定する．
void
SmdNode::set_fanout_num(ymuint n,
			void* p)
{
  mFanoutNum = n;
  mFanoutArray = new (p) SmdEdge*[n];
}

// @brief pos 番目のファンアウトを設定する．
void
SmdNode::set_fanout(ymuint pos,
		    SmdEdge* edge)
{
  mFanoutArray[pos] = edge;
}


//////////////////////////////////////////////////////////////////////
// クラス SbjMinDepth
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SbjMinDepth::SbjMinDepth() :
  mAlloc(4096)
{
}

// @brief デストラクタ
SbjMinDepth::~SbjMinDepth()
{
}

// @brief 各ノードの minimum depth を求める．
// @param[in] sbjgraph 対象のサブジェクトグラフ
// @param[in] k LUT の最大入力数
ymuint
SbjMinDepth::get_min_depth(const SbjGraph& sbjgraph,
			   ymuint k)
{
  ymuint n = sbjgraph.max_node_id();

  mNodeList.clear();
  mNodeList.reserve(n);

  mNodeNum = n;
  void* p = mAlloc.get_memory(sizeof(SmdNode) * n);
  mNodeArray = new (p) SmdNode[n];

  // sbjgraph の構造を SmdNode にコピーする．
  vector<const SbjNode*> ppi_list;
  sbjgraph.ppi_list(ppi_list);
  vector<SmdNode*> input_list;
  input_list.reserve(ppi_list.size());
  for (vector<const SbjNode*>::const_iterator p = ppi_list.begin();
       p != ppi_list.end(); ++ p) {
    const SbjNode* sbjnode = *p;
    ymuint id = sbjnode->id();
    SmdNode* node = &mNodeArray[id];
    node->set_id(id, false);
    node->set_depth(0);
    node->clear_rtfmark();
    node->clear_vmark();
    input_list.push_back(node);
  }
  vector<const SbjNode*> sbjnode_list;
  sbjgraph.sort(sbjnode_list);
  vector<SmdNode*> node_list;
  node_list.reserve(sbjgraph.lnode_num());
  for (vector<const SbjNode*>::const_iterator p = sbjnode_list.begin();
       p != sbjnode_list.end(); ++ p) {
    const SbjNode* sbjnode = *p;
    assert_cond( sbjnode->is_logic(), __FILE__, __LINE__);
    ymuint id = sbjnode->id();
    SmdNode* node = &mNodeArray[id];
    node_list.push_back(node);
    node->set_id(id, true);
    node->set_depth(0);
    node->clear_rtfmark();
    node->clear_vmark();
    const SbjNode* isbjnode0 = sbjnode->fanin(0);
    SmdNode* inode0 = &mNodeArray[isbjnode0->id()];
    node->set_fanin0(inode0);
    const SbjNode* isbjnode1 = sbjnode->fanin(1);
    SmdNode* inode1 = &mNodeArray[isbjnode1->id()];
    node->set_fanin1(inode1);
  }
  for (ymuint i = 0; i < n; ++ i) {
    const SbjNode* sbjnode = sbjgraph.node(i);
    if ( sbjnode == NULL ) continue;
    SmdNode* node = &mNodeArray[sbjnode->id()];
    const SbjEdgeList& fanout_list = sbjnode->fanout_list();
    ymuint nfo = 0;
    for (SbjEdgeList::const_iterator p = fanout_list.begin();
	 p != fanout_list.end(); ++ p) {
      const SbjEdge* sbjedge = *p;
      const SbjNode* sbjfonode = sbjedge->to();
      if ( !sbjfonode->is_ppo() ) {
	++nfo;
      }
    }
    void* p = mAlloc.get_memory(sizeof(SmdEdge*) * nfo);
    node->set_fanout_num(nfo, p);
    ymuint pos = 0;
    for (SbjEdgeList::const_iterator p = fanout_list.begin();
	 p != fanout_list.end(); ++ p, ++ pos) {
      const SbjEdge* sbjedge = *p;
      const SbjNode* sbjfonode = sbjedge->to();
      if ( sbjfonode->is_ppo() ) continue;
      SmdNode* fonode = &mNodeArray[sbjfonode->id()];
      assert_cond( fonode->id() == sbjfonode->id(), __FILE__, __LINE__);
      SmdEdge* edge = NULL;
      if ( sbjedge->pos() == 0 ) {
	edge = fonode->fanin0_edge();
      }
      else {
	edge = fonode->fanin1_edge();
      }
      assert_cond( edge->to() == fonode, __FILE__, __LINE__);
      assert_cond( edge->from() == node, __FILE__, __LINE__);
      node->set_fanout(pos, edge);
    }
  }

  // 入力側から depth を計算してゆく
  ymuint ans = 0;
  for (vector<SmdNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    SmdNode* node = *p;
    // ファンインの depth の最大値を max_depth に入れる．
    SmdNode* inode0 = node->fanin0();
    ymuint max_depth = inode0->depth();
    SmdNode* inode1 = node->fanin1();
    ymuint d1 = inode1->depth();
    if ( max_depth < d1 ) {
      max_depth = d1;
    }

    // max_depth 以下の depth を持つノードのみで構成される k-feasible cut を
    // 見つけることができたら node の depth も max_depth となる．
    // そうでなければ max_depth + 1
    if ( !find_k_cut(node, input_list, k, max_depth) ) {
      ++ max_depth;
    }
    node->set_depth(max_depth);
    if ( sbjgraph.node(node->id())->depth() != max_depth ) {
      cout << "Node#" << node->id()
	   << ": Sbjnode->depth() = " << sbjgraph.node(node->id())->depth()
	   << ", SmdNode->depth() = " << max_depth
	   << endl;
    }
    if ( ans < max_depth ) {
      ans = max_depth;
    }
  }
  return ans;
}

// @brief 各ノードの深さを求める．
ymuint
SbjMinDepth::node_depth(const SbjNode* node)
{
  return mNodeArray[node->id()].depth();
}

// node を根とする深さ d - 1 の k-feasible cut が存在するかどうか調べる．
bool
SbjMinDepth::find_k_cut(SmdNode* node,
			const vector<SmdNode*>& input_list,
			ymuint k,
			ymuint d)
{
  if ( d == 0 ) {
    return false;
  }

  // node の transitive fanin に rmark を付ける．
  // node および 深さ d のノードに tmark を付ける．
  node->set_tmark();
  mark_tfi(node, d);

  // PI から tmark の付いたノードまで至る素な経路が
  // k + 1 本以上あれば k-feasible cut は存在しない．
  bool found = true;
  ymuint c = 0;
  for (vector<SmdNode*>::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    SmdNode* start = *p;
    if ( !start->rmark() ) {
      continue;
    }
    bool stat = dfs_fanout(start);
    for (vector<SmdNode*>::iterator q = mNodeList.begin();
	 q != mNodeList.end(); ++ q) {
      SmdNode* node = *q;
      node->clear_vmark();
    }
    if ( stat ) {
      ++ c;
      if ( c > k ) {
	found = false;
	break;
      }
    }
  }
  for (vector<SmdNode*>::iterator q = mNodeList.begin();
       q != mNodeList.end(); ++ q) {
    SmdNode* node = *q;
    node->clear_rtfmark();
    if ( node->is_logic() ) {
      node->fanin0_edge()->clear_flow();
      node->fanin1_edge()->clear_flow();
    }
  }
  mNodeList.clear();

  return found;
}

// node およびその TFI に rmark を付ける．
// depth が d のノードに tmark を付ける．
void
SbjMinDepth::mark_tfi(SmdNode* node,
		      ymuint d)
{
  if ( !node->check_rmark() ) {
    mNodeList.push_back(node);
    if ( node->depth() == d ) {
      node->set_tmark();
    }
    if ( node->is_logic() ) {
      mark_tfi(node->fanin0(), d);
      mark_tfi(node->fanin1(), d);
    }
  }
}

// node のファンアウトを探索する．
bool
SbjMinDepth::dfs_fanout(SmdNode* node)
{
  ymuint n = node->fanout_num();
  for (ymuint i = 0; i < n; ++ i) {
    SmdEdge* edge = node->fanout_edge(i);
    SmdNode* to_node = edge->to();
    if ( !to_node->rmark() ) {
      // target の TFI ではない．
      continue;
    }
    if ( !to_node->check_vmark1() ) {
      bool stat = dfs(to_node, 0);
      if ( stat ) {
	edge->set_flow();
	return true;
      }
    }
  }
  return false;
}

bool
SbjMinDepth::dfs(SmdNode* cur_node,
		 int dir)
{
  if ( cur_node->tmark() ) {
    // target にたどり着いた
    return true;
  }

  if ( dir == 0 ) {
    // 前向き
    SmdEdge* edge0 = cur_node->fanin0_edge();
    SmdEdge* edge1 = cur_node->fanin1_edge();
    if ( edge0->flow() ) {
      SmdNode* from_node = edge0->from();
      if ( !from_node->check_vmark2() ) {
	bool stat = dfs(from_node, 1);
	if ( stat ) {
	  edge0->clear_flow();
	  return true;
	}
      }
    }
    else if ( edge1->flow() ) {
      SmdNode* from_node = edge1->from();
      if ( !from_node->check_vmark2() ) {
	bool stat = dfs(from_node, 1);
	if ( stat ) {
	  edge1->clear_flow();
	  return true;
	}
      }
    }
    else if ( !cur_node->check_vmark2() ) {
      bool stat = dfs_fanout(cur_node);
      if ( stat ) {
	return true;
      }
    }
  }
  else {
    if ( cur_node->is_logic() && !cur_node->check_vmark1() ) {
      SmdEdge* edge0 = cur_node->fanin0_edge();
      if ( edge0->flow() ) {
	SmdNode* from_node = edge0->from();
	if ( !from_node->check_vmark2() ) {
	  bool stat = dfs(from_node, 1);
	  if ( stat ) {
	    edge0->clear_flow();
	    return true;
	  }
	}
      }
      SmdEdge* edge1 = cur_node->fanin1_edge();
      if ( edge1->flow() ) {
	SmdNode* from_node = edge1->from();
	if ( !from_node->check_vmark2() ) {
	  bool stat = dfs(from_node, 1);
	  if ( stat ) {
	    edge1->clear_flow();
	    return true;
	  }
	}
      }
    }
    bool stat = dfs_fanout(cur_node);
    if ( stat ) {
      return true;
    }
  }
  return false;
}

END_NAMESPACE_YM_LUTMAP
