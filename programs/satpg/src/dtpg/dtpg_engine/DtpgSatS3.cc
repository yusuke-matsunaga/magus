
/// @file DtpgSatS3.cc
/// @brief DtpgSatS3 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSatS3.h"

#include "DtpgStats.h"
#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "NodeSet.h"
#include "SatEngine.h"
#include "GenVidMap.h"
#include "ModelValMap.h"
#include "Extractor.h"
#include "NodeValList.h"
#include "YmUtils/HashSet.h"


BEGIN_NAMESPACE_YM_SATPG

extern
bool
check_dominance(TpgFault* f1,
		TpgFault* f2,
		ymuint max_id);

BEGIN_NONAMESPACE
const bool do_verify = true;
END_NONAMESPACE

// @brief Single エンジンを作る．
// @param[in] sat_type SATソルバの種類を表す文字列
// @param[in] sat_option SATソルバに渡すオプション文字列
// @param[in] sat_outp SATソルバ用の出力ストリーム
// @param[in] bt バックトレーサー
// @param[in] dop パタンが求められた時に実行されるファンクタ
// @param[in] uop 検出不能と判定された時に実行されるファンクタ
DtpgEngine*
new_DtpgSatS3(const string& sat_type,
	      const string& sat_option,
	      ostream* sat_outp,
	      BackTracer& bt,
	      DetectOp& dop,
	      UntestOp& uop)
{
  return new DtpgSatS3(sat_type, sat_option, sat_outp, bt, dop, uop);
}

// @brief コンストラクタ
DtpgSatS3::DtpgSatS3(const string& sat_type,
		     const string& sat_option,
		     ostream* sat_outp,
		     BackTracer& bt,
		     DetectOp& dop,
		     UntestOp& uop) :
  DtpgSat(sat_type, sat_option, sat_outp, bt, dop, uop)
{
}

// @brief デストラクタ
DtpgSatS3::~DtpgSatS3()
{
}

BEGIN_NONAMESPACE

void
add_assumption(SatEngine& engine,
	       const NodeValList& as_list,
	       const VidMap& gvar_map)
{
  ymuint n = as_list.size();
  for (ymuint i = 0; i < n; ++ i) {
    NodeVal nv = as_list[i];
    TpgNode* node = nv.node();
    bool val = nv.val();
    Literal alit(gvar_map(node), !val);
    engine.assumption_add(alit);
  }
}

void
dfs(TpgNode* node,
    HashSet<ymuint>& mark,
    vector<TpgNode*>& node_list)
{
  if ( mark.check(node->id()) ) {
    return;
  }
  mark.add(node->id());

  node_list.push_back(node);

  ymuint nfo = node->active_fanout_num();
  for (ymuint i = 0; i < nfo; ++ i) {
    TpgNode* onode = node->active_fanout(i);
    dfs(onode, mark, node_list);
  }
}

struct NodeLt
{
  bool
  operator()(TpgNode* left,
	     TpgNode* right)
  {
    return left->id() < right->id();
  }
};

bool
check_intersect(const vector<ymuint>& list1,
		const vector<ymuint>& list2)
{
  ymuint n1 = list1.size();
  ymuint n2 = list2.size();
  ymuint i1 = 0;
  ymuint i2 = 0;
  while ( i1 < n2 && i2 < n2 ) {
    ymuint v1 = list1[i1];
    ymuint v2 = list2[i2];
    if ( v1 < v2 ) {
      ++ i1;
    }
    else if ( v1 > v2 ) {
      ++ i2;
    }
    else {
      return true;
    }
  }
  return false;
}

END_NONAMESPACE

BEGIN_NONAMESPACE

struct FiLt
{
  bool
  operator()(DtpgSatS3::FaultInfo* left,
	     DtpgSatS3::FaultInfo* right)
  {
    return left->mMaList.size() > right->mMaList.size();
  }
};

END_NONAMESPACE

// @brief テスト生成を行なう．
// @param[in] network 対象のネットワーク
// @param[in] stats 結果を格納する構造体
void
DtpgSatS3::run(TpgNetwork& network,
	       DtpgStats& stats)
{
  clear_stats();

  ymuint nn = network.active_node_num();
  ymuint max_fault_id = 0;
  { // fault_id の最大値を求める．
    for (ymuint i = 0; i < nn; ++ i) {
      TpgNode* node = network.active_node(i);
      if ( node->is_output() ) {
	continue;
      }

      ymuint nf = node->fault_num();
      for (ymuint i = 0; i < nf; ++ i) {
	TpgFault* f = node->fault(i);
	ymuint fid = f->id();
	if ( max_fault_id < fid ) {
	  max_fault_id = fid;
	}
      }
    }
    ++ max_fault_id;
  }

  mFaultInfoArray.clear();
  mFaultInfoArray.resize(max_fault_id);

  NodeSet node_set;

  // 一回目は普通のテスト生成と必要割当の検査
  vector<FaultInfo*> fault_list;
  ymuint max_id = network.max_node_id();
  for (ymuint i = 0; i < nn; ++ i) {
    TpgNode* node = network.active_node(i);
    if ( node->is_output() ) {
      continue;
    }

    node_set.mark_region(max_id, vector<TpgNode*>(1, node));

    ymuint nf = node->fault_num();
    for (ymuint i = 0; i < nf; ++ i) {
      TpgFault* f = node->fault(i);
      FaultInfo& fi = mFaultInfoArray[f->id()];
      fi.mId = f->id();
      fi.mFault = f;
      fi.mDetected = run_single(node_set, f);
      fi.mDominated = false;
      if ( fi.mDetected ) {
	fault_list.push_back(&fi);
	for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
	  TpgNode* node = node_set.tfo_tfi_node(i);
	  if ( node->is_input() ) {
	    fi.mInputList.push_back(node->id());
	  }
	}
      }
    }
  }

  ConflictSet conflict_set(max_fault_id);

  ymuint nf = fault_list.size();

