
/// @file libym_aig/AigSatMgr.cc
/// @brief AigSatMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: AigMgr.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_aig/AigSatMgr.h"
#include "ym_aig/AigMgr.h"
#include "ym_aig/AigNode.h"
#include "AigSatData.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス AigSatMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] aitmgr AigMgr
// @param[in] solver SAT-solver
AigSatMgr::AigSatMgr(AigMgr& aigmgr,
		     SatSolver& solver) :
  mAigMgr(aigmgr),
  mSolver(solver)
{
}

// @brief デストラクタ
AigSatMgr::~AigSatMgr()
{
}


BEGIN_NONAMESPACE

void
dfs(AigNode* node,
    vector<AigSatData>& data_array)
{
  AigSatData& data = data_array[node->node_id()];
  if ( !data.tfi_mark() ) {
    data.set_tfi_mark();
    if ( node->is_and() ) {
      dfs(node->fanin0(), data_array);
      dfs(node->fanin1(), data_array);
    }
  }
}

void
find_leaves(AigHandle handle,
	    vector<AigSatData>& data_array,
	    list<Literal>& leaves)
{
  ymuint id = handle.node_id();
  AigSatData& data = data_array[id];
  if ( data.root_mark() || handle.node()->is_input() ) {
    Literal lit(data.varid(), handle.inv() ? kPolNega : kPolPosi);
    leaves.push_back(lit);
  }
  else {
    AigNode* node = handle.node();
    find_leaves(node->fanin0_handle(), data_array, leaves);
    find_leaves(node->fanin1_handle(), data_array, leaves);
  }
}

END_NONAMESPACE


// @brief SAT 問題を解く．
// @param[in] edge この出力を1にできるか調べる．
// @param[out] model 外部入力の割り当てを入れる配列
// @retval kB3False 充足不能
// @retval kB3True 充足可能
// @retval kB3X 不明
Bool3
AigSatMgr::sat(AigHandle edge,
	       vector<Bool3>& model)
{
  vector<AigHandle> edge_list(1);
  edge_list[0] = edge;
  return sat(edge_list, model);
}

// @brief SAT 問題を解く．
// @param[in] edge_list これらの出力を同時に1にできるか調べる．
// @param[out] model 外部入力の割り当てを入れる配列
// @retval kB3False 充足不能
// @retval kB3True 充足可能
// @retval kB3X 不明
Bool3
AigSatMgr::sat(const vector<AigHandle>& edge_list,
	       vector<Bool3>& model)
{
  ymuint n = mAigMgr.node_num();
  vector<AigSatData> data_array(n);

  // edge_list の TFI に印を付ける．
  bool have_zero = false;
  bool all_one = true;
  for (vector<AigHandle>::const_iterator p = edge_list.begin();
       p != edge_list.end(); ++ p) {
    AigHandle edge = *p;
    if ( edge.is_zero() ) {
      return kB3False;
    }
    if ( !edge.is_one() ) {
      all_one = false;
      AigNode* node = edge.node();
      dfs(node, data_array);
      data_array[node->node_id()].set_root_mark();
    }
  }
  if ( all_one ) {
    // TODO: model をどうする．
    return kB3True;
  }

  // 極大AND木の根に印をつける．
  for (ymuint i = 0; i < n; ++ i) {
    AigNode* aignode = mAigMgr.node(i);
    if ( data_array[aignode->node_id()].tfi_mark() && aignode->is_and() ) {
      if ( aignode->fanin0_inv() ) {
	AigNode* node1 = aignode->fanin0();
	data_array[node1->node_id()].set_root_mark();
      }
      if ( aignode->fanin1_inv() ) {
	AigNode* node1 = aignode->fanin1();
	data_array[node1->node_id()].set_root_mark();
      }
    }
  }

  // CNF を作成する．
  for (ymuint i = 0; i < n; ++ i) {
    AigNode* aignode = mAigMgr.node(i);
    AigSatData& data = data_array[aignode->node_id()];
    if ( data.tfi_mark() && aignode->is_and() ) {
      if ( data.root_mark() ) {
	tVarId id = mSolver.new_var();
	data.set_varid(id);
	list<Literal> leaves;
	find_leaves(aignode->fanin0_handle(), data_array, leaves);
	find_leaves(aignode->fanin1_handle(), data_array, leaves);
	Literal lito(id, kPolPosi);
	vector<Literal> tmp_lits;
	tmp_lits.reserve(leaves.size() + 1);
	for (list<Literal>::iterator p = leaves.begin();
	     p != leaves.end(); ++ p) {
	  Literal lit = *p;
	  tmp_lits.push_back(~lit);
	  mSolver.add_clause(lit, ~lito);
	}
	tmp_lits.push_back(lito);
	mSolver.add_clause(tmp_lits);
      }
    }
    else {
      // 入力ノードにも印を付ける．
      tVarId id = mSolver.new_var();
      data.set_varid(id);
    }
  }

  cout << "# of variables: " << mSolver.variable_num() << endl;
  cout << "# of clauses: " << mSolver.clause_num() << endl;

  ymuint ne = edge_list.size();
  vector<Literal> assumptions(ne);
  for (ymuint i = 0; i < ne; ++ i) {
    AigHandle edge = edge_list[i];
    tPol pol = edge.inv() ? kPolNega : kPolPosi;
    assumptions[i] = Literal(data_array[edge.node_id()].varid(), pol);
  }

  Bool3 stat = mSolver.solve(assumptions, model);
  return stat;
}

END_NAMESPACE_YM_AIG
