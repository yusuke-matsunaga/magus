
/// @file DtpgSat.cc
/// @brief DtpgSat の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSat.h"

#include "DetectOp.h"
#include "UntestOp.h"
#include "DtpgStats.h"
#include "TpgFault.h"
#include "BackTracer.h"
#include "NodeValList.h"
#include "SatEngine.h"
#include "YmLogic/SatStats.h"
#include "ModelValMap.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief コンストラクタ
DtpgSat::DtpgSat(const string& sat_type,
		 const string& sat_option,
		 ostream* sat_outp,
		 BackTracer& bt,
		 DetectOp& dop,
		 UntestOp& uop) :
  mSatType(sat_type),
  mSatOption(sat_option),
  mSatOutP(sat_outp),
  mBackTracer(bt),
  mDetectOp(dop),
  mUntestOp(uop)
{
  mTimerEnable = false;
}

// @brief デストラクタ
DtpgSat::~DtpgSat()
{
}

// @brief オプションを設定する．
void
DtpgSat::set_option(const string& option_str)
{
  for (string::size_type next = 0; ; ++ next) {
    string::size_type pos = option_str.find(':', next);
    if ( pos == next ) {
      continue;
    }
    string option = option_str.substr(next, pos - next);
    if ( pos == string::npos ) {
      break;
    }
    next = pos;
  }
}

// @brief 統計情報をクリアする．
void
DtpgSat::clear_stats()
{
  mStats.mCnfGenCount = 0;
  mStats.mCnfGenTime.set(0.0, 0.0, 0.0);

  mStats.mDetCount = 0;
  mStats.mDetTime.set(0.0, 0.0, 0.0);
  mStats.mDetStats.clear();
  mStats.mDetStatsMax.clear();

  mStats.mRedCount = 0;
  mStats.mRedTime.set(0.0, 0.0, 0.0);
  mStats.mRedStats.clear();
  mStats.mRedStatsMax.clear();

  mStats.mPartRedCount = 0;
  mStats.mPartRedTime.set(0.0, 0.0, 0.0);
  mStats.mPartRedStats.clear();
  mStats.mPartRedStatsMax.clear();

  mStats.mAbortCount = 0;
  mStats.mAbortTime.set(0.0, 0.0, 0.0);
}

// @brief 統計情報を得る．
// @param[in] stats 結果を格納する構造体
void
DtpgSat::get_stats(DtpgStats& stats) const
{
  stats = mStats;
}

// @breif 時間計測を制御する．
void
DtpgSat::timer_enable(bool enable)
{
  mTimerEnable = enable;
}

// @brief タイマーをスタートする．
void
DtpgSat::cnf_begin()
{
  timer_start();
}

// @brief タイマーを止めて CNF 作成時間に加える．
void
DtpgSat::cnf_end()
{
  USTime time = timer_stop();
  mStats.mCnfGenTime += time;
  ++ mStats.mCnfGenCount;
}

// @brief 時間計測を開始する．
void
DtpgSat::timer_start()
{
  if ( mTimerEnable ) {
    mTimer.reset();
    mTimer.start();
  }
}

/// @brief 時間計測を終了する．
USTime
DtpgSat::timer_stop()
{
  USTime time(0, 0, 0);
  if ( mTimerEnable ) {
    mTimer.stop();
    time = mTimer.time();
  }
  return time;
}

// @brief 一つの SAT問題を解く．
Bool3
DtpgSat::solve(SatEngine& engine,
	       const vector<Literal>& assumptions,
	       TpgFault* f,
	       const NodeSet& node_set,
	       const VidMap& gvar_map,
	       const VidMap& fvar_map)
{
  vector<Bool3> model;
  SatStats sat_stats;
  USTime time;
  Bool3 ans = engine.solve(assumptions, model, sat_stats, time);

  if ( ans == kB3True ) {
    // パタンが求まった．
    ModelValMap val_map(gvar_map, fvar_map, model);

    detect_op(f, node_set, val_map, sat_stats, time);
  }
  else if ( ans == kB3False ) {
    // 検出不能と判定された．
    untest_op(f, sat_stats, time);
  }
  else { // ans == kB3X つまりアボート
    abort_op(f, sat_stats, time);
  }
  return ans;
}

// @brief 最後に生成された値割当リストを得る．
const NodeValList&
DtpgSat::last_assign()
{
  return mLastAssign;
}

// @brief 検出した場合の処理
void
DtpgSat::detect_op(TpgFault* fault,
		   const NodeSet& node_set,
		   const ValMap& val_map,
		   const SatStats& sat_stats,
		   const USTime& time)
{
  // バックトレースを行う．
  mBackTracer(fault->node(), node_set, val_map, mLastAssign);

  // パタンの登録などを行う．
  mDetectOp(fault, mLastAssign);

  mStats.update_det(sat_stats, time);
}

// @brief 検出不能と判定した時の処理
void
DtpgSat::untest_op(TpgFault* fault,
		   const SatStats& sat_stats,
		   const USTime& time)
{
  mUntestOp(fault);

  mStats.update_red(sat_stats, time);
}

// @brief 部分的な検出不能と判定した時の処理
void
DtpgSat::partially_untest_op(TpgFault* fault,
			     const SatStats& sat_stats,
			     const USTime& time)
{
  mStats.update_partred(sat_stats, time);
}

// @brief アボートした時の処理
void
DtpgSat::abort_op(TpgFault* fault,
		  const SatStats& sat_stats,
		  const USTime& time)
{
  mStats.update_abort(sat_stats, time);
}

END_NAMESPACE_YM_SATPG
