
/// @file libym_techmap/patgen/PgDumper.cc
/// @brief PgDumper の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "PgDumper.h"
#include "PgFuncRep.h"
#include "PgFunc.h"
#include "PgNode.h"
#include "PgHandle.h"
#include "../PatMgr.h"
#include "ym_cell/CellLibrary.h"
#include "ym_cell/Cell.h"
#include "ym_cell/CellPin.h"
#include "ym_lexp/LogExpr.h"
#include "ym_npn/NpnMap.h"
#include "ym_utils/BinIO.h"


BEGIN_NAMESPACE_YM_CELLMAP_PATGEN

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
display_edge(ostream& s,
	     PgNode* node,
	     ymuint fanin_pos)
{
  if ( node->fanin_inv(fanin_pos) ) {
    s << "~";
  }
  s << "Node#" << node->fanin(fanin_pos)->id();
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

void
dump_edge(ostream& s,
	  PgNode* node,
	  ymuint fanin_pos)
{
  ymuint v = 0U;
  if ( !node->is_input() ) {
    v = node->fanin(fanin_pos)->id() * 2;
    if ( node->fanin_inv(fanin_pos) ) {
      v |= 1U;
    }
  }
  BinIO::write_32(s, v);
}

void
dump_dfs(PgNode* node,
	 vector<bool>& vmark,
	 vector<ymuint>& val_list,
	 ymuint& max_input)
{
  if ( node->is_input() ) {
    if ( max_input < node->input_id() ) {
      max_input = node->input_id();
    }
    return;
  }
  if ( vmark[node->id()] ) {
    return;
  }
  vmark[node->id()] = true;
  val_list.push_back(node->id() * 2);
  dump_dfs(node->fanin(0), vmark, val_list, max_input);
  val_list.push_back(node->id() * 2 + 1);
  dump_dfs(node->fanin(1), vmark, val_list, max_input);
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス PgDumper
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PgDumper::PgDumper()
{
}

// @brief デストラクタ
PgDumper::~PgDumper()
{
}

// @brief ライブラリの情報からパタンを生成する．
// @param[in] library 対象のセルライブラリ
void
PgDumper::gen_pat(const CellLibrary& library)
{
  mPgfMgr.init();
  mPatGen.init();

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
    PgFunc* pgfunc = mPgfMgr.reg_func(tv, cell->id());

    reg_pat(pgfunc, expr);
  }
}

// @brief expr から生成されるパタンを登録する．
void
PgDumper::reg_expr(const LogExpr& expr)
{
  // expr に対応する PgFunc を求める．
  TvFunc f = expr2tvfunc(expr);
  PgFunc* pgfunc = mPgfMgr.find_func(f);

  // expr から生成されるパタンを pgfunc に登録する．
  reg_pat(pgfunc, expr);
}

// @brief 論理式から生成されるパタンを登録する．
// @param[in] pgfunc この式に対応する関数情報
// @param[in] expr パタンの元となる論理式
void
PgDumper::reg_pat(PgFunc* pgfunc,
		  const LogExpr& expr)
{
  PgFuncRep* pgrep = pgfunc->mRep;

  // pgrep->rep_func() を用いる理由は論理式に現れる変数が
  // 真のサポートとは限らないから
  if ( pgrep->rep_func().ni() > 1 ) {
    // expr を変換したパタンを登録する．
    LogExpr cexpr = xform_expr(expr, pgfunc->mMap);

    assert_cond( !cexpr.is_constant(), __FILE__, __LINE__);

    mPatGen.reg_pat(cexpr, pgrep->mPatList);
  }
}

// @brief グラフ構造全体をダンプする．
// @param[in] s 出力先のストリーム
void
PgDumper::display(ostream& s)
{
  s << "*** FUNCTION SECTION ***" << endl;
  for (ymuint i = 0; i < mPgfMgr.func_num(); ++ i) {
    const PgFunc* func = mPgfMgr.func(i);
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
    const PgFuncRep* rep = mPgfMgr.rep(i);
    assert_cond( rep->id() == i , __FILE__, __LINE__);
    s << "REP#" << i << ": ";
    rep->rep_func().dump(s, 2);
    s << endl;
    s << "  equivalence = ";
    for (ymuint j = 0; j < rep->func_num(); ++ j) {
      s << " FUNC#" << rep->func(j)->id();
    }
    s << endl;
    s << "  patterns = ";
    for (ymuint j = 0; j < rep->pat_num(); ++ j) {
      s << " PAT#" << rep->pat_id(j);
    }
    s << endl;
  }
  s << endl;

  s << "*** NODE SECTION ***" << endl;
  ymuint n = mPatGen.node_num();
  for (ymuint i = 0; i < n; ++ i) {
    PgNode* node = mPatGen.node(i);
    if ( node->is_locked() ) {
      s << "*";
    }
    else {
      s << " ";
    }
    s << "Node#" << node->id() << ": ";
    if ( node->is_input() ) {
      s << "Input#" << node->input_id();
    }
    else {
      if ( node->is_and() ) {
	s << "And";
      }
      else if ( node->is_xor() ) {
	s << "Xor";
      }
      else {
	assert_not_reached(__FILE__, __LINE__);
      }
      s << "( ";
      display_edge(s, node, 0);
      s << ", ";
      display_edge(s, node, 1);
      s << ")";
    }
    s << endl;
  }
  s << endl;

  s << "*** PATTERN SECTION ***" << endl;
  ymuint np = mPatGen.pat_num();
  for (ymuint i = 0; i < np; ++ i) {
    PgHandle root = mPatGen.pat_root(i);
    s << "Pat#" << i << ": ";
    if ( root.inv() ) {
      s << "~";
    }
    s << "Node#" << root.node()->id() << endl;
  }
  s << "*** END ***" << endl;
}

// @brief グラフ構造全体をダンプする．
// @param[in] s 出力先のストリーム
// @note ダンプされた情報はそのまま PatGraph で読み込むことができる．
void
PgDumper::dump(ostream& s)
{
  // 関数の情報をダンプする．
  ymuint nf = mPgfMgr.func_num();
  BinIO::write_32(s, nf);
  for (ymuint i = 0; i < nf; ++ i) {
    const PgFunc* func = mPgfMgr.func(i);
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

  ymuint np = mPatGen.pat_num();

  // 代表関数の情報をダンプする．
  ymuint nr = mPgfMgr.rep_num();
  BinIO::write_32(s, nr);
  // パタン番号をキーにして代表関数番号を記憶する配列
  vector<ymint> rep_array(np);
  for (ymuint i = 0; i < nr; ++ i) {
    const PgFuncRep* rep = mPgfMgr.rep(i);
    assert_cond( rep->id() == i , __FILE__, __LINE__);
    ymuint ne = rep->func_num();
    BinIO::write_32(s, ne);
    for (ymuint j = 0; j < ne; ++ j) {
      BinIO::write_32(s, rep->func(j)->id());
    }
    ymuint m = rep->pat_num();
    for (ymuint j = 0; j < m; ++ j) {
      ymuint pat_id = rep->pat_id(j);
      rep_array[pat_id] = i;
    }
  }


  // パタンノードの情報をダンプする．
  ymuint nn = mPatGen.node_num();
  BinIO::write_32(s, nn);
  for (ymuint i = 0; i < nn; ++ i) {
    PgNode* node = mPatGen.node(i);
    ymuint v = 0U;
    if ( node->is_input() ) {
      v = PatMgr::kInput | (node->input_id() << 2);
    }
    else if ( node->is_and() ) {
      v = PatMgr::kAnd;
    }
    else if ( node->is_xor() ) {
      v = PatMgr::kXor;
    }
    BinIO::write_32(s, v);
    dump_edge(s, node, 0);
    dump_edge(s, node, 1);
  }

  vector<bool> vmark(nn);
  vector<ymuint> val_list;
  val_list.reserve(nn * 2);

  // パタンの情報をダンプする．
  BinIO::write_32(s, np);
  for (ymuint i = 0; i < np; ++ i) {
    PgHandle root = mPatGen.pat_root(i);
    vmark.clear();
    vmark.resize(nn, false);
    val_list.clear();
    ymuint max_input = 0;
    dump_dfs(root.node(), vmark, val_list, max_input);
    ymuint v = (max_input + 1) << 1;
    if ( root.inv() ) {
      v |= 1U;
    }
    BinIO::write_32(s, v);
    ymuint ne = val_list.size();
    BinIO::write_32(s, ne);
    for (ymuint j = 0; j < ne; ++ j) {
      BinIO::write_32(s, val_list[j]);
    }
    BinIO::write_32(s, rep_array[i]);
  }
}

END_NAMESPACE_YM_CELLMAP_PATGEN
