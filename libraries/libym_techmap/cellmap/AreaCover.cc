
/// @file libym_techmap/cellmap/AreaCover.cc
/// @brief AreaCover の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "AreaCover.h"
#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnDff.h"
#include "ym_networks/CmnMgr.h"
#include "ym_cell/Cell.h"
#include "CellMgr.h"
#include "PatMgr.h"
#include "PatMatcher.h"
#include "PatGraph.h"
#include "RepFunc.h"
#include "FuncGroup.h"
#include "FFClass.h"
#include "FFGroup.h"
#include "MapRecord.h"


BEGIN_NAMESPACE_YM_CELLMAP

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
// @param[in] cell_mgr セルを管理するオブジェクト
// @param[out] mapnetwork マッピング結果
void
AreaCover::operator()(const BdnMgr& sbjgraph,
		      const CellMgr& cell_mgr,
		      CmnMgr& mapnetwork)
{
  MapRecord maprec;

  maprec.init(sbjgraph);

  // FF のマッピングを行う．
  ff_map(sbjgraph, cell_mgr, maprec);

  // マッピング結果を maprec に記録する．
  record_cuts(sbjgraph, cell_mgr, maprec);

  // maprec の情報から mapnetwork を生成する．
  const Cell* c0_cell = cell_mgr.const0_func().cell(0);
  const Cell* c1_cell = cell_mgr.const1_func().cell(0);
  maprec.gen_mapgraph(sbjgraph, c0_cell, c1_cell, mapnetwork);
}

// @brief FF のマッピングを行う．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] cell_mgr セルを管理するオブジェクト
// @param[in] maprec マッピング結果を保持するオブジェクト
void
AreaCover::ff_map(const BdnMgr& sbjgraph,
		  const CellMgr& cell_mgr,
		  MapRecord& maprec)
{
  ymuint ffc_num = cell_mgr.ff_class_num();
  const BdnDffList& dff_list = sbjgraph.dff_list();
  for (BdnDffList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const BdnDff* dff = *p;
    const BdnNode* clock = dff->clock();
    ymuint clock_sense = 0U;
    if ( clock->output_fanin_inv() ) {
      clock_sense = 2U;
    }
    else {
      clock_sense = 1U;
    }
    const BdnNode* clear = dff->clear();
    ymuint clear_sense = 0U;
    if ( clear->output_fanin() ) {
      if ( clear->output_fanin_inv() ) {
	clear_sense = 2U;
      }
      else {
	clear_sense = 1U;
      }
    }
    const BdnNode* preset = dff->preset();
    ymuint preset_sense = 0U;
    if ( preset->output_fanin() ) {
      if ( preset->output_fanin_inv() ) {
	preset_sense = 2U;
      }
      else {
	preset_sense = 1U;
      }
    }
    ymuint sig = (clock_sense - 1) + (clear_sense * 2) + (preset_sense) * 6;
    FFInfo& ff_info = mFFInfo[sig];
    if ( ff_info.mCell == NULL ) {
      ymuint min_diff = 7;
      const Cell* min_cell = NULL;
      FFPosArray min_pos_array;
      CellArea min_area = CellArea::infty();
      ymuint min_phase = 0U;
      for (ymuint i = 0; i < ffc_num; ++ i) {
	const FFClass& ffc = cell_mgr.ff_class(i);
	ymuint phase = 0U;
	// クロックおよび制御ピンの極性の差を数値化したもの
	ymuint diff = 0;
	if ( ffc.clock_sense() != clock_sense ) {
	  phase |= 1U;
	  diff += 4;
	}
	if ( clear_sense != 0U ) {
	  if ( ffc.clear_sense() == 0 ) {
	    continue;
	  }
	  if ( ffc.clear_sense() != clear_sense ) {
	    phase |= 2U;
	    diff += 1;
	  }
	}
	if ( preset_sense != 0U ) {
	  if ( ffc.preset_sense() == 0 ) {
	    continue;
	  }
	  if ( ffc.preset_sense() != preset_sense ) {
	    phase |= 4U;
	    diff += 1;
	  }
	}
	if ( min_diff < diff ) {
	  continue;
	}
	if ( min_diff > diff ) {
	  min_diff = diff;
	  min_cell = NULL;
	  min_area = CellArea::infty();
	}
	ymuint ng = ffc.group_num();
	for (ymuint j = 0; j < ng; ++ j) {
	  const FFGroup& ffg = ffc.group(j);
	  ymuint nc = ffg.cell_num();
	  for (ymuint k = 0; k < nc; ++ k) {
	    const Cell* cell = ffg.cell(j);
	    CellArea area = cell->area();
	    if ( min_area > area ) {
	      min_area = area;
	      min_cell = cell;
	      min_pos_array = ffg.pos_array();
	      min_phase = phase;
	    }
	  }
	}
      }
      assert_cond( min_cell != NULL, __FILE__, __LINE__);
      ff_info.mCell = min_cell;
      ff_info.mPosArray = min_pos_array;
      ff_info.mPhase = min_phase;
    }
    maprec.set_dff_match(dff, ff_info.mCell, ff_info.mPosArray, ff_info.mPhase);
  }
}

