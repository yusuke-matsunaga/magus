
/// @file libym_cell/ci/restore.cc
/// @brief バイナリファイルを読み込む関数
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CiLibrary.h"
#include "CiPin.h"
#include "ym_utils/BinIO.h"


BEGIN_NAMESPACE_YM_CELL

const CellLibrary*
restore_library(istream& s)
{
  string name = BinIO::read_str(s);

  CiLibrary* library = new CiLibrary(name);

  ymuint nc = BinIO::read_32(s);
  library->set_cell_num(nc);

  for (ymuint i = 0; i < nc; ++ i) {
    ymuint type = BinIO::read_8(s);
    ShString name( BinIO::read_str(s) );
    CellArea area( BinIO::read_double(s) );
    ymuint np = BinIO::read_32(s);
    ymuint nb = BinIO::read_32(s);
    ymuint nc = BinIO::read_32(s);
    CiCell* cell = NULL;
    switch ( type ) {
    case 0: // kLogic
      cell = library->new_logic_cell(i, name, area, np, nb, nc);
      break;

    case 1: // kFF
      {
	ShString var1;
	ShString var2;
	LogExpr next_state;
	LogExpr clocked_on;
	LogExpr clocked_on_also;
	LogExpr clear;
	LogExpr preset;
	ymuint clear_preset_var1;
	ymuint clear_preset_var2;
#warning "TODO:未完"
	cell = library->new_ff_cell(i, name, area,
				    var1, var2,
				    next_state, clocked_on, clocked_on_also,
				    clear, preset,
				    clear_preset_var1,
				    clear_preset_var2,
				    np, nb, nc);
      }
      break;

    case 2: // kLatch
      {
	ShString var1;
	ShString var2;
	LogExpr data_in;
	LogExpr enable;
	LogExpr enable_also;
	LogExpr clear;
	LogExpr preset;
	ymuint clear_preset_var1;
	ymuint clear_preset_var2;
#warning "TODO:未完"
	cell = library->new_latch_cell(i, name, area,
				       var1, var2,
				       data_in, enable, enable_also,
				       clear, preset,
				       clear_preset_var1,
				       clear_preset_var2,
				       np, nb, nc);
      }
      break;

    case 3: // kFSM
#warning "TODO: 未完"
      break;
    }

    ymuint nt = BinIO::read_32(s);
    vector<const CellTiming*> timing_list(nt);
    for (ymuint j = 0; j < nt; ++ j) {
      CellTime i_r( BinIO::read_double(s) );
      CellTime i_f( BinIO::read_double(s) );
      CellTime s_r( BinIO::read_double(s) );
      CellTime s_f( BinIO::read_double(s) );
      CellResistance r_r( BinIO::read_double(s) );
      CellResistance f_r( BinIO::read_double(s) );
      timing_list[j] = library->new_timing(j, CellTiming::kTimingCombinational,
					   i_r, i_f, s_r, s_f, r_r, f_r);
    }

    for (ymuint j = 0; j < np; ++ j) {
      ShString name( BinIO::read_str(s) );
      ymuint d = BinIO::read_8(s);
      switch ( d ) {
      case 1: // 入力
	{
	  CellCapacitance cap( BinIO::read_double(s) );
	  CellCapacitance r_cap( BinIO::read_double(s) );
	  CellCapacitance f_cap( BinIO::read_double(s) );
	  library->new_cell_input(cell, j, name, cap, r_cap, f_cap);
	}
	break;

      case 2: // 出力
	{
	  CellCapacitance max_f( BinIO::read_double(s) );
	  CellCapacitance min_f( BinIO::read_double(s) );
	  CellCapacitance max_c( BinIO::read_double(s) );
	  CellCapacitance min_c( BinIO::read_double(s) );
	  CellTime max_t( BinIO::read_double(s) );
	  CellTime min_t( BinIO::read_double(s) );
	  library->new_cell_output(cell, j, name,
				   max_f, min_f,
				   max_c, min_c,
				   max_t, min_t);
	  for ( ; ; ) {
	    ymuint unate = BinIO::read_8(s);
	    if ( unate == 0 ) break;
	    if ( unate == 1 ) {
	      ymuint pin_id = BinIO::read_32(s);
	      ymuint timing_id = BinIO::read_32(s);
	      const CellTiming* timing = timing_list[timing_id];
	      library->set_opin_timing(cell, j, pin_id,
				       CellPin::kSensePosiUnate, timing);
	    }
	    else if ( unate == 2 ) {
	      ymuint pin_id = BinIO::read_32(s);
	      ymuint timing_id = BinIO::read_32(s);
	      const CellTiming* timing = timing_list[timing_id];
	      library->set_opin_timing(cell, j, pin_id,
				       CellPin::kSenseNegaUnate, timing);
	    }
	  }
	}
	break;

      case 3: // 入出力
	{
	  CellCapacitance cap( BinIO::read_double(s) );
	  CellCapacitance r_cap( BinIO::read_double(s) );
	  CellCapacitance f_cap( BinIO::read_double(s) );
	  CellCapacitance max_f( BinIO::read_double(s) );
	  CellCapacitance min_f( BinIO::read_double(s) );
	  CellCapacitance max_c( BinIO::read_double(s) );
	  CellCapacitance min_c( BinIO::read_double(s) );
	  CellTime max_t( BinIO::read_double(s) );
	  CellTime min_t( BinIO::read_double(s) );
	  library->new_cell_inout(cell, j, name,
				  cap, r_cap, f_cap,
				  max_f, min_f,
				  max_c, min_c,
				  max_t, min_t);
	  for ( ; ; ) {
	    ymuint unate = BinIO::read_32(s);
	    if ( unate == 0 ) break;
	    if ( unate == 1 ) {
	      ymuint pin_id = BinIO::read_32(s);
	      ymuint timing_id = BinIO::read_32(s);
	      const CellTiming* timing = timing_list[timing_id];
	      library->set_opin_timing(cell, j, pin_id,
				       CellPin::kSensePosiUnate, timing);
	    }
	    else if ( unate == 2 ) {
	      ymuint pin_id = BinIO::read_32(s);
	      ymuint timing_id = BinIO::read_32(s);
	      const CellTiming* timing = timing_list[timing_id];
	      library->set_opin_timing(cell, j, pin_id,
				       CellPin::kSenseNegaUnate, timing);
	    }
	  }
	}
	break;

      case 4: // 内部ピン
	library->new_cell_internal(cell, j, name);
	break;
      }
    }
  }

  return library;
}

END_NAMESPACE_YM_CELL
