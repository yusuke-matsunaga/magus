
/// @file atpg/src/dtpg/DtpgSat.cc
/// @brief DtpgSat の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSat.h"
#include "DtpgNetwork.h"
#include "DtpgNode.h"
#include "DtpgFFR.h"
#include "DtpgFault.h"
#include "TvMgr.h"
#include "TestVector.h"
#include "ym_logic/SatSolver.h"
#include "ym_logic/LogExpr.h"
#include "ym_networks/TgNetwork.h"


BEGIN_NAMESPACE_YM_SATPG_DTPG

// @brief コンストラクタ
DtpgSat::DtpgSat()
{
  mNetwork = NULL;
}

// @brief デストラクタ
DtpgSat::~DtpgSat()
{
  delete mNetwork;
}

// @brief 回路と故障リストを設定する．
// @param[in] tgnetwork 対象のネットワーク
// @param[in] fault_list 故障リスト
void
DtpgSat::set_network(const TgNetwork& tgnetwork,
		     const vector<SaFault*>& fault_list)
{
  delete mNetwork;
  mNetwork = new DtpgNetwork(tgnetwork, fault_list);
}


BEGIN_NONAMESPACE

void
make_cnf_and(SatSolver& solver,
	     VarId i0,
	     VarId i1,
	     VarId o)
{
  Literal l0(i0, kPolNega);
  Literal l1(i1, kPolPosi);
  Literal l2(o, kPolPosi);
  solver.add_clause(l0, ~l2);
  solver.add_clause(l1, ~l2);
  solver.add_clause(~l0, ~l1, l2);
}

void
make_cnf_or(SatSolver& solver,
	    VarId i0,
	    VarId i1,
	    VarId o)
{
  Literal l0(i0, kPolPosi);
  Literal l1(i1, kPolPosi);
  Literal l2(o, kPolPosi);
  solver.add_clause(~l0, l2);
  solver.add_clause(~l1, l2);
  solver.add_clause(l0, l1, ~l2);
}

