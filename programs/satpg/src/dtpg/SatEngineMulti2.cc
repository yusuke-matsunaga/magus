
/// @file SatEngineMulti2.cc
/// @brief SatEngineMulti2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "SatEngineMulti2.h"

#include "DtpgStats.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "logic/SatSolver.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief Multi2 エンジンを作る．
// @param[in] th_val しきい値
// @param[in] sat_type SATソルバの種類を表す文字列
// @param[in] sat_option SATソルバに渡すオプション文字列
// @param[in] sat_outp SATソルバ用の出力ストリーム
// @param[in] max_id ノード番号の最大値 + 1
// @param[in] bt バックトレーサー
// @param[in] dop パタンが求められた時に実行されるファンクタ
// @param[in] uop 検出不能と判定された時に実行されるファンクタ
DtpgEngine*
new_SatEngineMulti2(ymuint th_val,
		    const string& sat_type,
		    const string& sat_option,
		    ostream* sat_outp,
		    ymuint max_id,
		    BackTracer& bt,
		    DetectOp& dop,
		    UntestOp& uop,
		    bool forget)
{
  return new SatEngineMulti2(th_val, sat_type, sat_option, sat_outp, max_id, bt, dop, uop, forget);
}

// @brief コンストラクタ
SatEngineMulti2::SatEngineMulti2(ymuint th_val,
				 const string& sat_type,
				 const string& sat_option,
				 ostream* sat_outp,
				 ymuint max_id,
				 BackTracer& bt,
				 DetectOp& dop,
				 UntestOp& uop,
				 bool forget) :
  SatEngine(sat_type, sat_option, sat_outp, max_id, bt, dop, uop),
  mThVal(th_val),
  mDone(max_id, false),
  mMark(max_id, 0),
  mForget(forget)
{
  if ( mForget ) {
    cout << "multi2_forget" << endl;
  }
}

// @brief デストラクタ
SatEngineMulti2::~SatEngineMulti2()
{
}

// @brief テスト生成を行なう．
// @param[in] f_tgt 対象の故障
void
SatEngineMulti2::run(TpgFault* f_tgt)
{
  run(vector<TpgFault*>(1, f_tgt));
}

// @brief テストパタン生成を行なう．
// @param[in] flist 故障リスト
// @param[in] th_val
void
SatEngineMulti2::run(const vector<TpgFault*>& flist)
{
  SatSolver solver(sat_type(), sat_option(), sat_outp());

  ymuint nf = flist.size();

  // 故障を活性化するとき true にする変数．
  vector<VarId> flt_var(nf);
  // FnodeInfo を持つノードのリスト
  vector<TpgNode*> fnode_list;
  fnode_list.reserve(nf);
  for (ymuint i = 0; i < nf; ++ i) {
    VarId fvar = solver.new_var();
    flt_var[i] = fvar;
    TpgFault* f = flist[i];
    int fval = f->val();
    TpgNode* node = f->node();
    if ( !mDone[node->id()] ) {
      mDone[node->id()] = true;
      fnode_list.push_back(node);
    }

    if ( f->is_output_fault() ) {
      node->set_ofvar(fval, fvar);
    }
    else {
      ymuint pos = f->pos();
      node->set_ifvar(pos, fval, fvar);
    }
  }

  mark_region(solver, fnode_list);

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
  bool faults_remain = false;
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

      make_fnode_cnf(solver, node);

      make_dchain_cnf(solver, node);
    }

    cnf_end();

    faults_remain = false;

    // 個々の故障に対するテスト生成を行なう．
    for (ymuint fid = 0; fid < nf; ++ fid) {
      TpgFault* f = flist[fid];

      if ( f->status() == kFsDetected ) {
	// 他の故障のパタンで検出済みになっている場合がある．
	continue;
      }


      timer_start();

      if ( mForget ) {
	solver.forget_learnt_clause();
      }

      tmp_lits_begin();

      // 該当の故障に対する変数のみ1にする．
      for (ymuint j = 0; j < nf; ++ j) {
	bool inv = (j != fid);
	tmp_lits_add(Literal(flt_var[j], inv));
      }

      TpgNode* fnode = f->node();

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

      // まだ作っていない部分の dlit を 0 にする．
      for (ymuint j = 0; j < tfo_size(); ++ j) {
	TpgNode* node = tfo_tfi_node(j);
	if ( mMark[node->id()] == 0 ) {
	  Literal dlit(node->dvar(), false);
	  tmp_lits_add(~dlit);
	}
      }

      // dominator ノードの dvar は1でなければならない．
      if ( use_dominator() ) {
	for (TpgNode* node = f->node(); node != NULL; node = node->imm_dom()) {
	  Literal dlit(node->dvar(), false);
	  tmp_lits_add(dlit);
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
	detect_op(f, sat_stats, time);
      }
      else if ( ans == kB3False ) {
	if ( opos == no - 1 ) {
	  untest_op(f, sat_stats, time);
	}
	else {
	  partially_untest_op(f, sat_stats, time);
	  faults_remain = true;
	}
      }
      else {
	abort_op(f, sat_stats, time);
      }
    }
    if ( !faults_remain ) {
      break;
    }
  }

  if ( faults_remain ) {
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
	make_dchain_cnf(solver, node);
      }
    }

    cnf_end();

    for (ymuint fid = 0; fid < nf; ++ fid) {
      TpgFault* f = flist[fid];

      if ( f->status() == kFsDetected ) {
	continue;
      }

      if ( mForget ) {
	solver.forget_learnt_clause();
      }

      tmp_lits_begin();

      // 該当の故障に対する変数のみ1にする．
      for (ymuint j = 0; j < nf; ++ j) {
	bool inv = (j != fid);
	tmp_lits_add(Literal(flt_var[j], inv));
      }

      TpgNode* fnode = f->node();

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

      if ( use_dominator() ) {
	// dominator ノードの dvar は1でなければならない．
	for (TpgNode* node = f->node(); node != NULL; node = node->imm_dom()) {
	  Literal dlit(node->dvar(), false);
	  tmp_lits_add(dlit);
	}
      }
      else {
	tmp_lits_add(Literal(f->node()->dvar(), false));
      }

      solve(solver, f);
    }
  }

  clear_node_mark();

  for (ymuint i = 0; i < tfo_tfi_size(); ++ i) {
    TpgNode* node = tfo_tfi_node(i);
    mMark[node->id()] = 0;
  }

  for (ymuint i = 0; i < fnode_list.size(); ++ i) {
    TpgNode* node = fnode_list[i];
    node->clear_oifvar();
    mDone[node->id()] = false;
  }
}

END_NAMESPACE_YM_SATPG
