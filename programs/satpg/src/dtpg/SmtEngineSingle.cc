
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
#include "../ymsat/YmSat.h"
#include "logic/SatStats.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief コンストラクタ
SmtEngineSingle::SmtEngineSingle()
{
}

// @brief デストラクタ
SmtEngineSingle::~SmtEngineSingle()
{
}

// @brief オプションを設定する．
void
SmtEngineSingle::set_option(const string& option_str)
{
  for (string::size_type next = 0; ; ++ next) {
    string::size_type pos = option_str.find(':', next);
    if ( pos == next ) {
      continue;
    }
    string option = option_str.substr(next, pos - next);
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
SmtEngineSingle::run(TpgFault* fault,
		     ymuint max_id,
		     BackTracer& bt,
		     DetectOp& dop,
		     UntestOp& uop)
{
  TpgNode* fnode = fault->node();
  int fval = fault->val();

  YmSat solver(sat_option());

  bt.set_max_id(max_id);

  mark_region(solver, vector<TpgNode*>(1, fnode), max_id);

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
      make_fault_cnf(solver, fault);
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
  }

  cnf_end();

  //////////////////////////////////////////////////////////////////////
  // 故障の検出条件
  //////////////////////////////////////////////////////////////////////
  solver.add_pgraph(fnode, output_list());

  // 故障に対するテスト生成を行なう．
  tmp_lits_begin();

  solve(solver, fault, bt, dop, uop);

  clear_node_mark();
}

END_NAMESPACE_YM_SATPG
