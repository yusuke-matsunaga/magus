
/// @file libym_seal/MCAnalysis.cc
/// @brief MCAnalysis の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MCAnalysis.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_seal/MCAnalysis.h"
#include "ym_bnet/BNetwork.h"
#include "ym_utils/StopWatch.h"
#include "Matrix.h"


BEGIN_NONAMESPACE

#define VERIFY_TRANS1 true

#ifndef VERIFY_TRANS1
#define VERIFY_TRANS1 false
#endif

const bool verify_trans1 = VERIFY_TRANS1;

END_NONAMESPACE


BEGIN_NAMESPACE_YM_SEAL

// 与えられた回路の到達可能状態および遷移確率を求める．
// 同時に正常回路と故障回路の対の到達可能状態および遷移確率も求める．
void
fsm_analysis(const BNetwork& bnetwork,
	     const vector<State>& init_states,
	     vector<State>& reachable_states1,
	     hash_map<State, double>& trans_map1,
	     vector<State>& reachable_states2,
	     hash_map<State, double>& trans_map2);

// 与えられた回路の到達可能状態および遷移確率を求める．
// 同時に正常回路と故障回路の対の到達可能状態および遷移確率も求める．
void
fsm_analysis2(const BNetwork& bnetwork,
	      const vector<State>& init_states,
	      vector<State>& reachable_states1,
	      hash_map<State, double>& trans_map1,
	      vector<State>& reachable_states2,
	      hash_map<State, double>& trans_map2);