/// @brief ノードの入出力の関係を表す CNF クローズを生成する．
/// @param[in] solver SAT ソルバ
/// @param[in] type ノードの型
/// @param[in] output 出力リテラル
/// @param[in] inputs 入力リテラル
void
make_cnf_from_type(SatSolver& solver,
		   tTgGateType type,
		   Literal output,
		   const vector<Literal>& inputs)
{
  ymuint ni = inputs.size();
  switch ( type ) {
  case kTgUndef:
    assert_not_reached(__FILE__, __LINE__);
    break;

  case kTgInput:
    break;

  case kTgOutput:
  case kTgBuff:
    solver.add_clause(inputs[0], ~output);
    solver.add_clause(~inputs[0], output);
    break;

  case kTgNot:
    solver.add_clause(inputs[0], output);
    solver.add_clause(~inputs[0], ~output);
    break;

  case kTgAnd:
    switch ( ni ) {
    case 2:
      solver.add_clause(~inputs[0], ~inputs[1], output);
      break;

    case 3:
      solver.add_clause(~inputs[0], ~inputs[1], ~inputs[2], output);
      break;

    case 4:
      solver.add_clause(~inputs[0], ~inputs[1], ~inputs[2], ~inputs[3], output);
      break;

    default:
      {
	vector<Literal> tmp(ni + 1);
	for (ymuint i = 0; i < ni; ++ i) {
	  tmp[i] = ~inputs[i];
	}
	tmp[ni] = output;
	solver.add_clause(tmp);
      }
      break;
    }
    for (ymuint i = 0; i < ni; ++ i) {
      solver.add_clause(inputs[i], ~output);
    }
    break;

  case kTgNand:
    switch ( ni ) {
    case 2:
      solver.add_clause(~inputs[0], ~inputs[1], ~output);
      break;

    case 3:
      solver.add_clause(~inputs[0], ~inputs[1], ~inputs[2], ~output);
      break;

    case 4:
      solver.add_clause(~inputs[0], ~inputs[1], ~inputs[2], ~inputs[3], ~output);
      break;

    default:
      {
	vector<Literal> tmp(ni + 1);
	for (ymuint i = 0; i < ni; ++ i) {
	  tmp[i] = ~inputs[i];
	}
	tmp[ni] = ~output;
	solver.add_clause(tmp);
      }
      break;
    }
    for (ymuint i = 0; i < ni; ++ i) {
      solver.add_clause(inputs[i], output);
    }
    break;

  case kTgOr:
    switch ( ni ) {
    case 2:
      solver.add_clause(inputs[0], inputs[1], ~output);
      break;

    case 3:
      solver.add_clause(inputs[0], inputs[1], inputs[2], ~output);
      break;

    case 4:
      solver.add_clause(inputs[0], inputs[1], inputs[2], inputs[3], ~output);
      break;

    default:
      {
	vector<Literal> tmp(ni + 1);
	for (ymuint i = 0; i < ni; ++ i) {
	  tmp[i] = inputs[i];
	}
	tmp[ni] = ~output;
	solver.add_clause(tmp);
      }
      break;
    }
    for (ymuint i = 0; i < ni; ++ i) {
      solver.add_clause(~inputs[i], output);
    }
    break;

  case kTgNor:
    switch ( ni ) {
    case 2:
      solver.add_clause(inputs[0], inputs[1], output);
      break;

    case 3:
      solver.add_clause(inputs[0], inputs[1], inputs[2], output);
      break;

    case 4:
      solver.add_clause(inputs[0], inputs[1], inputs[2], inputs[3], output);
      break;

    default:
      {
	vector<Literal> tmp(ni + 1);
	for (ymuint i = 0; i < ni; ++ i) {
	  tmp[i] = inputs[i];
	}
	tmp[ni] = output;
	solver.add_clause(tmp);
      }
      break;
    }
    for (ymuint i = 0; i < ni; ++ i) {
      solver.add_clause(~inputs[i], ~output);
    }
    break;

  case kTgXor:
    if ( ni == 2 ) {
      solver.add_clause(~inputs[0],  inputs[1],  output);
      solver.add_clause( inputs[0], ~inputs[1],  output);
      solver.add_clause( inputs[0],  inputs[1], ~output);
      solver.add_clause(~inputs[0], ~inputs[1], ~output);
    }
    else {
      vector<Literal> tmp(ni + 1);
      ymuint nip = (1U << ni);
      for (ymuint p = 0; p < nip; ++ p) {
	ymuint c = 0;
	for (ymuint i = 0; i < ni; ++ i) {
	  if ( p & (1U << i) ) {
	    tmp[i] = inputs[i];
	  }
	  else {
	    tmp[i] = ~inputs[i];
	    ++ c;
	  }
	}
	if ( (c % 2) == 0 ) {
	  tmp[ni] = ~output;
	}
	else {
	  tmp[ni] = output;
	}
	solver.add_clause(tmp);
      }
    }
    break;

  case kTgXnor:
    if ( ni == 2 ) {
      solver.add_clause(~inputs[0],  inputs[1], ~output);
      solver.add_clause( inputs[0], ~inputs[1], ~output);
      solver.add_clause( inputs[0],  inputs[1],  output);
      solver.add_clause(~inputs[0], ~inputs[1],  output);
    }
    else {
      vector<Literal> tmp(ni + 1);
      ymuint nip = (1U << ni);
      for (ymuint p = 0; p < nip; ++ p) {
	ymuint c = 0;
	for (ymuint i = 0; i < ni; ++ i) {
	  if ( p & (1U << i) ) {
	    tmp[i] = inputs[i];
	  }
	  else {
	    tmp[i] = ~inputs[i];
	    ++ c;
	  }
	}
	if ( (c % 2) == 0 ) {
	  tmp[ni] = output;
	}
	else {
	  tmp[ni] = ~output;
	}
	solver.add_clause(tmp);
      }
    }
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
}

/// @brief ノードの入出力の関係を表す CNF クローズを生成する．
/// @param[in] solver SAT ソルバ
/// @param[in] lexp 論理式
/// @param[in] output 出力リテラル
/// @param[in] inputs 入力リテラル
void
make_cnf_from_lexp(SatSolver& solver,
		   const LogExpr& lexp,
		   Literal output,
		   const vector<Literal>& inputs)
{
  if ( lexp.is_constant() || lexp.is_literal() ) {
    assert_not_reached(__FILE__, __LINE__);
    return;
  }

  ymuint nc = lexp.child_num();
  vector<Literal> local_inputs(nc);
  for (ymuint i = 0; i < nc; ++ i) {
    LogExpr lexp1 = lexp.child(i);
    if ( lexp1.is_posiliteral() ) {
      local_inputs[i] = inputs[lexp1.varid().val()];
    }
    else if ( lexp1.is_negaliteral() ) {
      local_inputs[i] = ~inputs[lexp1.varid().val()];
    }
    else {
      VarId new_varid = solver.new_var();
      local_inputs[i] = Literal(new_varid, kPolPosi);
      make_cnf_from_lexp(solver, lexp1, local_inputs[i], inputs);
    }
  }
  vector<Literal> tmp(nc + 1);
  if ( lexp.is_and() ) {
    for (ymuint i = 0; i < nc; ++ i) {
      tmp[i] = ~local_inputs[i];
    }
    tmp[nc] = output;
    solver.add_clause(tmp);
    for (ymuint i = 0; i < nc; ++ i) {
      solver.add_clause(local_inputs[i], ~output);
    }
  }
  else if ( lexp.is_or() ) {
    for (ymuint i = 0; i < nc; ++ i) {
      tmp[i] = local_inputs[i];
    }
    tmp[nc] = ~output;
    solver.add_clause(tmp);
    for (ymuint i = 0; i < nc; ++ i) {
      solver.add_clause(~local_inputs[i], output);
    }
  }
  else if ( lexp.is_xor() ) {
    ymuint np = (1U << nc);
    for (ymuint p = 0; p < np; ++ p) {
      ymuint c = 0;
      for (ymuint i = 0; i < nc; ++ i) {
	if ( p & (1U << i) ) {
	  tmp[i] = local_inputs[i];
	}
	else {
	  tmp[i] = ~local_inputs[i];
	  ++ c;
	}
      }
      if ( (c % 2) == 0 ) {
	tmp[nc] = ~output;
      }
      else {
	tmp[nc] = output;
      }
      solver.add_clause(tmp);
    }
  }
}

END_NONAMESPACE


// @brief 一つの故障に対してテストパタン生成を行なう．
// @param[in] tvmgr テストベクタの管理用オブジェクト
// @param[out] d_list 検出された故障のリスト
// @param[out] u_list 検出不能と判定された故障のリスト
void
DtpgSat::dtpg_single(TvMgr& tvmgr,
		     vector<SaFault*>& d_list,
		     vector<SaFault*>& u_list)
{
  mNetwork->activate_all();
  const vector<DtpgFault*>& flist = mNetwork->fault_list();
  for (vector<DtpgFault*>::const_iterator p = flist.begin();
       p != flist.end(); ++ p) {
    DtpgFault* f = *p;
    if ( f->safault()->status() != kFsDetected ) {
      single_mode(f, tvmgr, d_list, u_list);
    }
  }
}

// @brief dual モードでテスト生成を行なう．
// @param[in] tvmgr テストベクタの管理用オブジェクト
// @param[out] d_list 検出された故障のリスト
// @param[out] u_list 検出不能と判定された故障のリスト
void
DtpgSat::dtpg_dual(TvMgr& tvmgr,
		   vector<SaFault*>& d_list,
		   vector<SaFault*>& u_list)
{
  mNetwork->activate_all();
  ymuint nn = mNetwork->node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    DtpgNode* node = mNetwork->node(i);
    DtpgFault* f0 = node->output_fault(0);
    DtpgFault* f1 = node->output_fault(1);
    if ( f0 != NULL && f0->safault()->status() == kFsDetected ) {
      f0 = NULL;
    }
    if ( f1 != NULL && f1->safault()->status() == kFsDetected ) {
      f1 = NULL;
    }
    if ( f0 != NULL && f1 != NULL ) {
      dual_mode(f0, f1, tvmgr, d_list, u_list);
    }
    else if ( f0 != NULL ) {
      single_mode(f0, tvmgr, d_list, u_list);
    }
    else if ( f1 != NULL ) {
      single_mode(f1, tvmgr, d_list, u_list);
    }
    ymuint ni = node->fanin_num();
    for (ymuint j = 0; j < ni; ++ j) {
      DtpgFault* f0 = node->input_fault(0, j);
      DtpgFault* f1 = node->input_fault(1, j);
      if ( f0 != NULL && f0->safault()->status() == kFsDetected ) {
	f0 = NULL;
      }
      if ( f1 != NULL && f1->safault()->status() == kFsDetected ) {
	f1 = NULL;
      }
      if ( f0 != NULL && f1 != NULL ) {
	dual_mode(f0, f1, tvmgr, d_list, u_list);
      }
      else if ( f0 != NULL ) {
	single_mode(f0, tvmgr, d_list, u_list);
      }
      else if ( f1 != NULL ) {
	single_mode(f1, tvmgr, d_list, u_list);
      }
    }
  }
}

