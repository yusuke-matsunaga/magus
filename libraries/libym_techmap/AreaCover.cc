
/// @file libym_techmap/AreaCover.cc
/// @brief AreaCover の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "AreaCover.h"


BEGIN_NAMESPACE_YM_TECHMAP

//////////////////////////////////////////////////////////////////////
// クラス AreaCover
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AreaCover::AreaCover()
{
}

// @brief デストラクタ
AreaCover::~AreaCover()
{
}

// @brief 面積最小化マッピングを行う．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] pat_mgr パタングラフを管理するオブジェクト
// @param[out] mapnetwork マッピング結果
void
AreaCover::operator()(const SbjGraph& sbjgraph,
		      const PatMgr& patmgr,
		      BNetwork& mapnetwork)
{
}

// @brief best cut の記録を行う．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] pat_mgr パタングラフを管理するオブジェクト
// @param[out] maprec マッピング結果を記録するオブジェクト
void
AreaCover::record_cuts(const SbjGraph& sbjgraph,
		       const PatMgr& patmgr,
		       MapRecord& maprec)
{
  ymuint n = sbjgraph.max_node_id();
  mBestCover.clear();
  mBestCover.resize(n * 2);
  mWeight.resize(max_input);

  maprec.init(sbjgraph);

  // 入力のコストを設定
  const SbjNodeList& input_list = sbjgraph.input_list();
  for (SbjNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const SbjNode* node = *p;
    maprec.set_cut(node, NULL);
    mBestCost[node->id() * 2 + 0] = 0.0;
    mBestCost[node->id() * 2 + 1] = 0.0;// インバーター
  }

  // 論理ノードのコストを入力側から計算
  PatMatcher pat_match(patmgr);
  ymuint np = patmgr.pat_num();
  vector<const SbjNode*> snode_list;
  sbjgraph.sort(snode_list);
  for (vector<const SbjNode*>::const_iterator p = snode_list.begin();
       p != snode_list.end(); ++ p) {
    const SbjNode* node = *p;

    double min_cost = DBL_MAX;
    for (ymuint i = 0; i < np; ++ i) {
      const PatGraph& pat = patmgr.pat(i);
      Match match;
      if ( pat_match(node, pat, match) ) {
	ymuint ni = match.leaf_num();
	for (ymuint j = 0; j < ni; ++ j) {
	  const SbjNode* leaf_node = match.leaf_node(j);
	  bool leaf_inv = match.leaf_inv(j);

	}
      }
    }
  }
}

// @brief node から各入力にいたる経路の重みを計算する．
void
AreaCover::calc_weight(const SbjNode* node,
		       const Match& match,
		       double cur_weight)
{
}

END_NAMESPACE_YM_TECHMAP
