
/// @file atpg/src/dtpg/DtpgSat.cc
/// @brief DtpgSat の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSat.h"
#include "DtpgNetwork.h"
#include "DtpgNode.h"
#include "DtpgPrimitive.h"
#include "DtpgFFR.h"
#include "DtpgFault.h"
#include "DtpgOperator.h"
#include "InputLiteral.h"
#include "ym_logic/SatSolver.h"
#include "ym_logic/SatStats.h"


#define VERIFY_MAIMP 0


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
set_fvar(SatSolver& solver,
	 DtpgNode* node)
{
  // ノードそのものに割り当てる．
  VarId fvar = solver.new_var();
  VarId dvar = solver.new_var();
  node->set_fvar(fvar, dvar);
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
	      const InputLiteral& inputs)
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

  if ( node->is_cplx_logic() ) {
    // 複数のプリミティブで構成されたノードの場合
    ymuint n = node->primitive_num();
    for (ymuint i = 0; i < n; ++ i) {
      DtpgPrimitive* prim = node->primitive(i);
      VarId gvar;
      if ( prim->is_input() ) {
	// 入力プリミティブの場合
	// 対応する DtpgNode の変数を持ってくる．
	ymuint ipos = prim->input_id();
	DtpgNode* inode = node->fanin(ipos);
	gvar = inode->gvar();
      }
      else {
	if ( i == n - 1 ) {
	  // 根のプリミティブの場合
	  // node の変数を使う．
	  gvar = node->gvar();
	}
	else {
	  // それ以外の場合
	  // 新たな変数を割り当てる．
	  gvar = solver.new_var();
	}

	// プリミティブの入出力の関係を表す CNF 式を作る．
	Literal output(gvar, kPolPosi);
	make_gate_cnf(solver, prim->gate_type(), output, PrimGvarInputLiteral(prim));
      }
      // prim の変数 gvar と fvar の両方に登録しておく
      prim->set_gvar(gvar);
      prim->set_fvar(gvar);
    }
  }
  else {
    // 単純な組み込み型の場合
    make_gate_cnf(solver, node->gate_type(), output, GvarInputLiteral(node));
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
  Literal dlit(node->dvar(), kPolPosi);

  if ( node->is_output() ) {
    DtpgNode* inode = node->fanin(0);
    Literal input(inode->fvar(), kPolPosi);
    solver.add_clause( input, ~output);
    solver.add_clause(~input,  output);

    // dlit 用の節の追加
    Literal idlit(inode->dvar(), kPolPosi);
    solver.add_clause(idlit, ~dlit);

    return;
  }

  ymuint ni = node->fanin_num();

  // dlit が 1 の時，入力のノードの dlit の最低1つは1でなければならない．
  vector<Literal> dep;
  dep.reserve(ni + 1);
  dep.push_back(~dlit);
  for (ymuint i = 0; i < ni; ++ i) {
    DtpgNode* inode = node->fanin(i);
    if ( inode->has_fvar() ) {
      dep.push_back(Literal(inode->dvar(), kPolPosi));
    }
  }
  solver.add_clause(dep);

  if ( node->is_cplx_logic() ) {
    // 複数のプリミティブで構成されたノードの場合
    ymuint n = node->primitive_num();
    for (ymuint i = 0; i < n; ++ i) {
      DtpgPrimitive* prim = node->primitive(i);
      VarId fvar;
      if ( prim->is_input() ) {
	// 入力プリミティブの場合
	// 対応する DtpgNode の fvar を用いる．
	ymuint ipos = prim->input_id();
	DtpgNode* inode = node->fanin(ipos);
	fvar = inode->fvar();
      }
      else {
	if ( i == n - 1 ) {
	  // 根のプリミティブの場合
	  // node の fvar を用いる．
	  fvar = node->fvar();
	}
	else {
	  // それ以外の場合
	  // 新たな変数を割り当てる．
	  fvar = solver.new_var();
	}

	// プリミティブの入出力の関係を表す CNF 式を作る．
	Literal output(fvar, kPolPosi);
	make_gate_cnf(solver, prim->gate_type(), output, PrimFvarInputLiteral(prim));
      }
      // プリミティブの変数を登録する．
      prim->set_fvar(fvar);
    }
  }
  else {
    make_gate_cnf(solver, node->gate_type(), output, FvarInputLiteral(node));
  }
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
  mDryRun = false;
  mTimerEnable = false;
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

// @brief dry-run フラグを設定する．
void
DtpgSat::set_dry_run(bool flag)
{
  mDryRun = flag;
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
      mNetwork->activate_po(po_pos);
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

// @brief テスト生成を行なう．
// @param[in] flist 対象の故障リスト
// @param[in] op テスト生成後に呼ばれるファンクター
void
DtpgSat::run(const vector<SaFault*>& flist,
	     DtpgOperator& op)
{
  ymuint nf = flist.size();
  vector<DtpgFault*> dtpg_flist;
  dtpg_flist.reserve(nf);
  for (ymuint i = 0; i < nf; ++ i) {
    SaFault* f = flist[i];
    if ( f != NULL ) {
      dtpg_flist.push_back(mNetwork->conv_fault(f));
    }
  }

  if ( dtpg_flist.empty() ) {
    return;
  }

  mNetwork->activate_all();
  dtpg_group(dtpg_flist, op);
}

// @brief テスト生成を行なう．
// @param[in] flist 対象の故障リスト
// @param[in] po_pos 外部出力番号
// @param[in] op テスト生成後に呼ばれるファンクター
void
DtpgSat::run(const vector<SaFault*>& flist,
	     ymuint po_pos,
	     DtpgOperator& op)
{
  mNetwork->activate_po(po_pos);

  ymuint nf = flist.size();
  vector<DtpgFault*> dtpg_flist;
  dtpg_flist.reserve(nf);
  for (ymuint i = 0; i < nf; ++ i) {
    SaFault* f = flist[i];
    if ( f != NULL ) {
      dtpg_flist.push_back(mNetwork->conv_fault(f));
    }
  }

  if ( dtpg_flist.empty() ) {
    return;
  }

  dtpg_group(dtpg_flist, op);
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

BEGIN_NONAMESPACE

void
add_faults(DtpgNode* node,
	   vector<DtpgFault*>& fault_list)
{
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    DtpgFault* f0 = node->input_fault(0, i);
    if ( f0 != NULL && !f0->is_skip() ) {
      fault_list.push_back(f0);
    }
    DtpgFault* f1 = node->input_fault(1, i);
    if ( f1 != NULL && !f1->is_skip() ) {
      fault_list.push_back(f1);
    }
  }
  DtpgFault* f0 = node->output_fault(0);
  if ( f0 != NULL && !f0->is_skip() ) {
    fault_list.push_back(f0);
  }
  DtpgFault* f1 = node->output_fault(1);
  if ( f1 != NULL && !f1->is_skip() ) {
    fault_list.push_back(f1);
  }
}

void
dfs_ffr(DtpgNode* node,
	vector<DtpgFault*>& fault_list)
{
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    DtpgNode* inode = node->fanin(i);
    if ( inode->active_fanout_num() == 1 ) {
      dfs_ffr(inode, fault_list);
    }
  }

  add_faults(node, fault_list);
}

END_NONAMESPACE

// @brief ffr モードの共通処理
void
DtpgSat::ffr_sub(DtpgOperator& op)
{
#if 0
  vector<DtpgFFR*> ffr_list;
  mNetwork->get_ffr_list(ffr_list);
  for (vector<DtpgFFR*>::const_iterator p = ffr_list.begin();
       p != ffr_list.end(); ++ p) {
    DtpgFFR* ffr = *p;
    ffr_mode(ffr, op);
    delete ffr;
  }
#else
  vector<DtpgFault*> tmp_list;
  ymuint n = mNetwork->active_node_num();
  for (ymuint i = 0; i < n; ++ i) {
    DtpgNode* node = mNetwork->active_node(i);
    if ( node->is_output() || node->active_fanout_num() > 1 ) {
      tmp_list.clear();
      dfs_ffr(node, tmp_list);
      if ( tmp_list.empty() ) {
	continue;
      }
      dtpg_group(tmp_list, op);
    }
  }
#endif
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
  for (ymuint i = 0; i < n; ++ i) {
    DtpgNode* node = mNetwork->active_node(i);
    add_faults(node, flist);
  }
  if ( flist.empty() ) {
    return;
  }
  dtpg_group(flist, op);
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
    add_faults(node, flist);
  }

  if ( flist.empty() ) {
    return;
  }
  dtpg_group(flist, op);
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

  mCnfCount = 0;
  mCnfTime.set(0.0, 0.0, 0.0);
  mDetCount = 0;
  mDetTime.set(0.0, 0.0, 0.0);
  mUndetCount = 0;
  mUndetTime.set(0.0, 0.0, 0.0);
  mAbortCount = 0;
  mAbortTime.set(0.0, 0.0, 0.0);
}


