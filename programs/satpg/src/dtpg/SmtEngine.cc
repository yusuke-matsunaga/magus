
/// @file SmtEngine.cc
/// @brief SmtEngine の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "SmtEngine.h"

#include "DetectOp.h"
#include "UntestOp.h"
#include "DtpgStats.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "BackTracer.h"
#include "LitMap.h"
#include "../ymsat/YmSat.h"
#include "logic/SatStats.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

// @brief ノードに正常回路用の変数を設定する．
// @param[in] solver SAT ソルバー
// @param[in] node 対象のノード
void
set_gvar(YmSat& solver,
	 TpgNode* node)
{
  // ノードそのものに割り当てる．
  VarId gvar = solver.new_var();
  node->set_gvar(gvar);
}

// @brief ノードに正常回路用の変数を設定する．
// @param[in] solver SAT ソルバー
// @param[in] node 対象のノード
void
set_fvar(YmSat& solver,
	 TpgNode* node)
{
  // ノードそのものに割り当てる．
  VarId fvar = solver.new_var();
  VarId dvar = solver.new_var();
  node->set_fvar(fvar, dvar);
}

// バッファの入出力の関係を表す CNF 式を生成する．
inline
void
make_buff_cnf(YmSat& solver,
	      Literal i,
	      Literal o)
{
  solver.add_clause( i, ~o);
  solver.add_clause(~i,  o);
}

// 2入力 AND ゲートの入出力の関係を表す CNF 式を生成する．
inline
void
make_and2_cnf(YmSat& solver,
	      Literal i0,
	      Literal i1,
	      Literal o)
{
  solver.add_clause( i0, ~o);
  solver.add_clause( i1, ~o);
  solver.add_clause(~i0, ~i1, o);
}

