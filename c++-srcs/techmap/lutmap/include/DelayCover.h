#ifndef DELAYCOVER_H
#define DELAYCOVER_H

/// @file DelayCover.h
/// @brief DelayCover のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2016, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "DagCover.h"
#include "ADCost.h"


BEGIN_NAMESPACE_LUTMAP

class Cut;

//////////////////////////////////////////////////////////////////////
/// @class DelayCover DelayCover.h "DelayCover.h"
/// @brief depth/area optimal cover を求めるためのクラス
//////////////////////////////////////////////////////////////////////
class DelayCover :
  public DagCover
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] fanout_mode ファンアウトモードの時に true にするフラグ
  /// @param[in] slack 最小段数に対するスラック
  DelayCover(bool fanout_mode,
	     int slack);

  /// @brief デストラクタ
  ~DelayCover();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief best cut の記録を行う．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] cut_holder 各ノードのカットを保持するオブジェクト
  /// @param[out] maprec マッピング結果を記録するオブジェクト
  virtual
  void
  record_cuts(const SbjGraph& sbjgraph,
	      const CutHolder& cut_holder,
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

  // スラックの値
  int mSlack;

  // マッピング用の作業領域
  vector<NodeInfo> mNodeInfo;

  // カットの葉の重みを入れる作業領域
  vector<double> mWeight;

  // ADCost のメモリ管理用オブジェクト
  ADCostMgr<double> mCostMgr;

  // カットの葉の ADCost の反復子を格納する配列
  vector<ADCostIterator<double>> mIcostLists;

};

END_NAMESPACE_LUTMAP

#endif // DELAYCOVER_H
