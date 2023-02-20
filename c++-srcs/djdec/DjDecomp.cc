
/// @file DjDecomp.cc
/// @brief DjDecomp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "DjDecomp.h"
#include "DgMgr.h"
#include "DgEdge.h"
#include "ym/BnNodeMap.h"


BEGIN_NAMESPACE_DG

// @brief BDD タイプのノードを disjoint decomposition する．
void
DjDecomp::decomp(
  const BnNetwork& src_network ///< [in] 元のネットワーク
)
{
  // ポートの情報をコピーする．
  auto node_map = make_skelton_copy(src_network);

  for ( auto dff: src_network.dff_list() ) {
    copy_dff(dff, node_map);
  }

  // 論理ノードを分解しつつコピーする．
  for ( auto src_node: src_network.logic_list() ) {
    // ファンインのノード番号を求める．
    BnNode dst_node;
    if ( src_node.type() == BnNodeType::Bdd ) {
      // disjoint 分解を行う．
      DgMgr dgmgr;
      auto root = dgmgr.make_dg(src_node.bdd());
      // 本体を作る．
      SizeType ni = src_node.fanin_num();
      mInputList.clear();
      mInputList.reserve(ni);
      for ( auto src_inode: src_node.fanin_list() ) {
	SizeType src_id = src_inode.id();
	ASSERT_COND( node_map.is_in(src_id) > 0 );
	auto dst_inode = node_map.get(src_id);
	mInputList.push_back(dst_inode);
      }
      dst_node = make_network(root);
    }
    else {
      // 単純にコピーする．
      dst_node = copy_logic(src_node, node_map);
    }
    node_map.put(src_node.id(), dst_node);
  }

  // 出力を作る．
  for ( auto src_node: src_network.output_list() ) {
    copy_output(src_node, node_map);
  }
}

// @brief edge に対応するネットワークを作る．
BnNode
DjDecomp::make_network(
  DgEdge edge
)
{
  if ( edge.is_zero() ) {
    return new_logic_primitive({}, PrimType::C0, {});
  }
  if ( edge.is_one() ) {
    return new_logic_primitive({}, PrimType::C1, {});
  }

  auto node = edge.node();
  auto inv = edge.inv();

  if ( node->is_lit() ) {
    auto id = node->top();
    ASSERT_COND( 0 <= id && id < mInputList.size() );
    if ( inv ) {
      return new_logic_primitive({}, PrimType::Not, {mInputList[id]});
    }
    else {
      return mInputList[id];
    }
  }

  auto nc = node->child_num();
  vector<BnNode> fanin_list(nc);
  for ( SizeType i = 0; i < nc; ++ i ) {
    fanin_list[i] = make_network(node->child(i));
  }
  if ( node->is_or() ) {
    if ( inv ) {
      return new_logic_primitive({}, PrimType::Nor, fanin_list);
    }
    else {
      return new_logic_primitive({}, PrimType::Or, fanin_list);
    }
  }
  if ( node->is_xor() ) {
    if ( inv ) {
      return new_logic_primitive({}, PrimType::Xnor, fanin_list);
    }
    else {
      return new_logic_primitive({}, PrimType::Xor, fanin_list);
    }
  }
  if ( node->is_cplx() ) {
    auto lf = node->local_func() ^ inv;
    return new_logic_bdd({}, lf, fanin_list);
  }
  ASSERT_NOT_REACHED;
  return BnNode{}; // 不正値
}

END_NAMESPACE_DG
