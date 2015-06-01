
/// @file nlview.cc
/// @brief nlview の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlView.h"
#include "NlProblem.h"
#include "NlSolution.h"
#include "NlSolverGs.h"
#include <QApplication>


BEGIN_NAMESPACE_YM_NLINK

int
nlview(int argc,
       char** argv)
{
  QApplication app(argc, argv);

  char* filename = argv[1];
  NlProblem problem = read_problem(filename);

  NlSolverGs solver;
  NlSolution solution;

  solver.solve(problem, false, solution);

  print_solution(cout, solution);

  NlView* view = new NlView(solution);

  view->show();

  return app.exec();
}

END_NAMESPACE_YM_NLINK

int
main(int argc,
     char** argv)
{
  return nsYm::nsNlink::nlview(argc, argv);
}
