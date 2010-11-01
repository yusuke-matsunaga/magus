
/// @file libym_cell/dump.cc
/// @brief CellLibrary の内容をダンプする関数
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/Cell.h"
#include "ym_cell/CellLibrary.h"
#include "ym_cell/CellPin.h"
#include "ym_cell/CellTiming.h"
#include "ym_lexp/LogExpr.h"


BEGIN_NAMESPACE_YM_CELL

BEGIN_NONAMESPACE

void
dump_word(ostream& s,
	  ymuint val)
{
  static char buf[4];
  buf[0] = val & 255U; val >>= 8;
  buf[1] = val & 255U; val >>= 8;
  buf[2] = val & 255U; val >>= 8;
  buf[3] = val & 255U;

  s.write(buf, 4);
}

void
dump_str(ostream& s,
	 const string& str)
{
  ymuint l = strlen(str.c_str());
  dump_word(s, l);
  s.write(str.c_str(), l);
}

void
dump_real(ostream& s,
	  double val)
{
  static char buf[sizeof(double)];
  *(reinterpret_cast<double*>(buf)) = val;

  s.write(buf, sizeof(double));
}

void
dump_timing(ostream& s,
	    const CellTiming* timing)
{
  dump_real(s, timing->intrinsic_rise().value());
  dump_real(s, timing->rise_resistance().value());
  dump_real(s, timing->intrinsic_fall().value());
  dump_real(s, timing->fall_resistance().value());
}

END_NONAMESPACE

/// @brief 内容をバイナリダンプする．
void
dump_library(ostream& s,
	     const CellLibrary& library)
{
  // セル数
  ymuint nc = library.cell_num();
  dump_word(s, nc);
  for (ymuint i = 0; i < nc; ++ i) {
    const Cell* cell = library.cell(i);
    dump_str(s, cell->name());
    dump_real(s, cell->area().value());
    ymuint np = cell->pin_num();
    dump_word(s, np);
    for (ymuint j = 0; j < np; ++ j) {
      const CellPin* pin = cell->pin(j);
      switch ( pin->direction() ) {
      case kDirInput:
	dump_word(s, 1);
	dump_str(s, pin->name());
	dump_real(s, pin->capacitance().value());
	dump_real(s, pin->rise_capacitance().value());
	dump_real(s, pin->fall_capacitance().value());
	break;

      case kDirOutput:
	dump_word(s, 2);
	dump_str(s, pin->name());
	dump_real(s, pin->max_fanout().value());
	dump_real(s, pin->min_fanout().value());
	dump_real(s, pin->max_capacitance().value());
	dump_real(s, pin->min_capacitance().value());
	dump_real(s, pin->max_transition().value());
	dump_real(s, pin->min_transition().value());
	for (ymuint k = 0; k < np; ++ k) {
	  const CellTiming* p_timing = pin->timing(k, kSensePosiUnate);
	  if ( p_timing ) {
	    dump_word(s, 1);
	    dump_word(s, k);
	    dump_timing(s, p_timing);
	  }
	  const CellTiming* n_timing = pin->timing(k, kSenseNegaUnate);
	  if ( p_timing ) {
	    dump_word(s, 2);
	    dump_word(s, k);
	    dump_timing(s, n_timing);
	  }
	}
	dump_word(s, 0); // timing 情報が終わった印
	break;

      case kDirInout:
	dump_word(s, 3);
	dump_str(s, pin->name());
	dump_real(s, pin->capacitance().value());
	dump_real(s, pin->rise_capacitance().value());
	dump_real(s, pin->fall_capacitance().value());
	dump_real(s, pin->max_fanout().value());
	dump_real(s, pin->min_fanout().value());
	dump_real(s, pin->max_capacitance().value());
	dump_real(s, pin->min_capacitance().value());
	dump_real(s, pin->max_transition().value());
	dump_real(s, pin->min_transition().value());
	for (ymuint k = 0; k < np; ++ k) {
	  const CellTiming* p_timing = pin->timing(k, kSensePosiUnate);
	  if ( p_timing ) {
	    dump_word(s, 1);
	    dump_word(s, k);
	    dump_timing(s, p_timing);
	  }
	  const CellTiming* n_timing = pin->timing(k, kSenseNegaUnate);
	  if ( p_timing ) {
	    dump_word(s, 2);
	    dump_word(s, k);
	    dump_timing(s, n_timing);
	  }
	}
	dump_word(s, 0); // timing 情報が終わった印
	break;
      }
    }
  }
}