// @brief best cut の記録を行う．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] cell_mgr セルを管理するオブジェクト
// @param[out] maprec マッピング結果を記録するオブジェクト
void
AreaCover::record_cuts(const BdnMgr& sbjgraph,
		       const CellMgr& cell_mgr,
		       MapRecord& maprec)
{
  const PatMgr& pat_mgr = cell_mgr.pat_mgr();
  ymuint n = sbjgraph.max_node_id();
  mCostArray.resize(n * 2);
  ymuint max_input = pat_mgr.max_input();
  mWeight.resize(max_input);
  mLeafNum.clear();
  mLeafNum.resize(n, -1);

  const FuncGroup& inv_func = cell_mgr.inv_func();

  // 入力のコストを設定
  const BdnNodeList& input_list = sbjgraph.input_list();
  for (BdnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const BdnNode* node = *p;
    assert_cond( node->is_input(), __FILE__, __LINE__);
    double& p_cost = cost(node, false);
    double& n_cost = cost(node, true);
    switch ( node->input_type() ) {
    case BdnNode::kPRIMARY_INPUT:
      // 外部入力の場合，肯定の極性のみが利用可能
      p_cost = 0.0;
      n_cost = DBL_MAX;
      add_inv(node, true, inv_func, maprec);
      break;

    case BdnNode::kDFF_OUTPUT:
    case BdnNode::kLATCH_OUTPUT:
      // DFFとラッチの場合，肯定，否定のどちらの極性も利用可能
      p_cost = 0.0;
      n_cost = 0.0;
      break;
    }
  }

  // 論理ノードのコストを入力側から計算
  PatMatcher pat_match(pat_mgr);
  ymuint np = pat_mgr.pat_num();
  vector<BdnNode*> snode_list;
  sbjgraph.sort(snode_list);
  for (vector<BdnNode*>::const_iterator p = snode_list.begin();
       p != snode_list.end(); ++ p) {
    const BdnNode* node = *p;
    double& p_cost = cost(node, false);
    double& n_cost = cost(node, true);
    p_cost = DBL_MAX;
    n_cost = DBL_MAX;
    for (ymuint pat_id = 0; pat_id < np; ++ pat_id) {
      const PatGraph& pat = pat_mgr.pat(pat_id);
      ymuint ni = pat.input_num();
      if ( pat_match(node, pat) ) {
	ymuint rep_id = pat.rep_id();
	const RepFunc& rep = cell_mgr.rep(rep_id);
	ymuint nf = rep.func_num();
	for (ymuint f_pos = 0; f_pos < nf; ++ f_pos) {
	  ymuint func_id = rep.func_id(f_pos);
	  const FuncGroup& func = cell_mgr.func_group(func_id);
	  const NpnMap& npn_map = func.npn_map();
	  Match c_match(ni);
	  for (ymuint i = 0; i < ni; ++ i) {
	    tNpnImap imap = npn_map.imap(i);
	    ymuint pos = npnimap_pos(imap);
	    const BdnNode* inode = pat_match.leaf_node(pos);
	    bool iinv = pat_match.leaf_inv(pos);
	    if ( npnimap_pol(imap) == kPolNega ) {
	      iinv = !iinv;
	    }
	    c_match.set_leaf(i, inode, iinv);
	    mLeafNum[inode->id()] = i;
	  }
	  bool root_inv = pat.root_inv();
	  if ( npn_map.opol() == kPolNega ) {
	    root_inv = !root_inv;
	  }
	  double& c_cost = root_inv ? n_cost : p_cost;

	  for (ymuint i = 0; i < ni; ++ i) {
	    mWeight[i] = 0.0;
	  }
	  calc_weight(node, 1.0);
	  for (ymuint i = 0; i < ni; ++ i) {
	    mLeafNum[c_match.leaf_node(i)->id()] = -1;
	  }

	  double leaf_cost = 0.0;
	  for (ymuint i = 0; i < ni; ++ i) {
	    const BdnNode* leaf_node = c_match.leaf_node(i);
	    bool leaf_inv = c_match.leaf_inv(i);
	    leaf_cost += cost(leaf_node, leaf_inv) * mWeight[i];
	  }

	  ymuint nc = func.cell_num();
	  for (ymuint c_pos = 0; c_pos < nc; ++ c_pos) {
	    const Cell* cell = func.cell(c_pos);
	    double cur_cost = cell->area().value() + leaf_cost;
	    if ( c_cost >= cur_cost ) {
	      c_cost = cur_cost;
	      maprec.set_match(node, root_inv, c_match, cell);
	    }
	  }
	}
      }
    }
    bool has_match = false;
    if ( p_cost != DBL_MAX ) {
      has_match = true;
      add_inv(node, true, inv_func, maprec);
    }
    if ( n_cost != DBL_MAX ) {
      has_match = true;
      add_inv(node, false, inv_func, maprec);
    }
    assert_cond( has_match, __FILE__, __LINE__);
  }
}

