
/// @file AreaCover.cc
/// @brief AreCover の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2016, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "AreaCover.h"
#include "Cut.h"
#include "CutHolder.h"
#include "MapRecord.h"
#include "SbjGraph.h"
#include "SbjNode.h"


BEGIN_NAMESPACE_LUTMAP

// コンストラクタ
AreaCover::AreaCover(bool fanout_mode) :
  DagCover(fanout_mode)
{
}

// デストラクタ
AreaCover::~AreaCover()
{
}

// @brief best cut の記録を行う．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] cut_holder 各ノードのカットを保持するオブジェクト
// @param[out] maprec マッピング結果を記録するオブジェクト
void
AreaCover::record_cuts(const SbjGraph& sbjgraph,
		       const CutHolder& cut_holder,
		       MapRecord& maprec)
{
  record_cuts(sbjgraph, cut_holder, vector<const SbjNode*>(0), vector<const SbjNode*>(0), maprec);
}

// @brief best cut の記録を行う．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] cut_holder 各ノードのカットを保持するオブジェクト
// @param[in] boundary_list 境界ノードのリスト
// @param[out] maprec マッピング結果を記録するオブジェクト
void
AreaCover::record_cuts(const SbjGraph& sbjgraph,
		       const CutHolder& cut_holder,
		       const vector<const SbjNode*>& boundary_list,
		       MapRecord& maprec)
{
  record_cuts(sbjgraph, cut_holder, boundary_list, vector<const SbjNode*>(0), maprec);
}

// @brief best cut の記録を行う．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] cut_holder 各ノードのカットを保持するオブジェクト
// @param[in] boundary_list 境界ノードのリスト
// @param[in] dupnode_list 重複ノードのリスト
// @param[out] maprec マッピング結果を記録するオブジェクト
void
AreaCover::record_cuts(const SbjGraph& sbjgraph,
		       const CutHolder& cut_holder,
		       const vector<const SbjNode*>& boundary_list,
		       const vector<const SbjNode*>& dupnode_list,
		       MapRecord& maprec)
{
  int n = sbjgraph.node_num();

  // 作業領域の初期化
  mBestCost.clear();
  mBestCost.resize(n);

  // 境界マークをつける．
  mBoundaryMark.clear();
  mBoundaryMark.resize(n, 0);
  for ( auto node: boundary_list ) {
    mBoundaryMark[node->id()] = 1;
  }
  for ( auto node: dupnode_list ) {
    if ( mBoundaryMark[node->id()] == 0 ) {
      mBoundaryMark[node->id()] = 2;
    }
  }

  mWeight.resize(cut_holder.limit());

  maprec.init(sbjgraph);

  // 入力のコストを設定
  int ni = sbjgraph.input_num();
  for ( int i = 0; i < ni; ++ i ) {
    const SbjNode* node = sbjgraph.input(i);
    maprec.set_cut(node, nullptr);
    mBestCost[node->id()] = 0.0;
  }

  // 論理ノードのコストを入力側から計算
  int nl = sbjgraph.logic_num();
  for ( int i = 0; i < nl; ++ i ) {
    const SbjNode* node = sbjgraph.logic(i);

    double min_cost = DBL_MAX;
    const Cut* best_cut = nullptr;
    for ( auto cut: cut_holder.cut_list(node) ) {
      int ni = cut->input_num();
      bool ng = false;
      for ( int i = 0; i < ni; ++ i ) {
	const SbjNode* inode = cut->input(i);
	if ( mBestCost[inode->id()] == DBL_MAX ) {
	  ng = true;
	  break;
	}
      }
      if ( ng ) continue;

      if ( fanout_mode() ) {
	// ファンアウトモード
	for ( int i = 0; i < ni; ++ i)  {
	  const SbjNode* inode = cut->input(i);
	  switch ( mBoundaryMark[inode->id()] ) {
	  case 0:
	    mWeight[i] = 1.0 / inode->fanout_num();
	    break;

	  case 1:
	    mWeight[i] = 0.0;
	    break;

	  case 2:
	    mWeight[i] = 1.0;
	    break;
	  }
	}
      }
      else {
	// フローモード
	for ( int i = 0; i < ni; ++ i ) {
	  mWeight[i] = 0.0;
	}
	calc_weight(node, cut, 1.0);
	for ( int i = 0; i < ni; ++ i ) {
	  const SbjNode* inode = cut->input(i);
	  switch ( mBoundaryMark[inode->id()] ) {
	  case 0:
	    break;

	  case 1:
	    mWeight[i] = 0.0;
	    break;

	  case 2:
	    mWeight[i] = 1.0;
	    break;
	  }
	}
      }

      double cur_cost = 1.0;
      for ( int i = 0; i < ni; ++ i ) {
	const SbjNode* inode = cut->input(i);
	cur_cost += mBestCost[inode->id()] * mWeight[i];
      }
      if ( min_cost > cur_cost ) {
	min_cost = cur_cost;
	best_cut = cut;
      }
    }
    ASSERT_COND( min_cost != DBL_MAX );
    maprec.set_cut(node, best_cut);
    mBestCost[node->id()] = min_cost;
  }
}

// node から各入力にいたる経路の重みを計算する．
void
AreaCover::calc_weight(const SbjNode* node,
		       const Cut* cut,
		       double cur_weight)
{
  for ( ; ; ) {
    for ( int i = 0; i < cut->input_num(); ++ i ) {
      if ( cut->input(i) == node ) {
	// node は cut の葉だった．
	if  ( !node->pomark() ) {
	  mWeight[i] += cur_weight;
	}
	return;
      }
    }
    const SbjNode* inode0 = node->fanin(0);
    double cur_weight0 = cur_weight / inode0->fanout_num();
    calc_weight(inode0, cut, cur_weight0);
    node = node->fanin(1);
    cur_weight /= node->fanout_num();
  }
}

END_NAMESPACE_LUTMAP