BEGIN_NONAMESPACE

string
f_str(DtpgFault* f)
{
  ostringstream buf;
  buf << "Node#" << f->node()->id() << ": ";
  if ( f->is_output_fault() ) {
    buf << "O";
  }
  else {
    buf << "I" << f->pos();
  }
  buf << ": SA" << f->val();
  return buf.str();
}

struct FaultGroup
{
  vector<DtpgFault*> mFaultList;

  vector<ymuint32> mMaList;

};

bool
check_conflict(const vector<ymuint32>& list_a,
	       const vector<ymuint32>& list_b)
{
  if ( list_a.empty() || list_b.empty() ) {
    return false;
  }

  vector<ymuint32>::const_iterator p_a = list_a.begin();
  vector<ymuint32>::const_iterator p_b = list_b.begin();
  ymuint32 tmp_a = *p_a;
  ymuint32 tmp_b = *p_b;
  ymuint idx_a = tmp_a / 2;
  ymuint idx_b = tmp_b / 2;
  for ( ; ; ) {
    if ( idx_a < idx_b ) {
      ++ p_a;
      if ( p_a == list_a.end() ) {
	return false;
      }
      tmp_a = *p_a;
      idx_a = tmp_a / 2;
    }
    else if ( idx_a > idx_b ) {
      ++ p_b;
      if ( p_b == list_b.end() ) {
	return false;
      }
      tmp_b = *p_b;
      idx_b = tmp_b / 2;
    }
    else {
      if ( tmp_a != tmp_b ) {
	// インデックスが同じで極性が異なっていた．
	return true;
      }
      ++ p_a;
      if ( p_a == list_a.end() ) {
	return false;
      }
      tmp_a = *p_a;
      idx_a = tmp_a / 2;

	++ p_b;
      if ( p_b == list_b.end() ) {
	return false;
      }
      tmp_b = *p_b;
      idx_b = tmp_b / 2;
    }
  }
}

