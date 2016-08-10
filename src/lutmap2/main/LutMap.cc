
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
#include "MapState.h"
#include "Cut.h"
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


  RandGen rg;

  for (ymuint i = 0; i < 10; ++ i) {
    MapState state(sbjgraph, limit);
    for ( ; ; ) {
      //state.check_cover(sbjgraph);

      // カット中に出力ノードが含まれていたら優先的に選ぶ．
      const vector<const SbjNode*>& po_list = state.next_pocandidates();
      if ( !po_list.empty() ) {
	const SbjNode* node = po_list[0];
	state.update(node);
	continue;
      }

      const vector<const SbjNode*>& cut_list = state.next_candidates();
      const vector<ymuint>& w_list = state.weight_list();
      if ( cut_list.empty() ) {
	break;
      }

#if UNIFORM_SAMPLING
      ymuint n = cut_list.size();
      ymuint r = rg.int32() % n;
      const SbjNode* root = cut_list[r];
      state.update(root);
#else

      ymuint n = cut_list.size();
      vector<ymuint> acc_w(n);
      ymuint sum = 0;
      for (ymuint i = 0; i < n; ++ i) {
	ymuint w = 1U << (w_list[i] - 1);
	sum += w;
	acc_w[i] = sum;
      }
      ymuint r = rg.int32() % sum;
      ymuint pos = 0;
      for ( ; pos < n; ++ pos) {
	if ( r < acc_w[pos] ) {
	  break;
	}
      }
      ASSERT_COND( pos < n );
      const SbjNode* root = cut_list[pos];
      state.update(root);
#endif
    }
    state.gen_mapgraph(map_network, lut_num, depth);
    cout << "#LUT: " << lut_num << ", level = " << depth << endl;
  }
}

END_NAMESPACE_YM_LUTMAP2
