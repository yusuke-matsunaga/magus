
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
  string name = BinIO::read_str(s);

  CiLibrary* library = new CiLibrary(name);

  ymuint nc = BinIO::read_32(s);
  library->set_cell_num(nc);

  for (ymuint i = 0; i < nc; ++ i) {
    ymuint type = BinIO::read_8(s);
    ShString name( BinIO::read_str(s) );
    CellArea area( BinIO::read_double(s) );
    ymuint ni = BinIO::read_32(s);
    ymuint no = BinIO::read_32(s);
    ymuint nio = BinIO::read_32(s);
    ymuint nb = BinIO::read_32(s);
    ymuint nc = BinIO::read_32(s);
    CiCell* cell = library->new_cell(i, name, area, ni, no, nio, nb, nc);
    switch ( type ) {
    case 0: // kLogic
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
      }
      break;

    case 3: // kFSM
#warning "TODO: 未完"
      break;
    }

    ymuint nt = BinIO::read_32(s);
    vector<CiTiming*> timing_list(nt);
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

    // 入力ピンの設定
    for (ymuint j = 0; j < ni; ++ j) {
      ShString name( BinIO::read_str(s) );
      CellCapacitance cap( BinIO::read_double(s) );
      CellCapacitance r_cap( BinIO::read_double(s) );
      CellCapacitance f_cap( BinIO::read_double(s) );
      library->new_cell_input(cell, j, name, cap, r_cap, f_cap);
    }

    // 出力ピンの設定
    for (ymuint j = 0; j < no; ++ j) {
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
    }

    // 入出力ピンの設定
    for (ymuint j = 0; j < nio; ++ j) {
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
    }

    // タイミング情報の設定
    for ( ; ; ) {
      ymuint unate = BinIO::read_32(s);
      if ( unate == 0 ) {
	// エンドマーカー
	break;
      }
      ymuint ipin_id = BinIO::read_32(s);
      ymuint opin_id = BinIO::read_32(s);
      ymuint timing_id = BinIO::read_32(s);
      CiTiming* timing = timing_list[timing_id];
      tCellTimingSense sense = ( unate == 1 ) ? kCellPosiUnate : kCellNegaUnate;
      cell->set_timing(ipin_id, opin_id, sense, timing);
    }
  }

  return library;
}

END_NAMESPACE_YM_CELL
