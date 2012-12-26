
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
#include "DtpgOperator.h"
#include "ym_logic/SatSolver.h"


BEGIN_NAMESPACE_YM_SATPG

Dtpg*
new_DtpgSat()
{
  return new nsDtpg::DtpgSat();
}

END_NAMESPACE_YM_SATPG


BEGIN_NAMESPACE_YM_SATPG_DTPG

BEGIN_NONAMESPACE

void
make_flt_cnf(SatSolver& solver,
	     VarId ivar,
	     VarId fvar,
	     VarId ovar,
	     int val)
{
  Literal l0(ivar, kPolPosi);
  Literal l1(fvar, kPolPosi);
  Literal l2(ovar, kPolPosi);
  if ( val == 0 ) {
    solver.add_clause( l0, ~l2);
    solver.add_clause(~l1, ~l2);
    solver.add_clause(~l0,  l1, l2);
  }
  else {
    solver.add_clause(~l0, l2);
    solver.add_clause(~l1, l2);
    solver.add_clause( l0, l1, ~l2);
  }
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
    solver.add_clause( inputs[0], ~output);
    solver.add_clause(~inputs[0],  output);
    break;

  case kTgNot:
    solver.add_clause( inputs[0],  output);
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

// @brief ノードの入出力の関係を表す CNF を作る．
void
make_node_cnf(SatSolver& solver,
	      DtpgNode* node,
	      Literal olit,
	      const vector<Literal>& ilits)
{
  if ( node->is_cplx_logic() ) {
    make_cnf_from_lexp(solver, node->expr(), olit, ilits);
  }
  else {
    make_cnf_from_type(solver, node->type(), olit, ilits);
  }
}

// @brief 入力に故障を持つノードの CNF を作る．
// @param[in] solver SAT ソルバ
// @param[in] fnode 対象のノード
// @param[in] ipos 故障のある入力の番号
void
make_ifault_cnf(SatSolver& solver,
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

void
mark_tfo(DtpgNode* node,
	 hash_set<ymuint>& tfo_mark)
{
  if ( tfo_mark.count(node->id()) > 0 ) {
    return;
  }
  tfo_mark.insert(node->id());

  ymuint nfo = node->active_fanout_num();
  for (ymuint i = 0; i < nfo; ++ i) {
    mark_tfo(node->active_fanout(i), tfo_mark);
  }
}

END_NONAMESPACE


// @brief コンストラクタ
DtpgSat::DtpgSat()
{
  mNetwork = NULL;
  mSkip = false;
}

// @brief デストラクタ
DtpgSat::~DtpgSat()
{
  delete mNetwork;
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

// @brief モードでテスト生成を行なう．
// @param[in] op テスト生成後に呼ばれるファンクター
// @param[in] option オプション文字列
void
DtpgSat::run(DtpgOperator& op,
	     const string& option)
{
  bool single = false;
  bool dual = false;
  bool ffr = false;
  bool mffc = false;
  bool all = false;
  bool po = false;
  mSkip = false;

  if ( option == string() || option == "single" ) {
    single = true;
  }
  else if ( option == "dual" ) {
    dual = true;
  }
  else if ( option == "ffr" ) {
    ffr = true;
  }
  else if ( option == "mffc" ) {
    mffc = true;
  }
  else if ( option == "single_po" ) {
    single = true;
    po = true;
  }
  else if ( option == "single_po_skip" ) {
    single = true;
    po = true;
    mSkip = true;
  }
  else if ( option == "dual_po" ) {
    dual = true;
    po = true;
  }
  else if ( option == "dual_po_skip" ) {
    dual = true;
    po = true;
    mSkip = true;

  }
  else if ( option == "ffr_po" ) {
    ffr = true;
    po = true;
  }
  else if ( option == "ffr_po_skip" ) {
    ffr = true;
    po = true;
    mSkip = true;
  }
  else if ( option == "mffc_po" ) {
    mffc = true;
    po = true;
  }
  else if ( option == "mffc_po_skip" ) {
    mffc = true;
    po = true;
    mSkip = true;
  }
  else if ( option == "all_po" ) {
    all = true;
    po = true;
  }
  else if ( option == "all_po_skip" ) {
    all = true;
    po = true;
    mSkip = true;
  }
  else {
    cout << "illegal option " << option << ". ignored" << endl;
    return;
  }

  if ( !po ) {
    mNetwork->activate_all();
    if ( single ) {
      single_sub(op);
    }
    else if ( dual ) {
      dual_sub(op);
    }
    else if ( ffr ) {
      ffr_sub(op);
    }
    else if ( mffc ) {
      mffc_sub(op);
    }
  }
  else {
    ymuint no = mNetwork->output_num2();
    for (ymuint po_pos = 0; po_pos < no; ++ po_pos) {
      DtpgNode* onode = mNetwork->output2(po_pos);
      mNetwork->activate_po(onode);
      if ( single ) {
	single_sub(op);
      }
      else if ( dual ) {
	dual_sub(op);
      }
      else if ( ffr ) {
	ffr_sub(op);
      }
      else if ( mffc ) {
	mffc_sub(op);
      }
      else if ( all ) {
	all_sub(op);
      }
    }
    if ( mSkip ) {
      clear_skip();
    }

    mSkip = false;
  }
}

// @brief single モードの共通処理
void
DtpgSat::single_sub(DtpgOperator& op)
{
  ymuint nn = mNetwork->active_node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    DtpgNode* node = mNetwork->active_node(i);

    // 出力の故障
    DtpgFault* f0 = node->output_fault(0);
    if ( f0 != NULL && !f0->is_skip() ) {
      SatSolver solver(mType, mOption, mOutP);
      dtpg_single(solver, f0, op);
    }

    DtpgFault* f1 = node->output_fault(1);
    if ( f1 != NULL && !f1->is_skip() ) {
      SatSolver solver(mType, mOption, mOutP);
      dtpg_single(solver, f1, op);
    }

    // 入力の故障
    ymuint ni = node->fanin_num();
    for (ymuint j = 0; j < ni; ++ j) {
      DtpgFault* f0 = node->input_fault(0, j);
      if ( f0 != NULL && !f0->is_skip() ) {
	SatSolver solver(mType, mOption, mOutP);
	dtpg_single(solver, f0, op);
      }

      DtpgFault* f1 = node->input_fault(1, j);
      if ( f1 != NULL && !f1->is_skip() ) {
	SatSolver solver(mType, mOption, mOutP);
	dtpg_single(solver, f1, op);
      }
    }
  }
}

// @brief dual モードの共通処理
void
DtpgSat::dual_sub(DtpgOperator& op)
{
  ymuint nn = mNetwork->active_node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    DtpgNode* node = mNetwork->active_node(i);

    // 出力の故障
    DtpgFault* f0 = node->output_fault(0);
    DtpgFault* f1 = node->output_fault(1);
    if ( f0 != NULL && f0->is_skip() ) {
      f0 = NULL;
    }
    if ( f1 != NULL && f1->is_skip() ) {
      f1 = NULL;
    }
    if ( f0 != NULL && f1 != NULL ) {
      SatSolver solver(mType, mOption, mOutP);
      dtpg_dual(solver, f0, f1, op);
    }
    else if ( f0 != NULL ) {
      SatSolver solver(mType, mOption, mOutP);
      dtpg_single(solver, f0, op);
    }
    else if ( f1 != NULL ) {
      SatSolver solver(mType, mOption, mOutP);
      dtpg_single(solver, f1, op);
    }

    // 入力の故障
    ymuint ni = node->fanin_num();
    for (ymuint j = 0; j < ni; ++ j) {
      DtpgFault* f0 = node->input_fault(0, j);
      DtpgFault* f1 = node->input_fault(1, j);
      if ( f0 != NULL && f0->is_skip() ) {
	f0 = NULL;
      }
      if ( f1 != NULL && f1->is_skip() ) {
	f1 = NULL;
      }
      if ( f0 != NULL && f1 != NULL ) {
	SatSolver solver(mType, mOption, mOutP);
	dtpg_dual(solver, f0, f1, op);
      }
      else if ( f0 != NULL ) {
	SatSolver solver(mType, mOption, mOutP);
	dtpg_single(solver, f0, op);
      }
      else if ( f1 != NULL ) {
	SatSolver solver(mType, mOption, mOutP);
	dtpg_single(solver, f1, op);
      }
    }
  }
}

// @brief ffr モードの共通処理
void
DtpgSat::ffr_sub(DtpgOperator& op)
{
  vector<DtpgFFR*> ffr_list;
  mNetwork->get_ffr_list(ffr_list);
  for (vector<DtpgFFR*>::const_iterator p = ffr_list.begin();
       p != ffr_list.end(); ++ p) {
    DtpgFFR* ffr = *p;
    ffr_mode(ffr, op);
    delete ffr;
  }
}

// @brief mffc モードの共通処理
void
DtpgSat::mffc_sub(DtpgOperator& op)
{
  vector<DtpgFFR*> mffc_list;
  mNetwork->get_mffc_list(mffc_list);
  for (vector<DtpgFFR*>::const_iterator p = mffc_list.begin();
       p != mffc_list.end(); ++ p) {
    DtpgFFR* ffr = *p;
    ffr_mode(ffr, op);
    delete ffr;
  }
}

// @brief all モードの共通処理
void
DtpgSat::all_sub(DtpgOperator& op)
{
  vector<DtpgFault*> flist;
  ymuint n = mNetwork->active_node_num();
  vector<DtpgNode*> node_list;
  node_list.reserve(n);
  DtpgNode* onode = NULL;
  for (ymuint i = 0; i < n; ++ i) {
    DtpgNode* node = mNetwork->active_node(i);
    node_list.push_back(node);
    if ( node->is_output() ) {
      onode = node;
    }
    for (int val = 0; val < 2; ++ val) {
      DtpgFault* f = node->output_fault(val);
      if ( f != NULL && !f->is_skip() ) {
	flist.push_back(f);
      }
    }
    ymuint ni = node->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      for (int val = 0; val < 2; ++ val) {
	DtpgFault* f = node->input_fault(val, i);
	if ( f != NULL && !f->is_skip() ) {
	  flist.push_back(f);
	}
      }
    }
  }
  if ( flist.empty() ) {
    return;
  }

  SatSolver solver(mType, mOption, mOutP);
  dtpg_ffr(solver, flist, onode, node_list, op);
}