#if 0
  // ma_list からわかる関係を調べる．
  for (ymuint i1 = 0; i1 < nf; ++ i1) {
    FaultInfo* fi1 = fault_list[i1];
    for (ymuint i2 = 0; i2 < nf; ++ i2) {
      if ( i1 == i2 ) {
	continue;
      }

      FaultInfo* fi2 = fault_list[i2];
      if ( fi2->mDominated ) {
	continue;
      }
      if ( check_conflict(fi1->mMaList, fi2->mMaList) ) {
	conflict_set.add(fi1->mId, fi2->mId);
	cout << fi1->mFault->str() << " and " << fi2->mFault->str() << " conflict" << endl;
      }
      if ( check_contain(fi1->mMaList, fi2->mSufList) ) {
	fi2->mDominated = true;
	cout << fi1->mFault->str() << " dominates " << fi2->mFault->str() << endl;
      }
    }
  }
#endif

  for (ymuint i = 0; i < nf; ++ i) {
    FaultInfo* fi1 = fault_list[i];
    cout << "check_dominance(" << fi1->mFault->str() << ")" << endl;
    for (ymuint j = 0; j < nf; ++ j) {
      FaultInfo* fi2 = fault_list[j];
      if ( fi1 == fi2 ) {
	continue;
      }
      if ( fi2->mDominated ) {
	continue;
      }
      if ( conflict_set.check(fi1->mId, fi2->mId) ) {
	continue;
      }

      if ( !check_intersect(fi1->mInputList, fi2->mInputList) ) {
	continue;
      }

#if 0
      if ( dominance_check(max_id, fi1, fi2) ) {
	fi2->mDominated = true;
      }
#else
      if ( check_dominance(fi1->mFault, fi2->mFault, max_id) ) {
	fi2->mDominated = true;
      }
#endif
    }
  }

#if 0
  vector<FaultInfo*> tmp_list;
  tmp_list.reserve(fault_list.size());
  for (ymuint i = 0; i < nf; ++ i) {
    FaultInfo* fi = fault_list[i];
    if ( !fi->mDominated ) {
      tmp_list.push_back(fi);
    }
  }
  sort(tmp_list.begin(), tmp_list.end(), FiLt());
  ymuint wpos = 0;
  for (ymuint i = 0; i < tmp_list.size(); ++ i) {
    FaultInfo* fi1 = tmp_list[i];
    bool contained = false;
    for (ymuint j = 0; j < i; ++ j) {
      FaultInfo* fi2 = tmp_list[j];
      if ( check_contain(fi2->mMaList, fi1->mMaList) ) {
	contained = true;
	break;
      }
    }
    if ( contained ) {
      continue;
    }
    if ( wpos < i ) {
      tmp_list[wpos] = fi1;
    }
    ++ wpos;
  }
  if ( wpos < nf ) {
    tmp_list.erase(tmp_list.begin() + wpos, tmp_list.end());
  }

  for (ymuint i = 0; i < nf; ++ i) {
    FaultInfo* fi = fault_list[nf - i - 1];
    if ( fi->mDominated ) {
      continue;
    }

    TpgFault* f = fi->mFault;
    TpgNode* fnode = f->node();

    node_set.mark_region(max_id, vector<TpgNode*>(1, fnode));

    check_other_faults(network, node_set, conflict_set, fi, tmp_list);
  }
#endif

  vector<FaultInfo*> rep_list;
  rep_list.reserve(nf);

  for (ymuint i = 0; i < nf; ++ i) {
    FaultInfo* fi = fault_list[i];
    if ( !fi->mDominated ) {
      rep_list.push_back(fi);
    }
  }

  ymuint cf0 = 0;

  ymuint nf2 = rep_list.size();
  for (ymuint i = 0; i < nf2; ++ i) {
    FaultInfo* fi1 = rep_list[i];
    cout << "check_conflict(" << fi1->mFault->str() << ")" << endl;
    for (ymuint j = i + 1; j < nf2; ++ j) {
      FaultInfo* fi2 = rep_list[j];
      if ( conflict_set.check(fi1->mId, fi2->mId) ) {
	++ cf0;
	continue;
      }

      if ( !check_intersect(fi1->mInputList, fi2->mInputList) ) {
	continue;
      }

      if ( conflict_check(max_id, fi1, fi2) ) {
	conflict_set.add(fi1->mId, fi2->mId);
      }
    }
  }

  ymuint cf1 = 0;
  for (ymuint i = 0; i < nf2; ++ i) {
    FaultInfo* fi1 = rep_list[i];
    for (ymuint j = i + 1; j < nf2; ++ j) {
      FaultInfo* fi2 = rep_list[j];
      if ( conflict_set.check(fi1->mId, fi2->mId) ) {
	++ cf1;
      }
    }
  }

  cout << "Total " << nf << " detectable faults" << endl
       << "Total " << nf2 << " representative faults" << endl
       << "Total " << cf0 << " origninal conflicts" << endl
       << "Total " << cf1 << " conflicts" << endl;

  //print_network(cout, network);

#if 0
  ymuint nf = 0;
  for (ymuint i = 0; i < fault_list.size(); ++ i) {
    TpgFault* f = fault_list[i];
    FaultInfo& fi = mFaultInfoArray[f->id()];
    if ( fi.mDominaterList.empty() ) {
      ++ nf;
    }
    cout << f->str() << ":";
    const NodeValList& suf_list = fi.mSufList;
    for (ymuint j = 0; j < suf_list.size(); ++ j) {
      NodeVal nv = suf_list[j];
      cout << " " << nv.node()->id();
      if ( nv.val() ) {
	cout << "P";
      }
      else {
	cout << "N";
      }
    }
    cout << endl;
    const NodeValList& ma_list = fi.mMaList;
    cout << "    ";
    for (ymuint j = 0; j < ma_list.size(); ++ j) {
      NodeVal nv = ma_list[j];
      cout << " " << nv.node()->id();
      if ( nv.val() ) {
	cout << "P";
      }
      else {
	cout << "N";
      }
    }
    cout << endl;
  }
  cout << "Total " << fault_list.size() << " detectable faults" << endl
       << "Total " << nf << " representative faults" << endl;

  print_network(cout, network);
