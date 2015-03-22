
/// @file SatEngine.cc
/// @brief SatEngine の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "SatEngine.h"

#include "TpgNode.h"
#include "TpgFault.h"
#include "YmLogic/SatSolver.h"


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

// @brief 多入力 AND ゲートの入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバー
// @param[in] litmap 入力のリテラルを返すファンクタオブジェクト
// @param[in] output 出力のリテラル
inline
void
make_and_cnf(SatSolver& solver,
	     const LitMap& litmap,
	     Literal output)
{
  ymuint n = litmap.input_size();
  switch ( n ) {
  case 0: ASSERT_NOT_REACHED; break;
  case 1: make_buff_cnf(solver, litmap.input(0), output); return;
  case 2: make_and2_cnf(solver, litmap.input(0), litmap.input(1), output); return;
  case 3: make_and3_cnf(solver, litmap.input(0), litmap.input(1), litmap.input(2), output); return;
  case 4: make_and4_cnf(solver, litmap.input(0), litmap.input(1), litmap.input(2), litmap.input(3), output); return;
  default: break;
  }

  vector<Literal> tmp(n + 1);
  for (ymuint i = 0; i < n; ++ i) {
    Literal input = litmap.input(i);
    solver.add_clause(input, ~output);
    tmp[i] = ~input;
  }
  tmp[n] = output;
  solver.add_clause(tmp);
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

// @brief 多入力 OR ゲートの入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバー
// @param[in] litmap 入力のリテラルを返すファンクタオブジェクト
// @param[in] output 出力のリテラル
inline
void
make_or_cnf(SatSolver& solver,
	    const LitMap& litmap,
	    Literal output)
{
  ymuint n = litmap.input_size();
  switch ( n ) {
  case 0: ASSERT_NOT_REACHED; break;
  case 1: make_buff_cnf(solver, litmap.input(0), output); return;
  case 2: make_or2_cnf(solver, litmap.input(0), litmap.input(1), output); return;
  case 3: make_or3_cnf(solver, litmap.input(0), litmap.input(1), litmap.input(2), output); return;
  case 4: make_or4_cnf(solver, litmap.input(0), litmap.input(1), litmap.input(2), litmap.input(3), output); return;
  default: break;
  }

  vector<Literal> tmp(n + 1);
  for (ymuint i = 0; i < n; ++ i) {
    solver.add_clause(~litmap.input(i), output);
    tmp[i] = litmap.input(i);
  }
  tmp[n] = ~output;
  solver.add_clause(tmp);
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

// @brief 多入力 XOR ゲートの入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバー
// @param[in] litmap 入力のリテラルを返すファンクタオブジェクト
// @param[in] output 出力のリテラル
inline
void
make_xor_cnf(SatSolver& solver,
	     const LitMap& litmap,
	     Literal output)
{
  ymuint n = litmap.input_size();
  switch ( n ) {
  case 0: ASSERT_NOT_REACHED; break;
  case 1: make_buff_cnf(solver, litmap.input(0), output); return;
  case 2: make_xor2_cnf(solver, litmap.input(0), litmap.input(1), output); return;
  case 3: make_xor3_cnf(solver, litmap.input(0), litmap.input(1), litmap.input(2), output); return;
  default: break;
  }

  VarId tmp_var = solver.new_var();
  Literal tmp_lit(tmp_var, false);
  make_xor2_cnf(solver, litmap.input(0), litmap.input(1), tmp_lit);

  for (ymuint i = 2; i < n; ++ i) {
    Literal tmp_out;
    if ( i == n - 1 ) {
      tmp_out = output;
    }
    else {
      VarId new_var = solver.new_var();
      tmp_out = Literal(new_var, false);
    }
    make_xor2_cnf(solver, litmap.input(i), tmp_lit, tmp_out);
    tmp_lit = tmp_out;
  }
}

// @brief ゲートの入出力の関係を表す CNF を作る．
// @param[in] solver SATソルバ
// @param[in] type ゲートの種類
// @param[in] litmap 入出力のリテラルを保持するクラス
inline
void
make_gate_cnf(SatSolver& solver,
	      tTgGateType type,
	      const LitMap& litmap,
	      Literal output)
{
  ymuint ni = litmap.input_size();
  switch ( type ) {
  case kTgGateNot:
    output = ~output;
    // わざと次に続く

  case kTgGateBuff:
    make_buff_cnf(solver, litmap.input(0), output);
    break;

  case kTgGateNand:
    output = ~output;
    // わざと次に続く
  case kTgGateAnd:
    make_and_cnf(solver, litmap, output);
    break;

  case kTgGateNor:
    output = ~output;
    // わざと次に続く

  case kTgGateOr:
    make_or_cnf(solver, litmap, output);
    break;

  case kTgGateXnor:
    output = ~output;
    // わざと次に続く

  case kTgGateXor:
    make_xor_cnf(solver, litmap, output);
    break;

  default:
    ASSERT_NOT_REACHED;
    break;
  }
}

// @brief ノードの入出力の関係を表す CNF を作る．
// @param[in] solver SATソルバ
// @param[in] node 対象のノード
// @param[in] litmap 入出力のリテラルを保持するクラス
inline
void
make_node_cnf(SatSolver& solver,
	      TpgNode* node,
	      const LitMap& litmap,
	      Literal output)
{
  if ( node->is_input() ) {
    return;
  }

  if ( node->is_output() ) {
    Literal input = litmap.input(0);
    make_buff_cnf(solver, input, output);
    return;
  }

  if ( node->is_logic() ) {
    make_gate_cnf(solver, node->gate_type(), litmap, output);
    return;
  }
}

// @brief 故障挿入回路を表す CNF 式を作る．
// @param[in] solver SAT ソルバー
// @param[in] ivar 入力の変数
// @param[in] fvar 故障変数
// @param[in] ovar 出力の変数
inline
void
make_flt0_cnf(SatSolver& solver,
	      VarId ivar,
	      VarId fvar,
	      VarId ovar)
{
  Literal ilit(ivar, false);
  Literal flit(fvar, false);
  Literal olit(ovar, false);

  solver.add_clause( ilit,        ~olit);
  solver.add_clause(       ~flit, ~olit);
  solver.add_clause(~ilit,  flit,  olit);
}

// @brief 故障挿入回路を表す CNF 式を作る．
// @param[in] solver SAT ソルバー
// @param[in] ivar 入力の変数
// @param[in] fvar 故障変数
// @param[in] ovar 出力の変数
inline
void
make_flt1_cnf(SatSolver& solver,
	      VarId ivar,
	      VarId fvar,
	      VarId ovar)
{
  Literal ilit(ivar, false);
  Literal flit(fvar, false);
  Literal olit(ovar, false);

  solver.add_clause(~ilit,         olit);
  solver.add_clause(       ~flit,  olit);
  solver.add_clause( ilit,  flit, ~olit);
}

// @brief 故障挿入回路を表す CNF 式を作る．
// @param[in] solver SAT ソルバー
// @param[in] ivar 入力の変数
// @param[in] fvar0 故障変数
// @param[in] fvar1 故障変数
// @param[in] ovar 出力の変数
inline
void
make_flt01_cnf(SatSolver& solver,
	       VarId ivar,
	       VarId fvar0,
	       VarId fvar1,
	       VarId ovar)
{
  Literal ilit(ivar, false);
  Literal f0lit(fvar0, false);
  Literal f1lit(fvar1, false);
  Literal olit(ovar, false);

  solver.add_clause(       ~f0lit,         ~olit);
  solver.add_clause(               ~f1lit,  olit);
  solver.add_clause( ilit,  f0lit,  f1lit, ~olit);
  solver.add_clause(~ilit,  f0lit,  f1lit,  olit);
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

// @brief 正常回路のノードの入出力の関係を表す CNF を作る．
// @param[in] node 対象のノード
void
SatEngine::make_gnode_cnf(TpgNode* node)
{
  Literal output(node->gvar(), false);
  make_node_cnf(mSolver, node, GvarLitMap(node), output);
}

// @brief 故障回路のノードの入出力の関係を表す CNF を作る．
// @param[in] node 対象のノード
void
SatEngine::make_fnode_cnf(TpgNode* node)
{
  // node そのものには故障箇所がない場合は
  // ただ fvar を使った CNF を作れば良い．
  Literal output(node->fvar(), false);
  make_node_cnf(mSolver, node, FvarLitMap(node), output);
}

// @brief 故障回路のノードの入出力の関係を表す CNF を作る．
// @param[in] node 対象のノード
void
SatEngine::make_fnode_cnf2(TpgNode* node)
{
  ymuint ni = node->fanin_num();
  vector<VarId> ivars(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    TpgNode* inode = node->fanin(i);
    VarId f0_var = node->if0var(i);
    VarId f1_var = node->if1var(i);
    if ( f0_var == kVarIdIllegal ) {
      if ( f1_var == kVarIdIllegal ) {
	ivars[i] = inode->fvar();
      }
      else {
	VarId tmp_var = mSolver.new_var();
	make_flt1_cnf(mSolver, inode->fvar(), f1_var, tmp_var);
	ivars[i] = tmp_var;
      }
    }
    else {
      if ( f1_var == kVarIdIllegal ) {
	VarId tmp_var = mSolver.new_var();
	make_flt0_cnf(mSolver, inode->fvar(), f0_var, tmp_var);
	ivars[i] = tmp_var;
      }
      else {
	VarId tmp_var = mSolver.new_var();
	make_flt01_cnf(mSolver, inode->fvar(), f0_var, f1_var, tmp_var);
	ivars[i] = tmp_var;
      }
    }
  }

  VarId f0_var = node->of0var();
  VarId f1_var = node->of1var();
  VarId ovar = node->fvar();
  if ( f0_var == kVarIdIllegal ) {
    if ( f1_var == kVarIdIllegal ) {
      ;
    }
    else {
      ovar = mSolver.new_var();
      make_flt1_cnf(mSolver, ovar, f1_var, node->fvar());
    }
  }
  else {
    if ( f1_var == kVarIdIllegal ) {
      ovar = mSolver.new_var();
      make_flt0_cnf(mSolver, ovar, f0_var, node->fvar());
    }
    else {
      ovar = mSolver.new_var();
      make_flt01_cnf(mSolver, ovar, f0_var, f1_var, node->fvar());
    }
  }
  Literal output(ovar, false);

  if ( node->is_input() ) {
    Literal glit(node->gvar(), false);
    make_buff_cnf(mSolver, glit, output);
  }
  else {
    make_node_cnf(mSolver, node, VectLitMap(ivars), output);
  }
}

// @brief 故障ゲートの CNF を作る．
void
SatEngine::make_fault_cnf(TpgFault* fault)
{
  TpgNode* node = fault->node();
  int fval = fault->val();

  if ( fault->is_output_fault() ) {
    Literal flit(node->fvar(), false);
    if ( fval == 0 ) {
      mSolver.add_clause(~flit);
    }
    else {
      mSolver.add_clause(flit);
    }
  }
  else {
    ymuint fpos = fault->pos();

    Literal output(node->fvar(), false);

    // fpos 以外の入力を ivars[] に入れる．
    ymuint ni = node->fanin_num();
    vector<VarId> ivars;
    ivars.reserve(ni - 1);
    for (ymuint i = 0; i < ni; ++ i) {
      if ( i == fpos ) {
	continue;
      }
      TpgNode* inode = node->fanin(i);
      ivars.push_back(inode->gvar());
    }

    switch ( node->gate_type() ) {
    case kTgGateBuff:
    case kTgGateNot:
      ASSERT_NOT_REACHED;
      break;

    case kTgGateNand:
      output = ~output;
      // わざと次に続く

    case kTgGateAnd:
      ASSERT_COND( fval == 1 );
      make_and_cnf(mSolver, VectLitMap(ivars), output);
      break;

    case kTgGateNor:
      output = ~output;
      // わざと次に続く

    case kTgGateOr:
      ASSERT_COND( fval == 0 );
      make_or_cnf(mSolver, VectLitMap(ivars), output);
      break;

    case kTgGateXnor:
      output = ~output;
      // わざと次に続く

    case kTgGateXor:
      if ( fval == 1 ) {
	output = ~output;
      }
      make_xor_cnf(mSolver, VectLitMap(ivars), output);
      break;

    default:
      ASSERT_NOT_REACHED;
      break;
    }
  }
}

// @brief ノードの故障差関数を表すCNFを作る．
void
SatEngine::make_dlit_cnf(TpgNode* node)
{
  Literal dlit(node->dvar());

  ymuint ni = node->fanin_num();

  if ( node->is_logic() ) {
    // ゲートの種類ごとの処理
    switch ( node->gate_type() ) {
    case kTgGateBuff:
    case kTgGateNot:
      // なにもしない．
      break;

    case kTgGateAnd:
    case kTgGateNand:
      for (ymuint i = 0; i < ni; ++ i) {
	TpgNode* inode = node->fanin(i);
	if ( inode->has_fvar() ) {
	  // side input が 0 かつ故障差が伝搬していなければ dlit も 0
	  Literal iglit(inode->gvar(), false);
	  Literal iflit(inode->fvar(), false);
	  mSolver.add_clause(iglit, iflit, ~dlit);
	}
	else {
	  // side input が 0 なら dlit も 0
	  Literal iglit(inode->gvar(), false);
	  mSolver.add_clause(iglit, ~dlit);
	}
      }
      break;

    case kTgGateOr:
    case kTgGateNor:
      for (ymuint i = 0; i < ni; ++ i) {
	TpgNode* inode = node->fanin(i);
	if ( inode->has_fvar() ) {
	  // side input が 1 かつ故障差が伝搬していなければ dlit も 0
	  Literal iglit(inode->gvar(), false);
	  Literal iflit(inode->fvar(), false);
	  mSolver.add_clause(~iglit, ~iflit, ~dlit);
	}
	else {
	  // side input が 1 なら dlit も 0
	  Literal iglit(inode->gvar(), false);
	  mSolver.add_clause(~iglit, ~dlit);
	}
      }
      break;

    case kTgGateXor:
    case kTgGateXnor:
      // なにもしない．
      break;

    default:
      ASSERT_NOT_REACHED;
      break;
    }
  }

  // 全ゲートタイプ共通
  // 全てのファンインに故障差が伝搬していなければ
  // このゲートの出力にも故障差は伝搬しない．
  tmp_lits_begin(ni + 1);
  tmp_lits_add(~dlit);
  for (ymuint i = 0; i < ni; ++ i) {
    TpgNode* inode = node->fanin(i);
    if ( inode->has_fvar() ) {
      Literal idlit(inode->dvar(), false);
      tmp_lits_add(idlit);
    }
  }
  tmp_lits_end();
}

// @brief 故障伝搬条件を表すCNFを作る．
void
SatEngine::make_dchain_cnf(TpgNode* node)
{
  Literal glit(node->gvar(), false);
  Literal flit(node->fvar(), false);
  Literal dlit(node->dvar(), false);

  // dlit -> XOR(glit, flit) を追加する．
  // 要するに dlit が 1 の時，正常回路と故障回路で異なっていなければならない．
  add_clause(~glit, ~flit, ~dlit);
  add_clause( glit,  flit, ~dlit);

  if ( !node->is_output() ) {
    // dlit が 1 の時，ファンアウトの dlit が最低1つは 1 でなければならない．
    ymuint nfo = node->active_fanout_num();
    tmp_lits_begin(nfo + 1);
    tmp_lits_add(~dlit);
    for (ymuint j = 0; j < nfo; ++ j) {
      TpgNode* onode = node->active_fanout(j);
      tmp_lits_add(Literal(onode->dvar(), false));
    }
    tmp_lits_end();

    // dominator の dlit が 0 なら自分も 0
    for (TpgNode* idom = node->imm_dom();
	 idom != NULL; idom = idom->imm_dom() ) {
      Literal idlit(idom->dvar(), false);
      add_clause(~dlit, idlit);
    }
  }
}

END_NAMESPACE_YM_SATPG
