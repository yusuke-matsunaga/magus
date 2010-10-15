
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
		       vector<ymuint> input_map)
{
  mSbjMap.clear();
  mSbjMap.resize(pat_graph.node_num(), NULL);
  mInvMap.clear();
  mInvMap.resize(pat_graph.node_num(), false);
  mPatMap.clear();
  mPatMap.resize(max_node_id);

  bind(sbj_root, pat_graph.root_id(), false);
  ymuint ne = pat_graph.edge_num();
  for (ymuint i = 0; i < ne; ++ i) {
    const PatEdge& e = pat_graph.edge(i);
    ymuint to_id = e.to_id();
    ymuint from_id = e.from_id();
    ymuint fpos = e.fanin_pos();
    const SbjNode* to_node = SbjMap[to_id];
    const SbjNode* from_node = to_node->fanin(f_pos);
    ymuint fcode = from_node->fcode();
    bool false;
    switch ( pat_graph.node_type(from_id) ) {
    case PatGraph::kInput:

      break;

    case PatGraph::kAnd:
      if ( fcode & 4U ) {
	// 型が違う
	return false;
      }
      break;

    case PatGraph::kXor:
      if ( (fcode & 4U) == 0 ) {
	// 型が違う
	return false;
      }
      break;

    default:
      assert_not_reached(__FILE__, __LINE__);
      break;
    }
    if ( !bind(from_node, e.from_id(), inv) ) {
      return false;
    }
  }
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
  if ( mSbjMap[pat_id] != NULL && mSbjMap[pat_id] != sbj_node ) {
    return false;
  }
  mSbjMap[pat_id] = sbj_node;
  mInvMap[pat_id] = inv;
  mPatMap[sbj_node->id()] = pat_id;
}

END_NAMESPACE_YM_TECHMAP
