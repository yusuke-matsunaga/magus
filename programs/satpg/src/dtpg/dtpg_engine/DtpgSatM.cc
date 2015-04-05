
/// @file DtpgSatM.cc
/// @brief DtpgSatM の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSatM.h"

#include "DtpgStats.h"
#include "NodeSet.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "SatEngine.h"
#include "GenVidMap.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief Multi エンジンを作る．
// @param[in] sat_type SATソルバの種類を表す文字列
// @param[in] sat_option SATソルバに渡すオプション文字列
// @param[in] sat_outp SATソルバ用の出力ストリーム
// @param[in] bt バックトレーサー
// @param[in] dop パタンが求められた時に実行されるファンクタ
// @param[in] uop 検出不能と判定された時に実行されるファンクタ
DtpgEngine*
new_DtpgSatM(const string& sat_type,
	     const string& sat_option,
	     ostream* sat_outp,
	     BackTracer& bt,
	     DetectOp& dop,
	     UntestOp& uop)
{
  return new DtpgSatM(sat_type, sat_option, sat_outp, bt, dop, uop);
}

// @brief コンストラクタ
DtpgSatM::DtpgSatM(const string& sat_type,
		   const string& sat_option,
		   ostream* sat_outp,
		   BackTracer& bt,
		   DetectOp& dop,
		   UntestOp& uop) :
  DtpgSatBaseM(sat_type, sat_option, sat_outp, bt, dop, uop)
{
}

// @brief デストラクタ
DtpgSatM::~DtpgSatM()
{
}

// @brief テストパタン生成を行なう．
// @param[in] node_set 対象のノード集合
// @param[in] fnode_list 対象の故障を持つノードのリスト
// @param[in] flist 故障リスト
void
DtpgSatM::run_multi(const NodeSet& node_set,
		    const vector<const TpgNode*>& fnode_list,
		    const vector<TpgFault*>& flist)
{
  cnf_begin();

  SatEngine engine(sat_type(), sat_option(), sat_outp());

  ymuint nf = flist.size();

  ymuint max_id = node_set.max_id();

  GenVidMap gvar_map(max_id);
  GenVidMap fvar_map(max_id);
  GenVidMap dvar_map(max_id);

  mMarkArray.clear();
  mMarkArray.resize(max_id, false);

  //////////////////////////////////////////////////////////////////////
  // 変数の割当
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    VarId gvar = engine.new_var();
    gvar_map.set_vid(node, gvar);
    fvar_map.set_vid(node, gvar);
  }
  for (ymuint i = 0; i < node_set.tfo_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
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
    const TpgNode* node = f->node();

    if ( f->is_output_fault() ) {
      //node->set_ofvar(fval, fvar);
    }
    else {
      ymuint pos = f->pos();
      //node->set_ifvar(pos, fval, fvar);
    }
  }

  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    engine.make_node_cnf(node, gvar_map);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);

    if ( node->has_flt_var() ) {
      engine.make_fnode_cnf(node, gvar_map, fvar_map);
    }
    else {
      engine.make_node_cnf(node, fvar_map);
    }

    engine.make_dchain_cnf(node, gvar_map, fvar_map, dvar_map);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障の検出条件
  //////////////////////////////////////////////////////////////////////
  ymuint npo = node_set.output_list().size();
  engine.tmp_lits_begin(npo);
  for (ymuint i = 0; i < npo; ++ i) {
    const TpgNode* node = node_set.output_list()[i];
    Literal dlit(dvar_map(node), false);
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

    const TpgNode* fnode = flist[fid]->node();

    // 故障ノードの TFO 以外の dlit を0にする．
    mTmpNodeList.clear();
    mTmpNodeList.reserve(node_set.tfo_tfi_size());
    mMarkArray[fnode->id()] = true;
    mTmpNodeList.push_back(fnode);
    for (ymuint rpos = 0; rpos < mTmpNodeList.size(); ++ rpos) {
      const TpgNode* node = mTmpNodeList[rpos];
      ymuint nfo = node->active_fanout_num();
      for (ymuint i = 0; i < nfo; ++ i) {
	const TpgNode* fonode = node->active_fanout(i);
	if ( !mMarkArray[fonode->id()] ) {
	  mMarkArray[fonode->id()] = true;
	  mTmpNodeList.push_back(fonode);
	}
      }
    }
    for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
      const TpgNode* node = node_set.tfo_tfi_node(i);
      if ( node_set.tfo_mark(node) && !mMarkArray[node->id()] ) {
	Literal dlit(dvar_map(node), true);
	engine.assumption_add(dlit);
      }
    }
    for (vector<const TpgNode*>::iterator p = mTmpNodeList.begin();
	 p != mTmpNodeList.end(); ++ p) {
      const TpgNode* node = *p;
      mMarkArray[node->id()] = false;
    }
    mTmpNodeList.clear();

    // dominator ノードの dvar は1でなければならない．
    for (const TpgNode* node = f->node(); node != NULL; node = node->imm_dom()) {
      Literal dlit(dvar_map(node), false);
      engine.assumption_add(dlit);
    }

    solve(engine, f, node_set, gvar_map, fvar_map);
  }

  for (vector<const TpgNode*>::const_iterator p = fnode_list.begin();
       p != fnode_list.end(); ++ p) {
    const TpgNode* node = *p;
    //node->clear_oifvar();
  }
}

END_NAMESPACE_YM_SATPG
