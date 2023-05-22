
/// @file MctSearch.cc
/// @brief MctSearch の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "mct2/MctSearch.h"
#include "mct2/MctState.h"
#include "MctNode.h"
#include "MapRecord.h"
#include "MapGen.h"
#include "MapEst.h"
#include "LbCalc.h"
#include "SbjGraph.h"

#include "SbjDumper.h"


#define UNIFORM_SAMPLING 0


BEGIN_NAMESPACE_LUTMAP_MCT2

BEGIN_NONAMESPACE

int
dfs(const SbjNode* node,
    vector<bool>& mark)
{
  if ( mark[node->id()] ) {
    return 0;
  }
  mark[node->id()] = true;

  int n = 1;
  if ( node->fanout_num() == 1 && node->is_logic() ) {
    n += dfs(node->fanin(0), mark);
    n += dfs(node->fanin(1), mark);
  }
  return n;
}

struct Lt
{
  bool
  operator()(const pair<const SbjNode*, int>& left,
	     const pair<const SbjNode*, int>& right) {
    return left.second < right.second;
  }
};

struct Gt
{
  bool
  operator()(const pair<const SbjNode*, int>& left,
	     const pair<const SbjNode*, int>& right) {
    return left.second > right.second;
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
// @param[in] flow_mode area_cover のモード
MctSearch::MctSearch(const SbjGraph& sbjgraph,
		     const CutHolder& cut_holder,
		     SizeType cut_size,
		     bool flow_mode) :
  mSbjGraph(sbjgraph),
  mCutHolder(cut_holder),
  mCutSize(cut_size),
  mAreaCover(flow_mode),
  mState(sbjgraph),
  mVerbose(false)
{

  mUpperBound = sbjgraph.logic_num();
  LbCalc lbcalc;
  mLowerBound = lbcalc.lower_bound(sbjgraph, cut_holder);
  mWidth = static_cast<double>(mUpperBound - mLowerBound);

  // ファンアウトポイントを求める．
  vector<pair<const SbjNode*, int> > tmp_list;
  for (int i = 0; i < sbjgraph.logic_num(); ++ i) {
    const SbjNode* node = sbjgraph.logic(i);
    if ( node->fanout_num() > 1 ) {
#if 0
      vector<bool> mark(sbjgraph.node_num(), false);
      int ni = dfs(node->fanin(0), mark);
      ni += dfs(node->fanin(1), mark);
      tmp_list.push_back(make_pair(node, ni));
#else
      tmp_list.push_back(make_pair(node, node->fanout_num()));
#endif
    }
  }
  // サイズの降順にソートする．
  //sort(tmp_list.begin(), tmp_list.end(), Gt());
  int n = tmp_list.size();
  mFanoutPointList.reserve(n);
  mInputSizeList.reserve(n);
  for (int i = 0; i < n; ++ i) {
    const pair<const SbjNode*, int>& p = tmp_list[i];
    mFanoutPointList.push_back(p.first);
    mInputSizeList.push_back(p.second);
  }

  cout << "#logic = " << sbjgraph.logic_num() << ", #fp = " << mFanoutPointList.size() << endl;

  mMinimumLutNum = sbjgraph.node_num() + 1;
  mRootNode = new MctNode(nullptr, 0, false);
}

// @brief デストラクタ
MctSearch::~MctSearch()
{
}

// @brief 探索を行う．
// @param[in] search_limit 試行回数
// @param[in] verbose verbose フラグ
// @return 最良解を返す．
const MapRecord&
MctSearch::search(SizeType search_limit,
		  bool verbose)
{
  mVerbose = verbose;
  for (mNumAll = 1; mNumAll <= search_limit; ++ mNumAll) {
    mState.init();
    MctNode* node = tree_policy(mRootNode);
    double val = default_policy(node);
    back_up(node, val);
  }
  return mBestRecord;
}

// @brief 評価値の良い子ノードを見つける．
MctNode*
MctSearch::tree_policy(MctNode* node)
{
  while ( mState.index() < mFanoutPointList.size() ) {
    if ( !node->is_expanded() ) {
      MctNode* child_node = node->expand_child();
      const SbjNode* fpnode = mFanoutPointList[mState.index()];
      if ( child_node->is_selected() ) {
	mState.add_boundary(fpnode);
      }
      else {
	mState.add_block(fpnode);
      }
      mState.next_index();
      return child_node;
    }
    node = node->best_child();
    const SbjNode* fpnode = mFanoutPointList[mState.index()];
    if ( node->is_selected() ) {
      mState.add_boundary(fpnode);
    }
    else {
      mState.add_block(fpnode);
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
  int nf = mFanoutPointList.size();
  int lut_num = INT_MAX;
  if ( mState.index() < nf ) {
    vector<bool> state(nf, false);
    vector<bool> old_state(nf, false);
    for (int i = 0; i < 100; ++ i) {
      vector<const SbjNode*> boundary_list = mState.boundary_list();
      boundary_list.reserve(nf);
      for (int i = mState.index(); i < nf; ++ i) {
	if ( state[i] ) {
	  boundary_list.push_back(mFanoutPointList[i]);
	}
      }
      MapRecord record;
      mAreaCover.record_cuts(mSbjGraph, mCutHolder, boundary_list, record);
      MapEst gen;
      int lut_num1;
      int depth;
      gen.estimate(mSbjGraph, record, lut_num1, depth);
      cout << "  #LUT1 = " << lut_num1 << " / " << mMinimumLutNum << endl;
      if ( lut_num > lut_num1 ) {
	lut_num = lut_num1;
	if ( mMinimumLutNum > lut_num ) {
	  mMinimumLutNum = lut_num;
	  mBestRecord = record;
	}
      }
      else {
	int delta = lut_num - lut_num1;
	double t = exp(delta);
	std::uniform_real_distribution<double> rd(0, 1.0);
	double r = rd(mRandGen);
	if ( r < t ) {
	  // 受け入れる．
	  lut_num = lut_num1;
	}
	else {
	  state = old_state;
	}
      }
      old_state = state;
      std::uniform_int_distribution rd(0, nf - mState.index() - 1);
      int pos = rd(mRandGen);
      state[pos] = !state[pos + mState.index()];
    }
  }
  else {
    MapRecord record;
    mAreaCover.record_cuts(mSbjGraph, mCutHolder, mState.boundary_list(), record);
    MapEst gen;
    int depth;
    gen.estimate(mSbjGraph, record, lut_num, depth);
    if ( mMinimumLutNum > lut_num ) {
      mMinimumLutNum = lut_num;
      mBestRecord = record;
    }
  }
#else
#if 1
  while ( mState.index() < mFanoutPointList.size() ) {
    int index = mState.index();
    int ni = mInputSizeList[index];
#if 1
    // 1/2 の確率で選ばない．
    double ratio = 0.50;
    ratio = 0.99;
#else
#if 0
    // 1 / ni の確率で選ばない．
    double ratio = 1.0 / ni;
#else
    // 1 / 2^(ni - 1) の確率で選ばない．
    double ratio = 1.0 / (1U << (ni - 1));
#endif
#endif
    std::uniform_real_distribution<double> rd(0, 1.0);
    double r = rd(mRandGen);
    const SbjNode* fanout_node = mFanoutPointList[index];
    if ( r > ratio ) {
      mState.add_boundary(fanout_node);
    }
    else {
      //mState.add_block(fanout_node);
    }
    mState.next_index();
  }

  MapRecord record;
  mAreaCover.record_cuts(mSbjGraph, mCutHolder, mState.boundary_list(), mState.block_list(), record);
#else

  int nf = mFanoutPointList.size();
  int idx = mState.index();
  MapRecord record;
  if ( idx < nf ) {
    vector<const SbjNode*> boundary_list = mState.boundary_list();
    std::uniform_int_distribution<int> rd(0, nf - 1);
    int pos = rd(mRandGen);
    if ( pos > idx ) {
      const SbjNode* node = mFanoutPointList[pos];
      boundary_list.push_back(node);
    }
    mAreaCover.record_cuts(mSbjGraph, mCutHolder, boundary_list, mState.block_list(), record);
  }
  else {
    mAreaCover.record_cuts(mSbjGraph, mCutHolder, mState.boundary_list(), mState.block_list(), record);
  }
#endif

  MapEst gen;
  SizeType lut_num;
  SizeType depth;
  gen.estimate(mSbjGraph, record, lut_num, depth);
  if ( mMinimumLutNum > lut_num ) {
    mMinimumLutNum = lut_num;
    mBestRecord = record;
  }
#endif
  double val = static_cast<double>(mUpperBound - lut_num) / mWidth;
  if ( mVerbose ) {
    int slack = mFanoutPointList.size() - node->index();
    cout << "#LUT = " << lut_num << "(" << slack << ")" << " / " << mMinimumLutNum << endl;
  }
  return val;
}

// @brief 評価値の更新を行う．
void
MctSearch::back_up(MctNode* node,
		   double val)
{
  double num_all_ln = log(mNumAll);
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

END_NAMESPACE_LUTMAP_MCT2