// @brief ffr モードでテスト生成を行なう．
// @param[in] tvmgr テストベクタの管理用オブジェクト
// @param[out] d_list 検出された故障のリスト
// @param[out] u_list 検出不能と判定された故障のリスト
void
DtpgSat::dtpg_ffr(TvMgr& tvmgr,
		  vector<SaFault*>& d_list,
		  vector<SaFault*>& u_list)
{
  mNetwork->activate_all();
  vector<DtpgFFR*> ffr_list;
  mNetwork->get_ffr_list(ffr_list);
  for (vector<DtpgFFR*>::const_iterator p = ffr_list.begin();
       p != ffr_list.end(); ++ p) {
    DtpgFFR* ffr = *p;
    ffr_mode(ffr, tvmgr, d_list, u_list);
  }
}

// @brief mffc モードでテスト生成を行なう．
// @param[in] tvmgr テストベクタの管理用オブジェクト
// @param[out] d_list 検出された故障のリスト
// @param[out] u_list 検出不能と判定された故障のリスト
void
DtpgSat::dtpg_mffc(TvMgr& tvmgr,
		   vector<SaFault*>& d_list,
		   vector<SaFault*>& u_list)
{
  mNetwork->activate_all();
  vector<DtpgFFR*> mffc_list;
  mNetwork->get_mffc_list(mffc_list);
  for (vector<DtpgFFR*>::const_iterator p = mffc_list.begin();
       p != mffc_list.end(); ++ p) {
    DtpgFFR* ffr = *p;
    ffr_mode(ffr, tvmgr, d_list, u_list);
  }
}