#endif
#if 0
  // 二回目は正常回路の CNF のみを作る
  {
    SatEngine engine(sat_type(), sat_option(), sat_outp());

    GenVidMap gvar_map(network.max_node_id());

    //////////////////////////////////////////////////////////////////////
    // 変数の割当
    //////////////////////////////////////////////////////////////////////
    for (ymuint i = 0; i < nn; ++ i) {
      TpgNode* node = network.active_node(i);
      VarId gvar = engine.new_var();
      gvar_map.set_vid(node, gvar);
    }

    //////////////////////////////////////////////////////////////////////
    // 正常回路の CNF を生成
    //////////////////////////////////////////////////////////////////////
    for (ymuint i = 0; i < nn; ++ i) {
      TpgNode* node = network.active_node(i);
      engine.make_node_cnf(node, gvar_map);
    }

    ymuint ng = 0;

    while ( !fault_list.empty() ) {
      // fault_list 中で必要割当が最大のものを探す．
      ymuint max = 0;
      TpgFault* max_f = NULL;
      ymuint max_pos = 0;
      ymuint nf = fault_list.size();
      for (ymuint i = 0; i < nf; ++ i) {
	TpgFault* f = fault_list[i];
	FaultInfo& fi = mFaultInfoArray[f->id()];
	ymuint n = fi.mSufList.size();
	if ( max < n ) {
	  max = n;
	  max_f = f;
	  max_pos = i;
	}
      }

      cout << "[" << ng << "]: Select " << max_f->str();
      cout.flush();

      ++ ng;

      // max_f を核にして同時に検出できる故障をマージしてゆく．
      NodeValList cur_assign;
      FaultInfo& fi = mFaultInfoArray[max_f->id()];
      cur_assign = fi.mSufList;

      vector<bool> mark(nf, false);
      vector<bool> conflict(nf, false);
      mark[max_pos] = true;
      for ( ; ; ) {
	ymuint min_assign = 0;
	ymuint min_fpos = 0;
	bool found = false;
	for (ymuint i = 0; i < nf; ++ i) {
	  if ( mark[i] || conflict[i] ) {
	    continue;
	  }

	  TpgFault* f = fault_list[i];
	  FaultInfo& fi = mFaultInfoArray[f->id()];

	  // cur_assign と fi.mSufList がコンフリクトしていないか調べる．
	  if ( check_conflict(cur_assign, fi.mSufList) ) {
	    conflict[i] = true;
	    continue;
	  }

	  NodeValList tmp_list = fi.mSufList;
	  tmp_list.merge(cur_assign);

	  ymuint new_assign = tmp_list.size() - cur_assign.size();
	  if ( !found || min_assign > new_assign ) {
	    found = true;
	    min_assign = new_assign;
	    min_fpos = i;
	  }
	}
	if ( !found ) {
	  break;
	}

	TpgFault* min_f = fault_list[min_fpos];
	FaultInfo& fi = mFaultInfoArray[min_f->id()];

	// cur_assign + fi.mSufList が充足可能か調べる．

	engine.assumption_begin();
	add_assumption(engine, cur_assign, gvar_map);
	add_assumption(engine, fi.mSufList, gvar_map);

	vector<Bool3> tmp_model;
	Bool3 stat = engine.solve(tmp_model);
	if ( stat == kB3True ) {
	  // 充足可能だった．
	  cur_assign.merge(fi.mSufList);
	  mark[min_fpos] = true;
	  cout << ".";
	  cout.flush();
	}
	else {
	  // 充足不能だった．
	  conflict[min_fpos] = true;
	}
      }
      cout << endl;

      // fault_list の更新
      ymuint wpos = 0;
      for (ymuint i = 0; i < nf; ++ i) {
	if ( !mark[i] ) {
	  if ( wpos < i ) {
	    fault_list[wpos] = fault_list[i];
	  }
	  ++ wpos;
	}
      }
      if ( wpos < nf ) {
	fault_list.erase(fault_list.begin() + wpos, fault_list.end());
      }
    }

    cout << "Total " << ng << " patterns" << endl;
  }
#endif

  get_stats(stats);
}

