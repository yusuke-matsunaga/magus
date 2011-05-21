
/// @file libym_techmap/cellmap/libdump/LibDump.cc
/// @brief LibDump の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "LibDump.h"
#include "LdFuncRep.h"
#include "LdFunc.h"
#include "LdPatNode.h"
#include "LdPatHandle.h"
#include "ym_cell/CellLibrary.h"
#include "ym_cell/Cell.h"
#include "ym_cell/CellPin.h"
#include "ym_lexp/LogExpr.h"
#include "ym_npn/NpnMap.h"
#include "ym_utils/BinIO.h"


BEGIN_NAMESPACE_YM_CELLMAP_LIBDUMP

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
  return expr;
}

void
dump_func(ostream& s,
	  const TvFunc& f)
{
  ymuint ni = f.ni();
  BinIO::write_32(s, ni);
  ymuint nip = (1U << ni);
  ymuint32 v = 0U;
  ymuint base = 0;
  for (ymuint p = 0; p < nip; ++ p) {
    v |= (f.value(p) << (p - base));
    if ( (p % 32) == 31 ) {
      BinIO::write_32(s, v);
      base += 32;
      v = 0U;
    }
  }
  if ( ni <= 4 ) {
    BinIO::write_32(s, v);
  }
}

void
dump_map(ostream& s,
	 const NpnMap& map)
{
  ymuint ni = map.ni();
  ymuint32 v = (ni << 1);
  if ( map.opol() == kPolNega ) {
    v |= 1U;
  }
  BinIO::write_32(s, v);
  for (ymuint i = 0; i < ni; ++ i) {
    tNpnImap imap = map.imap(i);
    // 手抜きでは imap を ymuint32 にキャストすればよい．
    ymuint j = npnimap_pos(imap);
    ymuint32 v = (j << 1);
    if ( npnimap_pol(imap) ) {
      v |= 1U;
    }
    BinIO::write_32(s, v);
  }
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
  mPgfMgr.init();
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
    ymuint np = cell->pin_num();
    const CellPin* opin = NULL;
    ymuint ni = 0;
    for (ymuint j = 0; j < np; ++ j) {
      const CellPin* pin = cell->pin(j);
      if ( pin->direction() == nsCell::kDirOutput ) {
	if ( opin != NULL ) {
	  // 出力ピンが複数あるセルは対象外
	  opin = NULL;
	  break;
	}
	opin = pin;
      }
      else if ( pin->direction() == nsCell::kDirInput ) {
	++ ni;
      }
    }
    if ( opin == NULL ) continue;

    if ( !opin->has_function() ) {
      // 論理式を持たないセルも対象外
      continue;
    }

    LogExpr expr = opin->function();
    TvFunc tv = expr2tvfunc(expr);
    LdFunc* pgfunc = mPgfMgr.reg_func(tv, cell->id());

    reg_pat(pgfunc, expr);
  }
}

// @brief expr から生成されるパタンを登録する．
void
LibDump::reg_expr(const LogExpr& expr)
{
  // expr に対応する LdFunc を求める．
  TvFunc f = expr2tvfunc(expr);
  LdFunc* pgfunc = mPgfMgr.find_func(f);

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
  LdFuncRep* pgrep = pgfunc->mRep;

  // pgrep->rep_func() を用いる理由は論理式に現れる変数が
  // 真のサポートとは限らないから
  if ( pgrep->rep_func().ni() > 1 ) {
    // expr を変換したパタンを登録する．
    LogExpr cexpr = xform_expr(expr, pgfunc->mMap);

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

  s << "*** FUNCTION SECTION ***" << endl;
  for (ymuint i = 0; i < mPgfMgr.func_num(); ++ i) {
    const LdFunc* func = mPgfMgr.func(i);
    assert_cond( func->id() == i, __FILE__, __LINE__);
    s << "FUNC#" << i << ": REP#" << func->rep()->id()
      << ": " << func->map() << endl;
    s << "  CELL#ID" << endl;
    const vector<ymuint>& cell_list = func->cell_list();
    for (vector<ymuint>::const_iterator p = cell_list.begin();
	 p != cell_list.end(); ++ p) {
      s << "    " << *p << endl;
    }
  }
  s << endl;

  s << "*** REPRESENTATIVE SECTION ***" << endl;
  for (ymuint i = 0; i < mPgfMgr.rep_num(); ++ i) {
    const LdFuncRep* rep = mPgfMgr.rep(i);
    assert_cond( rep->id() == i , __FILE__, __LINE__);
    s << "REP#" << i << ": ";
    rep->rep_func().dump(s, 2);
    s << endl;
    s << "  equivalence = ";
    for (ymuint j = 0; j < rep->func_num(); ++ j) {
      s << " FUNC#" << rep->func(j)->id();
    }
    s << endl;
  }
  s << endl;

  // パタングラフの情報を出力する．
  mLdPatMgr.display(s);
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
  ymuint nf = mPgfMgr.func_num();
  BinIO::write_32(s, nf);
  for (ymuint i = 0; i < nf; ++ i) {
    const LdFunc* func = mPgfMgr.func(i);
    assert_cond( func->id() == i, __FILE__, __LINE__);
    // 代表関数に対する変換マップをダンプする．
    dump_map(s, func->map());
    // 属しているセル番号をダンプする．
    const vector<ymuint>& cell_list = func->cell_list();
    ymuint nc = cell_list.size();
    BinIO::write_32(s, nc);
    for (ymuint i = 0; i < nc; ++ i) {
      BinIO::write_32(s, cell_list[i]);
    }
  }

  // 代表関数の情報をダンプする．
  ymuint nr = mPgfMgr.rep_num();
  BinIO::write_32(s, nr);
  for (ymuint i = 0; i < nr; ++ i) {
    const LdFuncRep* rep = mPgfMgr.rep(i);
    assert_cond( rep->id() == i , __FILE__, __LINE__);
    ymuint ne = rep->func_num();
    BinIO::write_32(s, ne);
    for (ymuint j = 0; j < ne; ++ j) {
      BinIO::write_32(s, rep->func(j)->id());
    }
  }

  // パタングラフの情報をダンプする．
  mLdPatMgr.dump(s);
}

END_NAMESPACE_YM_CELLMAP_LIBDUMP
