
/// @file NlSolver.cc
/// @brief NlSolver の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlSolver.h"
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

// @brief 問題を解く
void
NlSolver::solve(const NlProblem& problem)
{
  SatSolver solver(string(), string(), NULL);

  ymuint w = problem.width();
  ymuint h = problem.height();
  ymuint n = problem.elem_num();

  vector<ymuint> start_mark(w * h, 0);
  vector<VarId> var_array(w * h * n);
  for (ymuint x = 0; x < w; ++ x) {
    for (ymuint y = 0; y < h; ++ y) {
      for (ymuint k = 0; k < n; ++ k) {
	VarId var = solver.new_var();
	var_array[(x * h + y) * n + k] = var;
      }
    }
  }

  // 排他制約を作る．
  for (ymuint x = 0; x < w; ++ x) {
    for (ymuint y = 0; y < h; ++ y) {
      ymuint base = (x * h + y) * n;
      for (ymuint k1 = 0; k1 < n; ++ k1) {
	for (ymuint k2 = k1 + 1; K2 < n; ++ k2) {
	  VarId v1 = var_array[base + k1];
	  VarId v2 = var_array[base + k2];
	  Literal lit1(v1);
	  Literal lit2(v2);
	  solver.add_clause(~lit1, ~lit2);
	}
      }
    }
  }

  // 始点に印をつける．
  for (ymuint k = 0; k < n; ++ k) {
    NlConnection con = problem.connection(k);
    NlPoint start_point = con.start_point();
    NlPoint end_point = con.end_point();
    ymuint x1 = start_point.x();
    ymuint y1 = start_point.y();
    ymuint x2 = end_point.x();
    ymuint y2 = end_point.y();
    VarId v1 = var_array[(x1 * h + y1) * n + k];
    solver.add_clause(Literal(v1));
    start_mark[x1 * h + y1] = k + 1;
  }

  // 始点以外のセルの条件を作る．
  ymuint dx = h * n;
  ymuint dy = n;
  for (ymuint x = 0; x < w; ++ x) {
    for (ymuint y = 0; y < h; ++ y) {
      if ( start_mark[x * h + y] > 0 ) {
	// 始点は無条件で値が決まっている．
	continue;
      }
      ymuint base = (x * h + y) * n;
      for (ymuint k = 0; k < n; ++ k) {
	ymuint base0 = base + k;
	VarId v0 = var_array[base0];
	Literal lit0(v0);
	if ( x > 0 ) {
	  // 左がある．
	  VarId v_l = var_array[base0 - dx];
	  Literal lit_l(v_l);
	  if ( x < w - 1 ) {
	    // 右がある．
	    VarId v_r = var_array[base0 + dx];
	    Literal lit_r(v_r);
	    if ( y > 0 ) {
	      // 上がある．
	      VarId v_u = var_array[base0 - dy];
	      Literal lit_u(v_u);
	      if ( y < h - 1 ) {
		// 下がある．
		VarId v_d = var_array[base0 + dy];
		Literal lit_d(v_d);
		solver.add_clause(~lit0, lit_l, lit_r, lit_u, lit_d);
	      }
	      else {
		// 下がない
		solver.add_clause(~lit0, lit_l, lit_r, lit_u);
	      }
	    }
	    else {
	      // 上がないので必ず下がある．
	      VarId v_d = var_array[base0 + dy];
	      Literal lit_d(v_d);
	      solver.add_clause(~lit0, lit_l, lit_r, lit_d);
	    }
	  }
	  else {
	    // 右がない．
	    if ( y > 0 ) {
	      // 上がある．
	      VarId v_u = var_array[base0 - dy];
	      Literal lit_u(v_u);
	      if ( y < h - 1 ) {
		// 下がある．
		VarId v_d = var_array[base0 + dy];
		Literal lit_d(v_d);
		solver.add_clause(~lit0, lit_l, lit_u, lit_d);
	      }
	      else {
		// 下がない
		solver.add_clause(~lit0, lit_l, lit_u);
	      }
	    }
	    else {
	      // 上がないので必ず下がある．
	      VarId v_d = var_array[base0 + dy];
	      Literal lit_d(v_d);
	      solver.add_clause(~lit0, lit_l, lit_d);
	    }
	  }
	}
	else {
	  // 左がないので必ず右がある．
	  VarId v_r = var_array[base0 + dx];
	  Literal lit_r(v_r);
	  if ( y > 0 ) {
	    // 上がある．
	    VarId v_u = var_array[base0 - dy];
	    Literal lit_u(v_u);
	    if ( y < h - 1 ) {
	      // 下がある．
	      VarId v_d = var_array[base0 + dy];
	      Literal lit_d(v_d);
	      solver.add_clause(~lit0, lit_r, lit_u, lit_d);
	    }
	    else {
	      // 下がない
	      solver.add_clause(~lit0, lit_r, lit_u);
	    }
	  }
	  else {
	    // 上がないので必ず下がある．
	    VarId v_d = var_array[base0 + dy];
	    Literal lit_d(v_d);
	    solver.add_clause(~lit0, lit_r, lit_d);
	  }
	}
      }
    }
  }
}

END_NAMESPACE_YM_NLINK
