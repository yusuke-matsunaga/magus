
/// @file NlSolver.cc
/// @brief NlSolver の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlSolver.h"
#include "NlProblem.h"
#include "NlGraph.h"
#include "NlSolution.h"
#include "GraphSat.h"
#include "GsGraphBuilder.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
// クラス NlSolver
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NlSolver::NlSolver()
{
}

// @brief デストラクタ
NlSolver::~NlSolver()
{
}

BEGIN_NONAMESPACE

inline
void
one_hot2(GraphSat& solver,
	 Literal lit1,
	 Literal lit2)
{
  solver.add_clause(~lit1, ~lit2);
  solver.add_clause( lit1,  lit2);
}

inline
void
one_hot3(GraphSat& solver,
	 Literal lit1,
	 Literal lit2,
	 Literal lit3)
{
  solver.add_clause(~lit1, ~lit2);
  solver.add_clause(~lit1, ~lit3);
  solver.add_clause(~lit2, ~lit3);
  solver.add_clause( lit1,  lit2,  lit3);
}

inline
void
one_hot4(GraphSat& solver,
	 Literal lit1,
	 Literal lit2,
	 Literal lit3,
	 Literal lit4)
{
  solver.add_clause(~lit1, ~lit2);
  solver.add_clause(~lit1, ~lit3);
  solver.add_clause(~lit1, ~lit4);
  solver.add_clause(~lit2, ~lit3);
  solver.add_clause(~lit2, ~lit4);
  solver.add_clause(~lit3, ~lit4);
  solver.add_clause( lit1,  lit2,  lit3,  lit4);
}

inline
void
zero_two_hot2(GraphSat& solver,
	      Literal lit1,
	      Literal lit2)
{
  solver.add_clause(~lit1,  lit2);
  solver.add_clause( lit1, ~lit2);
}

inline
void
zero_two_hot3(GraphSat& solver,
	      Literal lit1,
	      Literal lit2,
	      Literal lit3)
{
  solver.add_clause(~lit1, ~lit2, ~lit3);
  solver.add_clause( lit1, ~lit2,  lit3);
  solver.add_clause( lit1,  lit2, ~lit3);
  solver.add_clause(~lit1,  lit2,  lit3);
}

inline
void
zero_two_hot4(GraphSat& solver,
	      Literal lit1,
	      Literal lit2,
	      Literal lit3,
	      Literal lit4)
{
  solver.add_clause( lit1,  lit2,  lit3, ~lit4);
  solver.add_clause( lit1,  lit2, ~lit3,  lit4);
  solver.add_clause( lit1, ~lit2,  lit3,  lit4);
  solver.add_clause( lit1, ~lit2, ~lit3, ~lit4);
  solver.add_clause(~lit1,  lit2,  lit3,  lit4);
  solver.add_clause(~lit1,  lit2, ~lit3, ~lit4);
  solver.add_clause(~lit1, ~lit2,  lit3, ~lit4);
  solver.add_clause(~lit1, ~lit2, ~lit3,  lit4);
  solver.add_clause(~lit1, ~lit2, ~lit3, ~lit4);
}

END_NONAMESPACE


