
/// @file DtpgSatM2.cc
/// @brief DtpgSatM2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSatM2.h"

#include "DtpgStats.h"
#include "NodeSet.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "SatEngine.h"
#include "GenVidMap.h"
#include "ModelValMap.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief Multi2 エンジンを作る．
// @param[in] th_val しきい値
// @param[in] sat_type SATソルバの種類を表す文字列
// @param[in] sat_option SATソルバに渡すオプション文字列
// @param[in] sat_outp SATソルバ用の出力ストリーム
// @param[in] bt バックトレーサー
// @param[in] dop パタンが求められた時に実行されるファンクタ
// @param[in] uop 検出不能と判定された時に実行されるファンクタ
DtpgEngine*
new_DtpgSatM2(ymuint th_val,
	      const string& sat_type,
	      const string& sat_option,
	      ostream* sat_outp,
	      BackTracer& bt,
	      DetectOp& dop,
	      UntestOp& uop)
{
  return new DtpgSatM2(th_val, sat_type, sat_option, sat_outp, bt, dop, uop);
}

// @brief コンストラクタ
DtpgSatM2::DtpgSatM2(ymuint th_val,
		     const string& sat_type,
		     const string& sat_option,
		     ostream* sat_outp,
		     BackTracer& bt,
		     DetectOp& dop,
		     UntestOp& uop) :
  DtpgSatBaseM(sat_type, sat_option, sat_outp, bt, dop, uop),
  mThVal(th_val)
{
}

// @brief デストラクタ
DtpgSatM2::~DtpgSatM2()
{
}

