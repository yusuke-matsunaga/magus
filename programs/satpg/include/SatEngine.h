#ifndef SATENGINE_H
#define SATENGINE_H

/// @file SatEngine.h
/// @brief SatEngine のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "Val3.h"
#include "YmNetworks/tgnet.h"
#include "YmLogic/Literal.h"
#include "YmLogic/Bool3.h"
#include "YmLogic/SatSolver.h"
#include "YmLogic/SatStats.h"
#include "YmUtils/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class SatEngine SatEngine.h "SatEngine.h"
/// @brief 論理ゲートのネットワークに関する SAT 問題を解くためのクラス
///
/// AND/OR/XORなどの論理ゲートの入出力の関係を表す CNF 式を作る関数
/// を用意している．
/// ゲートの入出力の値を表すリテラルは LitMap クラスを用いて指定する．
/// また作業用にリテラル配列を持っているので動的にリテラルを追加して
/// 節を作りたい時に利用できる (tmp_lits_XXX 関数)
//////////////////////////////////////////////////////////////////////
class SatEngine
{
public:

  /// @brief コンストラクタ
  /// @param[in] sat_type SATソルバの種類を表す文字列
  /// @param[in] sat_option SATソルバに渡すオプション文字列
  /// @param[in] sat_outp SATソルバ用の出力ストリーム
  SatEngine(const string& sat_type,
	    const string& sat_option,
	    ostream* sat_outp);

  /// @brief デストラクタ
  virtual
  ~SatEngine();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief NodeSet の内容に応じて正常回路のCNFを作る．
  /// @param[in] gval_cnf 正常回路用のデータ構造
  /// @param[in] node_set 対象のノード集合
  ///
  /// 具体的には tfo_tfi_node() を対象にする．
  void
  make_gval_cnf(GvalCnf& gval_cnf,
		const NodeSet& node_set);

  /// @brief ノードのTFI全体の正常回路のCNFを作る．
  /// @param[in] gval_cnf 正常回路用のデータ構造
  /// @param[in] node ノード
  void
  make_gval_cnf(GvalCnf& gval_cnf,
		const TpgNode* node);

  /// @brief 故障回路のCNFを作る．
  /// @param[in] fval_cnf 故障回路用のデータ構造
  /// @param[in] fault 故障
  /// @param[in] node_set 故障に関係するノード集合
  /// @param[in] detect 検出条件
  ///
  /// detect = kVal0: 検出しないCNFを作る．
  ///        = kVal1: 検出するCNFを作る．
  ///        = kValX: fd_var() で制御するCNFを作る．
  void
  make_fval_cnf(FvalCnf& fval_cnf,
		const TpgFault* fault,
		const NodeSet& node_set,
		Val3 detect);

  /// @brief 故障回路のCNFを作る．
  /// @param[in] fval_cnf 故障回路用のデータ構造
  /// @param[in] fault 故障
  /// @param[in] detect 検出条件
  ///
  /// detect = kVal0: 検出しないCNFを作る．
  ///        = kVal1: 検出するCNFを作る．
  ///        = kValX: fd_var() で制御するCNFを作る．
  void
  make_fval_cnf(FvalCnf& fval_cnf,
		const TpgFault* fault,
		Val3 detect);

  /// @brief 2つの故障を持つ故障回路のCNFを作る．
  /// @param[in] fval_cnf0 共通部分の故障回路用のデータ構造
  /// @param[in] fval_cnf1 故障1の故障回路用のデータ構造
  /// @param[in] fval_cnf2 故障2の故障回路用のデータ構造
  /// @param[in] root_node 共通部分の開始点
  /// @param[in] fault1 故障1
  /// @param[in] fault2 故障2
  /// @param[in] node_set0 共通部分に関係するノード集合
  /// @param[in] node_set1 故障1に関係するノード集合
  /// @param[in] node_set2 故障2に関係するノード集合
  void
  make_fval_cnf2(FvalCnf& fval_cnf0,
		 FvalCnf& fval_cnf1,
		 FvalCnf& fval_cnf2,
		 const TpgNode* root_node,
		 const TpgFault* fault1,
		 const TpgFault* fault2,
		 const NodeSet& node_set0,
		 const NodeSet& node_set1,
		 const NodeSet& node_set2);