// @brief テストパタン生成を行なう．
// @param[in] node_set ノード集合
// @param[in] flist 故障リスト
bool
DtpgSatS3::run_single(const NodeSet& node_set,
		      TpgFault* fault)
{
  TpgNode* fnode = fault->node();

  cnf_begin();

  SatEngine engine(sat_type(), sat_option(), sat_outp());

  ymuint max_id = node_set.max_id();

  GenVidMap gvar_map(max_id);
  GenVidMap fvar_map(max_id);
  GenVidMap dvar_map(max_id);

  //////////////////////////////////////////////////////////////////////
  // 変数の割当
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
    TpgNode* node = node_set.tfo_tfi_node(i);
    VarId gvar = engine.new_var();
    gvar_map.set_vid(node, gvar);
    fvar_map.set_vid(node, gvar);
  }
  for (ymuint i = 0; i < node_set.tfo_size(); ++ i) {
    TpgNode* node = node_set.tfo_tfi_node(i);
    VarId fvar = engine.new_var();
    VarId dvar = engine.new_var();
    fvar_map.set_vid(node, fvar);
    dvar_map.set_vid(node, dvar);
  }

  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
    TpgNode* node = node_set.tfo_tfi_node(i);
    engine.make_node_cnf(node, gvar_map);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_size(); ++ i) {
    TpgNode* node = node_set.tfo_tfi_node(i);

    // 故障回路のゲートの入出力関係を表すCNFを作る．
    if ( node == fnode ) {
      engine.make_fault_cnf(fault, gvar_map, fvar_map);
    }
    else {
      engine.make_node_cnf(node, fvar_map);
    }

    // D-Chain 制約を作る．
    engine.make_dchain_cnf(node, gvar_map, fvar_map, dvar_map);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障の検出条件
  //////////////////////////////////////////////////////////////////////
  ymuint npo = node_set.output_list().size();
  engine.tmp_lits_begin(npo);
  for (ymuint i = 0; i < npo; ++ i) {
    TpgNode* node = node_set.output_list()[i];
    Literal dlit(dvar_map(node), false);
    engine.tmp_lits_add(dlit);
  }
  engine.tmp_lits_end();

  cnf_end();

  // 故障に対するテスト生成を行なう．
  engine.assumption_begin();

  // dominator ノードの dvar は1でなければならない．
  for (TpgNode* node = fnode; node != NULL; node = node->imm_dom()) {
    Literal dlit(dvar_map(node), false);
    engine.assumption_add(dlit);
  }

  vector<Bool3> sat_model;
  SatStats sat_stats;
  USTime sat_time;
  Bool3 sat_ans = engine.solve(sat_model, sat_stats, sat_time);
  if ( sat_ans != kB3True ) {
    return false;
  }

  // 十分割当を求める．
  ModelValMap val_map(gvar_map, fvar_map, sat_model);
  Extractor extract(val_map);
  NodeValList& suf_list = mFaultInfoArray[fault->id()].mSufList;
  extract(fault, suf_list);

  if ( do_verify ) { // 検証
    verify_suf_list(fault, node_set, suf_list, sat_model);
  }

  // 必要割当を求める．
  vector<Bool3> tmp_model;
  NodeValList& ma_list = mFaultInfoArray[fault->id()].mMaList;
#if 1
  ymuint n = suf_list.size();
  for (ymuint i = 0; i < n; ++ i) {
    NodeVal nv = suf_list[i];

    engine.assumption_begin();

    // dominator ノードの dvar は1でなければならない．
    for (TpgNode* node = fnode; node != NULL; node = node->imm_dom()) {
      Literal dlit(dvar_map(node), false);
      engine.assumption_add(dlit);
    }

    // node の割当の反対を試す．
    TpgNode* node = nv.node();
    bool inv = nv.val();
    Literal alit(gvar_map(node), inv);
    engine.assumption_add(alit);

    SatStats sat_stats;
    USTime sat_time;
    Bool3 tmp_stat = engine.solve(tmp_model, sat_stats, sat_time);
    if ( tmp_stat == kB3True ) {
      // 反対でも検出できたので必要割当ではない．
      ;
    }
    else if ( tmp_stat == kB3False ) {
      // 失敗したということはこの割当は必要である．
      ma_list.add(node, nv.val());
    }
    else {
      // アボート．とりあえず無視
      ;
    }
  }
#else
  for (ymuint i = 0; i < tfo_tfi_size(); ++ i) {
    TpgNode* node = tfo_tfi_node(i);
    Bool3 nval = sat_model[node->gvar().val()];
    engine.assumption_begin();

    // dominator ノードの dvar は1でなければならない．
    for (TpgNode* node = fnode; node != NULL; node = node->imm_dom()) {
      Literal dlit(dvar_map(node), false);
      engine.assumption_add(dlit);
    }

    // node の割当の反対を試す．
    bool inv = (nval == kB3True);
    Literal alit(gvar_map(node), inv);
    engine.assumption_add(alit);

    Bool3 tmp_stat = engine.solve(tmp_model);
    if ( tmp_stat == kB3True ) {
      // 反対でも検出できたので必要割当ではない．
      ;
    }
    else if ( tmp_stat == kB3False ) {
      // 失敗したということはこの割当は必要である．
      ma_list.add(node, nval);
    }
    else {
      // アボート．とりあえず無視
      ;
    }
  }
  suf_list.merge(ma_list);
#endif

  cout << fault->str() << ": sufficient assignment = " << suf_list.size()
       << ", mandatory assignment = " << ma_list.size() << endl;

  return true;
}

