
/// @file NlSolver.cc
/// @brief NlSolver の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlSolver.h"
#include "NlProblem.h"
#include "NlSolution.h"
#include "MazeRouter.h"
#include "YmLogic/SatSolver.h"


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

END_NONAMESPACE


// @brief 問題を解く
// @param[in] problem 問題
// @param[out] solution 解
void
NlSolver::solve(const NlProblem& problem,
		NlSolution& solution)
{
  SatSolver solver("minisat2", string(), NULL);
  //SatSolver solver("satrec", string(), &cout);

  ymuint w = problem.width();
  ymuint h = problem.height();
  ymuint n = problem.elem_num();

  ASSERT_COND( w > 0 );
  ASSERT_COND( h > 0 );
  ASSERT_COND( n > 0 );

  init(solver, w, h, n);

  // 始点と終点に印をつける．
  for (ymuint k = 0; k < n; ++ k) {
    NlConnection con = problem.connection(k);
    {
      NlPoint start_point = con.start_point();
      ymuint x1 = start_point.x();
      ymuint y1 = start_point.y();
      Node* node1 = _node(x1, y1);
      node1->mEndMark = k + 1;
      Literal lit1(node1->mVarArray[k]);
      solver.add_clause(lit1);
    }
    {
      NlPoint end_point = con.end_point();
      ymuint x2 = end_point.x();
      ymuint y2 = end_point.y();
      Node* node2 = _node(x2, y2);
      node2->mEndMark = k + 1;
      Literal lit2(node2->mVarArray[k]);
      solver.add_clause(lit2);
    }
  }

  // one-hot 制約を作る．
  // 一つのノードでは高々1つの変数しか1にならない．
  for (ymuint x = 0; x < w; ++ x) {
    for (ymuint y = 0; y < h; ++ y) {
      Node* node = _node(x, y);
      for (ymuint k1 = 0; k1 < n; ++ k1) {
	Literal lit1(node->mVarArray[k1]);
	for (ymuint k2 = k1 + 1; k2 < n; ++ k2) {
	  Literal lit2(node->mVarArray[k2]);
	  solver.add_clause(~lit1, ~lit2);
	}
      }
    }
  }

  // 枝の条件を作る．
  for (ymuint x = 0; x < w; ++ x) {
    for (ymuint y = 0; y < h; ++ y) {
      Node* node = _node(x, y);

      const vector<Edge*>& edge_list = node->mEdgeList;
      ymuint ne = edge_list.size();
      vector<Literal> lit_list(ne);
      for (ymuint i = 0; i < ne; ++ i) {
	lit_list[i] = Literal(edge_list[i]->mVar);
      }

      if ( node->mEndMark > 0 ) {
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
  }

  // 横方向の隣接関係の条件を作る．
  for (ymuint x = 0; x < w - 1; ++ x) {
    for (ymuint y = 0; y < h; ++ y) {
      Edge* edge = right_edge(x, y);
      Literal e_lit(edge->mVar);
      Node* node1 = _node(x, y);
      Node* node2 = _node(x + 1, y);
      for (ymuint k = 0; k < n; ++ k) {
	Literal v_lit1(node1->mVarArray[k]);
	Literal v_lit2(node2->mVarArray[k]);
	solver.add_clause(~e_lit,  v_lit1, ~v_lit2);
	solver.add_clause(~e_lit, ~v_lit1,  v_lit2);
      }
    }
  }

  // 縦方向の隣接関係の条件を作る．
  for (ymuint y = 0; y < h - 1; ++ y) {
    for (ymuint x = 0; x < w; ++ x) {
      Edge* edge = lower_edge(x, y);
      Literal e_lit(edge->mVar);
      Node* node1 = _node(x, y);
      Node* node2 = _node(x, y + 1);
      for (ymuint k = 0; k < n; ++ k) {
	Literal v_lit1(node1->mVarArray[k]);
	Literal v_lit2(node2->mVarArray[k]);
	solver.add_clause(~e_lit,  v_lit1, ~v_lit2);
	solver.add_clause(~e_lit, ~v_lit1,  v_lit2);
      }
    }
  }

  solution.init(problem);

  vector<Bool3> model;
  Bool3 stat = solver.solve(model);
  switch ( stat ) {
  case kB3True:
    cout << "SAT" << endl;
    setup_solution(model, solution);
    break;

  case kB3False:
    cout << "UNSAT" << endl;
    break;

  case kB3X:
    cout << "ABORT" << endl;
    break;
  }
}

// @brief 内容をクリアする．
void
NlSolver::clear()
{
  for (ymuint i = 0; i < mHarray.size(); ++ i) {
    delete mHarray[i];
  }
  mHarray.clear();

  for (ymuint i = 0; i < mVarray.size(); ++ i) {
    delete mVarray[i];
  }
  mVarray.clear();

  for (ymuint i = 0; i < mNodeArray.size(); ++ i) {
    delete mNodeArray[i];
  }
  mNodeArray.clear();
}

// @brief 内容を初期化する．
// @param[in] solver SAT ソルバ
// @param[in] width 幅
// @param[in] height 高さ
// @param[in] num 線分数
void
NlSolver::init(SatSolver& solver,
	       ymuint width,
	       ymuint height,
	       ymuint num)
{
  mWidth = width;
  mHeight = height;
  mNum = num;

  // 横の辺を作る．
  ymuint nh = (width - 1) * height;
  mHarray.resize(nh);
  for (ymuint i = 0; i < nh; ++ i) {
    Edge* edge = new Edge;
    mHarray[i] = edge;
    VarId var = solver.new_var();
    edge->mVar = var;
  }

  // 縦の辺を作る．
  ymuint nv = (height - 1) * width;
  mVarray.resize(nv);
  for (ymuint i = 0; i < nv; ++ i) {
    Edge* edge = new Edge;
    mVarray[i] = edge;
    VarId var = solver.new_var();
    edge->mVar = var;
  }

  // ノードを作る．
  ymuint nn = width * height;
  mNodeArray.resize(nn);
  for (ymuint i = 0; i < nn; ++ i) {
    Node* node = new Node;
    node->mEndMark = 0;
    mNodeArray[i] = node;
    node->mVarArray.resize(num);
    for (ymuint j = 0; j < num; ++ j) {
      VarId var = solver.new_var();
      node->mVarArray[j] = var;
    }
  }

  // 辺の隣接関係を作る．
  for (ymuint x = 0; x < mWidth; ++ x) {
    for (ymuint y = 0; y < mHeight; ++ y) {
      Node* node = mNodeArray[x * mHeight + y];
      node->mEdgeList.reserve(4);
      if ( x > 0 ) {
	// 左に辺がある．
	Edge* edge = left_edge(x, y);
	node->mEdgeList.push_back(edge);
      }
      if ( x < mWidth - 1 ) {
	// 右に辺がある．
	Edge* edge = right_edge(x, y);
	node->mEdgeList.push_back(edge);
      }
      if ( y > 0 ) {
	// 上に辺がある．
	Edge* edge = upper_edge(x, y);
	node->mEdgeList.push_back(edge);
      }
      if ( y < mHeight - 1 ) {
	// 下に辺がある．
	Edge* edge = lower_edge(x, y);
	node->mEdgeList.push_back(edge);
      }
      ymuint n = node->mEdgeList.size();
      for (ymuint i = 0; i < n; ++ i) {
	Edge* edge0 = node->mEdgeList[i];
	edge0->mAdjList.reserve(n - 1);
	for (ymuint j = 0; j < n; ++ j) {
	  if ( j == i ) {
	    continue;
	  }
	  Edge* edge = node->mEdgeList[j];
	  edge0->mAdjList.push_back(edge);
	}
      }
    }
  }
}

// @brief ノードを得る．
NlSolver::Node*
NlSolver::_node(ymuint x,
		ymuint y)
{
  ASSERT_COND( x >= 0 );
  ASSERT_COND( x < mWidth );
  ASSERT_COND( y >= 0 );
  ASSERT_COND( y < mHeight );

  return mNodeArray[x * mHeight + y];
}

// @brief 左の辺を得る．
// @param[in] x, y 辺の右端の座標
NlSolver::Edge*
NlSolver::left_edge(ymuint x,
		    ymuint y)
{
  ASSERT_COND( x > 0 );
  ASSERT_COND( x < mWidth );
  ASSERT_COND( y >= 0 );
  ASSERT_COND( y < mHeight );

  return mHarray[(x - 1) * mHeight + y];
}

// @brief 右の辺を得る．
// @param[in] x, y 辺の左端の座標
NlSolver::Edge*
NlSolver::right_edge(ymuint x,
		     ymuint y)
{
  ASSERT_COND( x >= 0 );
  ASSERT_COND( x < mWidth - 1 );
  ASSERT_COND( y >= 0 );
  ASSERT_COND( y < mHeight );

  return mHarray[x * mHeight + y];
}

// @brief 上の辺を得る．
// @param[in] x, y 辺の下端の座標
NlSolver::Edge*
NlSolver::upper_edge(ymuint x,
		     ymuint y)
{
  ASSERT_COND( x >= 0 );
  ASSERT_COND( x < mWidth );
  ASSERT_COND( y > 0 );
  ASSERT_COND( y < mHeight );

  return mVarray[(y - 1) * mWidth + x];
}

// @brief 下の辺を得る．
// @param[in] x, y 辺の上端の座標
NlSolver::Edge*
NlSolver::lower_edge(ymuint x,
		     ymuint y)
{
  ASSERT_COND( x >= 0 );
  ASSERT_COND( x < mWidth );
  ASSERT_COND( y >= 0 );
  ASSERT_COND( y < mHeight - 1 );

  return mVarray[y * mWidth + x];
}

// @brief 解を出力する．
// @param[in] model SATの解
// @param[in] solution 解
void
NlSolver::setup_solution(const vector<Bool3>& model,
			 NlSolution& solution)
{
  for (ymuint y = 0; y < mHeight; ++ y) {
    for (ymuint x = 0; x < mWidth; ++ x) {
      if ( solution.get(x, y) < 0 ) {
	continue;
      }
      Node* node = _node(x, y);
      for (ymuint k = 0; k < mNum; ++ k) {
	VarId var = node->mVarArray[k];
	if ( model[var.val()] == kB3True ) {
	  solution.set(x, y, k + 1);
	  break;
	}
      }
    }
  }
}

// @brief 解を出力する．
// @param[in] s 出力先のストリーム
// @param[in] model SATの解
void
NlSolver::print_solution(ostream& s,
			 const vector<Bool3>& model)
{
  for (ymuint y = 0; y < mHeight; ++ y) {
    for (ymuint x = 0; x < mWidth; ++ x) {
      Node* node = _node(x, y);
      bool found = false;
      for (ymuint k = 0; k < mNum; ++ k) {
	VarId var = node->mVarArray[k];
	if ( model[var.val()] == kB3True ) {
	  cout << " " << setw(2) << (k + 1);
	  found = true;
	  break;
	}
      }
      if ( !found ) {
	cout << "   ";
      }
    }
    cout << endl;
  }
}

END_NAMESPACE_YM_NLINK
