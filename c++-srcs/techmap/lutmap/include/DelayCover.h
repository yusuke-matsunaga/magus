#ifndef DELAYCOVER_H
#define DELAYCOVER_H

/// @file DelayCover.h
/// @brief DelayCover のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2016, 2018, 2022 Yusuke Matsunaga
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
  DelayCover(
    bool fanout_mode, ///< [in] ファンアウトモードの時に true にするフラグ
    int slack         ///< [in] 最小段数に対するスラック
  );

  /// @brief デストラクタ
  ~DelayCover();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief best cut の記録を行う．
  void
  record_cuts(
    const SbjGraph& sbjgraph,    ///< [in] サブジェクトグラフ
    const CutHolder& cut_holder, ///< [in] 各ノードのカットを保持するオブジェクト
    MapRecord& maprec            ///< [out] マッピング結果を記録するオブジェクト
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // 下請けの関数
  //////////////////////////////////////////////////////////////////////

  // node のカットを記録する．
  void
  record(
    const SbjNode* node,
    const CutHolder& cut_holder
  );

  // node のカットを選択する．
  void
  select(
    const SbjNode* node,
    MapRecord& maprec
  );

  // node から各入力にいたる経路の重みを計算する．
  void
  calc_weight(
    const SbjNode* node,
    const Cut* cut,
    double cur_weight
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // ノードごとの作業領域
  struct NodeInfo
  {
    ADCostList<double> mCostList;
    int mMinDepth{0};
    int mReqDepth{0};
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

  // カットの葉の ADCost の末尾の反復子を格納する配列
  vector<ADCostIterator<double>> mIcostListEnds;

};

END_NAMESPACE_LUTMAP

#endif // DELAYCOVER_H
