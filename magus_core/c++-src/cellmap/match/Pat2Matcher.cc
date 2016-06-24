
/// @file Pat2Matcher.cc
/// @brief Pat2Matcher の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "Pat2Matcher.h"
#include "Cut.h"


BEGIN_NAMESPACE_CELLMAP

//////////////////////////////////////////////////////////////////////
// クラス Pat2Matcher
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pat_mgr パタンを管理するクラス
Pat2Matcher::Pat2Matcher(const CellLibrary& cell_library) :
  mLibrary(cell_library),
  mSbjMap(mLibrary.pg2_node_num(), nullptr),
  mInvMap(mLibrary.pg2_node_num(), false),
  mLeafNodeArray(mLibrary.pg2_max_input()),
  mLeafInvArray(mLibrary.pg2_max_input())
{
}

// @brief デストラクタ
Pat2Matcher::~Pat2Matcher()
{
}

// @brief パタンマッチングを行う．
// @param[in] sbj_root サブジェクトグラフの根のノード
// @param[in] pat_graph パタングラフ
// @retval true マッチした．
// @retval false マッチしなかった．
bool
Pat2Matcher::operator()(const BdnNode* sbj_root,
			const CellPat2Graph& pat_graph)
{
  // 根のノードを調べる．
  ymuint root_id = pat_graph.root_id();
  switch ( mLibrary.pg2_node_type(root_id) ) {
  case kCellPatInput:
    // これはなんでも OK
    break;

  case kCellPatAnd:
    if ( !sbj_root->is_and() ) {
      // 型が違う．
      return false;
    }
    break;

  case kCellPatXor:
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
  ymuint ne = pat_graph.edge_num();
  for (ymuint i = 0; i < ne; ++ i) {
    ymuint edge_id = pat_graph.edge(i);
    ymuint to_id = mLibrary.pg2_edge_to(edge_id);
    ymuint from_id = mLibrary.pg2_edge_from(edge_id);
    ymuint f_pos = mLibrary.pg2_edge_pos(edge_id);
    const BdnNode* to_node = mSbjMap[to_id];
    ASSERT_COND( to_node->is_logic() );
    const BdnNode* from_node = to_node->fanin(f_pos);
    bool iinv = to_node->fanin_inv(f_pos);
    bool inv = false;
    switch ( mLibrary.pg2_node_type(from_id) ) {
    case kCellPatInput:
      // どんな型でも OK
      // 極性が違っても OK
      inv =  mLibrary.pg2_edge_inv(edge_id) ^ iinv;
      break;

    case kCellPatAnd:
      if ( !from_node->is_and() ) {
	// 型が違う
	goto end;
      }
      if ( mLibrary.pg2_edge_inv(edge_id) != iinv ) {
	// 極性が違う
	goto end;
      }
      break;

    case kCellPatXor:
      if ( !from_node->is_xor() ) {
	// 型が違う
	goto end;
      }
      if ( mLibrary.pg2_edge_inv(edge_id) != iinv ) {
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
    success= true;
    ymuint ni = pat_graph.input_num();
    for (ymuint i = 0; i < ni; ++ i) {
      ymuint node_id = mLibrary.pg2_input_node(i);
      mLeafNodeArray[i] = mSbjMap[node_id];
      mLeafInvArray[i] = mInvMap[node_id];
    }
  }

 end:// 後始末．

  for (vector<ymuint>::iterator p = mClearQueue.begin();
       p != mClearQueue.end(); ++ p) {
    ymuint id = *p;
    mSbjMap[id] = nullptr;
    mInvMap[id] = false;
  }
  mClearQueue.clear();
  mPatMap.clear();

  return success;
}

// @brief マッチングを行なう下請け関数
// @param[in] pat_graph パタングラフ
// @param[in] pos 枝番号
bool
Pat2Matcher::match_sub(const CellPat2Graph& pat_graph,
		       ymuint pos)
{
  ymuint edge_id = pat_graph.edge(pos);
  ymuint to_id = mLibrary.pg2_edge_to(edge_id);
  ymuint from_id = mLibrary.pg2_edge_from(edge_id);
  ymuint f_pos = mLibrary.pg2_edge_pos(edge_id);
  ymuint ni = mLibrary.pg2_fanin_num(to_id);
  const BdnNode* to_node = mSbjMap[to_id];
  ASSERT_COND( to_node->is_logic() );

  // to_node から ni 入力のクラスタを切り出す．
  for ( ; ; ) {
    const BdnNode* from_node = to_node->fanin(f_pos);
    bool iinv = to_node->fanin_inv(f_pos);
    bool inv = false;
    switch ( mLibrary.pg2_node_type(from_id) ) {
    case kCellPatInput:
      // どんな型でも OK
      // 極性が違っても OK
      inv =  mLibrary.pg2_edge_inv(edge_id) ^ iinv;
      break;

    case kCellPatAnd:
      if ( !from_node->is_and() ) {
	// 型が違う
	goto end;
      }
      if ( mLibrary.pg2_edge_inv(edge_id) != iinv ) {
	// 極性が違う
	goto end;
      }
      break;

    case kCellPatXor:
      if ( !from_node->is_xor() ) {
	// 型が違う
	goto end;
      }
      if ( mLibrary.pg2_edge_inv(edge_id) != iinv ) {
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

    ymuint pos1 = pos + 1;
    if ( pos1 < pat_graph.edge_num() ) {
      bool stat = match_sub(pat_graph, pos + 1);
      if ( !stat ) {
	goto end;
      }
    }
    else { // 成功した．
      success= true;
      ymuint ni = pat_graph.input_num();
      for (ymuint i = 0; i < ni; ++ i) {
	ymuint node_id = mLibrary.pg2_input_node(i);
	mLeafNodeArray[i] = mSbjMap[node_id];
	mLeafInvArray[i] = mInvMap[node_id];
      }
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
Pat2Matcher::bind(const BdnNode* sbj_node,
		  ymuint pat_id,
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

  hash_map<ymuint, ymuint>::iterator p = mPatMap.find(sbj_node->id());
  if ( p != mPatMap.end() ) {
    if ( p->second != pat_id ) {
      // SbjNode が既に他のノードにバインドしていた．
      return false;
    }
  }
  else {
    mPatMap.insert(make_pair(sbj_node->id(), pat_id));
  }
  return true;
}

END_NAMESPACE_CELLMAP
