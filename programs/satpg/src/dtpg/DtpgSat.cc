
/// @file atpg/src/dtpg/DtpgSat.cc
/// @brief DtpgSat の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSat.h"
#include "ym_networks/TgNetwork.h"
#include "ym_networks/TgNode.h"
#include "TgFFR.h"
#include "SaFault.h"
#include "TestVector.h"
#include "ym_logic/SatSolver.h"


BEGIN_NAMESPACE_YM_SATPG_DTPG

// @brief コンストラクタ
DtpgSat::DtpgSat()
{
}

// @brief デストラクタ
DtpgSat::~DtpgSat()
{
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
  switch ( type ) {
  case kTgUndef:
  case kTgInput:
    assert_not_reached(__FILE__, __LINE__);
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
    {
      ymuint ni = inputs.size();
      vector<Literal> tmp(ni + 1);
      for (ymuint i = 0; i < ni; ++ i) {
	tmp[i] = ~inputs[i];
      }
      tmp[ni] = output;
      solver.add_clause(tmp);
      for (ymuint i = 0; i < ni; ++ i) {
	solver.add_clause(inputs[i], ~output);
      }
    }
    break;

  case kTgNand:
    {
      ymuint ni = inputs.size();
      vector<Literal> tmp(ni + 1);
      for (ymuint i = 0; i < ni; ++ i) {
	tmp[i] = ~inputs[i];
      }
      tmp[ni] = ~output;
      solver.add_clause(tmp);
      for (ymuint i = 0; i < ni; ++ i) {
	solver.add_clause(inputs[i], output);
      }
    }
    break;

  case kTgOr:
    {
      ymuint ni = inputs.size();
      vector<Literal> tmp(ni + 1);
      for (ymuint i = 0; i < ni; ++ i) {
	tmp[i] = inputs[i];
      }
      tmp[ni] = ~output;
      solver.add_clause(tmp);
      for (ymuint i = 0; i < ni; ++ i) {
	solver.add_clause(~inputs[i], output);
      }
    }
    break;

  case kTgNor:
    {
      ymuint ni = inputs.size();
      vector<Literal> tmp(ni + 1);
      for (ymuint i = 0; i < ni; ++ i) {
	tmp[i] = inputs[i];
      }
      tmp[ni] = output;
      solver.add_clause(tmp);
      for (ymuint i = 0; i < ni; ++ i) {
	solver.add_clause(~inputs[i], ~output);
      }
    }
    break;

  case kTgXor:
    {
      ymuint ni = inputs.size();
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
    {
      ymuint ni = inputs.size();
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

/// @brief ノードの入出力の関係を表す CNF クローズを生成する．
/// @param[in] solver SAT ソルバ
/// @param[in] node TgNode
/// @param[in] network TgNetwork
/// @param[in] output 出力リテラル
/// @param[in] inputs 入力リテラル
void
make_node_cnf(SatSolver& solver,
	      const TgNode* node,
	      const TgNetwork& network,
	      Literal output,
	      const vector<Literal>& inputs)
{
  if ( node->is_cplx_logic() ) {
    LogExpr lexp = network.get_lexp(node);
    make_cnf_from_lexp(solver, lexp, output, inputs);
  }
  else {
    make_cnf_from_type(solver, node->type(), output, inputs);
  }
}

END_NONAMESPACE


// 故障 f に対するテストパタン生成を行う．
tStat
DtpgSat::run(const TgNetwork& network,
	     SaFault* f,
	     TestVector* tv)
{
  const TgNode* fnode = f->node();
  const TgNode* fsrc = fnode;
  bool is_input_fault = f->is_input_fault();
  if ( is_input_fault ) {
    fsrc = fnode->fanin(f->pos());
  }

  return dtpg_single(network, fnode, is_input_fault, f->pos(), fsrc, f->val(), tv);
}

// @brief 一つの故障に対してテストパタン生成を行なう．
// @param[in] network 対象の回路
// @param[in] fnode 故障ノード
// @param[in] is_input_fault 出力の故障の時 true
// @param[in] ipos 故障の入力位置
// @param[in] fsrc 故障ノードの入力
// @param[in] fval 故障値
// @param[in] tv 生成したパタンを入れるベクタ
// @retval kDetect パタン生成が成功した．
// @retval kUntest テスト不能故障だった．
// @retval kAbort アボートした．
tStat
DtpgSat::dtpg_single(const TgNetwork& network,
		     const TgNode* fnode,
		     bool is_input_fault,
		     ymuint ipos,
		     const TgNode* fsrc,
		     int fval,
		     TestVector* tv)
{
  SatSolver solver(mType, mOption, mOutP);

  init_varmap(network);

  make_cnf(solver, network, fnode, is_input_fault, ipos, fsrc);

  vector<Literal> assumptions(2);
  if ( fval ) {
    assumptions[0] = Literal(gvar(fsrc), kPolNega);
    assumptions[1] = Literal(fvar(fsrc), kPolPosi);
  }
  else {
    assumptions[0] = Literal(gvar(fsrc), kPolPosi);
    assumptions[1] = Literal(fvar(fsrc), kPolNega);
  }

  vector<Bool3> model;
  Bool3 stat = solver.solve(assumptions, model);
  tStat ans = kAbort;
  if ( stat == kB3True ) {
    tv->init();
    ymuint npi = network.input_num2();
    for (ymuint i = 0; i < npi; ++ i) {
      const TgNode* node = network.input(i);
      if ( mark(node) != kNone ) {
	VarId idx = gvar(node);
	if ( model[idx.val()] == kB3True ) {
	  tv->set_val(i, kVal1);
	}
	else {
	  tv->set_val(i, kVal0);
	}
      }
    }
    ans = kDetect;
  }
  else if ( stat == kB3False ) {
    ans = kUntest;
  }

  solver.get_stats(mStats);

  return ans;
}

// @brief 同じ位置の2つの故障に対してテストパタン生成を行なう．
// @param[in] network 対象の回路
// @param[in] fnode 故障ノード
// @param[in] is_input_fault 入力の故障の時 true
// @param[in] ipos 故障の入力位置
// @param[in] fsrc 故障ノードの入力
// @param[in] tv 生成したパタンを入れるベクタ
// @retval kDetect パタン生成が成功した．
// @retval kUntest テスト不能故障だった．
// @retval kAbort アボートした．
pair<tStat, tStat>
DtpgSat::dtpg_dual(const TgNetwork& network,
		   const TgNode* fnode,
		   bool is_input_fault,
		   ymuint ipos,
		   const TgNode* fsrc,
		   TestVector* tv[])
{
  SatSolver solver(mType, mOption, mOutP);

  init_varmap(network);

  make_cnf(solver, network, fnode, is_input_fault, ipos, fsrc);

  tStat ans[2];
  for (ymuint fval = 0; fval < 2; ++ fval ) {
    vector<Literal> assumptions(2);
    if ( fval ) {
      assumptions[0] = Literal(gvar(fsrc), kPolNega);
      assumptions[1] = Literal(fvar(fsrc), kPolPosi);
    }
    else {
      assumptions[0] = Literal(gvar(fsrc), kPolPosi);
      assumptions[1] = Literal(fvar(fsrc), kPolNega);
    }

    vector<Bool3> model;
    Bool3 stat = solver.solve(assumptions, model);
    ans[fval] = kAbort;
    if ( stat == kB3True ) {
      tv[fval]->init();
      ymuint npi = network.input_num2();
      for (ymuint i = 0; i < npi; ++ i) {
	const TgNode* node = network.input(i);
	if ( mark(node) != kNone ) {
	  VarId idx = gvar(node);
	  if ( model[idx.val()] == kB3True ) {
	    tv[fval]->set_val(i, kVal1);
	  }
	  else {
	    tv[fval]->set_val(i, kVal0);
	  }
	}
      }
      ans[fval] = kDetect;
    }
    else if ( stat == kB3False ) {
      ans[fval] = kUntest;
    }
  }
  solver.get_stats(mStats);

  return make_pair(ans[0], ans[1]);
}

BEGIN_NONAMESPACE

struct NodeFaults
{
  NodeFaults(ymuint ni = 0)
  {
    mInputs[0].resize(ni);
    mInputs[1].resize(ni);
  }

  ymuint mOutput[2];
  vector<ymuint> mInputs[2];
};

END_NONAMESPACE

// @brief FFR 内の故障に対してテストパタン生成を行なう．
// @param[in] network 対象の回路
// @param[in] ffr FFR を表すクラス
// @param[in] flist 故障リスト
// @param[in] tv_list 生成したパタンを入れるベクタ
// @param[in] stat_list 結果を入れるベクタ
// @note flist の故障は必ず root が dominator となっていなければならない．
void
DtpgSat::dtpg_ffr(const TgNetwork& network,
		  const TgFFR* ffr,
		  const vector<SaFault*>& flist,
		  vector<TestVector*>& tv_list,
		  vector<tStat>& stat_list)
{
  SatSolver solver(mType, mOption, mOutP);

  init_varmap(network);

  const TgNode* root = ffr->root();

  make_cnf(solver, network, root, false, 0, root);

  ymuint nf = flist.size();

  hash_set<ymuint> fmark;
  for (ymuint i = 0; i < nf; ++ i) {
    SaFault* f = flist[i];
    const TgNode* node = f->node();
    fmark.insert(node->gid());
  }

  // 故障回路用の変数を割り当てる．
  const vector<const TgNode*>& node_list = ffr->node_list();
  hash_map<ymuint, NodeFaults> fmap;
  for (vector<const TgNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    const TgNode* node = *p;
    ymuint ni = node->ni();
    bool found = false;
    for (ymuint i = 0; i < ni; ++ i) {
      const TgNode* inode = node->fanin(i);
      if ( fmark.count(inode->gid()) > 0 ) {
	found = true;
	break;
      }
    }
    if ( found ) {
      fmark.insert(node->gid());
    }

    if ( node != root ) {
      Var& var = mVarMap[node->gid()];
      if ( fmark.count(node->gid()) > 0 ) {
	var.mFid = solver.new_var();
      }
    }

    fmap.insert(make_pair(node->gid(), NodeFaults(ni)));
    NodeFaults& faults = fmap[node->gid()];
    faults.mOutput[0] = nf;
    faults.mOutput[1] = nf;
    for (ymuint i = 0; i < ni; ++ i) {
      faults.mInputs[0][i] = nf;
      faults.mInputs[1][i] = nf;
    }
  }

  vector<VarId> flt_var(nf);
  vector<VarId> tmp_var(nf);
  for (ymuint i = 0; i < nf; ++ i) {
    flt_var[i] = solver.new_var();
    tmp_var[i] = solver.new_var();
    SaFault* f = flist[i];
    const TgNode* fnode = f->node();
    hash_map<ymuint, NodeFaults>::iterator p = fmap.find(fnode->gid());
    assert_cond( p != fmap.end(), __FILE__, __LINE__);
    NodeFaults& faults = p->second;

    if ( f->is_input_fault() ) {
      ymuint pos = f->pos();
      if ( f->val() == 0 ) {
	faults.mInputs[0][pos] = i;
      }
      else {
	faults.mInputs[1][pos] = i;
      }
    }
    else {
      if ( f->val() == 0 ) {
	faults.mOutput[0] = i;
      }
      else {
	faults.mOutput[1] = i;
      }
    }
  }

  // FFR 内の故障回路を作る．
  for (vector<const TgNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    const TgNode* node = *p;
    if ( fmark.count(node->gid()) == 0 ) {
      continue;
    }

    hash_map<ymuint, NodeFaults>::iterator q = fmap.find(node->gid());
    assert_cond( q != fmap.end(), __FILE__, __LINE__);
    NodeFaults& faults = q->second;

    ymuint ni = node->ni();
    vector<Literal> inputs(ni);
    for (ymuint i = 0; i < ni; ++ i) {
      const TgNode* inode = node->fanin(i);
      VarId fvar;
      if ( fmark.count(inode->gid()) > 0 ) {
	fvar = mVarMap[inode->gid()].mFid;
      }
      else {
	fvar = mVarMap[inode->gid()].mGid;
      }
      ymuint f0id = faults.mInputs[0][i];
      if ( f0id < nf ) {
	make_cnf_and(solver, flt_var[f0id], fvar, tmp_var[f0id]);
	fvar = tmp_var[f0id];
      }
      ymuint f1id = faults.mInputs[1][i];
      if ( f1id < nf ) {
	make_cnf_or(solver, flt_var[f1id], fvar, tmp_var[f1id]);
	fvar = tmp_var[f1id];
      }
      inputs[i] = Literal(fvar, kPolPosi);
    }
    VarId fvar = mVarMap[node->gid()].mFid;

    ymuint f1id = faults.mOutput[1];
    if ( f1id < nf ) {
      make_cnf_or(solver, flt_var[f1id], tmp_var[f1id], fvar);
      fvar = tmp_var[f1id];
    }

    ymuint f0id = faults.mOutput[0];
    if ( f0id < nf ) {
      make_cnf_and(solver, flt_var[f0id], tmp_var[f0id], fvar);
      fvar = tmp_var[f0id];
    }

    if ( !node->is_input() ) {
      make_node_cnf(solver, node, network, Literal(fvar, kPolPosi), inputs);
    }
  }

  // 個々の故障に対するテスト生成を行なう．
  for (ymuint i = 0; i < nf; ++ i) {
    vector<Literal> assumptions(nf + 1);
    for (ymuint j = 0; j < nf; ++ j) {
      if ( j != i ) {
	assumptions[j] = Literal(flt_var[j], kPolNega);
      }
      else {
	assumptions[j] = Literal(flt_var[j], kPolPosi);
      }
    }

    SaFault* f = flist[i];
    const TgNode* fnode = f->node();
    if ( f->is_input_fault() ) {
      fnode = fnode->fanin(f->pos());
    }
    tPol pol = (f->val() == 0) ? kPolPosi : kPolNega;
    assumptions[nf] = Literal(gvar(fnode), pol);

    vector<Bool3> model;
    Bool3 stat = solver.solve(assumptions, model);
    if ( stat == kB3True ) {
      TestVector* tv = tv_list[i];
      tv->init();
      ymuint npi = network.input_num2();
      for (ymuint j = 0; j < npi; ++ j) {
	const TgNode* node = network.input(j);
	if ( mark(node) != kNone ) {
	  VarId idx = gvar(node);
	  if ( model[idx.val()] == kB3True ) {
	    tv->set_val(j, kVal1);
	  }
	  else {
	    tv->set_val(j, kVal0);
	  }
	}
      }
      stat_list[i] = kDetect;
    }
    else if ( stat == kB3False ) {
      stat_list[i] = kUntest;
    }
    else {
      stat_list[i] = kAbort;
    }
  }
}

// @brief mVarMap を初期化する．
void
DtpgSat::init_varmap(const TgNetwork& network)
{
  ymuint n = network.node_num();

  mVarMap.clear();
  mVarMap.resize(n);
  for (ymuint i = 0; i < n; ++ i) {
    mVarMap[i].mMark = kNone;
  }
}

// @brief 同じ位置の故障に対する CNF を作る．
void
DtpgSat::make_cnf(SatSolver& solver,
		  const TgNetwork& network,
		  const TgNode* fnode,
		  bool is_input_fault,
		  ymuint ipos,
		  const TgNode* fsrc)
{
  ymuint n = network.node_num();
  ymuint nl = network.logic_num();
  ymuint npo = network.output_num2();

  // まず fnode の TFO にマークをつける．
  // 同時にマークの付いたノードは queue につまれる．
  vector<const TgNode*> queue;
  queue.reserve(n);
  mark(fnode) = kTFO;
  queue.push_back(fnode);
  {
    ymuint rpos = 0;
    while ( rpos < queue.size() ) {
      const TgNode* node = queue[rpos];
      ++ rpos;
      ymuint nfo = node->fanout_num();
      for (ymuint i = 0; i < nfo; ++ i) {
	const TgNode* onode = node->fanout(i);
	if ( mark(onode) == kNone ) {
	  mark(onode) = kTFO;
	  queue.push_back(onode);
	}
      }
    }

    // マークの付いたノードの TFI に別のマークをつける．
    vector<const TgNode*> queue2;
    for (ymuint rpos = 0; rpos < queue.size(); ++ rpos) {
      const TgNode* node = queue[rpos];
      ymuint ni = node->ni();
      for (ymuint i = 0; i < ni; ++ i) {
	const TgNode* inode = node->fanin(i);
	if ( mark(inode) == kNone ) {
	  // マークの付いていないファンインがあった．
	  mark(inode) = kTFI;
	  queue2.push_back(inode);
	}
      }
    }
    for (ymuint rpos2 = 0; rpos2 < queue2.size(); ++ rpos2) {
      const TgNode* node = queue2[rpos2];
      ymuint ni = node->ni();
      for (ymuint i = 0; i < ni; ++ i) {
	const TgNode* inode = node->fanin(i);
	if ( mark(inode) == kNone ) {
	  // マークの付いていないファンインがあった．
	  mark(inode) = kTFI;
	  queue2.push_back(inode);
	}
      }
    }
  }
  // 以降は kTFO か kTFI マークのついたノードのみを対象とする．

  // 変数の生成 (glit, flit, dlit の3つを作る)
  // ちょっと分かりにくいが fsrc は普通のゲートにも
  // ファンアウトしているので kTFO マークは付かない．
  // ただし故障ゲートにファンアウトしているブランチの値
  // は故障値なので mFid を持つ．
  for (ymuint i = 0; i < n; ++ i) {
    const TgNode* node = network.node(i);
    Var& var = mVarMap[node->gid()];
    if ( var.mMark != kNone ) {
      var.mGid = solver.new_var();
      if ( node == fsrc || var.mMark == kTFO ) {
	var.mFid = solver.new_var();
	var.mDid = solver.new_var();
      }
    }
  }


  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////

  for (ymuint i = 0; i < nl; ++ i) {
    const TgNode* node = network.sorted_logic(i);
    if ( mark(node) == kNone ) continue;
    ymuint ni = node->ni();
    vector<Literal> inputs(ni);
    for (ymuint j = 0; j < ni; ++ j) {
      const TgNode* inode = node->fanin(j);
      inputs[j] = Literal(gvar(inode), kPolPosi);
    }
    Literal output(gvar(node), kPolPosi);
    make_node_cnf(solver, node, network, output, inputs);
  }
  for (ymuint i = 0; i < npo; ++ i) {
    const TgNode* node = network.output(i);
    if ( mark(node) == kNone ) continue;
    const TgNode* inode = node->fanin(0);
    Literal input(gvar(inode), kPolPosi);
    Literal output(gvar(node), kPolPosi);
    solver.add_clause(~input,  output);
    solver.add_clause( input, ~output);
  }


  //////////////////////////////////////////////////////////////////////
  // 故障回路の CNF を生成
  //////////////////////////////////////////////////////////////////////

  Literal glit(gvar(fnode), kPolPosi);
  Literal flit(fvar(fnode), kPolPosi);
  Literal dlit(dvar(fnode), kPolPosi);
  if ( is_input_fault ) {
    ymuint ni = fnode->ni();
    vector<Literal> inputs(ni);
    for (ymuint i = 0; i < ni; ++ i) {
      const TgNode* inode = fnode->fanin(i);
      if ( i == ipos ) {
	inputs[i] = Literal(fvar(inode), kPolPosi);
      }
      else {
	inputs[i] = Literal(gvar(inode), kPolPosi);
      }
    }
    make_node_cnf(solver, fnode, network, flit, inputs);
  }
  solver.add_clause(~glit, ~flit, ~dlit);
  solver.add_clause(~glit,  flit,  dlit);
  solver.add_clause( glit, ~flit,  dlit);
  solver.add_clause( glit,  flit, ~dlit);

  // mark の付いていないノードは正常回路，付いているノードは故障回路
  // を用いて CNF を作る．
  vector<Literal> odiff;
  odiff.reserve(npo);
  for (ymuint i = 1; i < queue.size(); ++ i) {
    const TgNode* node = queue[i];
    ymuint ni = node->ni();

    Literal glit(gvar(node), kPolPosi);
    Literal flit(fvar(node), kPolPosi);
    Literal dlit(dvar(node), kPolPosi);

    // 入力のリテラル
    vector<Literal> inputs(ni);

    // dlit が 1 の時，入力のノードの dlit も1でなければならない．
    vector<Literal> dep;
    dep.reserve(ni + 1);
    dep.push_back(~dlit);

    for (ymuint j = 0; j < ni; ++ j) {
      const TgNode* inode = node->fanin(j);
      if ( mark(inode) == kTFO ) {
	inputs[j] = Literal(fvar(inode), kPolPosi);
	dep.push_back(Literal(dvar(inode), kPolPosi));
      }
      else {
	inputs[j] = Literal(gvar(inode), kPolPosi);
      }
    }

    // node の入力と出力の関係の clause を追加する．
    make_node_cnf(solver, node, network, flit, inputs);

    // XOR(glit, flit, dlit) を追加する．
    // 要するに正常回路と故障回路で異なっているとき dlit が 1 となる．
    solver.add_clause(~glit, ~flit, ~dlit);
    solver.add_clause(~glit,  flit,  dlit);
    solver.add_clause( glit, ~flit,  dlit);
    solver.add_clause( glit,  flit, ~dlit);

    solver.add_clause(dep);

    // node が出力の時には odiff に追加しておく．
    if ( node->is_output() ) {
      odiff.push_back(dlit);
    }
  }
  // 出力のうち，最低1つは故障差が伝搬しなければならない．
  solver.add_clause(odiff);
}

// @brief 直前の実行結果を得る．
const SatStats&
DtpgSat::stats() const
{
  return mStats;
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

END_NAMESPACE_YM_SATPG_DTPG
