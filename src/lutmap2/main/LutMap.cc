﻿
/// @file LutMap2.cc
/// @brief LutMap2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "LutMap2.h"
#include "SbjGraph.h"
#include "SbjNode.h"
#include "Bn2Sbj.h"
#include "MapState.h"
#include "MapRecord.h"
#include "MctSearch.h"

#include "ym/RandGen.h"

#define UNIFORM_SAMPLING 0


BEGIN_NAMESPACE_YM_LUTMAP2

//////////////////////////////////////////////////////////////////////
// クラス LutMap2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LutMap2::LutMap2()
{
}

// @brief デストラクタ
LutMap2::~LutMap2()
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
LutMap2::area_map(const BnNetwork& src_network,
		  ymuint limit,
		  ymuint mode,
		  BnBuilder& map_network,
		  ymuint& lut_num,
		  ymuint& depth)
{
  SbjGraph sbjgraph;
  Bn2Sbj bn2sbj;
  bn2sbj.convert(src_network, sbjgraph);

  MctSearch mct(sbjgraph, limit);

  mct.search(1000000);

  //const MapRecord& record = mct.best_record();

  //record.gen_mapgraph(sbjgraph, map_network, lut_num, depth);
}

END_NAMESPACE_YM_LUTMAP2