
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
#include "TpgNetwork.h"
#include "TpgNode.h"
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
  clear_sat_stats(mStats.mDetStats);
  clear_sat_stats(mStats.mDetStatsMax);

  mStats.mRedCount = 0;
  mStats.mRedTime.set(0.0, 0.0, 0.0);
  clear_sat_stats(mStats.mRedStats);
  clear_sat_stats(mStats.mRedStatsMax);

  mStats.mPartRedCount = 0;
  mStats.mPartRedTime.set(0.0, 0.0, 0.0);
  clear_sat_stats(mStats.mPartRedStats);
  clear_sat_stats(mStats.mPartRedStatsMax);

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
// @param[in] max_node_id ノード番号の最大値
// @param[in] fnode_list 故障位置のノードのリスト
//
// 結果は mTfoList に格納される．
// 故障位置の TFO が mTfoList の [0: mTfoEnd1 - 1] に格納される．
void
DtpgSat::mark_region(ymuint max_node_id,
		     const vector<TpgNode*>& fnode_list)
{
  mMarkArray.clear();
  mMarkArray.resize(max_node_id, 0U);

  mTfoList.clear();
  mTfoList.reserve(max_node_id);

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
    }
  }

  for (ymuint rpos = 0; rpos < mTfoList.size(); ++ rpos) {
    TpgNode* node = mTfoList[rpos];
    ymuint nfo = node->active_fanout_num();
    for (ymuint i = 0; i < nfo; ++ i) {
      TpgNode* fonode = node->active_fanout(i);
      if ( !tfo_mark(fonode) ) {
	set_tfo_mark(fonode);
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
      }
    }
  }

  sort(mOutputList.begin(), mOutputList.end(), Lt());
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
	       TpgFault* f,
	       const VidMap& gvar_map,
	       const VidMap& fvar_map)
{
  SatStats sat_stats;
  USTime time;
  Bool3 ans = solve(engine, sat_stats, time);

  if ( ans == kB3True ) {
    // パタンが求まった．
    detect_op(f, gvar_map, fvar_map, sat_stats, time);
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

// @brief 一つの SAT問題を解く．
// @param[in] engine SAT エンジン
// @param[out] sat_stats 統計情報
// @param[out] time 処理時間
// @return 結果を返す．
Bool3
DtpgSat::solve(SatEngine& engine,
	       SatStats& sat_stats,
	       USTime& time)
{
  SatStats prev_stats;
  engine.get_stats(prev_stats);

  timer_start();

  Bool3 ans = engine.solve(mModel);

  time = timer_stop();

  engine.get_stats(sat_stats);

  sub_sat_stats(sat_stats, prev_stats);

  return ans;
}

// @brief 最後に生成されたパタンを得る．
TestVector*
DtpgSat::last_pat()
{
  return mLastPat;
}

// @brief 検出した場合の処理
void
DtpgSat::detect_op(TpgFault* fault,
		   const VidMap& gvar_map,
		   const VidMap& fvar_map,
		   const SatStats& sat_stats,
		   const USTime& time)
{
  // バックトレースを行う．
  ModelValMap val_map(gvar_map, fvar_map, mModel);
  NodeValList assign_list;
  mBackTracer(fault->node(), val_map, mInputList, mOutputList, assign_list);

  // パタンの登録などを行う．
  mDetectOp(fault, assign_list);

  ++ mStats.mDetCount;
  mStats.mDetTime += time;
  add_sat_stats(mStats.mDetStats, sat_stats);
  max_sat_stats(mStats.mDetStatsMax, sat_stats);
}

// @brief 検出不能と判定した時の処理
void
DtpgSat::untest_op(TpgFault* fault,
		   const SatStats& sat_stats,
		   const USTime& time)
{
  mUntestOp(fault);

  ++ mStats.mRedCount;
  mStats.mRedTime += time;
  add_sat_stats(mStats.mRedStats, sat_stats);
  max_sat_stats(mStats.mRedStatsMax, sat_stats);
}

// @brief 部分的な検出不能と判定した時の処理
void
DtpgSat::partially_untest_op(TpgFault* fault,
			     const SatStats& sat_stats,
			     const USTime& time)
{
  ++ mStats.mPartRedCount;
  mStats.mPartRedTime += time;
  add_sat_stats(mStats.mPartRedStats, sat_stats);
  max_sat_stats(mStats.mPartRedStatsMax, sat_stats);
}

// @brief アボートした時の処理
void
DtpgSat::abort_op(TpgFault* fault,
		  const SatStats& sat_stats,
		  const USTime& time)
{
  ++ mStats.mAbortCount;
  mStats.mAbortTime += time;
}

// SatStats をクリアする．
void
DtpgSat::clear_sat_stats(SatStats& stats)
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
DtpgSat::add_sat_stats(SatStats& dst_stats,
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
DtpgSat::sub_sat_stats(SatStats& dst_stats,
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
DtpgSat::max_sat_stats(SatStats& dst_stats,
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

END_NAMESPACE_YM_SATPG