  /// @brief 複数故障検出回路のCNFを作る．
  /// @param[in] mval_cnf 故障回路用のデータ構造
  /// @param[in] fault_list 故障リスト
  /// @param[in] fnode_list 故障を持つノードのリスト
  /// @param[in] node_set 故障に関係するノード集合
  void
  make_mval_cnf(MvalCnf& mval_cnf,
		const vector<const TpgFault*>& fault_list,
		const vector<const TpgNode*>& fnode_list,
		const NodeSet& node_set);

  /// @brief 割当リストに従って値を固定する．
  /// @param[in] gval_cnf 正常回路用のデータ構造
  /// @param[in] assignment 割当リスト
  void
  add_assignments(GvalCnf& gval_cnf,
		  const NodeValList& assignment);

  /// @brief 割当リストの否定の節を加える．
  /// @param[in] gval_cnf 正常回路用のデータ構造
  /// @param[in] assignment 割当リスト
  void
  add_negation(GvalCnf& gval_cnf,
	       const NodeValList& assignment);

  /// @brief 割当リストのもとでチェックを行う．
  /// @param[in] gval_cnf 正常回路用のデータ構造
  /// @param[in] assign_list 割当リスト
  /// @param[out] sat_model SATの場合の解
  Bool3
  check_sat(GvalCnf& gval_cnf,
	    const NodeValList& assign_list,
	    vector<Bool3>& sat_model);

  /// @brief 割当リストのもとでチェックを行う．
  /// @param[in] gval_cnf 正常回路用のデータ構造
  /// @param[in] assign_list 割当リスト
  ///
  /// こちらは結果のみを返す．
  Bool3
  check_sat(GvalCnf& gval_cnf,
	    const NodeValList& assign_list);

  /// @brief 割当リストのもとでチェックを行う．
  /// @param[in] gval_cnf 正常回路用のデータ構造
  /// @param[in] assign_list1, assign_list2 割当リスト
  /// @param[out] sat_model SATの場合の解
  Bool3
  check_sat(GvalCnf& gval_cnf,
	    const NodeValList& assign_list1,
	    const NodeValList& assign_list2,
	    vector<Bool3>& sat_model);

  /// @brief 割当リストのもとでチェックを行う．
  /// @param[in] gval_cnf 正常回路用のデータ構造
  /// @param[in] assign_list1, assign_list2 割当リスト
  ///
  /// こちらは結果のみを返す．
  Bool3
  check_sat(GvalCnf& gval_cnf,
	    const NodeValList& assign_list1,
	    const NodeValList& assign_list2);

  /// @brief SAT 問題を解く．
  /// @param[in] assumptions 仮定を表すリテラルのリスト
  /// @param[out] sat_model SATの場合の解
  Bool3
  check_sat(const vector<Literal>& assumptions,
	    vector<Bool3>& sat_model);

  /// @brief SAT 問題を解く．
  /// @param[out] sat_model SATの場合の解
  Bool3
  check_sat(vector<Bool3>& sat_model);

  /// @brief SAT 問題を解く．
  /// @param[in] assumptions 仮定を表すリテラルのリスト
  ///
  /// こちらは結果のみを返す．
  Bool3
  check_sat(const vector<Literal>& assumptions);

  /// @brief SAT 問題を解く．
  ///
  /// こちらは結果のみを返す．
  Bool3
  check_sat();

  /// @brief SAT 問題を解く．
  /// @param[in] assumptions 仮定を表すリテラルのリスト
  /// @param[out] model 結果の割当を格納するベクタ
  /// @param[out] sat_stats 統計情報
  /// @param[out] time 処理時間
  Bool3
  solve(const vector<Literal>& assumptions,
	vector<Bool3>& model,
	SatStats& sat_stats,
	USTime& time);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの入出力の関係を表すCNFを作る．
  /// @param[in] node 対象のノード
  /// @param[in] vid_map 変数番号のマップ
  void
  make_node_cnf(const TpgNode* node,
		const VidMap& vid_map);

