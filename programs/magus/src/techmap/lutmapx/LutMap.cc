
/// @file libym_techmap/lutmap/LutMap.cc
/// @brief LutMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LutMap.h"
#include "AreaCover.h"
#include "DelayCover.h"


BEGIN_NAMESPACE_YM_LUTMAP

//////////////////////////////////////////////////////////////////////
// クラス LutMap
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LutMap::LutMap()
{
}

// @brief デストラクタ
LutMap::~LutMap()
{
}

// @brief 面積最小化 DAG covering のヒューリスティック関数
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] limit カットサイズ
// @param[in] mode モード
//  - 0: fanout フロー, resub なし
//  - 1: weighted フロー, resub なし
//  - 2: fanout フロー, resub あり
//  - 3: weighted フロー, resub あり
// @param[out] mapnetwork マッピング結果
// @param[out] lut_num LUT数
// @param[out] depth 段数
void
LutMap::area_map(const BdnMgr& sbjgraph,
		 ymuint limit,
		 ymuint mode,
		 LnGraph& mapnetwork,
		 ymuint& lut_num,
		 ymuint& depth)
{
  AreaCover area_cover;

  area_cover(sbjgraph, limit, mode, mapnetwork, lut_num, depth);
}

// @brief 段数最小化 DAG covering のヒューリスティック関数
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] limit カットサイズ
// @param[in] slack 最小段数に対するスラック
// @param[in] mode モード
//  - 0: fanout フロー, resub なし
//  - 1: weighted フロー, resub なし
//  - 2: fanout フロー, resub あり
//  - 3: weighted フロー, resub あり
// @param[out] mapnetwork マッピング結果
// @param[out] lut_num LUT数
// @param[out] depth 段数
void
LutMap::delay_map(const BdnMgr& sbjgraph,
		  ymuint limit,
		  ymuint slack,
		  ymuint mode,
		  LnGraph& mapnetwork,
		  ymuint& lut_num,
		  ymuint& depth)
{
  DelayCover delay_cover;

  delay_cover(sbjgraph, limit, slack, mode, mapnetwork, lut_num, depth);
}

END_NAMESPACE_YM_LUTMAP
