#ifndef LUTMAP2_H
#define LUTMAP2_H

/// @file LutMap2.h
/// @brief LutMap2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap_nsdef.h"
#include "ym/bnet.h"


BEGIN_NAMESPACE_YM_LUTMAP

//////////////////////////////////////////////////////////////////////
/// @class LutMap2 LutMap2.h "LutMap2.h"
/// @brief LUT用のテクノロジマッパー
//////////////////////////////////////////////////////////////////////
class LutMap2
{
public:

  /// @brief コンストラクタ
  LutMap2();

  /// @brief デストラクタ
  ~LutMap2();


public:
  //////////////////////////////////////////////////////////////////////
  // メンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 面積最小化 DAG covering のヒューリスティック関数
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] limit カットサイズ
  /// @param[in] mode モード
  ///  - 0: fanout フロー, resub なし
  ///  - 1: weighted フロー, resub なし
  ///  - 2: fanout フロー, resub あり
  ///  - 3: weighted フロー, resub あり
  /// @param[in] count 試行回数
  /// @param[out] mapnetwork マッピング結果
  /// @param[out] lut_num LUT数
  /// @param[out] depth 段数
  void
  area_map(const BnNetwork& sbjgraph,
	   ymuint limit,
	   ymuint mode,
	   ymuint count,
	   bool verbose,
	   BnNetwork& mapnetwork,
	   ymuint& lut_num,
	   ymuint& depth);

#if 0
  /// @brief 段数最小化 DAG covering のヒューリスティック関数
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] limit カットサイズ
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
  delay_map(const BnNetwork& sbjgraph,
	    ymuint limit,
	    ymuint slack,
	    ymuint mode,
	    BnNetwork& mapnetwork,
	    ymuint& lut_num,
	    ymuint& depth);
#endif

};

END_NAMESPACE_YM_LUTMAP

#endif // LUTMAP2_H
