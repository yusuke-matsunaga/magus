
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
SatEngineSingle2::run(TpgFault* fault,
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

  // fnode の dlit を1と仮定しているので
  // ここでは fvar か gvar のどちらかを仮定すればよい．
  bool inv = (fval == 0);
  solver.add_clause(Literal(fnode->fvar(), inv));

  vector<bool> mark(max_id, false);
  for (ymuint opos = 0; opos < no; ++ opos) {
    vector<bool> mark2(max_id, false);

    cnf_begin();

    //////////////////////////////////////////////////////////////////////
    // 正常回路の CNF を生成
    //////////////////////////////////////////////////////////////////////
    TpgNode* node = olist[opos];
    vector<TpgNode*> queue;
    queue.reserve(tfo_tfi_size());
    queue.push_back(node);
    for (ymuint rpos = 0; rpos < queue.size(); ++ rpos) {
      TpgNode* node = queue[rpos];
      if ( !mark[node->id()] ) {
	mark[node->id()] = true;
	mark2[node->id()] = true;

	make_node_cnf(solver, node, GvarLitMap(node));

	ymuint ni = node->fanin_num();
	for (ymuint i = 0; i < ni; ++ i) {
	  TpgNode* inode = node->fanin(i);
	  queue.push_back(inode);
	}
      }
    }

    //////////////////////////////////////////////////////////////////////
    // 故障回路の CNF を生成
    //////////////////////////////////////////////////////////////////////
    for (ymuint i = 0; i < tfo_size(); ++ i) {
      TpgNode* node = tfo_tfi_node(i);
      if ( !mark2[node->id()] ) {
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

    cnf_end();

    Literal dlit(node->dvar(), false);

    // 故障に対するテスト生成を行なう．
    tmp_lits_begin();

    tmp_lits_add(dlit);

    if ( opos < no - 1 ) {
      solve(solver, fault, bt, dop, *mUopDummy);
      if ( fault->status() == kFsDetected ) {
	break;
      }
    }
    else {
      solve(solver, fault, bt, dop, uop);
    }
  }
#if 0
  // dominator ノードの dvar は1でなければならない．
  for (TpgNode* node = fnode; node != NULL; node = node->imm_dom()) {
    Literal dlit(node->dvar(), false);
    solver.add_clause(dlit);
  }
#endif

  clear_node_mark();
}

END_NAMESPACE_YM_SATPG