// @brief all モードでテスト生成を行なう．
// @param[in] tvmgr テストベクタの管理用オブジェクト
// @param[out] d_list 検出された故障のリスト
// @param[out] u_list 検出不能と判定された故障のリスト
void
DtpgSat::dtpg_all(TvMgr& tvmgr,
		  vector<SaFault*>& d_list,
		  vector<SaFault*>& u_list)
{
#if 0
  mNetwork->activate_all();
  vector<DtpgFFR*> mffc_list;
  mNetwork->get_mffc_list(mffc_list);
  for (vector<DtpgFFR*>::const_iterator p = mffc_list.begin();
       p != mffc_list.end(); ++ p) {
    DtpgFFR* ffr = *p;
    ffr_mode(ffr, tvmgr, d_list, u_list);
  }
#endif
}

// @brief 一つの故障に対してテストパタン生成を行なう．
// @param[in] tvmgr テストベクタの管理用オブジェクト
// @param[out] d_list 検出された故障のリスト
// @param[out] u_list 検出不能と判定された故障のリスト
void
DtpgSat::dtpg_single_posplit(ymuint po_pos,
			     TvMgr& tvmgr,
			     vector<SaFault*>& d_list,
			     vector<SaFault*>& u_list)
{
  const vector<DtpgFault*>& flist = mNetwork->fault_list();
  mNetwork->activate_po(po_pos);
  for (vector<DtpgFault*>::const_iterator p = flist.begin();
       p != flist.end(); ++ p) {
    DtpgFault* f = *p;
    if ( f->safault()->status() != kFsDetected && f->node()->is_active() ) {
      single_mode(f, tvmgr, d_list, u_list);
    }
  }
  mNetwork->activate_all();
}

// @brief dual モードでテスト生成を行なう．
// @param[in] tvmgr テストベクタの管理用オブジェクト
// @param[out] d_list 検出された故障のリスト
// @param[out] u_list 検出不能と判定された故障のリスト
void
DtpgSat::dtpg_dual_posplit(ymuint po_pos,
			   TvMgr& tvmgr,
			   vector<SaFault*>& d_list,
			   vector<SaFault*>& u_list)
{
  mNetwork->activate_po(po_pos);
  ymuint nn = mNetwork->active_node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    DtpgNode* node = mNetwork->active_node(i);
    DtpgFault* f0 = node->output_fault(0);
    DtpgFault* f1 = node->output_fault(1);
    if ( f0 != NULL && f0->safault()->status() == kFsDetected ) {
      f0 = NULL;
    }
    if ( f1 != NULL && f1->safault()->status() == kFsDetected ) {
      f1 = NULL;
    }
    if ( f0 != NULL && f1 != NULL ) {
      dual_mode(f0, f1, tvmgr, d_list, u_list);
    }
    else if ( f0 != NULL ) {
      single_mode(f0, tvmgr, d_list, u_list);
    }
    else if ( f1 != NULL ) {
      single_mode(f1, tvmgr, d_list, u_list);
    }
    ymuint ni = node->fanin_num();
    for (ymuint j = 0; j < ni; ++ j) {
      DtpgFault* f0 = node->input_fault(0, j);
      DtpgFault* f1 = node->input_fault(1, j);
      if ( f0 != NULL && f0->safault()->status() == kFsDetected ) {
	f0 = NULL;
      }
      if ( f1 != NULL && f1->safault()->status() == kFsDetected ) {
	f1 = NULL;
      }
      if ( f0 != NULL && f1 != NULL ) {
	dual_mode(f0, f1, tvmgr, d_list, u_list);
      }
      else if ( f0 != NULL ) {
	single_mode(f0, tvmgr, d_list, u_list);
      }
      else if ( f1 != NULL ) {
	single_mode(f1, tvmgr, d_list, u_list);
      }
    }
  }
}

// @brief ffr モードでテスト生成を行なう．
// @param[in] tvmgr テストベクタの管理用オブジェクト
// @param[out] d_list 検出された故障のリスト
// @param[out] u_list 検出不能と判定された故障のリスト
void
DtpgSat::dtpg_ffr_posplit(ymuint po_pos,
			  TvMgr& tvmgr,
			  vector<SaFault*>& d_list,
			  vector<SaFault*>& u_list)
{
  mNetwork->activate_po(po_pos);
  vector<DtpgFFR*> ffr_list;
  mNetwork->get_ffr_list(ffr_list);
  for (vector<DtpgFFR*>::const_iterator p = ffr_list.begin();
       p != ffr_list.end(); ++ p) {
    DtpgFFR* ffr = *p;
    ffr_mode(ffr, tvmgr, d_list, u_list);
  }
}

// @brief mffc モードでテスト生成を行なう．
// @param[in] tvmgr テストベクタの管理用オブジェクト
// @param[out] d_list 検出された故障のリスト
// @param[out] u_list 検出不能と判定された故障のリスト
void
DtpgSat::dtpg_mffc_posplit(ymuint po_pos,
			   TvMgr& tvmgr,
			   vector<SaFault*>& d_list,
			   vector<SaFault*>& u_list)
{
  mNetwork->activate_po(po_pos);
  vector<DtpgFFR*> mffc_list;
  mNetwork->get_mffc_list(mffc_list);
  for (vector<DtpgFFR*>::const_iterator p = mffc_list.begin();
       p != mffc_list.end(); ++ p) {
    DtpgFFR* ffr = *p;
    ffr_mode(ffr, tvmgr, d_list, u_list);
  }
}

