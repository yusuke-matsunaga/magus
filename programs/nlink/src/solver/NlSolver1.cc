
/// @file NlSolver1.cc
/// @brief NlSolver1 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlSolver1.h"
#include "NlProblem.h"
#include "NlSolution.h"
#include "NlGraph.h"
#include "MazeRouter.h"
#include "YmLogic/SatSolver.h"
#include "YmLogic/SatMsgHandlerImpl1.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
// クラス NlSolver1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] sat_type SAT ソルバ名
NlSolver1::NlSolver1(const string& sat_type) :
  mSatType(sat_type)
{
}

// @brief デストラクタ
NlSolver1::~NlSolver1()
{
}

BEGIN_NONAMESPACE

inline
void
one_hot2(SatSolver& solver,
	 Literal lit1,
	 Literal lit2)
{
  solver.add_clause(~lit1, ~lit2);
  solver.add_clause( lit1,  lit2);
}

inline
void
one_hot3(SatSolver& solver,
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
one_hot4(SatSolver& solver,
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
one_hot(SatSolver& solver,
	const vector<Literal>& lit_list)
{
  ymuint nl = lit_list.size();
  switch ( nl ) {
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

inline
void
zero_two_hot2(SatSolver& solver,
	      Literal lit1,
	      Literal lit2)
{
  solver.add_clause(~lit1,  lit2);
  solver.add_clause( lit1, ~lit2);
}

inline
void
zero_two_hot3(SatSolver& solver,
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
zero_two_hot4(SatSolver& solver,
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

inline
void
zero_two_hot(SatSolver& solver,
	     const vector<Literal>& lit_list)
{
  ymuint nl = lit_list.size();
  switch ( nl ) {
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

END_NONAMESPACE


// @brief 問題を解く
// @param[in] problem 問題
// @param[in] verbose verbose フラグ
// @param[out] solution 解
void
NlSolver1::solve(const NlProblem& problem,
		 bool verbose,
		 NlSolution& solution)
{
  SatSolver solver(mSatType, string(), NULL);

  NlGraph graph;

  graph.set_problem(problem);

  make_base_cnf(solver, graph);

  solution.init(problem);

  ymuint num = problem.elem_num();

  if ( verbose ) {
    SatMsgHandler* msg_handler = new SatMsgHandlerImpl1(cout);
    solver.reg_msg_handler(msg_handler);
  }

  vector<Bool3> model;
  Bool3 stat = solver.solve(model);
  switch ( stat ) {
  case kB3True:
    setup_solution(graph, model, solution);
    break;

  case kB3False:
    cerr << "UNSAT" << endl;
    break;

  case kB3X:
    cerr << "ABORT" << endl;
    break;
  }
}

// @brief 基本的な制約を作る．
void
NlSolver1::make_base_cnf(SatSolver& solver,
			 const NlGraph& graph)
{
  ymuint width = graph.width();
  ymuint height = graph.height();
  ymuint num = graph.num();

  mNum = num;
  ymuint max_node_id = graph.max_node_id();
  ymuint max_edge_id = graph.max_edge_id();

  mNodeVarArray.clear();
  mNodeVarArray.resize(max_node_id * num);
  mEdgeVarArray.clear();
  mEdgeVarArray.resize(max_edge_id);

  // ノードの変数を作る．
  for (ymuint node_id = 0; node_id < max_node_id; ++ node_id) {
    for (ymuint i = 0; i < mNum; ++ i) {
      VarId var = solver.new_var();
      mNodeVarArray[node_id * mNum + i] = var;
    }
  }

  // 枝の変数を作る．
  for (ymuint edge = 1; edge <= max_edge_id; ++ edge) {
    VarId var = solver.new_var();
    set_edge_var(edge, var);
  }

  // ノードの変数に対する one-hot 制約を作る．
  for (ymuint node_id = 0; node_id < max_node_id; ++ node_id) {
    ymuint term_id = graph.node(node_id)->terminal_id();
    if ( term_id > 0 ) {
      for (ymuint k = 0; k < mNum; ++ k) {
	VarId var = mNodeVarArray[node_id * mNum + k];
	Literal lit(var);
	if ( k + 1 == term_id ) {
	  solver.add_clause(lit);
	}
	else {
	  solver.add_clause(~lit);
	}
      }
    }
    else {
      for (ymuint k1 = 0; k1 < mNum; ++ k1) {
	VarId var1 = mNodeVarArray[node_id * mNum + k1];
	Literal lit1(var1);
	for (ymuint k2 = k1 + 1; k2 < mNum; ++ k2) {
	  VarId var2 = mNodeVarArray[node_id * mNum + k2];
	  Literal lit2(var2);
	  solver.add_clause(~lit1, ~lit2);
	}
      }
    }
  }

  // 枝の条件を作る．
  for (ymuint node_id = 0; node_id < max_node_id; ++ node_id) {
    const NlNode* node = graph.node(node_id);
    const vector<ymuint>& edge_list = node->edge_list();
    ymuint ne = edge_list.size();
    // ノードに隣接する枝のリテラルのリストを作る．
    vector<Literal> lit_list(ne);
    for (ymuint i = 0; i < ne; ++ i) {
      ymuint edge = edge_list[i];
      VarId var = edge_var(edge);
      lit_list[i] = Literal(var);
    }

    ymuint term_id = node->terminal_id();
    if ( term_id > 0 ) {
      // 端点の場合
      // 必ずただ1つの枝が選ばれる．
      one_hot(solver, lit_list);
    }
    else {
      // そうでない場合
      // 0個か2個の枝が選ばれる．
      zero_two_hot(solver, lit_list);
    }
  }

  // 隣接したノード間の条件を作る．
  for (ymuint x = 1; x < width; ++ x) {
    for (ymuint y = 0; y < height; ++ y) {
      const NlNode* node1 = graph.node(x, y);
      const NlNode* node2 = graph.node(x - 1, y);
      ymuint edge = node1->left_edge();
      for (ymuint k = 0; k < num; ++ k) {
	VarId var1 = node_var(node1->id(), k);
	Literal lit1(var1);
	VarId var2 = node_var(node2->id(), k);
	Literal lit2(var2);
	VarId evar = edge_var(edge);
	Literal elit(evar);
	solver.add_clause(~elit,  lit1, ~lit2);
	solver.add_clause(~elit, ~lit1,  lit2);
      }
    }
  }
  for (ymuint y = 1; y < height; ++ y) {
    for (ymuint x = 0; x < width; ++ x) {
      const NlNode* node1 = graph.node(x, y);
      const NlNode* node2 = graph.node(x, y - 1);
      ymuint edge = node1->upper_edge();
      for (ymuint k = 0; k < num; ++ k) {
	VarId var1 = node_var(node1->id(), k);
	Literal lit1(var1);
	VarId var2 = node_var(node2->id(), k);
	Literal lit2(var2);
	VarId evar = edge_var(edge);
	Literal elit(evar);
	solver.add_clause(~elit,  lit1, ~lit2);
	solver.add_clause(~elit, ~lit1,  lit2);
      }
    }
  }
}

// @brief 解を出力する．
// @param[in] model SATの解
// @param[in] solution 解
void
NlSolver1::setup_solution(const NlGraph& graph,
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
      for (ymuint k = 0; k < num; ++ k) {
	VarId var = node_var(node->id(), k);
	if ( model[var.val()] == kB3True ) {
	  solution.set(x, y, k + 1);
	  break;
	}
      }
    }
  }
}

// @brief 節点の変数番号を得る．
// @param[in] node 節点番号
// @param[in] idx 線分番号
VarId
NlSolver1::node_var(ymuint node,
		    ymuint idx)
{
  return mNodeVarArray[node * mNum + idx];
}

// @brief 枝の変数番号をセットする．
// @param[in] edge 枝番号 ( 1 〜 )
// @param[in] idx 線分番号
// @param[in] var 変数番号
void
NlSolver1::set_edge_var(ymuint edge,
			VarId var)
{
  mEdgeVarArray[edge - 1] = var;
}

// @brief 枝の変数番号を得る．
// @param[in] edge 枝番号 ( 1 〜 )
VarId
NlSolver1::edge_var(ymuint edge)
{
  return mEdgeVarArray[edge - 1];
}

END_NAMESPACE_YM_NLINK
