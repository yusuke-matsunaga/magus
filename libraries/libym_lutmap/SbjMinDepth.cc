
/// @file libym_lutmap/SbjMinDepth.cc
/// @brief SbjMinDepth の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_lutmap/SbjMinDepth.h"
#include "ym_lutmap/SbjGraph.h"


BEGIN_NAMESPACE_YM_LUTMAP

//////////////////////////////////////////////////////////////////////
// クラス SbjMinDepth
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SbjMinDepth::SbjMinDepth()
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
  mMarkArray.clear();
  mMarkArray.resize(n, 0U);
  mDepthArray.clear();
  mDepthArray.resize(n);
  mFlowArray.clear();
  mFlowArray.resize(n * 2, false);

  // 外部入力ノードの depth を 0 にする．
  vector<const SbjNode*> input_list;
  sbjgraph.ppi_list(input_list);
  for (vector<const SbjNode*>::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const SbjNode* node = *p;
    mDepthArray[node->id()] = 0;
  }

  // 入力側から depth を計算してゆく
  ymuint ans = 0;
  vector<const SbjNode*> node_list;
  sbjgraph.sort(node_list);
  for (vector<const SbjNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    const SbjNode* node = *p;
    // ファンインの depth の最大値を max_depth に入れる．
    ymuint max_depth = 0;
    for (ymuint i = 0; i < 2; ++ i) {
      const SbjNode* inode0 = node->fanin(i);
      ymuint d0 = mDepthArray[inode0->id()];
      if ( max_depth < d0 ) {
	max_depth = d0;
      }
    }

    // max_depth 以下の depth を持つノードのみで構成される k-feasible cut を
    // 見つけることができたら node の depth も max_depth となる．
    // そうでなければ max_depth + 1
    if ( !find_k_cut(node, input_list, k, max_depth) ) {
      ++ max_depth;
    }
    mDepthArray[node->id()] = max_depth;
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
  return mDepthArray[node->id()];
}

// node を根とする深さ d - 1 の k-feasible cut が存在するかどうか調べる．
bool
SbjMinDepth::find_k_cut(const SbjNode* node,
			const vector<const SbjNode*>& input_list,
			ymuint k,
			ymuint d)
{
  if ( d == 0 ) {
    return false;
  }

  // node の transitive fanin に rmark を付ける．
  // node および 深さ d のノードに tmark を付ける．
  set_tmark(node);
  mark_tfi(node, d);

  // PI から tmark の付いたノードまで至る素な経路が
  // k + 1 本以上あれば k-feasible cut は存在しない．
  bool found = true;
  ymuint c = 0;
  for (vector<const SbjNode*>::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const SbjNode* start = *p;
    if ( !rmark(start) ) {
      continue;
    }
    bool stat = dfs_fanout(start);
    for (vector<const SbjNode*>::iterator q = mNodeList.begin();
	 q != mNodeList.end(); ++ q) {
      const SbjNode* node = *q;
      clear_vmark(node);
    }
    if ( stat ) {
      ++ c;
      if ( c > k ) {
	found = false;
	break;
      }
    }
  }
  for (vector<const SbjNode*>::iterator q = mNodeList.begin();
       q != mNodeList.end(); ++ q) {
    const SbjNode* node = *q;
    clear_rtfmark(node);
    if ( node->is_logic() ) {
      clear_flow(node->fanin_edge(0));
      clear_flow(node->fanin_edge(1));
    }
  }
  mNodeList.clear();

  return found;
}

// node およびその TFI に rmark を付ける．
// depth が d のノードに tmark を付ける．
void
SbjMinDepth::mark_tfi(const SbjNode* node,
		      ymuint d)
{
  if ( !check_rmark(node) ) {
    mNodeList.push_back(node);
    if ( node_depth(node) == d ) {
      set_tmark(node);
    }
    if ( node->is_logic() ) {
      mark_tfi(node->fanin(0), d);
      mark_tfi(node->fanin(1), d);
    }
  }
}

// node のファンアウトを探索する．
bool
SbjMinDepth::dfs_fanout(const SbjNode* node)
{
  const SbjEdgeList& fanout_list = node->fanout_list();
  for (SbjEdgeList::const_iterator p = fanout_list.begin();
       p != fanout_list.end(); ++ p) {
    SbjEdge* edge = *p;
    const SbjNode* to_node = edge->to();
    if ( !rmark(to_node) ) {
      // target の TFI ではない．
      continue;
    }
    if ( !check_vmark1(to_node) ) {
      bool stat = dfs(to_node, 0);
      if ( stat ) {
	set_flow(edge);
	return true;
      }
    }
  }
  return false;
}

bool
SbjMinDepth::dfs(const SbjNode* cur_node,
		 int dir)
{
  if ( tmark(cur_node) ) {
    // target にたどり着いた
    return true;
  }

  if ( dir == 0 ) {
    // 前向き
    const SbjEdge* edge0 = cur_node->fanin_edge(0);
    const SbjEdge* edge1 = cur_node->fanin_edge(1);
    if ( flow(edge0) ) {
      const SbjNode* from_node = edge0->from();
      if ( !check_vmark2(from_node) ) {
	bool stat = dfs(from_node, 1);
	if ( stat ) {
	  clear_flow(edge0);
	  return true;
	}
      }
    }
    else if ( flow(edge1) ) {
      const SbjNode* from_node = edge1->from();
      if ( !check_vmark2(from_node) ) {
	bool stat = dfs(from_node, 1);
	if ( stat ) {
	  clear_flow(edge1);
	  return true;
	}
      }
    }
    else if ( !check_vmark2(cur_node) ) {
      bool stat = dfs_fanout(cur_node);
      if ( stat ) {
	return true;
      }
    }
  }
  else {
    if ( cur_node->is_logic() && !check_vmark1(cur_node) ) {
      const SbjEdge* edge0 = cur_node->fanin_edge(0);
      if ( flow(edge0) ) {
	const SbjNode* from_node = edge0->from();
	if ( !check_vmark2(from_node) ) {
	  bool stat = dfs(from_node, 1);
	  if ( stat ) {
	    clear_flow(edge0);
	    return true;
	  }
	}
      }
      const SbjEdge* edge1 = cur_node->fanin_edge(1);
      if ( flow(edge1) ) {
	const SbjNode* from_node = edge1->from();
	if ( !check_vmark2(from_node) ) {
	  bool stat = dfs(from_node, 1);
	  if ( stat ) {
	    clear_flow(edge0);
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
