
/// @file AreaCover.cc
/// @brief AreCover の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "AreaCover.h"
#include "Cut.h"
#include "CutHolder.h"
#include "MapRecord.h"
#include "SbjGraph.h"
#include "SbjNode.h"


BEGIN_NAMESPACE_YM_LUTMAP

// コンストラクタ
AreaCover::AreaCover(ymuint mode)
{
  mMode = mode;
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
  record_cuts(sbjgraph, cut_holder, vector<const SbjNode*>(0), maprec);
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
  ymuint n = sbjgraph.max_node_id();

  // 作業領域の初期化
  mBestCost.clear();
  mBestCost.resize(n);

  // 境界マークをつける．
  mBoundaryMark.clear();
  mBoundaryMark.resize(n, 0);
  for (ymuint i = 0; i < boundary_list.size(); ++ i) {
    const SbjNode* node = boundary_list[i];
    mBoundaryMark[node->id()] = 1;
  }

  mWeight.resize(cut_holder.limit());

  maprec.init(sbjgraph);

  // 入力のコストを設定
  ymuint ni = sbjgraph.input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* node = sbjgraph.input(i);
    maprec.set_cut(node, nullptr);
    mBestCost[node->id()] = 0.0;
  }

  // 論理ノードのコストを入力側から計算
  ymuint nl = sbjgraph.logic_num();
  for (ymuint i = 0; i < nl; ++ i) {
    const SbjNode* node = sbjgraph.logic(i);

    double min_cost = DBL_MAX;
    const Cut* best_cut = nullptr;
    const CutList& cut_list = cut_holder.cut_list(node);
    for (CutListIterator p = cut_list.begin();
	 p != cut_list.end(); ++ p) {
      const Cut* cut = *p;

      ymuint ni = cut->input_num();
      bool ng = false;
      for (ymuint i = 0; i < ni; ++ i) {
	const SbjNode* inode = cut->input(i);
	if ( mBestCost[inode->id()] == DBL_MAX ) {
	  ng = true;
	  break;
	}
      }
      if ( ng ) continue;

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

      double cur_cost = 1.0;
      for (ymuint i = 0; i < ni; ++ i) {
	const SbjNode* inode = cut->input(i);
	if ( mBoundaryMark[inode->id()] == 0 ) {
	  cur_cost += mBestCost[inode->id()] * mWeight[i];
	}
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
    for (ymuint i = 0; i < cut->input_num(); ++ i) {
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

END_NAMESPACE_YM_LUTMAP
