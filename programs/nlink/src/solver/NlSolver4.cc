
/// @file NlSolver4.cc
/// @brief NlSolver4 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlSolver4.h"
#include "NlSolution.h"
#include "NlGraph.h"
#include "NlNode.h"
#include "NlEdge.h"
#include "NpGraph2.h"

#include "YmLogic/SatSolver.h"
#include "YmLogic/SatMsgHandlerImpl1.h"
#include "YmLogic/SatStats.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
// クラス NlSolver4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] sat_type SATソルバ名
NlSolver4::NlSolver4(const string& sat_type) :
  mSatType(sat_type)
{
}

// @brief デストラクタ
NlSolver4::~NlSolver4()
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
NlSolver4::solve(const NlGraph& graph,
		 bool verbose,
		 NlSolution& solution)
{
  solution.init(graph);

  SatSolver solver(mSatType, string(), NULL);

  mNum = graph.num();
  ymuint mod = 5;
  mGroupNum = (mNum + 2) / mod;
  mGroupMap.resize(mNum);
  for (ymuint i = 0; i < mNum; ++ i) {
    mGroupMap[i] = i % mod;
  }

  make_base_cnf(solver, graph);

  NpGraph2 np_graph(solver, graph, mGroupNum, mGroupMap, mVarArray);

  if ( verbose ) {
    SatMsgHandler* msg_handler = new SatMsgHandlerImpl1(cout);
    solver.reg_msg_handler(msg_handler);
  }

  cout << "# of variables: " << solver.variable_num() << endl
       << "# of clauses:   " << solver.clause_num() << endl
       << "# of literals:  " << solver.literal_num() << endl;

  mNodeArray.clear();
  mNodeArray.resize(graph.max_node_id(), 0);

  vector<Bool3> model;
  Bool3 stat = solver.solve(model);
  switch ( stat ) {
  case kB3True:
    for (ymuint i = 0; i < mNum; ++ i) {
      const NlNode* node1 = graph.start_node(i);
      const NlNode* node2 = graph.end_node(i);
      const NlNode* end_node = search_path(node1, model);
      if ( end_node == node2 ) {
	//cout << " route check #" << (i + 1) << " OK" << endl;
      }
      else {
	cout << " route check #" << (i + 1) << " NG" << endl;
	cout << "(" << node1->x() << ", " << node1->y() << ") - ("
	     << node2->x() << ", " << node2->y() << ")" << endl;
      }
    }
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
NlSolver4::make_base_cnf(SatSolver& solver,
			 const NlGraph& graph)
{
  ymuint width = graph.width();
  ymuint height = graph.height();

  ymuint max_edge_id = graph.max_edge_id();
  ymuint max_node_id = graph.max_node_id();

  ASSERT_COND( mGroupNum == 1 );
  // 枝の変数(枝数 x グループ数)を作る．
  mVarArray.clear();
  mVarArray.resize(max_edge_id * mGroupNum);
  for (ymuint edge_id = 0; edge_id < max_edge_id; ++ edge_id) {
    for (ymuint k = 0; k < mGroupNum; ++ k) {
      VarId var = solver.new_var();
      mVarArray[edge_id * mGroupNum + k] = var;
    }
  }

  // 同じ枝上の変数の排他制約を作る．
  // 上の for ループとマージできるけど一応，わかりやすさ優先で．
  for (ymuint edge_id = 0; edge_id < max_edge_id; ++ edge_id) {
    vector<Literal> tmp_lits(mGroupNum);
    for (ymuint k = 0; k < mGroupNum; ++ k) {
      VarId var = mVarArray[edge_id * mGroupNum + k];
      tmp_lits[k] = Literal(var);
    }
    // 同時に2つ以上の変数が1にならないようにする．
    for (ymuint k1 = 0; k1 < mGroupNum; ++ k1) {
      Literal lit1 = tmp_lits[k1];
      for (ymuint k2 = k1 + 1; k2 < mGroupNum; ++ k2) {
	Literal lit2 = tmp_lits[k2];
	solver.add_clause(~lit1, ~lit2);
      }
    }
  }

  // 同じノード上で異なる線分が交わらないための条件を作る．
  for (ymuint id = 0; id < max_node_id; ++ id) {
    const NlNode* node = graph.node(id);
    const vector<const NlEdge*>& edge_list = node->edge_list();
    ymuint ne = edge_list.size();
    for (ymuint i1 = 0; i1 < ne; ++ i1) {
      const NlEdge* edge1 = edge_list[i1];
      for (ymuint i2 = 0; i2 < ne; ++ i2) {
	if ( i1 == i2 ) {
	  continue;
	}
	const NlEdge* edge2 = edge_list[i2];
	for (ymuint k1 = 0; k1 < mGroupNum; ++ k1) {
	  VarId var1 = edge_var(edge1, k1);
	  Literal lit1(var1);
	  for (ymuint k2 = k1 + 1; k2 < mGroupNum; ++ k2) {
	    VarId var2 = edge_var(edge2, k2);
	    Literal lit2(var2);
	    solver.add_clause(~lit1, ~lit2);
	  }
	}
      }
    }
  }

  // 枝が径路となる条件を作る．
  for (ymuint id = 0; id < max_node_id; ++ id) {
    const NlNode* node = graph.node(id);
    const vector<const NlEdge*>& edge_list = node->edge_list();
    ymuint ne = edge_list.size();
    for (ymuint k = 0; k < mGroupNum; ++ k) {
      vector<Literal> lit_list(ne);
      for (ymuint i = 0; i < ne; ++ i) {
	const NlEdge* edge = edge_list[i];
	VarId var = edge_var(edge, k);
	lit_list[i] = Literal(var);
      }

      ymuint term_id = node->terminal_id();
      if ( term_id > 0 ) {
	-- term_id;
	if ( mGroupMap[term_id] == k ) {
	  // 端点の場合
	  // 必ずただ1つの枝が選ばれる．
	  one_hot(solver, lit_list);
	}
	else {
	  // 選ばれない．
	  // 実はこの条件はなくても他の制約から導かれる．
	  // けど unit clause はやって損はないので．
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

// @brief 経路を求める．
// @param[in] node ノード
// @param[in] model SATの解
const NlNode*
NlSolver4::search_path(const NlNode* node,
		       const vector<Bool3>& model)
{
  ymuint idx = node->terminal_id();
  ASSERT_COND( idx > 0 );
  -- idx;
  ymuint grp = mGroupMap[idx];

  // SAT の制約で端子から端子までの経路は一意に決まるので
  // それをたどる．
  const NlEdge* from_edge = NULL;
  for ( ; ; ) {
    // ノードに番号を書いておく．
    mNodeArray[node->id()] = idx + 1;
    const vector<const NlEdge*>& edge_list = node->edge_list();
    const NlNode* next_node = NULL;
    const NlEdge* next_edge = NULL;
    for (ymuint i = 0; i < edge_list.size(); ++ i) {
      const NlEdge* edge = edge_list[i];
      if ( from_edge == edge ) {
	// 今来た枝は辿らない．
	continue;
      }

      VarId var = edge_var(edge, grp);
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
NlSolver4::setup_solution(const NlGraph& graph,
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
      ymuint val = mNodeArray[x * height + y];
      solution.set(x, y, val);
    }
  }
}

// @brief 枝の変数番号を得る．
// @param[in] edge 枝
// @param[in] idx 線分番号
VarId
NlSolver4::edge_var(const NlEdge* edge,
		    ymuint idx) const
{
  ASSERT_COND( edge != NULL );

  return mVarArray[edge->id() * mGroupNum + idx];
}

END_NAMESPACE_YM_NLINK
