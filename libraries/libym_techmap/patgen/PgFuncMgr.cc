
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

// 同形チェックの再帰関数
bool
isom_recur(PgNode* node1,
	   PgNode* node2)
{
  if ( node1 == node2 ) {
    return true;
  }
  if ( node1->is_input() ) {
    // node1 が入力で node2 が入力でなかったら異なる．
    return node2->is_input();
  }
  if ( node2->is_input() ) {
    // ここに来ているということは node1 が入力でない．
    return false;
  }
  if ( node1->is_and() ) {
    if ( !node2->is_and() ) {
      // node1 が AND で node2 が XOR
      return false;
    }
  }
  else if ( node2->is_and() ) {
    // ここに来ているということは node1 は XOR
    return false;
  }
  if ( node1->fanin_inv(0) != node2->fanin_inv(0) ||
       node1->fanin_inv(1) != node2->fanin_inv(1) ) {
    return false;
  }
  if ( !isom_recur(node1->fanin(0), node2->fanin(0)) ) {
    return false;
  }
  if ( !isom_recur(node1->fanin(1), node2->fanin(1)) ) {
    return false;
  }
  return true;
}

// @brief 2つのパタンが同形かどうか調べる．
// @param[in] pat1, pat2 パタンの根のハンドル
// @retval true pat1 と pat2 は同形だった．
// @retval false pat1 と pat2 は同形ではなかった．
// @note ここでいう「同形」とは終端番号以外がおなじこと
bool
check_isomorphic(PgHandle pat1,
		 PgHandle pat2)
{
  if ( pat1.inv() != pat2.inv() ) {
    return false;
  }
  PgNode* node1 = pat1.node();
  PgNode* node2 = pat2.node();

  // あとは実際に再帰して調べる．
  return isom_recur(node1, node2);
}

END_NONAMESPACE


// @brief 論理式を登録する．
// @param[in] expr 元になる論理式
// @return 論理関数番号を返す．
ymuint
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
      if ( check_isomorphic(pat1, pat2) ) {
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

  return pgfunc->id();
}

END_NAMESPACE_YM_TECHMAP_PATGEN

