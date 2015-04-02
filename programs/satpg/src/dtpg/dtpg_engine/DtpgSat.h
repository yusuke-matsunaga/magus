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
#include "NodeValList.h"
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
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  DtpgSat(const string& sat_type,
	  const string& sat_option,
	  ostream* sat_outp,
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

  /// @brief 最後に生成された値割当リストを得る．
  const NodeValList&
  last_assign();

  /// @brief 時間計測を開始する．
  void
  timer_start();

  /// @brief 時間計測を終了する．
  USTime
  timer_stop();

  /// @brief 一つの SAT問題を解く．
  /// @param[in] engine SAT エンジン
  Bool3
  solve(SatEngine& engine,
	TpgFault* f,
	const NodeSet& node_set,
	const VidMap& gvar_map,
	const VidMap& fvar_map);

  /// @brief 検出した場合の処理
  void
  detect_op(TpgFault* fault,
	    const NodeSet& node_set,
	    const ValMap& val_map,
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

  // 最後に生成された値割当
  NodeValList mLastAssign;

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

END_NAMESPACE_YM_SATPG

#endif // DTPGSAT_H
