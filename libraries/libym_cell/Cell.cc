
/// @file Cell.cc
/// @brief Cell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/Cell.h"
#include "ym_cell/CellPin.h"
#include "ym_cell/CellTiming.h"
#include "ym_logic/LogExpr.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス Cell
//////////////////////////////////////////////////////////////////////

// @brief 内容をバイナリダンプする．
// @param[in] s 出力先のストリーム
void
Cell::dump(BinO& s) const
{
  ymuint8 tid = 0;
  if ( is_logic() ) {
    tid = 0;
  }
  else if ( is_ff() ) {
    tid = 1;
  }
  else if ( is_latch() ) {
    tid = 2;
  }
  else if ( is_fsm() ) {
    tid = 3;
  }
  else {
    // 無視？
    assert_not_reached(__FILE__, __LINE__);
  }
  ymuint32 ni = input_num();
  ymuint32 no = output_num();
  ymuint32 nio = inout_num();
  ymuint32 nit = internal_num();
  ymuint32 nbus = bus_num();
  ymuint32 nbundle = bundle_num();

  s << tid
    << name()
    << area()
    << ni
    << no
    << nio
    << nit
    << nbus
    << nbundle;

  ymuint no2 = no + nio;
  for (ymuint opos = 0; opos < no2; ++ opos) {
    s << has_logic()
      << logic_expr(opos)
      << tristate_expr(opos);
  }

  if ( is_ff() ) {
    s << next_state_expr()
      << clock_expr()
      << clock2_expr()
      << clear_expr()
      << preset_expr()
      << static_cast<ymuint8>(clear_preset_var1())
      << static_cast<ymuint8>(clear_preset_var2());
  }
  else if ( is_latch() ) {
    s << data_in_expr()
      << enable_expr()
      << enable2_expr()
      << clear_expr()
      << preset_expr()
      << static_cast<ymuint8>(clear_preset_var1())
      << static_cast<ymuint8>(clear_preset_var2());
  }

  // 入力ピンのダンプ
  for (ymuint32 ipin = 0; ipin < ni; ++ ipin) {
    input(ipin)->dump(s);
  }

  // 出力ピンのダンプ
  for (ymuint32 opin = 0; opin < no; ++ opin) {
    output(opin)->dump(s);
  }

  // 入出力ピンのダンプ
  for (ymuint32 iopin = 0; iopin < nio; ++ iopin) {
    output(iopin)->dump(s);
  }

  // 内部ピンのダンプ
  for (ymuint32 itpin = 0; itpin < nit; ++ itpin) {
    internal(itpin)->dump(s);
  }

  // タイミング情報のダンプ
  ymuint32 nt = timing_num();
  s << nt;
  for (ymuint i = 0; i < nt; ++ i) {
    timing(i)->dump(s);
  }

  // ちょっと効率が悪いけど線形探索を用いている．
  // 効率が気になるならハッシュ関数を用いればよい．
  for (ymuint32 ipos = 0; ipos < ni + nio; ++ ipos) {
    for (ymuint32 opos = 0; opos < no + nio; ++ opos) {
      ymuint32 np = timing_num(ipos, opos, kCellPosiUnate);
      s << np;
      for (ymuint i = 0; i < np; ++ i) {
	const CellTiming* timing1 = timing(ipos, opos, kCellPosiUnate, i);
	for (ymuint32 j = 0; j < nt; ++ j) {
	  if ( timing(j) == timing1 ) {
	    s << j;
	  }
	}
      }
      ymuint32 nn = timing_num(ipos, opos, kCellNegaUnate);
      s << nn;
      for (ymuint i = 0; i < nn; ++ i) {
	const CellTiming* timing1 = timing(ipos, opos, kCellNegaUnate, i);
	for (ymuint32 j = 0; j < nt; ++ j) {
	  if ( timing(j) == timing1 ) {
	    s << j;
	  }
	}
      }
    }
  }
}

END_NAMESPACE_YM_CELL

