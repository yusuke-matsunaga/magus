﻿
/// @file SatEngine.cc
/// @brief SatEngine の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "SatEngine.h"
#include "GvalCnf.h"
#include "FvalCnf.h"
#include "MvalCnf.h"
#include "NodeValList.h"
#include "NodeSet.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "VidMap.h"
#include "VidLitMap.h"
#include "VectLitMap.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

// @brief バッファの入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバー
// @param[in] i 入力のリテラル
// @param[in] o 出力のリテラル
inline
void
make_buff_cnf(SatSolver& solver,
	      Literal i,
	      Literal o)
{
  solver.add_clause( i, ~o);
  solver.add_clause(~i,  o);
}

// @brief 2入力 AND ゲートの入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバー
// @param[in] i0, i1 入力のリテラル
// @param[in] o 出力のリテラル
inline
void
make_and2_cnf(SatSolver& solver,
	      Literal i0,
	      Literal i1,
	      Literal o)
{
  solver.add_clause( i0, ~o);
  solver.add_clause( i1, ~o);
  solver.add_clause(~i0, ~i1, o);
}

// @brief 3入力 AND ゲートの入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバー
// @param[in] i0, i1, i2 入力のリテラル
// @param[in] o 出力のリテラル
inline
void
make_and3_cnf(SatSolver& solver,
	      Literal i0,
	      Literal i1,
	      Literal i2,
	      Literal o)
{
  solver.add_clause( i0, ~o);
  solver.add_clause( i1, ~o);
  solver.add_clause( i2, ~o);
  solver.add_clause(~i0, ~i1, ~i2, o);
}

// @brief 4入力 AND ゲートの入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバー
// @param[in] i0, i1, i2, i3 入力のリテラル
// @param[in] o 出力のリテラル
inline
void
make_and4_cnf(SatSolver& solver,
	      Literal i0,
	      Literal i1,
	      Literal i2,
	      Literal i3,
	      Literal o)
{
  solver.add_clause( i0, ~o);
  solver.add_clause( i1, ~o);
  solver.add_clause( i2, ~o);
  solver.add_clause( i3, ~o);
  solver.add_clause(~i0, ~i1, ~i2, ~i3, o);
}

// @brief 2入力 OR ゲートの入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバー
// @param[in] i0, i1 入力のリテラル
// @param[in] o 出力のリテラル
inline
void
make_or2_cnf(SatSolver& solver,
	     Literal i0,
	     Literal i1,
	     Literal o)
{
  solver.add_clause(~i0,  o);
  solver.add_clause(~i1,  o);
  solver.add_clause( i0,  i1, ~o);
}

// @brief 3入力 OR ゲートの入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバー
// @param[in] i0, i1, i2 入力のリテラル
// @param[in] o 出力のリテラル
inline
void
make_or3_cnf(SatSolver& solver,
	     Literal i0,
	     Literal i1,
	     Literal i2,
	     Literal o)
{
  solver.add_clause(~i0,  o);
  solver.add_clause(~i1,  o);
  solver.add_clause(~i2,  o);
  solver.add_clause( i0,  i1, i2, ~o);
}

// @brief 4入力 OR ゲートの入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバー
// @param[in] i0, i1, i2, i3 入力のリテラル
// @param[in] o 出力のリテラル
inline
void
make_or4_cnf(SatSolver& solver,
	     Literal i0,
	     Literal i1,
	     Literal i2,
	     Literal i3,
	     Literal o)
{
  solver.add_clause(~i0,  o);
  solver.add_clause(~i1,  o);
  solver.add_clause(~i2,  o);
  solver.add_clause(~i3,  o);
  solver.add_clause( i0,  i1, i2, i3, ~o);
}

// @brief 2入力 XOR ゲートの入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバー
// @param[in] i0, i1 入力のリテラル
// @param[in] o 出力のリテラル
inline
void
make_xor2_cnf(SatSolver& solver,
	      Literal i0,
	      Literal i1,
	      Literal o)
{
  solver.add_clause( i0, ~i1,  o);
  solver.add_clause(~i0,  i1,  o);
  solver.add_clause( i0,  i1, ~o);
  solver.add_clause(~i0, ~i1, ~o);
}

