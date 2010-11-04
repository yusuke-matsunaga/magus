
/// @file libym_techmap/patgen/PgFuncMgr.cc
/// @brief PgFuncMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "PgFuncMgr.h"
#include "PgFuncRep.h"
#include "PgFunc.h"
#include "PgNode.h"
#include "PgHandle.h"
#include "ym_cell/CellLibrary.h"
#include "ym_cell/Cell.h"
#include "ym_cell/CellPin.h"
#include "ym_lexp/LogExpr.h"
#include "ym_npn/NpnMgr.h"


BEGIN_NONAMESPACE

const int debug = 0;

END_NONAMESPACE

BEGIN_NAMESPACE_YM_TECHMAP_PATGEN

//////////////////////////////////////////////////////////////////////
// クラス PgFuncMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PgFuncMgr::PgFuncMgr()
{
}

// @brief デストラクタ
PgFuncMgr::~PgFuncMgr()
{
  for (vector<PgFunc*>::iterator p = mFuncList.begin();
       p != mFuncList.end(); ++ p) {
    delete *p;
  }
  for (vector<PgFuncRep*>::iterator p = mRepList.begin();
       p != mRepList.end(); ++ p) {
    delete *p;
  }
}


BEGIN_NONAMESPACE

void
check_input(const LogExpr& expr,
	    vector<ymuint>& input_map)
{
  if ( expr.is_literal() ) {
    ymuint id = expr.varid();
    while ( input_map.size() <= id ) {
      input_map.push_back(0);
    }
    input_map[id] = 1;
  }
  else if ( expr.is_op() ) {
    ymuint n = expr.child_num();
    for (ymuint i = 0; i < n; ++ i) {
      check_input(expr.child(i), input_map);
    }
  }
}

END_NONAMESPACE


// @brief セルライブラリに対応したパタンを作る．
void
PgFuncMgr::set_library(const CellLibrary* library)
{
  mLibrary = library;

  ymuint nc = library->cell_num();
  for (ymuint i = 0; i < nc; ++ i) {
    const Cell* cell = library->cell(i);
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
    if ( ni < 2 ) continue;

    if ( !opin->has_function() ) {
      // 論理式を持たないセルも対象外
      continue;
    }

    LogExpr expr = opin->function();
    cout << "reg_expr(" << expr << ")" << endl;
    PgFunc* func = reg_expr(expr);
    cout << "end" << endl;
    func->mCellList.push_back(cell);
  }
}


// @brief 論理式を登録する．
// @param[in] expr 元になる論理式
// @return 論理関数番号を返す．
PgFunc*
PgFuncMgr::reg_expr(const LogExpr& expr)
{
  // expr に現れる入力番号を調べる．
  vector<ymuint> input_map;
  check_input(expr, input_map);
  ymuint ni = input_map.size();

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

  TvFunc f(ni, vals);

  if ( debug ) {
    cout << "expr = " << expr << endl
	 << "tvfunc = ";
    f.dump(cout, 2);
    cout << endl;
  }

  // f に対応する PgFunc および PgFuncRep を求める．
  PgFunc* pgfunc = NULL;
  PgFuncRep* pgrep = NULL;
  hash_map<TvFunc, PgFunc*>::iterator p = mFuncMap.find(f);
  if ( p == mFuncMap.end() ) {
    // なかったので新たに作る．
    pgfunc = new PgFunc;
    pgfunc->mId = mFuncList.size();
    mFuncList.push_back(pgfunc);
    mFuncMap.insert(make_pair(f, pgfunc));

    // 代表関数を求める．
    NpnMgr npnmgr;
    npnmgr.cannonical(f, pgfunc->mMap);

    TvFunc repfunc = f.xform(pgfunc->mMap);
    hash_map<TvFunc, PgFuncRep*>::iterator p = mRepMap.find(repfunc);
    if ( p == mRepMap.end() ) {
      // まだ登録されていない．
      pgrep = new PgFuncRep;
      pgrep->mId = mRepList.size();
      mRepList.push_back(pgrep);
      mRepMap.insert(make_pair(repfunc, pgrep));
      pgrep->mFunc = repfunc;
    }
    else {
      // 登録されていた．
      pgrep = p->second;
    }
    // 関数を追加する．
    pgrep->mFuncList.push_back(pgfunc);
    pgfunc->mRep = pgrep;
  }
  else {
    // 既に登録されていた．
    pgfunc = p->second;
    pgrep = pgfunc->mRep;
  }
  if ( debug ) {
    cout << "func id = " << pgfunc->id() << endl;
    cout << "rep id  = " << pgrep->id() << endl;
  }

  // expr を変換したパタンを登録する．
  VarLogExprMap vlm;
  for (ymuint i = 0; i < ni; ++ i) {
    tNpnImap imap = pgfunc->mMap.imap(i);
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
  if ( pgfunc->mMap.opol() == kPolNega ) {
    cexpr = ~cexpr;
  }
  vector<PgHandle> pat_list;
  mPatGen(cexpr, pat_list);

  // 重複チェック
  // 今は2乗オーダーのバカなアルゴリズムを使っている．
  for (vector<PgHandle>::iterator p = pat_list.begin();
       p != pat_list.end(); ++ p) {
    PgHandle pat1 = *p;
    bool found = false;
    for (vector<ymuint>::iterator q = pgrep->mPatList.begin();
	 q != pgrep->mPatList.end(); ++ q) {
      ymuint pat_id2 = *q;
      PgHandle pat2 = mPatGen.pat_root(pat_id2);
      if ( pat1.inv() == pat2.inv() &&
	   check_isomorphic(pat1.node(), pat2.node()) ) {
	found = true;
	break;
      }
    }
    if ( !found ) {
      ymuint pat_id = mPatGen.reg_pat(pat1);
      pgrep->mPatList.push_back(pat_id);
    }
  }
  mPatGen.sweep();

  return pgfunc;
}

END_NAMESPACE_YM_TECHMAP_PATGEN

