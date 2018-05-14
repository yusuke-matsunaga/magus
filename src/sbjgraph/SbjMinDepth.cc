
/// @file SbjMinDepth.cc
/// @brief SbjMinDepth の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2016, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "SbjMinDepth.h"
#include "SmdNode.h"
#include "SbjGraph.h"
#include "SbjNode.h"


BEGIN_NAMESPACE_YM_SBJ

//////////////////////////////////////////////////////////////////////
// クラス SmdNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SmdNode::SmdNode() :
  mFanoutNum(0),
  mFanoutArray(nullptr)
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
SmdNode::set_id(int id,
		bool logic)
{
  mId = (id << 1) | static_cast<int>(logic);
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

// @brief ファンアウト配列を設定する．
void
SmdNode::set_fanout_array(const vector<SmdEdge*>& foedge_list,
			  Alloc& alloc)
{
  mFanoutNum = foedge_list.size();
  mFanoutArray = alloc.get_array<SmdEdge*>(mFanoutNum);
  for ( int i = 0; i < mFanoutNum; ++ i ) {
    mFanoutArray[i] = foedge_list[i];
  }
}


//////////////////////////////////////////////////////////////////////
// クラス SbjMinDepth
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] sbjgraph 対象のサブジェクトグラフ
SbjMinDepth::SbjMinDepth(const SbjGraph& sbjgraph) :
  mAlloc(4096),
  mSbjGraph(sbjgraph)
{
  int n = sbjgraph.node_num();

  mTfiNodeList.reserve(n);

  mNodeNum = n;
  mNodeArray = mAlloc.get_array<SmdNode>(n);

  // sbjgraph の構造を SmdNode にコピーする．
  mInputList.reserve(sbjgraph.input_list().size());
  for ( auto sbjnode: sbjgraph.input_list() ) {
    int id = sbjnode->id();
    SmdNode* node = &mNodeArray[id];
    node->set_id(id, false);
    mInputList.push_back(node);
  }

  mLogicNodeList.reserve(sbjgraph.logic_num());
  for ( auto sbjnode: sbjgraph.logic_list() ) {
    int id = sbjnode->id();
    SmdNode* node = &mNodeArray[id];
    node->set_id(id, true);
    mLogicNodeList.push_back(node);

    const SbjNode* isbjnode0 = sbjnode->fanin(0);
    SmdNode* inode0 = &mNodeArray[isbjnode0->id()];
    node->set_fanin0(inode0);

    const SbjNode* isbjnode1 = sbjnode->fanin(1);
    SmdNode* inode1 = &mNodeArray[isbjnode1->id()];
    node->set_fanin1(inode1);
  }

  for ( auto sbjnode: sbjgraph.logic_list() ) {
    SmdNode* node = &mNodeArray[sbjnode->id()];
    int nfo0 = sbjnode->fanout_num();
    vector<SmdEdge*> foedge_list;
    foedge_list.reserve(nfo0);
    int nfo = 0;
    for ( int j = 0; j < nfo0; ++ j ) {
      const SbjEdge* sbjedge = sbjnode->fanout_edge(j);
      const SbjNode* sbjfonode = sbjedge->to();
      if ( !sbjfonode->is_output() ) {
	SmdNode* fonode = &mNodeArray[sbjfonode->id()];
	SmdEdge* edge = nullptr;
	if ( sbjedge->pos() == 0 ) {
	  edge = fonode->fanin0_edge();
	}
	else {
	  edge = fonode->fanin1_edge();
	}
	ASSERT_COND( edge->to() == fonode );
	ASSERT_COND( edge->from() == node );
	foedge_list.push_back(edge);
      }
    }
    node->set_fanout_array(foedge_list, mAlloc);
  }
}

// @brief デストラクタ
SbjMinDepth::~SbjMinDepth()
{
}

// @brief 各ノードの minimum depth を求める．
// @param[in] sbjgraph 対象のサブジェクトグラフ
// @param[in] k LUT の最大入力数
// @param[out] depth_array 各ノードの深さを収める配列
// @return 出力の最大深さを返す．
int
SbjMinDepth::operator()(int k,
			vector<int>& depth_array)
{
  depth_array.clear();
  depth_array.resize(mNodeNum, 0);

  // 入力側から depth を計算してゆく
  for ( auto node: mInputList ) {
    node->clear_rtfmark();
    node->clear_vmark();
    node->set_depth(0);
    depth_array[node->id()] = 0;
  }
  int ans = 0;
  for ( auto node: mLogicNodeList ) {
    node->clear_rtfmark();
    node->clear_vmark();
    // ファンインの depth の最大値を max_depth に入れる．
    SmdNode* inode0 = node->fanin0();
    int max_depth = inode0->depth();
    SmdNode* inode1 = node->fanin1();
    int d1 = inode1->depth();
    if ( max_depth < d1 ) {
      max_depth = d1;
    }

    // max_depth 以下の depth を持つノードのみで構成される k-feasible cut を
    // 見つけることができたら node の depth も max_depth となる．
    // そうでなければ max_depth + 1
    if ( !find_k_cut(node, k, max_depth) ) {
      ++ max_depth;
    }
    node->set_depth(max_depth);
    depth_array[node->id()] = max_depth;
    if ( ans < max_depth ) {
      ans = max_depth;
    }
  }
  return ans;
}

// node を根とする深さ d - 1 の k-feasible cut が存在するかどうか調べる．
bool
SbjMinDepth::find_k_cut(SmdNode* node,
			int k,
			int d)
{
  if ( d == 0 ) {
    return false;
  }

  // node の transitive fanin に rmark を付ける．
  // node および 深さ d のノードに tmark を付ける．
  mTfiNodeList.clear();
  node->set_tmark();
  mark_tfi(node, d);

  // PI から tmark の付いたノードまで至る素な経路が
  // k + 1 本以上あれば k-feasible cut は存在しない．
  bool found = true;
  int c = 0;
  for ( auto start: mInputList ) {
    if ( !start->rmark() ) {
      continue;
    }
    bool stat = dfs_fanout(start);
    for ( auto node: mTfiNodeList ) {
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
  for ( auto node: mTfiNodeList ) {
    node->clear_rtfmark();
    if ( node->is_logic() ) {
      node->fanin0_edge()->clear_flow();
      node->fanin1_edge()->clear_flow();
    }
  }

  return found;
}

// node およびその TFI に rmark を付ける．
// depth が d のノードに tmark を付ける．
void
SbjMinDepth::mark_tfi(SmdNode* node,
		      int d)
{
  if ( !node->check_rmark() ) {
    mTfiNodeList.push_back(node);
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
  int n = node->fanout_num();
  for ( int i = 0; i < n; ++ i ) {
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

END_NAMESPACE_YM_SBJ