// @brief 3入力 XOR ゲートの入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバー
// @param[in] i0, i1, i2 入力のリテラル
// @param[in] o 出力のリテラル
inline
void
make_xor3_cnf(SatSolver& solver,
	      Literal i0,
	      Literal i1,
	      Literal i2,
	      Literal o)
{
  solver.add_clause(~i0,  i1,  i2,  o);
  solver.add_clause( i0, ~i1,  i2,  o);
  solver.add_clause( i0,  i1, ~i2,  o);
  solver.add_clause( i0,  i1,  i2, ~o);
  solver.add_clause( i0, ~i1, ~i2, ~o);
  solver.add_clause(~i0,  i1, ~i2, ~o);
  solver.add_clause(~i0, ~i1,  i2, ~o);
  solver.add_clause(~i0, ~i1, ~i2,  o);
}

END_NONAMESPACE


// @brief コンストラクタ
// @param[in] sat_type SATソルバの種類を表す文字列
// @param[in] sat_option SATソルバに渡すオプション文字列
// @param[in] sat_outp SATソルバ用の出力ストリーム
SatEngine::SatEngine(const string& sat_type,
		     const string& sat_option,
		     ostream* sat_outp) :
  mSolver(sat_type, sat_option, sat_outp)
{
}

// @brief デストラクタ
SatEngine::~SatEngine()
{
}

// @brief NodeSet の内容に応じて正常回路のCNFを作る．
// @param[in] gval_cnf 正常回路用のデータ構造
// @param[in] node_set 対象のノード集合
//
// 具体的には tfo_tfi_node() を対象にする．
void
SatEngine::make_gval_cnf(GvalCnf& gval_cnf,
			 const NodeSet& node_set)
{
  ymuint n = node_set.tfo_tfi_size();
  for (ymuint i = 0; i < n; ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    if ( gval_cnf.mark(node) ) {
      continue;
    }
    VarId gvar = new_var();
    gval_cnf.set_var(node, gvar);
  }
  for (ymuint i = 0; i < n; ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    if ( gval_cnf.mark(node) ) {
      continue;
    }
    gval_cnf.set_mark(node);
    make_node_cnf(node, gval_cnf.var_map());
  }
}

// @brief ノードのTFI全体の正常回路のCNFを作る．
// @param[in] gval_cnf 正常回路用のデータ構造
// @param[in] node ノード
void
SatEngine::make_gval_cnf(GvalCnf& gval_cnf,
			 const TpgNode* node)
{
  if ( gval_cnf.mark(node) ) {
    return;
  }
  gval_cnf.set_mark(node);

  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const TpgNode* inode = node->fanin(i);
    make_gval_cnf(gval_cnf, inode);
  }

  VarId gvar = new_var();
  gval_cnf.set_var(node, gvar);
  make_node_cnf(node, gval_cnf.var_map());
}

