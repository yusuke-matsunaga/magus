
/// @file restore.cc
/// @brief バイナリファイルを読み込む関数
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CiLibrary.h"
#include "CiCell.h"
#include "CiPin.h"
#include "ym_utils/BinIO.h"


BEGIN_NAMESPACE_YM_CELL

const CellLibrary*
restore_library(istream& s)
{
  BinI bis(s);

  string name;
  bis >> name;

  CiLibrary* library = new CiLibrary(name);

  ymuint32 nc;
  bis >> nc;
  library->set_cell_num(nc);

  for (ymuint i = 0; i < nc; ++ i) {
    ymuint8 type;
    string name_val;
    double aval;
    ymuint32 ni;
    ymuint32 no;
    ymuint32 nio;
    ymuint32 nbus;
    ymuint32 nbundle;
    bis >> type
	>> name_val
	>> aval
	>> ni
	>> no
	>> nio
	>> nbus
	>> nbundle;
    ShString name( name_val );
    CellArea area( aval );
    ymuint no2 = no + nio;
    vector<LogExpr> logic_array(no2);
    vector<LogExpr> tristate_array(no2);
    for (ymuint opos = 0; opos < no2; ++ opos) {
      bis >> logic_array[opos]
	  >> tristate_array[opos];
    }

    CiCell* cell = NULL;
    switch ( type ) {
    case 0: // kLogic
      cell = library->new_logic_cell(i, name, area, ni, no, nio, nbus, nbundle,
				     logic_array, tristate_array);
      break;

    case 1: // kFF
      {
	LogExpr next_state;
	LogExpr clocked_on;
	LogExpr clocked_on_also;
	LogExpr clear;
	LogExpr preset;
	ymuint clear_preset_var1;
	ymuint clear_preset_var2;
	bis >> next_state
	    >> clocked_on
	    >> clocked_on_also
	    >> clear
	    >> preset;
	cell = library->new_ff_cell(i, name, area, ni, no, nio, nbus, nbundle,
				    logic_array, tristate_array,
				    next_state,
				    clocked_on, clocked_on_also,
				    clear, preset);
      }
      break;

    case 2: // kLatch
      {
	LogExpr data_in;
	LogExpr enable;
	LogExpr enable_also;
	LogExpr clear;
	LogExpr preset;
	ymuint clear_preset_var1;
	ymuint clear_preset_var2;
	bis >> data_in
	    >> enable
	    >> enable_also
	    >> clear
	    >> preset;
	cell = library->new_latch_cell(i, name, area, ni, no, nio, nbus, nbundle,
				       logic_array, tristate_array,
				       data_in,
				       enable, enable_also,
				       clear, preset);
      }
      break;

    case 3: // kFSM
#warning "TODO: 未完"
      break;
    }

    ymuint nt = BinIO::read_32(s);
    vector<CiTiming*> timing_list(nt);
    for (ymuint j = 0; j < nt; ++ j) {
      double v1, v2, v3, v4, v5, v6;
      bis >> v1
	  >> v2
	  >> v3
	  >> v4
	  >> v5
	  >> v6;
      CellTime i_r( v1 );
      CellTime i_f( v2 );
      CellTime s_r( v3 );
      CellTime s_f( v4 );
      CellResistance r_r( v5 );
      CellResistance f_r( v6 );
      timing_list[j] = library->new_timing(j, CellTiming::kTimingCombinational,
					   i_r, i_f, s_r, s_f, r_r, f_r);
    }

    // 入力ピンの設定
    for (ymuint j = 0; j < ni; ++ j) {
      string nstr;
      double v1, v2, v3;
      bis >> nstr
	  >> v1
	  >> v2
	  >> v3;
      ShString name( nstr );
      CellCapacitance cap( v1 );
      CellCapacitance r_cap( v2 );
      CellCapacitance f_cap( v3 );
      library->new_cell_input(cell, j, name, cap, r_cap, f_cap);
    }

    // 出力ピンの設定
    for (ymuint j = 0; j < no; ++ j) {
      string nstr;
      double v1, v2, v3, v4, v5, v6;
      bis >> nstr
	  >> v1
	  >> v2
	  >> v3
	  >> v4
	  >> v5
	  >> v6;
      ShString name( nstr );
      CellCapacitance max_f( v1 );
      CellCapacitance min_f( v2 );
      CellCapacitance max_c( v3 );
      CellCapacitance min_c( v4 );
      CellTime max_t( v5 );
      CellTime min_t( v6 );
      library->new_cell_output(cell, j, name,
			       max_f, min_f,
			       max_c, min_c,
			       max_t, min_t);
    }

    // 入出力ピンの設定
    for (ymuint j = 0; j < nio; ++ j) {
      string nstr;
      double v1, v2, v3, v4, v5, v6, v7, v8, v9;
      bis >> nstr
	  >> v1
	  >> v2
	  >> v3
	  >> v4
	  >> v5
	  >> v6
	  >> v7
	  >> v8
	  >> v9;
      ShString name( nstr );
      CellCapacitance cap( v1 );
      CellCapacitance r_cap( v2 );
      CellCapacitance f_cap( v3 );
      CellCapacitance max_f( v4 );
      CellCapacitance min_f( v5 );
      CellCapacitance max_c( v6 );
      CellCapacitance min_c( v7 );
      CellTime max_t( v8 );
      CellTime min_t( v9 );
      library->new_cell_inout(cell, j, name,
			      cap, r_cap, f_cap,
			      max_f, min_f,
			      max_c, min_c,
			      max_t, min_t);
    }

    // タイミング情報の設定
    for ( ; ; ) {
      ymuint8 unate;
      bis >> unate;
      if ( unate == 0 ) {
	// エンドマーカー
	break;
      }
      ymuint32 ipin_id;
      ymuint32 opin_id;
      ymuint32 timing_id;
      bis >> ipin_id
	  >> opin_id
	  >> timing_id;
      CiTiming* timing = timing_list[timing_id];
      tCellTimingSense sense = ( unate == 1 ) ? kCellPosiUnate : kCellNegaUnate;
      cell->set_timing(ipin_id, opin_id, sense, timing);
    }
  }

  return library;
}

END_NAMESPACE_YM_CELL