// @brief 一つの FFR に対してテストパタン生成を行う．
// @param[in] ffr 対象の FFR
void
DtpgSat::ffr_mode(DtpgFFR* ffr,
		  DtpgOperator& op)
{
  vector<DtpgFault*> flist;
  const vector<DtpgNode*>& node_list = ffr->node_list();
  for (vector<DtpgNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    DtpgNode* node = *p;
    for (int val = 0; val < 2; ++ val) {
      DtpgFault* f = node->output_fault(val);
      if ( f != NULL && !f->is_skip() ) {
	flist.push_back(f);
      }
    }
    ymuint ni = node->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      for (int val = 0; val < 2; ++ val) {
	DtpgFault* f = node->input_fault(val, i);
	if ( f != NULL && !f->is_skip() ) {
	  flist.push_back(f);
	}
      }
    }
  }

  if ( flist.empty() ) {
    return;
  }

  SatSolver solver(mType, mOption, mOutP);
  dtpg_ffr(solver, flist, ffr->root(), node_list, op);
}

// @brief スキップフラグを解除する．
void
DtpgSat::clear_skip()
{
  const vector<DtpgFault*>& flist = mNetwork->fault_list();
  for (vector<DtpgFault*>::const_iterator p = flist.begin();
       p != flist.end(); ++ p) {
    DtpgFault* f = *p;
    if ( f->is_skip() && f->is_untestable() ) {
      f->clear_skip();
      f->clear_untestable();
    }
  }
}

