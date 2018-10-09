
/// @file LutmapMgr.cc
/// @brief LutmapMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "LutmapMgr.h"
#include "Bn2Sbj.h"
#include "SbjGraph.h"
#include "AreaCover.h"
#include "DelayCover.h"
#include "CutHolder.h"
#include "CutResub.h"
#include "MapGen.h"
#include "MapRecord.h"


BEGIN_NAMESPACE_LUTMAP

BEGIN_NONAMESPACE

// @brief マッピングを行う実際の関数
void
do_mapping(const BnNetwork& src_network,
	   DagCover& dag_cover,
	   int lut_size,
	   int slack,
	   bool do_cut_resub,
	   BnNetwork& map_network,
	   int& lut_num,
	   int& depth)
{
  SbjGraph sbjgraph;
  Bn2Sbj bn2sbj;
  bn2sbj.convert(src_network, sbjgraph);

  // カットを列挙する．
  CutHolder cut_holder;
  cut_holder.enum_cut(sbjgraph, lut_size);

  // 最良カットを記録する．
  MapRecord maprec;
  dag_cover.record_cuts(sbjgraph, cut_holder, maprec);

  if ( do_cut_resub ) {
    // cut resubstituion
    CutResub cut_resub;
    cut_resub(sbjgraph, cut_holder, maprec, slack);
  }

  // 最終的なネットワークを生成する．
  MapGen gen;
  gen.generate(sbjgraph, maprec, map_network, lut_num, depth);
}

END_NONAMESPACE

END_NAMESPACE_LUTMAP


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// クラス LutmapMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] lut_size LUTの入力数
// @param[in] algorithm アルゴリズムを表す文字列
LutmapMgr::LutmapMgr(int lut_size,
		     const string& algorithm) :
  mLutSize(lut_size),
  mFanoutMode(false),
  mDoCutResub(false)
{
  set_algorithm(algorithm);
}

// @brief デストラクタ
LutmapMgr::~LutmapMgr()
{
}

// @brief 面積最小化 DAG covering のヒューリスティック関数
// @param[in] src_network もとのネットワーク
// @param[out] map_network マッピング結果
void
LutmapMgr::area_map(const BnNetwork& src_network,
		    BnNetwork& map_network)
{
  using namespace nsLutmap;

  AreaCover area_cover(mFanoutMode);
  do_mapping(src_network, area_cover, mLutSize, -1, mDoCutResub,
	     map_network, mLutNum, mDepth);
}

// @brief 段数最小化 DAG covering のヒューリスティック関数
// @param[in] src_network もとのネットワーク
// @param[in] slack 最小段数に対するスラック
// @param[out] mapnetwork マッピング結果
void
LutmapMgr::delay_map(const BnNetwork& src_network,
		  int slack,
		  BnNetwork& map_network)
{
  using namespace nsLutmap;

  DelayCover delay_cover(mFanoutMode, slack);
  do_mapping(src_network, delay_cover, mLutSize, slack, mDoCutResub,
	     map_network, mLutNum, mDepth);
}

// @brief アルゴリズムの文字列をパースする．
void
LutmapMgr::parse_algorithm()
{
}

END_NAMESPACE_MAGUS
