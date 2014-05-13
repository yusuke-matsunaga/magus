
/// @file SatEngine.cc
/// @brief SatEngine の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "SatEngine.h"

#include "DetectOp.h"
#include "UntestOp.h"
#include "DtpgStats.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "BackTracer.h"
#include "LitMap.h"
#include "logic/SatSolver.h"
#include "logic/SatStats.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

// @brief 故障挿入回路を表す CNF 式を作る．
// @param[in] solver SAT ソルバー
// @param[in] ivar 入力の変数
// @param[in] fvar 故障変数
// @param[in] ovar 出力の変数
// @param[in] fval 故障値
void
make_flt_cnf(SatSolver& solver,
	     VarId ivar,
	     VarId fvar,
	     VarId ovar,
	     int vval)
{
  Literal l0(ivar, false);
  Literal l1(fvar, false);
  Literal l2(ovar, false);
  if ( vval == 0 ) {
    solver.add_clause( l0, ~l2);
    solver.add_clause(~l1, ~l2);
    solver.add_clause(~l0,  l1, l2);
  }
  else {
    solver.add_clause(~l0, l2);
    solver.add_clause(~l1, l2);
    solver.add_clause( l0, l1, ~l2);
  }
}

// @brief ノードに正常回路用の変数を設定する．
// @param[in] solver SAT ソルバー
// @param[in] node 対象のノード
void
set_gvar(SatSolver& solver,
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
set_fvar(SatSolver& solver,
	 TpgNode* node)
{
  // ノードそのものに割り当てる．
  VarId fvar = solver.new_var();
  VarId dvar = solver.new_var();
  node->set_fvar(fvar, dvar);
}

END_NONAMESPACE


// @brief コンストラクタ
SatEngine::SatEngine()
{
  mTimerEnable = false;
}

// @brief デストラクタ
SatEngine::~SatEngine()
{
}

// @brief 使用する SAT エンジンを指定する．
void
SatEngine::set_mode(const string& type,
		    const string& option,
		    ostream* outp)
{
  mType = type;
  mOption = option;
  mOutP = outp;
}

// @brief 統計情報をクリアする．
void
SatEngine::clear_stats()
{
  mRunCount = 0;
  mSatCount = 0;
  mSatStats.mRestart = 0;
  mSatStats.mVarNum = 0;
  mSatStats.mConstrClauseNum = 0;
  mSatStats.mConstrLitNum = 0;
  mSatStats.mLearntClauseNum = 0;
  mSatStats.mLearntLitNum = 0;
  mSatStats.mConflictNum = 0;
  mSatStats.mDecisionNum = 0;
  mSatStats.mPropagationNum = 0;

  mCnfCount = 0;
  mCnfTime.set(0.0, 0.0, 0.0);
  mDetCount = 0;
  mDetTime.set(0.0, 0.0, 0.0);
  mUndetCount = 0;
  mUndetTime.set(0.0, 0.0, 0.0);
  mAbortCount = 0;
  mAbortTime.set(0.0, 0.0, 0.0);
}

// @brief 統計情報を得る．
// @param[in] stats 結果を格納する構造体
void
SatEngine::get_stats(DtpgStats& stats) const
{
  stats.mCnfGenCount = mCnfCount;
  stats.mCnfGenTime = mCnfTime;
  stats.mDetCount = mDetCount;
  stats.mDetTime = mDetTime;
  stats.mRedCount = mUndetCount;
  stats.mRedTime = mUndetTime;
  stats.mAbortCount = mAbortCount;
  stats.mAbortTime = mAbortTime;
  stats.mRunCount = mRunCount;
  stats.mSatCount = mSatCount;
  stats.mSatStats = mSatStats;

#if 0
  if ( mRunCount > 0 ) {
    cout << "# of runs:                       " << mRunCount << endl
	 << "# of problems:                   " << mSatCount << endl
	 << "Ave. # of restarts:              " << (double) mRestart / mSatCount << endl
	 << "Ave. # of variables:             " << (double) mVarNum / mRunCount << endl
	 << "Ave. # of constraint clauses:    " << (double) mConstrClauseNum / mRunCount << endl
	 << "Ave. # of constraint literals:   " << (double) mConstrLitNum / mRunCount << endl
	 << "Ave. # of learnt clauses:        " << (double) mLearntClauseNum / mRunCount << endl
	 << "Ave. # of learnt literals:       " << (double) mLearntLitNum / mRunCount << endl
	 << "Ave. # of conflicts:             " << (double) mConflictNum / mSatCount << endl
	 << "Ave. # of decisions:             " << (double) mDecisionNum / mSatCount << endl
	 << "Ave. # of implications:          " << (double) mPropagationNum / mSatCount << endl;
  }
#endif
}

// @breif 時間計測を制御する．
void
SatEngine::timer_enable(bool enable)
{
  mTimerEnable = enable;
}

// @brief 故障位置を与えてその TFO の TFI リストを作る．
// @param[in] solver SAT ソルバ
// @param[in] fnode_list 故障位置のノードのリスト
// @param[in] max_id ノード番号の最大値
//
// 結果は mTfoList に格納される．
// 故障位置の TFO が mTfoList の [0: mTfoEnd1 - 1] に格納される．
void
SatEngine::mark_region(SatSolver& solver,
		       const vector<TpgNode*>& fnode_list,
		       ymuint max_id)
{
  mMarkArray.clear();
  mMarkArray.resize(max_id, 0U);

  mTfoList.clear();
  mTfoList.reserve(max_id);

  mInputList.clear();
  mOutputList.clear();

  // 故障に一時的なID番号を割り振る．
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
}

// @brief 節の作成用の作業領域の使用を開始する．
// @param[in] exp_size 予想されるサイズ
void
SatEngine::tmp_lits_begin(ymuint exp_size)
{
  mTmpLits.clear();
  if ( exp_size > 0 ) {
    mTmpLits.reserve(exp_size);
  }
}

// @brief 作業領域にリテラルを追加する．
void
SatEngine::tmp_lits_add(Literal lit)
{
  mTmpLits.push_back(lit);
}

// @brief 作業領域の冊を SAT ソルバに加える．
void
SatEngine::tmp_lits_end(SatSolver& solver)
{
  solver.add_clause(mTmpLits);
}

// @brief ノードの入出力の関係を表す CNF を作る．
// @param[in] solver SATソルバ
// @param[in] node 対象のノード
// @param[in] litmap 入出力のリテラルを保持するクラス
void
SatEngine::make_node_cnf(SatSolver& solver,
			 TpgNode* node,
			 const LitMap& litmap)
{
  if ( node->is_input() ) {
    return;
  }

  Literal output = litmap.output();

  if ( node->is_output() ) {
    Literal input = litmap.input(0);
    solver.add_clause( input, ~output);
    solver.add_clause(~input,  output);
    return;
  }

  ymuint ni = node->fanin_num();
  switch ( node->gate_type() ) {
  case kTgGateBuff:
    solver.add_clause( litmap.input(0), ~output);
    solver.add_clause(~litmap.input(0),  output);
    break;

  case kTgGateNot:
    solver.add_clause( litmap.input(0),  output);
    solver.add_clause(~litmap.input(0), ~output);
    break;

  case kTgGateAnd:
    switch ( ni ) {
    case 2:
      solver.add_clause(~litmap.input(0), ~litmap.input(1), output);
      break;

    case 3:
      solver.add_clause(~litmap.input(0), ~litmap.input(1), ~litmap.input(2), output);
      break;

    case 4:
      solver.add_clause(~litmap.input(0), ~litmap.input(1), ~litmap.input(2), ~litmap.input(3), output);
      break;

    default:
      {
	vector<Literal> tmp(ni + 1);
	for (ymuint i = 0; i < ni; ++ i) {
	  tmp[i] = ~litmap.input(i);
	}
	tmp[ni] = output;
	solver.add_clause(tmp);
      }
      break;
    }
    for (ymuint i = 0; i < ni; ++ i) {
      solver.add_clause(litmap.input(i), ~output);
    }
    break;

  case kTgGateNand:
    switch ( ni ) {
    case 2:
      solver.add_clause(~litmap.input(0), ~litmap.input(1), ~output);
      break;

    case 3:
      solver.add_clause(~litmap.input(0), ~litmap.input(1), ~litmap.input(2), ~output);
      break;

    case 4:
      solver.add_clause(~litmap.input(0), ~litmap.input(1), ~litmap.input(2), ~litmap.input(3), ~output);
      break;

    default:
      {
	vector<Literal> tmp(ni + 1);
	for (ymuint i = 0; i < ni; ++ i) {
	  tmp[i] = ~litmap.input(i);
	}
	tmp[ni] = ~output;
	solver.add_clause(tmp);
      }
      break;
    }
    for (ymuint i = 0; i < ni; ++ i) {
      solver.add_clause(litmap.input(i), output);
    }
    break;

  case kTgGateOr:
    switch ( ni ) {
    case 2:
      solver.add_clause(litmap.input(0), litmap.input(1), ~output);
      break;

    case 3:
      solver.add_clause(litmap.input(0), litmap.input(1), litmap.input(2), ~output);
      break;

    case 4:
      solver.add_clause(litmap.input(0), litmap.input(1), litmap.input(2), litmap.input(3), ~output);
      break;

    default:
      {
	vector<Literal> tmp(ni + 1);
	for (ymuint i = 0; i < ni; ++ i) {
	  tmp[i] = litmap.input(i);
	}
	tmp[ni] = ~output;
	solver.add_clause(tmp);
      }
      break;
    }
    for (ymuint i = 0; i < ni; ++ i) {
      solver.add_clause(~litmap.input(i), output);
    }
    break;

  case kTgGateNor:
    switch ( ni ) {
    case 2:
      solver.add_clause(litmap.input(0), litmap.input(1), output);
      break;

    case 3:
      solver.add_clause(litmap.input(0), litmap.input(1), litmap.input(2), output);
      break;

    case 4:
      solver.add_clause(litmap.input(0), litmap.input(1), litmap.input(2), litmap.input(3), output);
      break;

    default:
      {
	vector<Literal> tmp(ni + 1);
	for (ymuint i = 0; i < ni; ++ i) {
	  tmp[i] = litmap.input(i);
	}
	tmp[ni] = output;
	solver.add_clause(tmp);
      }
      break;
    }
    for (ymuint i = 0; i < ni; ++ i) {
      solver.add_clause(~litmap.input(i), ~output);
    }
    break;

  case kTgGateXor:
    if ( ni == 2 ) {
      solver.add_clause(~litmap.input(0),  litmap.input(1),  output);
      solver.add_clause( litmap.input(0), ~litmap.input(1),  output);
      solver.add_clause( litmap.input(0),  litmap.input(1), ~output);
      solver.add_clause(~litmap.input(0), ~litmap.input(1), ~output);
    }
    else {
      vector<Literal> tmp(ni + 1);
      ymuint nip = (1U << ni);
      for (ymuint p = 0; p < nip; ++ p) {
	ymuint c = 0;
	for (ymuint i = 0; i < ni; ++ i) {
	  if ( p & (1U << i) ) {
	    tmp[i] = litmap.input(i);
	  }
	  else {
	    tmp[i] = ~litmap.input(i);
	    ++ c;
	  }
	}
	if ( (c % 2) == 0 ) {
	  tmp[ni] = ~output;
	}
	else {
	  tmp[ni] = output;
	}
	solver.add_clause(tmp);
      }
    }
    break;

  case kTgGateXnor:
    if ( ni == 2 ) {
      solver.add_clause(~litmap.input(0),  litmap.input(1), ~output);
      solver.add_clause( litmap.input(0), ~litmap.input(1), ~output);
      solver.add_clause( litmap.input(0),  litmap.input(1),  output);
      solver.add_clause(~litmap.input(0), ~litmap.input(1),  output);
    }
    else {
      vector<Literal> tmp(ni + 1);
      ymuint nip = (1U << ni);
      for (ymuint p = 0; p < nip; ++ p) {
	ymuint c = 0;
	for (ymuint i = 0; i < ni; ++ i) {
	  if ( p & (1U << i) ) {
	    tmp[i] = litmap.input(i);
	  }
	  else {
	    tmp[i] = ~litmap.input(i);
	    ++ c;
	  }
	}
	if ( (c % 2) == 0 ) {
	  tmp[ni] = output;
	}
	else {
	  tmp[ni] = ~output;
	}
	solver.add_clause(tmp);
      }
    }
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
}

// @brief 一つの SAT問題を解く．
void
SatEngine::solve(SatSolver& solver,
		 TpgFault* f,
		 BackTracer& bt,
		 DetectOp& dop,
		 UntestOp& uop)
{
  if ( mTimerEnable ) {
    mTimer.reset();
    mTimer.start();
  }

  Bool3 ans = solver.solve(mTmpLits, mModel);
  if ( ans == kB3True ) {
    // パタンが求まった．

    // バックトレースを行う．
    TestVector* tv = bt(f->node(), mModel, mInputList, mOutputList);

    // パタンの登録などを行う．
    dop(f, tv);

    if ( mTimerEnable ) {
      mTimer.stop();
      mDetTime += mTimer.time();
      ++ mDetCount;
    }
  }
  else if ( ans == kB3False ) {
    // 検出不能と判定された．
    uop(f);

    if ( mTimerEnable ) {
      mTimer.stop();
      mUndetTime += mTimer.time();
      ++ mUndetCount;
    }
  }
  else { // ans == kB3X つまりアボート
    if ( mTimerEnable ) {
      mTimer.stop();
      mAbortTime += mTimer.time();
      ++ mAbortCount;
    }
  }
}

// @brief 統計情報を得る．
void
SatEngine::update_stats(SatSolver& solver,
			ymuint n)
{
  SatStats sat_stat;
  solver.get_stats(sat_stat);

  ++ mRunCount;
  mSatCount += n;
  mSatStats.mRestart += sat_stat.mRestart;
  mSatStats.mVarNum += sat_stat.mVarNum;
  mSatStats.mConstrClauseNum += sat_stat.mConstrClauseNum;
  mSatStats.mConstrLitNum += sat_stat.mConstrLitNum;
  mSatStats.mLearntClauseNum += sat_stat.mLearntClauseNum;
  mSatStats.mLearntLitNum += sat_stat.mLearntLitNum;
  mSatStats.mConflictNum += sat_stat.mConflictNum;
  mSatStats.mDecisionNum += sat_stat.mDecisionNum;
  mSatStats.mPropagationNum += sat_stat.mPropagationNum;
}

// @brief ノードの変数割り当てフラグを消す．
void
SatEngine::clear_node_mark()
{
  for (vector<TpgNode*>::iterator p = mTfoList.begin();
       p != mTfoList.end(); ++ p) {
    TpgNode* node = *p;
    node->clear_var();
  }
}

END_NAMESPACE_YM_SATPG
