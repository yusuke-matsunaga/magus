﻿
/// @file SatEngineSingle2.cc
/// @brief SatEngineSingle2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "SatEngineSingle2.h"

#include "DtpgStats.h"
#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "YmLogic/SatSolver.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief Single2 エンジンを作る．
// @param[in] th_val しきい値
// @param[in] sat_type SATソルバの種類を表す文字列
// @param[in] sat_option SATソルバに渡すオプション文字列
// @param[in] sat_outp SATソルバ用の出力ストリーム
// @param[in] max_id ノード番号の最大値 + 1
// @param[in] bt バックトレーサー
// @param[in] dop パタンが求められた時に実行されるファンクタ
// @param[in] uop 検出不能と判定された時に実行されるファンクタ
DtpgEngine*
new_SatEngineSingle2(ymuint th_val,
		     const string& sat_type,
		     const string& sat_option,
		     ostream* sat_outp,
		     ymuint max_id,
		     BackTracer& bt,
		     DetectOp& dop,
		     UntestOp& uop)
{
  return new SatEngineSingle2(th_val, sat_type, sat_option, sat_outp, max_id, bt, dop, uop);
}

// @brief コンストラクタ
SatEngineSingle2::SatEngineSingle2(ymuint th_val,
				   const string& sat_type,
				   const string& sat_option,
				   ostream* sat_outp,
				   ymuint max_id,
				   BackTracer& bt,
				   DetectOp& dop,
				   UntestOp& uop) :
  SatEngine(sat_type, sat_option, sat_outp, max_id, bt, dop, uop),
  mThVal(th_val),
  mMark(max_id, 0)
{
}

// @brief デストラクタ
SatEngineSingle2::~SatEngineSingle2()
{
}

// @brief テストパタン生成を行なう．
void
SatEngineSingle2::run(TpgFault* fault)
{
  TpgNode* fnode = fault->node();
  int fval = fault->val();

  SatSolver solver(sat_type(), sat_option(), sat_outp());

  mark_region(solver, vector<TpgNode*>(1, fnode));

  const vector<TpgNode*>& olist = output_list();
  ymuint no = olist.size();

  //////////////////////////////////////////////////////////////////////
  // 故障の検出条件
  //////////////////////////////////////////////////////////////////////
  tmp_lits_begin(no);
  for (vector<TpgNode*>::const_iterator p = olist.begin();
       p != olist.end(); ++ p) {
    TpgNode* node = *p;
    Literal dlit(node->dvar(), false);
    tmp_lits_add(dlit);
  }
  tmp_lits_end(solver);

  ymuint th_val = mThVal;
  if ( th_val > no ) {
    th_val = no;
  }
  bool fault_remain = false;
  for (ymuint opos = 0; opos < th_val; ++ opos) {
    // onode に関係する部分の CNF を生成する．
    TpgNode* onode = olist[opos];
    ymuint oid = onode->output_id2();

    cnf_begin();

    //////////////////////////////////////////////////////////////////////
    // 正常回路の CNF を生成
    //////////////////////////////////////////////////////////////////////
    for (ymuint i = 0; i < tfo_tfi_size(); ++ i) {
      TpgNode* node = tfo_tfi_node(i);
      if ( node->is_in_TFI_of(oid) && mMark[node->id()] == 0 ) {
	mMark[node->id()] = opos + 1;

	make_gnode_cnf(solver, node);
      }
    }

    //////////////////////////////////////////////////////////////////////
    // 故障回路の CNF を生成
    //////////////////////////////////////////////////////////////////////
    for (ymuint i = 0; i < tfo_size(); ++ i) {
      TpgNode* node = tfo_tfi_node(i);
      if ( mMark[node->id()] != (opos + 1) ) {
	continue;
      }

      // 故障回路のゲートの入出力関係を表すCNFを作る．
      if ( node == fnode ) {
	make_fault_cnf(solver, fault);
      }
      else {
	make_fnode_cnf(solver, node);
      }

      make_dchain_cnf(solver, node, fault);
    }

    cnf_end();

    timer_start();

    tmp_lits_begin();

    // まだ作っていない部分の dlit を 0 にする．
    for (ymuint i = 0; i < tfo_size(); ++ i) {
      TpgNode* node = tfo_tfi_node(i);
      if ( mMark[node->id()] == 0 ) {
	Literal dlit(node->dvar(), false);
	tmp_lits_add(~dlit);
      }
    }

    if ( use_dominator() ) {
      // dominator ノードの dvar は1でなければならない．
      for (TpgNode* node = fnode; node != NULL; node = node->imm_dom()) {
	tmp_lits_add(Literal(node->dvar(), false));
      }
    }

    SatStats prev_stats;
    solver.get_stats(prev_stats);

    Bool3 ans = _solve(solver);

    SatStats sat_stats;
    solver.get_stats(sat_stats);

    sub_sat_stats(sat_stats, prev_stats);

    USTime time = timer_stop();

    if ( ans == kB3True ) {
      detect_op(fault, sat_stats, time);
      break;
    }
    else if ( ans == kB3False ) {
      if ( opos == no - 1 ) {
	untest_op(fault, sat_stats, time);
      }
      else {
	partially_untest_op(fault, sat_stats, time);
	fault_remain = true;
      }
    }
    else {
      abort_op(fault, sat_stats, time);
    }
  }

  if ( fault_remain ) {
    // 最後に全部の出力を対象にしてテスト生成を実行する．

    cnf_begin();

    //////////////////////////////////////////////////////////////////////
    // 正常回路の CNF を生成
    //////////////////////////////////////////////////////////////////////
    for (ymuint i = 0; i < tfo_tfi_size(); ++ i) {
      TpgNode* node = tfo_tfi_node(i);
      if ( mMark[node->id()] == 0 ) {
	make_gnode_cnf(solver, node);
      }
    }

    //////////////////////////////////////////////////////////////////////
    // 故障回路の CNF を生成
    //////////////////////////////////////////////////////////////////////
    for (ymuint i = 0; i < tfo_size(); ++ i) {
      TpgNode* node = tfo_tfi_node(i);
      if ( mMark[node->id()] == 0 ) {
	make_fnode_cnf(solver, node);
	make_dchain_cnf(solver, node, fault);
      }
    }

    cnf_end();

    tmp_lits_begin();

    if ( use_dominator() ) {
      // dominator ノードの dvar は1でなければならない．
      for (TpgNode* node = fnode; node != NULL; node = node->imm_dom()) {
	tmp_lits_add(Literal(node->dvar(), false));
      }
    }
    else {
      solver.add_clause(Literal(fnode->dvar(), false));
    }

    solve(solver, fault);

  }

  clear_node_mark();

  for (ymuint i = 0; i < tfo_tfi_size(); ++ i) {
    TpgNode* node = tfo_tfi_node(i);
    mMark[node->id()] = 0;
  }
}

// @brief テスト生成を行なう．
// @param[in] flist 対象の故障リスト
void
SatEngineSingle2::run(const vector<TpgFault*>& flist)
{
  for (vector<TpgFault*>::const_iterator p = flist.begin();
       p != flist.end(); ++ p) {
    TpgFault* f = *p;
    run(f);
  }
}

END_NAMESPACE_YM_SATPG
