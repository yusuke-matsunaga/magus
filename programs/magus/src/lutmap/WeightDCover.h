#ifndef MAGUS_LUTMAP_WEIGHTDCOVER_H
#define MAGUS_LUTMAP_WEIGHTDCOVER_H

/// @file magus/lutmap/WeightDCover.h
/// @brief WeightDCover のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: WeightDCover.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.

#include "DagDCover.h"
#include "CutHolder.h"
#include "CutResub.h"
#include "ADCost.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

class SbjNode;

//////////////////////////////////////////////////////////////////////
/// @class WeightDCover WeightDCover.h "WeightDCover.h"
/// @brief depth/area optimal cover を求めるためのクラス
//////////////////////////////////////////////////////////////////////
class WeightDCover :
  public DagDCover
{
public:

  /// @brief コンストラクタ
  /// @param[in] mode モード
  WeightDCover(int mode);

  /// @brief デストラクタ
  virtual
  ~WeightDCover();


public:

  /// @brief best cut の記録を行う．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] limit LUT の入力数
  /// @param[in] slack 最小段数に対するスラック
  virtual
  void
  record_cuts(const SbjGraph& sbjgraph,
	      int limit,
	      ymuint slack,
	      MapRecord& maprec);


private:
  
  // node のカットを記録する．
  void
  record(SbjNode* node);

  // node のカットを選択する．
  void
  select(SbjNode* node,
	 MapRecord& maprec);
  
  // node から各入力にいたる経路の重みを計算する．
  void
  calc_weight(SbjNode* node,
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
  int mMode;
  
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

END_NAMESPACE_MAGUS_LUTMAP

#endif // MAGUS_LUTMAP_WEIGHTDCOVER_H
