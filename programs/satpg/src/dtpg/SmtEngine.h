#ifndef SMTENGINE_H
#define SMTENGINE_H

/// @file SmtEngine.h
/// @brief SmtEngine のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "TpgNode.h"
#include "DtpgStats.h"
#include "LitMap.h"
#include "../ymsat/YmSat.h"
#include "logic/Literal.h"
#include "logic/Bool3.h"
#include "logic/SatStats.h"
#include "utils/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class SmtEngine SmtEngine.h "SmtEngine.h"
/// @brief SAT ベースのATPGエンジン
//////////////////////////////////////////////////////////////////////
class SmtEngine
{
public:

  /// @brief コンストラクタ
  SmtEngine();

  /// @brief デストラクタ
  virtual
  ~SmtEngine();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 使用する SAT エンジンを指定する．
  void
  set_mode(const string& type = string(),
	   const string& option = string(),
	   ostream* outp = NULL);

  /// @brief 統計情報をクリアする．
  void
  clear_stats();

  /// @brief 統計情報を得る．
  /// @param[in] stats 結果を格納する構造体
  void
  get_stats(DtpgStats& stats) const;

  /// @breif 時間計測を制御する．
  void
  timer_enable(bool enable);


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief SATソルバのタイプを得る．
  string
  sat_type() const;

  /// @brief SATソルバのオプションを得る．
  string
  sat_option() const;

  /// @brief SATソルバのログ出力を得る．
  ostream*
  sat_outp() const;

  /// @brief CNF 作成を開始する．
  void
  cnf_begin();

  /// @brief CNF 作成を終了する．
  void
  cnf_end();

  /// @brief 故障位置を与えてその TFO の TFI リストを作る．
  /// @param[in] solver SAT ソルバ
  /// @param[in] fnode_list 故障位置のノードのリスト
  /// @param[in] max_id ノード番号の最大値
  ///
  /// 結果は mTfoList に格納される．
  /// 故障位置の TFO が mTfoList の [0: mTfoEnd - 1] に格納される．
  void
  mark_region(YmSat& solver,
	      const vector<TpgNode*>& fnode_list,
	      ymuint max_id);

  /// @brief TFO ノードの数を得る．
  ymuint
  tfo_size() const;

  /// @brief TFI ノードの数を得る．
  ymuint
  tfi_size() const;

  /// @brief TFO ノードと TFI ノードの総数を得る．
  ymuint
  tfo_tfi_size() const;

  /// @brief TFO/TFI ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < tfo_tfi_size() )
  ///
  /// pos が tfo_size() 未満のときは TFO ノード
  /// それ以上は TFI ノードとなっている．
  TpgNode*
  tfo_tfi_node(ymuint pos) const;

  /// @brief 出力のノードのリストを返す．
  const vector<TpgNode*>&
  output_list() const;

  /// @brief 節の作成用の作業領域の使用を開始する．
  /// @param[in] exp_size 予想されるサイズ
  void
  tmp_lits_begin(ymuint exp_size = 0);

  /// @brief 作業領域にリテラルを追加する．
  void
  tmp_lits_add(Literal lit);

  /// @brief 作業領域の冊を SAT ソルバに加える．
  void
  tmp_lits_end(YmSat& solver);

  /// @brief 正常回路のノードの入出力の関係を表す CNF を作る．
  /// @param[in] solver SATソルバ
  /// @param[in] node 対象のノード
  static
  void
  make_gnode_cnf(YmSat& solver,
		 TpgNode* node);

  /// @brief 故障回路のノードの入出力の関係を表す CNF を作る．
  /// @param[in] solver SATソルバ
  /// @param[in] node 対象のノード
  static
  void
  make_fnode_cnf(YmSat& solver,
		 TpgNode* node);

  /// @brief 故障ゲートの CNF を作る．
  static
  void
  make_fault_cnf(YmSat& solver,
		 TpgFault* fault);

