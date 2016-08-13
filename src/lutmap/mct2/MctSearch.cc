
/// @file MctSearch.cc
/// @brief MctSearch の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "MctSearch.h"
#include "MctNode.h"
#include "MctState.h"
#include "MapRecord.h"
#include "MapGen.h"
#include "LbCalc.h"
#include "SbjGraph.h"
#include "ym/BnBuilder.h"
#include "SbjDumper.h"


#define UNIFORM_SAMPLING 0


BEGIN_NAMESPACE_YM_LUTMAP_MCT2

BEGIN_NONAMESPACE

ymuint
dfs(const SbjNode* node,
    vector<bool>& mark)
{
  if ( mark[node->id()] ) {
    return 0;
  }
  mark[node->id()] = true;

  ymuint n = 1;
  if ( node->fanout_num() == 1 && node->is_logic() ) {
    n += dfs(node->fanin(0), mark);
    n += dfs(node->fanin(1), mark);
  }
  return n;
}

struct Lt
{
  bool
  operator()(const pair<const SbjNode*, ymuint>& left,
	     const pair<const SbjNode*, ymuint>& right) {
    return left.second < right.second;
  }
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス MctSearch
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] cut_holder カットフォルダー
// @param[in] cut_size カットサイズ
// @param[in] mode area_cover のモード
MctSearch::MctSearch(const SbjGraph& sbjgraph,
		     const CutHolder& cut_holder,
		     ymuint cut_size,
		     ymuint mode) :
  mSbjGraph(sbjgraph),
  mCutHolder(cut_holder),
  mCutSize(cut_size),
  mAreaCover(mode),
  mState(sbjgraph)
{

  mUpperBound = sbjgraph.logic_num();
  LbCalc lbcalc;
  mLowerBound = lbcalc.lower_bound(sbjgraph, cut_holder);
  mWidth = static_cast<double>(mUpperBound - mLowerBound);

  // ファンアウトポイントを求める．
  vector<pair<const SbjNode*, ymuint> > tmp_list;
  for (ymuint i = 0; i < sbjgraph.logic_num(); ++ i) {
    const SbjNode* node = sbjgraph.logic(i);
    if ( node->fanout_num() > 1 ) {
      vector<bool> mark(sbjgraph.max_node_id(), false);
      ymuint ni = dfs(node->fanin(0), mark);
      ni += dfs(node->fanin(1), mark);
      tmp_list.push_back(make_pair(node, ni));
    }
  }
  // サイズの降順にソートする．
  //sort(tmp_list.begin(), tmp_list.end(), Lt());
  ymuint n = tmp_list.size();
  mFanoutPointList.reserve(n);
  mInputSizeList.reserve(n);
  for (ymuint i = 0; i < n; ++ i) {
    const pair<const SbjNode*, ymuint>& p = tmp_list[i];
    mFanoutPointList.push_back(p.first);
    mInputSizeList.push_back(p.second);
  }

  mMinimumLutNum = sbjgraph.max_node_id() + 1;
  mRootNode = new MctNode(nullptr, 0, false);
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
#if 1
  for (mNumAll = 1; mNumAll <= search_limit; ++ mNumAll) {
    mState.init();
    MctNode* node = tree_policy(mRootNode);
    double val = default_policy(node);
    back_up(node, val);
  }
#else
  ymuint nf = mFanoutPointList.size();
  vector<bool> state(nf, false);
  for (mNumAll = 1; mNumAll <= search_limit; ++ mNumAll) {
    vector<const SbjNode*> boundary_list;
    boundary_list.reserve(nf);
    for (ymuint i = 0; i < nf; ++ i) {
      if ( state[i] ) {
	boundary_list.push_back(mFanoutPointList[i]);
      }
    }
    MapRecord record;
    mAreaCover.record_cuts(mSbjGraph, mCutHolder, 0, boundary_list, record);

    MapGen mapgen;
    ymuint lut_num;
    ymuint depth;
    BnBuilder mapgraph;
    mapgen.generate(mSbjGraph, record, mapgraph, lut_num, depth);
    if ( mMinimumLutNum > lut_num ) {
      mMinimumLutNum = lut_num;
      mBestRecord = record;
    }
    cout << "#LUT = " << lut_num << " / " << mMinimumLutNum << endl;
    ymuint pos = mRandGen.int32() % nf;
    state[pos] = !state[pos];
  }
#endif
}

