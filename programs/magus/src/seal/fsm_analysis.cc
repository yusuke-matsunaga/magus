
/// @file magus/src/seal/fsm_analysis.cc
/// @brief fsm_analysis の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: fsm_analysis.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.

#if HAVE_CONFIG_H
#include "magus_config.h"
#endif


#include "MCAnalysis.h"
#include "BddFsm.h"
#include "IdxMapper.h"
#include <ym_utils/StopWatch.h>
#include <ym_bnet/BNetwork.h>
#include <ym_bdd/Bdd.h>
#include <ym_bdd/BddVarSet.h>
#include <ym_bdd/BmcFactory.h>


BEGIN_NONAMESPACE

#define REPORT_SIZE true
#define VERIFY_TRANS1 true

#ifndef VERIFY_TRANS1
#define VERIFY_TRANS1 false
#endif

#ifndef REPORT_SIZE
#define REPORT_SIZE false
#endif

const bool verify_trans1 = VERIFY_TRANS1;
const bool report_size = REPORT_SIZE;

END_NONAMESPACE


BEGIN_NAMESPACE_MAGUS_SEAL

// 与えられた回路の到達可能状態および遷移確率を求める．
// 同時に正常回路と故障回路の対の到達可能状態および遷移確率も求める．
void
fsm_analysis2(const BNetwork& bnetwork,
	      const vector<State>& init_states,
	      vector<State>& reachable_states1,
	      hash_map<State, double>& trans_map1,
	      vector<State>& reachable_states2,
	      hash_map<State, double>& trans_map2);

void
dump_trans(ostream& s,
	   const Bdd& trans,
	   const vector<State>& reachable_states,
	   const vector<ymuint>& input_vars,
	   const vector<pair<ymuint, ymuint> >& state_vars)
{
  BddMgrRef mgr = trans.mgr();
  ymuint ni = input_vars.size();
  ymuint nip = 1U << ni;
  ymuint nff = state_vars.size();
  ymuint ns = reachable_states.size();
  for (ymuint s1 = 0; s1 < ns; ++ s1) {
    Bdd tmp = trans;
    string cspat = reachable_states[s1];
    for (ymuint i = 0; i < nff; ++ i) {
      ymuint csidx = state_vars[i].first;
      if ( cspat[i] == '1' ) {
	tmp &= mgr.make_posiliteral(csidx);
      }
      else {
	tmp &= mgr.make_negaliteral(csidx);
      }
    }
    for (ymuint iv = 0; iv < nip; ++ iv) {
      Bdd tmp2 = tmp;
      string ipat = "";
      for (ymuint i = 0; i < ni; ++ i) {
	ymuint iidx = input_vars[i];
	if ( iv & (1U << i) ) {
	  tmp2 &= mgr.make_posiliteral(iidx);
	  ipat += '1';
	}
	else {
	  tmp2 &= mgr.make_negaliteral(iidx);
	  ipat += '0';
	}
      }

      ymuint count = 0;
      for (ymuint s2 = 0; s2 < ns; ++ s2) {
	Bdd tmp3 = tmp2;
	string nspat = reachable_states[s2];
	for (ymuint i = 0; i < nff; ++ i) {
	  ymuint nsidx = state_vars[i].second;
	  if ( nspat[i] == '1' ) {
	    tmp3 &= mgr.make_posiliteral(nsidx);
	  }
	  else {
	    tmp3 &= mgr.make_negaliteral(nsidx);
	  }
	}
	if ( !tmp3.is_zero() ) {
	  s << ipat << '|' << cspat << '|' << nspat << endl;
	  ++ count;
	}
	else {
	  continue;
	}
      }
      if ( count == 0 ) {
	s << ipat << '|' << cspat << '|' << "------" << endl;
	//assert_not_reached(__FILE__, __LINE__);
      }
    }
  }
}


