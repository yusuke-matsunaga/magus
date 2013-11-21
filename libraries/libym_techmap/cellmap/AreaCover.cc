
/// @file AreaCover.cc
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
#include "ym_cell/CellLibrary.h"
#include "ym_cell/CellPatGraph.h"
#include "ym_cell/CellClass.h"
#include "ym_cell/CellGroup.h"
#include "PatMatcher.h"
#include "MapRecord.h"

#include "ym_logic/NpnMapM.h"

#include "ym_networks/BdnVerilogWriter.h"
#include "ym_networks/BdnDumper.h"


BEGIN_NAMESPACE_YM_CELLMAP

BEGIN_NONAMESPACE
// デバッグする時に true にするフラグ
const bool debug = false;
END_NONAMESPACE

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
		      const CellLibrary& cell_library,
		      CmnMgr& mapnetwork)
{
  if ( debug ) {
    BdnDumper dump;
    dump(cout, sbjgraph);
  }

  MapRecord maprec;

  maprec.init(sbjgraph);

  // FF のマッピングを行う．
  ff_map(sbjgraph, cell_library, maprec);

  // マッピング結果を maprec に記録する．
  record_cuts(sbjgraph, cell_library, maprec);

  // maprec の情報から mapnetwork を生成する．
  const Cell* c0_cell = NULL;
  if ( cell_library.const0_func()->cell_num() > 0 ) {
    c0_cell = cell_library.const0_func()->cell(0);
  }
  const Cell* c1_cell = NULL;
  if ( cell_library.const1_func()->cell_num() > 0 ) {
    c1_cell = cell_library.const1_func()->cell(0);
  }
  maprec.gen_mapgraph(sbjgraph, c0_cell, c1_cell, mapnetwork);
}

// @brief FF のマッピングを行う．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] cell_mgr セルを管理するオブジェクト
// @param[in] maprec マッピング結果を保持するオブジェクト
void
AreaCover::ff_map(const BdnMgr& sbjgraph,
		  const CellLibrary& cell_library,
		  MapRecord& maprec)
{
  // FFの割り当て情報を作る．
  for (ymuint i = 0; i < 4; ++ i) {
    FFInfo& ff_info = mFFInfo[i];
    bool has_clear = false;
    bool has_preset = false;
    if ( i & 1U ) {
      has_clear = true;
    }
    if ( i & 2U ) {
      has_preset = true;
    }

    const Cell* min_cell = NULL;
    CellFFInfo min_pin_info;
    CellArea min_area = CellArea::infty();
    const CellClass* ff_class = cell_library.simple_ff_class(has_clear, has_preset);
    if ( ff_class ) {
      ymuint ng = ff_class->group_num();
      for (ymuint g = 0; g < ng; ++ g) {
	const CellGroup* ff_group = ff_class->cell_group(g);
	ymuint nc = ff_group->cell_num();
	for (ymuint i = 0; i < nc; ++ i) {
	  const Cell* cell = ff_group->cell(i);
	  CellArea area = cell->area();
	  if ( min_area > area ) {
	    min_area = area;
	    min_cell = cell;
	    min_pin_info = ff_group->ff_info();
	  }
	}
      }
    }
    if ( min_cell != NULL ) {
      ff_info.mCell = min_cell;
      ff_info.mPinInfo = min_pin_info;
    }
    else {
      ff_info.mCell = NULL;
    }
  }

  const BdnDffList& dff_list = sbjgraph.dff_list();
  for (BdnDffList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const BdnDff* dff = *p;
    const BdnNode* clear = dff->clear();
    const BdnNode* preset = dff->preset();
    bool has_clear = false;
    bool has_preset = false;
    ymuint sig = 0U;
    ymuint xsig = 0U;
    if ( clear->output_fanin() ) {
      has_clear = true;
      sig |= 1U;
      xsig |= 2U;
    }
    if ( preset->output_fanin() ) {
      has_preset = true;
      sig |= 2U;
      xsig |= 1U;
    }
    FFInfo& ff_info1 = mFFInfo[sig];
    if ( ff_info1.mCell != NULL ) {
      maprec.set_dff_match(dff, false, ff_info1.mCell, ff_info1.mPinInfo);
    }
    FFInfo& ff_info2 = mFFInfo[xsig];
    if ( ff_info2.mCell != NULL ) {
      maprec.set_dff_match(dff, true, ff_info2.mCell, ff_info2.mPinInfo);
    }
  }
}

