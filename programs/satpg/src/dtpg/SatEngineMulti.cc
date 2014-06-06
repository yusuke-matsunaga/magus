
/// @file SatEngineMulti.cc
/// @brief SatEngineMulti の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "SatEngineMulti.h"

#include "DetectOp.h"
#include "UntestOp.h"
#include "DtpgStats.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "BackTracer.h"
#include "LitMap.h"
#include "logic/SatSolver.h"
#include "logic/SatStats.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

// @brief 故障挿入回路を表す CNF 式を作る．
// @param[in] solver SAT ソルバー
// @param[in] ivar 入力の変数
// @param[in] fvar 故障変数
// @param[in] ovar 出力の変数
// @param[in] fval 故障値
void
make_flt_cnf(SatSolver& solver,
	     VarId ivar,
	     VarId fvar,
	     VarId ovar,
	     int vval)
{
  Literal l0(ivar, false);
  Literal l1(fvar, false);
  Literal l2(ovar, false);
  if ( vval == 0 ) {
    solver.add_clause( l0, ~l2);
    solver.add_clause(~l1, ~l2);
    solver.add_clause(~l0,  l1, l2);
  }
  else {
    solver.add_clause(~l0, l2);
    solver.add_clause(~l1, l2);
    solver.add_clause( l0, l1, ~l2);
  }
}

END_NONAMESPACE


// @brief コンストラクタ
SatEngineMulti::SatEngineMulti()
{
  mTgGrasp = true;
  mUseDominator = true;
}

// @brief デストラクタ
SatEngineMulti::~SatEngineMulti()
{
}

// @brief オプションを設定する．
void
SatEngineMulti::set_option(const string& option_str)
{
  for (string::size_type next = 0; ; ++ next) {
    string::size_type pos = option_str.find(':', next);
    if ( pos == next ) {
      continue;
    }
    string option = option_str.substr(next, pos - next);
    if ( option == "TG-GRASP" ) {
      mTgGrasp = true;
    }
    else if ( option == "NEMESIS" ) {
      mTgGrasp = false;
    }
    else if ( option == "DOM" ) {
      mUseDominator = true;
    }
    else if ( option == "NODOM" ) {
      mUseDominator = false;
    }
    if ( pos == string::npos ) {
      break;
    }
    next = pos;
  }
}

