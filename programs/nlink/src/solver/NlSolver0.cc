
/// @file NlSolver0.cc
/// @brief NlSolver0 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlSolver0.h"
#include "NlSolution.h"
#include "NlGraph.h"
#include "NlNode.h"
#include "NlEdge.h"
#include "NpGraph.h"
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
// @param[in] graph 問題のグラフ
// @param[in] verbose verbose フラグ
// @param[out] solution 解
void
NlSolver0::solve(const NlGraph& graph,
		 bool verbose,
		 NlSolution& solution)
{
  SatSolver solver(mSatType, string(), NULL);

  mWidth = graph.width();
  mHeight = graph.height();
  mNum = graph.num();

  make_base_cnf(solver, graph);

  NpGraph np_graph(solver, graph, mEdgeVarArray);

  solution.init(graph);

  if ( verbose ) {
    SatMsgHandler* msg_handler = new SatMsgHandlerImpl1(cout);
    solver.reg_msg_handler(msg_handler);
  }

  cout << "# of variables: " << solver.variable_num() << endl
       << "# of clauses:   " << solver.clause_num() << endl
       << "# of literals:  " << solver.literal_num() << endl;


  mNodeArray.clear();
  mNodeArray.resize(graph.max_node_id(), 0);

  for ( ; ; ) {
    vector<Bool3> model;
    Bool3 stat = solver.solve(model);
    if ( stat == kB3X ) {
      cerr << "ABORT" << endl;
      return;
    }
    if ( stat == kB3False ) {
      cerr << "UNSAT" << endl;
      return;
    }

    bool success = true;
    for (ymuint i = 0; i < mNum; ++ i) {
      const NlNode* node1 = graph.start_node(i);
      const NlNode* node2 = graph.end_node(i);
      const NlNode* end_node = search_path(node1, model);
      if ( end_node == node2 ) {
	//cout << " route check #" << (i + 1) << " OK" << endl;
      }
      else {
	cout << " route check #" << (i + 1) << " NG" << endl;
	np_graph.add_path_constr(solver, model, node1->id(), end_node->id());
	success = false;
      }
    }
    if ( success ) {
      setup_solution(graph, model, solution);
      break;
    }
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
  for (ymuint edge_id = 0; edge_id < max_edge_id; ++ edge_id) {
    VarId var = solver.new_var();
    mEdgeVarArray[edge_id] = var;
  }

  // 枝の条件を作る．
  for (ymuint node_id = 0; node_id < max_node_id; ++ node_id) {
    const NlNode* node = graph.node(node_id);
    const vector<const NlEdge*>& edge_list = node->edge_list();
    ymuint ne = edge_list.size();
    // ノードに隣接する枝のリテラルのリストを作る．
    vector<Literal> lit_list(ne);
    for (ymuint i = 0; i < ne; ++ i) {
      const NlEdge* edge = edge_list[i];
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

// @brief 経路を求める．
// @param[in] node ノード
// @param[in] model SATの解
const NlNode*
NlSolver0::search_path(const NlNode* node,
		       const vector<Bool3>& model)
{
  ymuint idx = node->terminal_id();

  // SAT の制約で端子から端子までの経路は一意に決まるので
  // それをたどる．
  const NlEdge* from_edge = NULL;
  for ( ; ; ) {
    // ノードに番号を書いておく．
    mNodeArray[node->id()] = idx;

    const vector<const NlEdge*>& edge_list = node->edge_list();
    const NlNode* next_node = NULL;
    const NlEdge* next_edge = NULL;
    for (ymuint i = 0; i < edge_list.size(); ++ i) {
      const NlEdge* edge = edge_list[i];
      if ( from_edge == edge ) {
	// 今来た枝は辿らない．
	continue;
      }

      VarId var = edge_var(edge);
      if ( model[var.val()] != kB3True ) {
	// 選ばれていない枝も辿らない．
	continue;
      }

      // edge の反対側のノード
      next_node = edge->alt_node(node);
      next_edge = edge;
      break;
    }
    if ( next_node == NULL ) {
      // たぶん，node は終端
      return node;
    }
    node = next_node;
    from_edge = next_edge;
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
  for (ymuint y = 0; y < mHeight; ++ y) {
    for (ymuint x = 0; x < mWidth; ++ x) {
      if ( solution.get(x, y) < 0 ) {
	continue;
      }
      ymuint val = mNodeArray[x * mHeight + y];
      solution.set(x, y, val);
    }
  }
}

// @brief 枝の変数番号を得る．
// @param[in] edge 枝
VarId
NlSolver0::edge_var(const NlEdge* edge)
{
  ASSERT_COND( edge != NULL );

  return mEdgeVarArray[edge->id()];
}

END_NAMESPACE_YM_NLINK
