﻿
/// @file DelayCover.cc
/// @brief DelayCover の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "DelayCover.h"
#include "LnGraph.h"
#include "Cut.h"
#include "CutHolder.h"
#include "MapRecord.h"


BEGIN_NAMESPACE_YM_LUTMAP

// コンストラクタ
DelayCover::DelayCover()
{
}

// デストラクタ
DelayCover::~DelayCover()
{
}

// @brief 遅延最小化マッピングを行う．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] limit LUT の入力数
// @param[in] slack 最小段数に対するスラック
// @param[in] mode モード
//  - 0: fanout フロー, resub なし
//  - 1: weighted フロー, resub なし
//  - 2: fanout フロー, resub あり
//  - 3: weighted フロー, resub あり
// @param[out] map_network マッピング結果
// @param[out] lut_num LUT数
// @param[out] depth 段数
void
DelayCover::operator()(const SbjGraph& sbjgraph,
		       ymuint limit,
		       ymuint slack,
		       ymuint mode,
		       LnGraph& map_network,
		       ymuint& lut_num,
		       ymuint& depth)
{
  mMode = mode;

  // カットを列挙する．
  mCutHolder.enum_cut(sbjgraph, limit);

  // 最良カットを記録する．
  MapRecord maprec;
  record_cuts(sbjgraph, limit, slack, maprec);

  if ( mode & 2 ) {
    // cut resubstituion
    mCutResub(sbjgraph, mCutHolder, maprec, slack);
  }

  // 最終的なネットワークを生成する．
  maprec.gen_mapgraph(sbjgraph, map_network, lut_num, depth);
}

// @brief best cut の記録を行う．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] limit LUT の入力数
// @param[in] slack 最小段数に対するスラック
// @param[out] maprec マッピング結果を記録するオブジェクト
void
DelayCover::record_cuts(const SbjGraph& sbjgraph,
			ymuint limit,
			ymuint slack,
			MapRecord& maprec)
{
  ymuint n = sbjgraph.max_node_id();

  maprec.init(sbjgraph);

  // 作業領域の初期化
  mNodeInfo.clear();
  mNodeInfo.resize(n);
  for (ymuint i = 0; i < n; ++ i) {
    mNodeInfo[i].mCostList.set_mgr(&mCostMgr);
  }
  mWeight.resize(limit);
  mIcostLists.resize(limit);

  // 入力のコストの設定
  ymuint ni = sbjgraph.input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* node = sbjgraph.input(i);
    NodeInfo& t = mNodeInfo[node->id()];
    t.mCostList.insert(nullptr, 0, 0.0);
    t.mMinDepth = 0;
  }

  // 各ノードごとにカットを記録
  vector<const SbjNode*> snode_list;
  sbjgraph.sort(snode_list);
  for (vector<const SbjNode*>::const_iterator p = snode_list.begin();
       p != snode_list.end(); ++ p) {
    const SbjNode* node = *p;
    record(node);
  }

  // 最小段数の最大値をもとめる．
  ymuint no = sbjgraph.output_num();
  vector<const SbjNode*> onode_list;
  onode_list.reserve(no);
  int min_depth = 0;
  for (ymuint i = 0; i < no; ++ i) {
    const SbjNode* onode = sbjgraph.output(i);
    const SbjNode* node = onode->fanin(0);
    if ( node == nullptr) continue;
    if ( node->is_logic() ) {
      onode_list.push_back(node);
    }
    NodeInfo& node_info = mNodeInfo[node->id()];
    int depth = node_info.mMinDepth;
    if ( min_depth < depth ) {
      min_depth = depth;
    }
  }

  // それに slack を足したものが制約となる．
  min_depth += slack;
  for (ymuint i = 0; i < no; ++ i) {
    const SbjNode* node = sbjgraph.output(i);
    mNodeInfo[node->id()].mReqDepth = min_depth;
  }

  // 要求された段数制約を満たす中でコスト最小の解を選ぶ．
  for (vector<const SbjNode*>::reverse_iterator p = snode_list.rbegin();
       p != snode_list.rend(); ++ p) {
    const SbjNode* node = *p;
    select(node, maprec);
  }
}

