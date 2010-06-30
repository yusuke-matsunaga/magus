
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
#include "SMatrix.h"


BEGIN_NONAMESPACE

#define VERIFY_TRANS1 false;

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
	     hash_map<ymuint, double>& trans_map1,
	     vector<State>& reachable_states2,
	     hash_map<ymuint, double>& trans_map2,
	     vector<double>& failure_prob);

// 与えられた回路の到達可能状態および遷移確率を求める．
// 同時に正常回路と故障回路の対の到達可能状態および遷移確率も求める．
void
fsm_analysis2(const BNetwork& bnetwork,
	      const vector<State>& init_states,
	      vector<State>& reachable_states1,
	      hash_map<ymuint, double>& trans_map1,
	      vector<State>& reachable_states2,
	      hash_map<ymuint, double>& trans_map2,
	      vector<double>& failure_prob);

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

  ymuint ff_num = network.latch_node_num();
  mFFNum = ff_num;
  
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


// @brief 解析を行う．
void
MCAnalysis::analyze2(istream& s)
{
  StopWatch watch;
  StopWatch total_timer;
  
  total_timer.start();

  restore_trans(s);

#if 0
  watch.reset();
  watch.start();
  calc_steady_prob();
  watch.stop();
  USTime time2 = watch.time();

  watch.reset();
  watch.start();
  vector<double> error_rate(mFFNum, 1.0e-10);
  calc_error_prob(error_rate);
  watch.stop();
  USTime time3 = watch.time();
#endif

  watch.reset();
  watch.start();
  calc_failure_prob();
  watch.stop();
  USTime time5 = watch.time();
#if 0
  for (ymuint i = 0; i < mReachableStates2.size(); ++ i) {
    cout << mFailureProb[i] << endl;
  }
#endif

#if 0
  double abs_prob_sum = 0.0;
  for (ymuint i = 0; i < mReachableStates2.size(); ++ i) {
    State st_pair = mReachableStates2[i];
    hash_map<State, double>::iterator p = mInitialProb.find(st_pair);
    if ( p != mInitialProb.end() ) {
      double init_prob = p->second;
      abs_prob_sum += mFailureProb[i] * init_prob;
    }
  }
#endif
  
  total_timer.stop();
  USTime ttime = total_timer.time();
  
  cout.precision(6);

#if 0
  cout << " " << abs_prob_sum << endl;
  cout << "steady_prob: " << time2 << endl;
  cout << "init_prob  : " << time3 << endl;
#endif
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
		 mReachableStates2, mTransProb2, mFailureProb0);

    if ( verify_trans1 ) {
      bool error = false;
      ymuint ff_num = bnetwork.latch_node_num();
      
      vector<State> c_states2;
      hash_map<ymuint, double> trans_map2;
      vector<State> pair_vec2;
      hash_map<ymuint, double> trans2_map2;
      vector<double> fprob2;
      
      fsm_analysis2(bnetwork, init_states,
		    c_states2, trans_map2,
		    pair_vec2, trans2_map2, fprob2);

      // 正常回路の到達可能状態の比較
      ymuint state_num = mReachableStates1.size();
      if ( c_states2.size() != state_num ) {
	cout << "mReachableStates1.size() != state_num" << endl;
	error = true;
      }
    
      hash_map<State, ymuint> state_map1;
      for (ymuint i = 0; i < state_num; ++ i) {
	state_map1.insert(make_pair(mReachableStates1[i], i));
      }
      hash_map<State, ymuint> state_map2;
      for (ymuint i = 0; i < c_states2.size(); ++ i) {
	state_map2.insert(make_pair(c_states2[i], i));
      }
      for (ymuint i = 0; i < c_states2.size(); ++ i) {
	if ( state_map2.count(mReachableStates1[i]) == 0 ) {
	  cout << mReachableStates1[i] << " no found in c_states2" << endl;
	  error = true;
	}
      }
      assert_cond( !error, __FILE__, __LINE__);

      // 正常回路の遷移確率行列の比較
      for (hash_map<ymuint, double>::iterator p = mTransProb1.begin();
	   p != mTransProb1.end(); ++ p) {
	ymuint tmp = p->first;
	double prob = p->second;
	ymuint csid = tmp / state_num;
	ymuint nsid = tmp % state_num;
	State cur_state = mReachableStates1[csid];
	State next_state = mReachableStates1[nsid];
	hash_map<State, ymuint>::iterator s = state_map2.find(cur_state);
	assert_cond( s != state_map2.end(), __FILE__, __LINE__);
	ymuint csid2 = s->second;
	s = state_map2.find(next_state);
	assert_cond( s != state_map2.end(), __FILE__, __LINE__);
	ymuint nsid2 = s->second;
	ymuint tmp2 = csid2 * state_num + nsid2;
	hash_map<ymuint, double>::iterator q = trans_map2.find(tmp2);
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
      for (hash_map<ymuint, double>::iterator p = trans_map2.begin();
	   p != trans_map2.end(); ++ p) {
	ymuint tmp = p->first;
	ymuint csid = tmp / state_num;
	ymuint nsid = tmp % state_num;
	State cur_state = c_states2[csid];
	State next_state = c_states2[nsid];
	hash_map<State, ymuint>::iterator s = state_map1.find(cur_state);
	assert_cond( s != state_map1.end(), __FILE__, __LINE__);
	ymuint csid2 = s->second;
	s = state_map1.find(next_state);
	assert_cond( s != state_map1.end(), __FILE__, __LINE__);
	ymuint nsid2 = s->second;
	ymuint tmp2 = csid2 * state_num + nsid2;
	hash_map<ymuint, double>::iterator q = mTransProb1.find(tmp2);
	if ( q == mTransProb1.end() ) {
	  cout << cur_state << " --> " << next_state
	       << " not found in trans_map" << endl;
	  error = true;
	}
      }
      assert_cond(!error, __FILE__, __LINE__);

      // 回路対の到達可能状態の比較
      ymuint pair_num = pair_vec2.size();
      if ( mReachableStates2.size() != pair_num ) {
	cout << "mReachableStates2.size() != pair_num" << endl;
	error = true;
      }

      hash_map<State, ymuint> pair_map1;
      for (ymuint i = 0; i < mReachableStates2.size(); ++ i) {
	pair_map1.insert(make_pair(mReachableStates2[i], i));
      }
      hash_map<State, ymuint> pair_map2;
      for (ymuint i = 0; i < pair_num; ++ i) {
	pair_map2.insert(make_pair(pair_vec2[i], i));
      }
      for (ymuint i = 0; i < mReachableStates2.size(); ++ i) {
	State tmp = mReachableStates2[i];
	if ( pair_map2.count(tmp) == 0 ) {
	  State c_state = tmp.substr(0, ff_num);
	  State e_state = tmp.substr(ff_num, ff_num);
	  cout << tmp << "(" << c_state << ":" << e_state << ")"
	       << " not found in pair_vec2[]" << endl;
	  error = true;
	}
      }
      for (ymuint i = 0; i < pair_num; ++ i) {
	State tmp = pair_vec2[i];
	if ( pair_map1.count(tmp) == 0 ) {
	  State c_state = tmp.substr(0, ff_num);
	  State e_state = tmp.substr(ff_num, ff_num);
	  cout << c_state << ":" << e_state
	       << " not found in mReachableStates2[]" << endl;
	  error = true;
	}
      }
      assert_cond( !error, __FILE__, __LINE__);
      
      if ( mTransProb2.size() != trans2_map2.size() ) {
	cout << "mTransProb2.size() != trans2_map2.size()" << endl;
	error = true;
      }
      for (hash_map<ymuint, double>::iterator p = mTransProb2.begin();
	   p != mTransProb2.end(); ++ p) {
	ymuint tmp = p->first;
	double prob = p->second;
	ymuint csid = tmp / pair_num;
	ymuint nsid = tmp % pair_num;
	State cur_state = mReachableStates2[csid];
	State next_state = mReachableStates2[nsid];
	hash_map<State, ymuint>::iterator s;
	s = pair_map2.find(cur_state);
	assert_cond( s != pair_map2.end(), __FILE__, __LINE__);
	ymuint csid2 = s->second;
	s = pair_map2.find(next_state);
	assert_cond( s != pair_map2.end(), __FILE__, __LINE__);
	ymuint nsid2 = s->second;
	ymuint tmp2 = csid2 * pair_num + nsid2;
	hash_map<ymuint, double>::iterator q = trans2_map2.find(tmp2);
	State c_state = cur_state.substr(0, ff_num);
	State e_state = cur_state.substr(ff_num);
	if ( q == trans2_map2.end() ) {
	  cout << c_state << ":" << e_state << " --> " << next_state
	       << " not found in trans2_map2" << endl;
	  error = true;
	}
	else if ( prob != q->second ) {
	  cout << c_state << ":" << e_state << " --> " << next_state
	       << " prob = " << prob
	       << ", prob2 = " << q->second << endl;
	  error = true;
	}
      }
      for (hash_map<ymuint, double>::iterator p = trans2_map2.begin();
	   p != trans2_map2.end(); ++ p) {
	ymuint tmp = p->first;
	ymuint csid = tmp / pair_num;
	ymuint nsid = tmp % pair_num;
	State cur_state = pair_vec2[csid];
	State next_state = pair_vec2[nsid];
	hash_map<State, ymuint>::iterator s;
	s = pair_map1.find(cur_state);
	assert_cond( s != pair_map1.end(), __FILE__, __LINE__);
	ymuint csid2 = s->second;
	s = pair_map1.find(next_state);
	assert_cond( s != pair_map1.end(), __FILE__, __LINE__);
	ymuint nsid2 = s->second;
	ymuint tmp2 = csid2 * pair_num + nsid2;
	hash_map<ymuint, double>::iterator q = mTransProb2.find(tmp2);
	State c_state = cur_state.substr(0, ff_num);
	State e_state = cur_state.substr(ff_num, ff_num);
	if ( q == mTransProb2.end() ) {
	  cout << c_state << ":" << e_state << " --> " << next_state
	       << " not found in mTransProb2" << endl;
	  error = true;
	}
      }
      for (ymuint i = 0; i < pair_num; ++ i) {
	State cur_state = mReachableStates2[i];
	hash_map<State, ymuint>::iterator p = pair_map2.find(cur_state);
	assert_cond( p != state_map2.end(), __FILE__, __LINE__);
	ymuint id2 = p->second;
	assert_cond( pair_vec2[id2] == cur_state, __FILE__, __LINE__);
	
	if ( mFailureProb0[i] != fprob2[id2] ) {
	  cout << cur_state << ": failure probability mismatch : "
	       << "prob = " << mFailureProb0[i]
	       << ", prob2 = " << fprob2[id2] << endl;
	  error = true;
	}
      }
      assert_cond(!error, __FILE__, __LINE__);
    }
  }
  else {
    fsm_analysis2(bnetwork, init_states,
		  mReachableStates1, mTransProb1,
		  mReachableStates2, mTransProb2, mFailureProb0);
  }
}

