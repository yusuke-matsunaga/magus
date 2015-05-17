
/// @file NlSolver2.cc
/// @brief NlSolver2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlSolver2.h"
#include "NlProblem.h"
#include "NlSolution.h"
#include "YmLogic/SatSolver.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
// クラス NlSolver2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NlSolver2::NlSolver2()
{
}

// @brief デストラクタ
NlSolver2::~NlSolver2()
{
  clear();
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
NlSolver2::solve(const NlProblem& problem,
		 NlSolution& solution)
{
  SatSolver solver("minisat2", string(), NULL);

  ymuint w = problem.width();
  ymuint h = problem.height();
  ymuint n = problem.elem_num();

  ASSERT_COND( w > 0 );
  ASSERT_COND( h > 0 );
  ASSERT_COND( n > 0 );

  init(solver, w, h, n);

  // 横の辺に対する one-hot 制約を作る．
  for (ymuint x = 1; x < w; ++ x) {
    for (ymuint y = 0; y < h; ++ y) {
      Edge* edge = left_edge(x, y);
      vector<Literal> tmp_lits(n);
      for (ymuint k = 0; k < n; ++ k) {
	VarId var = edge->mVarArray[k];
	Literal lit = Literal(var);
	tmp_lits[k] = lit;
      }
      for (ymuint k1 = 0; k1 < n; ++ k1) {
	Literal lit1 = tmp_lits[k1];
	for (ymuint k2 = k1 + 1; k2 < n; ++ k2) {
	  Literal lit2 = tmp_lits[k2];
	  solver.add_clause(~lit1, ~lit2);
	}
      }
    }
  }

  // 縦の辺に対する one-hot 制約を作る．
  for (ymuint y = 1; y < h; ++ y) {
    for (ymuint x = 0; x < w; ++ x) {
      Edge* edge = upper_edge(x, y);
      vector<Literal> tmp_lits(n);
      for (ymuint k = 0; k < n; ++ k) {
	VarId var = edge->mVarArray[k];
	Literal lit = Literal(var);
	tmp_lits[k] = lit;
      }
      for (ymuint k1 = 0; k1 < n; ++ k1) {
	Literal lit1 = tmp_lits[k1];
	for (ymuint k2 = k1 + 1; k2 < n; ++ k2) {
	  Literal lit2 = tmp_lits[k2];
	  solver.add_clause(~lit1, ~lit2);
	}
      }
    }
  }

  // 始点と終点に印をつける．
  for (ymuint k = 0; k < n; ++ k) {
    NlConnection con = problem.connection(k);
    {
      NlPoint start_point = con.start_point();
      ymuint x1 = start_point.x();
      ymuint y1 = start_point.y();
      Node* node1 = _node(x1, y1);
      node1->mEndMark = k + 1;
    }
    {
      NlPoint end_point = con.end_point();
      ymuint x2 = end_point.x();
      ymuint y2 = end_point.y();
      Node* node2 = _node(x2, y2);
      node2->mEndMark = k + 1;
    }
  }

  // 枝の条件を作る．
  for (ymuint x = 0; x < w; ++ x) {
    for (ymuint y = 0; y < h; ++ y) {
      Node* node = _node(x, y);
      const vector<Edge*>& adj_list = node->mEdgeList;
      ymuint na = adj_list.size();
      for (ymuint k = 0; k < n; ++ k) {
	// (x, y) のノードに隣接する枝のリストを作る．
	vector<Literal> edge_list;
	edge_list.reserve(na);
	for (ymuint i = 0; i < na; ++ i) {
	  Edge* edge = adj_list[i];
	  VarId var = edge->mVarArray[k];
	  Literal lit(var);
	  edge_list.push_back(lit);
	}

	if ( node->mEndMark > 0 ) {
	  if ( node->mEndMark == k + 1 ) {
	    // 端点の場合
	    // 必ずただ1つの枝が選ばれる．
	    switch ( edge_list.size() ) {
	    case 2:
	      one_hot2(solver, edge_list[0], edge_list[1]);
	      break;

	    case 3:
	      one_hot3(solver, edge_list[0], edge_list[1], edge_list[2]);
	      break;

	    case 4:
	      one_hot4(solver, edge_list[0], edge_list[1], edge_list[2], edge_list[3]);
	      break;

	    default:
	      ASSERT_NOT_REACHED;
	    }
	  }
	  else {
	    // 選ばれない．
	    switch ( edge_list.size() ) {
	    case 2:
	      solver.add_clause(~edge_list[0]);
	      solver.add_clause(~edge_list[1]);
	      break;

	    case 3:
	      solver.add_clause(~edge_list[0]);
	      solver.add_clause(~edge_list[1]);
	      solver.add_clause(~edge_list[2]);
	      break;

	    case 4:
	      solver.add_clause(~edge_list[0]);
	      solver.add_clause(~edge_list[1]);
	      solver.add_clause(~edge_list[2]);
	      solver.add_clause(~edge_list[3]);
	      break;

	    default:
	      ASSERT_NOT_REACHED;
	    }
	  }
	}
	else {
	  // そうでない場合
	  // 0個か2個の枝が選ばれる．
	  switch ( edge_list.size() ) {
	  case 2:
	    zero_two_hot2(solver, edge_list[0], edge_list[1]);
	    break;

	  case 3:
	    zero_two_hot3(solver, edge_list[0], edge_list[1], edge_list[2]);
	    break;

	  case 4:
	    zero_two_hot4(solver, edge_list[0], edge_list[1], edge_list[2], edge_list[3]);
	    break;

	  default:
	    ASSERT_NOT_REACHED;
	  }
	}
      }

      for (ymuint i1 = 0; i1 < na; ++ i1) {
	Edge* edge1 = adj_list[i1];
	for (ymuint i2 = 0; i2 < na; ++ i2) {
	  Edge* edge2 = adj_list[i2];
	  for (ymuint k1 = 0; k1 < n; ++ k1) {
	    VarId var1 = edge1->mVarArray[k1];
	    Literal lit1(var1);
	    for (ymuint k2 = k1 + 1; k2 < n; ++ k2) {
	      VarId var2 = edge2->mVarArray[k2];
	      Literal lit2(var2);
	      solver.add_clause(~lit1, ~lit2);
	    }
	  }
	}
      }
    }
  }

  solution.init(problem);

  vector<Bool3> model;
  Bool3 stat = solver.solve(model);
  switch ( stat ) {
  case kB3True:
    setup_solution(model, solution);
    break;

  case kB3False:
    cerr << "UNSAT" << endl;
    break;

  case kB3X:
    cerr << "ABORT" << endl;
    break;
  }
}

// @brief 内容をクリアする．
void
NlSolver2::clear()
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
NlSolver2::init(SatSolver& solver,
		ymuint width,
		ymuint height,
		ymuint num)
{
  clear();

  mWidth = width;
  mHeight = height;
  mNum = num;

  // 横の辺を作る．
  ymuint nh = (width - 1) * height;
  mHarray.resize(nh);
  for (ymuint i = 0; i < nh; ++ i) {
    Edge* edge = new Edge;
    mHarray[i] = edge;
    edge->mVarArray.resize(num);
    for (ymuint j = 0; j < num; ++ j) {
      VarId var = solver.new_var();
      edge->mVarArray[j] = var;
    }
  }

  // 縦の辺を作る．
  ymuint nv = (height - 1) * width;
  mVarray.resize(nv);
  for (ymuint i = 0; i < nv; ++ i) {
    Edge* edge = new Edge;
    mVarray[i] = edge;
    edge->mVarArray.resize(num);
    for (ymuint j = 0; j < num; ++ j) {
      VarId var = solver.new_var();
      edge->mVarArray[j] = var;
    }
  }

  // ノードを作る．
  ymuint nn = width * height;
  mNodeArray.resize(nn);
  for (ymuint i = 0; i < nn; ++ i) {
    Node* node = new Node;
    mNodeArray[i] = node;
    node->mEndMark = 0;
  }

  // 辺の隣接関係を作る．
  for (ymuint x = 0; x < mWidth; ++ x) {
    for (ymuint y = 0; y < mHeight; ++ y) {
      Node* node = _node(x, y);
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
// @param[in] x, y 座標
NlSolver2::Node*
NlSolver2::_node(ymuint x,
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
NlSolver2::Edge*
NlSolver2::left_edge(ymuint x,
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
NlSolver2::Edge*
NlSolver2::right_edge(ymuint x,
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
NlSolver2::Edge*
NlSolver2::upper_edge(ymuint x,
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
NlSolver2::Edge*
NlSolver2::lower_edge(ymuint x,
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
NlSolver2::setup_solution(const vector<Bool3>& model,
			  NlSolution& solution)
{
  for (ymuint y = 0; y < mHeight; ++ y) {
    for (ymuint x = 0; x < mWidth; ++ x) {
      if ( solution.get(x, y) < 0 ) {
	continue;
      }
      Node* node = _node(x, y);
      const vector<Edge*>& edge_list = node->mEdgeList;
      bool found = false;
      for (ymuint i = 0; i < edge_list.size(); ++ i) {
	Edge* edge = edge_list[i];
	for (ymuint k = 0; k < mNum; ++ k) {
	  VarId var = edge->mVarArray[k];
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

// @brief 解を出力する．
// @param[in] s 出力先のストリーム
// @param[in] model SATの解
void
NlSolver2::print_solution(ostream& s,
			  const vector<Bool3>& model)
{
  for (ymuint y = 0; y < mHeight; ++ y) {
    for (ymuint x = 0; x < mWidth; ++ x) {
      Node* node = _node(x, y);
      const vector<Edge*>& edge_list = node->mEdgeList;
      bool found = false;
      for (ymuint i = 0; i < edge_list.size(); ++ i) {
	Edge* edge = edge_list[i];
	for (ymuint k = 0; k < mNum; ++ k) {
	  VarId var = edge->mVarArray[k];
	  if ( model[var.val()] == kB3True ) {
	    cout << " " << setw(2) << (k + 1);
	    found = true;
	    break;
	  }
	}
	if ( found ) {
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
