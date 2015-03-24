
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
#include "LitMap.h"
#include "GvarLitMap.h"
#include "FvarLitMap.h"
#include "VectLitMap.h"
#include "SatEngine.h"
#include "YmLogic/SatStats.h"


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

// @brief 正常回路用の CNF 式を作る．
// @param[in] engine SAT エンジン
// @param[in] node 対象のノード
void
DtpgSat::make_gval_cnf(SatEngine& engine,
		       TpgNode* node)
{
  if ( node->is_input() ) {
    ;
  }
  else if ( node->is_output() ) {
    engine.make_gate_cnf(kTgGateBuff, GvarLitMap(node));
  }
  else if ( node->is_logic() ) {
    engine.make_gate_cnf(node->gate_type(), GvarLitMap(node));
  }
  else {
    ASSERT_NOT_REACHED;
  }
}

// @brief 故障回路用の CNF 式を作る．
// @param[in] engine SAT エンジン
// @param[in] node 対象のノード
void
DtpgSat::make_fval_cnf(SatEngine& engine,
		       TpgNode* node)
{
  if ( node->is_input() ) {
    ;
  }
  else if ( node->is_output() ) {
    engine.make_gate_cnf(kTgGateBuff, FvarLitMap(node));
  }
  else if ( node->is_logic() ) {
    engine.make_gate_cnf(node->gate_type(), FvarLitMap(node));
  }
  else {
    ASSERT_NOT_REACHED;
  }
}

// @brief 故障回路のノードの入出力の関係を表す CNF を作る．
// @param[in] engine SAT エンジン
// @param[in] node 対象のノード
void
DtpgSat::make_fnode_cnf(SatEngine& engine,
			TpgNode* node)
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
	VarId tmp_var = engine.new_var();
	engine.make_flt1_cnf(inode->fvar(), f1_var, tmp_var);
	ivars[i] = tmp_var;
      }
    }
    else {
      if ( f1_var == kVarIdIllegal ) {
	VarId tmp_var = engine.new_var();
	engine.make_flt0_cnf(inode->fvar(), f0_var, tmp_var);
	ivars[i] = tmp_var;
      }
      else {
	VarId tmp_var = engine.new_var();
	engine.make_flt01_cnf(inode->fvar(), f0_var, f1_var, tmp_var);
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
      ovar = engine.new_var();
      engine.make_flt1_cnf(ovar, f1_var, node->fvar());
    }
  }
  else {
    if ( f1_var == kVarIdIllegal ) {
      ovar = engine.new_var();
      engine.make_flt0_cnf(ovar, f0_var, node->fvar());
    }
    else {
      ovar = engine.new_var();
      engine.make_flt01_cnf(ovar, f0_var, f1_var, node->fvar());
    }
  }

  if ( node->is_input() ) {
    Literal glit(node->gvar(), false);
    Literal output(ovar, false);
    engine.add_clause( glit, ~output);
    engine.add_clause(~glit,  output);
  }
  else {
    engine.make_gate_cnf(node->gate_type(), VectLitMap(ivars, ovar));
  }
}

// @brief 故障箇所の関係を表す CNF を作る．
// @param[in] engine SAT エンジン
// @param[in] fault 対象の故障
void
DtpgSat::make_fault_cnf(SatEngine& engine,
			TpgFault* fault)
{
  TpgNode* node = fault->node();
  int fval = fault->val();

  if ( fault->is_output_fault() ) {
    // 出力の故障の場合
    // ただ単に故障値を固定するだけ．
    Literal flit(node->fvar(), false);
    if ( fval == 0 ) {
      engine.add_clause(~flit);
    }
    else {
      engine.add_clause(flit);
    }
  }
  else {
    // 入力の故障の場合
    // 故障値は非制御値のはずなので，
    // side input だけのゲートを仮定する．

    ymuint fpos = fault->pos();
    // fpos 以外の入力を ivars[] に入れる．
    // fpos の入力の値は故障値の逆にする．
    ymuint ni = node->fanin_num();
    vector<VarId> ivars;
    ivars.reserve(ni - 1);
    for (ymuint i = 0; i < ni; ++ i) {
      TpgNode* inode = node->fanin(i);
      VarId ivar = inode->gvar();
      if ( i == fpos ) {
	Literal ilit(ivar, false);
	if ( fval == 0 ) {
	  engine.add_clause(ilit);
	}
	else {
	  engine.add_clause(~ilit);
	}
      }
      else {
	ivars.push_back(ivar);
      }
    }
    VarId ovar = node->fvar();

    bool inv = false;
    switch ( node->gate_type() ) {
    case kTgGateBuff:
    case kTgGateNot:
      ASSERT_NOT_REACHED;
      break;

    case kTgGateNand:
      ASSERT_COND( fval == 1 );
      engine.make_and_cnf(VectLitMap(ivars, ovar), true);
      break;

    case kTgGateAnd:
      ASSERT_COND( fval == 1 );
      engine.make_and_cnf(VectLitMap(ivars, ovar), false);
      break;

    case kTgGateNor:
      ASSERT_COND( fval == 0 );
      engine.make_or_cnf(VectLitMap(ivars, ovar), true);
      break;

    case kTgGateOr:
      ASSERT_COND( fval == 0 );
      engine.make_or_cnf(VectLitMap(ivars, ovar), false);
      break;

    case kTgGateXnor:
      inv = true;
      // わざと次に続く

    case kTgGateXor:
      if ( fval == 1 ) {
	inv = !inv;
      }
      engine.make_xor_cnf(VectLitMap(ivars, ovar), inv);
      break;

    default:
      ASSERT_NOT_REACHED;
      break;
    }
  }
}

