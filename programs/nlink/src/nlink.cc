
/// @file nlink.cc
/// @brief nlink の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "nlink_nsdef.h"
#include "NlProblem.h"
#include "NlSolution.h"
#include "NlSolver1.h"
#include "NlSolver2.h"
#include "NlSolverGs.h"


BEGIN_NAMESPACE_YM_NLINK

void
nlink(const string& filename,
      ymuint method)
{
  NlProblem problem = read_problem(filename);

  print_problem(cout, problem);

  NlSolution solution;

  if ( method == 0 ) {
    NlSolverGs solver;
    solver.solve(problem, solution);
  }
  else if ( method == 1 ) {
    NlSolver1 solver;
    solver.solve(problem, solution);
  }
  else if ( method == 2 ) {
    NlSolver2 solver;
    solver.solve(problem, solution);
  }

  print_solution(cout, solution);
}

END_NAMESPACE_YM_NLINK


int
main(int argc,
     const char** argv)
{
  using nsYm::nsNlink::nlink;
  using namespace std;

  ymuint method = 0;
  int base = 1;
  for (int i = 1; i < argc; ++ i) {
    if ( strcmp(argv[i], "-1") == 0 ) {
      method = 1;
      base = 2;
    }
    else if ( strcmp(argv[i], "-2") == 0 ) {
      method = 2;
      base = 2;
    }
    else if ( strcmp(argv[i], "-gs") == 0 ) {
      method = 0;
      base = 2;
    }
    else {
      break;
    }
  }

  for (int i = base; i < argc; ++ i) {
    nlink(argv[i], method);
  }

  return 0;
}