// @brief テストパタン生成を行なう．
// @param[in] flist 故障リスト
// @param[in] max_id ノード番号の最大値 + 1
void
SatEngineMulti::run(const vector<TpgFault*>& flist,
		    const vector<TpgNode*>& fnode_list,
		    ymuint max_id,
		    BackTracer& bt,
		    DetectOp& dop,
		    UntestOp& uop)
{
  cnf_begin();

  SatSolver solver(sat_type(), sat_option(), sat_outp());

  bt.set_max_id(max_id);

  mark_region(solver, fnode_list, max_id);

  ymuint nf = flist.size();

  // 故障を活性化するとき true にする変数．
  vector<VarId> flt_var(nf);
  // 故障挿入回路の出力に対応する変数．
  vector<VarId> tmp_var(nf);
  for (ymuint i = 0; i < nf; ++ i) {
    flt_var[i] = solver.new_var();
    tmp_var[i] = solver.new_var();
  }


  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < tfo_tfi_size(); ++ i) {
    TpgNode* node = tfo_tfi_node(i);
    make_gnode_cnf(solver, node);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < tfo_size(); ++ i) {
    TpgNode* node = tfo_tfi_node(i);

    // ovar に出力変数を入れる．
    // こちらは入力の故障と異なり，故障挿入回路の出力が node->fvar() となる．
    // 逆に ovar はゲートの直接の出力変数となる．
    VarId ovar = node->fvar();
    for (ymuint fid = 0; fid < nf; ++ fid) {
      if ( fnode_list[fid] == node ) {
	make_flt_cnf(solver, tmp_var[fid], flt_var[fid], ovar, flist[fid]->val());
	ovar = tmp_var[fid];
      }
    }

    Literal glit(node->gvar(), false);
    Literal flit(node->fvar(), false);
    Literal dlit(node->dvar(), false);

    Literal olit(ovar, false);
    if ( node->is_input() ) {
      solver.add_clause(~glit,  olit);
      solver.add_clause( glit, ~olit);
    }
    else {
      make_fnode_cnf(solver, node, ovar);
    }

    if ( mTgGrasp ) {
      // XOR(glit, flit, dlit) を追加する．
      // 要するに正常回路と故障回路で異なっているとき dlit が 1 となる．
      solver.add_clause(~glit, ~flit, ~dlit);
      solver.add_clause( glit,  flit, ~dlit);

      solver.add_clause(~glit,  flit,  dlit);
      solver.add_clause( glit, ~flit,  dlit);

      if ( !node->is_input() ) {
	//make_dlit_cnf(solver, node, fnode_list, flt_var);
      }
    }
    else {
      // XOR(glit, flit, dlit) を追加する．
      // 要するに正常回路と故障回路で異なっているとき dlit が 1 となる．
      solver.add_clause(~glit, ~flit, ~dlit);
      solver.add_clause( glit,  flit, ~dlit);

      if ( !node->is_output() ) {
	ymuint nfo = node->active_fanout_num();
	tmp_lits_begin(nfo + 1);
	tmp_lits_add(~dlit);
	for (ymuint j = 0; j < nfo; ++ j) {
	  TpgNode* onode = node->active_fanout(j);
	  tmp_lits_add(Literal(onode->dvar(), false));
	}
	tmp_lits_end(solver);
      }
    }
  }


  //////////////////////////////////////////////////////////////////////
  // 故障の検出条件
  //////////////////////////////////////////////////////////////////////
  ymuint npo = output_list().size();
  tmp_lits_begin(npo);
  for (ymuint i = 0; i < npo; ++ i) {
    TpgNode* node = output_list()[i];
    VarId ovar = solver.new_var();
    Literal olit(ovar, false);
    Literal glit(node->gvar(), false);
    Literal flit(node->fvar(), false);
    solver.add_clause( glit, ~flit,  olit);
    solver.add_clause(~glit,  flit,  olit);
    solver.add_clause( glit,  flit, ~olit);
    solver.add_clause(~glit, ~flit, ~olit);
    tmp_lits_add(olit);
  }
  tmp_lits_end(solver);

  cnf_end();

  // 個々の故障に対するテスト生成を行なう．
  for (ymuint fid = 0; fid < nf; ++ fid) {
    TpgFault* f = flist[fid];

    if ( f->status() == kFsDetected ) {
      // 他の故障のパタンで検出済みになっている場合がある．
      continue;
    }

    tmp_lits_begin();

    // 該当の故障に対する変数のみ1にする．
    for (ymuint j = 0; j < nf; ++ j) {
      bool inv = (j != fid);
      tmp_lits_add(Literal(flt_var[j], inv));
    }

    TpgNode* fnode = fnode_list[fid];

    // 故障ノードの TFO 以外の dlit を0にする．
    mTmpNodeList.clear();
    mTmpNodeList.reserve(tfo_tfi_size());
    set_tmp_mark(fnode);
    mTmpNodeList.push_back(fnode);
    for (ymuint rpos = 0; rpos < mTmpNodeList.size(); ++ rpos) {
      TpgNode* node = mTmpNodeList[rpos];
      ymuint nfo = node->active_fanout_num();
      for (ymuint i = 0; i < nfo; ++ i) {
	TpgNode* fonode = node->active_fanout(i);
	if ( !tmp_mark(fonode) ) {
	  set_tmp_mark(fonode);
	  mTmpNodeList.push_back(fonode);
	}
      }
    }
    for (ymuint i = 0; i < tfo_tfi_size(); ++ i) {
      TpgNode* node = tfo_tfi_node(i);
      if ( node->has_fvar() && !tmp_mark(node) ) {
	Literal dlit(node->dvar(), true);
	tmp_lits_add(dlit);
      }
    }
    for (vector<TpgNode*>::iterator p = mTmpNodeList.begin();
	 p != mTmpNodeList.end(); ++ p) {
      TpgNode* node = *p;
      clear_tmp_mark(node);
    }
    mTmpNodeList.clear();

    if ( mUseDominator ) {
      // dominator ノードの dvar は1でなければならない．
      for (TpgNode* node = f->node(); node != NULL; node = node->imm_dom()) {
	Literal dlit(node->dvar(), false);
	tmp_lits_add(dlit);
      }
    }
    else {
      tmp_lits_add(Literal(f->node()->dvar(), false));
    }

    bool inv = (f->val() == 0);
    tmp_lits_add(Literal(fnode->fvar(), inv));

    solve(solver, f, bt, dop, uop);
  }
  clear_node_mark();
}

END_NAMESPACE_YM_SATPG