// @brief 他の故障との関係を調べる．
void
DtpgSatS3::check_other_faults(TpgNetwork& network,
			      const NodeSet& node_set,
			      ConflictSet& conflict_set,
			      FaultInfo* fi1,
			      const vector<FaultInfo*>& fault_list)
{
  TpgFault* f1 = fi1->mFault;
  TpgNode* fnode = f1->node();
  TpgNode* fsnode = f1->source_node();

  cout << "check_other_faults(" << f1->str() << ")" << endl;

  cnf_begin();

  SatEngine engine(sat_type(), sat_option(), sat_outp());

  ymuint max_id = network.max_node_id();

  GenVidMap gvar_map(max_id);
  GenVidMap fvar_map(max_id);
  GenVidMap dvar_map(max_id);

  //////////////////////////////////////////////////////////////////////
  // 変数の割当
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
    TpgNode* node = node_set.tfo_tfi_node(i);
    VarId gvar = engine.new_var();
    gvar_map.set_vid(node, gvar);
    fvar_map.set_vid(node, gvar);
  }
  for (ymuint i = 0; i < node_set.tfo_size(); ++ i) {
    TpgNode* node = node_set.tfo_tfi_node(i);
    VarId fvar = engine.new_var();
    VarId dvar = engine.new_var();
    fvar_map.set_vid(node, fvar);
    dvar_map.set_vid(node, dvar);
  }

  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
    TpgNode* node = node_set.tfo_tfi_node(i);
    engine.make_node_cnf(node, gvar_map);
  }

  const NodeValList& ma_list1 = fi1->mMaList;

  vector<FaultInfo*> tmp_list;
  tmp_list.reserve(fault_list.size());
  for (ymuint i = 0; i < fault_list.size(); ++ i) {
    FaultInfo* fi2 = fault_list[i];

    if ( fi2->mDominated ) {
      continue;
    }

    if ( fi1 == fi2 ) {
      continue;
    }

    if ( conflict_set.check(fi1->mId, fi2->mId) ) {
      continue;
    }

    if ( !check_intersect(fi1->mInputList, fi2->mInputList) ) {
      continue;
    }

    // f2 の必要割当が f1 の TFI of TFO cone に含まれるか調べる．
    const NodeValList& ma_list2 = fi2->mMaList;
    bool found = false;
    for (ymuint i = 0; i < ma_list2.size(); ++ i) {
      NodeVal nv = ma_list2[i];
      TpgNode* node = nv.node();
      if ( node_set.tfo_mark(node) || node_set.tfi_mark(node) ) {
	found = true;
      }
    }
    if ( found ) {
      tmp_list.push_back(fi2);
    }
  }

  if ( tmp_list.empty() ) {
    return;
  }

  //////////////////////////////////////////////////////////////////////
  // 故障回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_size(); ++ i) {
    TpgNode* node = node_set.tfo_tfi_node(i);

    // 故障回路のゲートの入出力関係を表すCNFを作る．
    if ( node == fnode ) {
      engine.make_fault_cnf(f1, gvar_map, fvar_map);
    }
    else {
      engine.make_node_cnf(node, fvar_map);
    }

    // D-Chain 制約を作る．
    engine.make_dchain_cnf(node, gvar_map, fvar_map, dvar_map);
  }

  ymuint nf = tmp_list.size();
  for (ymuint i = 0; i < nf; ++ i) {
    FaultInfo* fi2 = tmp_list[i];

    // f2 の必要割当のものとで f1 が検出不能かどうか調べる．
    const NodeValList& ma_list2 = fi2->mMaList;

    // 故障に対するテスト生成を行なう．
    engine.assumption_begin();

    for (ymuint i = 0; i < ma_list2.size(); ++ i) {
      NodeVal nv = ma_list2[i];
      TpgNode* node1 = nv.node();
      if ( !node_set.tfo_mark(node1) && !node_set.tfi_mark(node1) ) {
	continue;
      }

      Literal mlit(gvar_map(node1), false);
      if ( nv.val() ) {
	engine.assumption_add(mlit);
      }
      else {
	engine.assumption_add(~mlit);
      }
    }

    { // f1 が検出不能となる条件
      ymuint npo = node_set.output_list().size();
      for (ymuint i = 0; i < npo; ++ i) {
	TpgNode* node = node_set.output_list()[i];
	Literal dlit(dvar_map(node), false);
	engine.assumption_add(~dlit);
      }
    }

    vector<Bool3> tmp_model;
    SatStats sat_stats;
    USTime sat_time;
    Bool3 tmp_stat = engine.solve(tmp_model, sat_stats, sat_time);
    if ( tmp_stat == kB3True ) {
      // 検出できなかった．
      ;
    }
    else if ( tmp_stat == kB3False ) {
      // 検出できた．
      // ということは f2 を検出する時に常に f1 も検出できることを意味する．
      fi1->mDominated = true;
      cout << fi1->mFault->str() << " is dominated by " << fi2->mFault->str() << endl;
      break;
    }
    else {
      // アボート．とりあえず無視
      ;
    }
  }

  if ( fi1->mDominated ) {
    return;
  }

  return;

  //////////////////////////////////////////////////////////////////////
  // 故障の検出条件
  //////////////////////////////////////////////////////////////////////
  ymuint npo = node_set.output_list().size();
  engine.tmp_lits_begin(npo);
  for (ymuint i = 0; i < npo; ++ i) {
    TpgNode* node = node_set.output_list()[i];
    Literal dlit(dvar_map(node), false);
    engine.tmp_lits_add(dlit);
  }
  engine.tmp_lits_end();

  for (ymuint i = 0; i < nf; ++ i) {
    FaultInfo* fi2 = tmp_list[i];

    // f2 の必要割当のものとで f1 が検出可能かどうか調べる．
    const NodeValList& ma_list2 = fi2->mMaList;

    // 故障に対するテスト生成を行なう．
    engine.assumption_begin();

    for (ymuint i = 0; i < ma_list2.size(); ++ i) {
      NodeVal nv = ma_list2[i];
      TpgNode* node1 = nv.node();
      if ( !node_set.tfo_mark(node1) && !node_set.tfi_mark(node1) ) {
	continue;
      }

      Literal mlit(gvar_map(node1), false);
      if ( nv.val() ) {
	engine.assumption_add(mlit);
      }
      else {
	engine.assumption_add(~mlit);
      }
    }

    // dominator ノードの dvar は1でなければならない．
    for (TpgNode* node = fnode; node != NULL; node = node->imm_dom()) {
      Literal dlit(dvar_map(node), false);
      engine.assumption_add(dlit);
    }

    vector<Bool3> tmp_model;
    SatStats tmp_stats;
    USTime tmp_time;
    Bool3 tmp_stat = engine.solve(tmp_model, tmp_stats, tmp_time);
    if ( tmp_stat == kB3True ) {
      // 検出できた．
      ;
    }
    else if ( tmp_stat == kB3False ) {
      // 検出できなかった．
      conflict_set.add(fi1->mId, fi2->mId);
      cout << fi1->mFault->str() << " and " << fi2->mFault->str() << " conflict" << endl;
    }
    else {
      // アボート．とりあえず無視
      ;
    }
  }
}

