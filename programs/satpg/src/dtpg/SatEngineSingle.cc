
/// @file SatEngineSingle.cc
/// @brief SatEngineSingle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "SatEngineSingle.h"

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

// @brief コンストラクタ
SatEngineSingle::SatEngineSingle()
{
  mTgGrasp = true;
  mUseDominator = true;
}

// @brief デストラクタ
SatEngineSingle::~SatEngineSingle()
{
}

// @brief オプションを設定する．
void
SatEngineSingle::set_option(const string& option_str)
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
      mUseDominator = true;
    }
    else if ( option == "NODOM" ) {
      mUseDominator = false;
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
SatEngineSingle::run(TpgFault* fault,
		     TpgNode* fnode,
		     int fval,
		     ymuint max_id,
		     BackTracer& bt,
		     DetectOp& dop,
		     UntestOp& uop)
{
  cnf_begin();

  SatSolver solver(sat_type(), sat_option(), sat_outp());

  bt.set_max_id(max_id);

  mark_region(solver, vector<TpgNode*>(1, fnode), max_id);


  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < tfo_tfi_size(); ++ i) {
    TpgNode* node = tfo_tfi_node(i);
    make_node_cnf(solver, node, GvarLitMap(node));
  }

  //////////////////////////////////////////////////////////////////////
  // 故障回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < tfo_size(); ++ i) {
    TpgNode* node = tfo_tfi_node(i);

    Literal glit(node->gvar(), false);
    Literal flit(node->fvar(), false);
    Literal dlit(node->dvar(), false);

    if ( mTgGrasp ) {
      // XOR(glit, flit, dlit) を追加する．
      // 要するに正常回路と故障回路で異なっているとき dlit が 1 となる．
      solver.add_clause(~glit, ~flit, ~dlit);
      solver.add_clause(~glit,  flit,  dlit);
      solver.add_clause( glit, ~flit,  dlit);
      solver.add_clause( glit,  flit, ~dlit);

      if ( node != fnode ) {
	// 故障回路のゲートの入出力関係を表すCNFを作る．
	make_node_cnf(solver, node, FvarLitMap(node));

	// 出力の dlit が1になる条件を作る．
	// - 入力の dlit のいずれかが 1
	ymuint ni = node->fanin_num();
	tmp_lits_begin(ni + 1);
	tmp_lits_add(~dlit);
	for (ymuint j = 0; j < ni; ++ j) {
	  TpgNode* inode = node->fanin(j);
	  if ( inode->has_fvar() ) {
	    tmp_lits_add(Literal(inode->dvar(), false));
	  }
	}
	tmp_lits_end(solver);
      }
    }
    else {
      // dlit -> XOR(glit, flit) を追加する．
      // 要するに dlit が 1 の時，正常回路と故障回路で異なっていなければならない．
      solver.add_clause(~glit, ~flit, ~dlit);
      solver.add_clause( glit,  flit, ~dlit);

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

      if ( node != fnode ) {
	// 故障回路のゲートの入出力関係を表すCNFを作る．
	make_node_cnf(solver, node, FvarLitMap(node));
      }
    }
  }

  if ( mTgGrasp ) {
    //////////////////////////////////////////////////////////////////////
    // 故障の検出条件
    //////////////////////////////////////////////////////////////////////
    tmp_lits_begin(output_list().size());
    for (vector<TpgNode*>::const_iterator p = output_list().begin();
	 p != output_list().end(); ++ p) {
      TpgNode* node = *p;
      Literal dlit(node->dvar(), false);
      tmp_lits_add(dlit);
    }
    tmp_lits_end(solver);
  }

  if ( mUseDominator ) {
    // dominator ノードの dvar は1でなければならない．
    for (TpgNode* node = fnode; node != NULL; node = node->imm_dom()) {
      Literal dlit(node->dvar(), false);
      solver.add_clause(dlit);
    }
  }
  else {
    solver.add_clause(Literal(fnode->dvar(), false));
  }

  // fnode の dlit を1と仮定しているので
  // ここでは fvar か gvar のどちらかを仮定すればよい．
  bool inv = (fval == 0);
  solver.add_clause(Literal(fnode->fvar(), inv));

  cnf_end();

  // 故障に対するテスト生成を行なう．
  tmp_lits_begin();

  solve(solver, fault, bt, dop, uop);

  clear_node_mark();
}

END_NAMESPACE_YM_SATPG
