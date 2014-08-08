
/// @file SatEngineSingle_tv.cc
/// @brief SatEngineSingle_tv の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "SatEngineSingle_tv.h"

#include "DtpgStats.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "BackTracer.h"
#include "YmLogic/SatSolver.h"
#include "YmLogic/SatStats.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief コンストラクタ
SatEngineSingle_tv::SatEngineSingle_tv()
{
  mTgGrasp = true;
  mExtTgGrasp = true;
  mUseDominator = true;
}

// @brief デストラクタ
SatEngineSingle_tv::~SatEngineSingle_tv()
{
}

// @brief オプションを設定する．
void
SatEngineSingle_tv::set_option(const string& option_str)
{
  for (string::size_type next = 0; ; ++ next) {
    string::size_type pos = option_str.find(':', next);
    if ( pos == next ) {
      continue;
    }
    string option = option_str.substr(next, pos - next);
    if ( option == "TG-GRASP" ) {
      mTgGrasp = true;
      mExtTgGrasp = false;
    }
    else if ( option == "EXT-TG-GRASP" ) {
      mTgGrasp = true;
      mExtTgGrasp = true;
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

BEGIN_NONAMESPACE

void
make_dlit_buff(SatSolver& solver,
	       TpgNode* node)
{
  TpgNode* inode = node->fanin(0);
  Literal ilit(inode->dvar(), false);
  Literal olit(node->dvar(), false);
  solver.add_clause(~ilit,  olit);
  solver.add_clause( ilit, ~olit);
}

void
make_dlit_and2(SatSolver& solver,
	       Literal g0,
	       Literal d0,
	       Literal g1,
	       Literal d1,
	       Literal olit)
{
  solver.add_clause( ~g0,  d0,    , ~d1,  olit);
  solver.add_clause(      ~d0, ~g1,  d1,  olit);
  solver.add_clause( ~g0, ~d0, ~g1, ~d1,  olit);
  solver.add_clause(  g0, ~d0,  g1, ~d1,  olit);
  solver.add_clause(       d0,       d1, ~olit);
  solver.add_clause(  g0,  d0,           ~olit);
  solver.add_clause(            g1,  d1, ~olit);
  solver.add_clause(  g0, ~d0, ~g1, ~d1, ~olit);
  solver.add_clause( ~g0, ~d0,  g1, ~d1, ~olit);
}

void
make_dlit_and2(SatSolver& solver,
	       Literal d0,
	       Literal g1,
	       Literal olit)
{
  solver.add_clause(~d0, ~g1,  olit);
  solver.add_clause( d0,      ~olit);
  solver.add_clause(      g1, ~olit);
}

bool
make_dlit_and2(SatSolver& solver,
	       TpgNode* inode0,
	       TpgNode* inode1,
	       Literal olit)
{
  if ( inode0->has_fvar() ) {
    if ( inode1->has_fvar() ) {
      Literal g0(inode0->gvar(), false);
      Literal d0(inode0->dvar(), false);
      Literal g1(inode1->gvar(), false);
      Literal d1(inode1->dvar(), false);
      make_dlit_and2(solver, g0, d1, g1, d1, olit);
      return true;
    }
    else {
      Literal d0(inode0->dvar(), false);
      Literal g1(inode1->gvar(), false);
      make_dlit_and2(solver, d1, g1 olit);
      return true;
    }
  }
  else {
    if ( inode1->has_fvar() ) {
      Literal d0(inode1->dvar(), false);
      Literal g1(inode0->gvar(), false);
      make_dlit_and2(solver, d1, g1 olit);
      return true;
    }
    else {
      return false;
    }
  }
}

make_dlit_and(SatSolver& solver,
	      TpgNode* node)
{
  // AND の出力に故障差が伝搬する条件
  // - 故障差が伝搬していない入力の値がすべて 1
  // - 故障差が伝搬している入力の値がどちらか1つ
  //
  // 故障差が伝搬しない条件
  // - 故障差が伝搬していない入力の値で 0 のものがある．
  // - 故障差が伝搬している入力の値が両方ある．
  Literal olit(node->dvar(), false);
  ymuint ni = node->fanin_num();

  // 要素数が2の時の dlit = 1 の条件
  //  -  g0       & ~d0 &  d1
  //  -        g1 &  d0 & ~d1
  //  -  g0 &  g1 &  d0 &  d1
  //  - ~g0 & ~g1 &  d0 &  d1
  //
  // 要素数が2の時の dlit = 0 の条件
  //  -           & ~d0 & ~d1
  //  - ~g0       & ~d0
  //  -       ~g1       & ~d1
  //  - ~g0 &  g1 &  d0 &  d1
  //  -  g0 & ~g1 &  d0 &  d1

  // 3入力以上の場合は最初の2入力分の gval と dval
  // を計算し，それを1つの入力だと思って繰り返し計算する．
  if ( ni == 2 ) {
    TpgNode* inode0 = node->fanin(0);
    TpgNode* inode1 = node->fanin(1);
    if (
    make_dlit_and2(solver, inode0, inode1);
  }
  else {
    TpgNode* inode0 = node->fanin(0);
    TpgNode* inode1 = node->fanin(1);
    VarId gvar = solver.new_var();
    VarId dvar = solver.new_var();
    bool stat = make_dlit_and2(solver, inode0, inode1, dvar);
    Literal g0(inode0->gvar(), false);
    Literal g1(inode1->gvar(), false);
    Literal glit(gvar, false);
    Literal dlit(dvar, false);
    solver.add_clause( g0, ~glit);
    solver.add_clause( g1, ~glit);
    solver.add_clause(~g0, ~g1, glit);
    for (ymuint i = 2; i < ni; ++ i) {
      g0 = glit;
      TpgNode* inode = node->fanin(i);
      Literal g1(inode->gvar(), false);

      if ( stat ) {
	d0 = dlit;
	if ( i < ni - 1 ) {
	  dvar = solver.new_var();
	  dlit = Literal(dvar, false);
	}
	else {
	  dlit = olit;
	}
	if ( inode->has_fvar() ) {
	  Literal d1(inode->dvar(), false);
	  make_dlit_and2(solver, g0, d0, g1, d1, dlit);
	}
	else {
	  make_dlit_and2(solver, d0, g1, dlit);
	}
      }
      else {
	if ( i < ni - 1 ) {
	  dvar = solver.new_var();
	  dlit = Literal(dvar, false);
	}
	else {
	  dlit = olit;
	}
	if ( inode->has_fvar() ) {
	  Literal d1(inode->dvar(), false);
	  make_dlit_and2(solver, d1, g0, dlit);
	  stat = true;
	}
      }

      if ( i < ni - 1 ) {
	gvar = solver.new_var();
	glit = Literal(gvar, false);
	solver.add_clause( g0, ~glit);
	solver.add_clause( g1, ~glit);
	solver.add_clause(~g0, ~g1, glit);
      }
    }
  }
}

END_NONAMESPACE


// @brief テストパタン生成を行なう．
// @param[in] flist 故障リスト
// @param[in] max_id ノード番号の最大値 + 1
void
SatEngineSingle_tv::run(TpgFault* fault,
			ymuint max_id,
			BackTracer& bt,
			DetectOp& dop,
			UntestOp& uop)
{
  TpgNode* fnode = fault->node();
  int fval = fault->val();

  SatSolver solver(sat_type(), sat_option(), sat_outp());

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
    Literal dlit(node->dvar(), false);

    // 故障回路のゲートの入出力関係を表すCNFを作る．
    if ( node == fnode ) {
      if ( fault->is_output_fault() ) {
	solver.add_clause(dlit);
	if ( fault->val() == 0 ) {
	  solver.add_clause(glit);
	}
	else {
	  solver.add_clause(~glit);
	}
      }
      else {
	ymuint pos = fault->pos();
	ymuint ni = node->fanin_num();
	switch( node->gate_type() ) {
	case kTgGateAnd:
	case kTgGateNand:
	  for (ymuint i = 0; i < ni; ++ i) {
	    TpgNode* inode = node->fanin(i);
	    Literal ilit(inode->gvar(), false);
	    if ( i == pos ) {
	      solver.add_clause(~ilit);
	    }
	    else {
	      solver.add_clause(ilit);
	    }
	  }
	  break;

	case kTgGateOr:
	case kTgGateNor:
	  for (ymuint i = 0; i < ni; ++ i) {
	    TpgNode* inode = node->fanin(i);
	    Literal ilit(inode->gvar(), false);
	    if ( i == pos ) {
	      solver.add_clause(ilit);
	    }
	    else {
	      solver.add_clause(~ilit);
	    }
	  }
	  break;

	case kTgGateXor:
	case kTgGateXnor:
	  {
	    TpgNode* inode = node->fanin(pos);
	    Literal ilit(inode->gvar(), false);
	    if ( fault->val() == 0 ) {
	      solver.add_clause(ilit);
	    }
	    else {
	      solver.add_clause(~ilit);
	    }
	  }
	  break;

	default:
	  break;
	}
      }
    }
    else {
      if ( node->is_output() ) {
	TpgNode* inode = node->fanin(0);
	Literal ilit(inode->dvar(), false);
	Literal olit(node->dvar(), false);
	solver.add_clause(~ilit,  olit);
	solver.add_clause( ilit, ~olit);
      }
      if ( node->is_logic() ) {
	Literal olit(node->dvar(), false);

	switch ( node->gate_type() ) {
	case kTgGateBuff:
	case kTgGateNot:
	  make_dlit_buff(solver, node);
	  break;

	case kTgGateAnd:
	case kTgGateNand:
	  make_dlit_and(solver, node);
	  break;

	case kTgGateOr:
	case kTgGateNor:
	  make_dlit_or(solver, node);
	  break;

	case kTgGateXor:
	case kTgGateXnor:
	  make_dlit_xor(solver, node);
	  break;

	default:
	  break;
	}
      }
    }
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

  cnf_end();

  // 故障に対するテスト生成を行なう．
  tmp_lits_begin();

  solve(solver, fault, bt, dop, uop);

  clear_node_mark();
}

END_NAMESPACE_YM_SATPG