// @brief コンストラクタ
DtpgSatS3::FaultInfo::FaultInfo()
{
  mDetected = false;
  mDominated = false;
}

// @brief デストラクタ
DtpgSatS3::FaultInfo::~FaultInfo()
{
}

void
DtpgSatS3::verify_suf_list(TpgFault* fault,
			   const NodeSet& node_set,
			   const NodeValList& suf_list,
			   const vector<Bool3>& ref_model)
{
  SatEngine engine(sat_type(), sat_option(), sat_outp());

  TpgNode* fnode = fault->node();

  ymuint max_id = node_set.max_id();

  GenVidMap gvar_map(max_id);
  GenVidMap fvar_map(max_id);
  GenVidMap dvar_map(max_id);

  //////////////////////////////////////////////////////////////////////
  // 変数の割当
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
    TpgNode* node = node_set.tfo_tfi_node(i);
    VarId gvar = engine.new_var();
    gvar_map.set_vid(node, gvar);
    fvar_map.set_vid(node, gvar);
  }
  for (ymuint i = 0; i < node_set.tfo_size(); ++ i) {
    TpgNode* node = node_set.tfo_tfi_node(i);
    VarId fvar = engine.new_var();
    VarId dvar = engine.new_var();
    fvar_map.set_vid(node, fvar);
    dvar_map.set_vid(node, dvar);
  }

  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
    TpgNode* node = node_set.tfo_tfi_node(i);
    engine.make_node_cnf(node, gvar_map);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_size(); ++ i) {
    TpgNode* node = node_set.tfo_tfi_node(i);

    // 故障回路のゲートの入出力関係を表すCNFを作る．
    if ( node == fnode ) {
      engine.make_fault_cnf(fault, gvar_map, fvar_map);
    }
    else {
      engine.make_node_cnf(node, fvar_map);
    }

    // D-Chain 制約を作る．
    engine.make_dchain_cnf(node, gvar_map, fvar_map, dvar_map);
  }

  // 上の割当のもとでは常に故障が検出できることを検証する．
  engine.assumption_begin();

  ymuint n = suf_list.size();
  for (ymuint i = 0; i < n; ++ i) {
    NodeVal nv = suf_list[i];
    TpgNode* node = nv.node();
    VarId vid = gvar_map(node);
    bool inv = nv.val();
    Literal alit(vid, !inv);
    engine.assumption_add(alit);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障の非検出条件
  //////////////////////////////////////////////////////////////////////
  ymuint npo = node_set.output_list().size();
  for (ymuint i = 0; i < npo; ++ i) {
    TpgNode* node = node_set.output_list()[i];
    Literal dlit(dvar_map(node), false);
    engine.assumption_add(~dlit);
  }
  vector<Bool3> tmp_model;
  SatStats sat_stats;
  USTime sat_time;
  Bool3 tmp_stat = engine.solve(tmp_model, sat_stats, sat_time);
  if ( tmp_stat == kB3True ) {
    cout << fault->str() << endl;
    cout << "ERROR: not a sufficient condition" << endl;
    {
      ModelValMap tmp_val(gvar_map, fvar_map, tmp_model);
      ModelValMap sat_val(gvar_map, fvar_map, ref_model);
      HashSet<ymuint> mark;
      vector<TpgNode*> node_list;
      dfs(fault->node(), mark, node_list);
      sort(node_list.begin(), node_list.end(), NodeLt());
      for (ymuint i = 0; i < node_list.size(); ++ i) {
	TpgNode* node = node_list[i];

	cout << "Node#" << setw(5) << node->id() << ": ";
	if ( tmp_val.gval(node) != sat_val.gval(node) ||
	     tmp_val.fval(node) != sat_val.fval(node) ) {
	  cout << "*";
	}
	else {
	  cout << " ";
	}
	cout << tmp_val.gval(node)
	     << "/" << tmp_val.fval(node)
	     << " <=> " << sat_val.gval(node)
	     << "/" << sat_val.fval(node)
	     << ": ";
	if ( node->is_input() ) {
	  cout << "INPUT#" << node->input_id();
	}
	else if ( node->is_output() ) {
	  cout << "OUTPUT#" << node->output_id();
	  const TpgNode* inode = node->fanin(0);
	  cout << " = Node#" << inode->id();
	}
	else if ( node->is_logic() ) {
	  cout << node->gate_type();
	  ymuint ni = node->fanin_num();
	  if ( ni > 0 ) {
	    cout << "(";
	    for (ymuint j = 0; j < ni; ++ j) {
	      const TpgNode* inode = node->fanin(j);
	      cout << " Node#" << inode->id();
	    }
	    cout << ")";
	  }
	}
	else {
	  ASSERT_NOT_REACHED;
	}
	cout << endl;
      }

      for (ymuint i = 0; i < n; ++ i) {
	NodeVal nv = suf_list[i];
	TpgNode* node = nv.node();
	if ( node_set.tfo_mark(node) ) {
	  continue;
	}
	cout << "Node#" << node->id() << ": ";
	cout << ", gval = " << tmp_val.gval(node)
	     << ": ";
	if ( node->is_input() ) {
	  cout << "INPUT#" << node->input_id();
	}
	else if ( node->is_output() ) {
	  cout << "OUTPUT#" << node->output_id();
	  const TpgNode* inode = node->fanin(0);
	  cout << " = Node#" << inode->id();
	}
	else if ( node->is_logic() ) {
	  cout << node->gate_type();
	  ymuint ni = node->fanin_num();
	  if ( ni > 0 ) {
	    cout << "(";
	    for (ymuint j = 0; j < ni; ++ j) {
	      const TpgNode* inode = node->fanin(j);
	      cout << " Node#" << inode->id();
	    }
	    cout << ")";
	  }
	}
	else {
	  ASSERT_NOT_REACHED;
	}
	cout << endl;
      }
      for (ymuint i = 0; i < n; ++ i) {
	NodeVal nv = suf_list[i];
	TpgNode* node = nv.node();
	cout << " Node#" << node->id() << ":";
	if ( nv.val() ) {
	  cout << "1";
	}
	else {
	  cout << "0";
	}
      }
      cout << endl;
    }
    exit(1);
  }
}