// @brief 問題を解く
// @param[in] problem 問題
// @param[out] solution 解
void
NlSolver::solve(const NlProblem& problem,
		NlSolution& solution)
{
  GraphSat solver;

  NlGraph graph;
  graph.set_problem(problem);

  make_base_cnf(solver, graph);

  ymuint w = graph.width();
  ymuint h = graph.height();
  ymuint max_node_id = graph.max_node_id();
  ymuint max_edge_id = graph.max_edge_id();
  for (ymuint wire_idx = 0; wire_idx < graph.num(); ++ wire_idx) {
    GsGraphBuilder builder(max_node_id);

    // 横の枝を作る．
    for (ymuint x = 0; x < w - 1; ++ x) {
      for (ymuint y = 0; y < h; ++ y) {
	const NlNode* node1 = graph.node(x,     y);
	const NlNode* node2 = graph.node(x + 1, y);
	if ( (node1->terminal_id() > 0 && node1->terminal_id() != wire_idx + 1) ||
	     (node2->terminal_id() > 0 && node2->terminal_id() != wire_idx + 1) ) {
	  // この枝は使えない．
	  continue;
	}
	ymuint node1_id = x * h + y;
	ymuint node2_id = (x + 1) * h + y;
	ymuint edge_id = node1->right_edge();
	VarId var = edge_var(edge_id, wire_idx);
	builder.add_edge(node1_id, node2_id, var);
      }
    }

    // 縦の枝を作る．
    for (ymuint y = 0; y < h - 1; ++ y) {
      for (ymuint x = 0; x < w; ++ x) {
	const NlNode* node1 = graph.node(x, y);
	const NlNode* node2 = graph.node(x, y + 1);
	if ( (node1->terminal_id() > 0 && node1->terminal_id() != wire_idx + 1) ||
	     (node2->terminal_id() > 0 && node2->terminal_id() != wire_idx + 1) ) {
	  // この枝は使えない．
	  continue;
	}
	ymuint node1_id = x * h + y;
	ymuint node2_id = x * h + y + 1;
	ymuint edge_id = node1->lower_edge();
	VarId var = edge_var(edge_id, wire_idx);
	builder.add_edge(node1_id, node2_id, var);
      }
    }

    // 始点の情報をセットする．
    const NlNode* start_node = graph.start_node(wire_idx);
    {
      ymuint x = start_node->x();
      ymuint y = start_node->y();
      ymuint node_id = x * h + y;
      builder.set_start_node(node_id);
    }

    // 終点の情報をセットする．
    const NlNode* end_node = graph.end_node(wire_idx);
    {
      ymuint x = end_node->x();
      ymuint y = end_node->y();
      ymuint node_id = x * h + y;
      builder.set_end_node(node_id);
    }

    // ソルバにセットする．
    solver.add_graph(builder);
  }

  solution.init(problem);

  vector<Literal> assumptions;
  vector<Bool3> model;
  Bool3 stat = solver.solve(assumptions, model);
  switch ( stat ) {
  case kB3True:
    cout << "SAT" << endl;
    setup_solution(graph, model, solution);
    break;

  case kB3False:
    cout << "UNSAT" << endl;
    break;

  case kB3X:
    cout << "ABORT" << endl;
    break;
  }
}

