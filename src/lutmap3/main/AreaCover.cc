
/// @file AreaCover.cc
/// @brief AreCover の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "AreaCover.h"
#include "Cut.h"
#include "MapRecord.h"
#include "SbjGraph.h"
#include "SbjNode.h"

#include "SbjDumper.h"


BEGIN_NAMESPACE_YM_LUTMAP3

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
MapRecord
AreaCover::operator()(const SbjGraph& sbjgraph,
		      ymuint limit)
{
  if ( false ) {
    SbjDumper dumper;
    dumper.dump(cout, sbjgraph);
  }

  // カットを列挙する．
  mCutHolder.enum_cut(sbjgraph, limit);

  vector<const SbjNode*> fp_list;
  ymuint nl = sbjgraph.logic_num();
  for (ymuint i = 0; i < nl; ++ i) {
    const SbjNode* node = sbjgraph.logic(i);
    if ( node->fanout_num() > 1 || node->pomark() ) {
      fp_list.push_back(node);
    }
  }

  // 最良カットを記録する．
  MapRecord maprec;
  record_cuts(sbjgraph, limit, fp_list, maprec);

  return maprec;
}

// @brief best cut の記録を行う．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] limit LUT の入力数
// @param[in] fp_list ファンアウトポイントのリスト
// @param[out] maprec マッピング結果を記録するオブジェクト
void
AreaCover::record_cuts(const SbjGraph& sbjgraph,
		       ymuint limit,
		       const vector<const SbjNode*>& fp_list,
		       MapRecord& maprec)
{
  ymuint n = sbjgraph.max_node_id();

  mBestCost.clear();
  mBestCost.resize(n);

  mFlags.clear();
  mFlags.resize(n, 0);

  for (ymuint i = 0; i < fp_list.size(); ++ i) {
    const SbjNode* node = fp_list[i];
    set_fpmark(node);
  }

  maprec.init(sbjgraph);

  // 入力のコストを設定
  ymuint ni = sbjgraph.input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* node = sbjgraph.input(i);
    maprec.set_cut(node, nullptr);
    mBestCost[node->id()] = 0.0;
  }

#if 0
  // 論理ノードのコストを入力側から計算
  ymuint nl = sbjgraph.logic_num();
  for (ymuint i = 0; i < nl; ++ i) {
    const SbjNode* node = sbjgraph.logic(i);

    double min_cost = DBL_MAX;
    const Cut* best_cut = nullptr;
    const CutList& cut_list = mCutHolder.cut_list(node);
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

      double cur_cost = 1.0;
      for (ymuint i = 0; i < ni; ++ i) {
	const SbjNode* inode = cut->input(i);
	cur_cost += mBestCost[inode->id()];
      }
      if ( min_cost > cur_cost ) {
	min_cost = cur_cost;
	best_cut = cut;
      }
    }
    ASSERT_COND(min_cost < DBL_MAX );
    ASSERT_COND( best_cut != nullptr );
    maprec.set_cut(node, best_cut);
    mBestCost[node->id()] = min_cost;
  }
#else
  // 各FFRごとにコストを計算
  for (ymuint i = 0; i < fp_list.size(); ++ i) {
    const SbjNode* ffr_root = fp_list[i];
    // ffr_root の TFI に印をつける．
    vector<const SbjNode*> node_list;
    clear_fpmark(ffr_root);
    dfs(ffr_root, node_list);

    ymuint n = node_list.size();
    for (ymuint j = 0; j < n; ++ j) {
      const SbjNode* node = node_list[n - j - 1];
      if ( node->is_input() || fpmark(node) ) {
	mBestCost[node->id()] = 0.0;
      }
      else {
	double min_cost = DBL_MAX;
	const Cut* best_cut = nullptr;
	const CutList& cut_list = mCutHolder.cut_list(node);
	for (CutListIterator p = cut_list.begin();
	     p != cut_list.end(); ++ p) {
	  const Cut* cut = *p;

	  ymuint ni = cut->input_num();
	  bool ng = false;
	  for (ymuint i = 0; i < ni; ++ i) {
	    const SbjNode* inode = cut->input(i);
	    if ( !mark(inode) || mBestCost[inode->id()] == DBL_MAX ) {
	      ng = true;
	      break;
	    }
	  }
	  if ( ng ) continue;

	  double cur_cost = 1.0;
	  for (ymuint i = 0; i < ni; ++ i) {
	    const SbjNode* inode = cut->input(i);
	    cur_cost += mBestCost[inode->id()];
	  }
	  if ( min_cost > cur_cost ) {
	    min_cost = cur_cost;
	    best_cut = cut;
	  }
	}
	ASSERT_COND(min_cost < DBL_MAX );
	ASSERT_COND( best_cut != nullptr );
	maprec.set_cut(node, best_cut);
	mBestCost[node->id()] = min_cost;
      }
    }
    set_fpmark(ffr_root);
    for (ymuint j = 0; j < n; ++ j) {
      const SbjNode* node = node_list[j];
      clear_mark(node);
    }
  }
#endif
}

// @brief FFR に印をつける．
void
AreaCover::dfs(const SbjNode* node,
	       vector<const SbjNode*>& node_list)
{
  if ( mark(node) ) {
    return;
  }
  set_mark(node);
  node_list.push_back(node);

  if ( node->is_logic() && !fpmark(node) ) {
    dfs(node->fanin(0), node_list);
    dfs(node->fanin(1), node_list);
  }
}

END_NAMESPACE_YM_LUTMAP3
