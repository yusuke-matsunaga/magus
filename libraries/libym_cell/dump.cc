
/// @file dump.cc
/// @brief CellLibrary の内容をダンプする関数
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/Cell.h"
#include "ym_cell/CellLibrary.h"
#include "ym_cell/CellPin.h"
#include "ym_cell/CellTiming.h"

#include "ym_logic/LogExpr.h"
#include "ym_utils/BinIO.h"


BEGIN_NAMESPACE_YM_CELL

/// @brief 内容をバイナリダンプする．
/// @param[in] s 出力先のストリーム
/// @param[in] library セルライブラリ
void
dump_library(ostream& s,
	     const CellLibrary& library)
{
  BinO bos(s);

  // 名前
  bos << library.name();

  // セル数
  ymuint32 nc = library.cell_num();
  bos << nc;
  for (ymuint i = 0; i < nc; ++ i) {
    const Cell* cell = library.cell(i);
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
	<< cell->area().value()
	<< ni
	<< no
	<< nio
	<< nbus
	<< nbundle;

    ymuint no2 = no + nio;
    for (ymuint opos = 0; opos < no2; ++ opos) {
      bos << cell->logic_expr(opos)
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
    ymuint32 nt = timing_list.size();
    bos << nt;
    for (ymuint32 j = 0; j < nt; ++ j) {
      const CellTiming* timing = timing_list[j];
      s << timing->intrinsic_rise().value()
	<< timing->intrinsic_fall().value()
	<< timing->slope_rise().value()
	<< timing->slope_fall().value()
	<< timing->rise_resistance().value()
	<< timing->fall_resistance().value();
    }

    // 入力ピンのダンプ
    for (ymuint32 ipin = 0; ipin < ni; ++ ipin) {
      const CellPin* pin = cell->input(ipin);
      bos << pin->name()
	  << pin->capacitance().value()
	  << pin->rise_capacitance().value()
	  << pin->fall_capacitance().value();
    }

    // 出力ピンのダンプ
    for (ymuint32 opin = 0; opin < no; ++ opin) {
      const CellPin* pin = cell->output(opin);
      bos << pin->name()
	  << pin->max_fanout().value()
	  << pin->min_fanout().value()
	  << pin->max_capacitance().value()
	  << pin->min_capacitance().value()
	  << pin->max_transition().value()
	  << pin->min_transition().value();
    }

    // 入出力ピンのダンプ
    for (ymuint32 iopin = 0; iopin < nio; ++ iopin) {
      const CellPin* pin = cell->output(iopin);
      bos << pin->name()
	  << pin->capacitance().value()
	  << pin->rise_capacitance().value()
	  << pin->fall_capacitance().value()
	  << pin->max_fanout().value()
	  << pin->min_fanout().value()
	  << pin->max_capacitance().value()
	  << pin->min_capacitance().value()
	  << pin->max_transition().value()
	  << pin->min_transition().value();
    }

    // タイミング情報のダンプ
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
	  bos << static_cast<ymuint8>(0)
	      << ipin
	      << opin
	      << p->second;
	}
      }
    }
    bos << static_cast<ymuint8>(0);
  }
}

BEGIN_NONAMESPACE

// タイミング情報を出力する．
void
display_timing(ostream& s,
	       const Cell* cell,
	       ymuint ipos,
	       ymuint opos,
	       tCellTimingSense sense)
{
  const CellTiming* timing = cell->timing(ipos, opos, sense);
  if ( timing ) {
    s << "  Timing:" << endl
      << "    Input Pin       = " << cell->input(ipos)->name() << endl
      << "    Output Pin      = " << cell->output(opos)->name() << endl
      << "    Sense           = ";
    if ( sense == kCellPosiUnate ) {
      s << "positive unate";
    }
    else if ( sense == kCellNegaUnate ) {
      s << "negative unate";
    }
    else {
      assert_not_reached(__FILE__, __LINE__);
    }
    s << endl
      << "    Rise Intrinsic  = " << timing->intrinsic_rise() << endl
      << "    Rise Resistance = " << timing->rise_resistance() << endl
      << "    Fall Intrinsic  = " << timing->intrinsic_fall() << endl
      << "    Fall Resistance = " << timing->fall_resistance() << endl;
  }
}