void
merge(vector<ymuint32>& list_a,
      const vector<ymuint32>& list_b)
{
  if ( list_a.empty() ) {
    list_a = list_b;
    return;
  }
  if ( list_b.empty() ) {
    return;
  }

  vector<ymuint32> dst_list;

  vector<ymuint32>::const_iterator p_a = list_a.begin();
  vector<ymuint32>::const_iterator p_b = list_b.begin();
  ymuint32 tmp_a = *p_a;
  ymuint32 tmp_b = *p_b;
  ymuint idx_a = tmp_a / 2;
  ymuint idx_b = tmp_b / 2;
  for ( ; ; ) {
    if ( idx_a < idx_b ) {
      dst_list.push_back(tmp_a);
      ++ p_a;
      if ( p_a == list_a.end() ) {
	break;
      }
      tmp_a = *p_a;
      idx_a = tmp_a / 2;
    }
    else if ( idx_a > idx_b ) {
      dst_list.push_back(tmp_b);
      ++ p_b;
      if ( p_b == list_b.end() ) {
	break;
      }
      tmp_b = *p_b;
      idx_b = tmp_b / 2;
    }
    else {
      assert_cond( tmp_a == tmp_b, __FILE__, __LINE__);
      dst_list.push_back(tmp_a);

      ++ p_a;
      if ( p_a == list_a.end() ) {
	break;
      }
      tmp_a = *p_a;
      idx_a = tmp_a / 2;

      ++ p_b;
      if ( p_b == list_b.end() ) {
	break;
      }
      tmp_b = *p_b;
      idx_b = tmp_b / 2;
    }
  }
  for ( ; p_a != list_a.end(); ++ p_a) {
    dst_list.push_back(*p_a);
  }
  for ( ; p_b != list_b.end(); ++ p_b) {
    dst_list.push_back(*p_b);
  }

  list_a = dst_list;
}

END_NONAMESPACE

