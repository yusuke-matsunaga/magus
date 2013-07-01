
/// @file PhfGen.cc
/// @brief PhfGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "PhfGen.h"
#include "PhfGraph.h"
#include "PhfNode.h"
#include "PhfEdge.h"
#include "ym_logic/SatSolver.h"
#include "ym_utils/CombiGen.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
// クラス PhfGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PhfGen::PhfGen()
{
}

// @brief デストラクタ
PhfGen::~PhfGen()
{
}

// @brief マッピングを求める．
// @param[in] func_list 関数のリスト
// @param[out] g_list マッピングのリスト
bool
PhfGen::mapping(const vector<const FuncVect*>& func_list,
		vector<vector<ymuint32>* >& g_list)
{
  PhfGraph pg(func_list);

  if ( !pg.simple_check() ) {
    return false;
  }

  vector<PhfEdge*> edge_list;
  if ( !pg.acyclic_check(edge_list) ) {
    return false;
  }

  ymuint d = func_list.size();
  ymuint ne = edge_list.size();
  for (ymuint i = 0; i < ne; ++ i) {
    PhfEdge* edge = edge_list[i];
    for (ymuint j = 0; j < d; ++ j) {
      PhfNode* node1 = edge->node(j);
      if ( !node1->is_assigned() ) {
	ymuint32 val = edge->val();
	for (ymuint k = 0; k < d; ++ k) {
	  if ( k != j ) {
	    PhfNode* node2 = edge->node(k);
	    val ^= node2->val();
	  }
	}
	node1->set_val(val);
	vector<ymuint32>& g1 = *g_list[j];
	g1[node1->pat()] = val;
      }
    }
  }

  return true;
}

// @brief 分割を求める．
// @param[in] func_list 関数のリスト
// @param[out] block_map 個々の入力ごとのブロック番号を入れる配列
ymuint
PhfGen::split(const vector<const FuncVect*>& func_list,
	      vector<ymuint>& block_map)
{
  ymuint nf = func_list.size();

  PhfGraph pg(func_list);

  ymuint edge_num = pg.edge_num();
  ymuint node_num = pg.node_num();

  block_map.clear();
  block_map.resize(edge_num);

  vector<PhfEdge*> edge_list;
  vector<bool> v_mark(pg.node_num(), false);
  if ( pg.simple_check() && pg.acyclic_check(edge_list) ) {
    for (vector<PhfEdge*>::iterator p = edge_list.begin();
	 p != edge_list.end(); ++ p) {
      PhfEdge* edge = *p;
      for (ymuint i = 0; i < nf; ++ i) {
	PhfNode* node = edge->node(i);
	if ( !v_mark[node->id()] ) {
	  block_map[edge->id()] = i;
	  v_mark[node->id()] = true;
	  break;
	}
      }
    }
    return 1;
  }

  SatSolver solver;

  vector<VarId> var_array(node_num * edge_num);
  for (ymuint i = 0; i < edge_num; ++ i) {
    const PhfEdge* edge = pg.edge(i);
    for (ymuint j = 0; j < nf; ++ j) {
      const PhfNode* node = edge->node(j);
      VarId var = solver.new_var();
      var_array[node->id() * edge_num + edge->id()] = var;
    }
  }

  // 各枝に関係する変数が最低1つは true にならないといけない
  for (ymuint i = 0; i < edge_num; ++ i) {
    const PhfEdge* edge = pg.edge(i);
    vector<Literal> tmp_clause(nf);
    for (ymuint j = 0; j < nf; ++ j) {
      const PhfNode* node = edge->node(j);
      VarId var = var_array[node->id() * edge_num + edge->id()];
      tmp_clause[j] = Literal(var, kPolPosi);
    }
    solver.add_clause(tmp_clause);
  }

  // 各ノードに関係する変数は最大1しか true になってはいけない．
  for (ymuint i = 0; i < node_num; ++ i) {
    const PhfNode* node = pg.node(i);
    ymuint ne = node->edge_num();
    if ( ne < 2 ) {
      continue;
    }
    for (CombiGen cg(ne, 2); !cg.is_end(); ++ cg) {
      ymuint j1 = cg(0);
      ymuint j2 = cg(1);
      const PhfEdge* edge1 = node->edge(j1);
      const PhfEdge* edge2 = node->edge(j2);
      VarId var1 = var_array[node->id() * edge_num + edge1->id()];
      VarId var2 = var_array[node->id() * edge_num + edge2->id()];
      solver.add_clause(Literal(var1, kPolNega), Literal(var2, kPolNega));
    }
  }

  vector<Bool3> model;
  Bool3 stat = solver.solve(model);
  if ( stat != kB3True ) {
    return 0;
  }

  for (ymuint i = 0; i < edge_num; ++ i) {
    const PhfEdge* edge = pg.edge(i);
    for (ymuint j = 0; j < nf; ++ j) {
      const PhfNode* node = edge->node(j);
      ymuint pos = var_array[node->id() * edge_num + edge->id()].val();
      if ( model[pos] == kB3True ) {
	block_map[i] = j;
	break;
      }
    }
  }

  return 2;
}

END_NAMESPACE_YM_IGF
