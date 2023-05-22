
/// @file PatMatcher.cc
/// @brief PatMatcher の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "PatMatcher.h"
#include "SbjGraph.h"
#include "ym/ClibCellLibrary.h"
#include "ym/ClibPatGraph.h"
#include "Cut.h"


BEGIN_NAMESPACE_CELLMAP

//////////////////////////////////////////////////////////////////////
// クラス PatMatcher
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pat_mgr パタンを管理するクラス
PatMatcher::PatMatcher(const ClibCellLibrary& cell_library) :
  mLibrary(cell_library),
  mSbjMap(mLibrary.pg_node_num(), nullptr),
  mInvMap(mLibrary.pg_node_num(), false)
{
}

// @brief デストラクタ
PatMatcher::~PatMatcher()
{
}

// @brief パタンマッチングを行う．
// @param[in] sbj_root サブジェクトグラフの根のノード
// @param[in] pat_graph パタングラフ
// @param[in] match マッチング結果を格納する変数
// @retval true マッチングが成功した
// @retval false マッチングが失敗した．
bool
PatMatcher::operator()(const SbjNode* sbj_root,
		       const ClibPatGraph& pat_graph,
		       Cut& match)
{
  // 根のノードを調べる．
  auto root_id = pat_graph.root_id();
  switch ( mLibrary.pg_node_type(root_id) ) {
  case ClibPatType::Input:
    // これはなんでも OK
    break;

  case ClibPatType::And:
    if ( !sbj_root->is_and() ) {
      // 型が違う．
      return false;
    }
    break;

  case ClibPatType::Xor:
    if ( !sbj_root->is_xor() ) {
      // 型が違う．
      return false;
    }
    break;

  default:
    ASSERT_NOT_REACHED;
    break;
  }
  bind(sbj_root, root_id, false);

  bool success = false;

  // 各枝の入力と出力の対応を調べる．
  auto ne = pat_graph.edge_num();
  for ( SizeType i = 0; i < ne; ++ i) {
    auto edge_id = pat_graph.edge(i);
    auto to_id = mLibrary.pg_edge_to(edge_id);
    auto from_id = mLibrary.pg_edge_from(edge_id);
    auto f_pos = mLibrary.pg_edge_pos(edge_id);
    const SbjNode* to_node = mSbjMap[to_id];
    ASSERT_COND( to_node->is_logic() );
    const SbjNode* from_node = to_node->fanin(f_pos);
    bool iinv = to_node->fanin_inv(f_pos);
    bool inv = false;
    switch ( mLibrary.pg_node_type(from_id) ) {
    case ClibPatType::Input:
      // どんな型でも OK
      // 極性が違っても OK
      inv =  mLibrary.pg_edge_inv(edge_id) ^ iinv;
      break;

    case ClibPatType::And:
      if ( !from_node->is_and() ) {
	// 型が違う
	goto end;
      }
      if ( mLibrary.pg_edge_inv(edge_id) != iinv ) {
	// 極性が違う
	goto end;
      }
      break;

    case ClibPatType::Xor:
      if ( !from_node->is_xor() ) {
	// 型が違う
	goto end;
      }
      if ( mLibrary.pg_edge_inv(edge_id) != iinv ) {
	// 極性が違う
	goto end;
      }
      break;

    default:
      ASSERT_NOT_REACHED;
      break;
    }
    if ( !bind(from_node, from_id, inv) ) {
      goto end;
    }
  }

  { // 成功した．
    success = true;
    auto ni = pat_graph.input_num();
    match.resize(ni);
    for ( SizeType i = 0; i < ni; ++ i) {
      auto node_id = mLibrary.pg_input_node(i);
      match.set_leaf(i, mSbjMap[node_id], mInvMap[node_id]);
    }
  }

 end:// 後始末．

  for ( auto id: mClearQueue ) {
    mSbjMap[id] = nullptr;
    mInvMap[id] = false;
  }
  mClearQueue.clear();
  mPatMap.clear();

  return success;
}

// @brief サブジェクトノードとパタンノードをバインドする．
// @param[in] sbj_node サブジェクトノード
// @param[in] pat_id パタンノードのID
// @param[in] inv 反転フラグ
// @retval true バインドが成功した．
// @retval false バインドが失敗した．
bool
PatMatcher::bind(const SbjNode* sbj_node,
		 SizeType pat_id,
		 bool inv)
{
  if ( mSbjMap[pat_id] != nullptr ) {
    if ( mSbjMap[pat_id] != sbj_node ) {
      // パタンノード(pat_id)が既に他のノードにバインドしていた．
      return false;
    }
    if ( mInvMap[pat_id] != inv ) {
      // 既に逆極性でバインドしていた．
      return false;
    }
  }
  else {
    mSbjMap[pat_id] = sbj_node;
    mInvMap[pat_id] = inv;
    mClearQueue.push_back(pat_id);
  }

  SizeType pat_id1;
  if ( mPatMap.count(sbj_node->id()) > 0 ) {
    pat_id1 = mPatMap.at(sbj_node->id());
    if ( pat_id1 != pat_id ) {
      // SbjNode が既に他のノードにバインドしていた．
      return false;
    }
  }
  else {
    mPatMap.emplace(sbj_node->id(), pat_id);
  }
  return true;
}

END_NAMESPACE_CELLMAP
