﻿
/// @file libym_techmap/lutmap/AreaCover.cc
/// @brief DAG covering のヒューリスティック
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: WeightCover.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "AreaCover.h"
#include "YmNetworks/BdnMgr.h"
#include "Cut.h"
#include "MapRecord.h"


BEGIN_NAMESPACE_YM_LUTMAP

// コンストラクタ
AreaCover::AreaCover()
{
}

// デストラクタ
AreaCover::~AreaCover()
{
}

// @brief 面積最小化マッピングを行う．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] limit LUT の入力数
// @param[in] mode モード
//  - 0: fanout フロー, resub なし
//  - 1: weighted フロー, resub なし
//  - 2: fanout フロー, resub あり
//  - 3: weighted フロー, resub あり
// @param[out] mapnetwork マッピング結果
// @param[out] lut_num LUT数
// @param[out] depth 段数
void
AreaCover::operator()(const BdnMgr& sbjgraph,
		      ymuint limit,
		      ymuint mode,
		      LnGraph& mapnetwork,
		      ymuint& lut_num,
		      ymuint& depth)
{
  mMode = mode;

  // カットを列挙する．
  mCutHolder.enum_cut(sbjgraph, limit);

  // 最良カットを記録する．
  MapRecord maprec;
  record_cuts(sbjgraph, limit, maprec);

  if ( mode & 2 ) {
    // cut resubstituion
    mCutResub(sbjgraph, mCutHolder, maprec);
  }

  // 最終的なネットワークを生成する．
  maprec.gen_mapgraph(sbjgraph, mapnetwork, lut_num, depth);
}

// @brief best cut の記録を行う．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] limit LUT の入力数
// @param[out] maprec マッピング結果を記録するオブジェクト
void
AreaCover::record_cuts(const BdnMgr& sbjgraph,
		       ymuint limit,
		       MapRecord& maprec)
{
  ymuint n = sbjgraph.max_node_id();
  mBestCost.clear();
  mBestCost.resize(n);
  mWeight.resize(limit);

  maprec.init(sbjgraph);

  // 入力のコストを設定
  const BdnNodeList& input_list = sbjgraph.input_list();
  for (BdnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const BdnNode* node = *p;
    maprec.set_cut(node, NULL);
    mBestCost[node->id()] = 0.0;
  }

  // 論理ノードのコストを入力側から計算
  vector<const BdnNode*> snode_list;
  sbjgraph.sort(snode_list);
  for (vector<const BdnNode*>::const_iterator p = snode_list.begin();
       p != snode_list.end(); ++ p) {
    const BdnNode* node = *p;

    double min_cost = DBL_MAX;
    const Cut* best_cut = NULL;
    const CutList& cut_list = mCutHolder.cut_list(node);
    for (CutListIterator p = cut_list.begin();
	 p != cut_list.end(); ++ p) {
      const Cut* cut = *p;

      ymuint ni = cut->input_num();
      bool ng = false;
      for (ymuint i = 0; i < ni; ++ i) {
	const BdnNode* inode = cut->input(i);
	if ( mBestCost[inode->id()] == DBL_MAX ) {
	  ng = true;
	  break;
	}
      }
      if ( ng ) continue;

      if ( mMode & 1 ) {
	// ファンアウトモード
	for (ymuint i = 0; i < ni; ++ i) {
	  const BdnNode* inode = cut->input(i);
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

      double cur_cost = 1.0;
      for (ymuint i = 0; i < ni; ++ i) {
	const BdnNode* inode = cut->input(i);
	cur_cost += mBestCost[inode->id()] * mWeight[i];
      }
      if ( min_cost > cur_cost ) {
	min_cost = cur_cost;
	best_cut = cut;
      }
    }
    ASSERT_COND(min_cost < DBL_MAX );
    ASSERT_COND( best_cut != NULL );
    maprec.set_cut(node, best_cut);
    mBestCost[node->id()] = min_cost;
  }
}

// node から各入力にいたる経路の重みを計算する．
void
AreaCover::calc_weight(const BdnNode* node,
		       const Cut* cut,
		       double cur_weight)
{
  for ( ; ; ) {
    for (ymuint i = 0; i < cut->input_num(); ++ i) {
      if ( cut->input(i) == node ) {
	// node は cut の葉だった．
	if  ( !node->pomark() ) {
	  mWeight[i] += cur_weight;
	}
	return;
      }
    }
    const BdnNode* inode0 = node->fanin0();
    double cur_weight0 = cur_weight / inode0->fanout_num();
    calc_weight(inode0, cut, cur_weight0);
    node = node->fanin1();
    cur_weight /= node->fanout_num();
  }
}

END_NAMESPACE_YM_LUTMAP
