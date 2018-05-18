#ifndef DELAYCOVER_H
#define DELAYCOVER_H

/// @file DelayCover.h
/// @brief DelayCover のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2016, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap_nsdef.h"
#include "sbj_nsdef.h"
#include "ADCost.h"


BEGIN_NAMESPACE_YM_LUTMAP

class Cut;
class CutHolder;
class MapRecord;

//////////////////////////////////////////////////////////////////////
/// @class DelayCover DelayCover.h "DelayCover.h"
/// @brief depth/area optimal cover を求めるためのクラス
//////////////////////////////////////////////////////////////////////
class DelayCover
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  DelayCover(int mode);

  /// @brief デストラクタ
  ~DelayCover();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief best cut の記録を行う．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] cut_holder 各ノードのカットを保持するオブジェクト
  /// @param[in] slack 最小段数に対するスラック
  /// @param[out] maprec マッピング結果を記録するオブジェクト
  void
  record_cuts(const SbjGraph& sbjgraph,
	      const CutHolder& cut_holder,
	      int slack,
	      MapRecord& maprec);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請けの関数
  //////////////////////////////////////////////////////////////////////

  // node のカットを記録する．
  void
  record(const SbjNode* node,
	 const CutHolder& cut_holder);

  // node のカットを選択する．
  void
  select(const SbjNode* node,
	 MapRecord& maprec);

  // node から各入力にいたる経路の重みを計算する．
  void
  calc_weight(const SbjNode* node,
	      const Cut* cut,
	      double cur_weight);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // ノードごとの作業領域
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

#endif // DELAYCOVER_H