// @brief 故障回路のCNFを作る．
// @param[in] fval_cnf 故障回路用のデータ構造
// @param[in] src_node 故障位置のノード
// @param[in] node_set 故障に関係するノード集合
// @param[in] detect 検出条件
//
// detect = kVal0: 検出しないCNFを作る．
//        = kVal1: 検出するCNFを作る．
//        = kValX: fd_var() で制御するCNFを作る．
void
SatEngine::make_fval_cnf(FvalCnf&  fval_cnf,
			 const TpgNode* src_node,
			 const NodeSet& node_set,
			 Val3 detect)
{
  make_gval_cnf(fval_cnf.gval_cnf(), node_set);

  const TpgNode* dom_node = node_set.dom_node();

  ymuint n = node_set.tfo_size();
  for (ymuint i = 0; i < n; ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    VarId fvar = new_var();
    VarId dvar = new_var();
    fval_cnf.set_fvar(node, fvar);
    fval_cnf.set_dvar(node, dvar);
  }
  ymuint n0 = node_set.tfo_tfi_size();
  for (ymuint i = n; i < n0; ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    fval_cnf.set_fvar(node, fval_cnf.gvar(node));
  }

  for (ymuint i = 0; i < n; ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);

    if ( node != src_node ) {
      // 故障回路のゲートの入出力関係を表すCNFを作る．
      make_node_cnf(node, fval_cnf.fvar_map());
    }

    // D-Chain 制約を作る．
    make_dchain_cnf(node, dom_node, fval_cnf.gvar_map(), fval_cnf.fvar_map(), fval_cnf.dvar_map());
  }

  const vector<const TpgNode*>& output_list = node_set.output_list();
  ymuint npo = output_list.size();

  if ( detect == kVal0 ) {
    for (ymuint i = 0; i < npo; ++ i) {
      const TpgNode* node = output_list[i];
      Literal dlit(fval_cnf.dvar(node));
      add_clause(~dlit);
    }
    if ( dom_node != NULL ) {
      Literal dlit(fval_cnf.dvar(dom_node));
      add_clause(~dlit);
    }
  }
  else if ( detect == kVal1 ) {
    tmp_lits_begin(npo + 1);
    for (ymuint i = 0; i < npo; ++ i) {
      const TpgNode* node = output_list[i];
      Literal dlit(fval_cnf.dvar(node));
      tmp_lits_add(dlit);
    }
    if ( dom_node != NULL ) {
      Literal dlit(fval_cnf.dvar(dom_node));
      tmp_lits_add(dlit);
    }
    tmp_lits_end();

    for (const TpgNode* node = src_node; node != NULL && node != dom_node; node = node->imm_dom()) {
      Literal dlit(fval_cnf.dvar(node));
      add_clause(dlit);
    }
  }
  else {
    VarId fdvar = new_var();
    fval_cnf.set_fdvar(fdvar);
    tmp_lits_begin(npo + 1);
    Literal fdlit(fdvar);
    for (ymuint i = 0; i < npo; ++ i) {
      const TpgNode* node = output_list[i];
      Literal dlit(fval_cnf.dvar(node));
      tmp_lits_add(dlit);
      add_clause(fdlit, ~dlit);
    }
    tmp_lits_add(~fdlit);
    tmp_lits_end();

    for (const TpgNode* node = src_node; node != NULL && node != dom_node; node = node->imm_dom()) {
      Literal dlit(fval_cnf.dvar(node));
      add_clause(~fdlit, dlit);
    }
  }
}

// @brief 故障回路のCNFを作る．
// @param[in] fval_cnf 故障回路用のデータ構造
// @param[in] fault 故障
// @param[in] node_set 故障に関係するノード集合
// @param[in] detect 検出条件
//
// detect = kVal0: 検出しないCNFを作る．
//        = kVal1: 検出するCNFを作る．
//        = kValX: fd_var() で制御するCNFを作る．
void
SatEngine::make_fval_cnf(FvalCnf&  fval_cnf,
			 const TpgFault* fault,
			 const NodeSet& node_set,
			 Val3 detect)
{
  make_fval_cnf(fval_cnf, fault->node(), node_set, detect);
  if ( detect == kVal1 ) {
    make_fault_cnf_d(fault, fval_cnf.gvar_map(), fval_cnf.fvar_map());
  }
  else {
    make_fault_cnf(fault, fval_cnf.gvar_map(), fval_cnf.fvar_map());
  }
}

// @brief 複数故障検出回路のCNFを作る．
// @param[in] mval_cnf 故障回路用のデータ構造
// @param[in] fault_list 故障リスト
// @param[in] fnode_list 故障を持つノードのリスト
// @param[in] node_set 故障に関係するノード集合
void
SatEngine::make_mval_cnf(MvalCnf& mval_cnf,
			 const vector<const TpgFault*>& fault_list,
			 const vector<const TpgNode*>& fnode_list,
			 const NodeSet& node_set)
{
  //////////////////////////////////////////////////////////////////////
  // 変数の割当
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    VarId gvar = new_var();
    mval_cnf.set_gvar(node, gvar);
    mval_cnf.set_fvar(node, gvar);
  }
  for (ymuint i = 0; i < node_set.tfo_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    VarId fvar = new_var();
    VarId dvar = new_var();
    mval_cnf.set_fvar(node, fvar);
    mval_cnf.set_dvar(node, dvar);
  }

  // 故障を活性化するとき true にする変数．
  ymuint nf = fault_list.size();
  mval_cnf.set_fault_num(nf);
  for (ymuint i = 0; i < nf; ++ i) {
    VarId fdvar = new_var();
    mval_cnf.set_fault_var(i, fdvar);
    const TpgFault* f = fault_list[i];
    int fval = f->val();
    const TpgNode* node = f->node();

    if ( f->is_output_fault() ) {
      mval_cnf.set_ofvar(node, fval, fdvar);
    }
    else {
      ymuint pos = f->pos();
      mval_cnf.set_ifvar(node, pos, fval, fdvar);
    }
  }

  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    make_node_cnf(node, mval_cnf.gvar_map());
  }

  //////////////////////////////////////////////////////////////////////
  // 故障回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);

    if ( mval_cnf.has_faults(node) ) {
      make_fnode_cnf(node, mval_cnf);
    }
    else {
      make_node_cnf(node, mval_cnf.fvar_map());
    }

    make_dchain_cnf(node, node_set.dom_node(), mval_cnf.gvar_map(), mval_cnf.fvar_map(), mval_cnf.dvar_map());
  }

  //////////////////////////////////////////////////////////////////////
  // 故障の検出条件
  //////////////////////////////////////////////////////////////////////
  ymuint npo = node_set.output_list().size();
  tmp_lits_begin(npo);
  for (ymuint i = 0; i < npo; ++ i) {
    const TpgNode* node = node_set.output_list()[i];
    Literal dlit(mval_cnf.dvar(node));
    tmp_lits_add(dlit);
  }
  tmp_lits_end();
}

