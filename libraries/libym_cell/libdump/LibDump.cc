
/// @file LibDump.cc
/// @brief LibDump の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LibDump.h"
#include "LdFuncClass.h"
#include "LdFunc.h"
#include "LdPatNode.h"
#include "LdPatHandle.h"
#include "LdFFGroup.h"
#include "ym_cell/CellLibrary.h"
#include "ym_cell/Cell.h"
#include "ym_cell/CellPin.h"
#include "ym_logic/LogExpr.h"
#include "ym_logic/NpnMap.h"


BEGIN_NAMESPACE_YM_CELL_LIBDUMP

BEGIN_NONAMESPACE

// expr に現れる最大の変数番号 + 1を返す．
ymuint
check_input(const LogExpr& expr)
{
  if ( expr.is_literal() ) {
    ymuint id = expr.varid();
    return id + 1;
  }
  else if ( expr.is_op() ) {
    ymuint n = expr.child_num();
    ymuint id = check_input(expr.child(0));
    for (ymuint i = 1; i < n; ++ i) {
      ymuint id1 = check_input(expr.child(i));
      if ( id < id1 ) {
	id = id1;
      }
    }
    return id;
  }
  else if ( expr.is_constant() ) {
    return 0;
  }
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// 論理式を TvFunc に変換する．
TvFunc
expr2tvfunc(const LogExpr& expr)
{
  // expr に現れる最大の入力番号を調べる．
  ymuint ni = check_input(expr);

  // expr に対応する関数を作る．
  vector<ymulong> ivals(ni);
  ymuint nip = (1U << ni);
  vector<int> vals(nip);
  for (ymuint p = 0U; p < nip; ++ p) {
    for (ymuint i = 0; i < ni; ++ i) {
      if ( p & (1U << i) ) {
	ivals[i] = 1;
      }
      else {
	ivals[i] = 0;
      }
    }
    if ( expr.eval(ivals, 1UL) ) {
      vals[p] = 1;
    }
    else {
      vals[p] = 0;
    }
  }

  return TvFunc(ni, vals);
}

// 論理式の変数を map にしたがって変換する．
LogExpr
xform_expr(const LogExpr& expr,
	   const NpnMap& map)
{
  ymuint ni = map.ni();
  VarLogExprMap vlm;
  for (ymuint i = 0; i < ni; ++ i) {
    tNpnImap imap = map.imap(i);
    ymuint j = npnimap_pos(imap);
    LogExpr expr;
    if ( npnimap_pol(imap) == kPolPosi) {
      expr = LogExpr::make_posiliteral(j);
    }
    else {
      expr = LogExpr::make_negaliteral(j);
    }
    vlm.insert(make_pair(i, expr));
  }
  LogExpr cexpr = expr.compose(vlm);
  if ( map.opol() == kPolNega ) {
    cexpr = ~cexpr;
  }
  return cexpr;
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス LibDump
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LibDump::LibDump()
{
}

// @brief デストラクタ
LibDump::~LibDump()
{
}

// @brief ライブラリの情報からパタンを生成する．
// @param[in] library 対象のセルライブラリ
void
LibDump::gen_pat(const CellLibrary& library)
{
  mLdFuncMgr.init();
  mLdPatMgr.init();

  // XOR のパタンを登録しておく．
  // これはちょっとしたハック
  {
    LogExpr lit0 = LogExpr::make_posiliteral(0);
    LogExpr lit1 = LogExpr::make_posiliteral(1);
    LogExpr xor_ex = lit0 ^ lit1;
    reg_expr(xor_ex);
  }

  ymuint nc = library.cell_num();
  for (ymuint i = 0; i < nc; ++ i) {
    const Cell* cell = library.cell(i);
    ymuint ni = cell->input_num();
    ymuint no = cell->output_num();
#if 0
    if ( cell->is_logic() ) {
      if ( no > 1 ) {
	// 出力ピンが複数あるセルは対象外
	continue;
      }
      const CellPin* opin = cell->output(0);
      if ( !opin->has_function() ) {
	// 論理式を持たないセルも対象外
	continue;
      }
      if ( opin->has_three_state() ) {
	// three_state 属性を持つセルも対象外
	continue;
      }

      LogExpr expr = opin->function();
      TvFunc tv = expr2tvfunc(expr);
      LdFunc* pgfunc = mLdFuncMgr.find_func(tv);
      pgfunc->add_cell(cell->id());

      reg_pat(pgfunc, expr);
    }
    if ( cell->is_ff() ) {
      LogExpr d_expr = cell->next_state();
      if ( !d_expr.is_posiliteral() ) {
	// next_state が肯定リテラルでなかった．
	continue;
      }
      ymuint data_pos = d_expr.varid();

      LogExpr c_expr = cell->clocked_on();
      if ( !c_expr.is_literal() ) {
	// clocked_on がリテラルでなかった．
	continue;
      }
      ymuint clock_pos = c_expr.varid();
      ymuint clock_sense = c_expr.is_posiliteral() ? 1 : 2;

      LogExpr r0_expr = cell->clear();
      ymuint clear_pos = 0;
      ymuint clear_sense = 0;
      if ( !r0_expr.is_zero() ) {
	if ( r0_expr.is_literal() ) {
	  clear_pos = r0_expr.varid();
	  clear_sense = r0_expr.is_posiliteral() ? 1 : 2;
	}
	else {
	  continue;
	}
      }

      LogExpr r1_expr = cell->preset();
      ymuint preset_pos = 0;
      ymuint preset_sense = 0;
      if ( !r1_expr.is_zero() ) {
	if ( r1_expr.is_literal() ) {
	  preset_pos = r1_expr.varid();
	  preset_sense = r1_expr.is_posiliteral() ? 1 : 2;
	}
	else {
	  continue;
	}
      }

      const CellPin* q_pin = NULL;
      const CellPin* iq_pin = NULL;
      for (ymuint j = 0; j < np; ++ j) {
	const CellPin* pin = cell->pin(j);
	if ( pin->is_output() ) {
	  if ( !pin->has_function() ) {
	    continue;
	  }
	  LogExpr expr = pin->function();
	  if ( !expr.is_posiliteral() ) {
	    continue;
	  }
	  ymuint pos = expr.varid();
	  if ( pos == np ) {
	    if ( q_pin != NULL ) {
	      // Q ピンが2つ以上ある．
	      q_pin = NULL;
	      break;
	    }
	    q_pin = pin;
	  }
	  else if ( pos == (np + 1) ) {
	    if ( iq_pin != NULL ) {
	      // IQ ピンが2つ以上ある．
	      iq_pin = NULL;
	      break;
	    }
	    iq_pin = pin;
	  }
	}
      }
      if ( q_pin == NULL || iq_pin == NULL ) {
	continue;
      }
      ymuint q_pos = q_pin->id();
      ymuint iq_pos = iq_pin->id();

      LdFFGroup* ff_group = mLdFFMgr.find_group(clock_sense,
						clear_sense,
						preset_sense,
						data_pos,
						clock_pos,
						clear_pos,
						preset_pos,
						q_pos,
						iq_pos);
      ff_group->add_cell(cell->id());
    }
#endif
  }
}

// @brief expr から生成されるパタンを登録する．
void
LibDump::reg_expr(const LogExpr& expr)
{
  // expr に対応する LdFunc を求める．
  TvFunc f = expr2tvfunc(expr);
  LdFunc* pgfunc = mLdFuncMgr.find_func(f);

  // expr から生成されるパタンを pgfunc に登録する．
  reg_pat(pgfunc, expr);
}

// @brief 論理式から生成されるパタンを登録する．
// @param[in] pgfunc この式に対応する関数情報
// @param[in] expr パタンの元となる論理式
void
LibDump::reg_pat(LdFunc* pgfunc,
		 const LogExpr& expr)
{
  const LdFuncClass* pgrep = pgfunc->rep();

  // pgrep->rep_func() を用いる理由は論理式に現れる変数が
  // 真のサポートとは限らないから
  if ( pgrep->rep_func().ni() > 1 ) {
    // expr を変換したパタンを登録する．
    LogExpr cexpr = xform_expr(expr, pgfunc->map());

    assert_cond( !cexpr.is_constant(), __FILE__, __LINE__);

    mLdPatMgr.reg_pat(cexpr, pgrep->id());
  }
}

// @brief グラフ構造全体をダンプする．
// @param[in] s 出力先のストリーム
// @param[in] library 対象のセルライブラリ
void
LibDump::display(ostream& s,
		 const CellLibrary& library)
{
  gen_pat(library);

  // ライブラリの情報を出力する．
  display_library(s, library);

  // 関数の情報を出力する．
  mLdFuncMgr.display(s);

  // パタングラフの情報を出力する．
  mLdPatMgr.display(s);

  // FF の情報を出力する．
  mLdFFMgr.display(s);
}

// @brief グラフ構造全体をダンプする．
// @param[in] s 出力先のストリーム
// @param[in] library 対象のセルライブラリ
// @note ダンプされた情報はそのまま PatGraph で読み込むことができる．
void
LibDump::dump(ostream& s,
	      const CellLibrary& library)
{
  gen_pat(library);

  // ライブラリの情報をダンプする．
  nsYm::nsCell::dump_library(s, library);

  // 関数の情報をダンプする．
  mLdFuncMgr.dump(s);

  // パタングラフの情報をダンプする．
  mLdPatMgr.dump(s);

  // FF の情報を出力する．
  mLdFFMgr.dump(s);
}

END_NAMESPACE_YM_CELL_LIBDUMP
