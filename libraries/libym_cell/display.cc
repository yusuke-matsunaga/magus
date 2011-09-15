
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
#include "ym_cell/CellClass.h"
#include "ym_cell/CellGroup.h"
#include "ym_cell/CellPatGraph.h"

#include "ym_logic/LogExpr.h"
#include "ym_logic/NpnMapM.h"
#include "ym_utils/BinIO.h"


BEGIN_NAMESPACE_YM_CELL

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

// セルクラスの情報を出力する．
void
display_class(ostream& s,
	      const char* title,
	      const CellClass* cclass)
{
  s << title << endl;
  for (ymuint i = 0; i < cclass->group_num(); ++ i) {
    const CellGroup* group = cclass->cell_group(i);
    s << "  Group: Map = " << group->map() << endl
      << "Cell = ";
    for (ymuint j = 0; j < group->cell_num(); ++ j) {
      const Cell* cell = group->cell(j);
      s << " " << cell->name();
    }
    s << endl;
  }
  s << endl;
}

// セルグループの情報を出力する．
void
display_group(ostream& s,
	      const char* title,
	      const CellGroup* group)
{
  s << title << endl
    << "  Cell =";
  for (ymuint i = 0; i < group->cell_num(); ++ i) {
    const Cell* cell = group->cell(i);
    s << " " << cell->name();
  }
  s << endl;
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
      s << "  Output#" << opos << ": " << pin->name() << endl;
      if ( cell->has_logic(opos) ) {
	s << "    Logic            = " << cell->logic_expr(opos) << endl;
	if ( cell->has_tristate(opos) ) {
	  s << "    Tristate         = " << cell->tristate_expr(opos) << endl;
	}
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
      s << "  Inout#" << iopos << ": " << pin->name() << endl;
      if ( cell->has_logic(iopos) ) {
	s << "    Logic            = " << cell->logic_expr(iopos) << endl;
	if ( cell->has_tristate(iopos) ) {
	  s << "    Tristate         = " << cell->tristate_expr(iopos) << endl;
	}
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

  s << "Cell Group" << endl;
  ymuint nc = library.npn_class_num();
  for (ymuint i = 0; i < nc; ++ i) {
    const CellClass* cclass = library.npn_class(i);
    ostringstream buf;
    buf << "Class#" << i;
    display_class(s, buf.str().c_str(), cclass);
  }

  display_group(s, "Const0 Group", library.const0_func());
  display_group(s, "Const1 Group", library.const1_func());
  display_group(s, "Buffer Group", library.buf_func());
  display_group(s, "Inverter Group", library.inv_func());

  display_class(s, "DFF Class", library.simple_ff_class(false, false));
  display_class(s, "DFF_R Class", library.simple_ff_class(true, false));
  display_class(s, "DFF_S Class", library.simple_ff_class(false, true));
  display_class(s, "DFF_RS Class", library.simple_ff_class(true, true));

  s << "==== PatMgr dump start ====" << endl;

  // ノードの種類の出力
  ymuint nn = library.node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    s << "Node#" << i << ": ";
    switch ( library.node_type(i) ) {
    case kCellPatInput: s << "INPUT#" << library.input_id(i) ; break;
    case kCellPatAnd:   s << "AND"; break;
    case kCellPatXor:   s << "XOR"; break;
    default: assert_not_reached(__FILE__, __LINE__);
    }
    s << endl;
  }
  s << endl;

  // 枝の情報の出力
  ymuint ne = library.edge_num();
  for (ymuint i = 0; i < ne; ++ i) {
    s << "Edge#" << i << ": " << library.edge_from(i)
      << " -> " << library.edge_to(i) << "(" << library.edge_pos(i) << ")";
    if ( library.edge_inv(i) ) {
      s << " ***";
    }
    s << endl;
  }
  s << endl;

  // パタングラフの情報の出力
  ymuint np = library.pat_num();
  for (ymuint i = 0; i < np; ++ i) {
    const CellPatGraph& pat = library.pat(i);
    s << "Pat#" << i << ": "
      << "Rep#" << pat.rep_id() << ": ";
    if ( pat.root_inv() ) {
      s << "[inv]";
    }
    s << "(" << pat.input_num() << "), ";
    ymuint n = pat.edge_num();
    for (ymuint i = 0; i < n; ++ i) {
      s << " " << pat.edge(i);
    }
    s << endl;
  }

  s << "==== PatMgr dump end ====" << endl;
}

END_NAMESPACE_YM_CELL