// @brief 統計情報をクリアする．
void
DtpgSat::clear_stats()
{
  mRunCount = 0;
  mSatCount = 0;
  mRestart = 0;
  mVarNum = 0;
  mConstrClauseNum = 0;
  mConstrLitNum = 0;
  mLearntClauseNum = 0;
  mLearntLitNum = 0;
  mConflictNum = 0;
  mDecisionNum = 0;
  mPropagationNum = 0;
  mTimer.reset();
  mTimer.start();
}

// @brief 一つの故障に対してテストパタン生成を行う．
// @param[in] solver SatSolver
// @param[in] f 故障
// @param[in] op テスト生成の結果を処理するファンクター
void
DtpgSat::dtpg_single(SatSolver& solver,
		     DtpgFault* f,
		     DtpgOperator& op)
{
  mTimer.start();

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

  ymuint nim = 0;
  for (DtpgNode* node = fnode; node != NULL; node = node->imm_dom()) {
    ++ nim;
  }

  vector<Literal> assumptions(nim + 2);
  if ( f->val() ) {
    assumptions[0] = Literal(fsrc->gvar(), kPolNega);
    assumptions[1] = Literal(fsrc->fvar(), kPolPosi);
  }
  else {
    assumptions[0] = Literal(fsrc->gvar(), kPolPosi);
    assumptions[1] = Literal(fsrc->fvar(), kPolNega);
  }

  // dominator ノードの dvar は1でなければならない．
  ymuint i = 2;
  for (DtpgNode* node = fnode; node != NULL; node = node->imm_dom(), ++ i) {
    assumptions[i] = Literal(node->dvar(), kPolPosi);
  }

  solve(solver, f, assumptions, input_list, op);

  update_stats(solver, 1);
}

