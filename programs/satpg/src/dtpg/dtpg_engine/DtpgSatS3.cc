
/// @file DtpgSatS3.cc
/// @brief DtpgSatS3 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSatS3.h"

#include "DtpgStats.h"
#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "SatEngine.h"
#include "NodeValList.h"
#include "BackTracer.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE
const bool do_verify = false;
END_NONAMESPACE

// @brief Single エンジンを作る．
// @param[in] sat_type SATソルバの種類を表す文字列
// @param[in] sat_option SATソルバに渡すオプション文字列
// @param[in] sat_outp SATソルバ用の出力ストリーム
// @param[in] bt バックトレーサー
// @param[in] dop パタンが求められた時に実行されるファンクタ
// @param[in] uop 検出不能と判定された時に実行されるファンクタ
DtpgEngine*
new_DtpgSatS3(const string& sat_type,
	      const string& sat_option,
	      ostream* sat_outp,
	      BackTracer& bt,
	      DetectOp& dop,
	      UntestOp& uop)
{
  return new DtpgSatS3(sat_type, sat_option, sat_outp, bt, dop, uop);
}

// @brief コンストラクタ
DtpgSatS3::DtpgSatS3(const string& sat_type,
		     const string& sat_option,
		     ostream* sat_outp,
		     BackTracer& bt,
		     DetectOp& dop,
		     UntestOp& uop) :
  DtpgSat(sat_type, sat_option, sat_outp, bt, dop, uop)
{
  mBt = new_BtJust3();
}

// @brief デストラクタ
DtpgSatS3::~DtpgSatS3()
{
  delete mBt;
}

// @brief テスト生成を行なう．
// @param[in] network 対象のネットワーク
// @param[in] stats 結果を格納する構造体
void
DtpgSatS3::run(TpgNetwork& network,
	       DtpgStats& stats)
{
  clear_stats();

  ymuint nn = network.active_node_num();
  ymuint max_fault_id = 0;
  for (ymuint i = 0; i < nn; ++ i) {
    TpgNode* node = network.active_node(i);
    if ( node->is_output() ) {
      continue;
    }

    ymuint nf = node->fault_num();
    for (ymuint i = 0; i < nf; ++ i) {
      TpgFault* f = node->fault(i);
      if ( f->status() != kFsDetected &&
	   !f->is_skip() ) {
	ymuint fid = f->id();
	if ( max_fault_id < fid ) {
	  max_fault_id = fid;
	}
      }
    }
  }

  ++ max_fault_id;
  mFaultInfoArray.clear();
  mFaultInfoArray.resize(max_fault_id);

  // 一回目は普通のテスト生成と必要割当の検査
  vector<TpgFault*> fault_list;
  ymuint max_id = network.max_node_id();
  for (ymuint i = 0; i < nn; ++ i) {
    TpgNode* node = network.active_node(i);
    if ( node->is_output() ) {
      continue;
    }

    mark_region(max_id, vector<TpgNode*>(1, node));

    ymuint nf = node->fault_num();
    for (ymuint i = 0; i < nf; ++ i) {
      TpgFault* f = node->fault(i);
      FaultInfo& fi = mFaultInfoArray[f->id()];
      fi.mDetected = run_single(network, f);
      if ( fi.mDetected ) {
	fault_list.push_back(f);
      }
    }

    clear_node_mark();
  }
#if 0
  // 二回目は包含関係と排他性の検査
  for (ymuint i = 0; i < nn; ++ i) {
    TpgNode* node = network.active_node(i);
    if ( node->is_output() ) {
      continue;
    }

    mark_region(max_id, vector<TpgNode*>(1, node));

    ymuint nf = node->fault_num();
    for (ymuint i = 0; i < nf; ++ i) {
      TpgFault* f = node->fault(i);
      FaultInfo& fi = mFaultInfoArray[f->id()];
      if ( fi.mDetected ) {
	check_other_faults(network, f, fault_list);
      }
    }
    clear_node_mark();
  }
#endif

  get_stats(stats);
}