#if 0
// @brief 必要割り当ての情報に基づいて故障をグループ分けする．
// @note 内部で dtpg_ffr() を呼ぶ．
void
DtpgSat::dtpg_ffr2(const vector<DtpgFault*>& flist,
		   DtpgNode* root,
		   const vector<DtpgNode*>& node_list,
		   DtpgOperator& op)
{
#if VERIFY_MAIMP
  // 必要割り当てで冗長と判定された故障のリストを作る．
  vector<DtpgFault*> r_list;
  for (vector<DtpgFault*>::const_iterator p = flist.begin();
       p != flist.end(); ++ p) {
    DtpgFault* f = *p;
    bool stat = get_mandatory_assignment(f, f->ma_list());
    if ( !stat ) {
      r_list.push_back(f);
    }
  }

  dtpg_ffr(flist, root, node_list, op);

  // SAT でも冗長と判定されたかチェックする．
  for (vector<DtpgFault*>::iterator p = r_list.begin();
       p != r_list.end(); ++ p) {
    DtpgFault* f = *p;
    if ( !f->is_untestable() ) {
      cout << "Error! " << f_str(f)
	   << " is not redundant" << endl;
    }
    assert_cond( f->is_untestable(), __FILE__, __LINE__);
  }

#else

  // 必要割り当てに基づいて故障をグループ分けする．
  vector<FaultGroup> fault_group;
  for (vector<DtpgFault*>::const_iterator p = flist.begin();
       p != flist.end(); ++ p) {
    DtpgFault* f = *p;
    bool stat = get_mandatory_assignment(f, f->ma_list());
    if ( stat ) {
      ymuint n = fault_group.size();
      bool found = false;
      for (ymuint i = 0; i < n; ++ i) {
	FaultGroup& fg = fault_group[i];
	if ( !check_conflict(fg.mMaList, f->ma_list()) ) {
	  fg.mFaultList.push_back(f);
	  merge(fg.mMaList, f->ma_list());
	  found = true;
	  break;
	}
      }
      if ( !found ) {
	fault_group.push_back(FaultGroup());
	FaultGroup& fg = fault_group.back();
	fg.mFaultList.push_back(f);
	fg.mMaList = f->ma_list();
      }
    }
    else {
      // ここで冗長とわかったら以下の処理は必要ない．
      f->set_untestable();
      if ( mSkip ) {
	f->set_skip();
      }
      else {
	op.set_untestable(f->safault());
      }
    }
  }

  if ( fault_group.empty() ) {
    return;
  }

#if 0
  for (vector<FaultGroup>::iterator p = fault_group.begin();
       p != fault_group.end(); ++ p) {
    FaultGroup& fg = *p;
    dtpg_ffr(fg.mFaultList, root, node_list, op);
  }
#else
  dtpg_ffr(flist, root, node_list, op);
#endif
#endif
}
#endif

BEGIN_NONAMESPACE

// f が対象の故障の場合 true を返す．
inline
bool
is_valid(DtpgFault* f,
	 const vector<DtpgFault*>& flist)
{
  if ( f == NULL ) {
    return false;
  }
  ymuint fid = f->tmp_id();
  if ( fid < flist.size() && flist[fid] == f ) {
    return true;
  }
  return false;
}

END_NONAMESPACE

