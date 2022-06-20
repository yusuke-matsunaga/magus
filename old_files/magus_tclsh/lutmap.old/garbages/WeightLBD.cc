﻿
/// @file magus/lutmap/WeightLBD.cc
/// @brief WeightLBD の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: WeightLBD.cc 1077 2007-12-03 10:18:47Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <magus_config.h>
#endif


#include <float.h>

#include "WeightLBD.h"
#include "SbjGraph.h"
#include "Cut.h"
#include "CutHolder.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

// コンストラクタ
WeightLBD::WeightLBD()
{
}

// デストラクタ
WeightLBD::~WeightLBD()
{
}

// @brief 深さ制約下での lower bound を求める．
// @param[in] cut_holder カットを保持するオブジェクト
// @param[in] slack 最小段数に対するスラック
// @return covering の結果の LUT 数
int
WeightLBD::operator()(const SbjGraph& sbjgraph,
		      const CutHolder& cut_holder,
		      size_t slack)
{
  size_t limit = cut_holder.limit();
  size_t n = sbjgraph.max_node_id();
  const SbjNodeList& input_list = sbjgraph.input_list();
  const SbjNodeList& output_list = sbjgraph.output_list();
  vector<SbjNode*> snode_list;
  sbjgraph.sort(snode_list);

  // 作業領域の初期化
  mNodeInfo.clear();
  mNodeInfo.resize(n);
  for (size_t i = 0; i < n; ++ i) {
    mNodeInfo[i].mCostList.set_mgr(&mCostMgr);
  }
  mWeight.resize(limit);

  // 入力のコストの設定
  for (SbjNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    SbjNode* node = *p;
    NodeInfo& t = mNodeInfo[node->id()];
    t.mCostList.insert(NULL, 0, 0.0);
    t.mMinDepth = 0;
  }

  // 各ノードごとにカットを記録
  for (vector<SbjNode*>::iterator p = snode_list.begin();
       p != snode_list.end(); ++ p) {
    SbjNode* node = *p;
    record(node, cut_holder);
  }

  // 最小段数をもとめる．
  int min_depth = 0;
  for (SbjNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    SbjNode* onode = *p;
    SbjNode* node = onode->fanin(0);
    if ( node ) {
      int depth = mNodeInfo[node->id()].mMinDepth;
      if ( min_depth < depth ) {
	min_depth = depth;
      }
    }
  }
  // それに slack を足したものが制約となる．
  min_depth += slack;

  double cost = 0.0;
  for (SbjNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    SbjNode* onode = *p;
    SbjNode* node = onode->fanin(0);
    if ( node ) {
      cost += count(node, min_depth);
    }
  }
  
  return static_cast<int>(ceil(cost));
}

// node のカットを選択する．
void
WeightLBD::record(SbjNode* node,
		  const CutHolder& cut_holder)
{
  int min_depth = INT_MAX;
  NodeInfo& t = mNodeInfo[node->id()];
  const CutList& cut_list = cut_holder.cut_list(node);
  for (CutListIterator p = cut_list.begin();
       p != cut_list.end(); ++ p) {
    const Cut* cut = *p;
    size_t ni = cut->ni();
    bool ng = false;
    for (size_t i = 0; i < ni; ++ i) {
      if ( cut->input(i)->is_unselected() ) {
	ng = true;
	break;
      }
    }
    if ( ng ) continue;

    for (size_t i = 0; i < ni; ++ i) {
      mWeight[i] = 0.0;
    }
    calc_weight(node, cut, 1.0);

    int max_input_depth = 0;
    vector<ADCostIterator<double> > icost_lists(ni);
    for (size_t i = 0; i < ni; ++ i) {
      SbjNode* inode = cut->input(i);
      NodeInfo& u = mNodeInfo[inode->id()];
      if ( max_input_depth < u.mMinDepth ) {
	max_input_depth = u.mMinDepth;
      }
      icost_lists[i] = u.mCostList.begin();
    }
    int cur_depth = max_input_depth + 1;
    if ( min_depth > cur_depth ) {
      min_depth = cur_depth;
    }
    
    // icost_lists から解を作る．
    for ( ; ; ) {
      // 各入力のなかでもっとも深い値を求める．
      int idepth = 0;
      double area = 1.0;
      bool empty = false;
      for (size_t i = 0; i < ni; ++ i) {
	if ( icost_lists[i].is_end() ) {
	  empty = true;
	  break;
	}
	ADCost<double>* cost = *icost_lists[i];
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
      for (size_t i = 0; i < ni; ++ i) {
	ADCost<double>* cost = *icost_lists[i];
	if ( cost->depth() == idepth ) {
	  ++ icost_lists[i];
	}
      }
    }
  }
  t.mMinDepth = min_depth;
}
  
// node を根とする木のノード数を数える．
double
WeightLBD::count(SbjNode* node,
		 int req)
{
  if ( node->is_input() ) {
    return 0.0;
  }
  
  NodeInfo& t = mNodeInfo[node->id()];
  for (ADCostIterator<double> p = t.mCostList.begin();
       !p.is_end(); ++ p) {
    ADCost<double>* cost = *p;
    if ( cost->depth() <= req ) {
      return cost->area() / node->n_fanout();
    }
  }
  ASSERT_NOT_REACHED;
  return 0;
}

// node から各入力にいたる経路の重みを計算する．
void
WeightLBD::calc_weight(SbjNode* node,
		       const Cut* cut,
		       double cur_weight)
{
  for (size_t i = 0; i < cut->ni(); ++ i) {
    if ( cut->input(i) == node ) {
      mWeight[i] += cur_weight;
      return;
    }
  }

  SbjNode* inode0 = node->fanin(0);
  size_t fo0 = inode0->n_fanout();
  double cur_weight0 = cur_weight / fo0;
  calc_weight(inode0, cut, cur_weight0);

  SbjNode* inode1 = node->fanin(1);
  size_t fo1 = inode1->n_fanout();
  double cur_weight1 = cur_weight / fo1;
  calc_weight(inode1, cut, cur_weight1);
}

END_NAMESPACE_MAGUS_LUTMAP
