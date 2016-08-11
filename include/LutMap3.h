#ifndef LUTMAP3_H
#define LUTMAP3_H

/// @file LutMap3.h
/// @brief LutMap3 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap3_nsdef.h"
#include "ym/ym_bnet.h"


BEGIN_NAMESPACE_YM_LUTMAP3

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
  /// @param[out] mapnetwork マッピング結果
  /// @param[out] lut_num LUT数
  /// @param[out] depth 段数
  void
  area_map(const BnNetwork& sbjgraph,
	   ymuint limit,
	   ymuint mode,
	   BnBuilder& mapnetwork,
	   ymuint& lut_num,
	   ymuint& depth);

};

END_NAMESPACE_YM_LUTMAP3

#endif // LUTMAP3_H
