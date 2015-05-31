
/// @file NlSolver2_5.cc
/// @brief NlSolver2_5 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlSolver2_5.h"
#include "NlProblem.h"
#include "NlSolution.h"
#include "NlGraph.h"
#include "NlNode.h"
#include "NlEdge.h"

#include "YmLogic/SatSolver.h"
#include "YmLogic/SatMsgHandlerImpl1.h"
#include "YmLogic/SatStats.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
// クラス NlSolver2_5
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] sat_type SATソルバ名
NlSolver2_5::NlSolver2_5(const string& sat_type) :
  mSatType(sat_type)
{
}

// @brief デストラクタ
NlSolver2_5::~NlSolver2_5()
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
NlSolver2_5::solve(const NlProblem& problem,
		   bool verbose,
		   NlSolution& solution)
{
  SatSolver solver(mSatType, string(), NULL);

  NlGraph graph;

  graph.set_problem(problem);

  make_base_cnf(solver, graph);

  solution.init(problem);

  if ( verbose ) {
    SatMsgHandler* msg_handler = new SatMsgHandlerImpl1(cout);
    solver.reg_msg_handler(msg_handler);
  }

  cout << "# of variables: " << solver.variable_num() << endl
       << "# of clauses:   " << solver.clause_num() << endl
       << "# of literals:  " << solver.literal_num() << endl;

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

  SatStats stats;
  solver.get_stats(stats);
  cout << "restarts          : " << stats.mRestart << endl
       << "conflicts         : " << stats.mConflictNum << endl
       << "decisions         : " << stats.mDecisionNum << endl
       << "propagations      : " << stats.mPropagationNum << endl
       << "conflict literals : " << stats.mLearntLitNum << endl
       << "CPU time          : " << stats.mTime << endl;
}

// @brief 基本的な制約を作る．
void
NlSolver2_5::make_base_cnf(SatSolver& solver,
			   const NlGraph& graph)
{
  ymuint width = graph.width();
  ymuint height = graph.height();
  ymuint num = graph.num();
  ymuint ln = 0;
  while ( (1 << ln) <= num ) ++ ln;

  mNum = num;
  mLogN = ln;

  ymuint max_edge_id = graph.max_edge_id();

  // 枝の変数(枝数 x log(線分数 + 1))を作る．
  mVarArray.clear();
  mVarArray.resize(max_edge_id * ln);
  for (ymuint edge_id = 0; edge_id < max_edge_id; ++ edge_id) {
    for (ymuint i = 0; i < ln; ++ i) {
      VarId var = solvner.new_var();
      mVarArray[edge_id * ln + i] = var;
    }
  }

  // 同じノード上で異なる線分が交わらないための条件を作る．
  for (ymuint x = 0; x < width; ++ x) {
    for (ymuint y = 0; y < height; ++ y) {
      const NlNode* node = graph.node(x, y);
      const vector<const NlEdge*>& edge_list = node->edge_list();
      ymuint ne = edge_list.size();
      for (ymuint i1 = 0; i1 < ne; ++ i1) {
	const NlEdge* edge1 = edge_list[i1];
	vector<Literal> tmp_lits(ln + 1);
	for (ymuint i1 = 0; i1 < ln; ++ i1) {
	  VarId var1 = edge_var(edge1, i1);
	  Literal lit1(var1);
	  tmp_lits[i1] = lit1;
	}
	for (ymuint i2 = i1 + 1; i2 < ne; ++ i2) {
	  if ( i1 == i2 ) {
	    continue;
	  }
	  const NlEdge* edge2 = edge_list[i2];
	  for (ymuint i2 = 0; i2 < ln + 1) {
	    VarId var2 = edge_var(edge2, i2);
	    Literal lit2(var2);
	    tmp_lits[ln] = ~lit2;
	  }
	  solver.add_clause(tmp_lits);
	}
      }
    }
  }

  // 枝が径路となる条件を作る．
  for (ymuint x = 0; x < width; ++ x) {
    for (ymuint y = 0; y < height; ++ y) {
      const NlNode* node = graph.node(x, y);
      const vector<const NlEdge*>& edge_list = node->edge_list();
      ymuint ne = edge_list.size();
      for (ymuint k = 0; k < num; ++ k) {
	// (x, y) のノードに隣接する枝のリテラルのリストを作る．
	vector<Literal> lit_list(ne);
	for (ymuint i = 0; i < ne; ++ i) {
	  const NlEdge* edge = edge_list[i];
	  VarId var = edge_var(edge, k);
	  lit_list[i] = Literal(var);
	}

	ymuint term_id = node->terminal_id();
	if ( term_id > 0 ) {
	  // 端点の場合
	  // 必ずただ1つの枝が選ばれる．
	  vector<Literal> tmp_lits(ln + 1);
	  for (ymuint i1 = 0; i1 < ne; ++ i1) {
	    const NlEdge* edge = edge_list[i1];
	    for (ymuint j = 0; j < ln; ++ j) {
	      VarId var = edge_var(edge, j);
	      Literal lit(var);
	      if ( term_id & (1U << j) ) {
		tmp_lits[j] = ~lit;
	      }
	      else {
		tmp_lits[j] = lit;
	      }
	    }
	    for (ymuint i2 = 0; i2 < ne; ++ i2) {
	      if ( i1 == i2 ) {
		continue;
	      }
	      const NlEdge* edge2 = edge_list[i2];
	      for (ymuint j = 0; j < ln; ++ j) {
		VarId var = edge_var(edge2, j);
		Literal lit2(var);
		tmp_lits[ln] = ~lit2;
	      }
	      solver.add_clause(tmp_lits);
	    }
	  }
	}
	else {
	  // そうでない場合
	  // 0個か2個の枝が選ばれる．
	  vector<Literal> tmp_lits(ln + 1);

	}
      }
    }
  }
}

// @brief 解を出力する．
// @param[in] model SATの解
// @param[in] solution 解
void
NlSolver2_5::setup_solution(const NlGraph& graph,
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
      const vector<const NlEdge*>& edge_list = node->edge_list();
      bool found = false;
      for (ymuint i = 0; i < edge_list.size(); ++ i) {
	const NlEdge* edge = edge_list[i];
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

// @brief 枝の変数番号を得る．
// @param[in] edge 枝
// @param[in] idx 線分番号
VarId
NlSolver2_5::edge_var(const NlEdge* edge,
		    ymuint idx) const
{
  ASSERT_COND( edge != NULL );

  return mVarArray[edge->id() * mNum + idx];
}

END_NAMESPACE_YM_NLINK