END_NONAMESPACE

void
display_library(ostream& s,
		const CellLibrary& library)
{
  ymuint n = library.cell_num();
  for (ymuint i = 0; i < n; ++ i) {
    const Cell* cell = library.cell(i);
    s << "Cell#" << cell->id() << " (" << cell->name() << ") : ";
    if ( cell->is_logic() ) {
      s << "Combinational Logic";
    }
    else if ( cell->is_ff() ) {
      s << "Flip-Flop";
    }
    else if ( cell->is_latch() ) {
      s << "Latch";
    }
    s << endl;
    s << "  area = " << cell->area() << endl;

    if ( cell->is_ff() ) {
      s << "  Next State         = " << cell->next_state_expr() << endl
	<< "  Clock              = " << cell->clock_expr() << endl;
      if ( !cell->clock2_expr().is_zero() ) {
	s << "  Clock2             = " << cell->clock2_expr() << endl;
      }
      if ( cell->has_clear() ) {
	s << "  Clear              = " << cell->clear_expr() << endl;
      }
      if ( cell->has_preset() ) {
	s << "  Preset             = " << cell->preset_expr() << endl;
      }
      if ( cell->has_clear() && cell->has_preset() ) {
	s << "  Clear Preset Var1  = " << cell->clear_preset_var1() << endl
	  << "  Clear Preset Var2  = " << cell->clear_preset_var2() << endl;
      }
    }
    if ( cell->is_latch() ) {
      s << "  Data In            = " << cell->data_in_expr() << endl
	<< "  Enable             = " << cell->enable_expr() << endl;
      if ( !cell->enable2_expr().is_zero() ) {
	s << "  Enable2            = " << cell->enable2_expr() << endl;
      }
      if ( cell->has_clear() ) {
	s << "  Clear              = " << cell->clear_expr() << endl;
      }
      if ( cell->has_preset() ) {
	s << "  Preset             = " << cell->preset_expr() << endl;
      }
      if ( cell->has_clear() && cell->has_preset() ) {
	s << "  Clear Preset Var1  = " << cell->clear_preset_var1() << endl
	  << "  Clear Preset Var2  = " << cell->clear_preset_var2() << endl;
      }
    }

    ymuint ni = cell->input_num();
    for (ymuint ipos = 0; ipos < ni; ++ ipos) {
      const CellPin* pin = cell->input(ipos);
      s << "  Input#" << ipos << ": " << pin->name() << endl
	<< "    Capacitance      = " << pin->capacitance() << endl
	<< "    Rise Capacitance = " << pin->rise_capacitance() << endl
	<< "    Fall Capacitance = " << pin->fall_capacitance() << endl;
    }

    ymuint no = cell->output_num();
    for (ymuint opos = 0; opos < no; ++ opos) {
      const CellPin* pin = cell->output(opos);
      s << "  Output#" << opos << ": " << pin->name()
	<< " = " << cell->logic_expr(opos) << endl;
      if ( !cell->tristate_expr(opos).is_zero() ) {
	s << "    Tristate         = " << cell->tristate_expr(opos) << endl;
      }
      s << "    Max Fanout       = " << pin->max_fanout() << endl
	<< "    Min Fanout       = " << pin->min_fanout() << endl
	<< "    Max Capacitance  = " << pin->max_capacitance() << endl
	<< "    Min Capacitance  = " << pin->min_capacitance() << endl
	<< "    Max Transition   = " << pin->max_transition() << endl
	<< "    Min Transition   = " << pin->min_transition() << endl;
    }

    ymuint nio = cell->inout_num();
    for (ymuint iopos = 0; iopos < nio; ++ iopos) {
      const CellPin* pin = cell->inout(iopos);
      s << "  Inout#" << iopos << ": " << pin->name() << endl
	<< "    Capacitance      = " << pin->capacitance() << endl
	<< "    Rise Capacitance = " << pin->rise_capacitance() << endl
	<< "    Fall Capacitance = " << pin->fall_capacitance() << endl
	<< "    Max Fanout       = " << pin->max_fanout() << endl
	<< "    Min Fanout       = " << pin->min_fanout() << endl
	<< "    Max Capacitance  = " << pin->max_capacitance() << endl
	<< "    Min Capacitance  = " << pin->min_capacitance() << endl
	<< "    Max Transition   = " << pin->max_transition() << endl
	<< "    Min Transition   = " << pin->min_transition() << endl;
    }

    ymuint ni2 = cell->input_num2();
    ymuint no2 = cell->output_num2();
    for (ymuint ipos = 0; ipos < ni2; ++ ipos) {
      for (ymuint opos = 0; opos < no2; ++ opos) {
	display_timing(s, cell, ipos, opos, kCellPosiUnate);
	display_timing(s, cell, ipos, opos, kCellNegaUnate);
      }
    }

#if 0
    for (ymuint j = 0; j < np; ++ j) {
      const CellPin* pin = cell->pin(j);
      s << "  Pin#" << pin->id() << ":" << endl
	<< "    Name             = " << pin->name() << endl;

      switch ( pin->direction() ) {
      case CellPin::kDirInput:
	s << "    Direction        = INPUT" << endl
	  << "    Capacitance      = " << pin->capacitance() << endl
	  << "    Rise Capacitance = " << pin->rise_capacitance() << endl
	  << "    Fall Capacitance = " << pin->fall_capacitance() << endl;
	break;

      case CellPin::kDirOutput:
	s << "    Direction        = OUTPUT" << endl;
	if ( pin->has_function() ) {
	  s << "    Function         = " << pin->function() << endl;
	}
	if ( pin->has_three_state() ) {
	  s << "    Three State      = " << pin->three_state() << endl;
	}
	s << "    Max Fanout       = " << pin->max_fanout() << endl
	  << "    Min Fanout       = " << pin->min_fanout() << endl
	  << "    Max Capacitance  = " << pin->max_capacitance() << endl
	  << "    Min Capacitance  = " << pin->min_capacitance() << endl
	  << "    Max Transition   = " << pin->max_transition() << endl
	  << "    Min Transition   = " << pin->min_transition() << endl;
	for (ymuint k = 0; k < np; ++ k) {
	  display_timing(s, cell, pin, k, kCellPosiUnate);
	  display_timing(s, cell, pin, k, kCellNegaUnate);
	}
	break;

      case CellPin::kDirInout:
	s << "    Direction        = INOUT" << endl;
	if ( pin->has_function() ) {
	  s << "    Function         = " << pin->function() << endl;
	}
	if ( pin->has_three_state() ) {
	  s << "    Three State      = " << pin->three_state() << endl;
	}
	s << "    Capacitance      = " << pin->capacitance() << endl
	  << "    Rise Capacitance = " << pin->rise_capacitance() << endl
	  << "    Fall Capacitance = " << pin->fall_capacitance() << endl
	  << "    Max Fanout       = " << pin->max_fanout() << endl
	  << "    Min Fanout       = " << pin->min_fanout() << endl
	  << "    Max Capacitance  = " << pin->max_capacitance() << endl
	  << "    Min Capacitance  = " << pin->min_capacitance() << endl
	  << "    Max Transition   = " << pin->max_transition() << endl
	  << "    Min Transition   = " << pin->min_transition() << endl;
	for (ymuint k = 0; k < np; ++ k) {
	  display_timing(s, cell, pin, k, kCellPosiUnate);
	  display_timing(s, cell, pin, k, kCellNegaUnate);
	}
	break;

      case CellPin::kDirInternal:
	s << "    Direction        = INTERNAL" << endl;
	break;

      default:
	assert_not_reached(__FILE__, __LINE__);
      }
    }
    s << endl;

    if ( cell->is_ff() ) {
      s << "  ff ( " << cell->var1_name()
	<< ", " << cell->var2_name() << " )" << endl
	<< "    next_state        = " << cell->next_state() << endl
	<< "    clocked_on        = " << cell->clocked_on() << endl;
      if ( !cell->clocked_on_also().is_zero() ) {
	s << "    clocked_on_also   = " << cell->clocked_on_also() << endl;
      }
      if ( !cell->clear().is_zero() ) {
	s << "    clear             = " << cell->clear() << endl;
      }
      if ( !cell->preset().is_zero() ) {
	s << "    preset            = " << cell->preset() << endl;
      }
      if ( !cell->clear().is_zero() && !cell->preset().is_zero() ) {
	s << "    clear_preset_var1 = " << cell->clear_preset_var1() << endl
	  << "    clear_preset_var2 = " << cell->clear_preset_var2() << endl;
      }
    }
    else if ( cell->is_latch() ) {
      s << "  latch ( " << cell->var1_name()
	<< ", " << cell->var2_name() << " )" << endl
	<< "    data_in           = " << cell->data_in() << endl
	<< "    enable            = " << cell->enable() << endl;
      if ( !cell->enable().is_zero() ) {
	s << "    enable_also       = " << cell->enable_also() << endl;
      }
      if ( !cell->clear().is_zero() ) {
	s << "    clear             = " << cell->clear() << endl;
      }
      if ( !cell->preset().is_zero() ) {
	s << "    preset            = " << cell->preset() << endl;
      }
      if ( !cell->clear().is_zero() && !cell->preset().is_zero() ) {
	s << "    clear_preset_var1 = " << cell->clear_preset_var1() << endl
	  << "    clear_preset_var2 = " << cell->clear_preset_var2() << endl;
      }
    }

    ymuint np = cell->pin_num();
    for (ymuint j = 0; j < np; ++ j) {
      const CellPin* pin = cell->pin(j);
      s << "  Pin#" << pin->id() << ":" << endl
	<< "    Name             = " << pin->name() << endl;

      switch ( pin->direction() ) {
      case CellPin::kDirInput:
	s << "    Direction        = INPUT" << endl
	  << "    Capacitance      = " << pin->capacitance() << endl
	  << "    Rise Capacitance = " << pin->rise_capacitance() << endl
	  << "    Fall Capacitance = " << pin->fall_capacitance() << endl;
	break;

      case CellPin::kDirOutput:
	s << "    Direction        = OUTPUT" << endl;
	if ( pin->has_function() ) {
	  s << "    Function         = " << pin->function() << endl;
	}
	if ( pin->has_three_state() ) {
	  s << "    Three State      = " << pin->three_state() << endl;
	}
	s << "    Max Fanout       = " << pin->max_fanout() << endl
	  << "    Min Fanout       = " << pin->min_fanout() << endl
	  << "    Max Capacitance  = " << pin->max_capacitance() << endl
	  << "    Min Capacitance  = " << pin->min_capacitance() << endl
	  << "    Max Transition   = " << pin->max_transition() << endl
	  << "    Min Transition   = " << pin->min_transition() << endl;
	for (ymuint k = 0; k < np; ++ k) {
	  display_timing(s, cell, pin, k, kCellPosiUnate);
	  display_timing(s, cell, pin, k, kCellNegaUnate);
	}
	break;

      case CellPin::kDirInout:
	s << "    Direction        = INOUT" << endl;
	if ( pin->has_function() ) {
	  s << "    Function         = " << pin->function() << endl;
	}
	if ( pin->has_three_state() ) {
	  s << "    Three State      = " << pin->three_state() << endl;
	}
	s << "    Capacitance      = " << pin->capacitance() << endl
	  << "    Rise Capacitance = " << pin->rise_capacitance() << endl
	  << "    Fall Capacitance = " << pin->fall_capacitance() << endl
	  << "    Max Fanout       = " << pin->max_fanout() << endl
	  << "    Min Fanout       = " << pin->min_fanout() << endl
	  << "    Max Capacitance  = " << pin->max_capacitance() << endl
	  << "    Min Capacitance  = " << pin->min_capacitance() << endl
	  << "    Max Transition   = " << pin->max_transition() << endl
	  << "    Min Transition   = " << pin->min_transition() << endl;
	for (ymuint k = 0; k < np; ++ k) {
	  display_timing(s, cell, pin, k, kCellPosiUnate);
	  display_timing(s, cell, pin, k, kCellNegaUnate);
	}
	break;

      case CellPin::kDirInternal:
	s << "    Direction        = INTERNAL" << endl;
	break;

      default:
	assert_not_reached(__FILE__, __LINE__);
      }
    }
#endif
    s << endl;
  }

}

END_NAMESPACE_YM_CELL
