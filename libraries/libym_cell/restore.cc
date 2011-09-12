
/// @file restore.cc
/// @brief バイナリファイルを読み込む関数
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellLibrary.h"
#include "ym_cell/CellArea.h"
#include "ym_cell/CellCapacitance.h"
#include "ym_cell/CellResistance.h"
#include "ym_cell/CellTime.h"
#include "ym_logic/LogExpr.h"
#include "ym_utils/BinIO.h"


BEGIN_NAMESPACE_YM_CELL

void
CellLibrary::restore(istream& s)
{
  BinI bis(s);

  string name;
  bis >> name;

  set_name(name);

  ymuint32 nc;
  bis >> nc;
  set_cell_num(nc);

  for (ymuint cell_id = 0; cell_id < nc; ++ cell_id) {
    ymuint8 type;
    string name;
    double aval;
    ymuint32 ni;
    ymuint32 no;
    ymuint32 nio;
    ymuint32 nbus;
    ymuint32 nbundle;
    bis >> type
	>> name
	>> aval
	>> ni
	>> no
	>> nio
	>> nbus
	>> nbundle;
    CellArea area( aval );
    ymuint no2 = no + nio;
    vector<LogExpr> logic_array(no2);
    vector<LogExpr> tristate_array(no2);
    for (ymuint opos = 0; opos < no2; ++ opos) {
      bis >> logic_array[opos]
	  >> tristate_array[opos];
    }

    switch ( type ) {
    case 0: // kLogic
      new_logic_cell(cell_id, name, area, ni, no, nio, nbus, nbundle,
		     logic_array, tristate_array);
      break;

    case 1: // kFF
      {
	LogExpr next_state;
	LogExpr clocked_on;
	LogExpr clocked_on_also;
	LogExpr clear;
	LogExpr preset;
	ymuint8 clear_preset_var1;
	ymuint8 clear_preset_var2;
	bis >> next_state
	    >> clocked_on
	    >> clocked_on_also
	    >> clear
	    >> preset
	    >> clear_preset_var1
	    >> clear_preset_var2;
	new_ff_cell(cell_id, name, area, ni, no, nio, nbus, nbundle,
		    logic_array, tristate_array,
		    next_state,
		    clocked_on, clocked_on_also,
		    clear, preset,
		    clear_preset_var1,
		    clear_preset_var2);
      }
      break;

    case 2: // kLatch
      {
	LogExpr data_in;
	LogExpr enable;
	LogExpr enable_also;
	LogExpr clear;
	LogExpr preset;
	ymuint8 clear_preset_var1;
	ymuint8 clear_preset_var2;
	bis >> data_in
	    >> enable
	    >> enable_also
	    >> clear
	    >> preset
	    >> clear_preset_var1
	    >> clear_preset_var2;
	new_latch_cell(cell_id, name, area, ni, no, nio, nbus, nbundle,
		       logic_array, tristate_array,
		       data_in,
		       enable, enable_also,
		       clear, preset,
		       clear_preset_var1,
		       clear_preset_var2);
      }
      break;

    case 3: // kFSM
#warning "TODO: 未完"
      break;
    }

    // 入力ピンの設定
    for (ymuint j = 0; j < ni; ++ j) {
      string name;
      double v1, v2, v3;
      bis >> name
	  >> v1
	  >> v2
	  >> v3;
      CellCapacitance cap( v1 );
      CellCapacitance r_cap( v2 );
      CellCapacitance f_cap( v3 );
      new_cell_input(cell_id, j, name, cap, r_cap, f_cap);
    }

    // 出力ピンの設定
    for (ymuint j = 0; j < no; ++ j) {
      string name;
      double v1, v2, v3, v4, v5, v6;
      bis >> name
	  >> v1
	  >> v2
	  >> v3
	  >> v4
	  >> v5
	  >> v6;
      CellCapacitance max_f( v1 );
      CellCapacitance min_f( v2 );
      CellCapacitance max_c( v3 );
      CellCapacitance min_c( v4 );
      CellTime max_t( v5 );
      CellTime min_t( v6 );
      new_cell_output(cell_id, j, name,
		      max_f, min_f,
		      max_c, min_c,
		      max_t, min_t);
    }

    // 入出力ピンの設定
    for (ymuint j = 0; j < nio; ++ j) {
      string name;
      double v1, v2, v3, v4, v5, v6, v7, v8, v9;
      bis >> name
	  >> v1
	  >> v2
	  >> v3
	  >> v4
	  >> v5
	  >> v6
	  >> v7
	  >> v8
	  >> v9;
      CellCapacitance cap( v1 );
      CellCapacitance r_cap( v2 );
      CellCapacitance f_cap( v3 );
      CellCapacitance max_f( v4 );
      CellCapacitance min_f( v5 );
      CellCapacitance max_c( v6 );
      CellCapacitance min_c( v7 );
      CellTime max_t( v8 );
      CellTime min_t( v9 );
      new_cell_inout(cell_id, j, name,
		     cap, r_cap, f_cap,
		     max_f, min_f,
		     max_c, min_c,
		     max_t, min_t);
    }

    // タイミング情報の生成
    ymuint nt = BinIO::read_32(s);
    vector<CellTiming*> tmp_list(nt);
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
      CellTiming* timing = new_timing(j, kCellTimingCombinational,
				      i_r, i_f, s_r, s_f, r_r, f_r);
      tmp_list[j] = timing;
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
      tCellTimingSense sense = ( unate == 1 ) ? kCellPosiUnate : kCellNegaUnate;
      CellTiming* timing = tmp_list[timing_id];
      set_timing(cell_id, ipin_id, opin_id, sense, timing);
    }
  }
}

END_NAMESPACE_YM_CELL
