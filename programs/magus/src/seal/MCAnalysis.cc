
/// @file magus/src/seal/MCAnalysis.cc
/// @brief MCAnalysis の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MCAnalysis.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MCAnalysis.h"
#include "ym_bnet/BNetwork.h"
#include "ym_utils/StopWatch.h"
#include "Matrix.h"


BEGIN_NAMESPACE_MAGUS_SEAL

// @brief コンストラクタ
MCAnalysis::MCAnalysis()
{
}

// @brief デストラクタ
MCAnalysis::~MCAnalysis()
{
}


// @brief 解析を行う．
// @param[in] network 対象のネットワーク
// @param[in] init_states 初期状態の集合
void
MCAnalysis::analyze(const BNetwork& network,
		    const vector<State>& init_states)
{
  StopWatch watch;
  StopWatch total_timer;
  
  total_timer.start();
  
  watch.start();
  enum_states(network, init_states);
  watch.stop();
  USTime time1 = watch.time();

  watch.reset();
  watch.start();
  calc_steady_prob();
  watch.stop();
  USTime time2 = watch.time();

  watch.reset();
  watch.start();
  ymuint ff_num = network.latch_node_num();
  vector<double> error_rate(ff_num, 1.0e-10);
  calc_error_prob(error_rate);
  watch.stop();
  USTime time3 = watch.time();
  
  watch.reset();
  watch.start();
  calc_failure_prob();
  watch.stop();
  USTime time5 = watch.time();
  
  double abs_prob_sum = 0.0;
  for (ymuint i = 0; i < mReachableStates2.size(); ++ i) {
    State st_pair = mReachableStates2[i];
    hash_map<State, double>::iterator p = mInitialProb.find(st_pair);
    if ( p != mInitialProb.end() ) {
      double init_prob = p->second;
      abs_prob_sum += mFailureProb[i] * init_prob;
    }
  }

  total_timer.stop();
  USTime ttime = total_timer.time();
  
  cout.precision(6);

  cout << " " << abs_prob_sum << endl;
  cout << "enum_state : " << time1 << endl;
  cout << "steady_prob: " << time2 << endl;
  cout << "init_prob  : " << time3 << endl;
  cout << "abs_prob   : " << time5 << endl;
  cout << "total_time : " << ttime << endl;
}

// @brief 正常回路の定常分布を求める関数
// @note 結果は mSteadyProb に格納される．
void
MCAnalysis::calc_steady_prob()
{
  ymuint32 n = mReachableStates1.size();
  Matrix m(n, n + 1);
  for (ymuint32 i = 0; i < n; ++ i) {
    State n_state = mReachableStates1[i];
    for(ymuint32 j = 0; j < n; ++ j) {
      State c_state = mReachableStates1[j];
      StatePair sp = c_state + n_state;
      hash_map<StatePair, double>::const_iterator p = mTransProb1.find(sp);
      double d = 0.0;
      if ( p != mTransProb1.end() ) {
	d = p->second;
      }
      if ( i != j ) {
	d += 1.0;
      }
      m.elem(i, j) = d;
    }
    m.elem(i, n) = 1.0;
  }
  bool stat = gaussian_elimination(m, mSteadyProb);
  assert_cond(stat, __FILE__, __LINE__);
}

// @brief SEU による初期状態確率を計算する．
// @note 結果は mInitialProb に格納される．
void
MCAnalysis::calc_error_prob(const vector<double>& error_rate)
{
  ymuint32 ff_num = error_rate.size();
  ymuint32 state_num = mReachableStates1.size();
  for (ymuint32 i = 0; i < state_num; ++ i) {
    State correct_state = mReachableStates1[i];
    for (ymuint32 j = 0; j < ff_num; ++ j) {
      State error_state = correct_state;
      // j ビット目を反転させる．
      if ( error_state[j] == '0' ) {
	error_state[j] = '1';
      }
      else {
	error_state[j] = '0';
      }
      StatePair tmp_pair = correct_state + error_state;
      double prob = mSteadyProb[i] * error_rate[j];
      mInitialProb.insert(make_pair(tmp_pair, prob));
    }
  }
}
  
// @brief エラー状態の吸収確率を求める．
// @note 結果は mFailureProb に格納される．
void
MCAnalysis::calc_failure_prob()
{
  StopWatch timer;
  timer.start();
  ymuint32 state_num = mReachableStates2.size();
  Matrix m(state_num, state_num + 1);
  for (ymuint32 i = 0; i < state_num; ++ i) {
    State cur_state = mReachableStates2[i];
    for (ymuint32 j = 0; j < state_num; ++ j) {
      State next_state = mReachableStates2[j];
      StatePair tmp = cur_state + next_state;
      hash_map<StatePair, double>::const_iterator p = mTransProb2.find(tmp);
      double d = 0.0;
      if ( p != mTransProb2.end() ) {
	d = - p->second;
      }
      if ( i == j ) {
	d += 1.0;
      }
      m.elem(i, j) = d;
    }
    StatePair tmp = cur_state + "ERROR";
    hash_map<StatePair, double>::const_iterator p = mTransProb2.find(tmp);
    double d = 0.0;
    if ( p != mTransProb2.end() ) {
      d = p->second;
    }
    m.elem(i, state_num) = d;
  }
  timer.stop();
  USTime time = timer.time();
  cout << "make prob. matrix: " << time << endl;

  timer.reset();
  timer.start();
  gaussian_elimination(m, mFailureProb);
  timer.stop();
  time = timer.time();
  cout << "gausian elimination: " << time << endl;
}

END_NAMESPACE_MAGUS_SEAL