// @brief 2つの変数の値が等しいという制約を追加する．
// @param[in] var1, var2 変数番号
void
SatEngine::add_eq_clause(VarId var1,
			 VarId var2)
{
  Literal lit1(var1);
  Literal lit2(var2);
  add_clause( lit1, ~lit2);
  add_clause(~lit1,  lit2);
}

// @brief 2つの変数の値が異なるという制約を追加する．
// @param[in] var1, var2 変数番号
void
SatEngine::add_diff_clause(VarId var1,
			   VarId var2)
{
  Literal lit1(var1);
  Literal lit2(var2);
  add_clause( lit1,  lit2);
  add_clause(~lit1, ~lit2);
}

// @brief 割当リストに従って値を固定する．
// @param[in] gval_cnf 正常回路用のデータ構造
// @param[in] assign_list 割当リスト
void
SatEngine::add_assignments(GvalCnf& gval_cnf,
			   const NodeValList& assign_list)
{
  ymuint n = assign_list.size();
  for (ymuint i = 0; i < n; ++ i) {
    NodeVal nv = assign_list[i];
    const TpgNode* node = nv.node();
    make_gval_cnf(gval_cnf, node);
    Literal alit(gval_cnf.var(node), false);
    if ( nv.val() ) {
      add_clause(alit);
    }
    else {
      add_clause(~alit);
    }
  }
}

// @brief 割当リストの否定の節を加える．
// @param[in] gval_cnf 正常回路用のデータ構造
// @param[in] assign_list 割当リスト
void
SatEngine::add_negation(GvalCnf& gval_cnf,
			const NodeValList& assign_list)
{
  ymuint n = assign_list.size();
  tmp_lits_begin(n);
  for (ymuint i = 0; i < n; ++ i) {
    NodeVal nv = assign_list[i];
    const TpgNode* node = nv.node();
    make_gval_cnf(gval_cnf, node);
    Literal alit(gval_cnf.var(node), false);
    if ( nv.val() ) {
      tmp_lits_add(~alit);
    }
    else {
      tmp_lits_add( alit);
    }
  }
  tmp_lits_end();
}

// @brief 割当リストに対応する仮定を追加する．
// @param[in] assumptions 仮定を表すリテラルのリスト
// @param[in] gval_cnf 正常回路用のデータ構造
// @param[in] assign_list 割当リスト
void
SatEngine::add_assumption(vector<Literal>& assumptions,
			  GvalCnf& gval_cnf,
			  const NodeValList& assign_list)
{
  ymuint n = assign_list.size();
  for (ymuint i = 0; i < n; ++ i) {
    NodeVal nv = assign_list[i];
    const TpgNode* node = nv.node();
    make_gval_cnf(gval_cnf, node);
    Literal alit(gval_cnf.var(node), false);
    if ( nv.val() ) {
      assumptions.push_back(alit);
    }
    else {
      assumptions.push_back(~alit);
    }
  }
}