  /// @brief 故障回路のノードの入出力の関係を表す CNF を作る．
  /// @param[in] node 対象のノード
  void
  make_fnode_cnf(const TpgNode* node,
		 const MvalCnf& mval_cnf);

  /// @brief 故障箇所の関係を表す CNF を作る．
  /// @param[in] fault 対象の故障
  /// @param[in] gvar_map 正常値の変数マップ
  /// @param[in] fvar_map 故障値の変数マップ
  void
  make_fault_cnf(const TpgFault* fault,
		 const VidMap& gvar_map,
		 const VidMap& fvar_map);

  /// @brief 故障伝搬条件を表すCNFを作る．
  /// @param[in] node 対象のノード
  /// @param[in] gvar_map 正常値の変数マップ
  /// @param[in] fvar_map 故障値の変数マップ
  /// @param[in] dvar_map 故障伝搬条件の変数マップ
  void
  make_dchain_cnf(const TpgNode* node,
		  const VidMap& gvar_map,
		  const VidMap& fvar_map,
		  const VidMap& dvar_map);

  /// @brief 故障伝搬条件を表すCNFを作る．
  /// @param[in] node 対象のノード
  /// @param[in] dst_node 伝搬条件の終点のノード
  /// @param[in] gvar_map 正常値の変数マップ
  /// @param[in] fvar_map 故障値の変数マップ
  /// @param[in] dvar_map 故障伝搬条件の変数マップ
  void
  make_dchain_cnf2(const TpgNode* node,
		   const TpgNode* dst_node,
		   const VidMap& gvar_map,
		   const VidMap& fvar_map,
		   const VidMap& dvar_map);

  /// @brief ゲートの入出力の関係を表す CNF を作る．
  /// @param[in] gate_type ゲートの種類
  /// @param[in] litmap 入出力のリテラルを保持するクラス
  void
  make_gate_cnf(tTgGateType gate_type,
		const LitMap& litmap);

  /// @brief 多入力 AND ゲートの入出力の関係を表す CNF 式を生成する．
  /// @param[in] litmap 入出力のリテラルを返すファンクタオブジェクト
  /// @param[in] inv 出力が反転している時 true にするフラグ
  void
  make_and_cnf(const LitMap& litmap,
	       bool inv);

  /// @brief 多入力 OR ゲートの入出力の関係を表す CNF 式を生成する．
  /// @param[in] litmap 入出力のリテラルを返すファンクタオブジェクト
  /// @param[in] inv 出力が反転している時 true にするフラグ
  void
  make_or_cnf(const LitMap& litmap,
	      bool inv);

  /// @brief 多入力 XOR ゲートの入出力の関係を表す CNF 式を生成する．
  /// @param[in] litmap 入出力のリテラルを返すファンクタオブジェクト
  /// @param[in] inv 出力が反転している時 true にするフラグ
  void
  make_xor_cnf(const LitMap& litmap,
	       bool inv);

  /// @brief 0縮退故障挿入回路の CNF を作る．
  /// @param[in] ivar 入力の変数
  /// @param[in] cvar 故障制御変数
  /// @param[in] ovar 出力の変数
  ///
  /// 通常は ovar <=> ivar だが
  /// cvar = 1 の時は ovar = 0 となる．
  void
  make_flt0_cnf(VarId ivar,
		VarId cvar,
		VarId ovar);

  /// @brief 1縮退故障挿入回路の CNF を作る．
  /// @param[in] ivar 入力の変数
  /// @param[in] cvar 故障制御変数
  /// @param[in] ovar 出力の変数
  ///
  /// 通常は ovar <=> ivar だが
  /// cvar = 1 の時は ovar = 1 となる．
  void
  make_flt1_cnf(VarId ivar,
		VarId cvar,
		VarId ovar);

  /// @brief 0/1縮退故障挿入回路の CNF を作る．
  /// @param[in] ivar 入力の変数
  /// @param[in] c0var 0縮退故障制御変数
  /// @param[in] c1var 1縮退故障制御変数
  /// @param[in] ovar 出力の変数
  ///
  /// 通常は ovar <=> ivar だが
  /// c0var = 1 の時は ovar = 0 となる．
  /// c1var = 1 の時は ovar = 1 となる．
  void
  make_flt01_cnf(VarId ivar,
		 VarId c0var,
		 VarId c1var,
		 VarId ovar);

