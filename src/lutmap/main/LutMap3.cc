
/// @file LutMap3.cc
/// @brief LutMap3 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "LutMap3.h"
#include "Bn2Sbj.h"
#include "SbjGraph.h"
#include "DelayCover.h"
#include "CutHolder.h"
#include "CutResub.h"
#include "SaSearch.h"
#include "MapGen.h"
#include "MapRecord.h"

#include "SbjDumper.h"


BEGIN_NAMESPACE_YM_LUTMAP

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
// @param[in] count 試行回数
// @param[out] map_network マッピング結果
// @param[out] lut_num LUT数
// @param[out] depth 段数
void
LutMap3::area_map(const BnNetwork& src_network,
		  ymuint limit,
		  ymuint mode,
		  ymuint count,
		  bool verbose,
		  BnNetwork& map_network,
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
  SaSearch sa(sbjgraph, cut_holder, limit, mode);

  MapRecord maprec = sa.search(count, verbose);

  if ( false ) {
    SbjDumper dumper;
    dumper.dump(cout, sbjgraph);

    MapGen gen;
    ymuint lut_num;
    ymuint depth;
    gen.estimate(sbjgraph, maprec, lut_num, depth);
    const vector<const SbjNode*>& fp_list = gen.fanoutpoint_list();
    vector<bool> mark(sbjgraph.node_num(), false);
    for (ymuint i = 0; i < fp_list.size(); ++ i) {
      const SbjNode* node = fp_list[i];
      mark[node->id()] = true;
    }
    for (ymuint i = 0; i < sbjgraph.logic_num(); ++ i) {
      const SbjNode* node = sbjgraph.logic(i);
      if ( node->fanout_num() > 1 ) {
	if ( mark[node->id()] ) {
	  cout << " " << node->id_str();
	}
	else {
	  cout << " ***" << node->id_str() << "***";
	}
      }
      else if ( mark[node->id()] ) {
	cout << " XXX" << node->id_str() << "XXX";
      }
    }
    cout << endl;
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
LutMap3::delay_map(const BnNetwork& src_network,
		  ymuint limit,
		  ymuint slack,
		  ymuint mode,
		  BnNetwork& map_network,
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