// @brief ノードの入出力の関係を表すCNFを作る．
// @param[in] node 対象のノード
// @param[in] vid_map 変数番号のマップ
void
SatEngine::make_node_cnf(const TpgNode* node,
			 const VidMap& vid_map)
{
  if ( node->is_input() ) {
    ;
  }
  else if ( node->is_output() ) {
    make_gate_cnf(kTgGateBuff, VidLitMap(node, vid_map));
  }
  else if ( node->is_logic() ) {
    make_gate_cnf(node->gate_type(), VidLitMap(node, vid_map));
  }
  else {
    ASSERT_NOT_REACHED;
  }
}

// @brief 故障回路のノードの入出力の関係を表す CNF を作る．
// @param[in] engine SAT エンジン
// @param[in] node 対象のノード
void
SatEngine::make_fnode_cnf(const TpgNode* node,
			  const MvalCnf& mval_cnf)
{
  const VidMap& gvar_map = mval_cnf.gvar_map();
  const VidMap& fvar_map = mval_cnf.fvar_map();

  ymuint ni = node->fanin_num();
  vector<VarId> ivars(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    const TpgNode* inode = node->fanin(i);
    VarId f0_var = mval_cnf.ifvar(node, i, 0);
    VarId f1_var = mval_cnf.ifvar(node, i, 1);
    if ( f0_var == kVarIdIllegal ) {
      if ( f1_var == kVarIdIllegal ) {
	ivars[i] = fvar_map(inode);
      }
      else {
	VarId tmp_var = new_var();
	make_flt1_cnf(fvar_map(inode), f1_var, tmp_var);
	ivars[i] = tmp_var;
      }
    }
    else {
      if ( f1_var == kVarIdIllegal ) {
	VarId tmp_var = new_var();
	make_flt0_cnf(fvar_map(inode), f0_var, tmp_var);
	ivars[i] = tmp_var;
      }
      else {
	VarId tmp_var = new_var();
	make_flt01_cnf(fvar_map(inode), f0_var, f1_var, tmp_var);
	ivars[i] = tmp_var;
      }
    }
  }

  VarId f0_var = mval_cnf.ofvar(node, 0);
  VarId f1_var = mval_cnf.ofvar(node, 1);
  VarId ovar = fvar_map(node);
  if ( f0_var == kVarIdIllegal ) {
    if ( f1_var == kVarIdIllegal ) {
      ;
    }
    else {
      ovar = new_var();
      make_flt1_cnf(ovar, f1_var, fvar_map(node));
    }
  }
  else {
    if ( f1_var == kVarIdIllegal ) {
      ovar = new_var();
      make_flt0_cnf(ovar, f0_var, fvar_map(node));
    }
    else {
      ovar = new_var();
      make_flt01_cnf(ovar, f0_var, f1_var, fvar_map(node));
    }
  }

  if ( node->is_input() ) {
    Literal glit(gvar_map(node), false);
    Literal output(ovar, false);
    add_clause( glit, ~output);
    add_clause(~glit,  output);
  }
  else {
    make_gate_cnf(node->gate_type(), VectLitMap(ivars, ovar));
  }
}

// @brief 故障箇所の関係を表す CNF を作る．
// @param[in] fault 対象の故障
// @param[in] gvar_map 正常値の変数マップ
// @param[in] fvar_map 故障値の変数マップ
void
SatEngine::make_fault_cnf(const TpgFault* fault,
			  const VidMap& gvar_map,
			  const VidMap& fvar_map)
{
  const TpgNode* node = fault->node();
  int fval = fault->val();

  if ( fault->is_output_fault() ) {
    // 出力の故障の場合
    // ただ単に故障値を固定するだけ．
    Literal flit(fvar_map(node), false);
    if ( fval == 0 ) {
      add_clause(~flit);
    }
    else {
      add_clause( flit);
    }
  }
  else {
    // 入力の故障の場合
    // 故障値は非制御値のはずなので，
    // side input だけのゲートを仮定する．
    ymuint fpos = fault->pos();
    // fpos 以外の入力を ivars[] に入れる．
    ymuint ni = node->fanin_num();
    vector<VarId> ivars;
    ivars.reserve(ni - 1);
    for (ymuint i = 0; i < ni; ++ i) {
      if ( i == fpos ) {
	continue;
      }
      const TpgNode* inode = node->fanin(i);
      VarId ivar = gvar_map(inode);
      ivars.push_back(ivar);
    }

    VarId ovar = fvar_map(node);

    bool inv = false;
    switch ( node->gate_type() ) {
    case kTgGateBuff:
    case kTgGateNot:
      ASSERT_NOT_REACHED;
      break;

    case kTgGateNand:
      ASSERT_COND( fval == 1 );
      make_and_cnf(VectLitMap(ivars, ovar), true);
      break;

    case kTgGateAnd:
      ASSERT_COND( fval == 1 );
      make_and_cnf(VectLitMap(ivars, ovar), false);
      break;

    case kTgGateNor:
      ASSERT_COND( fval == 0 );
      make_or_cnf(VectLitMap(ivars, ovar), true);
      break;

    case kTgGateOr:
      ASSERT_COND( fval == 0 );
      make_or_cnf(VectLitMap(ivars, ovar), false);
      break;

    case kTgGateXnor:
      inv = true;
      // わざと次に続く

    case kTgGateXor:
      if ( fval == 1 ) {
	inv = !inv;
      }
      make_xor_cnf(VectLitMap(ivars, ovar), inv);
      break;

    default:
      ASSERT_NOT_REACHED;
      break;
    }
  }
}