// node のカットを選択する．
void
DelayCover::record(const SbjNode* node)
{
  int min_depth = INT_MAX;
  NodeInfo& t = mNodeInfo[node->id()];
  const CutList& cut_list = mCutHolder.cut_list(node);
  for (CutListIterator p = cut_list.begin();
       p != cut_list.end(); ++ p) {
    const Cut* cut = *p;
    ymuint ni = cut->input_num();

    if ( mMode & 1 ) {
      // ファンアウトモード
      for (ymuint i = 0; i < ni; ++ i) {
	const SbjNode* inode = cut->input(i);
	mWeight[i] = 1.0 / inode->fanout_num();
      }
    }
    else {
      // フローモード
      for (ymuint i = 0; i < ni; ++ i) {
	mWeight[i] = 0.0;
      }
      calc_weight(node, cut, 1.0);
    }

    int max_input_depth = 0;
    for (ymuint i = 0; i < ni; ++ i) {
      const SbjNode* inode = cut->input(i);
      NodeInfo& u = mNodeInfo[inode->id()];
      if ( max_input_depth < u.mMinDepth ) {
	max_input_depth = u.mMinDepth;
      }
      mIcostLists[i] = u.mCostList.begin();
    }

    int cur_depth = max_input_depth + 1;
    if ( min_depth > cur_depth ) {
      min_depth = cur_depth;
    }
    // mIcostLists から解を作る．
    for ( ; ; ) {
      // 各入力のなかでもっとも深い値を求める．
      int idepth = 0;
      double area = 1.0;
      bool empty = false;
      for (ymuint i = 0; i < ni; ++ i) {
	if ( mIcostLists[i].is_end() ) {
	  empty = true;
	  break;
	}
	ADCost<double>* cost = *mIcostLists[i];
	if ( idepth < cost->depth() ) {
	  idepth = cost->depth();
	}
	area += cost->area() * mWeight[i];
      }
      if ( empty ) {
	break;
      }
      int depth = idepth + 1;

      // (depth, area) を登録
      t.mCostList.insert(cut, depth, area);

      // 深さが idepth に等しい解を次に進める．
      for (ymuint i = 0; i < ni; ++ i) {
	ADCost<double>* cost = *mIcostLists[i];
	if ( cost->depth() == idepth ) {
	  ++ mIcostLists[i];
	}
      }
    }
  }
  t.mMinDepth = min_depth;
}

// node から各入力にいたる経路の重みを計算する．
void
DelayCover::calc_weight(const SbjNode* node,
			const Cut* cut,
			double cur_weight)
{
  for ( ; ; ) {
    for (ymuint i = 0; i < cut->input_num(); ++ i) {
      if ( cut->input(i) == node ) {
	if  ( !node->pomark() ) {
	  mWeight[i] += cur_weight;
	}
	return;
      }
    }
    const SbjNode* inode0 = node->fanin(0);
    calc_weight(inode0, cut, cur_weight / inode0->fanout_num());
    node = node->fanin(1);
    cur_weight /= node->fanout_num();
  }
}

// node のカットを選択する．
void
DelayCover::select(const SbjNode* node,
		   MapRecord& maprec)
{
  NodeInfo& t = mNodeInfo[node->id()];
  int rd = t.mReqDepth;
  if ( rd == 0 ) {
    // このノードは必要ではない．
    return;
  }
  const Cut* cut = nullptr;
  for (ADCostIterator<double> p = t.mCostList.begin();
       !p.is_end(); ++ p) {
    ADCost<double>* cost = *p;
    if ( cost->depth() <= rd ) {
      cut = cost->cut();
      break;
    }
  }
  ASSERT_COND(cut );
  maprec.set_cut(node, cut);
  -- rd;
  for (ymuint i = 0; i < cut->input_num(); ++ i) {
    const SbjNode* inode = cut->input(i);
    NodeInfo& u = mNodeInfo[inode->id()];
    if ( u.mReqDepth == 0 || u.mReqDepth > rd ) {
      u.mReqDepth = rd;
    }
  }
}

END_NAMESPACE_YM_LUTMAP