// @brief best cut の記録を行う．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] cell_library セルを管理するオブジェクト
// @param[out] maprec マッピング結果を記録するオブジェクト
void
AreaCover::record_cuts(const BdnMgr& sbjgraph,
		       const CellLibrary& cell_library,
		       MapRecord& maprec)
{
  ymuint n = sbjgraph.max_node_id();
  mCostArray.resize(n * 2);
  ymuint max_input = cell_library.pg_max_input();
  mWeight.resize(max_input);
  mLeafNum.clear();
  mLeafNum.resize(n, -1);

  const CellGroup* inv_func = cell_library.inv_func();

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
  PatMatcher pat_match(cell_library);
  ymuint np = cell_library.pg_pat_num();
  vector<const BdnNode*> snode_list;
  sbjgraph.sort(snode_list);
  for (vector<const BdnNode*>::const_iterator p = snode_list.begin();
       p != snode_list.end(); ++ p) {
    const BdnNode* node = *p;
    if ( debug ) {
      cout << endl
	   << "Processing Node[" << node->id() << "]" << endl;
    }
    double& p_cost = cost(node, false);
    double& n_cost = cost(node, true);
    p_cost = DBL_MAX;
    n_cost = DBL_MAX;
    for (ymuint pat_id = 0; pat_id < np; ++ pat_id) {
      const CellPatGraph& pat = cell_library.pg_pat(pat_id);
      ymuint ni = pat.input_num();
      Match match(ni);
      if ( pat_match(node, pat, match) ) {
	ymuint rep_id = pat.rep_id();
	if ( debug ) {
	  cout << "Match with Pat#" << pat_id
	       << ", Rep#" << rep_id << endl;
	}
	const CellClass* rep = cell_library.npn_class(rep_id);
	ymuint ng = rep->group_num();
	for (ymuint g_pos = 0; g_pos < ng; ++ g_pos) {
	  const CellGroup* group = rep->cell_group(g_pos);
	  const NpnMapM& npn_map = group->map();
	  Match c_match(ni);
	  for (ymuint i = 0; i < ni; ++ i) {
	    NpnVmap imap = npn_map.imap(VarId(i));
	    VarId dst_var = imap.var();
	    ymuint pos = dst_var.val();
	    const BdnNode* inode = match.leaf_node(pos);
	    bool iinv = match.leaf_inv(pos);
	    if ( imap.pol() == kPolNega ) {
	      iinv = !iinv;
	    }
	    c_match.set_leaf(i, inode, iinv);
	    mLeafNum[inode->id()] = i;
	  }
	  bool root_inv = pat.root_inv();
	  if ( npn_map.omap(VarId(0)).pol() == kPolNega ) {
	    root_inv = !root_inv;
	  }
	  if ( debug ) {
	    cout << "  Group#" << group->id() << endl
		 << "    Root_inv = " << root_inv << endl;
	    for (ymuint i = 0; i < ni; ++ i) {
	      cout << "    Leaf#" << i << ": ";
	      if ( c_match.leaf_inv(i) ) {
		cout << "~";
	      }
	      cout << "Node[" << c_match.leaf_node(i)->id() << "]" << endl;
	    }
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

	  ymuint nc = group->cell_num();
	  for (ymuint c_pos = 0; c_pos < nc; ++ c_pos) {
	    const Cell* cell = group->cell(c_pos);
	    double cur_cost = cell->area().value() + leaf_cost;
	    if ( debug ) {
	      cout << "      Cell = " << cell->name()
		   << ", cost = " << cur_cost << endl;
	    }
	    if ( c_cost >= cur_cost ) {
	      c_cost = cur_cost;
	      maprec.set_logic_match(node, root_inv, c_match, cell);
	    }
	  }
	}
	if ( debug ) {
	  cout << endl;
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
		   const CellGroup* inv_func,
		   MapRecord& maprec)
{
  if ( maprec.get_match(node, !inv).leaf_num() == 1 ) {
    // 逆極性の解が自分の解＋インバーターだった
    return;
  }

  double& cur_cost = cost(node, inv);
  double alt_cost = cost(node, !inv);
  ymuint nc = inv_func->cell_num();
  const Cell* inv_cell = NULL;
  double min_cost = DBL_MAX;
  for (ymuint c_pos = 0; c_pos < nc; ++ c_pos) {
    const Cell* cell = inv_func->cell(c_pos);
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