// @brief 故障伝搬条件を表すCNFを作る．
// @param[in] engine SAT エンジン
void
DtpgSat::make_dchain_cnf(SatEngine& engine,
			 TpgNode* node)
{
  Literal glit(node->gvar(), false);
  Literal flit(node->fvar(), false);
  Literal dlit(node->dvar(), false);

  // dlit -> XOR(glit, flit) を追加する．
  // 要するに dlit が 1 の時，正常回路と故障回路で異なっていなければならない．
  engine.add_clause(~glit, ~flit, ~dlit);
  engine.add_clause( glit,  flit, ~dlit);

  if ( node->is_output() ) {
    // 出力ノードの場合，XOR(glit, flit) -> dlit となる．
    engine.add_clause(~glit,  flit, dlit);
    engine.add_clause( glit, ~flit, dlit);
  }
  else {
    // dlit が 1 の時，ファンアウトの dlit が最低1つは 1 でなければならない．
    ymuint nfo = node->active_fanout_num();
    engine.tmp_lits_begin(nfo + 1);
    engine.tmp_lits_add(~dlit);
    for (ymuint j = 0; j < nfo; ++ j) {
      TpgNode* onode = node->active_fanout(j);
      engine.tmp_lits_add(Literal(onode->dvar(), false));
    }
    engine.tmp_lits_end();

    // dominator の dlit が 0 なら自分も 0
    TpgNode* idom = node->imm_dom();
    if ( idom != NULL ) {
      Literal idlit(idom->dvar(), false);
      engine.add_clause(~dlit, idlit);
    }
  }
}

// @brief 一つの SAT問題を解く．
Bool3
DtpgSat::solve(SatEngine& engine,
	       TpgFault* f,
	       bool option)
{
  SatStats prev_stats;
  engine.get_stats(prev_stats);

  timer_start();

  Bool3 ans = _solve(engine);

  USTime time = timer_stop();

  SatStats sat_stats;
  engine.get_stats(sat_stats);

  sub_sat_stats(sat_stats, prev_stats);

  if ( ans == kB3True ) {
    // パタンが求まった．
    detect_op(f, sat_stats, time);
  }
  else if ( !option ) {
    if ( ans == kB3False ) {
      // 検出不能と判定された．
      untest_op(f, sat_stats, time);
    }
    else { // ans == kB3X つまりアボート
      abort_op(f, sat_stats, time);
    }
  }
  return ans;
}

// @brief 最後に生成されたパタンを得る．
TestVector*
DtpgSat::last_pat()
{
  return mLastPat;
}

// @brief 一つの SAT問題を解く．
Bool3
DtpgSat::_solve(SatEngine& engine)
{
  Bool3 ans = engine.solve(mModel);

  return ans;
}

// @brief 検出した場合の処理
void
DtpgSat::detect_op(TpgFault* fault,
		   const SatStats& sat_stats,
		   const USTime& time)
{
  // バックトレースを行う．
  TestVector* tv = mBackTracer(fault->node(), mModel, mInputList, mOutputList);

  // パタンの登録などを行う．
  mDetectOp(fault, tv);

  mLastPat = tv;

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

// @brief ノードの変数割り当てフラグを消す．
void
DtpgSat::clear_node_mark()
{
  for (vector<TpgNode*>::iterator p = mTfoList.begin();
       p != mTfoList.end(); ++ p) {
    TpgNode* node = *p;
    node->clear_var();
  }
}

END_NAMESPACE_YM_SATPG