// @brief 故障箇所の関係を表す CNF を作る．
// @param[in] fault 対象の故障
// @param[in] gvar_map 正常値の変数マップ
// @param[in] fvar_map 故障値の変数マップ
void
SatEngine::make_fault_cnf_d(const TpgFault* fault,
			    const VidMap& gvar_map,
			    const VidMap& fvar_map)
{
  const TpgNode* node = fault->node();
  int fval = fault->val();

  if ( fault->is_output_fault() ) {
    // 出力の故障の場合
    // ただ単に故障値を固定するだけ．
    Literal glit(gvar_map(node), false);
    Literal flit(fvar_map(node), false);
    if ( fval == 0 ) {
      add_clause( glit);
      add_clause(~flit);
    }
    else {
      add_clause(~glit);
      add_clause( flit);
    }
  }
  else {
    // 入力の故障の場合
    // 故障値は非制御値のはずなので，
    // side input だけのゲートを仮定する．
    ymuint fpos = fault->pos();
    // fpos 以外の入力を ivars[] に入れる．
    ymuint ni = node->fanin_num();
    vector<VarId> ivars;
    vector<Literal> ilits;
    ivars.reserve(ni - 1);
    ilits.reserve(ni - 1);
    for (ymuint i = 0; i < ni; ++ i) {
      if ( i == fpos ) {
	continue;
      }
      const TpgNode* inode = node->fanin(i);
      VarId ivar = gvar_map(inode);
      ivars.push_back(ivar);
      ilits.push_back(Literal(ivar));
    }

    VarId ofvar = fvar_map(node);
    Literal oglit(gvar_map(node));
    Literal oflit(ofvar);

    bool inv = false;
    switch ( node->gate_type() ) {
    case kTgGateBuff:
    case kTgGateNot:
      ASSERT_NOT_REACHED;
      break;

    case kTgGateNand:
      ASSERT_COND( fval == 1 );
      for (ymuint i = 0; i < ni - 1; ++ i) {
	add_clause(ilits[i]);
      }
      add_clause( oglit);
      add_clause(~oflit);
      break;

    case kTgGateAnd:
      ASSERT_COND( fval == 1 );
      for (ymuint i = 0; i < ni - 1; ++ i) {
	add_clause(ilits[i]);
      }
      add_clause(~oglit);
      add_clause( oflit);
      break;

    case kTgGateNor:
      ASSERT_COND( fval == 0 );
      for (ymuint i = 0; i < ni - 1; ++ i) {
	add_clause(~ilits[i]);
      }
      add_clause(~oglit);
      add_clause( oflit);
      break;

    case kTgGateOr:
      ASSERT_COND( fval == 0 );
      for (ymuint i = 0; i < ni - 1; ++ i) {
	add_clause(~ilits[i]);
      }
      add_clause( oglit);
      add_clause(~oflit);
      break;

    case kTgGateXnor:
      inv = true;
      // わざと次に続く

    case kTgGateXor:
      if ( fval == 1 ) {
	inv = !inv;
      }
      make_xor_cnf(VectLitMap(ivars, ofvar), inv);
      break;

    default:
      ASSERT_NOT_REACHED;
      break;
    }
  }
}