// @brief 複数の故障に対してテストパタン生成を行なう．
// @param[in] flist 故障リスト
// @param[in] root FFR の根のノード
// @param[in] node_list FFR 内のノードリスト
// @param[in] op テスト生成の結果を処理するファンクター
void
DtpgSat::dtpg_group(const vector<DtpgFault*>& flist,
		    DtpgOperator& op)
{
  if ( flist.empty() ) {
    return;
  }

  if ( mTimerEnable ) {
    mTimer.reset();
    mTimer.start();
  }

  SatSolver solver(mType, mOption, mOutP);

  // 故障に一時的なID番号を割り振る．
  // 故障のあるノードに mark2 をつける．
  ymuint nf = flist.size();
  vector<DtpgNode*> fnode_list;
  fnode_list.reserve(nf);
  for (ymuint i = 0; i < nf; ++ i) {
    DtpgFault* f = flist[i];
    f->set_tmp_id(i);
    DtpgNode* fnode = f->node();
    if ( !fnode->mark2() ) {
      fnode->set_mark2();
      fnode_list.push_back(fnode);
    }
  }

  // 故障のあるノードの TFO に mark1 をつける．
  // ついでに mark2 を消す．
  vector<DtpgNode*> tfo_list;
  for (vector<DtpgNode*>::iterator p = fnode_list.begin();
       p != fnode_list.end(); ++ p) {
    DtpgNode* fnode = *p;
    fnode->set_mark1();
    tfo_list.push_back(fnode);
    fnode->clear_mark2();
  }
  for (ymuint rpos = 0; rpos < tfo_list.size(); ++ rpos) {
    DtpgNode* node = tfo_list[rpos];
    ymuint nfo = node->active_fanout_num();
    for (ymuint i = 0; i < nfo; ++ i) {
      DtpgNode* fonode = node->active_fanout(i);
      if ( !fonode->mark1() ) {
	fonode->set_mark1();
	tfo_list.push_back(fonode);
      }
    }
  }

  // TFO の TFI に mark2 をつける．
  vector<DtpgNode*> tfi_list;
  for (vector<DtpgNode*>::iterator p = tfo_list.begin();
       p != tfo_list.end(); ++ p) {
    DtpgNode* node = *p;
    ymuint ni = node->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      DtpgNode* finode = node->fanin(i);
      if ( !finode->mark1() && !finode->mark2() ) {
	finode->set_mark2();
	tfi_list.push_back(finode);
      }
    }
  }
  for (ymuint rpos = 0; rpos < tfi_list.size(); ++ rpos) {
    DtpgNode* node = tfi_list[rpos];
    ymuint ni = node->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      DtpgNode* finode = node->fanin(i);
      if ( !finode->mark1() && !finode->mark2() ) {
	finode->set_mark2();
	tfi_list.push_back(finode);
      }
    }
  }

  // mark1 と mark2 をクリアする．
  for (vector<DtpgNode*>::iterator p = tfo_list.begin();
       p != tfo_list.end(); ++ p) {
    DtpgNode* node = *p;
    node->clear_mark1();
    node->clear_mark2();
  }
  for (vector<DtpgNode*>::iterator p = tfi_list.begin();
       p != tfi_list.end(); ++ p) {
    DtpgNode* node = *p;
    node->clear_mark1();
    node->clear_mark2();
  }

  // TFO マークのついたノード用の変数の生成
  mUsedNodeList.clear();
  for (vector<DtpgNode*>::iterator p = tfo_list.begin();
       p != tfo_list.end(); ++ p) {
    DtpgNode* node = *p;
    set_gvar(solver, node);
    set_fvar(solver, node);
    mUsedNodeList.push_back(node);
  }

  // TFI マークのついたノード用の変数の生成
  for (vector<DtpgNode*>::iterator p = tfi_list.begin();
       p != tfi_list.end(); ++ p) {
    DtpgNode* node = *p;
    set_gvar(solver, node);
    mUsedNodeList.push_back(node);
  }

  // mInputList を作る．
  mInputList.clear();
  for (vector<DtpgNode*>::iterator p = tfi_list.begin();
       p != tfi_list.end(); ++ p) {
    DtpgNode* node = *p;
    if ( node->is_input() ) {
      mInputList.push_back(node);
    }
  }

  // mOutputList を作る．
  mOutputList.clear();
  for (vector<DtpgNode*>::iterator p = tfo_list.begin();
       p != tfo_list.end(); ++ p) {
    DtpgNode* node = *p;
    if ( node->is_input() ) {
      mInputList.push_back(node);
    }
    if ( node->is_output() ) {
      mOutputList.push_back(node);
    }
  }

  // 故障を活性化するとき true にする変数．
  vector<VarId> flt_var(nf);
  // 故障挿入回路の出力に対応する変数．
  vector<VarId> tmp_var(nf);
  for (ymuint i = 0; i < nf; ++ i) {
    flt_var[i] = solver.new_var();
    tmp_var[i] = solver.new_var();
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
  for (ymuint i = 0; i < tfo_list.size(); ++ i) {
    DtpgNode* node = tfo_list[i];

    // inputs[] に入力変数を表すリテラルを格納する．
    // ただし，入力の故障を仮定する場合には故障挿入回路の出力変数となる．
    ymuint ni = node->fanin_num();
    vector<Literal> inputs(ni);
    for (ymuint i = 0; i < ni; ++ i) {
      DtpgNode* inode = node->fanin(i);
      VarId ivar = inode->fvar();
      for (ymint val = 0; val < 2; ++ val) {
	DtpgFault* f = node->input_fault(val, i);
	if ( is_valid(f, flist) ) {
	  ymuint fid = f->tmp_id();
	  make_flt_cnf(solver, ivar, flt_var[fid], tmp_var[fid], val);
	  ivar = tmp_var[fid];
	}
      }
      inputs[i] = Literal(ivar, kPolPosi);
    }

    // ovar に出力変数を入れる．
    // こちらは入力の故障と異なり，故障挿入回路の出力が node->fvar() となる．
    // 逆に ovar はゲートの直接の出力変数となる．
    VarId ovar = node->fvar();
    for (ymint val = 0; val < 2; ++ val) {
      DtpgFault* f = node->output_fault(val);
      if ( is_valid(f, flist) ) {
	ymuint fid = f->tmp_id();
	make_flt_cnf(solver, tmp_var[fid], flt_var[fid], ovar, val);
	ovar = tmp_var[fid];
      }
    }

    Literal glit(node->gvar(), kPolPosi);
    Literal flit(node->fvar(), kPolPosi);
    Literal dlit(node->dvar(), kPolPosi);

    // XOR(glit, flit, dlit) を追加する．
    // 要するに正常回路と故障回路で異なっているとき dlit が 1 となる．
    solver.add_clause(~glit, ~flit, ~dlit);
    solver.add_clause(~glit,  flit,  dlit);
    solver.add_clause( glit, ~flit,  dlit);
    solver.add_clause( glit,  flit, ~dlit);

    Literal gate_output(ovar, kPolPosi);
    if ( node->is_input() ) {
      solver.add_clause(~glit,  gate_output);
      solver.add_clause( glit, ~gate_output);
    }
    else {
      if ( node->is_output() ) {
	solver.add_clause( inputs[0], ~gate_output);
	solver.add_clause(~inputs[0],  gate_output);
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
		output = gate_output;
	      }
	      else {
		output = Literal(prim->fvar(), kPolPosi);
	      }
	      make_gate_cnf(solver, prim->gate_type(), output, VectorInputLiteral(inputs1));
	    }
	  }
	}
	else {
	  make_gate_cnf(solver, node->gate_type(), gate_output, VectorInputLiteral(inputs));
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
	if ( is_valid(fi0, flist) ) {
	  ymuint fid = fi0->tmp_id();
	  dep.push_back(Literal(flt_var[fid], kPolPosi));
	}

	DtpgFault* fi1 = node->input_fault(1, j);
	if ( is_valid(fi1, flist) ) {
	  ymuint fid = fi1->tmp_id();
	  dep.push_back(Literal(flt_var[fid], kPolPosi));
	}
      }

      DtpgFault* fo0 = node->output_fault(0);
      if ( is_valid(fo0, flist) ) {
	ymuint fid = fo0->tmp_id();
	dep.push_back(Literal(flt_var[fid], kPolPosi));
      }

      DtpgFault* fo1 = node->output_fault(1);
      if ( is_valid(fo1, flist) ) {
	ymuint fid = fo1->tmp_id();
	dep.push_back(Literal(flt_var[fid], kPolPosi));
      }

      solver.add_clause(dep);
    }
  }


  //////////////////////////////////////////////////////////////////////
  // 故障の検出条件
  //////////////////////////////////////////////////////////////////////
  vector<Literal> odiff;
  odiff.reserve(mOutputList.size());
  for (vector<DtpgNode*>::iterator p = mOutputList.begin();
       p != mOutputList.end(); ++ p) {
    DtpgNode* node = *p;
    Literal dlit(node->dvar(), kPolPosi);
    odiff.push_back(dlit);
  }
  solver.add_clause(odiff);

  if ( mTimerEnable ) {
    mTimer.stop();
    mCnfTime += mTimer.time();
    ++ mCnfCount;
  }

  // 個々の故障に対するテスト生成を行なう．
  for (ymuint i = 0; i < nf; ++ i) {
    DtpgFault* f = flist[i];

    mAssumptions.clear();
    mAssumptions.reserve(tfo_list.size() + nf);

    // 該当の故障に対する変数のみ1にする．
    for (ymuint j = 0; j < nf; ++ j) {
      tPol pol = (j == i) ? kPolPosi : kPolNega;
      mAssumptions.push_back(Literal(flt_var[j], pol));
    }

    // 故障ノードの TFO 以外の dlit を0にする．
    mark_tfo(f->node());
    for (vector<DtpgNode*>::const_iterator p = tfo_list.begin();
	 p != tfo_list.end(); ++ p) {
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

// @brief 一つの SAT問題を解く．
void
DtpgSat::solve(SatSolver& solver,
	       DtpgFault* f,
	       DtpgOperator& op)
{
  if ( mDryRun ) {
    f->set_skip();
    return;
  }

  if ( mTimerEnable ) {
    mTimer.reset();
    mTimer.start();
  }

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

    if ( mTimerEnable ) {
      mTimer.stop();
      mDetTime += mTimer.time();
      ++ mDetCount;
    }
  }
  else if ( ans == kB3False ) {
    f->set_untestable();
    if ( mSkip ) {
      f->set_skip();
    }
    else {
      op.set_untestable(f->safault());
    }

    if ( mTimerEnable ) {
      mTimer.stop();
      mUndetTime += mTimer.time();
      ++ mUndetCount;
    }
  }
  else { // ans == kB3X つまりアボート
    if ( mTimerEnable ) {
      mTimer.stop();
      mAbortTime += mTimer.time();
      ++ mAbortCount;
    }
  }
}

// @brief テストパタンを求める．
// @note 結果は mValList に格納される．
void
DtpgSat::get_pat(DtpgNode* fnode)
{
  mValList.clear();
  mBwdNodeList.clear();

  DtpgNode* onode = NULL;
  for (vector<DtpgNode*>::iterator p = mOutputList.begin();
       p != mOutputList.end(); ++ p) {
    DtpgNode* node = *p;
    if ( mModel[node->dvar().val()] == kB3True ) {
      onode = node;
      break;
    }
  }
  assert_cond( onode != NULL, __FILE__, __LINE__);
  justify(onode);

  // 一連の処理でつけたマークを消す．
  for (vector<DtpgNode*>::iterator p = mBwdNodeList.begin();
       p != mBwdNodeList.end(); ++ p) {
    DtpgNode* node = *p;
    node->clear_mark3();
  }
}

// @brief テストパタンを求める．
// @note 結果は mValList に格納される．
void
DtpgSat::get_pat2(DtpgNode* fnode)
{
#if 0
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
#endif
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

  Bool3 gval = mModel[node->gvar().val()];
  Bool3 fval = mModel[node->fvar().val()];

  if ( gval != fval ) {
    // 正常値と故障値が異なっていたら
    // すべてのファンインをたどる．
    just_sub1(node);
    return;
  }

  if ( node->is_cplx_logic() ) {
    ymuint np = node->primitive_num();
    DtpgPrimitive* prim = node->primitive(np - 1);
    justify_primitive(prim, node);
  }
  else {
    switch ( node->gate_type() ) {
    case kTgGateBuff:
    case kTgGateNot:
      // 無条件で唯一のファンインをたどる．
      justify(node->fanin(0));
      break;

    case kTgGateAnd:
      if ( gval == kB3True ) {
	// すべてのファンインノードをたどる．
	just_sub1(node);
      }
      else if ( gval == kB3False ) {
	// 0の値を持つ最初のノードをたどる．
	just_sub2(node, kB3False);
      }
      break;

    case kTgGateNand:
      if ( gval == kB3True ) {
	// 0の値を持つ最初のノードをたどる．
	just_sub2(node, kB3False);
      }
      else if ( gval == kB3False ) {
	// すべてのファンインノードをたどる．
	just_sub1(node);
      }
      break;

    case kTgGateOr:
      if ( gval == kB3True ) {
	// 1の値を持つ最初のノードをたどる．
	just_sub2(node, kB3True);
      }
      else if ( gval == kB3False ) {
	// すべてのファンインノードをたどる．
	just_sub1(node);
      }
      break;

    case kTgGateNor:
      if ( gval == kB3True ) {
	// すべてのファンインノードをたどる．
	just_sub1(node);
      }
      else if ( gval == kB3False ) {
	// 1の値を持つ最初のノードをたどる．
	just_sub2(node, kB3True);
      }
      break;

    case kTgGateXor:
    case kTgGateXnor:
      // すべてのファンインノードをたどる．
      just_sub1(node);
      break;

    default:
      assert_not_reached(__FILE__, __LINE__);
      break;
    }
  }
}

// @brief すべてのファンインに対して justify() を呼ぶ．
// @param[in] node 対象のノード
void
DtpgSat::just_sub1(DtpgNode* node)
{
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    DtpgNode* inode = node->fanin(i);
    justify(inode);
  }
}

