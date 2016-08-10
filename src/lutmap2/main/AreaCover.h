#ifndef AREACOVER_H
#define AREACOVER_H

/// @file AreaCover.h
/// @brief AreCover のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap_nsdef.h"
#include "sbj_nsdef.h"
#include "ym/ym_bnet.h"
#include "CutHolder.h"
#include "CutResub.h"


BEGIN_NAMESPACE_YM_LUTMAP

class MapRecord;

//////////////////////////////////////////////////////////////////////
/// @brief 面積モードの DAG covering のヒューリスティック
//////////////////////////////////////////////////////////////////////
class AreaCover
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  AreaCover();

  /// @brief デストラクタ
  ~AreaCover();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 面積最小化マッピングを行う．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] limit LUT の入力数
  /// @param[in] mode モード
  ///  - 0: fanout フロー, resub なし
  ///  - 1: weighted フロー, resub なし
  ///  - 2: fanout フロー, resub あり
  ///  - 3: weighted フロー, resub あり
  /// @param[out] map_network マッピング結果
  /// @param[out] lut_num LUT数
  /// @param[out] depth 段数
  void
  operator()(const SbjGraph& sbjgraph,
	     ymuint limit,
	     ymuint mode,
	     BnBuilder& map_network,
	     ymuint& lut_num,
	     ymuint& depth);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief best cut の記録を行う．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] limit LUT の入力数
  /// @param[out] maprec マッピング結果を記録するオブジェクト
  void
  record_cuts(const SbjGraph& sbjgraph,
	      ymuint limit,
	      MapRecord& maprec);

  // node から各入力にいたる経路の重みを計算する．
  void
  calc_weight(const SbjNode* node,
	      const Cut* cut,
	      double cur_weight);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // モード
  ymuint mMode;

  // カットを保持するオブジェクト
  CutHolder mCutHolder;

  // cut resubstitution を実行するファンクター
  CutResub mCutResub;

  // 各ノードのベスト値を記録する配列．
  vector<double> mBestCost;

  // 各入力から根の出力に抜ける経路上の重みを入れる配列
  vector<double> mWeight;

};

END_NAMESPACE_YM_LUTMAP

#endif // AREACOVER_H
