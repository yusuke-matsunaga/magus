
/// @file AreaCover.cc
/// @brief AreaCover の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015 Yusuke Matsunaga
/// All rights reserved.

#include "AreaCover.h"
#include "ym/ClibCell.h"
#include "ym/ClibCellList.h"
#include "ym/ClibCellLibrary.h"
#include "ym/ClibIOMap.h"
#include "ym/ClibPatGraph.h"
#include "ym/ClibCellClass.h"
#include "ym/ClibCellGroup.h"
#include "SbjGraph.h"
#include "PatMatcher.h"
#include "MapRecord.h"
#include "MapGen.h"
#include "SbjDumper.h"

#include "ym/NpnMapM.h"


BEGIN_NAMESPACE_CELLMAP

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
BnNetwork
AreaCover::operator()(
  const SbjGraph& sbjgraph,
  const ClibCellLibrary& cell_library
)
{
  if ( debug ) {
    SbjDumper::dump(cout, sbjgraph);
  }

  MapRecord maprec{cell_library};

  maprec.init(sbjgraph);

  // FF のマッピングを行う．
  ff_map(sbjgraph, cell_library, maprec);

  // マッピング結果を maprec に記録する．
  record_cuts(sbjgraph, cell_library, maprec);

  // 定数０のセルを登録する．
  if ( cell_library.const0_func().cell_num() > 0 ) {
    const ClibCell& c0_cell = cell_library.const0_func().cell(0);
    maprec.set_const0(c0_cell.id());
  }

  // 定数１のセルを登録する．
  if ( cell_library.const1_func().cell_num() > 0 ) {
    const ClibCell& c1_cell = cell_library.const1_func().cell(0);
    maprec.set_const1(c1_cell.id());
  }

  // 最終的なネットワークを生成する．
  MapGen gen;
  gen.generate(sbjgraph, maprec);
  return BnNetwork{std::move(gen)};
}

// @brief FF のマッピングを行う．
void
AreaCover::ff_map(
  const SbjGraph& sbjgraph,
  const ClibCellLibrary& cell_library,
  MapRecord& maprec
)
{
  // FFの割り当て情報を作る．
  for (ymuint i = 0; i < 8; ++ i) {
    FFInfo& ff_info = mFFInfo[i];
    bool master_slave = false;
    bool has_clear = false;
    bool has_preset = false;
    if ( i & 1 ) {
      master_slave = true;
    }
    if ( i & 2U ) {
      has_clear = true;
    }
    if ( i & 4U ) {
      has_preset = true;
    }

    int min_cell_id = -1;
    //ClibFFInfo min_pin_info;
    ClibArea min_area = ClibArea::infty();
    auto& ff_class = cell_library.simple_ff_class(master_slave, has_clear, has_preset);
    for ( auto& ff_group: ff_class.cell_group_list() ) {
      for ( auto& cell: ff_group.cell_list() ) {
	ClibArea area = cell.area();
	if ( min_area > area ) {
	  min_area = area;
	  min_cell_id = cell.id();
	  //min_pin_info = ff_group.ff_info();
	}
      }
    }
    if ( min_cell_id != -1 ) {
      ff_info.mCellId = min_cell_id;
      //ff_info.mPinInfo = min_pin_info;
    }
    else {
      ff_info.mCellId = -1;
    }
  }

  SizeType ndff = sbjgraph.dff_num();
  for ( SizeType i = 0; i < ndff; ++ i ) {
    const SbjDff* dff = sbjgraph.dff(i);
    const SbjNode* clear = dff->clear();
    const SbjNode* preset = dff->preset();
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
    if ( ff_info1.mCellId != -1 ) {
      maprec.set_dff_match(dff, false, ff_info1.mCellId);
    }
    FFInfo& ff_info2 = mFFInfo[xsig];
    if ( ff_info2.mCellId != -1 ) {
      maprec.set_dff_match(dff, true, ff_info2.mCellId);
    }
  }
}

