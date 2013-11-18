
/// @file GbmNaive.cc
/// @brief GbmNaive の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "GbmNaive.h"
#include "GbmEngine.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス GbmNaive
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GbmNaive::GbmNaive()
{
}

// @brief デストラクタ
GbmNaive::~GbmNaive()
{
}

// @brief 問題を解く
bool
GbmNaive::solve(const GbmMgr& mgr,
		GbmNodeHandle output1,
		GbmNodeHandle output2)
{
  SatSolver solver;

  ymuint nn = mgr.node_num();
  vector<VarId> node_vid_array(nn);
  for (ymuint i = 0; i < nn; ++ i) {
    VarId vid = solver.new_var();
    node_vid_array[i] = vid;
  }

  ymuint nc = mgr.conf_var_num();
  vector<VarId> conf_vid_array(nc);
  for (ymuint i = 0; i < nc; ++ i) {
    VarId vid = solver.new_var();
    conf_vid_array[i] = vid;
  }

  vector<GbmLit> node_var_array(nn);
  for (ymuint i = 0; i < nn; ++ i) {
    node_var_array[i] = node_vid_array[i];
  }

  vector<GbmLit> conf_var_array(nc);
  for (ymuint i = 0; i < nc; ++ i) {
    conf_var_array[i] = conf_vid_array[i];
  }

  vector<const GbmNode*> node_list;
  node_list.reserve(nn);
  for (ymuint id = 0; id < nn; ++ id) {
    const GbmNode* node = mgr.node(id);
    if ( node->is_input() ) {
      continue;
    }
    bool have_pi = false;
    ymuint ni = node->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      GbmNodeHandle ih = node->fanin(i);
      if ( ih.is_const() ) {
	continue;
      }
      ymuint iid = ih.id();
      const GbmNode* inode = mgr.node(iid);
      if ( inode->is_input() ) {
	have_pi = true;
	break;
      }
    }
    if ( have_pi ) {
      node_list.push_back(node);
    }
    else {
      make_node_cnf(solver, node, node_var_array, conf_var_array);
    }
  }

  ymuint ni = mgr.input_num();
  ymuint ni_exp = 1U << ni;
  for (ymuint b = 0U; b < ni_exp; ++ b) {
    for (ymuint i = 0; i < ni; ++ i) {
      const GbmNode* node = mgr.input_node(i);
      ymuint id = node->id();
      if ( b & (1U << i) ) {
	node_var_array[id] = GbmLit::make_one();
      }
      else {
	node_var_array[id] = GbmLit::make_zero();
      }
    }
    for (vector<const GbmNode*>::iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      const GbmNode* node = *p;
      make_node_cnf(solver, node, node_var_array, conf_var_array);
    }
  }

  vector<Bool3> model;
  Bool3 stat = solver.solve(model);

  return stat == kB3True;
}

// @brief ノードの入出力の関係を表す CNF 式を作る．
// @param[in] solver SAT ソルバ
// @param[in] node 対象のノード
// @param[in] node_var_array ノードの変数番号の配列
// @param[in] conf_var_array 設定変数番号の配列
void
GbmNaive::make_node_cnf(SatSolver& solver,
			const GbmNode* node,
			const vector<GbmLit>& node_var_array,
			const vector<GbmLit>& conf_var_array)
{
  if ( node->is_input() ) {
    return;
  }

  GbmEngine engine;

  GbmLit output = node_var_array[node->id()];

  ymuint ni = node->fanin_num();
  if ( node->is_and() ) {
    vector<GbmLit> inputs(ni);
    make_inputs(node, node_var_array, inputs);
    engine.make_AND(solver, inputs, output);
  }
  else if ( node->is_lut() ) {
    vector<GbmLit> inputs(ni);
    make_inputs(node, node_var_array, inputs);
    ymuint ni_exp = 1U << ni;
    vector<GbmLit> lut_vars(ni_exp);
    ymuint conf0 = node->conf_base();
    for (ymuint i = 0; i < ni_exp; ++ i) {
      lut_vars[i] = conf_var_array[conf0 + i];
    }
    engine.make_LUT(solver, inputs, lut_vars, output);
  }
  else if ( node->is_mux() ) {
    vector<GbmLit> inputs(ni);
    make_inputs(node, node_var_array, inputs);
    ymuint ns = node->conf_size();
    vector<GbmLit> s_vars(ns);
    ymuint s0 = node->conf_base();
    for (ymuint i = 0; i < ns; ++ i) {
      s_vars[i] = conf_var_array[s0 + i];
    }
    engine.make_MUX(solver, inputs, s_vars, output);
  }
}

// @brief ノードの入力を表すリテラルを作る．
// @param[in] node 対象のノード
// @param[in] node_var_array ノードの変数番号の配列
// @param[out] inputs 結果のリテラルを格納する配列
void
GbmNaive::make_inputs(const GbmNode* node,
		      const vector<GbmLit>& node_var_array,
		      vector<GbmLit>& inputs)
{
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    GbmNodeHandle ih = node->fanin(i);
    if ( ih.is_zero() ) {
      inputs[i] = GbmLit::make_zero();
    }
    else if ( ih.is_one() ) {
      inputs[i] = GbmLit::make_one();
    }
    else {
      ymuint iid = ih.id();
      GbmLit ivar = node_var_array[iid];
      if ( ih.inv() ) {
	ivar.negate();
      }
      inputs[i] = ivar;
    }
  }
}

END_NAMESPACE_YM
