
/// @file NodeMark.cc
/// @brief NodeMark の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "NodeMark.h"
#include "DgNode.h"
#include "DgEdge.h"


BEGIN_NAMESPACE_DG

// @brief 再帰的にマークをつける．
void
NodeMark::mark_recur(
  DgEdge edge,
  int mval
)
{
  auto node = edge.node();
  int val = mark(node);
  if ( (val & mval) == 0 ) {
    val |= mval;
    set_mark(node, val);
    for ( SizeType i = 0; i < node->child_num(); ++ i ) {
      mark_recur(node->child(i), mval);
    }
  }
}

// @brief 再帰的にTFIマークをつける．
int
NodeMark::tfimark_recur(
  DgEdge edge
)
{
  auto node = edge.node();
  int val = tfimark(node);
  if ( val == 0 ) {
    val = mark(node);
    if ( val < 3 ) {
      for ( SizeType i = 0; i < node->child_num(); ++ i ) {
	SizeType v1 = tfimark_recur(node->child(i));
	if ( v1 == 3 ) {
	  val = 3;
	}
      }
    }
    set_tfimark(node, val);
  }
  return val;
}

// @brief マークに基づいて境界ノードに印をつける．
int
NodeMark::get_boundary(
  DgEdge edge,
  vector<DgNode*>& or_list,
  vector<DgNode*>& xor_list
)
{
  auto node = edge.node();
  if ( bmark(node) == 0 ) {
    if ( is_boundary(node) ) {
      set_bmark(node, 1);
    }
    else {
      set_bmark(node, 2);
      // ファンインのうち境界ノードの数
      SizeType n = 0;
      for ( SizeType i = 0; i < node->child_num(); ++ i ) {
	int val1 = get_boundary(node->child(i), or_list, xor_list);
	if ( val1 == 1 ) {
	  ++ n;
	}
      }
      if ( n >= 2 ) {
	if ( node->is_or() ) {
	  or_list.push_back(node);
	}
	else if ( node->is_xor() ) {
	  xor_list.push_back(node);
	}
      }
    }
  }
  return bmark(node);
}

// @brief 境界ノードを求める．
void
NodeMark::find_bnode(
  DgNode* node,
  vector<DgEdge>& inputs
)
{
  int val = bmark(node);
  if ( val != 3 ) {
    if ( val == 1 ) {
      inputs.push_back(DgEdge{node});
    }
    else {
      SizeType nc = node->child_num();
      for ( SizeType i = 0; i < nc; ++ i ) {
	auto cnode = node->child(i).node();
	find_bnode(cnode, inputs);
      }
    }
    set_bmark(node, 3);
  }
}

END_NAMESPACE_DG