// @brief best cut の記録を行う．
void
AreaCover::record_cuts(
  const SbjGraph& sbjgraph,
  const ClibCellLibrary& cell_library,
  MapRecord& maprec
)
{
  int n = sbjgraph.node_num();
  mCostArray.resize(n * 2);
  int max_input = cell_library.pg_max_input();
  mWeight.resize(max_input);
  mLeafNum.clear();
  mLeafNum.resize(n, -1);

  const ClibCellGroup& inv_func = cell_library.inv_func();

  // 入力のコストを設定
  int ni = sbjgraph.input_num();
  for ( int i = 0; i < ni; ++ i ) {
    const SbjNode* node = sbjgraph.input(i);
    ASSERT_COND( node->is_input() );
    double& p_cost = cost(node, false);
    double& n_cost = cost(node, true);
    if ( sbjgraph.port(node) != nullptr ) {
      // 外部入力の場合，肯定の極性のみが利用可能
      p_cost = 0.0;
      n_cost = DBL_MAX;
      add_inv(node, true, inv_func, maprec);
    }
    else if ( sbjgraph.is_dff_output(node) ||
	      sbjgraph.is_latch_output(node) ) {
      // DFFとラッチの場合，肯定，否定のどちらの極性も利用可能
      #warning "dff/latch の利用可能な出力極性のチェックをする．"
      p_cost = 0.0;
      n_cost = 0.0;
    }
    else {
      // 肯定の極性のみが利用可能
      p_cost = 0.0;
      n_cost = DBL_MAX;
      add_inv(node, true, inv_func, maprec);
    }
  }

  // 論理ノードのコストを入力側から計算
  PatMatcher pat_match(cell_library);
  int np = cell_library.pg_pat_num();
  int nl = sbjgraph.logic_num();
  for ( int i = 0; i < nl; ++ i ) {
    const SbjNode* node = sbjgraph.logic(i);
    if ( debug ) {
      cout << endl
	   << "Processing Node[" << node->id() << "]" << endl;
    }
    double& p_cost = cost(node, false);
    double& n_cost = cost(node, true);
    p_cost = DBL_MAX;
    n_cost = DBL_MAX;
    for ( int pat_id = 0; pat_id < np; ++ pat_id ) {
      const ClibPatGraph& pat = cell_library.pg_pat(pat_id);
      int ni = pat.input_num();
      Cut cut(ni);
      if ( pat_match(node, pat, cut) ) {
	int rep_id = pat.rep_id();
	if ( debug ) {
	  cout << "Match with Pat#" << pat_id
	       << ", Rep#" << rep_id << endl;
	}
	const ClibCellClass& rep = cell_library.npn_class(rep_id);
	for ( auto& group: rep.cell_group_list() ) {
	  const ClibIOMap& iomap = group.iomap();
	  Cut c_cut(ni);
	  for ( int i = 0; i < ni; ++ i ) {
	    auto pinmap = iomap.input_map(i);
	    int pos = pinmap.id();
	    const SbjNode* inode = cut.leaf_node(pos);
	    bool iinv = cut.leaf_inv(pos);
	    if ( pinmap.inv() ) {
	      iinv = !iinv;
	    }
	    c_cut.set_leaf(i, inode, iinv);
	    mLeafNum[inode->id()] = i;
	  }
	  bool root_inv = pat.root_inv();
	  if ( iomap.output_map(0).inv() ) {
	    root_inv = !root_inv;
	  }
	  if ( debug ) {
	    cout << "  Group#" << group.id() << endl
		 << "    Root_inv = " << root_inv << endl;
	    for ( int i = 0; i < ni; ++ i ) {
	      cout << "    Leaf#" << i << ": ";
	      if ( c_cut.leaf_inv(i) ) {
		cout << "~";
	      }
	      cout << "Node[" << c_cut.leaf_node(i)->id() << "]" << endl;
	    }
	  }

	  double& c_cost = root_inv ? n_cost : p_cost;

	  for ( int i = 0; i < ni; ++ i ) {
	    mWeight[i] = 0.0;
	  }
	  calc_weight(node, 1.0);
	  for ( int i = 0; i < ni; ++ i ) {
	    mLeafNum[c_cut.leaf_node(i)->id()] = -1;
	  }

	  double leaf_cost = 0.0;
	  for ( int i = 0; i < ni; ++ i ) {
	    const SbjNode* leaf_node = c_cut.leaf_node(i);
	    bool leaf_inv = c_cut.leaf_inv(i);
	    leaf_cost += cost(leaf_node, leaf_inv) * mWeight[i];
	  }

	  for ( auto& cell: group.cell_list() ) {
	    double cur_cost = cell.area().value() + leaf_cost;
	    if ( debug ) {
	      cout << "      Cell = " << cell.name()
		   << ", cost = " << cur_cost << endl;
	    }
	    if ( c_cost >= cur_cost ) {
	      c_cost = cur_cost;
	      maprec.set_logic_match(node, root_inv, c_cut, cell.id());
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
    ASSERT_COND( has_match );
  }
}

// @brief 逆極性の解にインバーターを付加した解を追加する．
void
AreaCover::add_inv(
  const SbjNode* node,
  bool inv,
  const ClibCellGroup& inv_func,
  MapRecord& maprec
)
{
  if ( maprec.get_node_match(node, !inv).leaf_num() == 1 ) {
    // 逆極性の解が自分の解＋インバーターだった
    return;
  }

  double& cur_cost = cost(node, inv);
  double alt_cost = cost(node, !inv);
  int inv_cell_id = -1;
  double min_cost = DBL_MAX;
  for ( auto& cell: inv_func.cell_list() ) {
    double cost = cell.area().value();
    if ( min_cost > cost ) {
      min_cost = cost;
      inv_cell_id = cell.id();
    }
  }
  ASSERT_COND( inv_cell_id != -1 );
  min_cost += alt_cost;
  if ( cur_cost > min_cost ) {
    cur_cost = min_cost;
    maprec.set_inv_match(node, inv, inv_cell_id);
  }
}

// @brief node から各入力にいたる経路の重みを計算する．
void
AreaCover::calc_weight(
  const SbjNode* node,
  double cur_weight
)
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
    ASSERT_COND( !node->is_input() );

    const SbjNode* inode0 = node->fanin(0);
    double cur_weight0 = cur_weight / inode0->fanout_num();
    calc_weight(inode0, cur_weight0);
    node = node->fanin(1);
    cur_weight /= node->fanout_num();
  }
}

END_NAMESPACE_CELLMAP