// @brief 指定した値を持つのファンインに対して justify() を呼ぶ．
// @param[in] node 対象のノード
// @param[in] val 値
void
DtpgSat::just_sub2(DtpgNode* node,
		   Bool3 val)
{
  bool gfound = false;
  bool ffound = false;
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    DtpgNode* inode = node->fanin(i);
    Bool3 igval = mModel[inode->gvar().val()];
    Bool3 ifval = mModel[inode->fvar().val()];
    if ( !gfound && igval == val ) {
      justify(inode);
      gfound = true;
      if ( ifval == val ) {
	break;
      }
    }
    else if ( !ffound && ifval == val ) {
      justify(inode);
      ffound = true;
    }
    if ( gfound && ffound ) {
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
    return;
  }

  Bool3 gval = mModel[prim->gvar().val()];
  Bool3 fval = mModel[prim->fvar().val()];
  if ( gval != fval ) {
    // すべてのファンインノードをたどる．
    jp_sub1(prim, node);
    return;
  }

  switch ( prim->gate_type() ) {
  case kTgGateBuff:
  case kTgGateNot:
    // 唯一のファンインをたどる．
    justify_primitive(prim->fanin(0), node);
    break;

  case kTgGateAnd:
    if ( gval == kB3True ) {
      // すべてのファンインをたどる．
      jp_sub1(prim, node);
    }
    else if ( gval == kB3False ) {
      // 0 の値を持つ最初のファンインをたどる．
      jp_sub2(prim, node, kB3False);
      break;

    case kTgGateNand:
      if ( gval == kB3True ) {
	// 0 の値を持つ最初のファンインをたどる．
	jp_sub2(prim, node, kB3False);
      }
      else if ( gval == kB3False ) {
	// すべてのファンインをたどる．
	jp_sub1(prim, node);
      }
      break;

    case kTgGateOr:
      if ( gval == kB3True ) {
	// 1の値をもつ最初のファンインをたどる．
	jp_sub2(prim, node, kB3True);
      }
      else if ( gval == kB3False ) {
	// すべてのファンインをたどる．
	jp_sub1(prim, node);
      }
      break;

    case kTgGateNor:
      if ( gval == kB3True ) {
	// すべてのファンインをたどる．
	jp_sub1(prim, node);
      }
      else if ( gval == kB3False ) {
	// 1の値をもつすべてのファンインをたどる．
	jp_sub2(prim, node, kB3True);
      }
      break;

    case kTgGateXor:
    case kTgGateXnor:
      // すべてのファンインをたどる．
      jp_sub1(prim, node);
      break;

    default:
      assert_not_reached(__FILE__, __LINE__);
      break;
    }
  }
}

