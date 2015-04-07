
/// @file TpgCnf1.cc
/// @brief TpgCnf1 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "TpgCnf1.h"
#include "TpgFault.h"
#include "TpgNetwork.h"
#include "TpgNode.h"
#include "NodeSet.h"
#include "NodeValList.h"
#include "ModelValMap.h"
#include "Extractor.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

bool do_verify = false;

void
dfs(const TpgNode* node,
    HashSet<ymuint>& mark,
    vector<const TpgNode*>& node_list)
{
  if ( mark.check(node->id()) ) {
    return;
  }
  mark.add(node->id());

  node_list.push_back(node);

  ymuint nfo = node->active_fanout_num();
  for (ymuint i = 0; i < nfo; ++ i) {
    const TpgNode* onode = node->active_fanout(i);
    dfs(onode, mark, node_list);
  }
}

struct NodeLt
{
  bool
  operator()(const TpgNode* left,
	     const TpgNode* right)
  {
    return left->id() < right->id();
  }
};

void
mark_dfs(const TpgNode* node,
	 SatEngine& engine,
	 GenVidMap& gvar_map,
	 vector<bool>&mark)
{
  if ( mark[node->id()] ) {
    return;
  }
  mark[node->id()] = true;

  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const TpgNode* inode = node->fanin(i);
    mark_dfs(inode, engine, gvar_map, mark);
  }

  VarId gvar = engine.new_var();
  gvar_map.set_vid(node, gvar);
  engine.make_node_cnf(node, gvar_map);
}

END_NONAMESPACE

void
verify_suf_list(TpgFault* fault,
		ymuint max_id,
		const NodeValList& suf_list)
{
  SatEngine engine(string(), string(), NULL);

  NodeSet node_set;

  const TpgNode* fnode = fault->node();

  node_set.mark_region(max_id, fnode);

  GenVidMap gvar_map(max_id);
  GenVidMap fvar_map(max_id);
  GenVidMap dvar_map(max_id);

  vector<bool> mark(max_id, false);

  //////////////////////////////////////////////////////////////////////
  // 変数の割当
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    VarId gvar = engine.new_var();
    gvar_map.set_vid(node, gvar);
    fvar_map.set_vid(node, gvar);
    mark[node->id()] = true;
  }
  for (ymuint i = 0; i < node_set.tfo_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    VarId fvar = engine.new_var();
    VarId dvar = engine.new_var();
    fvar_map.set_vid(node, fvar);
    dvar_map.set_vid(node, dvar);
  }

  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    engine.make_node_cnf(node, gvar_map);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);

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
    const TpgNode* node = nv.node();
    mark_dfs(node, engine, gvar_map, mark);
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
    const TpgNode* node = node_set.output_list()[i];
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
      HashSet<ymuint> mark;
      vector<const TpgNode*> node_list;
      dfs(fault->node(), mark, node_list);
      sort(node_list.begin(), node_list.end(), NodeLt());
      for (ymuint i = 0; i < node_list.size(); ++ i) {
	const TpgNode* node = node_list[i];

	cout << "Node#" << setw(5) << node->id() << ": ";
	cout << tmp_val.gval(node)
	     << "/" << tmp_val.fval(node)
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
	const TpgNode* node = nv.node();
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
	const TpgNode* node = nv.node();
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


//////////////////////////////////////////////////////////////////////
// クラス TpgCnf1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] sat_type SATソルバの種類を表す文字列
// @param[in] sat_option SATソルバに渡すオプション文字列
// @param[in] sat_outp SATソルバ用の出力ストリーム
TpgCnf1::TpgCnf1(const string& sat_type,
		 const string& sat_option,
		 ostream* sat_outp) :
  mEngine(sat_type, sat_option, sat_outp)
{
  mBackTracer = new_BtJust2();
}

// @brief デストラクタ
TpgCnf1::~TpgCnf1()
{
}

// @brief 正常回路と故障回路のCNFを作る．
// @param[in] fault 故障
// @param[in] max_id ノードの最大番号
void
TpgCnf1::make_fval_cnf(TpgFault* fault,
		       ymuint max_id)
{
  mFault = fault;

  mMaxId = max_id;

  mNodeMark.clear();
  mNodeMark.resize(max_id, false);

  mGvarMap.init(max_id);
  mFvarMap.init(max_id);
  mDvarMap.init(max_id);

  TpgNode* fnode = fault->node();

  mNodeSet.mark_region(max_id, fnode);

  //////////////////////////////////////////////////////////////////////
  // 変数の割当
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < mNodeSet.tfo_tfi_size(); ++ i) {
    const TpgNode* node = mNodeSet.tfo_tfi_node(i);
    VarId gvar = mEngine.new_var();
    mGvarMap.set_vid(node, gvar);
    mFvarMap.set_vid(node, gvar);
    mNodeMark[node->id()] = true;
  }
  for (ymuint i = 0; i < mNodeSet.tfo_size(); ++ i) {
    const TpgNode* node = mNodeSet.tfo_tfi_node(i);
    VarId fvar = mEngine.new_var();
    VarId dvar = mEngine.new_var();
    mFvarMap.set_vid(node, fvar);
    mDvarMap.set_vid(node, dvar);
  }

  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < mNodeSet.tfo_tfi_size(); ++ i) {
    const TpgNode* node = mNodeSet.tfo_tfi_node(i);
    mEngine.make_node_cnf(node, mGvarMap);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < mNodeSet.tfo_size(); ++ i) {
    const TpgNode* node = mNodeSet.tfo_tfi_node(i);

    // 故障回路のゲートの入出力関係を表すCNFを作る．
    if ( node == fnode ) {
      mEngine.make_fault_cnf(fault, mGvarMap, mFvarMap);
    }
    else {
      mEngine.make_node_cnf(node, mFvarMap);
    }

    // D-Chain 制約を作る．
    mEngine.make_dchain_cnf(node, mGvarMap, mFvarMap, mDvarMap);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障の検出条件
  //////////////////////////////////////////////////////////////////////
  ymuint npo = mNodeSet.output_list().size();
  mEngine.tmp_lits_begin(npo);
  for (ymuint i = 0; i < npo; ++ i) {
    const TpgNode* node = mNodeSet.output_list()[i];
    Literal dlit(mDvarMap(node), false);
    mEngine.tmp_lits_add(dlit);
  }
  mEngine.tmp_lits_end();

  for (const TpgNode* node = fnode; node != NULL; node = node->imm_dom()) {
    Literal dlit(mDvarMap(node), false);
    mEngine.add_clause(dlit);
  }
}

