
/// @file libym_techmap/PatMatcher.cc
/// @brief PatMatcher の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "PatMatcher.h"
#include "ym_techmap/SbjGraph.h"
#include "PatGraph.h"


BEGIN_NAMESPACE_YM_TECHMAP

// @brief コンストラクタ
PatMatcher::PatMatcher()
{
}

// @brief デストラクタ
PatMatcher::~PatMatcher()
{
}

// @brief パタンマッチングを行う．
// @param[in] sbj_root サブジェクトグラフの根のノード
// @param[in] pat_graph パタングラフ
// @param[out] input_map 入力のマッピング
// @retval true マッチした．
// @retval false マッチしなかった．
// @note input_map の中身は (SbjNode->i() << 1) | pol
bool
PatMatcher::operator()(const SbjNode* sbj_root,
		       const PatGraph& pat_graph,
		       vector<ymuint>& input_map)
{
  ymuint nn = pat_graph.node_num();
  mSbjMap.clear();
  mSbjMap.resize(nn, NULL);
  mInvMap.clear();
  mInvMap.resize(nn, false);
  mPatMap.clear();

  // 根のノードを調べる．
  ymuint root_id = pat_graph.root_id();
  switch ( pat_graph.node_type(root_id) ) {
  case PatGraph::kInput:
    // これはなんでも OK
    break;

  case PatGraph::kAnd:
    if ( !sbj_root->is_and() ) {
      // 型が違う．
      return false;
    }
    break;

  case PatGraph::kXor:
    if ( !sbj_root->is_xor() ) {
      // 型が違う．
      return false;
    }
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  bind(sbj_root, root_id, false);

  // 各枝の入力と出力の対応を調べる．
  ymuint ne = pat_graph.edge_num();
  for (ymuint i = 0; i < ne; ++ i) {
    const PatEdge& e = pat_graph.edge(i);
    ymuint to_id = e.to_id();
    ymuint from_id = e.from_id();
    ymuint f_pos = e.fanin_pos();
    const SbjNode* to_node = mSbjMap[to_id];
    const SbjNode* from_node = to_node->fanin(f_pos);
    bool iinv = to_node->fanin_inv(f_pos);
    bool inv = false;
    switch ( pat_graph.node_type(from_id) ) {
    case PatGraph::kInput:
      // どんな型でも OK
      // 極性が違っても OK
      inv =  e.inverted() ^ iinv;
      break;

    case PatGraph::kAnd:
      if ( !from_node->is_and() ) {
	// 型が違う
	return false;
      }
      if ( e.inverted() != iinv ) {
	// 極性が違う
	return false;
      }
      break;

    case PatGraph::kXor:
      if ( !from_node->is_xor() ) {
	// 型が違う
	return false;
      }
      if ( e.inverted() != iinv ) {
	// 極性が違う
	return false;
      }
      break;

    default:
      assert_not_reached(__FILE__, __LINE__);
      break;
    }
    if ( !bind(from_node, from_id, inv) ) {
      return false;
    }
  }

  ymuint ni = pat_graph.input_num();
  input_map.clear();
  input_map.resize(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    input_map[i] = (mSbjMap[i]->id() << 1) | (mInvMap[i] ? 1 : 0);
  }
  return true;
}

// @brief サブジェクトノードとパタンノードをバインドする．
// @param[in] sbj_node サブジェクトノード
// @param[in] pat_id パタンノードのID
// @param[in] inv 反転フラグ
// @retval true バインドが成功した．
// @retval false バインドが失敗した．
bool
PatMatcher::bind(const SbjNode* sbj_node,
		 ymuint pat_id,
		 bool inv)
{
  if ( mSbjMap[pat_id] != NULL ) {
    if ( mSbjMap[pat_id] != sbj_node ) {
      return false;
    }
    if ( mInvMap[pat_id] != inv ) {
      return false;
    }
  }
  else {
    mSbjMap[pat_id] = sbj_node;
    mInvMap[pat_id] = inv;
  }

  hash_map<ymuint, ymuint>::iterator p = mPatMap.find(sbj_node->id());
  if ( p != mPatMap.end() ) {
    if ( p->second != pat_id ) {
      return false;
    }
  }
  else {
    mPatMap.insert(make_pair(sbj_node->id(), pat_id));
  }

  return true;
}

END_NAMESPACE_YM_TECHMAP
