
/// @file SatEngineMulti2.cc
/// @brief SatEngineMulti2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "SatEngineMulti2.h"

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
SatEngineMulti2::SatEngineMulti2()
{
  mTgGrasp = true;
  mUseDominator = true;
  mUseLocalDominator = true;
  mSkipThreshold = 2;
}

// @brief デストラクタ
SatEngineMulti2::~SatEngineMulti2()
{
}

// @brief オプションを設定する．
void
SatEngineMulti2::set_option(const string& option_str)
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
      mUseLocalDominator = false;
    }
    else if ( option == "NODOM" ) {
      mUseDominator = false;
    }
    else if ( option == "LOCALDOM" ) {
      mUseLocalDominator = true;
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
SatEngineMulti2::run(TpgNetwork& network,
		     const vector<TpgFault*>& flist,
		     const vector<TpgNode*>& fnode_list,
		     ymuint max_id,
		     BackTracer& bt,
		     DetectOp& dop,
		     UntestOp& uop)
{
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
    TpgFault* f = flist[i];
    f->clear_untest_num();
  }

  const vector<TpgNode*>& olist = output_list();
  ymuint no = olist.size();

  vector<ymuint> mark(max_id, 0);
  for (ymuint opos = 0; opos < no; ++ opos) {
    TpgNode* onode = olist[opos];
    ymuint oid = onode->output_id2();

    cnf_begin();

    //////////////////////////////////////////////////////////////////////
    // 正常回路の CNF を生成
    //////////////////////////////////////////////////////////////////////
    for (ymuint i = 0; i < tfo_tfi_size(); ++ i) {
      TpgNode* node = tfo_tfi_node(i);
      if ( node->is_in_TFI_of(oid) && mark[node->id()] == 0 ) {
	mark[node->id()] = oid + 1;

	make_node_cnf(solver, node, GvarLitMap(node));
      }
    }

    //////////////////////////////////////////////////////////////////////
    // 故障回路の CNF を生成
    //////////////////////////////////////////////////////////////////////
    for (ymuint i = 0; i < tfo_size(); ++ i) {
      TpgNode* node = tfo_tfi_node(i);
      if ( mark[node->id()] != (oid + 1) ) {
	continue;
      }

      // ovar に出力変数を入れる．
      // こちらは入力の故障と異なり，故障挿入回路の出力が node->fvar() となる．
      // 逆に ovar はゲートの直接の出力変数となる．
      VarId ovar = node->fvar();
      for (ymuint i = 0; i < nf; ++ i) {
	if ( fnode_list[i] == node ) {
	  make_flt_cnf(solver, tmp_var[i], flt_var[i], ovar, flist[i]->val());
	  ovar = tmp_var[i];
	}
      }

      Literal glit(node->gvar(), false);
      Literal flit(node->fvar(), false);
      Literal dlit(node->dvar(), false);

      // XOR(glit, flit, dlit) を追加する．
      // 要するに正常回路と故障回路で異なっているとき dlit が 1 となる．
      solver.add_clause(~glit, ~flit, ~dlit);
      solver.add_clause(~glit,  flit,  dlit);
      solver.add_clause( glit, ~flit,  dlit);
      solver.add_clause( glit,  flit, ~dlit);

      Literal olit(ovar, false);
      if ( node->is_input() ) {
	solver.add_clause(~glit,  olit);
	solver.add_clause( glit, ~olit);
      }
      else {
	make_node_cnf(solver, node, Fvar2LitMap(node, ovar));

	make_dlit_cnf(solver, node, fnode_list, flt_var);
      }
    }

    cnf_end();

    // 個々の故障に対するテスト生成を行なう．
    for (ymuint fid = 0; fid < nf; ++ fid) {
      TpgFault* f = flist[fid];

      if ( f->status() == kFsDetected ) {
	// 他の故障のパタンで検出済みになっている場合がある．
	continue;
      }

      if ( f->untest_num() > mSkipThreshold ) {
	// 他の出力での検出不能回数がしきい値を越えたらスキップする．
	continue;
      }

      if ( mTimerEnable ) {
	mTimer.reset();
	mTimer.start();
      }

      tmp_lits_begin();

      //////////////////////////////////////////////////////////////////////
      // 故障の検出条件
      //////////////////////////////////////////////////////////////////////
      tmp_lits_add(Literal(onode->dvar(), false));

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

      // dominator ノードの dvar は1でなければならない．
      if ( mUseLocalDominator ) {
	for (TpgNode* node = f->node(); node != NULL; node = node->imm_dom(oid)) {
	  Literal dlit(node->dvar(), false);
	  tmp_lits_add(dlit);
	}
      }
      else {
	for (TpgNode* node = f->node(); node != NULL; node = node->imm_dom()) {
	  Literal dlit(node->dvar(), false);
	  tmp_lits_add(dlit);
	}
      }

      // 故障値を設定する．
      bool inv = (f->val() == 0);
      tmp_lits_add(Literal(fnode->fvar(), inv));

      SatStats prev_stats;
      solver.get_stats(prev_stats);

      Bool3 ans = solver.solve(mTmpLits, mModel);

      SatStats sat_stats;
      solver.get_stats(sat_stats);

      sub_sat_stats(sat_stats, prev_stats);

      USTime time(0, 0, 0);
      if ( mTimerEnable ) {
	mTimer.stop();
	time = mTimer.time();
      }

      if ( ans == kB3True ) {

	// バックトレースを行う．
	TestVector* tv = bt(f->node(), mModel, mInputList, mOutputList);

	// パタンの登録などを行う．
	dop(f, tv);

	stats_detect(sat_stats, time);
      }
      else if ( ans == kB3False ) {
	if ( opos == no - 1 ) {
	  uop(f);
	}
	else {
	  f->inc_untest_num();
	}
	stats_undetect(sat_stats, time);
      }
      else {
	stats_abort(sat_stats, time);
      }
    }

    bool faults_remain = false;
    for (ymuint fid = 0; fid < nf; ++ fid) {
      TpgFault* f = flist[fid];

      if ( f->status() != kFsDetected ) {
	faults_remain = true;
	break;
      }
    }

    if ( !faults_remain ) {
      break;
    }
  }

  bool faults_remain = false;
  for (ymuint fid = 0; fid < nf; ++ fid) {
    TpgFault* f = flist[fid];

    if ( f->status() != kFsDetected ) {
      faults_remain = true;
      break;
    }
  }

  if ( faults_remain ) {
    tmp_lits_begin(output_list().size());
    for (vector<TpgNode*>::const_iterator p = output_list().begin();
	 p != output_list().end(); ++ p) {
      TpgNode* node = *p;
      Literal dlit(node->dvar(), false);
      tmp_lits_add(dlit);
    }
    tmp_lits_end(solver);

    for (ymuint fid = 0; fid < nf; ++ fid) {
      TpgFault* f = flist[fid];

      if ( f->status() == kFsDetected ) {
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
  }

  clear_node_mark();
}

END_NAMESPACE_YM_SATPG
