
/// @file NlSolver.cc
/// @brief NlSolver の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlSolver.h"
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

void
print_solution(const NlProblem& problem,
	       const vector<Bool3>& model,
	       const vector<Literal>& h_array,
	       const vector<Literal>& v_array)
{
  ymuint w = problem.width();
  ymuint h = problem.height();
  ymuint n = problem.elem_num();

  for (ymuint y = 0; y < h; ++ y) {
    if ( y > 0 ) {
      for (ymuint x = 0; x < w; ++ x) {
	bool fill = false;
	for (ymuint k = 0; k < n; ++ k) {
	  Literal lit = v_array[((y - 1) * w + x) *n + k];
	  VarId var = lit.varid();
	  if ( model[var.val()] == kB3True ) {
	    cout << " " << setw(2) << (k + 1);
	    fill = true;
	    break;
	  }
	}
	if ( !fill ) {
	  cout << "   ";
	}
	cout << "   ";
      }
      cout << endl;
    }
    cout << "   ";
    for (ymuint x = 1; x < w; ++ x) {
      bool fill = false;
      for (ymuint k = 0; k < n; ++ k) {
	Literal lit = h_array[((x - 1) * h + y) * n + k];
	VarId var = lit.varid();
	if ( model[var.val()] == kB3True ) {
	  cout << " " << setw(2) << (k + 1);
	  fill = true;
	  break;
	}
      }
      if ( !fill ) {
	cout << "   ";
      }
      cout << "   ";
    }
    cout << endl;
  }
}

END_NONAMESPACE

// @brief 問題を解く
void
NlSolver::solve(const NlProblem& problem)
{
  //SatSolver solver("minisat2", string(), NULL);
  SatSolver solver("satrec", string(), &cout);

  ymuint w = problem.width();
  ymuint h = problem.height();
  ymuint n = problem.elem_num();

  ASSERT_COND( w > 0 );
  ASSERT_COND( h > 0 );
  ASSERT_COND( n > 0 );

  vector<ymuint> end_mark(w * h, 0);
  vector<Literal> h_array((w - 1) * h * n);
  vector<Literal> v_array(w * (h - 1) * n);

  // 横方向の枝の変数
  // 各位置に対して n 個用意する．
  for (ymuint x = 1; x < w; ++ x) {
    for (ymuint y = 0; y < h; ++ y) {
      vector<Literal> tmp_lits(n);
      for (ymuint k = 0; k < n; ++ k) {
	VarId var = solver.new_var();
	Literal lit = Literal(var);
	h_array[((x - 1) * h + y) * n + k] = lit;
	tmp_lits[k] = lit;
      }
      // one hot 制約
      for (ymuint k1 = 0; k1 < n; ++ k1) {
	Literal lit1 = tmp_lits[k1];
	for (ymuint k2 = k1 + 1; k2 < n; ++ k2) {
	  Literal lit2 = tmp_lits[k2];
	  solver.add_clause(~lit1, ~lit2);
	}
      }
      //solver.add_clause(tmp_lits);
    }
  }

  // 縦方向の枝の変数
  // 各位置に対して n 個用意する．
  for (ymuint y = 1; y < h; ++ y) {
    for (ymuint x = 0; x < w; ++ x) {
      vector<Literal> tmp_lits(n);
      for (ymuint k = 0; k < n; ++ k) {
	VarId var = solver.new_var();
	Literal lit = Literal(var);
	v_array[((y - 1) * w + x) *n + k] = lit;
	tmp_lits[k] = lit;
      }
      // one hot 制約
      for (ymuint k1 = 0; k1 < n; ++ k1) {
	Literal lit1 = tmp_lits[k1];
	for (ymuint k2 = k1 + 1; k2 < n; ++ k2) {
	  Literal lit2 = tmp_lits[k2];
	  solver.add_clause(~lit1, ~lit2);
	}
      }
      //solver.add_clause(tmp_lits);
    }
  }

  // 始点と終点に印をつける．
  for (ymuint k = 0; k < n; ++ k) {
    NlConnection con = problem.connection(k);
    {
      NlPoint start_point = con.start_point();
      ymuint x1 = start_point.x();
      ymuint y1 = start_point.y();
      end_mark[x1 * h + y1] = k + 1;
    }
    {
      NlPoint end_point = con.end_point();
      ymuint x2 = end_point.x();
      ymuint y2 = end_point.y();
      end_mark[x2 * h + y2] = k + 1;
    }
  }

  // 枝の条件を作る．
  for (ymuint x = 0; x < w; ++ x) {
    for (ymuint y = 0; y < h; ++ y) {
      vector<ymuint> adj_list;
      if ( x > 0 ) {
	// 左の枝を加える．
	ymuint index = ((x - 1) * h + y) * n;
	adj_list.push_back(index);
      }
      if ( x < w - 1 ) {
	// 右の枝を加える．
	ymuint index = (x * h + y) * n;
	adj_list.push_back(index);
      }
      if ( y > 0 ) {
	// 上の枝を加える．
	ymuint index = ((y - 1) * w + x) * n;
	adj_list.push_back(index);
      }
      if ( y < h - 1 ) {
	// 下の枝を加える．
	ymuint index = (y * w + x) * n;
	adj_list.push_back(index);
      }

      for (ymuint k = 0; k < n; ++ k) {
	// (x, y) のノードに隣接する枝のリストを作る．
	vector<Literal> edge_list;
	edge_list.reserve(adj_list.size());
	for (ymuint i = 0; i < adj_list.size(); ++ i) {
	  Literal lit = h_array[adj_list[i] + k];
	  edge_list.push_back(lit);
	}

	if ( end_mark[x * h + y] > 0 ) {
	  if ( end_mark[x * h + y] == k + 1 ) {
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
      // 排他条件
      for (ymuint k1 = 0; k1 < n; ++ k1) {
	for (ymuint k2 = k1 + 1; k2 < n; ++ k2) {
	  for (ymuint i1 = 0; i1 < adj_list.size(); ++ i1) {
	    for (ymuint i2 = i1 + 1; i2 < adj_list.size(); ++ i2) {
	      Literal lit1 =

	}
      }
    }
  }

#if 0
  MazeRouter mr(problem);

  // セパレータの制約を作る．
  for (ymuint i = 0; i < n; ++ i) {
    cout << "LINE#" << (i + 1) << endl;
    ymuint lmax = mr.labeling(i);
    for (ymuint l = 1; l < lmax; ++ l) {
      vector<NlPoint> point_list;
      mr.get_cell_list(l, point_list);
      cout << "  @" << l << " ";
      for (ymuint j = 0; j < point_list.size(); ++ j) {
	const NlPoint& point = point_list[j];
	cout << point;
      }
      cout << endl;
      vector<Literal> tmp_lits;
      tmp_lits.reserve(point_list.size());
      for (ymuint j = 0; j < point_list.size(); ++ j) {
	const NlPoint& point = point_list[j];
	ymuint index = ((point.x() * h) + point.y()) * n + i;
	Literal lit = node_array[index];
	tmp_lits.push_back(lit);
      }
      solver.add_clause(tmp_lits);
    }
  }
#endif

  vector<Bool3> model;
  Bool3 stat = solver.solve(model);
  switch ( stat ) {
  case kB3True:
    cout << "SAT" << endl;
    print_solution(problem, model, h_array, v_array);
    break;

  case kB3False:
    cout << "UNSAT" << endl;
    break;

  case kB3X:
    cout << "ABORT" << endl;
    break;
  }
}

END_NAMESPACE_YM_NLINK
