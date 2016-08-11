
/// @file MctSearch.cc
/// @brief MctSearch の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "MctSearch.h"
#include "MctNode.h"
#include "MapState.h"
#include "MapRecord.h"
#include "CutCount.h"

#include "SbjDumper.h"


#define UNIFORM_SAMPLING 0


BEGIN_NAMESPACE_YM_LUTMAP2

//////////////////////////////////////////////////////////////////////
// クラス MctSearch
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] cut_size カットサイズ
MctSearch::MctSearch(const SbjGraph& sbjgraph,
		     ymuint cut_size) :
  mState(sbjgraph, cut_size),
  mInsideNodeMark(sbjgraph.max_node_id(), false)
{
  CutCount cc;
  mBaseline = cc.lower_bound(sbjgraph, cut_size);

  ymuint nf = 0;
  for (ymuint i = 0; i < sbjgraph.logic_num(); ++ i) {
    const SbjNode* node = sbjgraph.logic(i);
    if ( node->fanout_num() > 1 ) {
      ++ nf;
    }
  }
  cout << "# of logic nodes   = " << sbjgraph.logic_num() << endl;
  cout << "# of fanout points = " << nf << endl;

  mMinimumLutNum = sbjgraph.max_node_id() + 1;

  mState.init();
  trivial_move();
  mRootNode = new MctNode(nullptr, nullptr, mState.candidates());
}

// @brief デストラクタ
MctSearch::~MctSearch()
{
}

// @brief 探索を行う．
// @param[in] search_limit 試行回数
void
MctSearch::search(ymuint search_limit)
{
  for (mNumAll = 1; mNumAll <= search_limit; ++ mNumAll) {
    mState.init();
    trivial_move();
    MctNode* node = tree_policy(mRootNode);
    double val = default_policy(node);
    back_up(node, val);
  }
}

// @brief 評価値の良い子ノードを見つける．
MctNode*
MctSearch::tree_policy(MctNode* node)
{
  for ( ; ; ) {
    if ( !node->is_expanded() ) {
      const SbjNode* root = node->expand_child();
      move(root);
      MctNode* child_node = new MctNode(node, root, mState.candidates());
      node->insert_child(child_node);
      return child_node;
    }
    if ( node->child_num() == 0 ) {
      return node;
    }
    node = node->best_child();
    move(node->cut_root());
  }
}

// @brief ランダムサンプリングを行って評価値を求める．
double
MctSearch::default_policy(MctNode* node)
{
  ymuint ln0 = mState.lut_num();
  for ( ; ; ) {
    const vector<const SbjNode*>& cut_list = mState.candidates();
    const vector<ymuint>& w_list = mState.weight_list();
    if ( cut_list.empty() ) {
      break;
    }

#if UNIFORM_SAMPLING
    ymuint n = cut_list.size();
    ymuint r = rg.int32() % n;
    const SbjNode* root = cut_list[r];
    move(root);
#else
    ymuint n = cut_list.size();
    vector<ymuint> acc_w(n);
    ymuint sum = 0;
    for (ymuint i = 0; i < n; ++ i) {
      const SbjNode* node = cut_list[i];
      ymuint w = 1U << (w_list[i] - 1);
      w *= (node->fanout_num() * 2);
      sum += w;
      acc_w[i] = sum;
    }
    ymuint r = mRandGen.int32() % sum;
    ymuint pos = 0;
    for ( ; pos < n; ++ pos) {
      if ( r < acc_w[pos] ) {
	break;
      }
    }
    ASSERT_COND( pos < n );
    const SbjNode* root = cut_list[pos];
    move(root);
#endif
  }
  ymuint ln = mState.lut_num();
  if ( mMinimumLutNum > ln ) {
    mMinimumLutNum = ln;
    mState.copy_to(mBestRecord);
  }
  double val = mBaseline / ln;
  cout << "#LUT = " << ln << "(" << ln0 << ")" << " / " << mMinimumLutNum << endl;
  return val;
}

// @brief 評価値の更新を行う．
void
MctSearch::back_up(MctNode* node,
		   double val)
{
  double num_all_ln = log(mNumAll) / log(2);
  for ( ; ; ) {
    node->update(val);
    MctNode* parent = node->parent();
    if ( parent == nullptr ) {
      break;
    }
    ASSERT_COND( parent->best_child() == node );
    parent->reorder(num_all_ln);
    node = parent;
  }
}

// @brief 状態を遷移させる．
// @param[in] cut_root カットの根のノード
//
// 初期状態は cut_root = nullptr とする．
void
MctSearch::move(const SbjNode* cut_root)
{
  mState.update(cut_root);
  trivial_move();
}

// @brief trivial な選択を行う．
void
MctSearch::trivial_move()
{
  for ( ; ; ) {
    const vector<const SbjNode*>& po_list = mState.pocandidates();
    if ( po_list.empty() ) {
      break;
    }
    const SbjNode* root = po_list[0];
    mState.update(root);
  }
}

END_NAMESPACE_YM_LUTMAP2