// @brief 基本的な制約を作る．
void
NlSolver::make_base_cnf(GraphSat& solver,
			const NlGraph& graph)
{
  ymuint width = graph.width();
  ymuint height = graph.height();
  ymuint num = graph.num();

  mNum = num;
  ymuint max_edge_id = graph.max_edge_id();
  mVarArray.clear();
  mVarArray.resize(max_edge_id * num);

  // 横の辺に対する one-hot 制約を作る．
  for (ymuint x = 1; x < width; ++ x) {
    for (ymuint y = 0; y < height; ++ y) {
      ymuint edge = graph.left_edge(x, y);
      vector<Literal> tmp_lits(num);
      // 変数を作る．
      for (ymuint k = 0; k < num; ++ k) {
	VarId var = solver.new_var();
	set_edge_var(edge, k, var);
	Literal lit = Literal(var);
	tmp_lits[k] = lit;
      }
      // 同時に2つ以上の変数が1にならないようにする．
      for (ymuint k1 = 0; k1 < num; ++ k1) {
	Literal lit1 = tmp_lits[k1];
	for (ymuint k2 = k1 + 1; k2 < num; ++ k2) {
	  Literal lit2 = tmp_lits[k2];
	  solver.add_clause(~lit1, ~lit2);
	}
      }
    }
  }

  // 縦の辺に対する one-hot 制約を作る．
  for (ymuint y = 1; y < height; ++ y) {
    for (ymuint x = 0; x < width; ++ x) {
      ymuint edge = graph.upper_edge(x, y);
      vector<Literal> tmp_lits(num);
      // 変数を作る．
      for (ymuint k = 0; k < num; ++ k) {
	VarId var = solver.new_var();
	set_edge_var(edge, k, var);
	Literal lit = Literal(var);
	tmp_lits[k] = lit;
      }
      // 同時に2つ以上の変数が1にならないようにする．
      for (ymuint k1 = 0; k1 < num; ++ k1) {
	Literal lit1 = tmp_lits[k1];
	for (ymuint k2 = k1 + 1; k2 < num; ++ k2) {
	  Literal lit2 = tmp_lits[k2];
	  solver.add_clause(~lit1, ~lit2);
	}
      }
    }
  }

  // 枝の条件を作る．
  for (ymuint x = 0; x < width; ++ x) {
    for (ymuint y = 0; y < height; ++ y) {
      const NlNode* node = graph.node(x, y);
      const vector<ymuint>& edge_list = node->edge_list();
      ymuint ne = edge_list.size();
      for (ymuint k = 0; k < num; ++ k) {
	// (x, y) のノードに隣接する枝のリテラルのリストを作る．
	vector<Literal> lit_list;
	lit_list.reserve(ne);
	for (ymuint i = 0; i < ne; ++ i) {
	  ymuint edge = edge_list[i];
	  VarId var = edge_var(edge, k);
	  Literal lit(var);
	  lit_list.push_back(lit);
	}

	ymuint term_id = node->terminal_id();
	if ( term_id > 0 ) {
	  if ( term_id == k + 1 ) {
	    // 端点の場合
	    // 必ずただ1つの枝が選ばれる．
	    switch ( ne ) {
	    case 2:
	      one_hot2(solver, lit_list[0], lit_list[1]);
	      break;

	    case 3:
	      one_hot3(solver, lit_list[0], lit_list[1], lit_list[2]);
	      break;

	    case 4:
	      one_hot4(solver, lit_list[0], lit_list[1], lit_list[2], lit_list[3]);
	      break;

	    default:
	      ASSERT_NOT_REACHED;
	    }
	  }
	  else {
	    // 選ばれない．
	    switch ( ne ) {
	    case 2:
	      solver.add_clause(~lit_list[0]);
	      solver.add_clause(~lit_list[1]);
	      break;

	    case 3:
	      solver.add_clause(~lit_list[0]);
	      solver.add_clause(~lit_list[1]);
	      solver.add_clause(~lit_list[2]);
	      break;

	    case 4:
	      solver.add_clause(~lit_list[0]);
	      solver.add_clause(~lit_list[1]);
	      solver.add_clause(~lit_list[2]);
	      solver.add_clause(~lit_list[3]);
	      break;

	    default:
	      ASSERT_NOT_REACHED;
	    }
	  }
	}
	else {
	  // そうでない場合
	  // 0個か2個の枝が選ばれる．
	  switch ( ne ) {
	  case 2:
	    zero_two_hot2(solver, lit_list[0], lit_list[1]);
	    break;

	  case 3:
	    zero_two_hot3(solver, lit_list[0], lit_list[1], lit_list[2]);
	    break;

	  case 4:
	    zero_two_hot4(solver, lit_list[0], lit_list[1], lit_list[2], lit_list[3]);
	    break;

	  default:
	    ASSERT_NOT_REACHED;
	  }
	}
      }

      // このノード上で異なる番号の線分が交わることはない．
      for (ymuint i1 = 0; i1 < ne; ++ i1) {
	ymuint edge1 = edge_list[i1];
	for (ymuint i2 = 0; i2 < ne; ++ i2) {
	  ymuint edge2 = edge_list[i2];
	  for (ymuint k1 = 0; k1 < num; ++ k1) {
	    VarId var1 = edge_var(edge1, k1);
	    Literal lit1(var1);
	    for (ymuint k2 = k1 + 1; k2 < num; ++ k2) {
	      VarId var2 = edge_var(edge2, k2);
	      Literal lit2(var2);
	      solver.add_clause(~lit1, ~lit2);
	    }
	  }
	}
      }
    }
  }
}

// @brief 解を出力する．
// @param[in] model SATの解
// @param[in] solution 解
void
NlSolver::setup_solution(const NlGraph& graph,
			 const vector<Bool3>& model,
			 NlSolution& solution)
{
  ymuint width = graph.width();
  ymuint height = graph.height();
  ymuint num = graph.num();
  for (ymuint y = 0; y < height; ++ y) {
    for (ymuint x = 0; x < width; ++ x) {
      if ( solution.get(x, y) < 0 ) {
	continue;
      }
      const NlNode* node = graph.node(x, y);
      const vector<ymuint>& edge_list = node->edge_list();
      bool found = false;
      for (ymuint i = 0; i < edge_list.size(); ++ i) {
	ymuint edge = edge_list[i];
	for (ymuint k = 0; k < num; ++ k) {
	  VarId var = edge_var(edge, k);
	  if ( model[var.val()] == kB3True ) {
	    solution.set(x, y, k + 1);
	    found = true;
	    break;
	  }
	}
	if ( found ) {
	  break;
	}
      }
    }
  }
}

// @brief 枝の変数番号をセットする．
// @param[in] edge 枝番号 ( 1 〜 )
// @param[in] idx 線分番号
// @param[in] var 変数番号
void
NlSolver::set_edge_var(ymuint edge,
		       ymuint idx,
		       VarId var)
{
  mVarArray[(edge - 1) * mNum + idx] = var;
}

// @brief 枝の変数番号を得る．
// @param[in] edge 枝番号 ( 1 〜 )
// @param[in] idx 線分番号
VarId
NlSolver::edge_var(ymuint edge,
		   ymuint idx)
{
  return mVarArray[(edge - 1) * mNum + idx];
}

END_NAMESPACE_YM_NLINK
