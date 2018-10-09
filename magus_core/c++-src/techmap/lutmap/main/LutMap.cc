
/// @file LutMap.cc
/// @brief LutMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 208 Yusuke Matsunaga
/// All rights reserved.


#include "LutMap.h"
#include "Bn2Sbj.h"
#include "SbjGraph.h"
#include "AreaCover.h"
#include "DelayCover.h"
#include "CutHolder.h"
#include "CutResub.h"
#include "MapGen.h"
#include "MapRecord.h"


BEGIN_NAMESPACE_LUTMAP

//////////////////////////////////////////////////////////////////////
// クラス LutMap
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] lut_size LUTの入力数
// @param[in] algorithm アルゴリズムを表す文字列
LutMap::LutMap(int lut_size,
	       const string& algorithm) :
  mLutSize(lut_size),
  mFlowMode(false),
  mDoCutResub(false)
{
  set_algorithm(algorithm);
}

// @brief デストラクタ
LutMap::~LutMap()
{
}

// @brief 面積最小化 DAG covering のヒューリスティック関数
// @param[in] src_network もとのネットワーク
// @param[out] map_network マッピング結果
void
LutMap::area_map(const BnNetwork& src_network,
		 BnNetwork& map_network)
{
  mSlack = -1;
  AreaCover area_cover(mFlowMode);
  do_mapping(src_network, map_network, area_cover);
}

// @brief 段数最小化 DAG covering のヒューリスティック関数
// @param[in] src_network もとのネットワーク
// @param[in] slack 最小段数に対するスラック
// @param[out] mapnetwork マッピング結果
void
LutMap::delay_map(const BnNetwork& src_network,
		  int slack,
		  BnNetwork& map_network)
{
  mSlack = slack;
  DelayCover delay_cover(mFlowMode, slack);
  do_mapping(src_network, map_network, delay_cover);
}

// @brief アルゴリズムの文字列をパースする．
void
LutMap::parse_algorithm()
{
}

/// @brief マッピングを行う実際の関数
void
LutMap::do_mapping(const BnNetwork& src_network,
		   BnNetwork& map_network,
		   DagCover& dag_cover)
{
  SbjGraph sbjgraph;
  Bn2Sbj bn2sbj;
  bn2sbj.convert(src_network, sbjgraph);

  // カットを列挙する．
  CutHolder cut_holder;
  cut_holder.enum_cut(sbjgraph, mLutSize);

  // 最良カットを記録する．
  MapRecord maprec;
  dag_cover.record_cuts(sbjgraph, cut_holder, maprec);

  if ( mDoCutResub ) {
    // cut resubstituion
    CutResub cut_resub;
    cut_resub(sbjgraph, cut_holder, maprec, mSlack);
  }

  // 最終的なネットワークを生成する．
  MapGen gen;
  gen.generate(sbjgraph, maprec, map_network, mLutNum, mDepth);
}

END_NAMESPACE_LUTMAP
