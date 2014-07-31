#ifndef LIBYM_TECHMAP_LUTMAP_DELAYCOVER_H
#define LIBYM_TECHMAP_LUTMAP_DELAYCOVER_H

/// @file libym_techmap/lutmap/DelayCover.h
/// @brief DelayCover のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DelayCover.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap_nsdef.h"
#include "YmNetworks/bdn.h"
#include "CutHolder.h"
#include "CutResub.h"
#include "ADCost.h"


BEGIN_NAMESPACE_YM_LUTMAP

//////////////////////////////////////////////////////////////////////
/// @class DelayCover DelayCover.h "DelayCover.h"
/// @brief depth/area optimal cover を求めるためのクラス
//////////////////////////////////////////////////////////////////////
class DelayCover
{
public:

  /// @brief コンストラクタ
  DelayCover();

  /// @brief デストラクタ
  ~DelayCover();


public:

  /// @brief 遅延最小化マッピングを行う．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] limit LUT の入力数
  /// @param[in] slack 最小段数に対するスラック
  /// @param[in] mode モード
  ///  - 0: fanout フロー, resub なし
  ///  - 1: weighted フロー, resub なし
  ///  - 2: fanout フロー, resub あり
  ///  - 3: weighted フロー, resub あり
  /// @param[out] mapnetwork マッピング結果
  /// @param[out] lut_num LUT数
  /// @param[out] depth 段数
  void
  operator()(const BdnMgr& sbjgraph,
	     ymuint limit,
	     ymuint slack,
	     ymuint mode,
	     LnGraph& mapnetwork,
	     ymuint& lut_num,
	     ymuint& depth);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請けの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief best cut の記録を行う．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] limit LUT の入力数
  /// @param[in] slack 最小段数に対するスラック
  void
  record_cuts(const BdnMgr& sbjgraph,
	      ymuint limit,
	      ymuint slack,
	      MapRecord& maprec);

  // node のカットを記録する．
  void
  record(const BdnNode* node);

  // node のカットを選択する．
  void
  select(const BdnNode* node,
	 MapRecord& maprec);

  // node から各入力にいたる経路の重みを計算する．
  void
  calc_weight(const BdnNode* node,
	      const Cut* cut,
	      double cur_weight);


private:

  struct NodeInfo
  {

    // コンストラクタ
    NodeInfo() :
      mMinDepth(0),
      mReqDepth(0)
    {
    }

    ADCostList<double> mCostList;
    int mMinDepth;
    int mReqDepth;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // モード
  ymuint mMode;

  // カットを保持するオブジェクト
  CutHolder mCutHolder;

  // カットの置き換えを行うオブジェクト
  CutResub mCutResub;

  // マッピング用の作業領域
  vector<NodeInfo> mNodeInfo;

  // カットの葉の重みを入れる作業領域
  vector<double> mWeight;

  // ADCost のメモリ管理用オブジェクト
  ADCostMgr<double> mCostMgr;

  // カットの葉の ADCost の反復子を格納する配列
  vector<ADCostIterator<double> > mIcostLists;

};

END_NAMESPACE_YM_LUTMAP

#endif // LIBYM_TECHMAP_LUTMAP_DELAYCOVER_H
