
/// @file SatEngineSingleBase.cc
/// @brief SatEngineSingleBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "SatEngineSingleBase.h"

#include "TpgNode.h"
#include "TpgFault.h"
#include "YmLogic/SatSolver.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief コンストラクタ
SatEngineSingleBase::SatEngineSingleBase(const string& sat_type,
					 const string& sat_option,
					 ostream* sat_outp,
					 const TpgNetwork& network,
					 BackTracer& bt,
					 DetectOp& dop,
					 UntestOp& uop) :
  SatEngine(sat_type, sat_option, sat_outp, network, bt, dop, uop)
{
}

// @brief デストラクタ
SatEngineSingleBase::~SatEngineSingleBase()
{
}

// @brief テスト生成を行なう．
// @param[in] flist 対象の故障リスト
void
SatEngineSingleBase::run_multi(const vector<TpgFault*>& flist)
{
  for (vector<TpgFault*>::const_iterator p = flist.begin();
       p != flist.end(); ++ p) {
    TpgFault* f = *p;
    run_single(f);
  }
}

void
SatEngineSingleBase::make_dchain_cnf(SatSolver& solver,
				     TpgNode* node,
				     TpgFault* fault)
{
  Literal glit(node->gvar(), false);
  Literal flit(node->fvar(), false);
  Literal dlit(node->dvar(), false);

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

    for (TpgNode* idom = node->imm_dom();
	 idom != NULL; idom = idom->imm_dom() ) {
      Literal idlit(idom->dvar(), false);
      solver.add_clause(~dlit, idlit);
    }
  }
}

END_NAMESPACE_YM_SATPG