// @brief all モードでテスト生成を行なう．
// @param[in] tvmgr テストベクタの管理用オブジェクト
// @param[out] d_list 検出された故障のリスト
// @param[out] u_list 検出不能と判定された故障のリスト
void
DtpgSat::dtpg_all_posplit(ymuint po_pos,
			  TvMgr& tvmgr,
			  vector<SaFault*>& d_list,
			  vector<SaFault*>& u_list)
{
#if 0
  mNetwork->activate_po(po_pos);
  vector<DtpgFFR*> ffr_list;
  mNetwork->get_ffr_list(ffr_list);
  for (vector<DtpgFFR*>::const_iterator p = ffr_list.begin();
       p != ffr_list.end(); ++ p) {
    DtpgFFR* ffr = *p;
    ffr_mode(ffr, tvmgr, d_list, u_list);
  }
#endif
}

// @brief 使用する SAT エンジンを指定する．
void
DtpgSat::set_mode(const string& type,
		  const string& option,
		  ostream* outp)
{
  mType = type;
  mOption = option;
  mOutP = outp;
}

// @brief 一つの故障に対してテストパタン生成を行なう．
// @param[in] f 故障ノード
// @param[in] tvmgr テストベクタの管理用オブジェクト
void
DtpgSat::single_mode(DtpgFault* f,
		     TvMgr& tvmgr,
		     vector<SaFault*>& d_list,
		     vector<SaFault*>& u_list)
{
  SatSolver solver(mType, mOption, mOutP);

  DtpgNode* fnode = f->node();

  // fnode から外部出力へ至る部分の CNF を作る．
  vector<DtpgNode*> input_list;
  make_prop_cnf(solver, fnode, input_list);

  DtpgNode* fsrc = fnode;
  if ( f->is_input_fault() ) {
    ymuint ipos = f->pos();
    make_ifault_cnf(solver, fnode, ipos);
    fsrc = fnode->fanin(ipos);
  }

  vector<Literal> assumptions(2);
  if ( f->val() ) {
    assumptions[0] = Literal(fsrc->gvar(), kPolNega);
    assumptions[1] = Literal(fsrc->fvar(), kPolPosi);
  }
  else {
    assumptions[0] = Literal(fsrc->gvar(), kPolPosi);
    assumptions[1] = Literal(fsrc->fvar(), kPolNega);
  }

  vector<Bool3> model;
#if 1
  Bool3 ans = solver.solve(assumptions, model);
#else
  Bool3 ans = kB3True;
#endif
  SatStats sat_stat;
  solver.get_stats(sat_stat);
  //mStatsList.push_back(sat_stat);
  if ( ans == kB3True ) {
    TestVector* tv = tvmgr.new_vector();
    set_tv(model, input_list, tv);
    d_list.push_back(f->safault());
  }
  else if ( ans == kB3False ) {
    u_list.push_back(f->safault());
  }
}

// @brief 同じ位置の2つの出力故障に対してテストパタン生成を行なう．
// @param[in] f0 0縮退故障
// @param[in] f1 1縮退故障
// @param[in] tvmgr テストベクタの管理用オブジェクト
void
DtpgSat::dual_mode(DtpgFault* f0,
		   DtpgFault* f1,
		   TvMgr& tvmgr,
		   vector<SaFault*>& d_list,
		   vector<SaFault*>& u_list)
{
  SatSolver solver(mType, mOption, mOutP);

  DtpgNode* fnode = f0->node();

  // fnode から外部出力へ至る部分の CNF を作る．
  vector<DtpgNode*> input_list;
  make_prop_cnf(solver, fnode, input_list);

  DtpgNode* fsrc = fnode;
  if ( f0->is_input_fault() ) {
    ymuint ipos = f0->pos();
    make_ifault_cnf(solver, fnode, ipos);
    fsrc = f0->source_node();
  }

  DtpgFault* f[2] = { f0, f1 };
  for (int fval = 0; fval < 2; ++ fval) {
    vector<Literal> assumptions(2);
    if ( fval ) {
      assumptions[0] = Literal(fsrc->gvar(), kPolNega);
      assumptions[1] = Literal(fsrc->fvar(), kPolPosi);
    }
    else {
      assumptions[0] = Literal(fsrc->gvar(), kPolPosi);
      assumptions[1] = Literal(fsrc->fvar(), kPolNega);
    }

    vector<Bool3> model;
#if 1
    Bool3 ans = solver.solve(assumptions, model);
#else
    Bool3 ans = kB3True;
#endif
    SatStats sat_stat;
    solver.get_stats(sat_stat);
    //mStatsList.push_back(sat_stat);
    if ( ans == kB3True ) {
      TestVector* tv = tvmgr.new_vector();
      set_tv(model, input_list, tv);
      d_list.push_back(f[fval]->safault());
    }
    else if ( ans == kB3False ) {
      u_list.push_back(f[fval]->safault());
    }
  }
}

