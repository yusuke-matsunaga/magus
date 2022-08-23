
/// @file DjDecomp.cc
/// @brief DjDecomp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "DjDecomp.h"
#include "DgMgr.h"


BEGIN_NAMESPACE_DG

// @brief BDD タイプのノードを disjoint decomposition する．
void
DjDecomp::decomp(
  const BnNetwork& src_network ///< [in] 元のネットワーク
)
{
  // ポートの情報をコピーする．
  auto id_map = mImpl->make_skelton_copy(src_network.mImpl);

  for ( auto id: src_network.logid_id_list() ) {
    auto& src_node = src_network.node(id);
    // ファンインのノード番号を求める．
    SizeType ni = src_node->input_num();
    mInputList.clear();
    mInputList.resize(ni);
    for ( SizeType i = 0; i < ni; ++ i ) {
      SizeType src_id = src_node->fanin_id(i);
      ASSERT_COND( id_map.count(src_id) > 0 );
      SizeType dst_id = id_map.at(src_id);
      mInputList[i] = dst_id;
    }
    SizeType dst_id{BNET_NULL_ID};
    if ( src_node->type() == BnNodeType::Bdd ) {
      // disjoint 分解を行う．
      auto root = dgmgr.make_dg(src_node.bdd());
      // 本体を作る．
      dst_id = make_network(root);
    }
    else {
      // 単純にコピーする．
      dst_id = mImpl->_copy_logic(src_node, src_network.mImpl, id_map);
    }
  }

  // 出力を作る．
  SizeType no = src_network.output_num();
  for ( SizeType i = 0; i < no; ++ i ) {
    SizeType src_id = src_network.output_id(i);
    SizeType src_fanin_id = src_network.output_src_id(i);
    ASSERT_COND( id_map.count(src_id) > 0 );
    ASSERT_COND( id_map.count(src_fanin_id) > 0 );
    SizeType dst_id = id_map.at(src_id);
    SizeType dst_fanin_id = id_map.at(src_fanin_id);
    set_output(dst_id, dst_fanin_id);
  }
}

// @brief edge に対応するネットワークを作る．
SizeType
DjDecomp::make_network(
  DgEdge edge
)
{
  if ( edge.is_zero() ) {
    return new_logic({}, BnNodeType::C0, {});
  }
  if ( edge.is_one() ) {
    return new_logic({}, BnNodeType::C1, {});
  }

  auto node = edge.node();
  auto inv = edge.inv();

  if ( node->is_lit() ) {
    auto id = node->top();
    ASSERT_COND( 0 <= id && id < mInputList.size() );
    return mInputList[id];
  }

  auto nc = node->child_num();
  vector<SizeType> fanin_id_list(nc);
  for ( SizeType i = 0; i < nc; ++ i ) {
    fanin_id_list[i] = make_network(node->child(i));
  }
  if ( node->is_or() ) {
    if ( inv ) {
      return new_logic({}, BnNodeType::Nor, fanin_id_list);
    }
    else {
      return new_logic({}, BnNodeType::Or, fanin_id_list);
    }
  }
  if ( node->is_xor() ) {
    if ( inv ) {
      return new_logic({}, BnNodeType::Xnor, fanin_id_list);
    }
    else {
      return new_logic({}, BnNodeType::Xor, fanin_id_list);
    }
  }
  if ( node->is_cplx() ) {
    auto lf = node->local_func() ^ inv;
    return new_logic({}, lf, fanin_id_list);
  }
  ASSERT_NOT_REACHED;
  return BNET_NULLID;
}

END_NAMESPACE_DG