// @brief 故障伝搬条件を表すCNFを作る．
// @param[in] node 対象のノード
// @param[in] dst_node 伝搬条件の終点のノード
// @param[in] gvar_map 正常値の変数マップ
// @param[in] fvar_map 故障値の変数マップ
// @param[in] dvar_map 故障伝搬条件の変数マップ
void
SatEngine::make_dchain_cnf(const TpgNode* node,
			   const TpgNode* dst_node,
			   const VidMap& gvar_map,
			   const VidMap& fvar_map,
			   const VidMap& dvar_map)
{
  Literal glit(gvar_map(node), false);
  Literal flit(fvar_map(node), false);
  Literal dlit(dvar_map(node), false);

  // dlit -> XOR(glit, flit) を追加する．
  // 要するに dlit が 1 の時，正常回路と故障回路で異なっていなければならない．
  add_clause(~glit, ~flit, ~dlit);
  add_clause( glit,  flit, ~dlit);

  if ( node->is_output() || node == dst_node ) {
    // 出力ノードの場合，XOR(glit, flit) -> dlit となる．
    add_clause(~glit,  flit, dlit);
    add_clause( glit, ~flit, dlit);
  }
  else {
    // dlit が 1 の時，ファンアウトの dlit が最低1つは 1 でなければならない．
    ymuint nfo = node->active_fanout_num();
    tmp_lits_begin(nfo + 1);
    tmp_lits_add(~dlit);
    for (ymuint j = 0; j < nfo; ++ j) {
      const TpgNode* onode = node->active_fanout(j);
      Literal odlit(dvar_map(onode), false);
      tmp_lits_add(odlit);
    }
    tmp_lits_end();
  }
}

// @brief ゲートの入出力の関係を表す CNF を作る．
// @param[in] gate_type ゲートの種類
// @param[in] litmap 入出力のリテラルを保持するクラス
void
SatEngine::make_gate_cnf(tTgGateType gate_type,
			 const LitMap& litmap)
{
  switch ( gate_type ) {
  case kTgGateNot:
    make_buff_cnf(mSolver, litmap.input(0), ~litmap.output());
    return;

  case kTgGateBuff:
    make_buff_cnf(mSolver, litmap.input(0), litmap.output());
    return;

  case kTgGateNand:
    make_and_cnf(litmap, true);
    return;

  case kTgGateAnd:
    make_and_cnf(litmap, false);
    return;

  case kTgGateNor:
    make_or_cnf(litmap, true);
    return;

  case kTgGateOr:
    make_or_cnf(litmap, false);
    return;

  case kTgGateXnor:
    make_xor_cnf(litmap, true);
    return;

  case kTgGateXor:
    make_xor_cnf(litmap, false);
    return;

  default:
    break;
  }

  ASSERT_NOT_REACHED;
}

// @brief 故障挿入回路を表す CNF 式を作る．
// @param[in] ivar 入力の変数
// @param[in] fvar 故障変数
// @param[in] ovar 出力の変数
void
SatEngine::make_flt0_cnf(VarId ivar,
			 VarId fvar,
			 VarId ovar)
{
  Literal ilit(ivar, false);
  Literal flit(fvar, false);
  Literal olit(ovar, false);

  add_clause( ilit,        ~olit);
  add_clause(       ~flit, ~olit);
  add_clause(~ilit,  flit,  olit);
}

// @brief 故障挿入回路を表す CNF 式を作る．
// @param[in] solver SAT ソルバー
// @param[in] ivar 入力の変数
// @param[in] fvar 故障変数
// @param[in] ovar 出力の変数
void
SatEngine::make_flt1_cnf(VarId ivar,
			 VarId fvar,
			 VarId ovar)
{
  Literal ilit(ivar, false);
  Literal flit(fvar, false);
  Literal olit(ovar, false);

  add_clause(~ilit,         olit);
  add_clause(       ~flit,  olit);
  add_clause( ilit,  flit, ~olit);
}

// @brief 故障挿入回路を表す CNF 式を作る．
// @param[in] solver SAT ソルバー
// @param[in] ivar 入力の変数
// @param[in] fvar0 故障変数
// @param[in] fvar1 故障変数
// @param[in] ovar 出力の変数
void
SatEngine::make_flt01_cnf(VarId ivar,
			  VarId fvar0,
			  VarId fvar1,
			  VarId ovar)
{
  Literal ilit(ivar, false);
  Literal f0lit(fvar0, false);
  Literal f1lit(fvar1, false);
  Literal olit(ovar, false);

  add_clause(       ~f0lit,         ~olit);
  add_clause(               ~f1lit,  olit);
  add_clause( ilit,  f0lit,  f1lit, ~olit);
  add_clause(~ilit,  f0lit,  f1lit,  olit);
}

