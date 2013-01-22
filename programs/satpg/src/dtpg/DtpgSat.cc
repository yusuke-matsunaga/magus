
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
#include "ym_logic/SatStats.h"


BEGIN_NAMESPACE_YM_SATPG

Dtpg*
new_DtpgSat()
{
  return new nsDtpg::DtpgSat();
}

END_NAMESPACE_YM_SATPG


BEGIN_NAMESPACE_YM_SATPG_DTPG

BEGIN_NONAMESPACE

// @brief 故障挿入回路を表す CNF 式を作る．
// @param[in] solver SAT ソルバー
// @param[in] ivar 入力の変数
// @param[in] fvar 故障変数
// @param[in] ovar 出力の変数
// @param[in] fval 故障値
void
make_flt_cnf(SatSolver& solver,
	     VarId ivar,
	     VarId fvar,
	     VarId ovar,
	     int vval)
{
  Literal l0(ivar, kPolPosi);
  Literal l1(fvar, kPolPosi);
  Literal l2(ovar, kPolPosi);
  if ( vval == 0 ) {
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

// @brief ノードに正常回路用の変数を設定する．
// @param[in] solver SAT ソルバー
// @param[in] node 対象のノード
void
set_gvar(SatSolver& solver,
	 DtpgNode* node)
{
  // ノードそのものに割り当てる．
  VarId gvar = solver.new_var();
  node->set_gvar(gvar);
}

// @brief ノードに正常回路用の変数を設定する．
// @param[in] solver SAT ソルバー
// @param[in] node 対象のノード
void
set_gvar2(SatSolver& solver,
	  DtpgNode* node)
{
  // ノード内部のプリミティブに割り当てる．
  ymuint n = node->primitive_num();
  if ( n > 0 ) {
    ymuint n1 = n - 1;
    for (ymuint i = 0; i < n1; ++ i) {
      DtpgPrimitive* prim = node->primitive(i);
      VarId gvar;
      if ( prim->is_input() ) {
	ymuint ipos = prim->input_id();
	DtpgNode* inode = node->fanin(ipos);
	gvar = inode->gvar();
      }
      else {
	gvar = solver.new_var();
      }
      prim->set_gvar(gvar);
      prim->set_fvar(gvar);
    }
    // 根のプリミティブの場合はノードの変数をコピーする．
    DtpgPrimitive* prim = node->primitive(n1);
    VarId gvar = node->gvar();
    prim->set_gvar(gvar);
    prim->set_fvar(gvar);
  }
}

// @brief ノードに正常回路用の変数を設定する．
// @param[in] solver SAT ソルバー
// @param[in] node 対象のノード
void
set_fvar(SatSolver& solver,
	 DtpgNode* node)
{
  // ノードそのものに割り当てる．
  VarId fvar = solver.new_var();
  VarId dvar = solver.new_var();
  node->set_fvar(fvar, dvar);
}

// @brief ノードに正常回路用の変数を設定する．
// @param[in] solver SAT ソルバー
// @param[in] node 対象のノード
void
set_fvar2(SatSolver& solver,
	  DtpgNode* node)
{
  // ノード内部のプリミティブに割り当てる．
  ymuint n = node->primitive_num();
  if ( n > 0 ) {
    ymuint n1 = n - 1;
    for (ymuint i = 0; i < n1; ++ i) {
      DtpgPrimitive* prim = node->primitive(i);
      if ( prim->is_input() ) {
	ymuint ipos = prim->input_id();
	DtpgNode* inode = node->fanin(ipos);
	prim->set_fvar(inode->fvar());
      }
      else {
	VarId fvar = solver.new_var();
	prim->set_fvar(fvar);
      }
    }
    // 根のプリミティブの場合はノードの変数をコピーする．
    DtpgPrimitive* prim = node->primitive(n1);
    prim->set_fvar(node->fvar());
  }
}

// @brief ゲートの入出力関係を表す CNF を作る．
// @param[in] solver SAT ソルバー
// @param[in] gate_type ゲートタイプ
// @param[in] output 出力のリテラル
// @param[in] inputs 入力のリテラル
void
make_gate_cnf(SatSolver& solver,
	      tTgGateType gate_type,
	      Literal output,
	      const vector<Literal>& inputs)
{
  ymuint ni = inputs.size();
  switch ( gate_type ) {
  case kTgGateBuff:
    solver.add_clause( inputs[0], ~output);
    solver.add_clause(~inputs[0],  output);
    break;

  case kTgGateNot:
    solver.add_clause( inputs[0],  output);
    solver.add_clause(~inputs[0], ~output);
    break;

  case kTgGateAnd:
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

  case kTgGateNand:
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

  case kTgGateOr:
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

  case kTgGateNor:
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

  case kTgGateXor:
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

  case kTgGateXnor:
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

// @brief 正常回路におけるノードの入出力の関係を表す CNF を作る．
void
make_gnode_cnf(SatSolver& solver,
	       DtpgNode* node)
{
  if ( node->is_input() ) {
    return;
  }

  Literal output(node->gvar(), kPolPosi);

  if ( node->is_output() ) {
    DtpgNode* inode = node->fanin(0);
    Literal input(inode->gvar(), kPolPosi);

    solver.add_clause( input, ~output);
    solver.add_clause(~input,  output);
    return;
  }

  set_gvar2(solver, node);
  if ( node->is_cplx_logic() ) {
    ymuint n = node->primitive_num();
    for (ymuint i = 0; i < n; ++ i) {
      DtpgPrimitive* prim = node->primitive(i);
      if ( !prim->is_input() ) {
	Literal output(prim->gvar(), kPolPosi);
	ymuint ni = prim->fanin_num();
	vector<Literal> inputs(ni);
	for (ymuint j = 0; j < ni; ++ j) {
	  DtpgPrimitive* iprim = prim->fanin(j);
	  inputs[j] = Literal(iprim->gvar(), kPolPosi);
	}
	make_gate_cnf(solver, prim->gate_type(), output, inputs);
      }
    }
  }
  else {
    ymuint ni = node->fanin_num();
    vector<Literal> inputs(ni);
    for (ymuint i = 0; i < ni; ++ i) {
      DtpgNode* inode = node->fanin(i);
      inputs[i] = Literal(inode->gvar(), kPolPosi);
    }
    make_gate_cnf(solver, node->gate_type(), output, inputs);
  }
}

// @brief 故障回路におけるノードの入出力の関係を表す CNF を作る．
void
make_fnode_cnf(SatSolver& solver,
	       DtpgNode* node)
{
  if ( node->is_input() ) {
    return;
  }

  Literal output(node->fvar(), kPolPosi);

  ymuint ni = node->fanin_num();
  vector<Literal> inputs(ni);

  // dlit が 1 の時，入力のノードの dlit も1でなければならない．
  vector<Literal> dep;
  dep.reserve(ni + 1);

  Literal dlit(node->dvar(), kPolPosi);
  dep.push_back(~dlit);

  for (ymuint i = 0; i < ni; ++ i) {
    DtpgNode* inode = node->fanin(i);
    inputs[i] = Literal(inode->fvar(), kPolPosi);
    if ( inode->has_fvar() ) {
      dep.push_back(Literal(inode->dvar(), kPolPosi));
    }
  }

  set_fvar2(solver, node);
  if ( node->is_output() ) {
    solver.add_clause( inputs[0], ~output);
    solver.add_clause(~inputs[0],  output);
  }
  else {
    if ( node->is_cplx_logic() ) {
      ymuint n = node->primitive_num();
      for (ymuint i = 0; i < n; ++ i) {
	DtpgPrimitive* prim = node->primitive(i);
	if ( !prim->is_input() ) {
	  Literal output(prim->fvar(), kPolPosi);
	  ymuint ni = prim->fanin_num();
	  vector<Literal> inputs(ni);
	  for (ymuint j = 0; j < ni; ++ j) {
	    DtpgPrimitive* iprim = prim->fanin(j);
	    inputs[j] = Literal(iprim->fvar(), kPolPosi);
	  }
	  make_gate_cnf(solver, prim->gate_type(), output, inputs);
	}
      }
    }
    else {
      make_gate_cnf(solver, node->gate_type(), output, inputs);
    }
  }

  solver.add_clause(dep);
}

void
mark_tfo(DtpgNode* node)
{
  if ( node->mark1() ) {
    return;
  }
  node->set_mark1();

  ymuint nfo = node->active_fanout_num();
  for (ymuint i = 0; i < nfo; ++ i) {
    mark_tfo(node->active_fanout(i));
  }
}

void
clear_tfo(DtpgNode* node)
{
  if ( !node->mark1() ) {
    return;
  }
  node->clear_mark1();

  ymuint nfo = node->active_fanout_num();
  for (ymuint i = 0; i < nfo; ++ i) {
    clear_tfo(node->active_fanout(i));
  }
}

END_NONAMESPACE


// @brief コンストラクタ
DtpgSat::DtpgSat()
{
  mGetPatFlag = 0;
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

// @brief get_pat フラグを設定する．
void
DtpgSat::set_get_pat(ymuint val)
{
  mGetPatFlag = val;
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
      dtpg_single(f0, op);
    }

    DtpgFault* f1 = node->output_fault(1);
    if ( f1 != NULL && !f1->is_skip() ) {
      dtpg_single(f1, op);
    }

    // 入力の故障
    ymuint ni = node->fanin_num();
    for (ymuint j = 0; j < ni; ++ j) {
      DtpgFault* f0 = node->input_fault(0, j);
      if ( f0 != NULL && !f0->is_skip() ) {
	dtpg_single(f0, op);
      }

      DtpgFault* f1 = node->input_fault(1, j);
      if ( f1 != NULL && !f1->is_skip() ) {
	dtpg_single(f1, op);
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
      dtpg_dual(f0, f1, op);
    }
    else if ( f0 != NULL ) {
      dtpg_single(f0, op);
    }
    else if ( f1 != NULL ) {
      dtpg_single(f1, op);
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
	dtpg_dual(f0, f1, op);
      }
      else if ( f0 != NULL ) {
	dtpg_single(f0, op);
      }
      else if ( f1 != NULL ) {
	dtpg_single(f1, op);
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

  dtpg_ffr(flist, onode, node_list, op);
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

  dtpg_ffr(flist, ffr->root(), node_list, op);
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
}

// @brief 一つの故障に対してテストパタン生成を行う．
// @param[in] f 故障
// @param[in] op テスト生成の結果を処理するファンクター
void
DtpgSat::dtpg_single(DtpgFault* f,
		     DtpgOperator& op)
{
  SatSolver solver(mType, mOption, mOutP);

  DtpgNode* fnode = f->node();

  // fnode から外部出力へ至る部分の CNF を作る．
  make_prop_cnf(solver, fnode);

  DtpgNode* fsrc = fnode;
  if ( f->is_input_fault() ) {
    ymuint ipos = f->pos();
    DtpgNode* inode = fnode->fanin(ipos);
    set_fvar(solver, inode);
    make_fnode_cnf(solver, fnode);
    fsrc = fnode->fanin(ipos);
  }

  ymuint nim = 0;
  for (DtpgNode* node = fnode; node != NULL; node = node->imm_dom()) {
    ++ nim;
  }

  mAssumptions.clear();
  mAssumptions.resize(nim + 2);

  if ( f->val() ) {
    mAssumptions[0] = Literal(fsrc->gvar(), kPolNega);
    mAssumptions[1] = Literal(fsrc->fvar(), kPolPosi);
  }
  else {
    mAssumptions[0] = Literal(fsrc->gvar(), kPolPosi);
    mAssumptions[1] = Literal(fsrc->fvar(), kPolNega);
  }

  // dominator ノードの dvar は1でなければならない．
  ymuint i = 2;
  for (DtpgNode* node = fnode; node != NULL; node = node->imm_dom(), ++ i) {
    mAssumptions[i] = Literal(node->dvar(), kPolPosi);
  }

  solve(solver, f, op);

  clear_node_mark();

  update_stats(solver, 1);
}

// @brief 同じ位置の2つの出力故障に対してテストパタン生成を行なう．
// @param[in] f0 0縮退故障
// @param[in] f1 1縮退故障
// @param[in] op テスト生成の結果を処理するファンクター
void
DtpgSat::dtpg_dual(DtpgFault* f0,
		   DtpgFault* f1,
		   DtpgOperator& op)
{
  SatSolver solver(mType, mOption, mOutP);

  DtpgNode* fnode = f0->node();

  // fnode から外部出力へ至る部分の CNF を作る．
  make_prop_cnf(solver, fnode);

  DtpgNode* fsrc = fnode;
  if ( f0->is_input_fault() ) {
    ymuint ipos = f0->pos();
    DtpgNode* inode = fnode->fanin(ipos);
    set_fvar(solver, inode);
    make_fnode_cnf(solver, fnode);
    fsrc = f0->source_node();
  }

  ymuint nim = 0;
  for (DtpgNode* node = fnode; node != NULL; node = node->imm_dom()) {
    ++ nim;
  }

  mAssumptions.clear();
  mAssumptions.reserve(nim + 2);

  mAssumptions[0] = Literal(fsrc->gvar(), kPolPosi);
  mAssumptions[1] = Literal(fsrc->fvar(), kPolNega);

  // dominator ノードの dvar は1でなければならない．
  ymuint i = 2;
  for (DtpgNode* node = fnode; node != NULL; node = node->imm_dom(), ++ i) {
    mAssumptions[i] = Literal(node->dvar(), kPolPosi);
  }

  solve(solver, f0, op);

  mAssumptions[0] = Literal(fsrc->gvar(), kPolNega);
  mAssumptions[1] = Literal(fsrc->fvar(), kPolPosi);

  solve(solver, f1, op);

  clear_node_mark();

  update_stats(solver, 2);
}

// @brief FFR 内の故障に対してテストパタン生成を行なう．
// @param[in] flist 故障リスト
// @param[in] root FFR の根のノード
// @param[in] node_list FFR 内のノードリスト
// @param[in] op テスト生成の結果を処理するファンクター
void
DtpgSat::dtpg_ffr(const vector<DtpgFault*>& flist,
		  DtpgNode* root,
		  const vector<DtpgNode*>& node_list,
		  DtpgOperator& op)
{
  SatSolver solver(mType, mOption, mOutP);

  // root から外部出力までの正常回路，故障回路を作る．
  make_prop_cnf(solver, root);

  ymuint nf = flist.size();
  for (ymuint i = 0; i < nf; ++ i) {
    DtpgFault* f = flist[i];
    f->set_tmp_id(i);
    f->node()->set_mark1();
  }
  for (vector<DtpgNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    DtpgNode* node = *p;
    if ( node->mark1() ) {
      continue;
    }
    bool found = false;
    ymuint ni = node->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      DtpgNode* inode = node->fanin(i);
      if ( inode->mark1() ) {
	found = true;
	break;
      }
    }
    if ( found ) {
      node->set_mark1();
    }
  }

  // 故障回路用の変数を割り当てる．
  for (vector<DtpgNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    DtpgNode* node = *p;
    if ( (node != root) && node->mark1() ) {
      set_fvar(solver, node);
    }
    node->clear_mark1();
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
    if ( !node->has_fvar() ) {
      continue;
    }

    ymuint ni = node->fanin_num();
    vector<Literal> inputs(ni);
    hash_map<ymuint, ymuint> fanin_map;
    for (ymuint i = 0; i < ni; ++ i) {
      DtpgNode* inode = node->fanin(i);
      VarId fvar = inode->fvar();
      for (ymint val = 0; val < 2; ++ val) {
	DtpgFault* f = node->input_fault(val, i);
	if ( f == NULL ) {
	  continue;
	}
	ymuint fid = f->tmp_id();
	if ( flist[fid] == f ) {
	  make_flt_cnf(solver, fvar, flt_var[fid], tmp_var[fid], val);
	  fvar = tmp_var[fid];
	}
      }
      inputs[i] = Literal(fvar, kPolPosi);
      fanin_map.insert(make_pair(inode->id(), i));
    }

    VarId tmp_ovar = node->fvar();
    for (ymint val = 0; val < 2; ++ val) {
      DtpgFault* f = node->output_fault(val);
      if ( f == NULL ) {
	continue;
      }
      ymuint fid = f->tmp_id();
      if ( flist[fid] == f ) {
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
      if ( node->is_output() ) {
	solver.add_clause( inputs[0], ~olit);
	solver.add_clause(~inputs[0],  olit);
      }
      else {
	if ( node->is_cplx_logic() ) {
	  ymuint n = node->primitive_num();
	  ymuint n1 = n - 1;
	  for (ymuint i = 0; i < n; ++ i) {
	    DtpgPrimitive* prim = node->primitive(i);
	    if ( !prim->is_input() ) {
	      ymuint ni1 = prim->fanin_num();
	      vector<Literal> inputs1(ni1);
	      for (ymuint j = 0; j < ni1; ++ j) {
		DtpgPrimitive* iprim = prim->fanin(j);
		if ( iprim->is_input() ) {
		  ymuint ipos = iprim->input_id();
		  inputs1[j] = inputs[ipos];
		}
		else {
		  inputs1[j] = Literal(iprim->fvar(), kPolPosi);
		}
	      }
	      Literal output;
	      if ( i == n1 ) {
		output = olit;
	      }
	      else {
		output = Literal(prim->fvar(), kPolPosi);
	      }
	      make_gate_cnf(solver, prim->gate_type(), output, inputs1);
	    }
	  }
	}
	else {
	  make_gate_cnf(solver, node->gate_type(), olit, inputs);
	}
      }

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
	if ( inode->has_fvar() ) {
	  dep.push_back(Literal(inode->dvar(), kPolPosi));
	}
	DtpgFault* fi0 = node->input_fault(0, j);
	if ( fi0 != NULL ) {
	  ymuint fid = fi0->tmp_id();
	  if ( flist[fid] == fi0 ) {
	    dep.push_back(Literal(flt_var[fid], kPolPosi));
	  }
	}

	DtpgFault* fi1 = node->input_fault(1, j);
	if ( fi1 != NULL ) {
	  ymuint fid = fi1->tmp_id();
	  if ( flist[fid] == fi1 ) {
	    dep.push_back(Literal(flt_var[fid], kPolPosi));
	  }
	}
      }

      DtpgFault* fo0 = node->output_fault(0);
      if ( fo0 != NULL ) {
	ymuint fid = fo0->tmp_id();
	if ( flist[fid] == fo0 ) {
	  dep.push_back(Literal(flt_var[fid], kPolPosi));
	}
      }

      DtpgFault* fo1 = node->output_fault(1);
      if ( fo1 != NULL ) {
	ymuint fid = fo1->tmp_id();
	if ( flist[fid] == fo1 ) {
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
    mAssumptions.clear();
    mAssumptions.reserve(node_list.size() + nf);

    // 該当の故障に対する変数のみ1にする．
    for (ymuint j = 0; j < nf; ++ j) {
      tPol pol = (j == i) ? kPolPosi : kPolNega;
      mAssumptions.push_back(Literal(flt_var[j], pol));
    }

    // 故障ノードの TFO 以外の dlit を0にする．
    mark_tfo(f->node());
    for (vector<DtpgNode*>::const_iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      DtpgNode* node = *p;
      if ( node->has_fvar() && !node->mark1() ) {
	Literal dlit(node->dvar(), kPolNega);
	mAssumptions.push_back(dlit);
      }
    }
    clear_tfo(f->node());

    // dominator ノードの dvar は1でなければならない．
    for (DtpgNode* node = f->node(); node != NULL; node = node->imm_dom()) {
      Literal dlit(node->dvar(), kPolPosi);
      mAssumptions.push_back(dlit);
    }

    DtpgNode* fnode = f->node();
    if ( f->is_input_fault() ) {
      fnode = f->source_node();
    }
    tPol pol = (f->val() == 0) ? kPolPosi : kPolNega;
    mAssumptions.push_back(Literal(fnode->gvar(), pol));

    solve(solver, f, op);
  }

  clear_node_mark();

  update_stats(solver, nf);
}

// @brief fnode の故障が伝搬する条件を表す CNF を作る．
void
DtpgSat::make_prop_cnf(SatSolver& solver,
		       DtpgNode* fnode)
{
  vector<DtpgNode*> tfo_list;
  vector<DtpgNode*> tfi_list;
  mNetwork->mark_tfo_tfi(fnode, tfo_list, tfi_list);

  // 以降は tfo_list か tfi_list に含まれるノードのみを対象とする．

  // TFO マークのついたノード用の変数の生成 (glit, flit, dlit の3つを作る)
  for (vector<DtpgNode*>::iterator p = tfo_list.begin();
       p != tfo_list.end(); ++ p) {
    DtpgNode* node = *p;
    set_gvar(solver, node);
    set_fvar(solver, node);
    mUsedNodeList.push_back(node);
  }
  // TFI マークのついたノード用の変数の生成 (glit のみを作る)
  for (vector<DtpgNode*>::iterator p = tfi_list.begin();
       p != tfi_list.end(); ++ p) {
    DtpgNode* node = *p;
    set_gvar(solver, node);
    mUsedNodeList.push_back(node);
  }

  // mInputList を作る．
  mInputList.clear();
  if ( fnode->is_input() ) {
    mInputList.push_back(fnode);
  }
  for (vector<DtpgNode*>::iterator p = tfi_list.begin();
       p != tfi_list.end(); ++ p) {
    DtpgNode* node = *p;
    if ( node->is_input() ) {
      mInputList.push_back(node);
    }
  }

  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////

  for (vector<DtpgNode*>::const_iterator p = tfi_list.begin();
       p != tfi_list.end(); ++ p) {
    DtpgNode* node = *p;
    make_gnode_cnf(solver, node);
  }
  for (vector<DtpgNode*>::const_iterator p = tfo_list.begin();
       p != tfo_list.end(); ++ p) {
    DtpgNode* node = *p;
    make_gnode_cnf(solver, node);
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

    make_fnode_cnf(solver, node);
  }

  // 出力のうち，最低1つは故障差が伝搬しなければならない．
  assert_cond( !odiff.empty(), __FILE__, __LINE__);

  solver.add_clause(odiff);
}

/// @brief 一つの SAT問題を解く．
void
DtpgSat::solve(SatSolver& solver,
	       DtpgFault* f,
	       DtpgOperator& op)
{
#if 1
  Bool3 ans = solver.solve(mAssumptions, mModel);
  if ( ans == kB3True ) {
    f->set_skip();

    if ( mGetPatFlag == 1 ) {
      get_pat(f->node());
    }
    else if ( mGetPatFlag == 2 ) {
      get_pat2(f->node());
    }
    else {
      // フォールバック
      // TFI に含まれる全ての外部入力の値を記録する．
      mValList.clear();
      mValList.reserve(mInputList.size());
      for (vector<DtpgNode*>::const_iterator p = mInputList.begin();
	   p != mInputList.end(); ++ p) {
	DtpgNode* node = *p;
	record_value(node);
      }
    }

    op.set_detected(f->safault(), mValList);
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

// @brief テストパタンを求める．
// @note 結果は mValList に格納される．
void
DtpgSat::get_pat(DtpgNode* fnode)
{
  mValList.clear();
  mDiffNodeList.clear();
  mBwdNodeList.clear();

  // 故障差の伝搬経路にマークをつける．
  fwd_dfs(fnode);

  // 故障差を伝搬させるためのサイド入力の値を正当化する．
  for (vector<DtpgNode*>::iterator p = mDiffNodeList.begin();
       p != mDiffNodeList.end(); ++ p) {
    DtpgNode* node = *p;
    if ( node->is_input() ) {
      record_value(node);
    }
    else {
      ymuint ni = node->fanin_num();
      for (ymuint i = 0; i < ni; ++ i) {
	DtpgNode* inode = node->fanin(i);
	if ( !inode->mark2() ) {
	  justify(inode);
	}
      }
    }
  }

  // 一連の処理でつけたマークを消す．
  for (vector<DtpgNode*>::iterator p = mBwdNodeList.begin();
       p != mBwdNodeList.end(); ++ p) {
    DtpgNode* node = *p;
    node->clear_mark1();
    node->clear_mark2();
    node->clear_mark3();
  }
}

// @brief テストパタンを求める．
// @note 結果は mValList に格納される．
void
DtpgSat::get_pat2(DtpgNode* fnode)
{
  mValList.clear();
  mDiffNodeList.clear();
  mBwdNodeList.clear();

  // 故障差の伝搬経路にマークをつける．
  fwd_dfs(fnode);

  // 故障差を伝搬させるためのサイド入力の値を正当化する．
  for (vector<DtpgNode*>::iterator p = mDiffNodeList.begin();
       p != mDiffNodeList.end(); ++ p) {
    DtpgNode* node = *p;
    if ( node->is_input() ) {
      record_value(node);
    }
    else {
      ymuint ni = node->fanin_num();
      for (ymuint i = 0; i < ni; ++ i) {
	DtpgNode* inode = node->fanin(i);
	if ( !inode->mark2() ) {
	  justify(inode);
	}
      }
    }
  }

  // 一連の処理でつけたマークを消す．
  for (vector<DtpgNode*>::iterator p = mBwdNodeList.begin();
       p != mBwdNodeList.end(); ++ p) {
    DtpgNode* node = *p;
    node->clear_mark1();
    node->clear_mark2();
    node->clear_mark3();
  }
}

// @brief solve 中で故障差を持つノードをたどる．
// @param[in] node 対象のノード
// @retval true node を通って外部出力まで故障差が伝搬している．
// @retval false 故障差が伝搬していない．
// @note 故障差の伝搬経路上のノードは mDiffNodeList に格納される．
// @note 一旦調べたノードはすべて mark1 がつく．
// @note 故障差が伝搬しているノードは mark2 がつく．
// @note マークがついたノードは mBwdNodeList に格納される．
bool
DtpgSat::fwd_dfs(DtpgNode* node)
{
  if ( node->mark1() ) {
    return node->mark2();
  }
  node->set_mark1();
  mBwdNodeList.push_back(node);

  VarId gvar = node->gvar();
  VarId fvar = node->fvar();
  if ( mModel[gvar.val()] == mModel[fvar.val()] ) {
    // 故障差が伝搬していない．
    return false;
  }

  bool stat = true;
  ymuint nfo = node->active_fanout_num();
  for (ymuint i = 0; i < nfo; ++ i) {
    DtpgNode* onode = node->active_fanout(i);
    bool stat1 = fwd_dfs(onode);
    if ( stat1 ) {
      stat = true;
    }
  }
  if ( stat ) {
    node->set_mark2();
    mDiffNodeList.push_back(node);
  }
  return stat;
}

// @brief solve 中で変数割り当ての正当化を行なう．
// @param[in] node 対象のノード
// @note node の値割り当てを正当化する．
// @note 正当化に用いられているノードには mark3 がつく．
// @note mark3 がついたノードは mBwdNodeList に格納される．
void
DtpgSat::justify(DtpgNode* node)
{
  if ( node->mark3() ) {
    return;
  }
  node->set_mark3();
  mBwdNodeList.push_back(node);

  if ( node->is_input() ) {
    // val を記録
    record_value(node);
    return;
  }

  if ( node->is_cplx_logic() ) {
    ymuint np = node->primitive_num();
    DtpgPrimitive* prim = node->primitive(np - 1);
    justify_primitive(prim, node);
  }
  else {
    Bool3 val = mModel[node->gvar().val()];
    ymuint ni = node->fanin_num();
    switch ( node->gate_type() ) {
    case kTgGateBuff:
    case kTgGateNot:
      // 無条件で唯一のファンインをたどる．
      justify(node->fanin(0));
      break;

    case kTgGateAnd:
      if ( val == kB3True ) {
	// 故障差が伝搬していないノードをすべてたどる．
	for (ymuint i = 0; i < ni; ++ i) {
	  DtpgNode* inode = node->fanin(i);
	  if ( !inode->mark2() ) {
	    justify(inode);
	  }
	}
      }
      else if ( val == kB3False ) {
	// 故障差が伝搬していない0の値を持つ最初のノードをたどる．
	// ただし，既にたどっているノードがあればなにもしない．
	DtpgNode* first_node = NULL;
	bool found = false;
	for (ymuint i = 0; i < ni; ++ i) {
	  DtpgNode* inode = node->fanin(i);
	  if ( !inode->mark2() && mModel[inode->gvar().val()] == kB3False ) {
	    if ( inode->mark3() ) {
	      found = true;
	      break;
	    }
	    if ( first_node == NULL ) {
	      first_node = inode;
	    }
	  }
	}
	if ( !found && first_node != NULL ) {
	  justify(first_node);
	}
      }
      break;

    case kTgGateNand:
      if ( val == kB3True ) {
	// 故障差が伝搬していない0の値を持つ最初のノードをたどる．
	// ただし，既にたどっているノードがあればなにもしない．
	DtpgNode* first_node = NULL;
	bool found = false;
	for (ymuint i = 0; i < ni; ++ i) {
	  DtpgNode* inode = node->fanin(i);
	  if ( !inode->mark2() && mModel[inode->gvar().val()] == kB3False ) {
	    if ( inode->mark3() ) {
	      found = true;
	      break;
	    }
	    if ( first_node == NULL ) {
	      first_node = inode;
	    }
	  }
	}
	if ( !found && first_node != NULL ) {
	  justify(first_node);
	}
      }
      else if ( val == kB3False ) {
	// 故障差が伝搬していないノードをすべてたどる．
	for (ymuint i = 0; i < ni; ++ i) {
	  DtpgNode* inode = node->fanin(i);
	  if ( !inode->mark2() ) {
	    justify(inode);
	  }
	}
      }
      break;

    case kTgGateOr:
      if ( val == kB3True ) {
	// 故障差が伝搬していない1の値を持つ最初のノードをたどる．
	// ただし，既にたどっているノードがあればなにもしない．
	DtpgNode* first_node = NULL;
	bool found = false;
	for (ymuint i = 0; i < ni; ++ i) {
	  DtpgNode* inode = node->fanin(i);
	  if ( !inode->mark2() && mModel[inode->gvar().val()] == kB3True ) {
	    if ( inode->mark3() ) {
	      found = true;
	      break;
	    }
	    if ( first_node == NULL ) {
	      first_node = inode;
	    }
	  }
	}
	if ( !found && first_node != NULL ) {
	  justify(first_node);
	}
      }
      else if ( val == kB3False ) {
	// 故障差が伝搬していないノードをすべてたどる．
	for (ymuint i = 0; i < ni; ++ i) {
	  DtpgNode* inode = node->fanin(i);
	  if ( !inode->mark2() ) {
	    justify(inode);
	  }
	}
      }
      break;

    case kTgGateNor:
      if ( val == kB3True ) {
	// 故障差が伝搬していないノードをすべてたどる．
	for (ymuint i = 0; i < ni; ++ i) {
	  DtpgNode* inode = node->fanin(i);
	  if ( !inode->mark2() ) {
	    justify(inode);
	  }
	}
      }
      else if ( val == kB3False ) {
	// 故障差が伝搬していない1の値を持つ最初のノードをたどる．
	// ただし，既にたどっているノードがあればなにもしない．
	DtpgNode* first_node = NULL;
	bool found = false;
	for (ymuint i = 0; i < ni; ++ i) {
	  DtpgNode* inode = node->fanin(i);
	  if ( !inode->mark2() && mModel[inode->gvar().val()] == kB3True ) {
	    if ( inode->mark3() ) {
	      found = true;
	      break;
	    }
	    if ( first_node == NULL ) {
	      first_node = inode;
	    }
	  }
	}
	if ( !found && first_node != NULL ) {
	  justify(first_node);
	}
      }
      break;

    case kTgGateXor:
    case kTgGateXnor:
      // 故障差が伝搬していないノードをすべてたどる．
      for (ymuint i = 0; i < ni; ++ i) {
	DtpgNode* inode = node->fanin(i);
	if ( !inode->mark2() ) {
	  justify(node);
	}
      }
      break;

    default:
      assert_not_reached(__FILE__, __LINE__);
      break;
    }
  }
}

// @brief justify の下請け関数
// @param[in] prim 対象のプリミティブ
// @param[in] node 対象のノード
// @note node の値割り当てを正当化する．
// @note 正当化に用いられているノードには mark3 がつく．
// @note mark3 がついたノードは mBwdNodeList に格納される．
void
DtpgSat::justify_primitive(DtpgPrimitive* prim,
			   DtpgNode* node)
{
  if ( prim->is_input() ) {
    ymuint ipos = prim->input_id();
    DtpgNode* inode = node->fanin(ipos);
    justify(inode);
  }
  else {
    Bool3 val = mModel[prim->gvar().val()];
    ymuint ni = prim->fanin_num();
    switch ( prim->gate_type() ) {
    case kTgGateBuff:
    case kTgGateNot:
      justify_primitive(prim->fanin(0), node);
      break;

    case kTgGateAnd:
      if ( val == kB3True ) {
	for (ymuint i = 0; i < ni; ++ i) {
	  DtpgPrimitive* iprim = prim->fanin(i);
	  Bool3 igval = mModel[iprim->gvar().val()];
	  Bool3 ifval = mModel[iprim->fvar().val()];
	  if ( igval == ifval ) {
	    justify_primitive(iprim, node);
	  }
	}
      }
      else if ( val == kB3False ) {
	for (ymuint i = 0; i < ni; ++ i) {
	  DtpgPrimitive* iprim = prim->fanin(i);
	  Bool3 igval = mModel[iprim->gvar().val()];
	  Bool3 ifval = mModel[iprim->fvar().val()];
	  if ( igval == kB3False && ifval == kB3False ) {
	    justify_primitive(iprim, node);
	    break;
	  }
	}
      }
      break;

    case kTgGateNand:
      if ( val == kB3True ) {
	for (ymuint i = 0; i < ni; ++ i) {
	  DtpgPrimitive* iprim = prim->fanin(i);
	  Bool3 igval = mModel[iprim->gvar().val()];
	  Bool3 ifval = mModel[iprim->fvar().val()];
	  if ( igval == kB3False && ifval == kB3False ) {
	    justify_primitive(iprim, node);
	    break;
	  }
	}
      }
      else if ( val == kB3False ) {
	for (ymuint i = 0; i < ni; ++ i) {
	  DtpgPrimitive* iprim = prim->fanin(i);
	  Bool3 igval = mModel[iprim->gvar().val()];
	  Bool3 ifval = mModel[iprim->fvar().val()];
	  if ( igval == ifval ) {
	    justify_primitive(iprim, node);
	  }
	}
      }
      break;

    case kTgGateOr:
      if ( val == kB3True ) {
	for (ymuint i = 0; i < ni; ++ i) {
	  DtpgPrimitive* iprim = prim->fanin(i);
	  Bool3 igval = mModel[iprim->gvar().val()];
	  Bool3 ifval = mModel[iprim->fvar().val()];
	  if ( igval == kB3True && ifval == kB3True ) {
	    justify_primitive(iprim, node);
	    break;
	  }
	}
      }
      else if ( val == kB3False ) {
	for (ymuint i = 0; i < ni; ++ i) {
	  DtpgPrimitive* iprim = prim->fanin(i);
	  Bool3 igval = mModel[iprim->gvar().val()];
	  Bool3 ifval = mModel[iprim->fvar().val()];
	  if ( igval == ifval ) {
	    justify_primitive(iprim, node);
	  }
	}
      }
      break;

    case kTgGateNor:
      if ( val == kB3True ) {
	for (ymuint i = 0; i < ni; ++ i) {
	  DtpgPrimitive* iprim = prim->fanin(i);
	  Bool3 igval = mModel[iprim->gvar().val()];
	  Bool3 ifval = mModel[iprim->fvar().val()];
	  if ( igval == ifval ) {
	    justify_primitive(iprim, node);
	  }
	}
      }
      else if ( val == kB3False ) {
	for (ymuint i = 0; i < ni; ++ i) {
	  DtpgPrimitive* iprim = prim->fanin(i);
	  Bool3 igval = mModel[iprim->gvar().val()];
	  Bool3 ifval = mModel[iprim->fvar().val()];
	  if ( igval == kB3True && ifval == kB3True ) {
	    justify_primitive(iprim, node);
	    break;
	  }
	}
      }
      break;

    case kTgGateXor:
    case kTgGateXnor:
      for (ymuint i = 0; i < ni; ++ i) {
	DtpgPrimitive* iprim = prim->fanin(i);
	Bool3 igval = mModel[iprim->gvar().val()];
	Bool3 ifval = mModel[iprim->fvar().val()];
	if ( igval == ifval ) {
	  justify_primitive(iprim, node);
	}
      }
      break;

    default:
      assert_not_reached(__FILE__, __LINE__);
      break;
    }
  }
}

// @brief 入力ノードの値を記録する．
// @param[in] node 対象の外部入力ノード
// @note node の値を mValList に記録する．
// @note 単純だが mModel 上のインデックスと mValList の符号化は異なる．
void
DtpgSat::record_value(DtpgNode* node)
{
  assert_cond( node->is_input(), __FILE__, __LINE__);

  VarId idx = node->gvar();
  ymuint iid = node->input_id();

  // 今のところ model には 0 か 1 しか設定されていないはず．
  Bool3 v = mModel[idx.val()];
  ymuint packed_val = iid * 2;
  if ( v == kB3True ) {
    packed_val += 1;
  }
  mValList.push_back(packed_val);
}

// @brief ノードの変数割り当てフラグを消す．
void
DtpgSat::clear_node_mark()
{
  for (vector<DtpgNode*>::iterator p = mUsedNodeList.begin();
       p != mUsedNodeList.end(); ++ p) {
    DtpgNode* node = *p;
    node->clear_var();
  }
  mUsedNodeList.clear();
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
	 << "Ave. # of implications:        " << (double) mPropagationNum / mSatCount << endl;
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
}

END_NAMESPACE_YM_SATPG_DTPG