bool
DtpgSatS3::conflict_check(ymuint max_id,
			  FaultInfo* fi1,
			  FaultInfo* fi2)
{
  SatEngine engine(sat_type(), sat_option(), sat_outp());

  NodeSet node_set1;
  NodeSet node_set2;

  TpgFault* f1 = fi1->mFault;
  TpgFault* f2 = fi2->mFault;

  TpgNode* fnode1 = f1->node();
  TpgNode* fnode2 = f2->node();

  node_set1.mark_region(max_id, vector<TpgNode*>(1, fnode1));
  node_set2.mark_region(max_id, vector<TpgNode*>(1, fnode2));

  GenVidMap gvar_map(max_id);

  GenVidMap fvar1_map(max_id);
  GenVidMap dvar1_map(max_id);

  GenVidMap fvar2_map(max_id);
  GenVidMap dvar2_map(max_id);


  vector<bool> mark(max_id);
  vector<TpgNode*> all_list;
  all_list.reserve(node_set1.tfo_tfi_size() + node_set2.tfo_tfi_size());
  //////////////////////////////////////////////////////////////////////
  // 変数の割当
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set1.tfo_tfi_size(); ++ i) {
    TpgNode* node = node_set1.tfo_tfi_node(i);
    mark[node->id()] = true;
    all_list.push_back(node);
    VarId gvar = engine.new_var();
    gvar_map.set_vid(node, gvar);
    fvar1_map.set_vid(node, gvar);
    fvar2_map.set_vid(node, gvar);
  }
  for (ymuint i = 0; i < node_set2.tfo_tfi_size(); ++ i) {
    TpgNode* node = node_set2.tfo_tfi_node(i);
    if ( !mark[node->id()] ) {
      all_list.push_back(node);
      VarId gvar = engine.new_var();
      gvar_map.set_vid(node, gvar);
      fvar1_map.set_vid(node, gvar);
      fvar2_map.set_vid(node, gvar);
    }
  }

  for (ymuint i = 0; i < node_set1.tfo_size(); ++ i) {
    TpgNode* node = node_set1.tfo_tfi_node(i);
    VarId fvar = engine.new_var();
    VarId dvar = engine.new_var();
    fvar1_map.set_vid(node, fvar);
    dvar1_map.set_vid(node, dvar);
  }

  for (ymuint i = 0; i < node_set2.tfo_size(); ++ i) {
    TpgNode* node = node_set2.tfo_tfi_node(i);
    VarId fvar = engine.new_var();
    VarId dvar = engine.new_var();
    fvar2_map.set_vid(node, fvar);
    dvar2_map.set_vid(node, dvar);
  }

  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < all_list.size(); ++ i) {
    TpgNode* node = all_list[i];
    engine.make_node_cnf(node, gvar_map);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障回路1の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set1.tfo_size(); ++ i) {
    TpgNode* node = node_set1.tfo_tfi_node(i);

    // 故障回路のゲートの入出力関係を表すCNFを作る．
    if ( node == fnode1 ) {
      engine.make_fault_cnf(f1, gvar_map, fvar1_map);
    }
    else {
      engine.make_node_cnf(node, fvar1_map);
    }

    // D-Chain 制約を作る．
    engine.make_dchain_cnf(node, gvar_map, fvar1_map, dvar1_map);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障1の検出条件
  //////////////////////////////////////////////////////////////////////
  ymuint npo1 = node_set1.output_list().size();
  engine.tmp_lits_begin(npo1);
  for (ymuint i = 0; i < npo1; ++ i) {
    TpgNode* node = node_set1.output_list()[i];
    Literal dlit(dvar1_map(node), false);
    engine.tmp_lits_add(dlit);
  }
  engine.tmp_lits_end();

  //////////////////////////////////////////////////////////////////////
  // 故障回路2の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set2.tfo_size(); ++ i) {
    TpgNode* node = node_set2.tfo_tfi_node(i);

    // 故障回路のゲートの入出力関係を表すCNFを作る．
    if ( node == fnode2 ) {
      engine.make_fault_cnf(f2, gvar_map, fvar2_map);
    }
    else {
      engine.make_node_cnf(node, fvar2_map);
    }

    // D-Chain 制約を作る．
    engine.make_dchain_cnf(node, gvar_map, fvar2_map, dvar2_map);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障2の検出条件
  //////////////////////////////////////////////////////////////////////
  ymuint npo2 = node_set2.output_list().size();
  engine.tmp_lits_begin(npo2);
  for (ymuint i = 0; i < npo2; ++ i) {
    TpgNode* node = node_set2.output_list()[i];
    Literal dlit(dvar2_map(node), false);
    engine.tmp_lits_add(dlit);
  }
  engine.tmp_lits_end();

  // 両方故障に対するテスト生成を行なう．
  engine.assumption_begin();

  // dominator ノードの dvar は1でなければならない．
  for (TpgNode* node = fnode1; node != NULL; node = node->imm_dom()) {
    Literal dlit(dvar1_map(node), false);
    engine.assumption_add(dlit);
  }
  for (TpgNode* node = fnode2; node != NULL; node = node->imm_dom()) {
    Literal dlit(dvar2_map(node), false);
    engine.assumption_add(dlit);
  }

  vector<Bool3> sat_model;
  SatStats sat_stats;
  USTime sat_time;
  Bool3 sat_ans = engine.solve(sat_model, sat_stats, sat_time);
  if ( sat_ans == kB3False ) {
    // 矛盾していた．
    return true;
  }
  return false;
}

