
/// @file SatEngineSingle2.cc
/// @brief SatEngineSingle2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "SatEngineSingle2.h"

#include "DetectOp.h"
#include "UntestOp.h"
#include "DtpgStats.h"
#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "BackTracer.h"
#include "LitMap.h"
#include "logic/SatSolver.h"
#include "logic/SatStats.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief コンストラクタ
SatEngineSingle2::SatEngineSingle2()
{
  mUopDummy = new_UopDummy();
  mTgGrasp = true;
  mUseDominator = true;
  mUseLocalDominator = true;
}

// @brief デストラクタ
SatEngineSingle2::~SatEngineSingle2()
{
  delete mUopDummy;
}

// @brief オプションを設定する．
void
SatEngineSingle2::set_option(const string& option_str)
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
      mUseLocalDominator = false;
    }
    else if ( option == "NODOM" ) {
      mUseDominator = false;
    }
    else if ( option == "LOCALDOM" ) {
      mUseLocalDominator = true;
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
SatEngineSingle2::run(TpgNetwork& network,
		      TpgFault* fault,
		      TpgNode* fnode,
		      int fval,
		      ymuint max_id,
		      BackTracer& bt,
		      DetectOp& dop,
		      UntestOp& uop)
{
  SatSolver solver(sat_type(), sat_option(), sat_outp());

  bt.set_max_id(max_id);

  mark_region(solver, vector<TpgNode*>(1, fnode), max_id);

  const vector<TpgNode*>& olist = output_list();
  ymuint no = olist.size();

  USTime time(0, 0, 0);
  Bool3 last_ans;

  vector<ymuint> mark(max_id, 0);
  for (ymuint opos = 0; opos < no; ++ opos) {
    TpgNode* onode = olist[opos];
    ymuint oid = onode->output_id2();

    cnf_begin();

    //////////////////////////////////////////////////////////////////////
    // 正常回路の CNF を生成
    //////////////////////////////////////////////////////////////////////
    for (ymuint i = 0; i < tfo_tfi_size(); ++ i) {
      TpgNode* node = tfo_tfi_node(i);
      if ( node->is_in_TFI_of(oid) && mark[node->id()] == 0 ) {
	mark[node->id()] = oid + 1;

	make_node_cnf(solver, node, GvarLitMap(node));
      }
    }

    //////////////////////////////////////////////////////////////////////
    // 故障回路の CNF を生成
    //////////////////////////////////////////////////////////////////////
    for (ymuint i = 0; i < tfo_size(); ++ i) {
      TpgNode* node = tfo_tfi_node(i);
      if ( mark[node->id()] != (oid + 1) ) {
	continue;
      }

      Literal glit(node->gvar(), false);
      Literal flit(node->fvar(), false);
      Literal dlit(node->dvar(), false);

      // XOR(glit, flit, dlit) を追加する．
      // 要するに正常回路と故障回路で異なっているとき dlit が 1 となる．
      solver.add_clause(~glit, ~flit, ~dlit);
      solver.add_clause(~glit,  flit,  dlit);
      solver.add_clause( glit, ~flit,  dlit);
      solver.add_clause( glit,  flit, ~dlit);

      if ( node != fnode ) {
	// 故障回路のゲートの入出力関係を表すCNFを作る．
	make_node_cnf(solver, node, FvarLitMap(node));

	make_dlit_cnf(solver, node);
      }
    }

    cnf_end();

    //////////////////////////////////////////////////////////////////////
    // 故障の検出条件
    //////////////////////////////////////////////////////////////////////
    tmp_lits_begin();

    tmp_lits_add(Literal(onode->dvar(), false));

    if ( mUseLocalDominator ) {
      // dominator ノードの dvar は1でなければならない．
      for (TpgNode* node = fnode; node != NULL; node = node->imm_dom(oid)) {
	tmp_lits_add(Literal(node->dvar(), false));
      }
    }
    else {
      // dominator ノードの dvar は1でなければならない．
      for (TpgNode* node = fnode; node != NULL; node = node->imm_dom()) {
	tmp_lits_add(Literal(node->dvar(), false));
      }
    }

    // fnode の dlit を1と仮定しているので
    // ここでは fvar か gvar のどちらかを仮定すればよい．
    bool inv = (fval == 0);
    solver.add_clause(Literal(fnode->fvar(), inv));

    last_ans = _solve(solver, time);

    if ( last_ans != kB3False ) {
      break;
    }
  }

  SatStats sat_stats;
  solver.get_stats(sat_stats);

  if ( last_ans == kB3True ) {

    // バックトレースを行う．
    TestVector* tv = bt(fault->node(), mModel, mInputList, mOutputList);

    // パタンの登録などを行う．
    dop(fault, tv);

    stats_detect(sat_stats, time);
  }
  else if ( last_ans == kB3False ) {
    uop(fault);
    stats_undetect(sat_stats, time);
  }
  else {
    stats_abort(sat_stats, time);
  }

  clear_node_mark();
}

END_NAMESPACE_YM_SATPG
