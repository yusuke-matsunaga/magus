
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


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// c という文字がでるまで読み飛ばす．
void
skip_until(istream& s,
	   int c)
{
  while ( s ) {
    int c1 = s.get();
    if ( c1 == c ) {
      break;
    }
  }
}

// リテラルのリストを読み込む．
void
read_lits(istream& s,
	  vector<Literal>& lits)
{
  while ( s ) {
    int c = s.get();
    if ( c == ' ' || c == '\t' ) continue;
    if ( c == '\n' ) {
      break;
    }
    if ( c < '0' || c > '9' ) {
      cerr << "illegal input" << endl;
      exit(255);
    }
    int id = c - '0';
    while ( s ) {
      int c = s.get();
      if ( c == 'N' ) {
	VarId var(id);
	lits.push_back(Literal(var, true));
	break;
      }
      else if ( c == 'P' ) {
	VarId var(id);
	lits.push_back(Literal(var, false));
	break;
      }
      else if ( c >= '0' && c <= '9' ) {
	id = id * 10 + (c - '0');
      }
      else {
	cerr << "illegal input" << endl;
	exit(255);
      }
    }
  }
}

// @brief リテラルを出力する．
void
put_lit(Literal lit)
{
  cout << " " << lit.varid();
  if ( lit.is_positive() ) {
    cout << "P";
  }
  else {
    cout << "N";
  }
}

END_NONAMESPACE

int
ymsat(int argc,
      char** argv)
{
  int verbose = 0;
  bool minisat = false;
  bool minisat2 = false;
  bool satlog = false;

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
      else if ( strcmp(argv[rpos], "--satlog") == 0 ) {
	satlog = true;
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

    if ( verbose ) {
      SatMsgHandlerImpl1 satmsghandler(cout);
      solver.reg_msg_handler(&satmsghandler);
      solver.timer_on(true);
    }

    StopWatch sw;
    sw.start();

    if ( satlog ) {
      // satlog モード

      ifstream s(argv[1]);
      if ( !s.is_open() ) {
	cerr << "Could not open " << argv[1] << endl;
	return 3;
      }

      while ( s ) {
	int c = s.get();
	if ( c == '\n' ) continue;
	if ( c == '#' ) {
	  skip_until(s, '\n');
	  continue;
	}
	if ( c == 'N' ) {
	  skip_until(s, '\n');
	  VarId id = solver.new_var();
	  if ( verbose > 1 ) {
	    cout << "N" << endl
		 << "# varid = " << id << endl;
	  }
	}
	else if ( c == 'A' ) {
	  vector<Literal> lits;
	  read_lits(s, lits);
	  if ( verbose > 1 ) {
	    cout << "A";
	    for (vector<Literal>::iterator p = lits.begin();
		 p != lits.end(); ++ p) {
	      Literal l = *p;
	      put_lit(l);
	    }
	    cout << endl;
	  }
	  solver.add_clause(lits);
	}
	else if ( c == 'S' ) {
	  vector<Literal> lits;
	  read_lits(s, lits);
	  if ( verbose > 1 ) {
	    cout << "S";
	    for (vector<Literal>::iterator p = lits.begin();
		 p != lits.end(); ++ p) {
	      Literal l = *p;
	      put_lit(l);
	    }
	    cout << endl;
	  }
	  vector<Bool3> model;
	  solver.solve(lits, model);
	}
	else if ( c == 'R' ) {
	  if ( verbose > 1 ) {
	    cout << "R" << endl;
	  }
	  solver.reduce_learnt_clause();
	}
	else if ( c == 'F' ) {
	  if ( verbose > 1 ) {
	    cout << "F" << endl;
	  }
	  solver.forget_learnt_clause();
	}
      }
    }
    else {
      // DIMACS モード
      DimacsParser parser;
      SatDimacsHandler handler(solver);
      DimacsVerifier verifier;
      parser.add_handler(&handler);
      parser.add_handler(&verifier);

      CodecType codec_type = kCodecThrough;
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

END_NAMESPACE_YM

int
main(int argc,
     char** argv)
{
  return nsYm::ymsat(argc, argv);
}
