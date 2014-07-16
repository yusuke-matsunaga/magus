#ifndef SATENGINEBASE_H
#define SATENGINEBASE_H

/// @file SatEngineBase.h
/// @brief SatEngineBase のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "SatEngine.h"
#include "TpgNode.h"
#include "DtpgStats.h"
#include "LitMap.h"
#include "logic/Literal.h"
#include "logic/Bool3.h"
#include "logic/sat_nsdef.h"
#include "logic/SatStats.h"
#include "utils/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class SatEngineBase SatEngineBase.h "SatEngineBase.h"
/// @brief SatEngine の実装用の基底クラス
//////////////////////////////////////////////////////////////////////
class SatEngineBase :
  public SatEngine
{
public:

  /// @brief コンストラクタ
  /// @param[in] sat_type SATソルバの種類を表す文字列
  /// @param[in] sat_option SATソルバに渡すオプション文字列
  /// @param[in] sat_outp SATソルバ用の出力ストリーム
  /// @param[in] max_id ノード番号の最大値 + 1
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  SatEngineBase(const string& sat_type,
		const string& sat_option,
		ostream* sat_outp,
		ymuint max_id,
		BackTracer& bt,
		DetectOp& dop,
		UntestOp& uop);

  /// @brief デストラクタ
  virtual
  ~SatEngineBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief オプションを設定する．
  virtual
  void
  set_option(const string& option_str);

  /// @brief 統計情報をクリアする．
  virtual
  void
  clear_stats();

  /// @brief 統計情報を得る．
  /// @param[in] stats 結果を格納する構造体
  virtual
  void
  get_stats(DtpgStats& stats) const;

  /// @breif 時間計測を制御する．
  virtual
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

  /// @brief NEMESIS モード(含む EXT-NEMESIS)の時 true を返す．
  bool
  nemesis_mode() const;

  /// @brief EXT-NEMESIS モードの時 true を返す．
  bool
  ext_nemesis_mode() const;

  /// @brief TG-GRASP モード(含む EXT-TG-GRASP)の時 true を返す．
  bool
  tg_grasp_mode() const;

  /// @brief EXT-TG-GRASP モードの時 true を返す．
  bool
  ext_tg_grasp_mode() const;

  /// @brief dominator を用いた unique sensitization を行う時 true を返す．
  bool
  use_dominator() const;

  /// @brief CNF 作成を開始する．
  void
  cnf_begin();

  /// @brief CNF 作成を終了する．
  void
  cnf_end();

  /// @brief 時間計測を開始する．
  void
  timer_start();

  /// @brief 時間計測を終了する．
  USTime
  timer_stop();

  /// @brief 故障位置を与えてその TFO の TFI リストを作る．
  /// @param[in] solver SAT ソルバ
  /// @param[in] fnode_list 故障位置のノードのリスト
  /// @param[in] max_id ノード番号の最大値
  ///
  /// 結果は mTfoList に格納される．
  /// 故障位置の TFO が mTfoList の [0: mTfoEnd - 1] に格納される．
  void
  mark_region(SatSolver& solver,
	      const vector<TpgNode*>& fnode_list);

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
  tmp_lits_end(SatSolver& solver);

  /// @brief 正常回路のノードの入出力の関係を表す CNF を作る．
  /// @param[in] solver SATソルバ
  /// @param[in] node 対象のノード
  static
  void
  make_gnode_cnf(SatSolver& solver,
		 TpgNode* node);

  /// @brief 故障回路のノードの入出力の関係を表す CNF を作る．
  /// @param[in] solver SATソルバ
  /// @param[in] node 対象のノード
  static
  void
  make_fnode_cnf(SatSolver& solver,
		 TpgNode* node);

  /// @brief 故障ゲートの CNF を作る．
  static
  void
  make_fault_cnf(SatSolver& solver,
		 TpgFault* fault);

  /// @brief D-Chain 制約のCNFを作る．
  void
  make_dchain_cnf(SatSolver& solver,
		  TpgNode* node,
		  TpgFault* fault);

  /// @brief D-Chain 制約のCNFを作る．
  void
  make_dchain_cnf(SatSolver& solver,
		  TpgNode* node);

  /// @brief ノードの入出力の関係を表す CNF を作る．
  /// @param[in] solver SATソルバ
  /// @param[in] node 対象のノード
  /// @param[in] litmap 入出力のリテラルを保持するクラス
  static
  void
  make_node_cnf(SatSolver& solver,
		TpgNode* node,
		const LitMap& litmap,
		Literal output);

  /// @brief ゲートの入出力の関係を表す CNF を作る．
  /// @param[in] solver SATソルバ
  /// @param[in] type ゲートの種類
  /// @param[in] litmap 入出力のリテラルを保持するクラス
  static
  void
  make_gate_cnf(SatSolver& solver,
		tTgGateType type,
		const LitMap& litmap,
		Literal output);

  /// @brief ノードの故障差関数を表すCNFを作る．
  void
  make_dlit_cnf(SatSolver& solver,
		TpgNode* node);

  /// @brief 故障挿入回路を表す CNF 式を作る．
  /// @param[in] solver SAT ソルバー
  /// @param[in] ivar 入力の変数
  /// @param[in] fvar 故障変数
  /// @param[in] ovar 出力の変数
  static
  void
  make_flt0_cnf(SatSolver& solver,
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
  make_flt1_cnf(SatSolver& solver,
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
  make_flt01_cnf(SatSolver& solver,
		 VarId ivar,
		 VarId fvar0,
		 VarId fvar1,
		 VarId ovar);

  /// @brief 一つの SAT問題を解く．
  Bool3
  solve(SatSolver& solver,
	TpgFault* f);

  /// @brief 一つの SAT問題を解く．
  Bool3
  _solve(SatSolver& solver);

  /// @brief 検出した場合の処理
  void
  detect_op(TpgFault* fault,
	    const SatStats& sat_stats,
	    const USTime& time);

  /// @brief 検出不能と判定した時の処理
  void
  untest_op(TpgFault* fault,
	    const SatStats& sat_stats,
	    const USTime& time);

  /// @brief 部分的な検出不能と判定した時の処理
  void
  partially_untest_op(TpgFault* fault,
		      const SatStats& sat_stats,
		      const USTime& time);

  /// @brief アボートした時の処理
  void
  abort_op(TpgFault* fault,
	   const SatStats& sat_stats,
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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SAT solver のタイプ
  string mSatType;

  // SAT solver のオプション
  string mSatOption;

  // SAT solver の記録用ストリーム
  ostream* mSatOutP;

  // ノードのIDの最大値
  ymuint32 mMaxNodeId;

  // バックトレーサー
  BackTracer& mBackTracer;

  // 検出時に呼ばれるファンクタ
  DetectOp& mDetectOp;

  // 検出不能時に呼ばれるファンクタ
  UntestOp& mUntestOp;

  // NEMESIS モード
  bool mNemesis;

  // extected NEMESIS モード
  bool mExtNemesis;

  // TG-GRASP モード
  bool mTgGrasp;

  // extended TG-GRASP モード
  bool mExtTgGrasp;

  // unique sensitization を使う
  bool mUseDominator;

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

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief SATソルバのタイプを得る．
inline
string
SatEngineBase::sat_type() const
{
  return mSatType;
}

// @brief SATソルバのオプションを得る．
inline
string
SatEngineBase::sat_option() const
{
  return mSatOption;
}

// @brief SATソルバのログ出力を得る．
inline
ostream*
SatEngineBase::sat_outp() const
{
  return mSatOutP;
}

// @brief NEMESIS モード(含む EXT-NEMESIS)の時 true を返す．
inline
bool
SatEngineBase::nemesis_mode() const
{
  return mNemesis;
}

// @brief EXT-NEMESIS モードの時 true を返す．
inline
bool
SatEngineBase::ext_nemesis_mode() const
{
  return mExtNemesis;
}

// @brief TG-GRASP モード(含む EXT-TG-GRASP)の時 true を返す．
inline
bool
SatEngineBase::tg_grasp_mode() const
{
  return mTgGrasp;
}

// @brief EXT-TG-GRASP モードの時 true を返す．
inline
bool
SatEngineBase::ext_tg_grasp_mode() const
{
  return mExtTgGrasp;
}

// @brief dominator を用いた unique sensitization を行う時 true を返す．
inline
bool
SatEngineBase::use_dominator() const
{
  return mUseDominator;
}

// @brief TFO ノードの数を得る．
inline
ymuint
SatEngineBase::tfo_size() const
{
  return mTfoEnd;
}

// @brief TFI ノードの数を得る．
inline
ymuint
SatEngineBase::tfi_size() const
{
  return mTfoList.size() - mTfoEnd;
}

// @brief TFO ノードと TFI ノードの総数を得る．
inline
ymuint
SatEngineBase::tfo_tfi_size() const
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
SatEngineBase::tfo_tfi_node(ymuint pos) const
{
  return mTfoList[pos];
}

// @brief 出力のノードのリストを返す．
inline
const vector<TpgNode*>&
SatEngineBase::output_list() const
{
  return mOutputList;
}

// tfo マークをつける．
inline
void
SatEngineBase::set_tfo_mark(TpgNode* node)
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
SatEngineBase::tfo_mark(TpgNode* node)
{
  return static_cast<bool>((mMarkArray[node->id()] >> 0) & 1U);
}

// tfi マークをつける．
inline
void
SatEngineBase::set_tfi_mark(TpgNode* node)
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
SatEngineBase::tfi_mark(TpgNode* node)
{
  return static_cast<bool>((mMarkArray[node->id()] >> 1) & 1U);
}

// @brief tmp マークをつける．
inline
void
SatEngineBase::set_tmp_mark(TpgNode* node)
{
  mMarkArray[node->id()] |= 4U;
}

// @brief tmp マークを消す．
inline
void
SatEngineBase::clear_tmp_mark(TpgNode* node)
{
  mMarkArray[node->id()] &= ~4U;
}

// @brief tmp マークを読む．
inline
bool
SatEngineBase::tmp_mark(TpgNode* node)
{
  return static_cast<bool>((mMarkArray[node->id()] >> 2) & 1U);
}

END_NAMESPACE_YM_SATPG

#endif // SATENGINEBASE_H
