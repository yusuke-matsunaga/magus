
/// @file ymsat.cc
/// @brief 簡単な SAT プログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/DimacsParser.h"
#include "YmLogic/SatDimacsHandler.h"
#include "YmLogic/DimacsVerifier.h"
#include "YmLogic/SatSolver.h"
#include "YmLogic/SatMsgHandlerImpl1.h"
#include "YmLogic/SatStats.h"

#include "YmUtils/FileIDO.h"
#include "YmUtils/StopWatch.h"


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;

  int verbose = 0;
  bool minisat = false;
  bool minisat2 = false;

  int wpos = 1;
  string opt;
  bool z_flag = false;
  for (int rpos = 1; rpos < argc; ++ rpos) {
    if ( argv[rpos][0] == '-' ) {
      if ( strcmp(argv[rpos], "-v") == 0 ||
	   strcmp(argv[rpos], "--verbose") == 0 ) {
	++ verbose;
      }
      else if ( strcmp(argv[rpos], "--minisat") == 0 ) {
	minisat = true;
      }
      else if ( strcmp(argv[rpos], "--minisat2") == 0 ) {
	minisat2 = true;
      }
      else if ( strcmp(argv[rpos], "--analyzer-uip1") == 0 ) {
	opt = "uip1";
      }
      else if ( strcmp(argv[rpos], "--analyzer-simple") == 0 ) {
	opt = "simple";
      }
      else if ( strcmp(argv[rpos], "-Z") == 0 ) {
	z_flag = true;
      }
      else {
	cerr << argv[rpos] << " : illegal option" << endl;
	return 1;
      }
    }
    else {
      if ( rpos != wpos ) {
	argv[wpos] = argv[rpos];
      }
      ++ wpos;
    }
  }
  argc = wpos;

  if ( argc != 2 ) {
    cerr << "USAGE : " << argv[0] << " cnf-file" << endl;
    return 2;
  }

  try {
    string sat_type;
    if ( minisat ) {
      sat_type = "minisat";
      if ( verbose ) {
	opt = "verbose";
      }
    }
    else if ( minisat2 ) {
      sat_type = "minisat2";
      if ( verbose ) {
	opt = "verbose";
      }
    }
    SatSolver solver(sat_type, opt);

    DimacsParser parser;
    SatDimacsHandler handler(solver);
    DimacsVerifier verifier;
    parser.add_handler(&handler);
    parser.add_handler(&verifier);

    tCodecType codec_type = kCodecThrough;
    if ( z_flag ) {
      codec_type = kCodecZ;
    }
    FileIDO ido(codec_type);

    if ( !ido.open(argv[1]) ) {
      cerr << "Could not open " << argv[1] << endl;
      return -1;
    }

    if ( !parser.read(ido) ) {
      cerr << "Error in reading " << argv[1] << endl;
      return 4;
    }

    if ( verbose ) {
      SatMsgHandlerImpl1 satmsghandler(cout);
      solver.reg_msg_handler(&satmsghandler);
      solver.timer_on(true);
    }

    StopWatch sw;
    sw.start();
    vector<Bool3> model;
    Bool3 ans = solver.solve(model);
    sw.stop();

    if ( ans == kB3True ) {
      verifier.verify(model);
    }

    switch ( ans ) {
    case kB3True:  cout << "SAT" << endl; break;
    case kB3False: cout << "UNSAT" << endl; break;
    case kB3X:     cout << "UNKNOWN" << endl; break;
    }

    USTime ust = sw.time();
    cout << ust << endl;

    SatStats stats;
    solver.get_stats(stats);
    cout << "restarts          : " << stats.mRestart << endl
	 << "conflicts         : " << stats.mConflictNum << endl
	 << "decisions         : " << stats.mDecisionNum << endl
	 << "propagations      : " << stats.mPropagationNum << endl
	 << "conflict literals : " << stats.mLearntLitNum << endl
	 << "CPU time          : " << stats.mTime << endl;

  }
  catch ( AssertError x) {
    cout << x << endl;
  }

  return 0;
}