// @brief FFR 内の故障に対してテストパタン生成を行なう．
// @param[in] ffr FFR を表すクラス
// @param[in] tvmgr テストベクタの管理用オブジェクト
// @note flist の故障は必ず root が dominator となっていなければならない．
void
DtpgSat::ffr_mode(DtpgFFR* ffr,
		  TvMgr& tvmgr,
		  vector<SaFault*>& d_list,
		  vector<SaFault*>& u_list)
{
  vector<DtpgFault*> flist;
  hash_set<ymuint> fmark;
  hash_map<ymuint, ymuint> fmap;
  const vector<DtpgNode*>& node_list = ffr->node_list();
  for (vector<DtpgNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    DtpgNode* node = *p;
    bool found = false;
    for (int val = 0; val < 2; ++ val) {
      DtpgFault* f = node->output_fault(val);
      if ( f != NULL && f->safault()->status() != kFsDetected ) {
	flist.push_back(f);
	found = true;
      }
    }
    ymuint ni = node->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      for (int val = 0; val < 2; ++ val) {
	DtpgFault* f = node->input_fault(val, i);
	if ( f != NULL && f->safault()->status() != kFsDetected ) {
	  flist.push_back(f);
	  found = true;
	}
      }
      DtpgNode* inode = node->fanin(i);
      if ( fmark.count(inode->id()) > 0 ) {
	found = true;
      }
    }
    if ( found ) {
      fmark.insert(node->id());
    }
  }

  ymuint nf = flist.size();
  if ( nf == 0 ) {
    return;
  }

  for (ymuint i = 0; i < nf; ++ i) {
    DtpgFault* f = flist[i];
    fmap.insert(make_pair(f->id(), i));
  }

  DtpgNode* root = ffr->root();

  SatSolver solver(mType, mOption, mOutP);

  // root から外部出力までの正常回路，故障回路を作る．
  vector<DtpgNode*> input_list;
  make_prop_cnf(solver, root, input_list);

  // 故障回路用の変数を割り当てる．
  for (vector<DtpgNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    DtpgNode* node = *p;
    if ( (node != root) && (fmark.count(node->id()) > 0) ) {
      node->set_fvar(solver.new_var());
      node->set_dvar(solver.new_var());
    }
  }

  vector<VarId> flt_var(nf);
  vector<VarId> tmp_var(nf);
  for (ymuint i = 0; i < nf; ++ i) {
    flt_var[i] = solver.new_var();
    tmp_var[i] = solver.new_var();
  }

  // FFR 内の故障回路を作る．
  for (vector<DtpgNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    DtpgNode* node = *p;
    if ( fmark.count(node->id()) == 0 ) {
      continue;
    }

    ymuint ni = node->fanin_num();
    vector<Literal> inputs(ni);
    for (ymuint i = 0; i < ni; ++ i) {
      DtpgNode* inode = node->fanin(i);
      VarId fvar;
      if ( fmark.count(inode->id()) > 0 ) {
	fvar = inode->fvar();
      }
      else {
	fvar = inode->gvar();
      }
      for (ymint val = 0; val < 2; ++ val) {
	DtpgFault* f = node->input_fault(val, i);
	if ( f != NULL && f->safault()->status() != kFsDetected ) {
	  hash_map<ymuint, ymuint>::iterator p = fmap.find(f->id());
	  assert_cond( p != fmap.end(), __FILE__, __LINE__);
	  ymuint fid = p->second;
	  if ( val == 0 ) {
	    make_cnf_and(solver, flt_var[fid], fvar, tmp_var[fid]);
	  }
	  else {
	    make_cnf_or(solver, flt_var[fid], fvar, tmp_var[fid]);
	  }
	  fvar = tmp_var[fid];
	}
      }
      inputs[i] = Literal(fvar, kPolPosi);
    }

    VarId tmp_ovar = node->fvar();
    for (ymint val = 0; val < 2; ++ val) {
      DtpgFault* f = node->output_fault(val);
      if ( f != NULL && f->safault()->status() != kFsDetected ) {
	hash_map<ymuint, ymuint>::iterator p = fmap.find(f->id());
	assert_cond( p != fmap.end(), __FILE__, __LINE__);
	ymuint fid = p->second;
	if ( val == 0 ) {
	  make_cnf_and(solver, flt_var[fid], tmp_var[fid], tmp_ovar);
	}
	else {
	  make_cnf_or(solver, flt_var[fid], tmp_var[fid], tmp_ovar);
	}
	tmp_ovar = tmp_var[fid];
      }
    }

    Literal glit(node->gvar(), kPolPosi);
    Literal olit(tmp_ovar, kPolPosi);
    if ( node->is_input() ) {
      solver.add_clause(~glit,  olit);
      solver.add_clause( glit, ~olit);
    }
    else {
      make_node_cnf(solver, node, olit, inputs);
    }
    if ( node != root ) { // root の分は make_prop_cnf() で作っている．
      Literal flit(node->fvar(), kPolPosi);
      Literal dlit(node->dvar(), kPolPosi);
      // XOR(glit, flit, dlit) を追加する．
      // 要するに正常回路と故障回路で異なっているとき dlit が 1 となる．
      solver.add_clause(~glit, ~flit, ~dlit);
      solver.add_clause(~glit,  flit,  dlit);
      solver.add_clause( glit, ~flit,  dlit);
      solver.add_clause( glit,  flit, ~dlit);
    }
  }


  // 個々の故障に対するテスト生成を行なう．
  for (ymuint i = 0; i < nf; ++ i) {
    DtpgFault* f = flist[i];
    vector<Literal> assumptions;
    assumptions.reserve(nf + 1);
    for (ymuint j = 0; j < nf; ++ j) {
      tPol pol = (j == i) ? kPolPosi : kPolNega;
      assumptions.push_back(Literal(flt_var[j], pol));
    }
    for (DtpgNode* node = f->node(); ; node = node->active_fanout(0)) {
      Literal dlit(node->dvar(), kPolPosi);
      assumptions.push_back(dlit);
      if ( node == root ) {
	break;
      }
    }
    DtpgNode* fnode = f->node();
    if ( f->is_input_fault() ) {
      fnode = f->source_node();
    }
    tPol pol = (f->val() == 0) ? kPolPosi : kPolNega;
    assumptions.push_back(Literal(fnode->gvar(), pol));
    vector<Bool3> model;
#if 1
    Bool3 ans = solver.solve(assumptions, model);
#else
    Bool3 ans = kB3True;
#endif
    SatStats sat_stat;
    solver.get_stats(sat_stat);
    //mStatsList.push_back(sat_stat);
    if ( ans == kB3True ) {
      TestVector* tv = tvmgr.new_vector();
      set_tv(model, input_list, tv);
      d_list.push_back(f->safault());
    }
    else if ( ans == kB3False ) {
      u_list.push_back(f->safault());
    }
  }
}