// @brief テストパタン生成を行なう．
// @param[in] network 対象のネットワーク
// @param[in] flist 故障リスト
bool
DtpgSatS3::run_single(TpgNetwork& network,
		      TpgFault* fault)
{
  mBt->set_max_id(network.max_node_id());

  TpgNode* fnode = fault->node();

  cnf_begin();

  SatEngine engine(sat_type(), sat_option(), sat_outp());

  //////////////////////////////////////////////////////////////////////
  // 変数の割当
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < tfo_tfi_size(); ++ i) {
    TpgNode* node = tfo_tfi_node(i);
    VarId gvar = engine.new_var();
    node->set_gvar(gvar);
  }
  for (ymuint i = 0; i < tfo_size(); ++ i) {
    TpgNode* node = tfo_tfi_node(i);
    VarId fvar = engine.new_var();
    VarId dvar = engine.new_var();
    node->set_fvar(fvar, dvar);
  }

  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < tfo_tfi_size(); ++ i) {
    TpgNode* node = tfo_tfi_node(i);
    make_gval_cnf(engine, node);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < tfo_size(); ++ i) {
    TpgNode* node = tfo_tfi_node(i);

    // 故障回路のゲートの入出力関係を表すCNFを作る．
    if ( node == fnode ) {
      make_fault_cnf(engine, fault);
    }
    else {
      make_fval_cnf(engine, node);
    }

    // D-Chain 制約を作る．
    make_dchain_cnf(engine, node);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障の検出条件
  //////////////////////////////////////////////////////////////////////
  ymuint npo = output_list().size();
  engine.tmp_lits_begin(npo);
  for (ymuint i = 0; i < npo; ++ i) {
    TpgNode* node = output_list()[i];
    Literal dlit(node->dvar(), false);
    engine.tmp_lits_add(dlit);
  }
  engine.tmp_lits_end();

  cnf_end();

  // 故障に対するテスト生成を行なう．
  engine.assumption_begin();

  // dominator ノードの dvar は1でなければならない．
  for (TpgNode* node = fnode; node != NULL; node = node->imm_dom()) {
    Literal dlit(node->dvar(), false);
    engine.assumption_add(dlit);
  }

  vector<Bool3> sat_model;
  Bool3 sat_ans = engine.solve(sat_model);
  if ( sat_ans != kB3True ) {
    return false;
  }

  // 十分割当を求める．
  NodeValList& suf_list = mFaultInfoArray[fault->id()].mSufList;
  (*mBt)(fault->node(), sat_model, input_list(), output_list(), suf_list);

  if ( do_verify ) { // 検証
    SatEngine engine(sat_type(), sat_option(), sat_outp());

    //////////////////////////////////////////////////////////////////////
    // 変数の割当
    //////////////////////////////////////////////////////////////////////
    for (ymuint i = 0; i < tfo_tfi_size(); ++ i) {
      TpgNode* node = tfo_tfi_node(i);
      VarId gvar = engine.new_var();
      node->set_gvar(gvar);
    }
    for (ymuint i = 0; i < tfo_size(); ++ i) {
      TpgNode* node = tfo_tfi_node(i);
      VarId fvar = engine.new_var();
      VarId dvar = engine.new_var();
      node->set_fvar(fvar, dvar);
    }

    //////////////////////////////////////////////////////////////////////
    // 正常回路の CNF を生成
    //////////////////////////////////////////////////////////////////////
    for (ymuint i = 0; i < tfo_tfi_size(); ++ i) {
      TpgNode* node = tfo_tfi_node(i);
      make_gval_cnf(engine, node);
    }

    //////////////////////////////////////////////////////////////////////
    // 故障回路の CNF を生成
    //////////////////////////////////////////////////////////////////////
    for (ymuint i = 0; i < tfo_size(); ++ i) {
      TpgNode* node = tfo_tfi_node(i);

      // 故障回路のゲートの入出力関係を表すCNFを作る．
      if ( node == fnode ) {
	make_fault_cnf(engine, fault);
      }
      else {
	make_fval_cnf(engine, node);
      }

      // D-Chain 制約を作る．
      make_dchain_cnf(engine, node);
    }

    // 上の割当のもとでは常に故障が検出できることを検証する．
    engine.assumption_begin();

    ymuint n = suf_list.size();
    for (ymuint i = 0; i < n; ++ i) {
      NodeVal nv = suf_list[i];
      TpgNode* node = nv.node();
      VarId vid = node->gvar();
      bool inv = nv.val();
      Literal alit(vid, !inv);
      engine.assumption_add(alit);
    }

    //////////////////////////////////////////////////////////////////////
    // 故障の非検出条件
    //////////////////////////////////////////////////////////////////////
    ymuint npo = output_list().size();
    for (ymuint i = 0; i < npo; ++ i) {
      TpgNode* node = output_list()[i];
      Literal dlit(node->dvar(), false);
      engine.assumption_add(~dlit);
    }
    vector<Bool3> tmp_model;
    Bool3 tmp_stat = engine.solve(tmp_model);
    if ( tmp_stat == kB3True ) {
      cout << "ERROR: not a sufficient condition" << endl;
      print_network(cout, network);
      for (ymuint i = 0; i < n; ++ i) {
	NodeVal nv = suf_list[i];
	TpgNode* node = nv.node();
	cout << "Node#" << node->id()
	     << ", gval = " << tmp_model[node->gvar().val()];
	if ( node->has_fvar() ) {
	  cout << ", fval = " << tmp_model[node->fvar().val()]
	       << ", dval = " << tmp_model[node->dvar().val()];
	}
	cout << endl;
      }
      cout << fault->str() << endl;
      for (ymuint i = 0; i < n; ++ i) {
	NodeVal nv = suf_list[i];
	TpgNode* node = nv.node();
	cout << " Node#" << node->id() << ":";
	if ( nv.val() ) {
	  cout << "1";
	}
	else {
	  cout << "0";
	}
      }
      cout << endl;
      abort();
    }
  }

  // 必要割当を求める．
  vector<Bool3> tmp_model;
  ymuint n = suf_list.size();
  NodeValList& ma_list = mFaultInfoArray[fault->id()].mMaList;
  for (ymuint i = 0; i < n; ++ i) {
    NodeVal nv = suf_list[i];

    engine.assumption_begin();

    // dominator ノードの dvar は1でなければならない．
    for (TpgNode* node = fnode; node != NULL; node = node->imm_dom()) {
      Literal dlit(node->dvar(), false);
      engine.assumption_add(dlit);
    }

    // node の割当の反対を試す．
    TpgNode* node = nv.node();
    bool inv = nv.val();
    Literal alit(node->gvar(), inv);
    engine.assumption_add(alit);

    Bool3 tmp_stat = engine.solve(tmp_model);
    if ( tmp_stat == kB3True ) {
      // 反対でも検出できたので必要割当ではない．
      ;
    }
    else if ( tmp_stat == kB3False ) {
      // 失敗したということはこの割当は必要である．
      ma_list.add(node, nv.val());
    }
    else {
      // アボート．とりあえず無視
      ;
    }
  }
  cout << fault->str() << ": sufficient assignment = " << suf_list.size()
       << ", mandatory assignment = " << ma_list.size() << endl;

  return true;
}