// @brief すべてのファンインに対して justify_primitive() を呼ぶ．
// @param[in] prim 対象のプリミティブ
// @param[in] node 対象のノード
void
DtpgSat::jp_sub1(DtpgPrimitive* prim,
		 DtpgNode* node)
{
  ymuint ni = prim->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    DtpgPrimitive* iprim = prim->fanin(i);
    justify_primitive(iprim, node);
  }
}

// @brief 指定した値を持つファンインに対して justify_primitive() を呼ぶ．
// @param[in] prim 対象のプリミティブ
// @param[in] node 対象のノード
// @param[in] val 値
void
DtpgSat::jp_sub2(DtpgPrimitive* prim,
		 DtpgNode* node,
		 Bool3 val)
{
  bool gfound = false;
  bool ffound = false;
  ymuint ni = prim->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    DtpgPrimitive* iprim = prim->fanin(i);
    Bool3 igval = mModel[iprim->gvar().val()];
    Bool3 ifval = mModel[iprim->fvar().val()];
    if ( !gfound && igval == val ) {
      justify_primitive(iprim, node);
      gfound = true;
      if ( ifval == val ) {
	break;
      }
    }
    else if ( !ffound && ifval == val ) {
      justify_primitive(iprim, node);
      ffound = true;
    }
    if ( gfound && ffound ) {
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
// @param[in] stats 結果を格納する構造体
void
DtpgSat::get_stats(DtpgStats& stats) const
{
  stats.mCnfGenCount = mCnfCount;
  stats.mCnfGenTime = mCnfTime;
  stats.mDetCount = mDetCount;
  stats.mDetTime = mDetTime;
  stats.mRedCount = mUndetCount;
  stats.mRedTime = mUndetTime;
  stats.mAbortCount = mAbortCount;
  stats.mAbortTime = mAbortTime;

  if ( mRunCount > 0 ) {
    cout << "# of runs:                       " << mRunCount << endl
	 << "# of problems:                   " << mSatCount << endl
	 << "Ave. # of restarts:              " << (double) mRestart / mSatCount << endl
	 << "Ave. # of variables:             " << (double) mVarNum / mRunCount << endl
	 << "Ave. # of constraint clauses:    " << (double) mConstrClauseNum / mRunCount << endl
	 << "Ave. # of constraint literals:   " << (double) mConstrLitNum / mRunCount << endl
	 << "Ave. # of learnt clauses:        " << (double) mLearntClauseNum / mRunCount << endl
	 << "Ave. # of learnt literals:       " << (double) mLearntLitNum / mRunCount << endl
	 << "Ave. # of conflicts:             " << (double) mConflictNum / mSatCount << endl
	 << "Ave. # of decisions:             " << (double) mDecisionNum / mSatCount << endl
	 << "Ave. # of implications:          " << (double) mPropagationNum / mSatCount << endl;
  }
}

// @breif 時間計測を制御する．
void
DtpgSat::timer_enable(bool enable)
{
  mTimerEnable = enable;
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
