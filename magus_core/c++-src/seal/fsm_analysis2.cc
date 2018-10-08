﻿
/// @file libym_seal/fsm_analysis2.cc
/// @brief fsm_analysis2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: fsm_analysis2.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "LogicSim.h"
#include "MCAnalysis.h"


BEGIN_NAMESPACE_YM_SEAL

BEGIN_NONAMESPACE

void
enum_loop(LogicSim& logic_sim,
	  ymuint input_num,
	  const vector<State>& init_states,
	  vector<State>& reachable_states,
	  HashMap<State, ymuint>& state_map)
{
  list<State> state_queue;
  reachable_states.clear();
  state_map.clear();
  for (vector<State>::const_iterator p = init_states.begin();
       p != init_states.end(); ++ p) {
    State state = *p;
    if ( !state_map.check(state) ) {
      ymuint id = reachable_states.size();
      reachable_states.push_back(state);
      state_map.add(state, id);
      state_queue.push_back(state);
    }
  }
  ymuint n = 1U << input_num;
  while ( !state_queue.empty() ) {
    State cur_state = state_queue.front();
    state_queue.pop_front();
    for (ymuint input_vector = 0U; input_vector < n; ++ input_vector) {
      ymuint output_vector;
      State next_state;
      logic_sim(input_vector, cur_state, output_vector, next_state);
      if ( !state_map.check(next_state) ) {
	ymuint id = reachable_states.size();
	reachable_states.push_back(next_state);
	state_map.add(next_state, id);
	state_queue.push_back(next_state);
      }
    }
  }
}

void
enum_pair_loop(LogicSim& logic_sim,
	       ymuint input_num,
	       ymuint ff_num,
	       const vector<State>& init_states,
	       vector<State>& reachable_states,
	       HashMap<State, ymuint>& state_map)
{
  list<State> state_queue;
  reachable_states.clear();
  state_map.clear();
  for (vector<State>::const_iterator p = init_states.begin();
       p != init_states.end(); ++ p) {
    State state = *p;
    if ( !state_map.check(state) ) {
      ymuint id = reachable_states.size();
      reachable_states.push_back(state);
      state_map.add(state, id);
      state_queue.push_back(state);
    }
  }
  ymuint n = 1U << input_num;
  while ( !state_queue.empty() ) {
    State tmp = state_queue.front();
    state_queue.pop_front();
    State c_prev = tmp.substr(0, ff_num);
    State e_prev = tmp.substr(ff_num, ff_num);
    for (ymuint input_vector = 0U; input_vector < n; ++ input_vector) {
      ymuint c_output_vector;
      State c_next_state;
      logic_sim(input_vector, c_prev, c_output_vector, c_next_state);
      ymuint e_output_vector;
      State e_next_state;
      logic_sim(input_vector, e_prev, e_output_vector, e_next_state);
      if ( c_output_vector == e_output_vector &&
	   c_next_state != e_next_state ) {
	State sp = c_next_state + e_next_state;
	if ( !state_map.check(sp) ) {
	  ymuint id = reachable_states.size();
	  reachable_states.push_back(sp);
	  state_map.add(sp, id);
	  state_queue.push_back(sp);
	}
      }
    }
  }
}

END_NONAMESPACE


// 与えられた回路の到達可能状態および遷移確率を求める．
// 同時に正常回路と故障回路の対の到達可能状態および遷移確率も求める．
void
fsm_analysis2(const BNetwork& bnetwork,
	      const vector<State>& init_states,
	      vector<State>& reachable_states1,
	      vector<list<TransProb> >& trans_map1,
	      vector<State>& reachable_states2,
	      vector<list<TransProb> >& trans_map2,
	      vector<double>& failure_prob)
{
  LogicSim logic_sim(&bnetwork);

  ymuint input_num = bnetwork.input_num();
  ymuint ff_num = bnetwork.latch_node_num();

  HashMap<State, ymuint> state_map1;
  enum_loop(logic_sim, input_num, init_states, reachable_states1, state_map1);

  ymuint n = reachable_states1.size();

  ymuint sim_num = 1U << input_num;
  double m_weight = 1.0 / sim_num;
  trans_map1.clear();
  trans_map1.resize(n);
  for (ymuint i = 0; i < n; ++ i) {
    State cur_state = reachable_states1[i];
    HashMap<ymuint, ymuint> count_map;
    vector<ymuint> nid_list;
    for (ymuint input_vector = 0U; input_vector < sim_num; ++ input_vector) {
      ymuint output_vector;
      State next_state;
      logic_sim(input_vector, cur_state, output_vector, next_state);
      ymuint nid;
      bool stat = state_map1.find(next_state, nid);
      ASSERT_COND( stat );
      if ( count_map.check(nid) ) {
	++ count_map[nid];
      }
      else {
	count_map.add(nid, 1);
	nid_list.push_back(nid);
      }
    }

    for (vector<ymuint>::iterator p = nid_list.begin();
	 p != nid_list.end(); ++ p) {
      ymuint nid = *p;
      ymuint count = count_map[nid];
      double prob = count * m_weight;
      trans_map1[i].push_back(TransProb(nid, prob));
    }
  }

  // 回路対の初期状態の列挙
  vector<State> init_states2;
  for (ymuint i = 0; i < reachable_states1.size(); ++ i) {
    State c_state = reachable_states1[i];
    for (ymuint j = 0; j < ff_num; ++ j) {
      State e_state = c_state;
      if ( e_state[j] == '0' ) {
	e_state[j] = '1';
      }
      else {
	e_state[j] = '0';
      }
      init_states2.push_back(c_state + e_state);
    }
  }

  HashMap<State, ymuint> state_map2;
  enum_pair_loop(logic_sim, input_num, ff_num, init_states2,
		 reachable_states2, state_map2);

  ymuint pair_num = reachable_states2.size();
  failure_prob.resize(pair_num);
  trans_map2.clear();
  trans_map2.resize(pair_num);
  for (ymuint i = 0; i < pair_num; ++ i) {
    State p_pair = reachable_states2[i];
    State c_state = p_pair.substr(0, ff_num);
    State e_state = p_pair.substr(ff_num, ff_num);
    HashMap<ymuint, ymuint> count_map;
    vector<ymuint> nid_list;
    ymuint fcount = 0;
    for (ymuint input_vector = 0U; input_vector < sim_num; ++ input_vector) {
      ymuint c_output;
      State c_next;
      logic_sim(input_vector, c_state, c_output, c_next);
      ymuint e_output;
      State e_next;
      logic_sim(input_vector, e_state, e_output, e_next);
      if ( c_output != e_output ) {
	++ fcount;
      }
      else if ( c_next != e_next ) {
	State n_pair = c_next + e_next;
	ymuint nid;
	bool stat = state_map2.find(n_pair, nid);
	ASSERT_COND( stat );
	if ( count_map.check(nid) ) {
	  ++ count_map[nid];
	}
	else {
	  count_map.add(nid, 1);
	  nid_list.push_back(nid);
	}
      }
    }

    for (vector<ymuint>::iterator p = nid_list.begin();
	 p != nid_list.end(); ++ p) {
      ymuint nid = *p;
      double prob = static_cast<double>(count_map[nid]) * m_weight;
      trans_map2[i].push_back(TransProb(nid, prob));
    }
    failure_prob[i] = static_cast<double>(fcount) * m_weight;
  }
}

END_NAMESPACE_YM_SEAL
