
/// @file SatEngineSingle.cc
/// @brief SatEngineSingle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "SatEngineSingle.h"

#include "DtpgStats.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "YmLogic/SatSolver.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief Single エンジンを作る．
// @param[in] sat_type SATソルバの種類を表す文字列
// @param[in] sat_option SATソルバに渡すオプション文字列
// @param[in] sat_outp SATソルバ用の出力ストリーム
// @param[in] max_id ノード番号の最大値 + 1
// @param[in] bt バックトレーサー
// @param[in] dop パタンが求められた時に実行されるファンクタ
// @param[in] uop 検出不能と判定された時に実行されるファンクタ
DtpgEngine*
new_SatEngineSingle(const string& sat_type,
		    const string& sat_option,
		    ostream* sat_outp,
		    ymuint max_id,
		    BackTracer& bt,
		    DetectOp& dop,
		    UntestOp& uop)
{
  return new SatEngineSingle(sat_type, sat_option, sat_outp, max_id, bt, dop, uop);
}

// @brief コンストラクタ
SatEngineSingle::SatEngineSingle(const string& sat_type,
				 const string& sat_option,
				 ostream* sat_outp,
				 ymuint max_id,
				 BackTracer& bt,
				 DetectOp& dop,
				 UntestOp& uop) :
  SatEngine(sat_type, sat_option, sat_outp, max_id, bt, dop, uop)
{
}

// @brief デストラクタ
SatEngineSingle::~SatEngineSingle()
{
}

// @brief テストパタン生成を行なう．
// @param[in] flist 故障リスト
void
SatEngineSingle::run(TpgFault* fault)
{
  TpgNode* fnode = fault->node();
  int fval = fault->val();

  SatSolver solver(sat_type(), sat_option(), sat_outp());

  mark_region(solver, vector<TpgNode*>(1, fnode));

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

    // 故障回路のゲートの入出力関係を表すCNFを作る．
    if ( node == fnode ) {
      make_fault_cnf(solver, fault);
    }
    else {
      make_fnode_cnf(solver, node);
    }

    // D-Chain 制約を作る．
    make_dchain_cnf(solver, node, fault);
  }


  //////////////////////////////////////////////////////////////////////
  // 故障の検出条件
  //////////////////////////////////////////////////////////////////////
  ymuint npo = output_list().size();
  tmp_lits_begin(npo);
  for (ymuint i = 0; i < npo; ++ i) {
    TpgNode* node = output_list()[i];
    Literal dlit(node->dvar(), false);
    tmp_lits_add(dlit);
  }
  tmp_lits_end(solver);

  if ( use_dominator() ) {
    // dominator ノードの dvar は1でなければならない．
    for (TpgNode* node = fnode; node != NULL; node = node->imm_dom()) {
      Literal dlit(node->dvar(), false);
      solver.add_clause(dlit);
    }
  }
  else {
    solver.add_clause(Literal(fnode->dvar(), false));
  }

  cnf_end();

  // 故障に対するテスト生成を行なう．
  tmp_lits_begin();

  solve(solver, fault);

  clear_node_mark();
}

// @brief テスト生成を行なう．
// @param[in] flist 対象の故障リスト
void
SatEngineSingle::run(const vector<TpgFault*>& flist)
{
  for (vector<TpgFault*>::const_iterator p = flist.begin();
       p != flist.end(); ++ p) {
    TpgFault* f = *p;
    run(f);
  }
}

END_NAMESPACE_YM_SATPG
