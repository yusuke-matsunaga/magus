
/// @file dump.cc
/// @brief CellLibrary の内容をダンプする関数
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CiLibrary.h"

#include "ym_cell/Cell.h"
#include "ym_cell/CellPin.h"
#include "ym_cell/CellTiming.h"
#include "ym_cell/CellClass.h"
#include "ym_cell/CellGroup.h"
#include "ym_cell/CellPatGraph.h"

#include "ym_logic/LogExpr.h"
#include "ym_logic/NpnMapM.h"
#include "ym_utils/BinIO.h"


BEGIN_NAMESPACE_YM_CELL

/// @brief 内容をバイナリダンプする．
/// @param[in] s 出力先のストリーム
void
CiLibrary::dump(ostream& s) const
{
  BinO bos(s);

  // 名前
  bos << name();

  // セル数
  ymuint32 nc = cell_num();
  bos << nc;
  for (ymuint i = 0; i < nc; ++ i) {
    const Cell* cell = this->cell(i);
    ymuint8 tid = 0;
    if ( cell->is_logic() ) {
      tid = 0;
    }
    else if ( cell->is_ff() ) {
      tid = 1;
    }
    else if ( cell->is_latch() ) {
      tid = 2;
    }
    else if ( cell->is_seq() ) {
      tid = 3;
    }
    else {
      // 無視？
      assert_not_reached(__FILE__, __LINE__);
    }
    ymuint32 ni = cell->input_num();
    ymuint32 no = cell->output_num();
    ymuint32 nio = cell->inout_num();
    ymuint32 nbus = cell->bus_num();
    ymuint32 nbundle = cell->bundle_num();

    bos << tid
	<< cell->name()
	<< cell->area()
	<< ni
	<< no
	<< nio
	<< nbus
	<< nbundle;

    ymuint no2 = no + nio;
    for (ymuint opos = 0; opos < no2; ++ opos) {
      bos << cell->has_logic()
	  << cell->logic_expr(opos)
	  << cell->tristate_expr(opos);
    }

    if ( cell->is_ff() ) {
      bos << cell->next_state_expr()
	  << cell->clock_expr()
	  << cell->clock2_expr()
	  << cell->clear_expr()
	  << cell->preset_expr()
	  << static_cast<ymuint8>(cell->clear_preset_var1())
	  << static_cast<ymuint8>(cell->clear_preset_var2());
    }
    else if ( cell->is_latch() ) {
      bos << cell->data_in_expr()
	  << cell->enable_expr()
	  << cell->enable2_expr()
	  << cell->clear_expr()
	  << cell->preset_expr()
	  << static_cast<ymuint8>(cell->clear_preset_var1())
	  << static_cast<ymuint8>(cell->clear_preset_var2());
    }

    // 入力ピンのダンプ
    for (ymuint32 ipin = 0; ipin < ni; ++ ipin) {
      const CellPin* pin = cell->input(ipin);
      bos << pin->name()
	  << pin->capacitance()
	  << pin->rise_capacitance()
	  << pin->fall_capacitance();
    }

    // 出力ピンのダンプ
    for (ymuint32 opin = 0; opin < no; ++ opin) {
      const CellPin* pin = cell->output(opin);
      bos << pin->name()
	  << pin->max_fanout()
	  << pin->min_fanout()
	  << pin->max_capacitance()
	  << pin->min_capacitance()
	  << pin->max_transition()
	  << pin->min_transition();
    }

    // 入出力ピンのダンプ
    for (ymuint32 iopin = 0; iopin < nio; ++ iopin) {
      const CellPin* pin = cell->output(iopin);
      bos << pin->name()
	  << pin->capacitance()
	  << pin->rise_capacitance()
	  << pin->fall_capacitance()
	  << pin->max_fanout()
	  << pin->min_fanout()
	  << pin->max_capacitance()
	  << pin->min_capacitance()
	  << pin->max_transition()
	  << pin->min_transition();
    }

    // タイミング情報のID -> 通し番号のマップ
    hash_map<ymuint32, ymuint32> timing_map;
    // タイミング情報のリスト
    vector<const CellTiming*> timing_list;
    for (ymuint32 ipos = 0; ipos < ni + nio; ++ ipos) {
      for (ymuint32 opos = 0; opos < no + nio; ++ opos) {
	const CellTiming* timing_p = cell->timing(ipos, opos, kCellPosiUnate);
	if ( timing_p ) {
	  if ( timing_map.count(timing_p->id()) == 0 ) {
	    ymuint pos = timing_list.size();
	    timing_map.insert(make_pair(timing_p->id(), pos));
	    timing_list.push_back(timing_p);
	  }
	}
	const CellTiming* timing_n = cell->timing(ipos, opos, kCellNegaUnate);
	if ( timing_n ) {
	  if ( timing_map.count(timing_n->id()) == 0 ) {
	    ymuint pos = timing_list.size();
	    timing_map.insert(make_pair(timing_n->id(), pos));
	    timing_list.push_back(timing_n);
	  }
	}
      }
    }

    // タイミング情報のダンプ
    ymuint32 nt = timing_list.size();
    bos << nt;
    for (ymuint32 j = 0; j < nt; ++ j) {
      const CellTiming* timing = timing_list[j];
      bos << timing->intrinsic_rise()
	  << timing->intrinsic_fall()
	  << timing->slope_rise()
	  << timing->slope_fall()
	  << timing->rise_resistance()
	  << timing->fall_resistance();
    }
    for (ymuint32 ipin = 0; ipin < ni + nio; ++ ipin) {
      for (ymuint32 opin = 0; opin < no + nio; ++ opin) {
	const CellTiming* timing_p = cell->timing(ipin, opin, kCellPosiUnate);
	if ( timing_p ) {
	  hash_map<ymuint, ymuint32>::iterator p = timing_map.find(timing_p->id());
	  assert_cond( p != timing_map.end(), __FILE__, __LINE__);
	  bos << static_cast<ymuint8>(1)
	      << ipin
	      << opin
	      << p->second;
	}
	const CellTiming* timing_n = cell->timing(ipin, opin, kCellNegaUnate);
	if ( timing_n ) {
	  hash_map<ymuint, ymuint>::iterator p = timing_map.find(timing_n->id());
	  assert_cond( p != timing_map.end(), __FILE__, __LINE__);
	  bos << static_cast<ymuint8>(2)
	      << ipin
	      << opin
	      << p->second;
	}
      }
    }
    bos << static_cast<ymuint8>(0);
  }

  // セルクラスの個数だけダンプする．
  ymuint32 ncc = npn_class_num();
  bos << ncc;

  // セルグループ情報のダンプ
  ymuint32 ng = group_num();
  bos << ng;
  for (ymuint g = 0; g < ng; ++ g) {
    const CellGroup* group = this->group(g);
    ymuint32 parent_id = group->cell_class()->id();
    ymuint32 nc = group->cell_num();
    bos << parent_id
	<< group->map()
	<< nc;
    for (ymuint k = 0; k < nc; ++ k) {
      const Cell* cell = group->cell(k);
      ymuint32 cell_id = cell->id();
      bos << cell_id;
    }
  }

  // セルクラス情報のダンプ
  for (ymuint i = 0; i < ncc; ++ i) {
    const CellClass* cclass = npn_class(i);

    // 同位体変換情報のダンプ
    const vector<NpnMapM>& idmap_list = cclass->idmap_list();
    ymuint32 nm = idmap_list.size();
    bos << nm;
    for (vector<NpnMapM>::const_iterator p = idmap_list.begin();
	 p != idmap_list.end(); ++ p) {
      bos << *p;
    }

    // グループ情報のダンプ
    ymuint32 ng = cclass->group_num();
    bos << ng;
    for (ymuint j = 0; j < ng; ++ j) {
      const CellGroup* group = cclass->cell_group(j);
      ymuint32 group_id = group->id();
      bos << group_id;
    }
  }

  // 組み込み型の情報のダンプ
  for (ymuint i = 0; i < 4; ++ i) {
    ymuint32 group_id = mLogicGroup[i]->id();
    bos << group_id;
  }
  for (ymuint i = 0; i < 4; ++ i) {
    ymuint32 class_id = mFFClass[i]->id();
    bos << class_id;
  }
  for (ymuint i = 0; i < 4; ++ i) {
    ymuint32 class_id = mLatchClass[i]->id();
    bos << class_id;
  }

  // パタングラフの情報のダンプ
  mPatMgr.dump(bos);
}

END_NAMESPACE_YM_CELL
