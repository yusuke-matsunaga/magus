#ifndef YM_TECHMAP_LUTMAP_H
#define YM_TECHMAP_LUTMAP_H

/// @file ym_techmap/LutMap.h
/// @brief LutMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DagACover.h 1077 2007-12-03 10:18:47Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_techmap/lutmap_nsdef.h"
#include "ym_bdn/bdn_nsdef.h"


BEGIN_NAMESPACE_YM_LUTMAP

//////////////////////////////////////////////////////////////////////
/// @class LutMap LutMap.h "ym_techmap/LutMap.h"
/// @brief LUT用のテクノロジマッパー
//////////////////////////////////////////////////////////////////////
class LutMap
{
public:

  /// @brief コンストラクタ
  LutMap();

  /// @brief デストラクタ
  ~LutMap();


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
  /// @param[out] mapnetwork マッピング結果
  /// @param[out] lut_num LUT数
  /// @param[out] depth 段数
  void
  area_map(const BdnMgr& sbjgraph,
	   ymuint limit,
	   ymuint mode,
	   LnGraph& mapnetwork,
	   ymuint& lut_num,
	   ymuint& depth);

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
  delay_map(const BdnMgr& sbjgraph,
	    ymuint limit,
	    ymuint slack,
	    ymuint mode,
	    LnGraph& mapnetwork,
	    ymuint& lut_num,
	    ymuint& depth);

};

END_NAMESPACE_YM_LUTMAP

#endif // YM_TECHMAP_LUTMAP_H
