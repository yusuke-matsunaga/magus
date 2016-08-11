
/// @file LutMap.cc
/// @brief LutMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "LutMap3.h"
#include "SbjGraph.h"
#include "AreaCover.h"
#include "Bn2Sbj.h"
#include "MapRecord.h"


BEGIN_NAMESPACE_YM_LUTMAP3

//////////////////////////////////////////////////////////////////////
// クラス LutMap3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LutMap3::LutMap3()
{
}

// @brief デストラクタ
LutMap3::~LutMap3()
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
LutMap3::area_map(const BnNetwork& src_network,
		  ymuint limit,
		  ymuint mode,
		  BnBuilder& map_network,
		  ymuint& lut_num,
		  ymuint& depth)
{
  AreaCover area_cover;

  SbjGraph sbjgraph;
  Bn2Sbj bn2sbj;
  bn2sbj.convert(src_network, sbjgraph);

  MapRecord record = area_cover(sbjgraph, limit);

  record.gen_mapgraph(sbjgraph, map_network, lut_num, depth);

  cout << "#LUT = " << lut_num << ", depth = " << depth << endl;
}

END_NAMESPACE_YM_LUTMAP3