void
display_library(ostream& s,
		const CellLibrary& library)
{
  ymuint n = library.cell_num();
  for (ymuint i = 0; i < n; ++ i) {
    const Cell* cell = library.cell(i);
    s << "Cell (" << cell->name() << ")" << endl;
    s << "  area = " << cell->area() << endl;
    ymuint np = cell->pin_num();
    for (ymuint j = 0; j < np; ++ j) {
      const CellPin* pin = cell->pin(j);
      switch ( pin->direction() ) {
      case kDirInput:
	s << "  Input Pin (" << pin->name() << ")" << endl;
	s << "    Capacitance = " << pin->capacitance() << endl
	     << "    Rise Capacitance = " << pin->rise_capacitance() << endl
	     << "    Fall Capacitance = " << pin->fall_capacitance() << endl;
	break;

      case kDirOutput:
	s << "  Output Pin(" << pin->name() << ")" << endl;
	s << "    Function = " << pin->function() << endl
	     << "    Max Fanout = " << pin->max_fanout() << endl
	     << "    Min Fanout = " << pin->min_fanout() << endl
	     << "    Max Capacitance = " << pin->max_capacitance() << endl
	     << "    Min Capacitance = " << pin->min_capacitance() << endl
	     << "    Max Transition = " << pin->max_transition() << endl
	     << "    Min Transition = " << pin->min_transition() << endl;
	for (ymuint k = 0; k < np; ++ k) {
	  const CellTiming* timing = pin->timing(k, kSensePosiUnate);
	  if ( timing ) {
	    s << "    Timing ( PosiUnate )" << endl
		 << "      Related Pin = " << cell->pin(k)->name() << endl
		 << "      Rise Intrinsic = " << timing->intrinsic_rise() << endl
		 << "      Rise Resistance = " << timing->rise_resistance() << endl
		 << "      Fall Intrinsic = " << timing->intrinsic_fall() << endl
		 << "      Fall Resistance = " << timing->fall_resistance() << endl;
	  }
	  timing = pin->timing(k, kSenseNegaUnate);
	  if ( timing ) {
	    s << "    Timing ( NegaUnate )" << endl
		 << "      Related Pin = " << cell->pin(k)->name() << endl
		 << "      Rise Intrinsic = " << timing->intrinsic_rise() << endl
		 << "      Rise Resistance = " << timing->rise_resistance() << endl
		 << "      Fall Intrinsic = " << timing->intrinsic_fall() << endl
		 << "      Fall Resistance = " << timing->fall_resistance() << endl;
	  }
	}
	break;

      case kDirInout:
	s << "  Inout Pin (" << pin->name() << ")" << endl;
	s << "    Capacitance = " << pin->capacitance() << endl
	  << "    Rise Capacitance = " << pin->rise_capacitance() << endl
	  << "    Fall Capacitance = " << pin->fall_capacitance() << endl;
	s << "    Function = " << pin->function() << endl
	     << "    Max Fanout = " << pin->max_fanout() << endl
	     << "    Min Fanout = " << pin->min_fanout() << endl
	     << "    Max Capacitance = " << pin->max_capacitance() << endl
	     << "    Min Capacitance = " << pin->min_capacitance() << endl
	     << "    Max Transition = " << pin->max_transition() << endl
	     << "    Min Transition = " << pin->min_transition() << endl;
	for (ymuint k = 0; k < np; ++ k) {
	  const CellTiming* timing = pin->timing(k, kSensePosiUnate);
	  if ( timing ) {
	    s << "    Timing ( PosiUnate )" << endl
		 << "      Related Pin = " << cell->pin(k)->name() << endl
		 << "      Rise Intrinsic = " << timing->intrinsic_rise() << endl
		 << "      Rise Resistance = " << timing->rise_resistance() << endl
		 << "      Fall Intrinsic = " << timing->intrinsic_fall() << endl
		 << "      Fall Resistance = " << timing->fall_resistance() << endl;
	  }
	  timing = pin->timing(k, kSenseNegaUnate);
	  if ( timing ) {
	    s << "    Timing ( NegaUnate )" << endl
		 << "      Related Pin = " << cell->pin(k)->name() << endl
		 << "      Rise Intrinsic = " << timing->intrinsic_rise() << endl
		 << "      Rise Resistance = " << timing->rise_resistance() << endl
		 << "      Fall Intrinsic = " << timing->intrinsic_fall() << endl
		 << "      Fall Resistance = " << timing->fall_resistance() << endl;
	  }
	}
	break;
      }
    }
    s << endl;
  }
}

END_NAMESPACE_YM_CELL


