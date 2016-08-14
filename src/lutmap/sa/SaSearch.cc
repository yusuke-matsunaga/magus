
/// @file SaSearch.cc
/// @brief SaSearch の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "SaSearch.h"
#include "MapRecord.h"
#include "MapEst.h"
#include "LbCalc.h"
#include "SbjGraph.h"
#include "SbjDumper.h"


#define UNIFORM_SAMPLING 0


BEGIN_NAMESPACE_YM_LUTMAP

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
// クラス SaSearch
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] cut_holder カットフォルダー
// @param[in] cut_size カットサイズ
// @param[in] mode area_cover のモード
SaSearch::SaSearch(const SbjGraph& sbjgraph,
		   const CutHolder& cut_holder,
		   ymuint cut_size,
		   ymuint mode) :
  mSbjGraph(sbjgraph),
  mCutHolder(cut_holder),
  mCutSize(cut_size),
  mAreaCover(mode),
  mVerbose(false)
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
}

// @brief デストラクタ
SaSearch::~SaSearch()
{
}

// @brief 探索を行う．
// @param[in] search_limit 試行回数
void
SaSearch::search(ymuint search_limit)
{
  ymuint nf = mFanoutPointList.size();
  vector<bool> state(nf, false);
  ymuint prev_val = evaluate(state);
  for (double T = 5.0; T > 0.001; T = T * 0.9) {
    ymuint n_acc = 0;
    for (mNumAll = 1; mNumAll <= search_limit; ++ mNumAll) {
      ymuint pos = mRandGen.int32() % nf;
      state[pos] = !state[pos];
      ymuint val = evaluate(state);
      if ( mVerbose ) {
	cout << "#LUT = " << val << " / " << mMinimumLutNum
	     << " @ " << T << " (" << n_acc << " / " << mNumAll << ")" << endl;
      }
      if ( prev_val < val ) {
	ymint dint = prev_val - val;
	double d = static_cast<double>(dint);
	double t = exp(d / T);
	double r = mRandGen.real1();
	if ( r > t ) {
	  state[pos] = !state[pos];
	}
	else {
	  prev_val = val;
	  ++ n_acc;
	}
      }
      else {
	prev_val = val;
	++ n_acc;
      }
    }
  }
}

#if 0

// @brief 現在の割り当てのもとで評価を行う．
ymuint
SaSearch::evaluate(const vector<bool>& state)
{
  ymuint nf = mFanoutPointList.size();
  vector<const SbjNode*> boundary_list;
  boundary_list.reserve(nf);
  vector<const SbjNode*> dupnode_list;
  dupnode_list.reserve(nf);
  for (ymuint i = 0; i < nf; ++ i) {
    const SbjNode* fpnode = mFanoutPointList[i];
    if ( state[i] ) {
      boundary_list.push_back(fpnode);
    }
    else {
      dupnode_list.push_back(fpnode);
    }
  }

  MapRecord record;
  mAreaCover.record_cuts(mSbjGraph, mCutHolder, boundary_list, dupnode_list, record);

  MapEst mapest;
  ymuint lut_num;
  ymuint depth;
  mapest.estimate(mSbjGraph, record, lut_num, depth);

  if ( mMinimumLutNum > lut_num ) {
    mMinimumLutNum = lut_num;
    mBestRecord = record;
  }

  return lut_num;
}

#else

// @brief 現在の割り当てのもとで評価を行う．
ymuint
SaSearch::evaluate(const vector<bool>& state)
{
  ymuint nf = mFanoutPointList.size();
  vector<const SbjNode*> boundary_list;
  boundary_list.reserve(nf);
  for (ymuint i = 0; i < nf; ++ i) {
    const SbjNode* fpnode = mFanoutPointList[i];
    if ( state[i] ) {
      boundary_list.push_back(fpnode);
    }
  }

  MapRecord record;
  mAreaCover.record_cuts(mSbjGraph, mCutHolder, boundary_list, record);

  MapEst mapest;
  ymuint lut_num;
  ymuint depth;
  mapest.estimate(mSbjGraph, record, lut_num, depth);

  if ( mMinimumLutNum > lut_num ) {
    mMinimumLutNum = lut_num;
    mBestRecord = record;
  }

  return lut_num;
}

#endif

// @brief verbose フラグをセットする．
void
SaSearch::set_verbose(bool val)
{
  mVerbose = val;
}

END_NAMESPACE_YM_LUTMAP