  /// @brief 節の作成用の作業領域の使用を開始する．
  /// @param[in] exp_size 予想されるサイズ
  void
  tmp_lits_begin(ymuint exp_size = 0);

  /// @brief 作業領域にリテラルを追加する．
  /// @param[in] lit 追加するリテラル
  void
  tmp_lits_add(Literal lit);

  /// @brief 作業領域の節を SAT ソルバに加える．
  void
  tmp_lits_end();

  /// @brief 割当リストに対応する仮定を追加する．
  /// @param[in] assumptions 仮定を表すリテラルのリスト
  /// @param[in] gval_cnf 正常回路用のデータ構造
  /// @param[in] assign_list 割当リスト
  void
  add_assumption(vector<Literal>& assumptions,
		 GvalCnf& gval_cnf,
		 const NodeValList& assign_list);

  /// @brief 変数を追加する．
  /// @return 新しい変数番号を返す．
  /// @note 変数番号は 0 から始まる．
  VarId
  new_var();

  /// @brief 節を追加する．
  /// @param[in] lits リテラルのベクタ
  void
  add_clause(const vector<Literal>& lits);

  /// @brief 1項の節(リテラル)を追加する．
  void
  add_clause(Literal lit1);

  /// @brief 2項の節を追加する．
  void
  add_clause(Literal lit1,
	     Literal lit2);

  /// @brief 3項の節を追加する．
  void
  add_clause(Literal lit1,
	     Literal lit2,
	     Literal lit3);

  /// @brief 4項の節を追加する．
  void
  add_clause(Literal lit1,
	     Literal lit2,
	     Literal lit3,
	     Literal lit4);

  /// @brief 5項の節を追加する．
  void
  add_clause(Literal lit1,
	     Literal lit2,
	     Literal lit3,
	     Literal lit4,
	     Literal lit5);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SAT ソルバ
  SatSolver mSolver;

  // 作業用のリテラルのリスト
  vector<Literal> mTmpLits;