// @brief 同じ位置の2つの出力故障に対してテストパタン生成を行なう．
// @param[in] solver SatSolver
// @param[in] f0 0縮退故障
// @param[in] f1 1縮退故障
// @param[in] op テスト生成の結果を処理するファンクター
void
DtpgSat::dtpg_dual(SatSolver& solver,
		   DtpgFault* f0,
		   DtpgFault* f1,
		   DtpgOperator& op)
{
  mTimer.start();

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

  ymuint nim = 0;
  for (DtpgNode* node = fnode; node != NULL; node = node->imm_dom()) {
    ++ nim;
  }

  vector<Literal> assumptions(nim + 2);

  assumptions[0] = Literal(fsrc->gvar(), kPolPosi);
  assumptions[1] = Literal(fsrc->fvar(), kPolNega);

  // dominator ノードの dvar は1でなければならない．
  ymuint i = 2;
  for (DtpgNode* node = fnode; node != NULL; node = node->imm_dom(), ++ i) {
    assumptions[i] = Literal(node->dvar(), kPolPosi);
  }

  solve(solver, f0, assumptions, input_list, op);

  assumptions[0] = Literal(fsrc->gvar(), kPolNega);
  assumptions[1] = Literal(fsrc->fvar(), kPolPosi);

  solve(solver, f1, assumptions, input_list, op);

  update_stats(solver, 2);
}