// @brief テストパタン生成を行なう．
// @param[in] node_set 対象のノード集合
// @param[in] fnode_list 対象の故障を持つノードのリスト
// @param[in] flist 故障リスト
void
DtpgSatM2::run_multi(const NodeSet& node_set,
		     const vector<TpgNode*>& fnode_list,
		     const vector<TpgFault*>& flist)
{
  SatEngine engine(sat_type(), sat_option(), sat_outp());

  ymuint nf = flist.size();

  ymuint max_id = node_set.max_id();

  mMark.clear();
  mMark.resize(max_id, 0);

  mTmpMark.clear();
  mTmpMark.resize(max_id, false);

  GenVidMap gvar_map(max_id);
  GenVidMap fvar_map(max_id);
  GenVidMap dvar_map(max_id);

  //////////////////////////////////////////////////////////////////////
  // 変数の割当
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
    TpgNode* node = node_set.tfo_tfi_node(i);
    VarId gvar = engine.new_var();
    gvar_map.set_vid(node, gvar);
    fvar_map.set_vid(node, gvar);
  }
  for (ymuint i = 0; i < node_set.tfo_size(); ++ i) {
    TpgNode* node = node_set.tfo_tfi_node(i);
    VarId fvar = engine.new_var();
    VarId dvar = engine.new_var();
    fvar_map.set_vid(node, fvar);
    dvar_map.set_vid(node, dvar);
  }

  // 故障を活性化するとき true にする変数．
  vector<VarId> flt_var(nf);
  for (ymuint i = 0; i < nf; ++ i) {
    VarId fvar = engine.new_var();
    flt_var[i] = fvar;
    TpgFault* f = flist[i];
    int fval = f->val();
    TpgNode* node = f->node();

    if ( f->is_output_fault() ) {
      node->set_ofvar(fval, fvar);
    }
    else {
      ymuint pos = f->pos();
      node->set_ifvar(pos, fval, fvar);
    }
  }

  const vector<TpgNode*>& olist = node_set.output_list();
  ymuint no = olist.size();

  //////////////////////////////////////////////////////////////////////
  // 故障の検出条件
  //////////////////////////////////////////////////////////////////////
  engine.tmp_lits_begin(no);
  for (vector<TpgNode*>::const_iterator p = olist.begin();
       p != olist.end(); ++ p) {
    TpgNode* node = *p;
    Literal dlit(dvar_map(node), false);
    engine.tmp_lits_add(dlit);
  }
  engine.tmp_lits_end();

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
    for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
      TpgNode* node = node_set.tfo_tfi_node(i);
      if ( node->is_in_TFI_of(oid) && mMark[node->id()] == 0 ) {
	mMark[node->id()] = opos + 1;

	engine.make_node_cnf(node, gvar_map);
      }
    }

    //////////////////////////////////////////////////////////////////////
    // 故障回路の CNF を生成
    //////////////////////////////////////////////////////////////////////
    for (ymuint i = 0; i < node_set.tfo_size(); ++ i) {
      TpgNode* node = node_set.tfo_tfi_node(i);
      if ( mMark[node->id()] != (opos + 1) ) {
	continue;
      }

      if ( node->has_flt_var() ) {
	engine.make_fnode_cnf(node, gvar_map, fvar_map);
      }
      else {
	engine.make_node_cnf(node, fvar_map);
      }

      engine.make_dchain_cnf(node, gvar_map, fvar_map, dvar_map);
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

      engine.assumption_begin();

      // 該当の故障に対する変数のみ1にする．
      for (ymuint j = 0; j < nf; ++ j) {
	bool inv = (j != fid);
	engine.assumption_add(Literal(flt_var[j], inv));
      }

      TpgNode* fnode = f->node();

      // 故障ノードの TFO 以外の dlit を0にする．
      mTmpNodeList.clear();
      mTmpNodeList.reserve(node_set.tfo_tfi_size());
      mTmpMark[fnode->id()] = true;
      mTmpNodeList.push_back(fnode);
      for (ymuint rpos = 0; rpos < mTmpNodeList.size(); ++ rpos) {
	TpgNode* node = mTmpNodeList[rpos];
	ymuint nfo = node->active_fanout_num();
	for (ymuint i = 0; i < nfo; ++ i) {
	  TpgNode* fonode = node->active_fanout(i);
	  if ( !mTmpMark[fonode->id()] ) {
	    mTmpMark[fonode->id()] = true;
	    mTmpNodeList.push_back(fonode);
	  }
	}
      }
      for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
	TpgNode* node = node_set.tfo_tfi_node(i);
	if ( node_set.tfo_mark(node) && !mTmpMark[node->id()] ) {
	  Literal dlit(dvar_map(node), true);
	  engine.assumption_add(dlit);
	}
      }
      for (vector<TpgNode*>::iterator p = mTmpNodeList.begin();
	   p != mTmpNodeList.end(); ++ p) {
	TpgNode* node = *p;
	mTmpMark[node->id()] = false;
      }
      mTmpNodeList.clear();

      // まだ作っていない部分の dlit を 0 にする．
      for (ymuint j = 0; j < node_set.tfo_size(); ++ j) {
	TpgNode* node = node_set.tfo_tfi_node(j);
	if ( mMark[node->id()] == 0 ) {
	  Literal dlit(dvar_map(node), false);
	  engine.assumption_add(~dlit);
	}
      }

      // dominator ノードの dvar は1でなければならない．
      for (TpgNode* node = f->node(); node != NULL; node = node->imm_dom()) {
	Literal dlit(dvar_map(node), false);
	engine.assumption_add(dlit);
      }

      vector<Bool3> sat_model;
      SatStats sat_stats;
      USTime time;
      Bool3 ans = engine.solve(sat_model, sat_stats, time);

      if ( ans == kB3True ) {
	ModelValMap val_map(gvar_map, fvar_map, sat_model);
	detect_op(f, node_set, val_map, sat_stats, time);

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
    for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
      TpgNode* node = node_set.tfo_tfi_node(i);
      if ( mMark[node->id()] == 0 ) {
	engine.make_node_cnf(node, gvar_map);
      }
    }

    //////////////////////////////////////////////////////////////////////
    // 故障回路の CNF を生成
    //////////////////////////////////////////////////////////////////////
    for (ymuint i = 0; i < node_set.tfo_size(); ++ i) {
      TpgNode* node = node_set.tfo_tfi_node(i);
      if ( mMark[node->id()] == 0 ) {
	engine.make_node_cnf(node, fvar_map);
	engine.make_dchain_cnf(node, gvar_map, fvar_map, dvar_map);
      }
    }

    cnf_end();

    for (ymuint fid = 0; fid < nf; ++ fid) {
      TpgFault* f = flist[fid];

      if ( f->status() == kFsDetected ) {
	continue;
      }

      engine.assumption_begin();

      // 該当の故障に対する変数のみ1にする．
      for (ymuint j = 0; j < nf; ++ j) {
	bool inv = (j != fid);
	engine.assumption_add(Literal(flt_var[j], inv));
      }

      TpgNode* fnode = f->node();

      // 故障ノードの TFO 以外の dlit を0にする．
      mTmpNodeList.clear();
      mTmpNodeList.reserve(node_set.tfo_tfi_size());
      mTmpMark[fnode->id()] = true;
      mTmpNodeList.push_back(fnode);
      for (ymuint rpos = 0; rpos < mTmpNodeList.size(); ++ rpos) {
	TpgNode* node = mTmpNodeList[rpos];
	ymuint nfo = node->active_fanout_num();
	for (ymuint i = 0; i < nfo; ++ i) {
	  TpgNode* fonode = node->active_fanout(i);
	  if ( !mTmpMark[fonode->id()] ) {
	    mTmpMark[fonode->id()] = true;
	    mTmpNodeList.push_back(fonode);
	  }
	}
      }
      for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
	TpgNode* node = node_set.tfo_tfi_node(i);
	if ( node_set.tfo_mark(node) && !mTmpMark[node->id()] ) {
	  Literal dlit(dvar_map(node), true);
	  engine.assumption_add(dlit);
	}
      }
      for (vector<TpgNode*>::iterator p = mTmpNodeList.begin();
	   p != mTmpNodeList.end(); ++ p) {
	TpgNode* node = *p;
	mTmpMark[node->id()] = false;
      }
      mTmpNodeList.clear();

      // dominator ノードの dvar は1でなければならない．
      for (TpgNode* node = f->node(); node != NULL; node = node->imm_dom()) {
	Literal dlit(dvar_map(node), false);
	engine.assumption_add(dlit);
      }

      solve(engine, f, node_set, gvar_map, fvar_map);
    }
  }

  for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
    TpgNode* node = node_set.tfo_tfi_node(i);
    mMark[node->id()] = 0;
  }

  for (vector<TpgNode*>::const_iterator p = fnode_list.begin();
       p != fnode_list.end(); ++ p) {
    TpgNode* node = *p;
    node->clear_oifvar();
  }
}

END_NAMESPACE_YM_SATPG
