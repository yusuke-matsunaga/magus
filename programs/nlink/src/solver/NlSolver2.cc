
/// @file NlSolver2.cc
/// @brief NlSolver2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlSolver2.h"
#include "NlProblem.h"
#include "NlSolution.h"
#include "NlGraph.h"

#include "YmLogic/SatSolver.h"
#include "YmLogic/SatMsgHandlerImpl1.h"
#include "YmLogic/SatStats.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
// クラス NlSolver2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NlSolver2::NlSolver2(const char* option)
{
  if ( option != NULL && strcmp(option, "minisat2") == 0 ) {
    mMiniSat2 = true;
  }
  else {
    mMiniSat2 = false;
  }
}

// @brief デストラクタ
NlSolver2::~NlSolver2()
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
NlSolver2::solve(const NlProblem& problem,
		 bool verbose,
		 NlSolution& solution)
{
  string type;
  if ( mMiniSat2 ) {
    type = "minisat2";
  }
  SatSolver solver(type, string(), &cout);

  NlGraph graph;

  graph.set_problem(problem);

  make_base_cnf(solver, graph);

  solution.init(problem);

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
NlSolver2::make_base_cnf(SatSolver& solver,
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

  for (ymuint x = 0; x < width; ++ x) {
    for (ymuint y = 0; y < height; ++ y) {
      const NlNode* node = graph.node(x, y);
      const vector<ymuint>& edge_list = node->edge_list();
      ymuint ne = edge_list.size();
      // このノード上で異なる番号の線分が交わることはない．
      for (ymuint i1 = 0; i1 < ne; ++ i1) {
	ymuint edge1 = edge_list[i1];
	for (ymuint i2 = 0; i2 < ne; ++ i2) {
	  if ( i1 == i2 ) {
	    continue;
	  }
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
	    one_hot(solver, lit_list);
	  }
	  else {
	    // 選ばれない．
	    for (ymuint i = 0; i < ne; ++ i) {
	      solver.add_clause(~lit_list[i]);
	    }
	  }
	}
	else {
	  // そうでない場合
	  // 0個か2個の枝が選ばれる．
	  zero_two_hot(solver, lit_list);
	}
      }
    }
  }
}

#if 0
// @brief 自明な線分を引いたうえで解を求める．
void
NlSolver2::trivial_route(ymuint k,
			 const NlConnection& con,
			 vector<Literal>& assumption)
{
  NlPoint start_point = con.start_point();
  ymuint x1 = start_point.x();
  ymuint y1 = start_point.y();
  Node* node1 = _node(x1, y1);

  NlPoint end_point = con.end_point();
  ymuint x2 = end_point.x();
  ymuint y2 = end_point.y();
  Node* node2 = _node(x2, y2);

  if ( x1 == x2 ) {
    if ( y1 > y2 ) {
      ymuint tmp = y1;
      y1 = y2;
      y2 = tmp;
    }
    for (ymuint y = y1 + 1; y < y2; ++ y) {
      Edge* edge = upper_edge(x1, y);
      Literal lit(edge->var(k));
      assumption.push_back(lit);
    }
  }
  else if ( y1 == y2 ) {
    if ( x1 > x2 ) {
      ymuint tmp = x1;
      x1 = x2;
      x2 = tmp;
    }
    for (ymuint x = x1 + 1; x < x2; ++ x) {
      Edge* edge = left_edge(x, y1);
      Literal lit(edge->var(k));
      assumption.push_back(lit);
    }
  }
}
#endif

// @brief 解を出力する．
// @param[in] model SATの解
// @param[in] solution 解
void
NlSolver2::setup_solution(const NlGraph& graph,
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
NlSolver2::set_edge_var(ymuint edge,
			ymuint idx,
			VarId var)
{
  mVarArray[(edge - 1) * mNum + idx] = var;
}

// @brief 枝の変数番号を得る．
// @param[in] edge 枝番号 ( 1 〜 )
// @param[in] idx 線分番号
VarId
NlSolver2::edge_var(ymuint edge,
		    ymuint idx)
{
  return mVarArray[(edge - 1) * mNum + idx];
}

END_NAMESPACE_YM_NLINK