bool
DtpgSatS3::dominance_check(ymuint max_id,
			   FaultInfo* fi1,
			   FaultInfo* fi2)
{
  SatEngine engine(sat_type(), sat_option(), sat_outp());

  NodeSet node_set1;
  NodeSet node_set2;

  TpgFault* f1 = fi1->mFault;
  TpgFault* f2 = fi2->mFault;

  TpgNode* fnode1 = f1->node();
  TpgNode* fnode2 = f2->node();

  node_set1.mark_region(max_id, vector<TpgNode*>(1, fnode1));
  node_set2.mark_region(max_id, vector<TpgNode*>(1, fnode2));

  GenVidMap gvar_map(max_id);

  GenVidMap fvar1_map(max_id);
  GenVidMap dvar1_map(max_id);

  GenVidMap fvar2_map(max_id);
  GenVidMap dvar2_map(max_id);


  vector<bool> mark(max_id);
  vector<TpgNode*> all_list;
  all_list.reserve(node_set1.tfo_tfi_size() + node_set2.tfo_tfi_size());
  //////////////////////////////////////////////////////////////////////
  // 変数の割当
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set1.tfo_tfi_size(); ++ i) {
    TpgNode* node = node_set1.tfo_tfi_node(i);
    mark[node->id()] = true;
    all_list.push_back(node);
    VarId gvar = engine.new_var();
    gvar_map.set_vid(node, gvar);
    fvar1_map.set_vid(node, gvar);
    fvar2_map.set_vid(node, gvar);
  }
  for (ymuint i = 0; i < node_set2.tfo_tfi_size(); ++ i) {
    TpgNode* node = node_set2.tfo_tfi_node(i);
    if ( !mark[node->id()] ) {
      all_list.push_back(node);
      VarId gvar = engine.new_var();
      gvar_map.set_vid(node, gvar);
      fvar1_map.set_vid(node, gvar);
      fvar2_map.set_vid(node, gvar);
    }
  }

  for (ymuint i = 0; i < node_set1.tfo_size(); ++ i) {
    TpgNode* node = node_set1.tfo_tfi_node(i);
    VarId fvar = engine.new_var();
    VarId dvar = engine.new_var();
    fvar1_map.set_vid(node, fvar);
    dvar1_map.set_vid(node, dvar);
  }

  for (ymuint i = 0; i < node_set2.tfo_size(); ++ i) {
    TpgNode* node = node_set2.tfo_tfi_node(i);
    VarId fvar = engine.new_var();
    VarId dvar = engine.new_var();
    fvar2_map.set_vid(node, fvar);
    dvar2_map.set_vid(node, dvar);
  }

  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < all_list.size(); ++ i) {
    TpgNode* node = all_list[i];
    engine.make_node_cnf(node, gvar_map);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障回路1の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set1.tfo_size(); ++ i) {
    TpgNode* node = node_set1.tfo_tfi_node(i);

    // 故障回路のゲートの入出力関係を表すCNFを作る．
    if ( node == fnode1 ) {
      engine.make_fault_cnf(f1, gvar_map, fvar1_map);
    }
    else {
      engine.make_node_cnf(node, fvar1_map);
    }

    // D-Chain 制約を作る．
    engine.make_dchain_cnf(node, gvar_map, fvar1_map, dvar1_map);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障1の検出条件
  //////////////////////////////////////////////////////////////////////
  ymuint npo1 = node_set1.output_list().size();
  engine.tmp_lits_begin(npo1);
  for (ymuint i = 0; i < npo1; ++ i) {
    TpgNode* node = node_set1.output_list()[i];
    Literal dlit(dvar1_map(node), false);
    engine.tmp_lits_add(dlit);
  }
  engine.tmp_lits_end();

  //////////////////////////////////////////////////////////////////////
  // 故障回路2の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set2.tfo_size(); ++ i) {
    TpgNode* node = node_set2.tfo_tfi_node(i);

    // 故障回路のゲートの入出力関係を表すCNFを作る．
    if ( node == fnode2 ) {
      engine.make_fault_cnf(f2, gvar_map, fvar2_map);
    }
    else {
      engine.make_node_cnf(node, fvar2_map);
    }

    // D-Chain 制約を作る．
    engine.make_dchain_cnf(node, gvar_map, fvar2_map, dvar2_map);
  }

#if 0
  //////////////////////////////////////////////////////////////////////
  // 故障2の検出条件
  //////////////////////////////////////////////////////////////////////
  ymuint npo2 = node_set2.output_list().size();
  engine.tmp_lits_begin(npo2);
  for (ymuint i = 0; i < npo2; ++ i) {
    TpgNode* node = node_set2.output_list()[i];
    Literal dlit(dvar2_map(node), false);
    engine.tmp_lits_add(dlit);
  }
  engine.tmp_lits_end();
#endif

  // f1 が検出可能で f2 が検出不可能な入力があるか調べる．
  engine.assumption_begin();

  // dominator ノードの dvar は1でなければならない．
  for (TpgNode* node = fnode1; node != NULL; node = node->imm_dom()) {
    Literal dlit(dvar1_map(node), false);
    engine.assumption_add(dlit);
  }

  ymuint npo2 = node_set2.output_list().size();
  for (ymuint i = 0; i < npo2; ++ i) {
    TpgNode* node = node_set2.output_list()[i];
    Literal dlit(dvar2_map(node), false);
    engine.assumption_add(~dlit);
  }

  vector<Bool3> sat_model;
  SatStats sat_stats;
  USTime sat_time;
  Bool3 sat_ans = engine.solve(sat_model, sat_stats, sat_time);
  if ( sat_ans == kB3False ) {
    // f1 を検出するときは必ず f2 も検出する．
    return true;
  }
  return false;
}

END_NAMESPACE_YM_SATPG
