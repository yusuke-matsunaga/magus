
/// @file SmtEngineSingle.cc
/// @brief SmtEngineSingle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "SmtEngineSingle.h"

#include "DtpgStats.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "BackTracer.h"
#include "../../graphsat/GraphSat.h"
#include "logic/SatStats.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief SmtSingle エンジンを作る．
// @param[in] sat_type SATソルバの種類を表す文字列
// @param[in] sat_option SATソルバに渡すオプション文字列
// @param[in] sat_outp SATソルバ用の出力ストリーム
// @param[in] max_id ノード番号の最大値 + 1
// @param[in] bt バックトレーサー
// @param[in] dop パタンが求められた時に実行されるファンクタ
// @param[in] uop 検出不能と判定された時に実行されるファンクタ
DtpgEngine*
new_SmtEngineSingle(const string& sat_type,
		    const string& sat_option,
		    ostream* sat_outp,
		    ymuint max_id,
		    BackTracer& bt,
		    DetectOp& dop,
		    UntestOp& uop)
{
  return new SmtEngineSingle(sat_type, sat_option, sat_outp, max_id, bt, dop, uop);
}

// @brief コンストラクタ
// @param[in] sat_type SATソルバの種類を表す文字列
// @param[in] sat_option SATソルバに渡すオプション文字列
// @param[in] sat_outp SATソルバ用の出力ストリーム
// @param[in] max_id ノード番号の最大値 + 1
// @param[in] bt バックトレーサー
// @param[in] dop パタンが求められた時に実行されるファンクタ
// @param[in] uop 検出不能と判定された時に実行されるファンクタ
SmtEngineSingle::SmtEngineSingle(const string& sat_type,
				 const string& sat_option,
				 ostream* sat_outp,
				 ymuint max_id,
				 BackTracer& bt,
				 DetectOp& dop,
				 UntestOp& uop) :
  SmtEngine(sat_type, sat_option, sat_outp, max_id, bt, dop, uop)
{
}

// @brief デストラクタ
SmtEngineSingle::~SmtEngineSingle()
{
}

// @brief テストパタン生成を行なう．
// @param[in] f_tgt 対象の故障
void
SmtEngineSingle::run(TpgFault* f_tgt)
{
  TpgNode* fnode = f_tgt->node();
  int fval = f_tgt->val();

  Solver solver(sat_option());

  mark_region(solver, vector<TpgNode*>(1, fnode), mMaxNodeId);

  cnf_begin();

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

    Literal glit(node->gvar(), false);
    Literal flit(node->fvar(), false);
    Literal dlit(node->dvar(), false);

    // 故障回路のゲートの入出力関係を表すCNFを作る．
    if ( node == fnode ) {
      make_fault_cnf(solver, f_tgt);
    }
    else {
      make_fnode_cnf(solver, node);
    }

    // 要するに正常回路と故障回路で異なっているとき dlit が 1 となる．

    // dlit -> XOR(glit, flit) を追加する．
    solver.add_clause(~glit, ~flit, ~dlit);
    solver.add_clause( glit,  flit, ~dlit);
    // XOR(glit, flit) -> dlit を追加する．
    solver.add_clause(~glit,  flit,  dlit);
    solver.add_clause( glit, ~flit,  dlit);

#if 0
#if 0
    if ( node != fnode ) {
      make_dlit_cnf(solver, node);
    }
#else
    if ( !node->is_output() ) {
      // dlit が 1 の時，ファンアウトの dlit が最低1つは 1 でなければならない．
      ymuint nfo = node->active_fanout_num();
      tmp_lits_begin(nfo + 1);
      tmp_lits_add(~dlit);
      for (ymuint j = 0; j < nfo; ++ j) {
	  TpgNode* onode = node->active_fanout(j);
	  tmp_lits_add(Literal(onode->dvar(), false));
      }
      tmp_lits_end(solver);
    }
#endif
#endif
  }

  //////////////////////////////////////////////////////////////////////
  // 故障の検出条件
  //////////////////////////////////////////////////////////////////////
  solver.set_pgraph(fnode, output_list(), mMaxNodeId);
  if ( 0 ) {
    ymuint npo = output_list().size();
    tmp_lits_begin(npo);
    for (ymuint i = 0; i < npo; ++ i) {
      TpgNode* node = output_list()[i];
      Literal dlit(node->dvar(), false);
      tmp_lits_add(dlit);
    }
    tmp_lits_end(solver);
  }

  // dominator ノードの dvar は1でなければならない．
  for (TpgNode* node = fnode; node != NULL; node = node->imm_dom()) {
    Literal dlit(node->dvar(), false);
    solver.add_clause(dlit);
  }

  cnf_end();

  // 故障に対するテスト生成を行なう．
  tmp_lits_begin();

  solve(solver, f_tgt, mBackTracer, mDetectOp, mUntestOp);

  clear_node_mark();
}

// @brief テスト生成を行なう．
// @param[in] flist 対象の故障リスト
void
SmtEngineSingle::run(const vector<TpgFault*>& flist)
{
  for (vector<TpgFault*>::const_iterator p = flist.begin();
       p != flist.end(); ++ p) {
    TpgFault* f = *p;
    run(f);
  }
}

END_NAMESPACE_YM_SATPG
