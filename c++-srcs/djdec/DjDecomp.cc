
/// @file DjDecomp.cc
/// @brief DjDecomp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "DjDecomp.h"
#include "DgMgr.h"
#include "DgEdge.h"


BEGIN_NAMESPACE_DG

// @brief BDD タイプのノードを disjoint decomposition する．
void
DjDecomp::decomp(
  const BnNetwork& src_network ///< [in] 元のネットワーク
)
{
  unordered_map<SizeType, SizeType> id_map;

  // ポートの情報をコピーする．
  make_skelton_copy(src_network, id_map);

  for ( auto& dff: src_network.dff_list() ) {
    copy_dff(dff, id_map);
  }

  // 論理ノードを分解しつつコピーする．
  for ( auto& src_node: src_network.logic_list() ) {
    // ファンインのノード番号を求める．
    SizeType dst_id{BNET_NULLID};
    if ( src_node.type() == BnNodeType::Bdd ) {
      // disjoint 分解を行う．
      DgMgr dgmgr;
      auto root = dgmgr.make_dg(src_node.bdd());
      // 本体を作る．
      SizeType ni = src_node.fanin_num();
      mInputList.clear();
      mInputList.reserve(ni);
      for ( SizeType src_id: src_node.fanin_id_list() ) {
	ASSERT_COND( id_map.count(src_id) > 0 );
	SizeType dst_id = id_map.at(src_id);
	mInputList.push_back(dst_id);
      }
      dst_id = make_network(root);
    }
    else {
      // 単純にコピーする．
      dst_id = copy_logic(src_node, src_network, id_map);
    }
    id_map.emplace(src_node.id(), dst_id);
  }

  // 出力を作る．
  for ( auto& src_node: src_network.output_list() ) {
    copy_output(src_node, id_map);
  }
}

// @brief edge に対応するネットワークを作る．
SizeType
DjDecomp::make_network(
  DgEdge edge
)
{
  if ( edge.is_zero() ) {
    return new_logic_primitive({}, BnNodeType::C0, {});
  }
  if ( edge.is_one() ) {
    return new_logic_primitive({}, BnNodeType::C1, {});
  }

  auto node = edge.node();
  auto inv = edge.inv();

  if ( node->is_lit() ) {
    auto id = node->top();
    ASSERT_COND( 0 <= id && id < mInputList.size() );
    if ( inv ) {
      return new_logic_primitive({}, BnNodeType::Not, {mInputList[id]});
    }
    else {
      return mInputList[id];
    }
  }

  auto nc = node->child_num();
  vector<SizeType> fanin_id_list(nc);
  for ( SizeType i = 0; i < nc; ++ i ) {
    fanin_id_list[i] = make_network(node->child(i));
  }
  if ( node->is_or() ) {
    if ( inv ) {
      return new_logic_primitive({}, BnNodeType::Nor, fanin_id_list);
    }
    else {
      return new_logic_primitive({}, BnNodeType::Or, fanin_id_list);
    }
  }
  if ( node->is_xor() ) {
    if ( inv ) {
      return new_logic_primitive({}, BnNodeType::Xnor, fanin_id_list);
    }
    else {
      return new_logic_primitive({}, BnNodeType::Xor, fanin_id_list);
    }
  }
  if ( node->is_cplx() ) {
    auto lf = node->local_func() ^ inv;
    return new_logic_bdd({}, lf, fanin_id_list);
  }
  ASSERT_NOT_REACHED;
  return BNET_NULLID;
}

END_NAMESPACE_DG