  /// @brief ノードの入出力の関係を表す CNF を作る．
  /// @param[in] solver SATソルバ
  /// @param[in] node 対象のノード
  /// @param[in] litmap 入出力のリテラルを保持するクラス
  static
  void
  make_node_cnf(YmSat& solver,
		TpgNode* node,
		const LitMap& litmap,
		Literal output);

  /// @brief ゲートの入出力の関係を表す CNF を作る．
  /// @param[in] solver SATソルバ
  /// @param[in] type ゲートの種類
  /// @param[in] litmap 入出力のリテラルを保持するクラス
  static
  void
  make_gate_cnf(YmSat& solver,
		tTgGateType type,
		const LitMap& litmap,
		Literal output);

  /// @brief ノードの故障差関数を表すCNFを作る．
  void
  make_dlit_cnf(YmSat& solver,
		TpgNode* node);

  /// @brief 故障挿入回路を表す CNF 式を作る．
  /// @param[in] solver SAT ソルバー
  /// @param[in] ivar 入力の変数
  /// @param[in] fvar 故障変数
  /// @param[in] ovar 出力の変数
  static
  void
  make_flt0_cnf(YmSat& solver,
		VarId ivar,
		VarId fvar,
		VarId ovar);

  /// @brief 故障挿入回路を表す CNF 式を作る．
  /// @param[in] solver SAT ソルバー
  /// @param[in] ivar 入力の変数
  /// @param[in] fvar 故障変数
  /// @param[in] ovar 出力の変数
  static
  void
  make_flt1_cnf(YmSat& solver,
		VarId ivar,
		VarId fvar,
		VarId ovar);

  /// @brief 故障挿入回路を表す CNF 式を作る．
  /// @param[in] solver SAT ソルバー
  /// @param[in] ivar 入力の変数
  /// @param[in] fvar0 故障変数
  /// @param[in] fvar1 故障変数
  /// @param[in] ovar 出力の変数
  static
  void
  make_flt01_cnf(YmSat& solver,
		 VarId ivar,
		 VarId fvar0,
		 VarId fvar1,
		 VarId ovar);

  /// @brief 一つの SAT問題を解く．
  Bool3
  solve(YmSat& solver,
	TpgFault* f,
	BackTracer& bt,
	DetectOp& dop,
	UntestOp& uop);

  /// @brief 一つの SAT問題を解く．
  Bool3
  _solve(YmSat& solver,
	 USTime& time);

  /// @brief 検出した場合の処理
  void
  stats_detect(const SatStats& sat_stats,
	       const USTime& time);

  /// @brief 検出不能と判定した場合の処理
  void
  stats_undetect(const SatStats& sat_stats,
		 const USTime& time);

  /// @brief アボートした場合の処理
  void
  stats_abort(const SatStats& sat_stats,
	      const USTime& time);

  /// @brief ノードの変数割り当てフラグを消す．
  void
  clear_node_mark();

  /// @brief tmp マークをつける．
  /// @param[in] node 対象のノード
  void
  set_tmp_mark(TpgNode* node);

  /// @brief tmp マークを消す．
  /// @param[in] node 対象のノード
  void
  clear_tmp_mark(TpgNode* node);

  /// @brief tmp マークを読む．
  /// @param[in] node 対象のノード
  bool
  tmp_mark(TpgNode* node);

  static
  void
  clear_sat_stats(SatStats& stats);

  static
  void
  add_sat_stats(SatStats& dst_stats,
		const SatStats& src_stats);

  static
  void
  sub_sat_stats(SatStats& dst_stats,
		const SatStats& src_stats);

  static
  void
  max_sat_stats(SatStats& dst_stats,
		const SatStats& src_stats);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief tfo マークをつける．
  /// @param[in] node 対象のノード
  void
  set_tfo_mark(TpgNode* node);

  /// @brief tfo マークを読む．
  /// @param[in] node 対象のノード
  bool
  tfo_mark(TpgNode* node);

  /// @brief tfi マークをつける．
  /// @param[in] node 対象のノード
  void
  set_tfi_mark(TpgNode* node);