// @brief 他の故障との関係を調べる．
void
DtpgSatS3::check_other_faults(TpgNetwork& network,
			      TpgFault* f1,
			      const vector<TpgFault*>& fault_list)
{
  TpgNode* fnode = f1->node();

  cnf_begin();

  SatEngine engine(sat_type(), sat_option(), sat_outp());

  //////////////////////////////////////////////////////////////////////
  // 変数の割当
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < tfo_tfi_size(); ++ i) {
    TpgNode* node = tfo_tfi_node(i);
    VarId gvar = engine.new_var();
    node->set_gvar(gvar);
  }
  for (ymuint i = 0; i < tfo_size(); ++ i) {
    TpgNode* node = tfo_tfi_node(i);
    VarId fvar = engine.new_var();
    VarId dvar = engine.new_var();
    node->set_fvar(fvar, dvar);
  }

  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < tfo_tfi_size(); ++ i) {
    TpgNode* node = tfo_tfi_node(i);
    make_gval_cnf(engine, node);
    cout << "Node#" << node->id() << ": gvar = " << node->gvar() << endl;
  }

  //////////////////////////////////////////////////////////////////////
  // 故障回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < tfo_size(); ++ i) {
    TpgNode* node = tfo_tfi_node(i);

    // 故障回路のゲートの入出力関係を表すCNFを作る．
    if ( node == fnode ) {
      make_fault_cnf(engine, f1);
    }
    else {
      make_fval_cnf(engine, node);
    }

    // D-Chain 制約を作る．
    make_dchain_cnf(engine, node);

    cout << "Node#" << node->id() << ": fvar = " << node->fvar()
	 << ", dvar = " << node->dvar() << endl;
  }

  VarId switch_var = engine.new_var();
  Literal switch_lit(switch_var, false);

  cout << "switch_var = " << switch_var << endl;

  //////////////////////////////////////////////////////////////////////
  // 故障の検出条件
  //////////////////////////////////////////////////////////////////////
  ymuint npo = output_list().size();
  engine.tmp_lits_begin(npo + 1);
  for (ymuint i = 0; i < npo; ++ i) {
    TpgNode* node = output_list()[i];
    Literal dlit(node->dvar(), false);
    engine.tmp_lits_add(dlit);
  }
  engine.tmp_lits_add(~switch_lit);
  engine.tmp_lits_end();

  cnf_end();

  for (vector<TpgFault*>::const_iterator p = fault_list.begin();
       p != fault_list.end(); ++ p) {
    TpgFault* f2 = *p;

    // f2 の必要割当のもとで f1 が求められるか調べる．

    // 故障に対するテスト生成を行なう．
    engine.assumption_begin();

    engine.assumption_add(switch_lit);

    // dominator ノードの dvar は1でなければならない．
    for (TpgNode* node = fnode; node != NULL; node = node->imm_dom()) {
      Literal dlit(node->dvar(), false);
      engine.assumption_add(dlit);
    }

    bool assumed = false;
    const NodeValList& ma_list = mFaultInfoArray[f2->id()].mMaList;
    for (ymuint i = 0; i < ma_list.size(); ++ i) {
      NodeVal nv = ma_list[i];
      TpgNode* node1 = nv.node();
      if ( node1->has_gvar() ) {
	assumed = true;
	Literal mlit(node1->gvar(), false);
	if ( nv.val() ) {
	  engine.assumption_add(mlit);
	}
	else {
	  engine.assumption_add(~mlit);
	}
	cout << " mlit = " << mlit << ", Node#" << node1->id() << endl;
      }
    }

    if ( !assumed ) {
      continue;
    }

    vector<Bool3> tmp_model;
    Bool3 tmp_stat = engine.solve(tmp_model);
    if ( tmp_stat == kB3True ) {
      // 検出できた．
      ;
    }
    else if ( tmp_stat == kB3False ) {
      // 検出できなかった．
      cout << f1->str() << " and " << f2->str() << " conflict with each other" << endl;
    }
    else {
      // アボート．とりあえず無視
      ;
    }
  }
}

// @brief コンストラクタ
DtpgSatS3::FaultInfo::FaultInfo()
{
  mDetected = false;
}

// @brief デストラクタ
DtpgSatS3::FaultInfo::~FaultInfo()
{
}

END_NAMESPACE_YM_SATPG
