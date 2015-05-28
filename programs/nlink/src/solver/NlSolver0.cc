
/// @file NlSolver0.cc
/// @brief NlSolver0 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlSolver0.h"
#include "NlProblem.h"
#include "NlSolution.h"
#include "NlGraph.h"
#include "MazeRouter.h"
#include "YmLogic/SatSolver.h"
#include "YmLogic/SatMsgHandlerImpl1.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
// クラス NlSolver0
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] sat_type SAT ソルバ名
NlSolver0::NlSolver0(const string& sat_type) :
  mSatType(sat_type)
{
}

// @brief デストラクタ
NlSolver0::~NlSolver0()
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
NlSolver0::solve(const NlProblem& problem,
		 bool verbose,
		 NlSolution& solution)
{
  SatSolver solver(mSatType, string(), NULL);

  NlGraph graph;

  graph.set_problem(problem);

  mWidth = graph.width();
  mHeight = graph.height();
  mNum = graph.num();

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
}

// @brief 基本的な制約を作る．
void
NlSolver0::make_base_cnf(SatSolver& solver,
			 const NlGraph& graph)
{
  ymuint max_node_id = graph.max_node_id();
  ymuint max_edge_id = graph.max_edge_id();

  mEdgeVarArray.clear();
  mEdgeVarArray.resize(max_edge_id);

  // 枝の変数を作る．
  for (ymuint edge = 1; edge <= max_edge_id; ++ edge) {
    VarId var = solver.new_var();
    set_edge_var(edge, var);
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
}

// @brief 枝の割当結果からノードの割当を得る．
// @param[in] graph 問題を表すグラフ
// @param[in] model SAT の解
// @param[in] node_array ノード割当の結果
void
NlSolver0::get_node_assignment(const NlGraph& graph,
			       const vector<Bool3>& model,
			       vector<ymuint>& node_array)
{
  ymuint nn = mWidth * mHeight;
  node_array.clear();
  node_array.resize(nn);

  ymuint num = graph.num();
  for (ymuint i = 0; i < num; ++ i) {
    const NlNode* node1 = graph.start_node(i);
    const NlNode* node2 = graph.end_node(i);
    vector<const NlNode*> path_list;
    search_path(node1, model, path_list);
    if ( path_list.back() != node2 ) {
      // 違う端子とつながっていた．

    }
  }
}


// @brief 経路を求める．
// @param[in] node ノード
// @param[in] from_edge,
// @param[in] model SATの解
// @param[in] path_list 経路上のノードを納めるリスト
void
NlSolver0::search_path(const NlNode* node,
		       ymuint from_edge,
		       const vector<Bool3>& model,
		       vector<const NlNode*>& path_list)
{
  path_list.push_back(node);

  const vector<ymuint>& edge_list = node->edge_list();
  const NlNode* next_node = NULL;
  ymuint next_edge = 0;
  for (ymuint i = 0; i < edge_list.size(); ++ i) {
    ymuint edge = edge_list[i];
    if ( from_edge == edge ) {
      continue;
    }
    VarId var = edge_var(edge);
    if ( model[var.val()] == kB3True ) {
      next_edge = edge;
      break;
    }
  }
  if ( next_node != NULL ) {
    sarch_path(next_node, next_edge, model, path_list);
  }
}

// @brief 解を出力する．
// @param[in] model SATの解
// @param[in] solution 解
void
NlSolver0::setup_solution(const NlGraph& graph,
			  const vector<Bool3>& model,
			  NlSolution& solution)
{
  vector<ymuint> node_array;
  get_node_assignment(model, node_array);
  for (ymuint y = 0; y < mHeight; ++ y) {
    for (ymuint x = 0; x < mWidth; ++ x) {
      if ( solution.get(x, y) < 0 ) {
	continue;
      }
      ymuint val = node_array[x * mHeight + y];
      solution.set(x, y, val);
    }
  }
}

// @brief 枝の変数番号をセットする．
// @param[in] edge 枝番号 ( 1 〜 )
// @param[in] idx 線分番号
// @param[in] var 変数番号
void
NlSolver0::set_edge_var(ymuint edge,
			VarId var)
{
  mEdgeVarArray[edge - 1] = var;
}

// @brief 枝の変数番号を得る．
// @param[in] edge 枝番号 ( 1 〜 )
VarId
NlSolver0::edge_var(ymuint edge)
{
  return mEdgeVarArray[edge - 1];
}

END_NAMESPACE_YM_NLINK