  /// @brief tfi マークを読む．
  /// @param[in] node 対象のノード
  bool
  tfi_mark(TpgNode* node);


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SAT solver のタイプ
  string mType;

  // SAT solver のオプション
  string mOption;

  // SAT solver の記録用ストリーム
  ostream* mOutP;

  // SAT の結果を格納する配列
  vector<Bool3> mModel;

  // ノードごとのいくつかのフラグをまとめた配列
  vector<ymuint8> mMarkArray;

  // 故障の TFO のノードリスト
  vector<TpgNode*> mTfoList;

  // TFO ノードの最後の位置
  ymuint32 mTfoEnd;

  // 現在の故障に関係のありそうな外部入力のリスト
  vector<TpgNode*> mInputList;

  // 現在の故障に関係ありそうな外部出力のリスト
  vector<TpgNode*> mOutputList;

  // 作業用のリテラルのリスト
  vector<Literal> mTmpLits;

  // 時間計測を行なうかどうかの制御フラグ
  bool mTimerEnable;

  // 時間計測用のタイマー
  StopWatch mTimer;

  // DTPG の統計情報
  DtpgStats mStats;

  // SATソルバの前回の統計情報
  SatStats mPrevStats;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief SATソルバのタイプを得る．
inline
string
SmtEngine::sat_type() const
{
  return mType;
}

// @brief SATソルバのオプションを得る．
inline
string
SmtEngine::sat_option() const
{
  return mOption;
}

// @brief SATソルバのログ出力を得る．
inline
ostream*
SmtEngine::sat_outp() const
{
  return mOutP;
}

// @brief TFO ノードの数を得る．
inline
ymuint
SmtEngine::tfo_size() const
{
  return mTfoEnd;
}

// @brief TFI ノードの数を得る．
inline
ymuint
SmtEngine::tfi_size() const
{
  return mTfoList.size() - mTfoEnd;
}

// @brief TFO ノードと TFI ノードの総数を得る．
inline
ymuint
SmtEngine::tfo_tfi_size() const
{
  return mTfoList.size();
}

// @brief TFO/TFI ノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < tfo_tfi_size() )
//
// pos が tfo_size() 未満のときは TFO ノード
// それ以上は TFI ノードとなっている．
inline
TpgNode*
SmtEngine::tfo_tfi_node(ymuint pos) const
{
  return mTfoList[pos];
}

// @brief 出力のノードのリストを返す．
inline
const vector<TpgNode*>&
SmtEngine::output_list() const
{
  return mOutputList;
}

// tfo マークをつける．
inline
void
SmtEngine::set_tfo_mark(TpgNode* node)
{
  mMarkArray[node->id()] |= 1U;
  mTfoList.push_back(node);
  if ( node->is_output() ) {
    mOutputList.push_back(node);
  }
}

// @brief tfo マークを読む．
inline
bool
SmtEngine::tfo_mark(TpgNode* node)
{
  return static_cast<bool>((mMarkArray[node->id()] >> 0) & 1U);
}

// tfi マークをつける．
inline
void
SmtEngine::set_tfi_mark(TpgNode* node)
{
  mMarkArray[node->id()] |= 2U;
  mTfoList.push_back(node);
  if ( node->is_input() ) {
    mInputList.push_back(node);
  }
}

// @brief tfi マークを読む．
inline
bool
SmtEngine::tfi_mark(TpgNode* node)
{
  return static_cast<bool>((mMarkArray[node->id()] >> 1) & 1U);
}

// @brief tmp マークをつける．
inline
void
SmtEngine::set_tmp_mark(TpgNode* node)
{
  mMarkArray[node->id()] |= 4U;
}

// @brief tmp マークを消す．
inline
void
SmtEngine::clear_tmp_mark(TpgNode* node)
{
  mMarkArray[node->id()] &= ~4U;
}

// @brief tmp マークを読む．
inline
bool
SmtEngine::tmp_mark(TpgNode* node)
{
  return static_cast<bool>((mMarkArray[node->id()] >> 2) & 1U);
}

END_NAMESPACE_YM_SATPG

#endif // SMTENGINEIMPL_H