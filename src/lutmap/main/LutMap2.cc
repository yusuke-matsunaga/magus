﻿
/// @file LutMap2.cc
/// @brief LutMap2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "LutMap2.h"
#include "Bn2Sbj.h"
#include "SbjGraph.h"
#include "AreaCover_MCT2.h"
#include "DelayCover.h"
#include "CutHolder.h"
#include "CutResub.h"
#include "MapGen.h"
#include "MapRecord.h"


BEGIN_NAMESPACE_YM_LUTMAP

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
// @param[in] count 試行回数
// @param[out] map_network マッピング結果
// @param[out] lut_num LUT数
// @param[out] depth 段数
void
LutMap2::area_map(const BnNetwork& src_network,
		  ymuint limit,
		  ymuint mode,
		  ymuint count,
		  bool verbose,
		  BnBuilder& map_network,
		  ymuint& lut_num,
		  ymuint& depth)
{

  SbjGraph sbjgraph;
  Bn2Sbj bn2sbj;
  bn2sbj.convert(src_network, sbjgraph);

  // カットを列挙する．
  CutHolder cut_holder;
  cut_holder.enum_cut(sbjgraph, limit);

  // 最良カットを記録する．
  MapRecord maprec;
  nsMct2::AreaCover_MCT2 area_cover(mode);
  area_cover.record_cuts(sbjgraph, cut_holder, count, verbose, maprec);

  if ( mode & 2 ) {
    // cut resubstituion
    CutResub cut_resub;
    cut_resub(sbjgraph, cut_holder, maprec);
  }

  // 最終的なネットワークを生成する．
  MapGen gen;

  gen.generate(sbjgraph, maprec, map_network, lut_num, depth);
}

#if 0
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
LutMap2::delay_map(const BnNetwork& src_network,
		  ymuint limit,
		  ymuint slack,
		  ymuint mode,
		  BnBuilder& map_network,
		  ymuint& lut_num,
		  ymuint& depth)
{

  SbjGraph sbjgraph;
  Bn2Sbj bn2sbj;
  bn2sbj.convert(src_network, sbjgraph);

  // カットを列挙する．
  CutHolder cut_holder;
  cut_holder.enum_cut(sbjgraph, limit);

  // 最良カットを記録する．
  DelayCover delay_cover(mode);
  MapRecord maprec;
  delay_cover.record_cuts(sbjgraph, cut_holder, slack, maprec);

  if ( mode & 2 ) {
    // cut resubstituion
    CutResub cut_resub;
    cut_resub(sbjgraph, cut_holder, maprec, slack);
  }

  // 最終的なネットワークを生成する．
  MapGen gen;
  gen.generate(sbjgraph, maprec, map_network, lut_num, depth);
}
#endif

END_NAMESPACE_YM_LUTMAP
