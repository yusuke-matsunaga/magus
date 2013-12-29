
/// @file libym_sat/tests/ymsat.cc
/// @brief 簡単な SAT プログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/DimacsParser.h"
#include "ym_logic/SatDimacsHandler.h"
#include "ym_logic/DimacsVerifier.h"
#include "ym_logic/SatSolver.h"
#include "ym_logic/SatMsgHandlerImpl1.h"
#include "ym_logic/SatStats.h"

#include "ym_utils/FileIDO.h"
#include "ym_utils/StopWatch.h"


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;

  int verbose = 0;
  bool minisat = false;

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

    if ( ido.open(argv[1]) ) {
      cerr << "Could not open " << argv[1] << endl;
    }

    if ( !parser.read(ido) ) {
      cerr << "Error in reading " << argv[1] << endl;
      return 4;
    }

    SatMsgHandlerImpl1 satmsghandler(cout);
    solver.reg_msg_handler(&satmsghandler);
    solver.timer_on(true);

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
  }
  catch ( AssertError x) {
    cout << x << endl;
  }

  return 0;
}
