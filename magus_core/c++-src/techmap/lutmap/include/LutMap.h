#ifndef LUTMAP_H
#define LUTMAP_H

/// @file LutMap.h
/// @brief LutMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap_nsdef.h"
#include "ym/bnet.h"


BEGIN_NAMESPACE_LUTMAP

//////////////////////////////////////////////////////////////////////
/// @class LutMap LutMap.h "LutMap.h"
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
  /// @param[in] src_network もとのネットワーク
  /// @param[in] limit カットサイズ
  /// @param[in] mode モード
  ///  - 0: fanout フロー, resub なし
  ///  - 1: weighted フロー, resub なし
  ///  - 2: fanout フロー, resub あり
  ///  - 3: weighted フロー, resub あり
  /// @param[out] dat_network マッピング結果
  /// @param[out] lut_num LUT数
  /// @param[out] depth 段数
  void
  area_map(const BnNetwork& src_network,
	   int limit,
	   int mode,
	   BnNetwork& dst_network,
	   int& lut_num,
	   int& depth);

  /// @brief 段数最小化 DAG covering のヒューリスティック関数
  /// @param[in] src_network もとのネットワーク
  /// @param[in] limit カットサイズ
  /// @param[in] slack 最小段数に対するスラック
  /// @param[in] mode モード
  ///  - 0: fanout フロー, resub なし
  ///  - 1: weighted フロー, resub なし
  ///  - 2: fanout フロー, resub あり
  ///  - 3: weighted フロー, resub あり
  /// @param[out] dat_network マッピング結果
  /// @param[out] lut_num LUT数
  /// @param[out] depth 段数
  void
  delay_map(const BnNetwork& src_network,
	    int limit,
	    int slack,
	    int mode,
	    BnNetwork& dst_network,
	    int& lut_num,
	    int& depth);

};

END_NAMESPACE_LUTMAP

#endif // LUTMAP_H