// @brief fnode の故障が伝搬する条件を表す CNF を作る．
void
DtpgSat::make_prop_cnf(SatSolver& solver,
		       DtpgNode* fnode,
		       vector<DtpgNode*>& input_list)
{
  vector<DtpgNode*> tfo_list;
  vector<DtpgNode*> tfi_list;
  mNetwork->mark_tfo_tfi(fnode, tfo_list, tfi_list);

  // 以降は kTFO か kTFI マークのついたノードのみを対象とする．

  // TFO マークのついたノード用の変数の生成 (glit, flit, dlit の3つを作る)
  for (vector<DtpgNode*>::iterator p = tfo_list.begin();
       p != tfo_list.end(); ++ p) {
    DtpgNode* node = *p;
    node->set_gvar(solver.new_var());
    node->set_fvar(solver.new_var());
    node->set_dvar(solver.new_var());
  }
  // TFI マークのついたノード用の変数の生成 (glit のみを作る)
  for (vector<DtpgNode*>::iterator p = tfi_list.begin();
       p != tfi_list.end(); ++ p) {
    DtpgNode* node = *p;
    node->set_gvar(solver.new_var());
  }

  if ( fnode->is_input() ) {
    input_list.push_back(fnode);
  }
  for (vector<DtpgNode*>::iterator p = tfi_list.begin();
       p != tfi_list.end(); ++ p) {
    DtpgNode* node = *p;
    if ( node->is_input() ) {
      input_list.push_back(node);
    }
  }

  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////

  for (vector<DtpgNode*>::const_iterator p = tfi_list.begin();
       p != tfi_list.end(); ++ p) {
    DtpgNode* node = *p;
    ymuint ni = node->fanin_num();
    vector<Literal> inputs(ni);
    for (ymuint i = 0; i < ni; ++ i) {
      DtpgNode* inode = node->fanin(i);
      inputs[i] = Literal(inode->gvar(), kPolPosi);
    }
    Literal output(node->gvar(), kPolPosi);
    make_node_cnf(solver, node, output, inputs);
  }
  for (vector<DtpgNode*>::const_iterator p = tfo_list.begin();
       p != tfo_list.end(); ++ p) {
    DtpgNode* node = *p;
    ymuint ni = node->fanin_num();
    vector<Literal> inputs(ni);
    for (ymuint i = 0; i < ni; ++ i) {
      DtpgNode* inode = node->fanin(i);
      inputs[i] = Literal(inode->gvar(), kPolPosi);
    }
    Literal output(node->gvar(), kPolPosi);
    make_node_cnf(solver, node, output, inputs);
  }


  //////////////////////////////////////////////////////////////////////
  // 故障回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  vector<Literal> odiff;
  odiff.reserve(mNetwork->output_num2());

  // mark の付いていないノードは正常回路，付いているノードは故障回路
  // を用いて CNF を作る．
  for (ymuint i = 0; i < tfo_list.size(); ++ i) {
    DtpgNode* node = tfo_list[i];

    Literal glit(node->gvar(), kPolPosi);
    Literal flit(node->fvar(), kPolPosi);
    Literal dlit(node->dvar(), kPolPosi);

    // XOR(glit, flit, dlit) を追加する．
    // 要するに正常回路と故障回路で異なっているとき dlit が 1 となる．
    solver.add_clause(~glit, ~flit, ~dlit);
    solver.add_clause(~glit,  flit,  dlit);
    solver.add_clause( glit, ~flit,  dlit);
    solver.add_clause( glit,  flit, ~dlit);

    // node が出力の時には odiff に追加しておく．
    if ( node->is_output() ) {
      odiff.push_back(dlit);
    }

    if ( node == fnode ) {
      continue;
    }

    // 入力のリテラル
    ymuint ni = node->fanin_num();
    vector<Literal> inputs(ni);

    // dlit が 1 の時，入力のノードの dlit も1でなければならない．
    vector<Literal> dep;
    dep.reserve(ni + 1);
    dep.push_back(~dlit);

    for (ymuint j = 0; j < ni; ++ j) {
      DtpgNode* inode = node->fanin(j);
      if ( inode->mark() == DtpgNode::kTFO ) {
	inputs[j] = Literal(inode->fvar(), kPolPosi);
	dep.push_back(Literal(inode->dvar(), kPolPosi));
      }
      else {
	inputs[j] = Literal(inode->gvar(), kPolPosi);
      }
    }

    // node の入力と出力の関係の clause を追加する．
    make_node_cnf(solver, node, flit, inputs);

    solver.add_clause(dep);
  }

  mNetwork->clear_node_mark(tfo_list, tfi_list);

  // 出力のうち，最低1つは故障差が伝搬しなければならない．
  assert_cond( !odiff.empty(), __FILE__, __LINE__);

  solver.add_clause(odiff);
}

