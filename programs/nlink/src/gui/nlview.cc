
/// @file nlview.cc
/// @brief nlview の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlView.h"
#include "NlProblem.h"
#include "NlSolution.h"
#include "NlSolver2.h"
#include <QApplication>


int
main(int argc,
     char** argv)
{
  using namespace nsYm::nsNlink;
  using namespace std;

  QApplication app(argc, argv);

  char* filename = argv[1];
  NlProblem problem = read_problem(filename);

  NlSolver2 solver;
  NlSolution solution;

  solver.solve(problem, solution);

  print_solution(cout, solution);

  NlView* view = new NlView(solution);

  view->show();

  return app.exec();
}
