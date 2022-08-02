
/// @file Bdd2Aig.cc
/// @brief Bdd2Aig の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "Bdd2Aig.h"
#include "FraigMgr.h"


BEGIN_NAMESPACE_FRAIG

// @brief BDDに対応するノード(木)を作る．
FraigHandle
FraigMgr::make_bdd(
  const Bdd& func,
  const vector<FraigHandle>& inputs
)
{
  Bdd2Aig bdd2aig{*this};
  return bdd2aig.conv(func, inputs);
}


//////////////////////////////////////////////////////////////////////
// クラス Bdd2Aig
//////////////////////////////////////////////////////////////////////

// @brief BDD を AIG に変換する．
FraigHandle
Bdd2Aig::conv(
  const Bdd& bdd,
  const vector<FraigHandle>& inputs
)
{
  SizeType root_edge;
  auto node_list = bdd.node_info(root_edge);
  // インデックスごとのノードリストを作る．
  SizeType max_index = 0;
  for ( auto node: node_list ) {
    if ( max_index < node.index ) {
      max_index = node.index;
    }
  }
  ++ max_index;
  vector<vector<BddInfo>> indexed_node_list(max_index);
  for ( auto node: node_list ) {
    indexed_node_list[node.index].push_back(node);
  }
  // 下位のインデックスから AIG を作る．
  mAigMap.clear();
  mAigMap.resize(node_list.size() + 1);
  for ( SizeType i = 0; i < max_index; ++ i ) {
    auto cedge = inputs[max_index - i - 1];
    for ( auto node: indexed_node_list[max_index - i - 1] ) {
      auto r0 = edge2aig(node.edge0);
      auto r1 = edge2aig(node.edge1);
      auto r = mMgr.make_mux(cedge, r0, r1);
      mAigMap[node.id] = r;
    }
  }
  return edge2aig(root_edge);
}

// @brief 枝をAIGに変換する．
FraigHandle
Bdd2Aig::edge2aig(
  SizeType edge
)
{
  if ( edge == 0 ) {
    return FraigHandle::zero();
  }
  if ( edge == 1 ) {
    return FraigHandle::one();
  }
  SizeType node = BddInfo::edge2node(edge);
  bool inv = BddInfo::edge2inv(edge);
  return mAigMap[node] ^ inv;
}

END_NAMESPACE_FRAIG
