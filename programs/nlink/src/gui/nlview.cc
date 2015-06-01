
/// @file nlview.cc
/// @brief nlview の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlView.h"
#include "NlProblem.h"
#include "NlSolution.h"
#include <QApplication>


BEGIN_NAMESPACE_YM_NLINK

int
nlview(int argc,
       char** argv)
{
  QApplication app(argc, argv);

  char* solution_filename = NULL;
  ymuint base = 1;
  for (ymuint i = 1;i < argc; ++ i) {
    if ( argv[i][0] != '-' ) {
      base = i;
      break;
    }
    if ( strcmp(argv[i], "--solution") == 0 ) {
      ++ i;
      if ( i >= argc ) {
	cerr << " --solution requires filename" << endl;
	return 1;
      }
      solution_filename = argv[i];
    }
    else {
      cerr << argv[i] << ": unknown option" << endl;
      return 1;
    }
  }

  char* filename = argv[base];
  NlProblem problem = read_problem(filename);

  if ( solution_filename != NULL ) {
    NlSolution solution = read_solution(solution_filename, problem);
    NlView* view = new NlView(solution);
    view->show();
  }
  else {
    NlView* view = new NlView(problem);
    view->show();
  }

  return app.exec();
}

END_NAMESPACE_YM_NLINK

int
main(int argc,
     char** argv)
{
  return nsYm::nsNlink::nlview(argc, argv);
}