// @brief FFR 内の故障に対してテストパタン生成を行なう．
// @param[in] solver SatSolver
// @param[in] flist 故障リスト
// @param[in] root FFR の根のノード
// @param[in] node_list FFR 内のノードリスト
// @param[in] op テスト生成の結果を処理するファンクター
void
DtpgSat::dtpg_ffr(SatSolver& solver,
		  const vector<DtpgFault*>& flist,
		  DtpgNode* root,
		  const vector<DtpgNode*>& node_list,
		  DtpgOperator& op)
{
  mTimer.start();

  // 故障の影響を受けるノードにつけるマーク
  hash_set<ymuint> fnode_mark;
  // 故障IDををキーにして flist[] 中の位置を格納するハッシュ表
  hash_map<ymuint, ymuint> fid_map;

  ymuint nf = flist.size();
  for (ymuint i = 0; i < nf; ++ i) {
    DtpgFault* f = flist[i];
    fid_map.insert(make_pair(f->id(), i));
    fnode_mark.insert(f->node()->id());
  }
  for (vector<DtpgNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    DtpgNode* node = *p;
    if ( fnode_mark.count(node->id()) > 0 ) {
      continue;
    }
    bool found = false;
    ymuint ni = node->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      DtpgNode* inode = node->fanin(i);
      if ( fnode_mark.count(inode->id()) > 0 ) {
	found = true;
      }
    }
    if ( found ) {
      fnode_mark.insert(node->id());
    }
  }

  // root から外部出力までの正常回路，故障回路を作る．
  vector<DtpgNode*> input_list;
  make_prop_cnf(solver, root, input_list);

  // 故障回路用の変数を割り当てる．
  for (vector<DtpgNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    DtpgNode* node = *p;
    if ( (node != root) && (fnode_mark.count(node->id()) > 0) ) {
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
    if ( fnode_mark.count(node->id()) == 0 ) {
      continue;
    }

    ymuint ni = node->fanin_num();
    vector<Literal> inputs(ni);
    for (ymuint i = 0; i < ni; ++ i) {
      DtpgNode* inode = node->fanin(i);
      VarId fvar;
      if ( fnode_mark.count(inode->id()) > 0 ) {
	fvar = inode->fvar();
      }
      else {
	fvar = inode->gvar();
      }
      for (ymint val = 0; val < 2; ++ val) {
	DtpgFault* f = node->input_fault(val, i);
	if ( f == NULL ) {
	  continue;
	}
	hash_map<ymuint, ymuint>::iterator p = fid_map.find(f->id());
	if ( p != fid_map.end() ) {
	  ymuint fid = p->second;
	  make_flt_cnf(solver, fvar, flt_var[fid], tmp_var[fid], val);
	  fvar = tmp_var[fid];
	}
      }
      inputs[i] = Literal(fvar, kPolPosi);
    }

    VarId tmp_ovar = node->fvar();
    for (ymint val = 0; val < 2; ++ val) {
      DtpgFault* f = node->output_fault(val);
      if ( f == NULL ) {
	continue;
      }
      hash_map<ymuint, ymuint>::iterator p = fid_map.find(f->id());
      if ( p != fid_map.end() ) {
	ymuint fid = p->second;
	make_flt_cnf(solver, tmp_var[fid], flt_var[fid], tmp_ovar, val);
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

      // 出力の dlit が1になる条件を作る．
      // - 入力の dlit のいずれかが 1
      // - 入力のいずれかに故障がある．
      // - 出力に故障がある．
      vector<Literal> dep;
      dep.reserve(ni * 3 + 3);
      Literal dlit(node->dvar(), kPolNega);
      dep.push_back(dlit);
      for (ymuint j = 0; j < ni; ++ j) {
	DtpgNode* inode = node->fanin(j);
	if ( fnode_mark.count(inode->id()) > 0 ) {
	  dep.push_back(Literal(inode->dvar(), kPolPosi));
	}

	DtpgFault* fi0 = node->input_fault(0, j);
	if ( fi0 != NULL ) {
	  hash_map<ymuint, ymuint>::iterator p = fid_map.find(fi0->id());
	  if ( p != fid_map.end() ) {
	    ymuint fid = p->second;
	    dep.push_back(Literal(flt_var[fid], kPolPosi));
	  }
	}

	DtpgFault* fi1 = node->input_fault(1, j);
	if ( fi1 != NULL ) {
	  hash_map<ymuint, ymuint>::iterator p = fid_map.find(fi1->id());
	  if ( p != fid_map.end() ) {
	    ymuint fid = p->second;
	    dep.push_back(Literal(flt_var[fid], kPolPosi));
	  }
	}
      }

      DtpgFault* fo0 = node->output_fault(0);
      if ( fo0 != NULL ) {
	hash_map<ymuint, ymuint>::iterator p = fid_map.find(fo0->id());
	if ( p != fid_map.end() ) {
	  ymuint fid = p->second;
	  dep.push_back(Literal(flt_var[fid], kPolPosi));
	}
      }

      DtpgFault* fo1 = node->output_fault(1);
      if ( fo1 != NULL ) {
	hash_map<ymuint, ymuint>::iterator p = fid_map.find(fo1->id());
	if ( p != fid_map.end() ) {
	  ymuint fid = p->second;
	  dep.push_back(Literal(flt_var[fid], kPolPosi));
	}
      }

      solver.add_clause(dep);
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
    assumptions.reserve(node_list.size() + nf);

    // 該当の故障に対する変数のみ1にする．
    for (ymuint j = 0; j < nf; ++ j) {
      tPol pol = (j == i) ? kPolPosi : kPolNega;
      assumptions.push_back(Literal(flt_var[j], pol));
    }

    // 故障ノードの TFO 以外の dlit を0にする．
    hash_set<ymuint> tfo_mark;
    mark_tfo(f->node(), tfo_mark);
    for (vector<DtpgNode*>::const_iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      DtpgNode* node = *p;
      if ( fnode_mark.count(node->id()) > 0 && tfo_mark.count(node->id()) == 0 ) {
	Literal dlit(node->dvar(), kPolNega);
	assumptions.push_back(dlit);
      }
    }

    // dominator ノードの dvar は1でなければならない．
    for (DtpgNode* node = f->node(); node != NULL; node = node->imm_dom()) {
      Literal dlit(node->dvar(), kPolPosi);
      assumptions.push_back(dlit);
    }

    DtpgNode* fnode = f->node();
    if ( f->is_input_fault() ) {
      fnode = f->source_node();
    }
    tPol pol = (f->val() == 0) ? kPolPosi : kPolNega;
    assumptions.push_back(Literal(fnode->gvar(), pol));

    solve(solver, f, assumptions, input_list, op);
  }

  update_stats(solver, nf);
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

/// @brief 一つの SAT問題を解く．
void
DtpgSat::solve(SatSolver& solver,
	       DtpgFault* f,
	       const vector<Literal>& assumptions,
	       const vector<DtpgNode*>& input_list,
	       DtpgOperator& op)
{
#if 1
  vector<Bool3> model;
  Bool3 ans = solver.solve(assumptions, model);
  if ( ans == kB3True ) {
    f->set_skip();

    vector<ymuint> val_list;
    val_list.reserve(input_list.size());
    for (vector<DtpgNode*>::const_iterator p = input_list.begin();
	 p != input_list.end(); ++ p) {
      DtpgNode* node = *p;
      VarId idx = node->gvar();
      ymuint iid = node->input_id();

      // 今のところ model には 0 か 1 しか設定されていないはず．
      if ( model[idx.val()] == kB3True ) {
	val_list.push_back(iid * 2 + 1);
      }
      else if ( model[idx.val()] == kB3False ) {
	val_list.push_back(iid * 2 + 0);
      }
    }
    op.set_detected(f->safault(), val_list);
  }
  else if ( ans == kB3False ) {
    f->set_untestable();
    if ( mSkip ) {
      f->set_skip();
    }
    else {
      op.set_untestable(f->safault());
    }
  }
#else
  f->set_skip();
#endif
}

// @brief 統計情報を得る．
void
DtpgSat::get_stats() const
{
  if ( mRunCount > 0 ) {
    cout << "# of runs:                     " << mRunCount << endl
	 << "# of problems:                 " << mSatCount << endl
	 << "Ave. # of restarts:            " << (double) mRestart / mSatCount << endl
	 << "Ave. # of variables:           " << (double) mVarNum / mRunCount << endl
	 << "Ave. # of constraint clauses:  " << (double) mConstrClauseNum / mRunCount << endl
	 << "Ave. # of constraint literals: " << (double) mConstrLitNum / mRunCount << endl
	 << "Ave. # of learnt clauses:      " << (double) mLearntClauseNum / mRunCount << endl
	 << "Ave. # of learnt literals:     " << (double) mLearntLitNum / mRunCount << endl
	 << "Ave. # of conflicts:           " << (double) mConflictNum / mSatCount << endl
	 << "Ave. # of decisions:           " << (double) mDecisionNum / mSatCount << endl
	 << "Ave. # of implications:        " << (double) mPropagationNum / mSatCount << endl
	 << "CPUT time:                     " << mTimer.time().usr_time() << endl
	 << endl;
  }
}

// @brief 統計情報を得る．
void
DtpgSat::update_stats(SatSolver& solver,
		      ymuint n)
{
  SatStats sat_stat;
  solver.get_stats(sat_stat);

  ++ mRunCount;
  mSatCount += n;
  mRestart += sat_stat.mRestart;
  mVarNum += sat_stat.mVarNum;
  mConstrClauseNum += sat_stat.mConstrClauseNum;
  mConstrLitNum += sat_stat.mConstrLitNum;
  mLearntClauseNum += sat_stat.mLearntClauseNum;
  mLearntLitNum += sat_stat.mLearntLitNum;
  mConflictNum += sat_stat.mConflictNum;
  mDecisionNum += sat_stat.mDecisionNum;
  mPropagationNum += sat_stat.mPropagationNum;
  mTimer.stop();
}

END_NAMESPACE_YM_SATPG_DTPG
