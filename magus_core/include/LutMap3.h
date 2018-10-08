#ifndef LUTMAP3_H
#define LUTMAP3_H

/// @file LutMap3.h
/// @brief LutMap3 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap_nsdef.h"
#include "ym/bnet.h"


BEGIN_NAMESPACE_LUTMAP

//////////////////////////////////////////////////////////////////////
/// @class LutMap3 LutMap3.h "LutMap3.h"
/// @brief LUT用のテクノロジマッパー
//////////////////////////////////////////////////////////////////////
class LutMap3
{
public:

  /// @brief コンストラクタ
  LutMap3();

  /// @brief デストラクタ
  ~LutMap3();


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
	   int limit,
	   int mode,
	   int count,
	   bool verbose,
	   BnNetwork& mapnetwork,
	   int& lut_num,
	   int& depth);

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

END_NAMESPACE_LUTMAP

#endif // LUTMAP3_H
