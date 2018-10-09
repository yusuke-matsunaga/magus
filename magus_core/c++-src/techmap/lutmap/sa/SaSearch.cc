
/// @file SaSearch.cc
/// @brief SaSearch の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "SaSearch.h"
#include "MapRecord.h"
#include "MapGen.h"
#include "LbCalc.h"
#include "SbjGraph.h"
#include "SbjDumper.h"


BEGIN_NAMESPACE_LUTMAP

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
  mInitTemp  = 5.0;
  mEndTemp   = 0.001;
  mDecrement = 0.9;

  mUpperBound = sbjgraph.logic_num();
  LbCalc lbcalc;
  mLowerBound = lbcalc.lower_bound(sbjgraph, cut_holder);
  mWidth = static_cast<double>(mUpperBound - mLowerBound);

  // ファンアウトポイントを求める．
  for (ymuint i = 0; i < sbjgraph.logic_num(); ++ i) {
    const SbjNode* node = sbjgraph.logic(i);
    if ( node->fanout_num() > 1 ) {
      mFanoutPointList.push_back(node);
    }
  }

  mMinimumLutNum = sbjgraph.node_num() + 1;
}

// @brief デストラクタ
SaSearch::~SaSearch()
{
}

// @brief 探索を行う．
// @param[in] search_limit 試行回数
// @brief verbose フラグをセットする．
// @brief 最良解を返す．
const MapRecord&
SaSearch::search(ymuint search_limit,
		 bool verbose)
{
  mVerbose = verbose;
  ymuint nf = mFanoutPointList.size();
  vector<bool> state(nf, false);
  ymuint prev_val = evaluate(state);
  for (double T = mInitTemp; T > mEndTemp; T = T * mDecrement) {
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
  return mBestRecord;
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

  MapGen gen;
  int lut_num;
  int depth;
  gen.estimate(mSbjGraph, record, lut_num, depth);

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

  MapGen gen;
  int lut_num;
  int depth;
  gen.estimate(mSbjGraph, record, lut_num, depth);

  if ( mMinimumLutNum > lut_num ) {
    mMinimumLutNum = lut_num;
    mBestRecord = record;
  }

  return lut_num;
}

#endif

END_NAMESPACE_LUTMAP
