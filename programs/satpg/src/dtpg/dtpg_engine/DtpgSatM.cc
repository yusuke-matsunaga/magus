
/// @file DtpgSatM.cc
/// @brief DtpgSatM の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSatM.h"

#include "DtpgStats.h"
#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "SatEngine.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief Multi エンジンを作る．
// @param[in] sat_type SATソルバの種類を表す文字列
// @param[in] sat_option SATソルバに渡すオプション文字列
// @param[in] sat_outp SATソルバ用の出力ストリーム
// @param[in] network 対象のネットワーク
// @param[in] bt バックトレーサー
// @param[in] dop パタンが求められた時に実行されるファンクタ
// @param[in] uop 検出不能と判定された時に実行されるファンクタ
DtpgEngine*
new_DtpgSatM(const string& sat_type,
	     const string& sat_option,
	     ostream* sat_outp,
	     const TpgNetwork& network,
	     BackTracer& bt,
	     DetectOp& dop,
	     UntestOp& uop,
	     bool forget)
{
  return new DtpgSatM(sat_type, sat_option, sat_outp, network, bt, dop, uop, forget);
}

// @brief コンストラクタ
DtpgSatM::DtpgSatM(const string& sat_type,
		   const string& sat_option,
		   ostream* sat_outp,
		   const TpgNetwork& network,
		   BackTracer& bt,
		   DetectOp& dop,
		   UntestOp& uop,
		   bool forget) :
  DtpgSatBaseM(sat_type, sat_option, sat_outp, network, bt, dop, uop),
  mForget(forget)
{
}

// @brief デストラクタ
DtpgSatM::~DtpgSatM()
{
}

// @brief テストパタン生成を行なう．
// @param[in] flist 故障リスト
void
DtpgSatM::run_multi(const vector<TpgFault*>& flist)
{
  cnf_begin();

  SatEngine engine(sat_type(), sat_option(), sat_outp());

  ymuint nf = flist.size();

  // 故障を活性化するとき true にする変数．
  vector<VarId> flt_var(nf);
  // FnodeInfo を持つノードのリスト
  vector<TpgNode*> fnode_list;
  make_fnode_list(flist, fnode_list);

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

  mark_region(engine, fnode_list);


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

    if ( node->has_flt_var() ) {
      make_fnode_cnf(engine, node);
    }
    else {
      make_fval_cnf(engine, node);
    }

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

  // 個々の故障に対するテスト生成を行なう．
  for (ymuint fid = 0; fid < nf; ++ fid) {
    TpgFault* f = flist[fid];

    if ( f->status() == kFsDetected ) {
      // 他の故障のパタンで検出済みになっている場合がある．
      continue;
    }

    engine.assumption_begin();

    // 該当の故障に対する変数のみ1にする．
    for (ymuint j = 0; j < nf; ++ j) {
      bool inv = (j != fid);
      engine.assumption_add(Literal(flt_var[j], inv));
    }

    TpgNode* fnode = flist[fid]->node();

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
	engine.assumption_add(dlit);
      }
    }
    for (vector<TpgNode*>::iterator p = mTmpNodeList.begin();
	 p != mTmpNodeList.end(); ++ p) {
      TpgNode* node = *p;
      clear_tmp_mark(node);
    }
    mTmpNodeList.clear();

    // dominator ノードの dvar は1でなければならない．
    for (TpgNode* node = f->node(); node != NULL; node = node->imm_dom()) {
      Literal dlit(node->dvar(), false);
      engine.assumption_add(dlit);
    }

    solve(engine, f);
  }
  clear_node_mark();

  clear_fnode_list(fnode_list);
}

END_NAMESPACE_YM_SATPG
