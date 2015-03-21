
/// @file SatEngineMultiBase.cc
/// @brief SatEngineMultiBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "SatEngineMultiBase.h"

#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "YmLogic/SatSolver.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief コンストラクタ
SatEngineMultiBase::SatEngineMultiBase(const string& sat_type,
				       const string& sat_option,
				       ostream* sat_outp,
				       const TpgNetwork& network,
				       BackTracer& bt,
				       DetectOp& dop,
				       UntestOp& uop) :
  SatEngine(sat_type, sat_option, sat_outp, network, bt, dop, uop),
  mDone(network.max_node_id(), false)
{
}

// @brief デストラクタ
SatEngineMultiBase::~SatEngineMultiBase()
{
}

// @brief テスト生成を行なう．
// @param[in] f_tgt 対象の故障
void
SatEngineMultiBase::run_single(TpgFault* f_tgt)
{
  run_multi(vector<TpgFault*>(1, f_tgt));
}

void
SatEngineMultiBase::make_dchain_cnf(SatSolver& solver,
				    TpgNode* node)
{
  Literal glit(node->gvar(), false);
  Literal flit(node->fvar(), false);
  Literal dlit(node->dvar(), false);

  // 正常回路と故障回路で異なっているとき dlit が 1 となる．
  solver.add_clause(~glit, ~flit, ~dlit);
  solver.add_clause( glit,  flit, ~dlit);

  // XOR(glit, flit, dlit) を追加する．
  // 要するに正常回路と故障回路で異なっているとき dlit が 1 となる．

  solver.add_clause(~glit,  flit,  dlit);
  solver.add_clause( glit, ~flit,  dlit);

  // 全てのファンインに故障差が伝搬していなければ
  // このゲートの出力にも故障差は伝搬しない．
  // ただし，このゲートの出力に故障がある場合を
  // 考慮しなければならない．
  if ( node->has_flt_var() ) {
    ymuint ni = node->fanin_num();
    tmp_lits_begin(ni * 3 + 3);
    tmp_lits_add(~dlit);
    if ( node->of0var() != kVarIdIllegal ) {
      tmp_lits_add(Literal(node->of0var(), false));
    }
    if ( node->of1var() != kVarIdIllegal ) {
      tmp_lits_add(Literal(node->of1var(), false));
    }
    for (ymuint i = 0; i < ni; ++ i) {
      TpgNode* inode = node->fanin(i);
      if ( inode->has_fvar() ) {
	Literal idlit(inode->dvar(), false);
	tmp_lits_add(idlit);
      }
      if ( node->if0var(i) != kVarIdIllegal ) {
	tmp_lits_add(Literal(node->if0var(i), false));
      }
      if ( node->if1var(i) != kVarIdIllegal ) {
	tmp_lits_add(Literal(node->if1var(i), false));
      }
    }
  }
  else {
    make_dlit_cnf(solver, node);
  }
}

// @brief 故障に関係するノードのリストを作る．
// @param[in] flist 故障のリスト
// @param[out] fnode_list 故障に関係するノードのリスト
void
SatEngineMultiBase::make_fnode_list(const vector<TpgFault*>& flist,
				    vector<TpgNode*>& fnode_list)
{
  ymuint nf = flist.size();
  fnode_list.clear();
  fnode_list.reserve(nf);
  for (ymuint i = 0; i < nf; ++ i) {
    TpgFault* f = flist[i];
    TpgNode* node = f->node();
    if ( !mDone[node->id()] ) {
      mDone[node->id()] = true;
      fnode_list.push_back(node);
    }
  }
}

// @brief fnode の情報をクリアする．
void
SatEngineMultiBase::clear_fnode_list(const vector<TpgNode*>& fnode_list)
{
  ymuint nf = fnode_list.size();
  for (ymuint i = 0; i < nf; ++ i) {
    TpgNode* node = fnode_list[i];
    node->clear_oifvar();
    mDone[node->id()] = false;
  }
}

END_NAMESPACE_YM_SATPG