// 3入力 AND ゲートの入出力の関係を表す CNF 式を生成する．
inline
void
make_and3_cnf(YmSat& solver,
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

// 4入力 AND ゲートの入出力の関係を表す CNF 式を生成する．
inline
void
make_and4_cnf(YmSat& solver,
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

// 多入力 AND ゲートの入出力の関係を表す CNF 式を生成する．
inline
void
make_and_cnf(YmSat& solver,
	     const LitMap& litmap,
	     Literal output)
{
  ymuint n = litmap.input_size();
  switch ( n ) {
  case 0: assert_not_reached(__FILE__, __LINE__); break;
  case 1: make_buff_cnf(solver, litmap.input(0), output); return;
  case 2: make_and2_cnf(solver, litmap.input(0), litmap.input(1), output); return;
  case 3: make_and3_cnf(solver, litmap.input(0), litmap.input(1), litmap.input(2), output); return;
  case 4: make_and4_cnf(solver, litmap.input(0), litmap.input(1), litmap.input(2), litmap.input(3), output); return;
  default: break;
  }

  vector<Literal> tmp(n + 1);
  for (ymuint i = 0; i < n; ++ i) {
    solver.add_clause(litmap.input(i), ~output);
    tmp[i] = ~litmap.input(i);
  }
  tmp[n] = output;
  solver.add_clause(tmp);
}

// 2入力 OR ゲートの入出力の関係を表す CNF 式を生成する．
inline
void
make_or2_cnf(YmSat& solver,
	     Literal i0,
	     Literal i1,
	     Literal o)
{
  solver.add_clause(~i0,  o);
  solver.add_clause(~i1,  o);
  solver.add_clause( i0,  i1, ~o);
}

// 3入力 OR ゲートの入出力の関係を表す CNF 式を生成する．
inline
void
make_or3_cnf(YmSat& solver,
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

// 4入力 OR ゲートの入出力の関係を表す CNF 式を生成する．
inline
void
make_or4_cnf(YmSat& solver,
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

// 多入力 OR ゲートの入出力の関係を表す CNF 式を生成する．
inline
void
make_or_cnf(YmSat& solver,
	    const LitMap& litmap,
	    Literal output)
{
  ymuint n = litmap.input_size();
  switch ( n ) {
  case 0: assert_not_reached(__FILE__, __LINE__); break;
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

// 2入力 XOR ゲートの入出力の関係を表す CNF 式を生成する．
inline
void
make_xor2_cnf(YmSat& solver,
	      Literal i0,
	      Literal i1,
	      Literal o)
{
  solver.add_clause( i0, ~i1,  o);
  solver.add_clause(~i0,  i1,  o);
  solver.add_clause( i0,  i1, ~o);
  solver.add_clause(~i0, ~i1, ~o);
}

// 3入力 XOR ゲートの入出力の関係を表す CNF 式を生成する．
inline
void
make_xor3_cnf(YmSat& solver,
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

// 多入力 XOR ゲートの入出力の関係を表す CNF 式を生成する．
inline
void
make_xor_cnf(YmSat& solver,
	     const LitMap& litmap,
	     Literal output)
{
  ymuint n = litmap.input_size();
  switch ( n ) {
  case 0: assert_not_reached(__FILE__, __LINE__); break;
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

END_NONAMESPACE


// @brief コンストラクタ
SmtEngine::SmtEngine()
{
  mOutP = NULL;
  mTimerEnable = false;
}

// @brief デストラクタ
SmtEngine::~SmtEngine()
{
}

// @brief 使用する SAT エンジンを指定する．
void
SmtEngine::set_mode(const string& type,
		    const string& option,
		    ostream* outp)
{
  mType = type;
  mOption = option;
  mOutP = outp;
}

// @brief 統計情報をクリアする．
void
SmtEngine::clear_stats()
{
  mStats.mCnfGenCount = 0;
  mStats.mCnfGenTime.set(0.0, 0.0, 0.0);

  mStats.mDetCount = 0;
  mStats.mDetTime.set(0.0, 0.0, 0.0);
  clear_sat_stats(mStats.mDetStats);
  clear_sat_stats(mStats.mDetStatsMax);

  mStats.mRedCount = 0;
  mStats.mRedTime.set(0.0, 0.0, 0.0);
  clear_sat_stats(mStats.mRedStats);
  clear_sat_stats(mStats.mRedStatsMax);

  mStats.mAbortCount = 0;
  mStats.mAbortTime.set(0.0, 0.0, 0.0);
}

// @brief 統計情報を得る．
// @param[in] stats 結果を格納する構造体
void
SmtEngine::get_stats(DtpgStats& stats) const
{
  stats = mStats;
}

// @breif 時間計測を制御する．
void
SmtEngine::timer_enable(bool enable)
{
  mTimerEnable = enable;
}

BEGIN_NONAMESPACE

struct Lt
{
  bool
  operator()(TpgNode* left,
	     TpgNode* right)
  {
    return left->output_id2() < right->output_id2();
  }
};

END_NONAMESPACE

// @brief 故障位置を与えてその TFO の TFI リストを作る．
// @param[in] solver SAT ソルバ
// @param[in] fnode_list 故障位置のノードのリスト
// @param[in] max_id ノード番号の最大値
//
// 結果は mTfoList に格納される．
// 故障位置の TFO が mTfoList の [0: mTfoEnd1 - 1] に格納される．
void
SmtEngine::mark_region(YmSat& solver,
		       const vector<TpgNode*>& fnode_list,
		       ymuint max_id)
{
  mMarkArray.clear();
  mMarkArray.resize(max_id, 0U);

  mTfoList.clear();
  mTfoList.reserve(max_id);

  mInputList.clear();
  mOutputList.clear();

  // 故障のあるノードの TFO を mTfoList に入れる．
  // TFO の TFI のノードを mTfiList に入れる．
  ymuint nf = fnode_list.size();
  for (ymuint i = 0; i < nf; ++ i) {
    TpgNode* fnode = fnode_list[i];
    if ( !tfo_mark(fnode) ) {
      set_tfo_mark(fnode);
      if ( fnode->is_input() ) {
	mInputList.push_back(fnode);
      }
      set_gvar(solver, fnode);
      set_fvar(solver, fnode);
    }
  }

  for (ymuint rpos = 0; rpos < mTfoList.size(); ++ rpos) {
    TpgNode* node = mTfoList[rpos];
    ymuint nfo = node->active_fanout_num();
    for (ymuint i = 0; i < nfo; ++ i) {
      TpgNode* fonode = node->active_fanout(i);
      if ( !tfo_mark(fonode) ) {
	set_tfo_mark(fonode);
	set_gvar(solver, fonode);
	set_fvar(solver, fonode);
      }
    }
  }

  mTfoEnd = mTfoList.size();
  for (ymuint rpos = 0; rpos < mTfoList.size(); ++ rpos) {
    TpgNode* node = mTfoList[rpos];
    ymuint ni = node->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      TpgNode* finode = node->fanin(i);
      if ( !tfo_mark(finode) && !tfi_mark(finode) ) {
	set_tfi_mark(finode);
	set_gvar(solver, finode);
      }
    }
  }

  sort(mOutputList.begin(), mOutputList.end(), Lt());
}

// @brief 節の作成用の作業領域の使用を開始する．
// @param[in] exp_size 予想されるサイズ
void
SmtEngine::tmp_lits_begin(ymuint exp_size)
{
  mTmpLits.clear();
  if ( exp_size > 0 ) {
    mTmpLits.reserve(exp_size);
  }
}

// @brief 作業領域にリテラルを追加する．
void
SmtEngine::tmp_lits_add(Literal lit)
{
  mTmpLits.push_back(lit);
}

// @brief 作業領域の冊を SAT ソルバに加える．
void
SmtEngine::tmp_lits_end(YmSat& solver)
{
  solver.add_clause(mTmpLits);
}

// @brief タイマーをスタートする．
void
SmtEngine::cnf_begin()
{
  if ( mTimerEnable ) {
    mTimer.reset();
    mTimer.start();
  }
}

// @brief タイマーを止めて CNF 作成時間に加える．
void
SmtEngine::cnf_end()
{
  if ( mTimerEnable ) {
    mTimer.stop();
    mStats.mCnfGenTime += mTimer.time();
  }
  ++ mStats.mCnfGenCount;
}

// @brief 正常回路のノードの入出力の関係を表す CNF を作る．
// @param[in] solver SATソルバ
// @param[in] node 対象のノード
void
SmtEngine::make_gnode_cnf(YmSat& solver,
			  TpgNode* node)
{
  Literal output(node->gvar(), false);
  make_node_cnf(solver, node, GvarLitMap(node), output);
}

// @brief 故障回路のノードの入出力の関係を表す CNF を作る．
// @param[in] solver SATソルバ
// @param[in] node 対象のノード
void
SmtEngine::make_fnode_cnf(YmSat& solver,
			  TpgNode* node)
{
  if ( !node->has_flt_var() ) {
    Literal output(node->fvar(), false);
    make_node_cnf(solver, node, FvarLitMap(node), output);
    return;
  }
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
	VarId tmp_var = solver.new_var();
	make_flt1_cnf(solver, inode->fvar(), f1_var, tmp_var);
	ivars[i] = tmp_var;
      }
    }
    else {
      if ( f1_var == kVarIdIllegal ) {
	VarId tmp_var = solver.new_var();
	make_flt0_cnf(solver, inode->fvar(), f0_var, tmp_var);
	ivars[i] = tmp_var;
      }
      else {
	VarId tmp_var = solver.new_var();
	make_flt01_cnf(solver, inode->fvar(), f0_var, f1_var, tmp_var);
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
      ovar = solver.new_var();
      make_flt1_cnf(solver, ovar, f1_var, node->fvar());
    }
  }
  else {
    if ( f1_var == kVarIdIllegal ) {
      ovar = solver.new_var();
      make_flt0_cnf(solver, ovar, f0_var, node->fvar());
    }
    else {
      ovar = solver.new_var();
      make_flt01_cnf(solver, ovar, f0_var, f1_var, node->fvar());
    }
  }
  Literal output(ovar, false);

  if ( node->is_input() ) {
    Literal glit(node->gvar(), false);
    make_buff_cnf(solver, glit, output);
  }
  else {
    make_node_cnf(solver, node, VectLitMap(ivars), output);
  }
}

// @brief 故障ゲートの CNF を作る．
void
SmtEngine::make_fault_cnf(YmSat& solver,
			  TpgFault* fault)
{
  TpgNode* node = fault->node();
  int fval = fault->val();

  if ( fault->is_output_fault() ) {
    Literal flit(node->fvar(), false);
    if ( fval == 0 ) {
      solver.add_clause(~flit);
    }
    else {
      solver.add_clause(flit);
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
      assert_not_reached(__FILE__, __LINE__);
      break;

    case kTgGateNand:
      output = ~output;
      // わざと次に続く

    case kTgGateAnd:
      assert_cond( fval == 1, __FILE__, __LINE__);
      make_and_cnf(solver, VectLitMap(ivars), output);
      break;

    case kTgGateNor:
      output = ~output;
      // わざと次に続く

    case kTgGateOr:
      assert_cond( fval == 0, __FILE__, __LINE__);
      make_or_cnf(solver, VectLitMap(ivars), output);
      break;

    case kTgGateXnor:
      output = ~output;
      // わざと次に続く

    case kTgGateXor:
      if ( fval == 1 ) {
	output = ~output;
      }
      make_xor_cnf(solver, VectLitMap(ivars), output);
      break;

    default:
      assert_not_reached(__FILE__, __LINE__);
      break;
    }
  }
}

// @brief ノードの入出力の関係を表す CNF を作る．
// @param[in] solver SATソルバ
// @param[in] node 対象のノード
// @param[in] litmap 入出力のリテラルを保持するクラス
void
SmtEngine::make_node_cnf(YmSat& solver,
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

// @brief ゲートの入出力の関係を表す CNF を作る．
// @param[in] solver SATソルバ
// @param[in] type ゲートの種類
// @param[in] litmap 入出力のリテラルを保持するクラス
void
SmtEngine::make_gate_cnf(YmSat& solver,
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
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
}

// @brief ノードの故障差関数を表すCNFを作る．
void
SmtEngine::make_dlit_cnf(YmSat& solver,
			 TpgNode* node)
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
	  solver.add_clause(iglit, iflit, ~dlit);
	}
	else {
	  // side input が 0 なら dlit も 0
	  Literal iglit(inode->gvar(), false);
	  solver.add_clause(iglit, ~dlit);
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
	  solver.add_clause(~iglit, ~iflit, ~dlit);
	}
	else {
	  // side input が 1 なら dlit も 0
	  Literal iglit(inode->gvar(), false);
	  solver.add_clause(~iglit, ~dlit);
	}
      }
      break;

    case kTgGateXor:
    case kTgGateXnor:
      // なにもしない．
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
  tmp_lits_end(solver);
}

// @brief 故障挿入回路を表す CNF 式を作る．
// @param[in] solver SAT ソルバー
// @param[in] ivar 入力の変数
// @param[in] fvar 故障変数
// @param[in] ovar 出力の変数
void
SmtEngine::make_flt0_cnf(YmSat& solver,
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
void
SmtEngine::make_flt1_cnf(YmSat& solver,
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
void
SmtEngine::make_flt01_cnf(YmSat& solver,
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

// @brief 一つの SAT問題を解く．
Bool3
SmtEngine::solve(YmSat& solver,
		 TpgFault* f,
		 BackTracer& bt,
		 DetectOp& dop,
		 UntestOp& uop)
{
  SatStats prev_stats;
  solver.get_stats(prev_stats);

  USTime time(0, 0, 0);
  Bool3 ans = _solve(solver, time);

  SatStats sat_stats;
  solver.get_stats(sat_stats);

  sub_sat_stats(sat_stats, prev_stats);

  if ( ans == kB3True ) {
    // パタンが求まった．

    // バックトレースを行う．
    TestVector* tv = bt(f->node(), mModel, mInputList, mOutputList);

    // パタンの登録などを行う．
    dop(f, tv);

    stats_detect(sat_stats, time);
  }
  else if ( ans == kB3False ) {
    // 検出不能と判定された．
    uop(f);

    stats_undetect(sat_stats, time);
  }
  else { // ans == kB3X つまりアボート
    stats_abort(sat_stats, time);
  }

  return ans;
}

// @brief 一つの SAT問題を解く．
Bool3
SmtEngine::_solve(YmSat& solver,
		  USTime& time)
{
  if ( mTimerEnable ) {
    mTimer.reset();
    mTimer.start();
  }

  Bool3 ans = solver.solve(mTmpLits, mModel);

  if ( mTimerEnable ) {
    mTimer.stop();
    time += mTimer.time();
  }

  return ans;
}

// @brief 検出した場合の処理
void
SmtEngine::stats_detect(const SatStats& sat_stats,
			const USTime& time)
{
  ++ mStats.mDetCount;
  mStats.mDetTime += time;
  add_sat_stats(mStats.mDetStats, sat_stats);
  max_sat_stats(mStats.mDetStatsMax, sat_stats);
}

// @brief 検出不能と判定した場合の処理
void
SmtEngine::stats_undetect(const SatStats& sat_stats,
			  const USTime& time)
{
  ++ mStats.mRedCount;
  mStats.mRedTime += time;
  add_sat_stats(mStats.mRedStats, sat_stats);
  max_sat_stats(mStats.mRedStatsMax, sat_stats);
}

// @brief アボートした場合の処理
void
SmtEngine::stats_abort(const SatStats& sat_stats,
		       const USTime& time)
{
  ++ mStats.mAbortCount;
  mStats.mAbortTime += time;
}

// SatStats をクリアする．
void
SmtEngine::clear_sat_stats(SatStats& stats)
{
  stats.mRestart = 0;
  stats.mVarNum = 0;
  stats.mConstrClauseNum = 0;
  stats.mConstrLitNum = 0;
  stats.mLearntClauseNum = 0;
  stats.mLearntLitNum = 0;
  stats.mConflictNum = 0;
  stats.mDecisionNum = 0;
  stats.mPropagationNum = 0;
}

// SatStats をたす．
void
SmtEngine::add_sat_stats(SatStats& dst_stats,
			 const SatStats& src_stats)
{
  dst_stats.mRestart += src_stats.mRestart;
  dst_stats.mVarNum += src_stats.mVarNum;
  dst_stats.mConstrClauseNum += src_stats.mConstrClauseNum;
  dst_stats.mConstrLitNum += src_stats.mConstrLitNum;
  dst_stats.mLearntClauseNum += src_stats.mLearntClauseNum;
  dst_stats.mLearntLitNum += src_stats.mLearntLitNum;
  dst_stats.mConflictNum += src_stats.mConflictNum;
  dst_stats.mDecisionNum += src_stats.mDecisionNum;
  dst_stats.mPropagationNum += src_stats.mPropagationNum;
}

// SatStats を引く
void
SmtEngine::sub_sat_stats(SatStats& dst_stats,
			 const SatStats& src_stats)
{
  dst_stats.mRestart -= src_stats.mRestart;
  dst_stats.mVarNum -= src_stats.mVarNum;
  dst_stats.mConstrClauseNum -= src_stats.mConstrClauseNum;
  dst_stats.mConstrLitNum -= src_stats.mConstrLitNum;
  dst_stats.mLearntClauseNum -= src_stats.mLearntClauseNum;
  dst_stats.mLearntLitNum -= src_stats.mLearntLitNum;
  dst_stats.mConflictNum -= src_stats.mConflictNum;
  dst_stats.mDecisionNum -= src_stats.mDecisionNum;
  dst_stats.mPropagationNum -= src_stats.mPropagationNum;
}

// SatStats の各々の最大値をとる．
void
SmtEngine::max_sat_stats(SatStats& dst_stats,
			 const SatStats& src_stats)
{
  if ( dst_stats.mRestart < src_stats.mRestart ) {
    dst_stats.mRestart = src_stats.mRestart;
  }
  if ( dst_stats.mVarNum < src_stats.mVarNum ) {
    dst_stats.mVarNum += src_stats.mVarNum;
  }
  if ( dst_stats.mConstrClauseNum < src_stats.mConstrClauseNum ) {
    dst_stats.mConstrClauseNum += src_stats.mConstrClauseNum;
  }
  if ( dst_stats.mConstrLitNum < src_stats.mConstrLitNum ) {
    dst_stats.mConstrLitNum += src_stats.mConstrLitNum;
  }
  if ( dst_stats.mLearntClauseNum < src_stats.mLearntClauseNum ) {
    dst_stats.mLearntClauseNum += src_stats.mLearntClauseNum;
  }
  if ( dst_stats.mLearntLitNum < src_stats.mLearntLitNum ) {
    dst_stats.mLearntLitNum += src_stats.mLearntLitNum;
  }
  if ( dst_stats.mConflictNum < src_stats.mConflictNum ) {
    dst_stats.mConflictNum += src_stats.mConflictNum;
  }
  if ( dst_stats.mDecisionNum < src_stats.mDecisionNum ) {
    dst_stats.mDecisionNum += src_stats.mDecisionNum;
  }
  if ( dst_stats.mPropagationNum < src_stats.mPropagationNum ) {
    dst_stats.mPropagationNum += src_stats.mPropagationNum;
  }
}

// @brief ノードの変数割り当てフラグを消す．
void
SmtEngine::clear_node_mark()
{
  for (vector<TpgNode*>::iterator p = mTfoList.begin();
       p != mTfoList.end(); ++ p) {
    TpgNode* node = *p;
    node->clear_var();
  }
}

END_NAMESPACE_YM_SATPG