// @brief コンストラクタ
MCAnalysis::MCAnalysis()
{
  mUseBdd = false;
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
		    const vector<State>& init_states,
		    bool use_bdd_flag,
		    bool dump_trans_flag)
{
  StopWatch watch;
  StopWatch total_timer;
  
  total_timer.start();

  mUseBdd = use_bdd_flag;
  
  watch.start();
  enum_states(network, init_states);
  watch.stop();
  USTime time1 = watch.time();

  if ( dump_trans_flag ) {
    dump_trans(cout);
    return;
  }

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

// 与えられた回路の到達可能状態および遷移確率を求める．
// 同時に正常回路と故障回路の対の到達可能状態および遷移確率も求める．
void
MCAnalysis::enum_states(const BNetwork& bnetwork,
			const vector<State>& init_states)
{
  if ( mUseBdd ) {
    fsm_analysis(bnetwork, init_states,
		 mReachableStates1, mTransProb1,
		 mReachableStates2, mTransProb2);
  
    if ( verify_trans1 ) {
      bool error = false;

      vector<State> c_states2;
      hash_map<State, double> trans_map2;
      vector<State> pair_vec2;
      hash_map<State, double> trans2_map2;

      fsm_analysis2(bnetwork, init_states,
		    c_states2, trans_map2,
		    pair_vec2, trans2_map2);

      ymuint n = c_states2.size();
      if ( mReachableStates1.size() != n ) {
	cout << "mReachableStates1.size() != n" << endl;
	error = true;
      }
    
      hash_set<State> state_set1;
      for (ymuint i = 0; i < mReachableStates1.size(); ++ i) {
	state_set1.insert(mReachableStates1[i]);
      }
      hash_set<State> state_set2;
      for (ymuint i = 0; i < n; ++ i) {
	state_set2.insert(c_states2[i]);
      }
      for (ymuint i = 0; i < n; ++ i) {
	if ( state_set2.count(mReachableStates1[i]) == 0 ) {
	  cout << mReachableStates1[i] << " no found in c_states2" << endl;
	  error = true;
	}
      }
      
      ymuint ff_num = bnetwork.latch_node_num();
      for (hash_map<State, double>::iterator p = mTransProb1.begin();
	   p != mTransProb1.end(); ++ p) {
	State tmp = p->first;
	State cur_state = tmp.substr(0, ff_num);
	State next_state = tmp.substr(ff_num, ff_num);
	double prob = p->second;
	hash_map<State, double>::iterator q = trans_map2.find(tmp);
	if ( q == trans_map2.end() ) {
	  cout << cur_state << " --> " << next_state
	       << " not found in trans_map2" << endl;
	  error = true;
	}
	else if ( prob != q->second ) {
	  cout << cur_state << " --> " << next_state
	       << ": " << " prob = " << prob
	       << ", prob2 = " << q->second << endl;
	  error = true;
	}
      }
      for (hash_map<State, double>::iterator p = trans_map2.begin();
	   p != trans_map2.end(); ++ p) {
	State tmp = p->first;
	State cur_state = tmp.substr(0, ff_num);
	State next_state = tmp.substr(ff_num, ff_num);
	hash_map<State, double>::iterator q = mTransProb1.find(tmp);
	if ( q == mTransProb1.end() ) {
	  cout << cur_state << " --> " << next_state
	       << " not found in trans_map" << endl;
	  error = true;
	}
      }
      assert_cond(!error, __FILE__, __LINE__);
      
      ymuint pair_num = pair_vec2.size();
      if ( mReachableStates2.size() != pair_num ) {
	cout << "mReachableStates2.size() != pair_num" << endl;
	error = true;
      }
      hash_set<State> pair_set1;
      hash_set<State> pair_set2;
      for (ymuint i = 0; i < mReachableStates2.size(); ++ i) {
	pair_set1.insert(mReachableStates2[i]);
      }
      for (ymuint i = 0; i < pair_num; ++ i) {
	pair_set2.insert(pair_vec2[i]);
      }
      for (ymuint i = 0; i < mReachableStates2.size(); ++ i) {
	State tmp = mReachableStates2[i];
	if ( pair_set2.count(tmp) == 0 ) {
	  State c_state = tmp.substr(0, ff_num);
	  State e_state = tmp.substr(ff_num, ff_num);
	  cout << tmp << "(" << c_state << ":" << e_state << ")"
	       << " not found in pair_vec2[]" << endl;
	  error = true;
	}
      }
      for (ymuint i = 0; i < pair_vec2.size(); ++ i) {
	State tmp = pair_vec2[i];
	if ( pair_set1.count(tmp) == 0 ) {
	  State c_state = tmp.substr(0, ff_num);
	  State e_state = tmp.substr(ff_num, ff_num);
	  cout << c_state << ":" << e_state
	       << " not found in mReachableStates2[]" << endl;
	  error = true;
	}
      }
      
      if ( mTransProb2.size() != trans2_map2.size() ) {
	cout << "mTransProb2.size() != trans2_map2.size()" << endl;
	error = true;
      }
      for (hash_map<State, double>::iterator p = mTransProb2.begin();
	   p != mTransProb2.end(); ++ p) {
	State tmp = p->first;
	State c_state = tmp.substr(0, ff_num);
	State e_state = tmp.substr(ff_num, ff_num);
	State last = tmp.substr(ff_num * 2);
	if ( last != "ERROR" && last != "IDENT" ) {
	  last = last.substr(0, ff_num) + ":" + last.substr(ff_num, ff_num);
	}
	double prob = p->second;
	hash_map<State, double>::iterator q = trans2_map2.find(tmp);
	if ( q == trans2_map2.end() ) {
	  cout << c_state << ":" << e_state << " --> " << last
	       << " not found in trans2_map2" << endl;
	  error = true;
	}
	else if ( prob != q->second ) {
	  cout << c_state << ":" << e_state << " --> " << last
	       << " prob = " << prob
	       << ", prob2 = " << q->second << endl;
	  error = true;
	}
      }
      for (hash_map<State, double>::iterator p = trans2_map2.begin();
	   p != trans2_map2.end(); ++ p) {
	State tmp = p->first;
	State c_state = tmp.substr(0, ff_num);
	State e_state = tmp.substr(ff_num, ff_num);
	State last = tmp.substr(ff_num * 2);
	if ( last != "ERROR" && last != "IDENT" ) {
	  last = last.substr(0, ff_num) + ":" + last.substr(ff_num, ff_num);
	}
	hash_map<State, double>::iterator q = mTransProb2.find(tmp);
	if ( q == mTransProb2.end() ) {
	  cout << c_state << ":" << e_state << " --> " << last
	       << " not found in mTransProb2" << endl;
	  error = true;
	}
      }
      assert_cond(!error, __FILE__, __LINE__);
    }
  }
  else {
    fsm_analysis2(bnetwork, init_states,
		  mReachableStates1, mTransProb1,
		  mReachableStates2, mTransProb2);
  }
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

// @brief 回路対の状態遷移確率行列をダンプする．
void
MCAnalysis::dump_trans(ostream& s)
{
  for (hash_map<StatePair, double>::const_iterator p = mTransProb2.begin();
       p != mTransProb2.end(); ++ p) {
    StatePair sp = p->first;
    double prob = p->second;
    s << sp << " " << prob << endl;
  }
}

// @brief 回路対の状態遷移確率行列を読み込む．
void
MCAnalysis::restore_trans(istream& s)
{
  mTrnasProb2.clear();
  while ( s ) {
    StatePair sp;
    double prob;
    s >> sp >> prob;
    mTransProb2.insert(make_pair(sp, prob));
  }
}

END_NAMESPACE_YM_SEAL