// @brief 評価値の良い子ノードを見つける．
MctNode*
MctSearch::tree_policy(MctNode* node)
{
  while ( mState.index() < mFanoutPointList.size() ) {
    if ( !node->is_expanded() ) {
      MctNode* child_node = node->expand_child();
      if ( child_node->is_selected() ) {
	mState.add_boundary(mFanoutPointList[mState.index()]);
      }
      mState.next_index();
      return child_node;
    }
    node = node->best_child();
    if ( node->is_selected() ) {
      mState.add_boundary(mFanoutPointList[mState.index()]);
    }
    mState.next_index();
  }
  return node;
}

// @brief ランダムサンプリングを行って評価値を求める．
double
MctSearch::default_policy(MctNode* node)
{
#if 0
  ymuint nf = mFanoutPointList.size();
  vector<bool> state(nf, false);
  for (mNumAll = 1; mNumAll <= search_limit; ++ mNumAll) {
    vector<const SbjNode*> boundary_list;
    boundary_list.reserve(nf);
    for (ymuint i = 0; i < nf; ++ i) {
      if ( state[i] ) {
	boundary_list.push_back(mFanoutPointList[i]);
      }
    }
    MapRecord record;
    mAreaCover.record_cuts(mSbjGraph, mCutHolder, 0, boundary_list, record);
    ymuint lut_num;
    ymuint depth;
    BnBuilder mapgraph;
    record.gen_mapgraph(mSbjGraph, mapgraph, lut_num, depth);
    if ( mMinimumLutNum > lut_num ) {
      mMinimumLutNum = lut_num;
      mBestRecord = record;
    }
    cout << "#LUT = " << lut_num << " / " << mMinimumLutNum << endl;
    ymuint pos = mRandGen.int32() % nf;
    state[pos] = !state[pos];
  }
#endif
  while ( mState.index() < mFanoutPointList.size() ) {
    ymuint index = mState.index();
    ymuint ni = mInputSizeList[index];
#if 1
    // 1/2 の確率で選ばない．
    double ratio = 0.99;
#else
#if 1
    // 1 / ni の確率で選ばない．
    double ratio = 1.0 / ni;
#else
    // 1 / 2^(ni - 1) の確率で選ばない．
    double ratio = 1.0 / (1U << (ni - 1));
#endif
#endif
    double r = mRandGen.real1();
    if ( r > ratio ) {
      const SbjNode* fanout_node = mFanoutPointList[index];
      mState.add_boundary(fanout_node);
    }
    mState.next_index();
  }

  MapRecord record;
  mAreaCover.record_cuts(mSbjGraph, mCutHolder, mState.boundary_list(), record);

  MapGen mapgen;
  ymuint lut_num;
  ymuint depth;
  BnBuilder mapgraph;
  mapgen.generate(mSbjGraph, record, mapgraph, lut_num, depth);
  if ( mMinimumLutNum > lut_num ) {
    mMinimumLutNum = lut_num;
    mBestRecord = record;
  }
  double val = static_cast<double>(mUpperBound - lut_num) / mWidth;
  ymuint slack = mFanoutPointList.size() - node->index();
  cout << "#LUT = " << lut_num << "(" << slack << ")" << " / " << mMinimumLutNum << endl;
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
    parent->reorder(num_all_ln, 0.5);
    node = parent;
  }
}

END_NAMESPACE_YM_LUTMAP_MCT2