// 与えられた回路の到達可能状態および遷移確率を求める．
// 同時に正常回路と故障回路の対の到達可能状態および遷移確率も求める．
void
MCAnalysis::enum_states(const BNetwork& bnetwork,
			const vector<State>& init_states)
{
  StopWatch sw;
  sw.start();
  
  BddMgrRef mgr(nsBdd::BmcFactory("classic mgr"));
  nsBdd::BddMgrParam param;
  param.mMemLimit = 1024 * 1024 * 1024;
  mgr.param(param, nsBdd::BddMgrParam::MEM_LIMIT);
  
  // 外部入力数
  ymuint input_num = bnetwork.input_num();
  // 外部出力数
  ymuint output_num = bnetwork.output_num();
  // FF数
  ymuint ff_num = bnetwork.latch_node_num();
  
  // 外部入力変数番号を納める配列
  vector<ymuint> input_vars(input_num);
  // 外部出力変数番号を納める配列
  vector<ymuint> output_vars(output_num);
  // もとの回路の状態変数(現状態変数と次状態変数のペア)番号を納める配列
  vector<pair<ymuint, ymuint> > state_vars(ff_num);
  // 正常回路と故障回路の積の状態変数番号を納める配列
  vector<pair<ymuint, ymuint> > state_vars2(ff_num * 2 + 2);
  
  // 変数番号のマップを作るオブジェクト
  IdxMapper idxmap(input_num, output_num, ff_num);

  // 論理ノードに対応する global function を納める配列
  vector<Bdd> bdd_array(bnetwork.max_node_id());
  
  // 入力の変数番号配列のセットと入力変数用のBDDの作成
  {
    ymuint var_num = 0;
    for (BNodeList::const_iterator p = bnetwork.inputs_begin();
	 p != bnetwork.inputs_end(); ++ p, ++ var_num) {
      BNode* node = *p;
      ymuint id = idxmap.input_idx(var_num);
      input_vars[var_num] = id;
      bdd_array[node->id()] = mgr.make_posiliteral(id);
    }
  }
  
  // 状態の変数番号配列のセットと現状態変数用のBDDの作成
  {
    ymuint var_num = 0;
    for (BNodeList::const_iterator p = bnetwork.latch_nodes_begin();
	 p != bnetwork.latch_nodes_end(); ++ p, ++ var_num) {
      BNode* node = *p;
      ymuint cur_id  = idxmap.cur_normal_idx(var_num);
      ymuint next_id = idxmap.next_normal_idx(var_num);
      state_vars[var_num] = make_pair(cur_id, next_id);
      bdd_array[node->id()] = mgr.make_posiliteral(cur_id);
    }
  }

  // 論理ノードのBDDの作成
  BNodeVector tmp_list;
  bnetwork.tsort(tmp_list);
  for (BNodeVector::iterator p = tmp_list.begin();
       p != tmp_list.end(); ++ p) {
    BNode* node = *p;
    LogExpr expr = node->func();
    ymuint ni = node->ni();
    VarBddMap fanin_map;
    for (ymuint i = 0; i < ni; ++ i) {
      BNode* inode = node->fanin(i);
      fanin_map.insert(make_pair(i, bdd_array[inode->id()]));
    }
    bdd_array[node->id()] = mgr.expr_to_bdd(expr, fanin_map);
  }
  
  // 出力の変数番号配列のセットと出力関係のBDDの作成
  Bdd output_rel = mgr.make_one();
  {
    ymuint var_num = 0;
    for (BNodeList::const_iterator p = bnetwork.outputs_begin();
	 p != bnetwork.outputs_end(); ++ p, ++ var_num) {
      BNode* node = *p;
      BNode* inode = node->fanin(0);
      Bdd ofunc = bdd_array[inode->id()];
      ymuint id = idxmap.output_idx(var_num);
      output_vars[var_num] = id;
      Bdd ovar = mgr.make_posiliteral(id);
      output_rel &= ~(ovar ^ ofunc);
    }
  }

  // 次状態遷移関係のBDDの作成
  Bdd trans_rel = mgr.make_one();
  {
    ymuint var_num = 0;
    for (BNodeList::const_iterator p = bnetwork.latch_nodes_begin();
	 p != bnetwork.latch_nodes_end(); ++ p, ++ var_num) {
      BNode* node = *p;
      BNode* inode = node->fanin(0);
      Bdd ofunc = bdd_array[inode->id()];
      ymuint id = idxmap.next_normal_idx(var_num);
      Bdd ovar = mgr.make_posiliteral(id);
      trans_rel &= ~(ovar ^ ofunc);
    }
  }
  
  // 正常回路の FSM の生成
  BddFsm fsm(mgr, input_vars, state_vars, trans_rel);
  
  // 正常回路の到達可能状態を列挙
  Bdd rs_bdd = fsm.enum_reachable_states(init_states);

  // 正常回路の状態遷移確率を計算
  fsm.calc_trans_prob(rs_bdd, mTransProb1);
  
  // 到達可能状態集合を表す BDD を状態集合に変換する．
  fsm.bdd2cur_states(rs_bdd, mReachableStates1);
  
  // 正常回路の変数をエラー回路にシフトさせるためのマップ
  VarVarMap c2e_map;
  for (ymuint i = 0; i < ff_num; ++ i) {
    ymuint cur_id1 = idxmap.cur_normal_idx(i);
    ymuint cur_id2 = idxmap.cur_error_idx(i);
    c2e_map.insert(make_pair(cur_id1, cur_id2));
    ymuint next_id1 = idxmap.next_normal_idx(i);
    ymuint next_id2 = idxmap.next_error_idx(i);
    c2e_map.insert(make_pair(next_id1, next_id2));
  }
  
  for (ymuint i = 0; i < ff_num; ++ i) {
    ymuint cur_id = idxmap.cur_normal_idx(i);
    ymuint next_id = idxmap.next_normal_idx(i);
    state_vars2[i] = make_pair(cur_id, next_id);
  }
  for (ymuint i = 0; i < ff_num; ++ i) {
    ymuint cur_id = idxmap.cur_error_idx(i);
    ymuint next_id = idxmap.next_error_idx(i);
    state_vars2[i + ff_num] = make_pair(cur_id, next_id);
  }
  state_vars2[ff_num * 2 + 0] = make_pair(idxmap.cur_error_bit(),
					  idxmap.next_error_bit());
  state_vars2[ff_num * 2 + 1] = make_pair(idxmap.cur_ident_bit(),
					  idxmap.next_ident_bit());

  // エラー状態
  // 普通のフリップフロップのビットが0で {cur|next}_err_bit のみが1
  Bdd cur_err_state = mgr.make_one();
  Bdd next_err_state = mgr.make_one();
  for (ymuint i = 0; i < ff_num * 2; ++ i) {
    cur_err_state &= mgr.make_negaliteral(state_vars2[i].first);
    next_err_state &= mgr.make_negaliteral(state_vars2[i].second);
  }
  cur_err_state &= mgr.make_posiliteral(idxmap.cur_error_bit());
  cur_err_state &= mgr.make_negaliteral(idxmap.cur_ident_bit());
  next_err_state &= mgr.make_posiliteral(idxmap.next_error_bit());
  next_err_state &= mgr.make_negaliteral(idxmap.next_ident_bit());

  // 同一状態 (正常状態対)
  // 普通のフリップフロップのビットが0で {cur|next}_ident_bit のみが1
  Bdd cur_ident_state = mgr.make_one();
  Bdd next_ident_state = mgr.make_one();
  for (ymuint i = 0; i < ff_num * 2; ++ i) {
    cur_ident_state &= mgr.make_negaliteral(state_vars2[i].first);
    next_ident_state &= mgr.make_negaliteral(state_vars2[i].second);
  }
  cur_ident_state &= mgr.make_negaliteral(idxmap.cur_error_bit());
  cur_ident_state &= mgr.make_posiliteral(idxmap.cur_ident_bit());
  next_ident_state &= mgr.make_negaliteral(idxmap.next_error_bit());
  next_ident_state &= mgr.make_posiliteral(idxmap.next_ident_bit());
  
  // ノーマル状態
  Bdd cur_normal = mgr.make_one();
  cur_normal &= mgr.make_negaliteral(idxmap.cur_error_bit());
  cur_normal &= mgr.make_negaliteral(idxmap.cur_ident_bit());
  Bdd next_normal = mgr.make_one();
  next_normal &= mgr.make_negaliteral(idxmap.next_error_bit());
  next_normal &= mgr.make_negaliteral(idxmap.next_ident_bit());
  
  // エラー回路の出力関係
  Bdd err_output_rel = output_rel.remap_var(c2e_map);

  // エラー回路の次状態遷移関係
  Bdd err_trans_rel = trans_rel.remap_var(c2e_map);

  // 正常回路と故障回路の出力が同じである条件を作る．
  Bdd eq = output_rel & err_output_rel;
  eq = eq.esmooth(BddVarSet(mgr, output_vars));

  // 正常回路と故障回路の次状態が同一である条件を作る．
  Bdd ident = mgr.make_zero();
  for (ymuint i = 0; i < mReachableStates1.size(); ++ i) {
    State state = mReachableStates1[i];
    Bdd tmp = mgr.make_one();
    for (ymuint j = 0; j < ff_num; ++ j) {
      ymuint id1 = idxmap.next_normal_idx(j);
      ymuint id2 = idxmap.next_error_idx(j);
      if ( state[j] == '1' ) {
	tmp &= mgr.make_posiliteral(id1);
	tmp &= mgr.make_posiliteral(id2);
      }
      else {
	tmp &= mgr.make_negaliteral(id1);
	tmp &= mgr.make_negaliteral(id2);
      }
    }
    ident |= tmp;
  }
  
  if ( report_size ) {
    cout << "trans_rel.size() = " << trans_rel.size() << endl
	 << "err_trans_rel.size() = " << err_trans_rel.size() << endl
	 << "eq.size() = " << eq.size() << endl
	 << "ident.size() = " << ident.size() << endl;
  }
  
  // 回路対の基本の状態遷移関係
  Bdd trans_rel2 = trans_rel & err_trans_rel & cur_normal;
  if ( report_size ) {
    cout << "trans_rel2.size() = " << trans_rel2.size() << endl;
  }
  
  // 回路対の一時状態の状態遷移関係
  Bdd trans_rel3 = eq & ~ident & trans_rel2 & next_normal;
  if ( report_size ) {
    cout << "trans_rel3.size() = " << trans_rel3.size() << endl;
  }
  
  // 次状態を消去するための変数集合
  VarVector tmp(ff_num * 2);
  for (ymuint i = 0; i < ff_num * 2; ++ i) {
    tmp[i] = state_vars2[i].second;
  }
  BddVarSet nvars(mgr, tmp);

  // 出力にエラーが伝搬する遷移関係
  Bdd trans_rel4 = (~eq & trans_rel2).esmooth(nvars) & next_err_state;
  if ( report_size ) {
    cout << "trans_rel4.size() = " << trans_rel4.size() << endl;
  }

  // 次状態対が同一になる遷移関係
  Bdd trans_rel5 = (eq & ident & trans_rel2).esmooth(nvars) & next_ident_state;
  if ( report_size ) {
    cout << "trans_rel5.size() = " << trans_rel5.size() << endl;
  }

  // エラー状態の遷移
  Bdd trans_rel6 = cur_err_state & next_err_state;
  
  // 同一状態の遷移
  Bdd trans_rel7 = cur_ident_state & next_ident_state;
  
  // 全遷移
  Bdd trans_rel_all = trans_rel3 | trans_rel4 | trans_rel5 |
    trans_rel6 | trans_rel7;
  if ( report_size ) {
    cout << "trans_rel_all.size() = " << trans_rel_all.size() << endl;
  }
  
  // 回路対の FSM の生成
  BddFsm fsm2(mgr, input_vars, state_vars2, trans_rel_all);
  
  // 回路対の初期状態の作成
  vector<State> init_statepairs;
  for(ymuint i = 0; i < mReachableStates1.size(); ++ i) {
    State normal_state = mReachableStates1[i];
    for(ymuint j = 0; j < ff_num; ++ j) {
      init_statepairs.push_back(idxmap.make_error_state(normal_state, j));
    }
  }

  // 回路対の到達可能状態の列挙
  Bdd tmp_bdd2 = fsm2.enum_reachable_states(init_statepairs);
  Bdd rs_bdd2 = tmp_bdd2 & cur_normal;

  // 到達可能状態集合を表す BDD を状態集合に変換する．
  fsm2.bdd2cur_states(rs_bdd2, mReachableStates2);
  for (vector<State>::iterator p = mReachableStates2.begin();
       p != mReachableStates2.end(); ++ p) {
    State state = *p;
    *p = state.substr(0, ff_num * 2);
  }
  
  // 回路対の状態遷移確率を計算
  hash_map<StatePair, double> tmp_map;
  fsm2.calc_trans_prob(rs_bdd2, tmp_map);
  mTransProb2.clear();
  for (hash_map<StatePair, double>::iterator p = tmp_map.begin();
       p != tmp_map.end(); ++ p) {
    StatePair sp1 = p->first;
    double prob = p->second;
    State c_st = sp1.substr(0, ff_num * 2);
    State c_ex = sp1.substr(ff_num * 2, 2);
    State n_st = sp1.substr(ff_num * 2 + 2, ff_num * 2);
    State n_ex = sp1.substr(ff_num * 4 + 2, 2);
    StatePair sp2;
    if ( n_ex == "10" ) {
      sp2 = c_st + "ERROR";
    }
    else if ( n_ex == "01" ) {
      sp2 = c_st + "IDENT";
    }
    else {
      sp2 = c_st + n_st;
    }
    mTransProb2.insert(make_pair(sp2, prob));
  }
  
  ymuint n = mReachableStates2.size();
  cout << "Reachable states analysis end" << endl;
  cout << "  " << n << " states" << endl;
  
  ios::fmtflags save = cout.flags();
  cout << "  " << mTransProb2.size() << " non-zero entries"
       << "  (" << setprecision(2)
       << static_cast<double>(mTransProb2.size()) / (n * n) * 100.0
       << "\%)" << endl;
  cout.flags(save);
  
  sw.stop();
  USTime time = sw.time();
  cout << "  " << time << endl;
  
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

END_NAMESPACE_MAGUS_SEAL