  // 時間計測用のタイマー
  StopWatch mTimer;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 節の作成用の作業領域の使用を開始する．
// @param[in] exp_size 予想されるサイズ
inline
void
SatEngine::tmp_lits_begin(ymuint exp_size)
{
  mTmpLits.clear();
  if ( exp_size > 0 ) {
    mTmpLits.reserve(exp_size);
  }
}

// @brief 作業領域にリテラルを追加する．
inline
void
SatEngine::tmp_lits_add(Literal lit)
{
  mTmpLits.push_back(lit);
}

// @brief 作業領域の冊を SAT ソルバに加える．
inline
void
SatEngine::tmp_lits_end()
{
  mSolver.add_clause(mTmpLits);
}

// @brief 変数を追加する．
// @return 新しい変数番号を返す．
// @note 変数番号は 0 から始まる．
inline
VarId
SatEngine::new_var()
{
  return mSolver.new_var();
}

// @brief 節を追加する．
// @param[in] lits リテラルのベクタ
inline
void
SatEngine::add_clause(const vector<Literal>& lits)
{
  mSolver.add_clause(lits);
}

// @brief 1項の節(リテラル)を追加する．
inline
void
SatEngine::add_clause(Literal lit1)
{
  mSolver.add_clause(lit1);
}

// @brief 2項の節を追加する．
inline
void
SatEngine::add_clause(Literal lit1,
		      Literal lit2)
{
  mSolver.add_clause(lit1, lit2);
}

// @brief 3項の節を追加する．
inline
void
SatEngine::add_clause(Literal lit1,
		      Literal lit2,
		      Literal lit3)
{
  mSolver.add_clause(lit1, lit2, lit3);
}

// @brief 4項の節を追加する．
inline
void
SatEngine::add_clause(Literal lit1,
		      Literal lit2,
		      Literal lit3,
		      Literal lit4)
{
  mSolver.add_clause(lit1, lit2, lit3, lit4);
}

/// @brief 5項の節を追加する．
inline
void
SatEngine::add_clause(Literal lit1,
		      Literal lit2,
		      Literal lit3,
		      Literal lit4,
		      Literal lit5)
{
  mSolver.add_clause(lit1, lit2, lit3, lit4, lit5);
}

// @brief SAT 問題を解く．
// @param[in] assumptions 仮定を表すリテラルのリスト
// @param[out] model 結果の割当を格納するベクタ
// @param[out] sat_stats 統計情報
// @param[out] time 処理時間
inline
Bool3
SatEngine::solve(const vector<Literal>& assumptions,
		 vector<Bool3>& model,
		 SatStats& sat_stats,
		 USTime& time)
{
  SatStats prev_stats;
  mSolver.get_stats(prev_stats);

  mTimer.reset();
  mTimer.start();

  Bool3 ans = mSolver.solve(assumptions, model);

  mTimer.stop();
  time = mTimer.time();

  mSolver.get_stats(sat_stats);

  sat_stats -= prev_stats;

  return ans;
}

// @brief 割当リストのもとでチェックを行う．
// @param[in] gval_cnf 正常回路用のデータ構造
// @param[in] assign_list 割当リスト
// @param[out] sat_model SATの場合の解
inline
Bool3
SatEngine::check_sat(GvalCnf& gval_cnf,
		     const NodeValList& assign_list,
		     vector<Bool3>& sat_model)
{
  vector<Literal> assumptions;

  add_assumption(assumptions, gval_cnf, assign_list);

  return check_sat(assumptions, sat_model);
}

// @brief 割当リストのもとでチェックを行う．
// @param[in] gval_cnf 正常回路用のデータ構造
// @param[in] assign_list1, assign_list2 割当リスト
// @param[out] sat_model SATの場合の解
inline
Bool3
SatEngine::check_sat(GvalCnf& gval_cnf,
		     const NodeValList& assign_list1,
		     const NodeValList& assign_list2,
		     vector<Bool3>& sat_model)
{
  vector<Literal> assumptions;

  add_assumption(assumptions, gval_cnf, assign_list1);
  add_assumption(assumptions, gval_cnf, assign_list2);

  return check_sat(assumptions, sat_model);
}

// @brief 割当リストのもとでチェックを行う．
// @param[in] gval_cnf 正常回路用のデータ構造
// @param[in] assign_list 割当リスト
//
// こちらは結果のみを返す．
inline
Bool3
SatEngine::check_sat(GvalCnf& gval_cnf,
		     const NodeValList& assign_list)
{
  vector<Bool3> model;
  return check_sat(gval_cnf, assign_list, model);
}

// @brief 割当リストのもとでチェックを行う．
// @param[in] gval_cnf 正常回路用のデータ構造
// @param[in] assign_list1, assign_list2 割当リスト
//
// こちらは結果のみを返す．
inline
Bool3
SatEngine::check_sat(GvalCnf& gval_cnf,
		     const NodeValList& assign_list1,
		     const NodeValList& assign_list2)
{
  vector<Bool3> model;
  return check_sat(gval_cnf, assign_list1, assign_list2, model);
}

// @brief SAT 問題を解く．
// @param[in] assumptions 仮定を表すリテラルのリスト
// @param[out] sat_model SATの場合の解
inline
Bool3
SatEngine::check_sat(const vector<Literal>& assumptions,
		     vector<Bool3>& sat_model)
{
  return mSolver.solve(assumptions, sat_model);
}

// @brief SAT 問題を解く．
// @param[out] sat_model SATの場合の解
inline
Bool3
SatEngine::check_sat(vector<Bool3>& sat_model)
{
  return check_sat(vector<Literal>(), sat_model);
}

// @brief SAT 問題を解く．
// @param[in] assumptions 仮定を表すリテラルのリスト
//
// こちらは結果のみを返す．
inline
Bool3
SatEngine::check_sat(const vector<Literal>& assumptions)
{
  vector<Bool3> model;
  return check_sat(assumptions, model);
}

// @brief SAT 問題を解く．
//
// こちらは結果のみを返す．
inline
Bool3
SatEngine::check_sat()
{
  vector<Bool3> model;
  return check_sat(vector<Literal>(), model);
}

END_NAMESPACE_YM_SATPG

#endif // SATENGINE_H