// @brief 故障回路のCNFのもとで割当が両立するか調べる．
// @param[in] list 割当リスト
bool
TpgCnf1::check_intersect(const NodeValList& list)
{
  mEngine.assumption_begin();

  add_assumptions(list);

  vector<Bool3> sat_model;
  SatStats sat_stats;
  USTime sat_time;
  Bool3 sat_ans = mEngine.solve(sat_model, sat_stats, sat_time);
  if ( sat_ans == kB3True ) {
    return true;
  }
  return false;
}

// @brief 故障回路のCNFのもとで割当が矛盾するか調べる．
// @param[in] list 割当リスト
bool
TpgCnf1::check_conflict(const NodeValList& list)
{
  mEngine.assumption_begin();

  add_assumptions(list);

  vector<Bool3> sat_model;
  SatStats sat_stats;
  USTime sat_time;
  Bool3 sat_ans = mEngine.solve(sat_model, sat_stats, sat_time);
  if ( sat_ans == kB3False ) {
    return true;
  }
  return false;
}

// @brief 故障回路のCNFのもとで割当が両立するか調べる．
// @param[in] src_list もとの割当リスト
// @param[in] new_list 新しい割当リスト
bool
TpgCnf1::get_suf_list(const NodeValList& src_list,
		      NodeValList& new_list)
{
  mEngine.assumption_begin();

  add_assumptions(src_list);

  vector<Bool3> sat_model;
  SatStats sat_stats;
  USTime sat_time;
  Bool3 sat_ans = mEngine.solve(sat_model, sat_stats, sat_time);
  if ( sat_ans == kB3True ) {
    ModelValMap val_map(mGvarMap, mFvarMap, sat_model);
    Extractor extract(val_map);
    extract(mFault, new_list);

    if ( do_verify ) {
      verify_suf_list(mFault, mMaxId, new_list);
    }

    return true;
  }
  return false;
}

// @brief 故障を検出する十分割当を求める．
// @param[out] suf_list 十分割当
// @param[out] pi_suf_list 外部入力のみの十分割当
bool
TpgCnf1::get_suf_list2(NodeValList& suf_list,
		       NodeValList& pi_suf_list)
{
  mEngine.assumption_begin();

  vector<Bool3> sat_model;
  SatStats sat_stats;
  USTime sat_time;
  Bool3 sat_ans = mEngine.solve(sat_model, sat_stats, sat_time);
  if ( sat_ans == kB3True ) {
    ModelValMap val_map(mGvarMap, mFvarMap, sat_model);
    Extractor extract(val_map);
    extract(mFault, suf_list);

    if ( do_verify ) {
      verify_suf_list(mFault, mMaxId, suf_list);
    }

    mBackTracer->set_max_id(mMaxId);

    (*mBackTracer)(mFault->node(), mNodeSet, val_map, pi_suf_list);

    verify_suf_list(mFault, mMaxId, pi_suf_list);

    return true;
  }
  return false;
}

// @brief 仮定を追加する．
// @param[in] assign_list 割当リスト
void
TpgCnf1::add_assumptions(const NodeValList& assign_list)
{
  for (ymuint i = 0; i < assign_list.size(); ++ i) {
    NodeVal nv = assign_list[i];
    const TpgNode* node = nv.node();
    make_gval_cnf(node);
    Literal alit(mGvarMap(node), false);
    if ( nv.val() ) {
      mEngine.assumption_add(alit);
    }
    else {
      mEngine.assumption_add(~alit);
    }
  }
}

// @brief ノードのCNFを作る．
void
TpgCnf1::make_gval_cnf(const TpgNode* node)
{
  if ( mNodeMark[node->id()] ) {
    return;
  }
  mNodeMark[node->id()] = true;

  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const TpgNode* inode = node->fanin(i);
    make_gval_cnf(inode);
  }

  VarId gvar = mEngine.new_var();
  mGvarMap.set_vid(node, gvar);
  mEngine.make_node_cnf(node, mGvarMap);
}

END_NAMESPACE_YM_SATPG