// @brief 多入力 AND ゲートの入出力の関係を表す CNF 式を生成する．
// @param[in] litmap 入出力のリテラルを返すファンクタオブジェクト
// @param[in] inv 出力が反転している時 true にするフラグ
void
SatEngine::make_and_cnf(const LitMap& litmap,
			bool inv)
{
  ymuint n = litmap.input_size();
  Literal output = inv ? ~litmap.output() : litmap.output();
  switch ( n ) {
  case 0: ASSERT_NOT_REACHED; break;
  case 1: make_buff_cnf(mSolver, litmap.input(0), output); return;
  case 2: make_and2_cnf(mSolver, litmap.input(0), litmap.input(1), output); return;
  case 3: make_and3_cnf(mSolver, litmap.input(0), litmap.input(1), litmap.input(2), output); return;
  case 4: make_and4_cnf(mSolver, litmap.input(0), litmap.input(1), litmap.input(2), litmap.input(3), output); return;
  default: break;
  }

  vector<Literal> tmp(n + 1);
  tmp_lits_begin(n + 1);
  for (ymuint i = 0; i < n; ++ i) {
    Literal input = litmap.input(i);
    add_clause(input, ~output);
    tmp_lits_add(~input);
  }
  tmp_lits_add(output);
  tmp_lits_end();
}

// @brief 多入力 OR ゲートの入出力の関係を表す CNF 式を生成する．
// @param[in] litmap 入出力のリテラルを返すファンクタオブジェクト
// @param[in] inv 出力が反転している時 true にするフラグ
void
SatEngine::make_or_cnf(const LitMap& litmap,
		       bool inv)
{
  ymuint n = litmap.input_size();
  Literal output = inv ? ~litmap.output() : litmap.output();
  switch ( n ) {
  case 0: ASSERT_NOT_REACHED; break;
  case 1: make_buff_cnf(mSolver, litmap.input(0), output); return;
  case 2: make_or2_cnf(mSolver, litmap.input(0), litmap.input(1), output); return;
  case 3: make_or3_cnf(mSolver, litmap.input(0), litmap.input(1), litmap.input(2), output); return;
  case 4: make_or4_cnf(mSolver, litmap.input(0), litmap.input(1), litmap.input(2), litmap.input(3), output); return;
  default: break;
  }

  tmp_lits_begin(n + 1);
  for (ymuint i = 0; i < n; ++ i) {
    Literal input = litmap.input(i);
    add_clause(~input, output);
    tmp_lits_add(input);
  }
  tmp_lits_add(~output);
  tmp_lits_end();
}

// @brief 多入力 XOR ゲートの入出力の関係を表す CNF 式を生成する．
// @param[in] litmap 入力のリテラルを返すファンクタオブジェクト
// @param[in] inv 出力が反転している時 true にするフラグ
void
SatEngine::make_xor_cnf(const LitMap& litmap,
			bool inv)
{
  ymuint n = litmap.input_size();
  Literal output = inv ? ~litmap.output() : litmap.output();
  switch ( n ) {
  case 0: ASSERT_NOT_REACHED; break;
  case 1: make_buff_cnf(mSolver, litmap.input(0), output); return;
  case 2: make_xor2_cnf(mSolver, litmap.input(0), litmap.input(1), output); return;
  case 3: make_xor3_cnf(mSolver, litmap.input(0), litmap.input(1), litmap.input(2), output); return;
  default: break;
  }

  VarId tmp_var = new_var();
  Literal tmp_lit(tmp_var, false);
  make_xor2_cnf(mSolver, litmap.input(0), litmap.input(1), tmp_lit);

  for (ymuint i = 2; i < n; ++ i) {
    Literal tmp_out;
    if ( i == n - 1 ) {
      tmp_out = output;
    }
    else {
      VarId tmp_var = new_var();
      tmp_out = Literal(tmp_var, false);
    }
    make_xor2_cnf(mSolver, litmap.input(i), tmp_lit, tmp_out);
    tmp_lit = tmp_out;
  }
}

END_NAMESPACE_YM_SATPG