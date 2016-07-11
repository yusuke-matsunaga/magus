
/// @file LutMap.cc
/// @brief LutMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "LutMap.h"
#include "SbjGraph.h"
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
// @param[in] src_network もとのネットワーク
// @param[in] limit カットサイズ
// @param[in] mode モード
//  - 0: fanout フロー, resub なし
//  - 1: weighted フロー, resub なし
//  - 2: fanout フロー, resub あり
//  - 3: weighted フロー, resub あり
// @param[out] map_network マッピング結果
// @param[out] lut_num LUT数
// @param[out] depth 段数
void
LutMap::area_map(const BnNetwork& src_network,
		 ymuint limit,
		 ymuint mode,
		 BnNetwork& map_network,
		 ymuint& lut_num,
		 ymuint& depth)
{
  AreaCover area_cover;

  SbjGraph sbjgraph;

  area_cover(sbjgraph, limit, mode, map_network, lut_num, depth);
}

// @brief 段数最小化 DAG covering のヒューリスティック関数
// @param[in] src_network もとのネットワーク
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
LutMap::delay_map(const BnNetwork& src_network,
		  ymuint limit,
		  ymuint slack,
		  ymuint mode,
		  BnNetwork& map_network,
		  ymuint& lut_num,
		  ymuint& depth)
{
  DelayCover delay_cover;

  SbjGraph sbjgraph;

  delay_cover(sbjgraph, limit, slack, mode, map_network, lut_num, depth);
}

END_NAMESPACE_YM_LUTMAP
