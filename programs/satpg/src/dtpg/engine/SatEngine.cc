
/// @file SatEngine.cc
/// @brief SatEngine の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "SatEngine.h"
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

// @brief ノードの入出力の関係を表すCNFを作る．
// @param[in] node 対象のノード
// @param[in] vid_map 変数番号のマップ
void
SatEngine::make_node_cnf(TpgNode* node,
			 const VidMap& vid_map)
{
  if ( node->is_input() ) {
    ;
  }
  else if ( node->is_output() ) {
    make_gate_cnf(kTgGateBuff, VidLitMap(node, vid_map));
  }
  else if ( node->is_logic() ) {
    make_gate_cnf(node->gate_type(), VidLitMap(node,vid_map));
  }
  else {
    ASSERT_NOT_REACHED;
  }
}

// @brief 故障回路のノードの入出力の関係を表す CNF を作る．
// @param[in] engine SAT エンジン
// @param[in] node 対象のノード
void
SatEngine::make_fnode_cnf(TpgNode* node,
			  const VidMap& gvar_map,
			  const VidMap& fvar_map)
{
  ymuint ni = node->fanin_num();
  vector<VarId> ivars(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    TpgNode* inode = node->fanin(i);
    VarId f0_var = node->if0var(i);
    VarId f1_var = node->if1var(i);
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

  VarId f0_var = node->of0var();
  VarId f1_var = node->of1var();
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
SatEngine::make_fault_cnf(TpgFault* fault,
			  const VidMap& gvar_map,
			  const VidMap& fvar_map)
{
  TpgNode* node = fault->node();
  int fval = fault->val();

  if ( fault->is_output_fault() ) {
    // 出力の故障の場合
    // ただ単に故障値を固定するだけ．
    Literal flit(fvar_map(node), false);
    if ( fval == 0 ) {
      add_clause(~flit);
    }
    else {
      add_clause(flit);
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
      TpgNode* inode = node->fanin(i);
      VarId ivar = gvar_map(inode);
      if ( i != fpos ) {
	ivars.push_back(ivar);
      }
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

// @brief 故障伝搬条件を表すCNFを作る．
// @param[in] node 対象のノード
// @param[in] gvar_map 正常値の変数マップ
// @param[in] fvar_map 故障値の変数マップ
// @param[in] dvar_map 故障伝搬条件の変数マップ
void
SatEngine::make_dchain_cnf(TpgNode* node,
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

  if ( node->is_output() ) {
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
      TpgNode* onode = node->active_fanout(j);
      Literal odlit(dvar_map(onode), false);
      tmp_lits_add(odlit);
    }
    tmp_lits_end();

    // dominator の dlit が 0 なら自分も 0
    TpgNode* idom = node->imm_dom();
    if ( idom != NULL && idom != node->active_fanout(0) ) {
      Literal idlit(dvar_map(idom), false);
      add_clause(~dlit, idlit);
    }
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
