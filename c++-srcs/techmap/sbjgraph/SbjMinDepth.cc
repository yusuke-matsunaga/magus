
/// @file SbjMinDepth.cc
/// @brief SbjMinDepth の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2016, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "SbjMinDepth.h"
#include "SmdNode.h"
#include "SbjGraph.h"
#include "SbjNode.h"


BEGIN_NAMESPACE_SBJ

//////////////////////////////////////////////////////////////////////
// クラス SbjMinDepth
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SbjMinDepth::SbjMinDepth(
  const SbjGraph& sbjgraph
) : mSbjGraph{sbjgraph}
{
  SizeType n = sbjgraph.node_num();

  mTfiNodeList.reserve(n);

  mNodeNum = n;
  mNodeArray = new SmdNode[n];

  vector<vector<SmdEdge*>> foedge_list_array(n);

  // sbjgraph の構造を SmdNode にコピーする．
  mInputList.reserve(sbjgraph.input_list().size());
  for ( auto sbjnode: sbjgraph.input_list() ) {
    SizeType id = sbjnode->id();
    auto node = &mNodeArray[id];
    node->set_id(id, false);
    mInputList.push_back(node);
  }

  mLogicNodeList.reserve(sbjgraph.logic_num());
  for ( auto sbjnode: sbjgraph.logic_list() ) {
    SizeType id = sbjnode->id();
    auto node = &mNodeArray[id];
    node->set_id(id, true);
    mLogicNodeList.push_back(node);

    auto isbjnode0 = sbjnode->fanin0();
    auto inode0 = &mNodeArray[isbjnode0->id()];
    node->set_fanin0(inode0);
    auto edge0 = node->fanin0_edge();
    foedge_list_array[inode0->id()].push_back(edge0);

    auto isbjnode1 = sbjnode->fanin1();
    auto inode1 = &mNodeArray[isbjnode1->id()];
    node->set_fanin1(inode1);
    auto edge1 = node->fanin1_edge();
    foedge_list_array[inode1->id()].push_back(edge1);
  }

  for ( SizeType i = 0; i < n; ++ i ) {
    auto node = &mNodeArray[i];
    node->set_fanout_array(foedge_list_array[node->id()]);
  }
}

// @brief デストラクタ
SbjMinDepth::~SbjMinDepth()
{
  delete [] mNodeArray;
}

// @brief 各ノードの minimum depth を求める．
SizeType
SbjMinDepth::operator()(
  SizeType k,
  vector<SizeType>& depth_array
)
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

  SizeType ans = 0;
  for ( auto node: mLogicNodeList ) {
    node->clear_rtfmark();
    node->clear_vmark();
    // ファンインの depth の最大値を max_depth に入れる．
    auto inode0 = node->fanin0();
    SizeType max_depth = inode0->depth();
    auto inode1 = node->fanin1();
    SizeType d1 = inode1->depth();
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
SbjMinDepth::find_k_cut(
  SmdNode* node,
  SizeType k,
  SizeType d
)
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
  SizeType c = 0;
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
SbjMinDepth::mark_tfi(
  SmdNode* node,
  SizeType d
)
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
SbjMinDepth::dfs_fanout(
  SmdNode* node
)
{
  SizeType n = node->fanout_num();
  for ( SizeType i = 0; i < n; ++ i ) {
    auto edge = node->fanout_edge(i);
    auto to_node = edge->to();
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
SbjMinDepth::dfs(
  SmdNode* cur_node,
  int dir
)
{
  if ( cur_node->tmark() ) {
    // target にたどり着いた
    return true;
  }

  if ( dir == 0 ) {
    // 前向き
    auto edge0 = cur_node->fanin0_edge();
    auto edge1 = cur_node->fanin1_edge();
    if ( edge0->flow() ) {
      auto from_node = edge0->from();
      if ( !from_node->check_vmark2() ) {
	bool stat = dfs(from_node, 1);
	if ( stat ) {
	  edge0->clear_flow();
	  return true;
	}
      }
    }
    else if ( edge1->flow() ) {
      auto from_node = edge1->from();
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
      auto edge0 = cur_node->fanin0_edge();
      if ( edge0->flow() ) {
	auto from_node = edge0->from();
	if ( !from_node->check_vmark2() ) {
	  bool stat = dfs(from_node, 1);
	  if ( stat ) {
	    edge0->clear_flow();
	    return true;
	  }
	}
      }
      auto edge1 = cur_node->fanin1_edge();
      if ( edge1->flow() ) {
	auto from_node = edge1->from();
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

END_NAMESPACE_SBJ