// @brief 逆極性の解にインバーターを付加した解を追加する．
// @param[in] node 対象のノード
// @param[in] inv 極性
// @param[in] inv_func インバータの関数グループ
void
AreaCover::add_inv(const BdnNode* node,
		   bool inv,
		   const FuncGroup& inv_func,
		   MapRecord& maprec)
{
  if ( maprec.get_match(node, !inv).leaf_num() == 1 ) {
    // 逆極性の解が自分の解＋インバーターだった
    return;
  }

  double& cur_cost = cost(node, inv);
  double alt_cost = cost(node, !inv);
  ymuint nc = inv_func.cell_num();
  const Cell* inv_cell = NULL;
  double min_cost = DBL_MAX;
  for (ymuint c_pos = 0; c_pos < nc; ++ c_pos) {
    const Cell* cell = inv_func.cell(c_pos);
    double cost = cell->area().value();
    if ( min_cost > cost ) {
      min_cost = cost;
      inv_cell = cell;
    }
  }
  assert_cond( inv_cell, __FILE__, __LINE__);
  min_cost += alt_cost;
  if ( cur_cost > min_cost ) {
    cur_cost = min_cost;
    maprec.set_inv_match(node, inv, inv_cell);
  }
}

// @brief node から各入力にいたる経路の重みを計算する．
void
AreaCover::calc_weight(const BdnNode* node,
		       double cur_weight)
{
  for ( ; ; ) {
    int c = mLeafNum[node->id()];
    if ( c != -1 ) {
      // node はマッチの葉だった．
      if  ( !node->pomark() ) {
	mWeight[c] += cur_weight;
      }
      return;
    }
    assert_cond( !node->is_input(), __FILE__, __LINE__);

    const BdnNode* inode0 = node->fanin(0);
    double cur_weight0 = cur_weight / inode0->fanout_num();
    calc_weight(inode0, cur_weight0);
    node = node->fanin(1);
    cur_weight /= node->fanout_num();
  }
}

END_NAMESPACE_YM_CELLMAP
