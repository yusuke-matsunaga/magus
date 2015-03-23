#ifndef DTPGSAT_H
#define DTPGSAT_H

/// @file DtpgSat.h
/// @brief DtpgSat のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgEngine.h"
#include "TpgNode.h"
#include "DtpgStats.h"
#include "LitMap.h"
#include "YmLogic/Literal.h"
#include "YmLogic/Bool3.h"
#include "YmLogic/sat_nsdef.h"
#include "YmLogic/SatStats.h"
#include "YmUtils/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG

class SatEngine;

//////////////////////////////////////////////////////////////////////
/// @class DtpgSat DtpgSat.h "DtpgSat.h"
/// @brief DtpgSat の実装用の基底クラス
//////////////////////////////////////////////////////////////////////
class DtpgSat :
  public DtpgEngine
{
public:

  /// @brief コンストラクタ
  /// @param[in] sat_type SATソルバの種類を表す文字列
  /// @param[in] sat_option SATソルバに渡すオプション文字列
  /// @param[in] sat_outp SATソルバ用の出力ストリーム
  /// @param[in] network 対象のネットワーク
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  DtpgSat(const string& sat_type,
	  const string& sat_option,
	  ostream* sat_outp,
	  const TpgNetwork& network,
	  BackTracer& bt,
	  DetectOp& dop,
	  UntestOp& uop);

  /// @brief デストラクタ
  virtual
  ~DtpgSat();


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

  /// @brief CNF 作成を開始する．
  void
  cnf_begin();

  /// @brief CNF 作成を終了する．
  void
  cnf_end();

  /// @brief 最後に生成されたパタンを得る．
  TestVector*
  last_pat();

  /// @brief 時間計測を開始する．
  void
  timer_start();

  /// @brief 時間計測を終了する．
  USTime
  timer_stop();

  /// @brief 故障位置を与えてその TFO の TFI リストを作る．
  /// @param[in] fnode_list 故障位置のノードのリスト
  ///
  /// 結果は mTfoList に格納される．
  /// 故障位置の TFO が mTfoList の [0: mTfoEnd - 1] に格納される．
  void
  mark_region(const vector<TpgNode*>& fnode_list);

  /// @brief 入力ノードを得る．
  /// @param[in] ipos 入力番号
  TpgNode*
  input_node(ymuint ipos) const;

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

  /// @brief 正常回路用の CNF 式を作る．
  /// @param[in] engine SAT エンジン
  /// @param[in] node 対象のノード
  static
  void
  make_gval_cnf(SatEngine& engine,
		TpgNode* node);

  /// @brief 故障回路用の CNF 式を作る．
  /// @param[in] engine SAT エンジン
  /// @param[in] node 対象のノード
  static
  void
  make_fval_cnf(SatEngine& engine,
		TpgNode* node);

  /// @brief 故障回路のノードの入出力の関係を表す CNF を作る．
  /// @param[in] engine SAT エンジン
  /// @param[in] node 対象のノード
  static
  void
  make_fnode_cnf(SatEngine& engine,
		 TpgNode* node);

  /// @brief 故障箇所の関係を表す CNF を作る．
  /// @param[in] engine SAT エンジン
  /// @param[in] fault 対象の故障
  static
  void
  make_fault_cnf(SatEngine& engine,
		 TpgFault* fault);

  /// @brief 故障伝搬条件を表すCNFを作る．
  /// @param[in] engine SAT エンジン
  /// @param[in] node 対象のノード
  static
  void
  make_dchain_cnf(SatEngine& engine,
		  TpgNode* node);

  /// @brief 一つの SAT問題を解く．
  /// @param[in] engine SAT エンジン
  Bool3
  solve(SatEngine& engine,
	TpgFault* f,
	bool option = false);

  /// @brief 一つの SAT問題を解く．
  /// @param[in] engine SAT エンジン
  Bool3
  _solve(SatEngine& engine);

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

  // 対象のネットワーク
  const TpgNetwork& mNetwork;

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

  // 最後に生成されたテストパタン
  TestVector* mLastPat;

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
DtpgSat::sat_type() const
{
  return mSatType;
}

// @brief SATソルバのオプションを得る．
inline
string
DtpgSat::sat_option() const
{
  return mSatOption;
}

// @brief SATソルバのログ出力を得る．
inline
ostream*
DtpgSat::sat_outp() const
{
  return mSatOutP;
}

// @brief TFO ノードの数を得る．
inline
ymuint
DtpgSat::tfo_size() const
{
  return mTfoEnd;
}

// @brief TFI ノードの数を得る．
inline
ymuint
DtpgSat::tfi_size() const
{
  return mTfoList.size() - mTfoEnd;
}

// @brief TFO ノードと TFI ノードの総数を得る．
inline
ymuint
DtpgSat::tfo_tfi_size() const
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
DtpgSat::tfo_tfi_node(ymuint pos) const
{
  return mTfoList[pos];
}

// @brief 出力のノードのリストを返す．
inline
const vector<TpgNode*>&
DtpgSat::output_list() const
{
  return mOutputList;
}

// tfo マークをつける．
inline
void
DtpgSat::set_tfo_mark(TpgNode* node)
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
DtpgSat::tfo_mark(TpgNode* node)
{
  return static_cast<bool>((mMarkArray[node->id()] >> 0) & 1U);
}

// tfi マークをつける．
inline
void
DtpgSat::set_tfi_mark(TpgNode* node)
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
DtpgSat::tfi_mark(TpgNode* node)
{
  return static_cast<bool>((mMarkArray[node->id()] >> 1) & 1U);
}

// @brief tmp マークをつける．
inline
void
DtpgSat::set_tmp_mark(TpgNode* node)
{
  mMarkArray[node->id()] |= 4U;
}

// @brief tmp マークを消す．
inline
void
DtpgSat::clear_tmp_mark(TpgNode* node)
{
  mMarkArray[node->id()] &= ~4U;
}

// @brief tmp マークを読む．
inline
bool
DtpgSat::tmp_mark(TpgNode* node)
{
  return static_cast<bool>((mMarkArray[node->id()] >> 2) & 1U);
}

END_NAMESPACE_YM_SATPG

#endif // DTPGSAT_H
