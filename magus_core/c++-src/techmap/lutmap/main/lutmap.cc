
/// @file lutmap.cc
/// @brief lutmap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap_nsdef.h"
#include "Bn2Sbj.h"
#include "SbjGraph.h"
#include "AreaCover.h"
#include "DelayCover.h"
#include "CutHolder.h"
#include "CutResub.h"
#include "MapGen.h"
#include "MapRecord.h"


BEGIN_NAMESPACE_LUTMAP

// @brief DAG covering のヒューリスティック関数
// @param[in] src_network もとのネットワーク
// @param[in] limit カットサイズ
// @param[in] mode モード
// @param[out] map_network マッピング結果
// @param[out] lut_num LUT数
// @param[out] depth 段数
void
lutmap(const BnNetwork& src_network,
       int limit,
       const string& mode,
       BnNetwork& map_network,
       int& lut_num,
       int& depth)
{
  // SbjGraph に変換する．
  SbjGraph sbjgraph;
  Bn2Sbj bn2sbj;
  bn2sbj.convert(src_network, sbjgraph);

  // カットを列挙する．
  CutHolder cut_holder;
  cut_holder.enum_cut(sbjgraph, limit);


  // 最良カットを記録する．
  MapRecord maprec;
  if ( area ) {
    AreaCover area_cover(mode);
    area_cover.record_cuts(sbjgraph, cut_holder, maprec);
  }
  else if ( "area_mct2" ) {
    nsMct2::MctSearch mct(sbjgraph, cut_holder, limit, mode);
    maprec = mct.search(count, verbose);
  }
  else if ( "area_sa" ) {
    SaSearch sa(sbjgraph, cut_holder, limit, mode);
    // count: 試行回数
    maprec = sa.search(count, verbose);
  }
  else if ( delay ) {
    DelayCover delay_cover(mode);
    MapRecord maprec;
    delay_cover.record_cuts(sbjgraph, cut_holder, slack, maprec);
  }

  if ( mode & 2 ) {
    // cut resubstituion
    CutResub cut_resub;
    cut_resub(sbjgraph, cut_holder, maprec);
  }

  // 最終的なネットワークを生成する．
  MapGen gen;
  gen.generate(sbjgraph, maprec, map_network, lut_num, depth);
}

END_NAMESPACE_LUTMAP