// @brief 入力に故障を持つノードの CNF を作る．
// @param[in] solver SAT ソルバ
// @param[in] fnode 対象のノード
// @param[in] ipos 故障のある入力の番号
void
DtpgSat::make_ifault_cnf(SatSolver& solver,
			 DtpgNode* fnode,
			 ymuint ipos)
{
  DtpgNode* fsrc = fnode->fanin(ipos);
  fsrc->set_fvar(solver.new_var());
  fsrc->set_dvar(solver.new_var());

  ymuint ni = fnode->fanin_num();
  vector<Literal> inputs(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    DtpgNode* inode = fnode->fanin(i);
    if ( i == ipos ) {
      inputs[i] = Literal(inode->fvar(), kPolPosi);
    }
    else {
      inputs[i] = Literal(inode->gvar(), kPolPosi);
    }
  }

  Literal glit(fnode->gvar(), kPolPosi);
  Literal flit(fnode->fvar(), kPolPosi);
  Literal dlit(fnode->dvar(), kPolPosi);

  // XOR(glit, flit, dlit) を追加する．
  // 要するに正常回路と故障回路で異なっているとき dlit が 1 となる．
  solver.add_clause(~glit, ~flit, ~dlit);
  solver.add_clause(~glit,  flit,  dlit);
  solver.add_clause( glit, ~flit,  dlit);
  solver.add_clause( glit,  flit, ~dlit);

  vector<Literal> dep(2);
  dep[0] = ~dlit;
  dep[1] = Literal(fsrc->dvar(), kPolPosi);
  solver.add_clause(dep);

  make_node_cnf(solver, fnode, flit, inputs);
}

// @brief ノードの入出力の関係を表す CNF を作る．
void
DtpgSat::make_node_cnf(SatSolver& solver,
		       DtpgNode* node,
		       Literal olit,
		       const vector<Literal>& ilits)
{
  if ( node->is_cplx_logic() ) {
    LogExpr lexp = mNetwork->tgnetwork().get_lexp(node->tgnode());
    make_cnf_from_lexp(solver, lexp, olit, ilits);
  }
  else {
    make_cnf_from_type(solver, node->type(), olit, ilits);
  }
}

// @brief SAT の結果からテストベクタを作る．
void
DtpgSat::set_tv(const vector<Bool3>& model,
		const vector<DtpgNode*>& input_list,
		TestVector* tv)
{
  tv->init();
  for (vector<DtpgNode*>::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    DtpgNode* node = *p;
    VarId idx = node->gvar();
    ymuint iid = node->input_id();
    // 今のところ model には 0 か 1 しか設定されていないはず．
    if ( model[idx.val()] == kB3True ) {
      tv->set_val(iid, kVal1);
    }
    else if ( model[idx.val()] == kB3False ) {
      tv->set_val(iid, kVal0);
    }
  }
}

// @brief StatsList をクリアする．
void
DtpgSat::clear_stats_list()
{
  mStatsList.clear();
}

// @brief StatsList を得る．
const vector<SatStats>&
DtpgSat::stats_list() const
{
  return mStatsList;
}

END_NAMESPACE_YM_SATPG_DTPG