// @brief 正常回路の定常分布を求める関数
// @note 結果は mSteadyProb に格納される．
void
MCAnalysis::calc_steady_prob()
{
  ymuint32 n = mReachableStates1.size();
  Matrix m(n, n + 1);
  for (ymuint i = 0; i < n; ++ i) {
    for (ymuint j = 0; j < n; ++ j) {
      if ( i != j ) {
	m.elem(i, j) += 1.0;
      }
    }
  }
  for (hash_map<ymuint, double>::const_iterator p = mTransProb1.begin();
       p != mTransProb1.end(); ++ p) {
    ymuint tmp = p->first;
    ymuint prob = p->second;
    ymuint row = tmp / n;
    ymuint col = tmp % n;
    m.elem(row, col) += prob;
  }
  for (ymuint i = 0; i < n; ++ i) {
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

  ymuint state_num = mReachableStates2.size();
  SMatrix m(state_num);
  for (hash_map<ymuint, double>::const_iterator p = mTransProb2.begin();
       p != mTransProb2.end(); ++ p) {
    ymuint sp_code = p->first;
    double prob = p->second;
    ymuint cur_state = sp_code / state_num;
    ymuint next_state = sp_code % state_num;
    m.set_value(cur_state, next_state, - prob);
  }
  for (ymuint i = 0; i < state_num; ++ i) {
    SmCell* cell = m.find_elem(i, i);
    double v = 1.0;
    if ( cell ) {
      v += cell->value();
    }
    m.set_value(i, i, v);
  }
  for (ymuint i = 0; i < state_num; ++ i) {
    m.set_const(i, mFailureProb0[i]);
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
  s << "# FF-num" << endl
    << mFFNum << endl
    << endl;
  ymuint ns1 = mReachableStates1.size();
  s << "# states num of original original circuit" << endl
    << ns1 << endl;
  for (ymuint i = 0; i < ns1; ++ i) {
    s << mReachableStates1[i] << endl;
  }
  s << endl;
  s << "# transition probability of original circuit" << endl;
  for (hash_map<ymuint, double>::const_iterator p = mTransProb1.begin();
       p != mTransProb1.end(); ++ p) {
    ymuint sp = p->first;
    ymuint csid = sp / ns1;
    ymuint nsid = sp % ns1;
    double prob = p->second;
    s << csid << " " << nsid << " " << prob << endl;
  }
  s << endl;
  ymuint ns2 = mReachableStates2.size();
  s << "# states num of product machine" << endl
    << ns2 << endl;
  for (ymuint i = 0; i < ns2; ++ i) {
    s << mReachableStates2[i] << endl;
  }
  s << endl;
  s << "# transition probability of product machine" << endl;
  for (hash_map<ymuint, double>::const_iterator p = mTransProb2.begin();
       p != mTransProb2.end(); ++ p) {
    ymuint sp = p->first;
    ymuint csid = sp / ns2;
    ymuint nsid = sp % ns2;
    double prob = p->second;
    s << csid << " " << nsid << " " << prob << endl;
  }
  s << endl;
  s << "# failure probability of produc machine" << endl;
  for (ymuint i = 0; i < ns2; ++ i) {
    s << mFailureProb0[i] << endl;
  }
}

// @brief 回路対の状態遷移確率行列を読み込む．
void
MCAnalysis::restore_trans(istream& s)
{
  string buff;

  while ( getline(s, buff) ) {
    if ( buff[0] == '#' || buff[0] == '\0' ) continue;
    mFFNum = atoi(buff.c_str());
#if 0
    cout << "# FF-num" << endl
	 << mFFNum << endl
	 << endl;
#endif
    break;
  }

  ymuint ns1 = 0;
  while ( getline(s, buff) ) {
    if ( buff[0] == '#' || buff[0] == '\0' ) continue;
    ns1 = atoi(buff.c_str());
#if 0
    cout << "# states num of original original circuit" << endl
	 << ns1 << endl;
#endif
    break;
  }
  mReachableStates1.clear();
  mReachableStates1.resize(ns1);
  for (ymuint i = 0; i < ns1; ++ i) {
    if ( !getline(s, buff) ) {
      cerr << "unexpected EOF" << endl;
      return;
    }
    mReachableStates1[i] = buff;
#if 0
    cout << mReachableStates1[i] << endl;
#endif
  }
#if 0
  cout << "# transition probability of original circuit" << endl;
#endif
  mTransProb1.clear();
  bool first = true;
  while ( getline(s, buff) ) {
    if ( buff[0] == '#' || buff[0] == '\0' ) {
      if ( first ) {
	continue;
      }
      else {
	break;
      }
    }
    first = false;
    istringstream tmp(buff);
    ymuint csid;
    ymuint nsid;
    double prob;
    tmp >> csid >> nsid >> prob;
#if 0
    cout << csid << " " << nsid << " " << prob << endl;
#endif
    mTransProb1.insert(make_pair(csid * ns1 + nsid, prob));
  }

  ymuint ns2 = 0;
  while ( getline(s, buff) ) {
    if ( buff[0] == '#' || buff[0] == '\0' ) continue;
    ns2 = atoi(buff.c_str());
#if 0
    cout<< "# states num of product machine" << endl
	<< ns2 << endl;
#endif
    break;
  }
  mReachableStates2.clear();
  mReachableStates2.resize(ns2);
  mFailureProb0.clear();
  mFailureProb0.resize(ns2);
  for (ymuint i = 0; i < ns2; ++ i) {
    if ( !getline(s, buff) ) {
      cerr << "unexpected EOF" << endl;
      return;
    }
    mReachableStates2[i] = buff;
#if 0
    cout << mReachableStates2[i] << endl;
#endif
  }
#if 0
  cout << "# transition probability of product machine" << endl;
#endif
  first = true;
  mTransProb2.clear();
  while ( getline(s, buff) ) {
    if ( buff[0] == '#' || buff[0] == '\0' ) {
      if ( first ) {
	continue;
      }
      else {
	break;
      }
    }
    first = false;
    istringstream tmp(buff);
    ymuint csid;
    ymuint nsid;
    double prob;
    tmp >> csid >> nsid >> prob;
#if 0
    cout << csid << " " << nsid << " " << prob << endl;
#endif
    mTransProb2.insert(make_pair(csid * ns2 + nsid, prob));
  }
#if 0
  cout << "# failure probability of produc machine" << endl;
#endif
  for (ymuint i = 0; i < ns2; ) {
    if ( !getline(s, buff) ) {
      cerr << "unexpected EOF" << endl;
      return;
    }
    if ( buff[0] == '#' || buff[0] == '\0' ) continue;
    istringstream tmp(buff);
    tmp >> mFailureProb0[i];
#if 0
    cout << mFailureProb0[i] << endl;
#endif
    ++ i;
  }
}

END_NAMESPACE_YM_SEAL
