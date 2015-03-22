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

#include "TpgNode.h"
#include "LitMap.h"
#include "YmLogic/Literal.h"
#include "YmLogic/Bool3.h"
#include "YmLogic/SatSolver.h"
#include "YmLogic/SatStats.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class SatEngine SatEngine.h "SatEngine.h"
/// @brief SatEngine の実装用の基底クラス
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

  /// @brief ノードに正常回路用の変数を割り当てる．
  /// @param[in] node 対象のノード
  void
  set_gvar(TpgNode* node);

  /// @brief ノードに故障回路用の変数を割り当てる．
  /// @param[in] node 対象のノード
  void
  set_fvar(TpgNode* node);

  /// @brief 正常回路のノードの入出力の関係を表す CNF を作る．
  /// @param[in] node 対象のノード
  void
  make_gnode_cnf(TpgNode* node);

  /// @brief 故障回路のノードの入出力の関係を表す CNF を作る．
  /// @param[in] node 対象のノード
  void
  make_fnode_cnf(TpgNode* node);

  /// @brief 故障回路のノードの入出力の関係を表す CNF を作る．
  /// @param[in] node 対象のノード
  void
  make_fnode_cnf2(TpgNode* node);

  /// @brief 故障ゲートの CNF を作る．
  void
  make_fault_cnf(TpgFault* fault);

  /// @brief ノードの故障差関数を表すCNFを作る．
  void
  make_dlit_cnf(TpgNode* node);

  /// @brief 故障伝搬条件を表すCNFを作る．
  void
  make_dchain_cnf(TpgNode* node);

  /// @brief 節の作成用の作業領域の使用を開始する．
  /// @param[in] exp_size 予想されるサイズ
  void
  tmp_lits_begin(ymuint exp_size = 0);

  /// @brief 作業領域にリテラルを追加する．
  /// @param[in] lit 追加するリテラル
  void
  tmp_lits_add(Literal lit);

  /// @brief 作業領域の冊を SAT ソルバに加える．
  void
  tmp_lits_end();

  /// @brief 仮定リストの使用を開始する．
  /// @param[in] exp_size 予想されるサイズ
  ///
  /// 実際のサイズが exp_size を超えても正しく動く
  void
  assumption_begin(ymuint exp_size = 0);

  /// @brief 仮定を追加する．
  /// @param[in] lit 仮定のリテラル
  void
  assumption_add(Literal lit);

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

  /// @brief SAT 問題を解く．
  /// @param[out] model 結果の割当を格納するベクタ
  Bool3
  solve(vector<Bool3>& model);

  /// @brief 現在の内部状態を得る．
  /// @param[out] stats 状態を格納する構造体
  void
  get_stats(SatStats& stats) const;


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

  // 仮定のリテラルを収めるリスト
  vector<Literal> mAssumptions;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ノードに正常回路用の変数を設定する．
// @param[in] node 対象のノード
inline
void
SatEngine::set_gvar(TpgNode* node)
{
  // ノードそのものに割り当てる．
  VarId gvar = new_var();
  node->set_gvar(gvar);
}

// @brief ノードに故障回路用の変数を設定する．
// @param[in] node 対象のノード
inline
void
SatEngine::set_fvar(TpgNode* node)
{
  // ノードそのものに割り当てる．
  VarId fvar = new_var();
  VarId dvar = new_var();
  node->set_fvar(fvar, dvar);
}

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

// @brief 仮定リストの使用を開始する．
// @param[in] exp_size 予想されるサイズ
//
// 実際のサイズが exp_size を超えても正しく動く
inline
void
SatEngine::assumption_begin(ymuint exp_size)
{
  mAssumptions.clear();
  if ( exp_size > 0 ) {
    mAssumptions.reserve(exp_size);
  }
}

// @brief 仮定を追加する．
// @param[in] lit 仮定のリテラル
inline
void
SatEngine::assumption_add(Literal lit)
{
  mAssumptions.push_back(lit);
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
// @param[out] model 結果の割当を格納するベクタ
inline
Bool3
SatEngine::solve(vector<Bool3>& model)
{
  return mSolver.solve(mAssumptions, model);
}

// @brief 現在の内部状態を得る．
// @param[out] stats 状態を格納する構造体
inline
void
SatEngine::get_stats(SatStats& stats) const
{
  mSolver.get_stats(stats);
}

END_NAMESPACE_YM_SATPG

#endif // SATENGINE_H
